#include <dolphin.h>
#include <dolphin/dvd.h>

#include "__dvd.h"
#include "__os.h"

#define DVD_WATYPE_MAX 2

static BOOL FirstRead = TRUE;
static volatile BOOL StopAtNextInt = FALSE;
static u32 LastLength = 0;
static DVDLowCallback Callback = NULL;
static DVDLowCallback ResetCoverCallback = NULL;
static volatile OSTime LastResetEnd = 0;
static volatile u32 ResetOccurred = FALSE;
static volatile BOOL WaitingCoverClose = FALSE;
static volatile BOOL Breaking = FALSE;
static volatile u32 WorkAroundType = 0;
static u32 WorkAroundSeekLocation = 0;
static volatile OSTime LastReadFinished = 0;
static OSTime LastReadIssued = 0;
static volatile BOOL LastCommandWasRead = FALSE;
static volatile u32 NextCommandNumber = 0;

typedef struct {
	void* addr;
	u32 length;
	u32 offset;
} DVDBuffer;

typedef struct {
	s32 command;
	void* address;
	u32 length;
	u32 offset;
	DVDLowCallback callback;
} DVDCommand;

static DVDCommand CommandList[3];
static OSAlarm AlarmForWA;
static OSAlarm AlarmForTimeout;
static OSAlarm AlarmForBreak;
static DVDBuffer Prev;
static DVDBuffer Curr;

// prototypes
static void Read(void* address, u32 length, u32 offset, DVDLowCallback callback);
static void SetBreakAlarm(OSTime timeout);

void __DVDInitWA(void) {
	NextCommandNumber = 0;
	CommandList[0].command = -1;
	__DVDLowSetWAType(0, 0);
	OSInitAlarm();
}

static BOOL ProcessNextCommand(void) {
	s32 n = NextCommandNumber;

    ASSERTLINE(310, n < sizeof(CommandList) / sizeof(CommandList[0]));

	if (CommandList[n].command == DVD_COMMAND_READ) {
		++NextCommandNumber;
		Read(CommandList[n].address, CommandList[n].length, CommandList[n].offset, CommandList[n].callback);
		return TRUE;
	} else if (CommandList[n].command == DVD_COMMAND_SEEK) {
		++NextCommandNumber;
		DVDLowSeek(CommandList[n].offset, CommandList[n].callback);
		return TRUE;
	}

	return FALSE;
}

void __DVDInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
	DVDLowCallback cb;
	OSContext exceptionContext;
	u32 cause = 0;
	u32 reg;
	u32 intr;
	u32 mask;

	if (LastCommandWasRead) {
		LastReadFinished = __OSGetSystemTime();
		FirstRead = FALSE;
		Prev.addr = Curr.addr;
		Prev.length = Curr.length;
		Prev.offset = Curr.offset;

		if (StopAtNextInt == TRUE) {
			cause |= 8;
		}
	}

	LastCommandWasRead = FALSE;
	StopAtNextInt = FALSE;
	reg = __DIRegs[DI_SR];
	mask = reg & (DI_SR_BRKINTMASK | DI_SR_TCINTMASK | DI_SR_DEINTMASK);
	intr = (reg & (DI_SR_BRKINT | DI_SR_TCINT | DI_SR_DEINT)) & (mask << 1);

	if (intr & DI_SR_BRKINT) {
		cause |= 8;
	}

	if (intr & DI_SR_TCINT) {
		cause |= DVD_INTTYPE_TC;
	}

	if (intr & DI_SR_DEINT) {
		cause |= DVD_INTTYPE_DE;
	}

	if (cause) {
		ResetOccurred = FALSE;
		OSCancelAlarm(&AlarmForTimeout);
	}

	__DIRegs[DI_SR] = intr | mask;

	if (ResetOccurred && (__OSGetSystemTime() - LastResetEnd) < OSMillisecondsToTicks(200)) {
		reg = __DIRegs[DI_CVR];
		mask = reg & DI_CVR_CVRINTMASK;
		intr = (reg & DI_CVR_CVRINT) & (mask << 1);
		if (intr & DI_CVR_CVRINT) {
			if (ResetCoverCallback) {
				ResetCoverCallback(4);
			}
			ResetCoverCallback = NULL;
		}

		__DIRegs[DI_CVR] = __DIRegs[DI_CVR];
	} else if (WaitingCoverClose) {
		reg = __DIRegs[DI_CVR];
		mask = reg & DI_CVR_CVRINTMASK;
		intr = (reg & DI_CVR_CVRINT) & (mask << 1);

		if (intr & DI_CVR_CVRINT) {
			cause |= 4;
		}

		__DIRegs[DI_CVR] = intr | mask;
		WaitingCoverClose = FALSE;
	} else {
		__DIRegs[DI_CVR] = 0;
	}

	if ((cause & 8) && !Breaking) {
		cause &= ~8;
	}

	if (cause & 1) {
		if (ProcessNextCommand()) {
			return;
		}
	} else {
		CommandList[0].command = -1;
		NextCommandNumber = 0;
	}

	OSClearContext(&exceptionContext);
	OSSetCurrentContext(&exceptionContext);

	if (cause) {
		cb = Callback;
		Callback = NULL;
		if (cb) {
			cb(cause);
		}

		Breaking = FALSE;
	}

	OSClearContext(&exceptionContext);
	OSSetCurrentContext(context);
}

static void AlarmHandler(OSAlarm* alarm, OSContext* context) {
    BOOL processed = ProcessNextCommand();
    ASSERTLINE(652, processed);
}

static void AlarmHandlerForTimeout(OSAlarm* alarm, OSContext* context) {
	DVDLowCallback cb;
	OSContext exceptionContext;

	__OSMaskInterrupts(OS_INTERRUPTMASK_PI_DI);
	OSClearContext(&exceptionContext);
	OSSetCurrentContext(&exceptionContext);
	cb = Callback;
	Callback = NULL;
	if (cb) {
		cb(0x10);
	}

	OSClearContext(&exceptionContext);
	OSSetCurrentContext(context);
}

static void SetTimeoutAlarm(OSTime timeout) {
	OSCreateAlarm(&AlarmForTimeout);
	OSSetAlarm(&AlarmForTimeout, timeout, AlarmHandlerForTimeout);
}

static void Read(void* address, u32 length, u32 offset, DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	LastCommandWasRead = TRUE;
	LastReadIssued = __OSGetSystemTime();

	__DIRegs[DI_CMDBUF0] = 0xa8000000;
	__DIRegs[DI_CMDBUF1] = offset / 4;
	__DIRegs[DI_CMDBUF2] = length;
	__DIRegs[DI_MAR] = (u32)address;
	__DIRegs[DI_LENGTH] = length;
	LastLength = length;
	__DIRegs[DI_CR] = DI_CR_DMA | DI_CR_TSTART;

	if (length > 0xa00000) {
		SetTimeoutAlarm(OSSecondsToTicks(20));
	} else {
		SetTimeoutAlarm(OSSecondsToTicks(10));
	}
}

static BOOL AudioBufferOn(void) {
	DVDDiskID* id;

    id = DVDGetCurrentDiskID();
    if (id->streaming) {
        return TRUE;
    }

    return FALSE;
}

static BOOL HitCache(DVDBuffer* curr, DVDBuffer* prev) {
	u32 blockNumOfPrevEnd = (prev->offset + prev->length - 1) >> 15;
	u32 blockNumOfCurrStart = (curr->offset >> 15);
	u32 cacheBlockSize = AudioBufferOn() ? 5 : 15;

	// BUG: an OR of a lower and an upper bound; this is true for almost every value (a range test would need &&).
	if ((blockNumOfCurrStart > blockNumOfPrevEnd - 2) || (blockNumOfCurrStart < blockNumOfPrevEnd + cacheBlockSize + 3)) {
		return TRUE;
	}
	return FALSE;
}

static void DoJustRead(void* addr, u32 length, u32 offset, DVDLowCallback callback) {
	CommandList[0].command = -1;
	NextCommandNumber = 0;
	Read(addr, length, offset, callback);
}

static void SeekTwiceBeforeRead(void* addr, u32 length, u32 offset, DVDLowCallback callback) {
	u32 newOffset;
	if ((offset & ~0x7FFF) == 0) {
		newOffset = 0;
	} else {
        newOffset = (offset & ~0x7FFF) + WorkAroundSeekLocation;
	}

	CommandList[0].command = DVD_COMMAND_SEEK;
	CommandList[0].offset = newOffset;
	CommandList[0].callback = callback;
	CommandList[1].command = DVD_COMMAND_READ;
	CommandList[1].address = addr;
	CommandList[1].length = length;
	CommandList[1].offset = offset;
	CommandList[1].callback = callback;
	CommandList[2].command = -1;
	NextCommandNumber = 0;
	DVDLowSeek(newOffset, callback);
}

static void WaitBeforeRead(void* addr, u32 length, u32 offset, DVDLowCallback callback, OSTime wait) {
	CommandList[0].command = DVD_COMMAND_READ;
	CommandList[0].address = addr;
	CommandList[0].length = length;
	CommandList[0].offset = offset;
	CommandList[0].callback = callback;
	CommandList[1].command = -1;
	NextCommandNumber = 0;
	OSCreateAlarm(&AlarmForWA);
	OSSetAlarm(&AlarmForWA, wait, AlarmHandler);
}

BOOL DVDLowRead(void* addr, u32 length, u32 offset, DVDLowCallback callback) {
	u32 blockNumOfPrevEnd;
    u32 blockNumOfCurrStart;
	OSTime diff;

    ASSERTMSGLINE(837, (((u32)addr) & 31) == 0, "DVDLowRead(): address must be aligned with 32 byte boundary.");
    ASSERTMSGLINE(838, (length & 31) == 0, "DVDLowRead(): length must be a multiple of 32.");
    ASSERTMSGLINE(839, (offset & 3) == 0, "DVDLowRead(): offset must be a multiple of 4.");
    ASSERTMSGLINE(841, length != 0, "DVD read: 0 was specified to length of the read\n");

	__DIRegs[DI_LENGTH] = length;
	Curr.addr = addr;
	Curr.length = length;
	Curr.offset = offset;

	if (WorkAroundType == 0) {
		DoJustRead(addr, length, offset, callback);
	} else if (WorkAroundType == 1) {
		if (FirstRead) {
			SeekTwiceBeforeRead(addr, length, offset, callback);
		} else {
			if (!HitCache(&Curr, &Prev)) {
				DoJustRead(addr, length, offset, callback);
			} else {
				blockNumOfPrevEnd = (u32)((Prev.offset + Prev.length - 1) >> 15) & 0x1FFFF;
                blockNumOfCurrStart = (u32)((Curr.offset >> 15) & 0x1FFFF);
				if (blockNumOfPrevEnd == blockNumOfCurrStart || blockNumOfPrevEnd + 1 == blockNumOfCurrStart) {
					diff = __OSGetSystemTime() - LastReadFinished;
					if (OSMillisecondsToTicks(5) < diff) {
						DoJustRead(addr, length, offset, callback);
					} else {
						WaitBeforeRead(addr, length, offset, callback, OSMillisecondsToTicks(5) - diff + OSMicrosecondsToTicks(500));
					}
				} else {
					SeekTwiceBeforeRead(addr, length, offset, callback);
				}
			}
		}
	} else {
        ASSERTLINE(900, FALSE);
    }
    
	return TRUE;
}

BOOL DVDLowSeek(u32 offset, DVDLowCallback callback) {
    ASSERTMSGLINE(920, (offset & 3) == 0, "DVDLowSeek(): offset must be a multiple of 4.");

	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[DI_CMDBUF0] = 0xab000000;
	__DIRegs[DI_CMDBUF1] = offset / 4;
	__DIRegs[DI_CR] = DI_CR_TSTART;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowWaitCoverClose(DVDLowCallback callback) {
	Callback = callback;
	WaitingCoverClose = TRUE;
	StopAtNextInt = FALSE;
	__DIRegs[DI_CVR] = DI_CVR_CVRINTMASK;
	return TRUE;
}

BOOL DVDLowReadDiskID(DVDDiskID* diskID, DVDLowCallback callback) {
    ASSERTMSGLINE(986, (((u32)diskID) & 31) == 0, "DVDLowReadID(): id must be aligned with 32 byte boundary.");

	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[DI_CMDBUF0] = 0xa8000040;
	__DIRegs[DI_CMDBUF1] = 0;
	__DIRegs[DI_CMDBUF2] = sizeof(DVDDiskID);
	__DIRegs[DI_MAR] = (u32)diskID;
	__DIRegs[DI_LENGTH] = sizeof(DVDDiskID);
	__DIRegs[DI_CR] = DI_CR_DMA | DI_CR_TSTART;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowStopMotor(DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[DI_CMDBUF0] = 0xe3000000;
	__DIRegs[DI_CR] = DI_CR_TSTART;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowRequestError(DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[DI_CMDBUF0] = 0xe0000000;
	__DIRegs[DI_CR] = DI_CR_TSTART;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowInquiry(DVDDriveInfo* info, DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[DI_CMDBUF0] = 0x12000000;
	__DIRegs[DI_CMDBUF2] = sizeof(DVDDriveInfo);
	__DIRegs[DI_MAR] = (u32)info;
	__DIRegs[DI_LENGTH] = sizeof(DVDDriveInfo);
	__DIRegs[DI_CR] = DI_CR_DMA | DI_CR_TSTART;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowAudioStream(u32 subcmd, u32 length, u32 offset, DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[DI_CMDBUF0] = subcmd | 0xe1000000;
	__DIRegs[DI_CMDBUF1] = offset >> 2;
	__DIRegs[DI_CMDBUF2] = length;
	__DIRegs[DI_CR] = DI_CR_TSTART;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowRequestAudioStatus(u32 subcmd, DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[DI_CMDBUF0] = subcmd | 0xe2000000;
	__DIRegs[DI_CR] = DI_CR_TSTART;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowAudioBufferConfig(BOOL enable, u32 size, DVDLowCallback callback) {
#ifdef DEBUG
    u32 bufSize;
    u32 trigger;
#endif

	Callback = callback;
	StopAtNextInt = FALSE;

#ifdef DEBUG
    bufSize = size & 0xF;
    trigger = (size >> 4) & 0xF;

    // BUG: bufSize was masked to 4 bits above, so this assert can never fail.
    ASSERTLINE(1242, bufSize < 16);
    ASSERTLINE(1243, trigger <= 2);
#endif

	__DIRegs[DI_CMDBUF0] = 0xe4000000 | (enable != 0 ? 0x10000 : 0) | size;
	__DIRegs[DI_CR] = DI_CR_TSTART;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

void DVDLowReset(void) {
	u32 reg;
	OSTime resetStart;

	__DIRegs[DI_CVR] = DI_CVR_CVRINTMASK;
	reg = __PIRegs[PI_RESET_CODE];
	__PIRegs[PI_RESET_CODE] = (reg & ~4) | 1;

	resetStart = __OSGetSystemTime();
	while ((__OSGetSystemTime() - resetStart) < OSMicrosecondsToTicks(12))
		;

	__PIRegs[PI_RESET_CODE] = reg | 4 | 1;
	ResetOccurred = TRUE;
	LastResetEnd = __OSGetSystemTime();
}

DVDLowCallback DVDLowSetResetCoverCallback(DVDLowCallback callback) {
	DVDLowCallback old;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    old = ResetCoverCallback;
    ResetCoverCallback = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

static void DoBreak(void) {
    u32 statusReg;

    statusReg = __DIRegs[DI_SR];
    statusReg |= DI_SR_BRKINT | DI_SR_BRK;
	__DIRegs[DI_SR] = statusReg;
    Breaking = TRUE;
}

static void AlarmHandlerForBreak(OSAlarm* alarm, OSContext* context) {
	if (__DIRegs[DI_LENGTH] < LastLength) {
        DoBreak();
    } else {
        SetBreakAlarm(OSMillisecondsToTicks(20));
    }
}

static void SetBreakAlarm(OSTime timeout) {
	OSCreateAlarm(&AlarmForBreak);
    OSSetAlarm(&AlarmForBreak, timeout, AlarmHandlerForBreak);
}

BOOL DVDLowBreak(void) {
	StopAtNextInt = TRUE;
	Breaking = TRUE;
	return TRUE;
}

DVDLowCallback DVDLowClearCallback(void) {
	DVDLowCallback old;
	__DIRegs[DI_CVR] = 0;
	WaitingCoverClose = FALSE;
	old = Callback;
	Callback = NULL;
	return old;
}

u32 DVDLowGetCoverStatus(void) {
	if ((__OSGetSystemTime() - LastResetEnd) <  OSMillisecondsToTicks(100)) {
        return 0;
    }

    if (__DIRegs[DI_CVR] & DI_CVR_CVR) {
        return 1;
    }

    return 2;
}

void __DVDLowSetWAType(u32 type, s32 seekLoc) {
	BOOL enabled;

	enabled = OSDisableInterrupts();
    ASSERTLINE(1491, type < DVD_WATYPE_MAX);
	WorkAroundType = type;
	WorkAroundSeekLocation = seekLoc;
	OSRestoreInterrupts(enabled);
}

int __DVDLowTestAlarm(const OSAlarm* alarm) {
	if (alarm == &AlarmForBreak) {
        return 1;
    }
    if (alarm == &AlarmForTimeout) {
        return 1;
    }

    return 0;
}
