#pragma once

#include <dolphin/types.h>

typedef struct PiantaParlorWork {
	u32 flags; //0x00, 1: token counter shown, 2: tokens taken out (playing), 4: counter raised, 8: window closed
	u32 keptTokens; //0x04, tokens deposited by yuugijou_montemedal_keep
	s32 tokens; //0x08
	u32 dispTick; //0x0C, counter-animation tick
	u32 dispTokens; //0x10, token count currently shown (counts toward tokens)
	// four minigames x five ranks (+1 spare slot); defaults set in yuugijou_init
#ifdef VERSION_G8ME01
	// US keeps eight record rows (0x258 bytes in all): recordScore 0xD4, recordParam 0x194, pad_134 at 0x254
	s32 recordOn[8][6]; //0x14
	s32 recordScore[8][6]; //0xD4
	s32 recordParam[8][6]; //0x194
#else
	s32 recordOn[4][6]; //0x14, all set to 1 at init
	s32 recordScore[4][6]; //0x74, default record values (game 1 ascending: times)
	s32 recordParam[4][6]; //0xD4, per-rank value derived from recordScore (game 0: constants)
#endif
	u32 pad_134; //0x134, never accessed
} PiantaParlorWork;

// Record arrays are [minigame][rank]; ranks 0-4 hold the defaults set in yuugijou_init, rank 5 is unused there.
// Element offsets (for patching / save editing):
//   0x014  recordOn[0][0]
//   0x018  recordOn[0][1]
//   0x01C  recordOn[0][2]
//   0x020  recordOn[0][3]
//   0x024  recordOn[0][4]
//   0x028  recordOn[0][5]
//   0x02C  recordOn[1][0]
//   0x030  recordOn[1][1]
//   0x034  recordOn[1][2]
//   0x038  recordOn[1][3]
//   0x03C  recordOn[1][4]
//   0x040  recordOn[1][5]
//   0x044  recordOn[2][0]
//   0x048  recordOn[2][1]
//   0x04C  recordOn[2][2]
//   0x050  recordOn[2][3]
//   0x054  recordOn[2][4]
//   0x058  recordOn[2][5]
//   0x05C  recordOn[3][0]
//   0x060  recordOn[3][1]
//   0x064  recordOn[3][2]
//   0x068  recordOn[3][3]
//   0x06C  recordOn[3][4]
//   0x070  recordOn[3][5]
//   0x074  recordScore[0][0]
//   0x078  recordScore[0][1]
//   0x07C  recordScore[0][2]
//   0x080  recordScore[0][3]
//   0x084  recordScore[0][4]
//   0x088  recordScore[0][5]
//   0x08C  recordScore[1][0]
//   0x090  recordScore[1][1]
//   0x094  recordScore[1][2]
//   0x098  recordScore[1][3]
//   0x09C  recordScore[1][4]
//   0x0A0  recordScore[1][5]
//   0x0A4  recordScore[2][0]
//   0x0A8  recordScore[2][1]
//   0x0AC  recordScore[2][2]
//   0x0B0  recordScore[2][3]
//   0x0B4  recordScore[2][4]
//   0x0B8  recordScore[2][5]
//   0x0BC  recordScore[3][0]
//   0x0C0  recordScore[3][1]
//   0x0C4  recordScore[3][2]
//   0x0C8  recordScore[3][3]
//   0x0CC  recordScore[3][4]
//   0x0D0  recordScore[3][5]
//   0x0D4  recordParam[0][0]
//   0x0D8  recordParam[0][1]
//   0x0DC  recordParam[0][2]
//   0x0E0  recordParam[0][3]
//   0x0E4  recordParam[0][4]
//   0x0E8  recordParam[0][5]
//   0x0EC  recordParam[1][0]
//   0x0F0  recordParam[1][1]
//   0x0F4  recordParam[1][2]
//   0x0F8  recordParam[1][3]
//   0x0FC  recordParam[1][4]
//   0x100  recordParam[1][5]
//   0x104  recordParam[2][0]
//   0x108  recordParam[2][1]
//   0x10C  recordParam[2][2]
//   0x110  recordParam[2][3]
//   0x114  recordParam[2][4]
//   0x118  recordParam[2][5]
//   0x11C  recordParam[3][0]
//   0x120  recordParam[3][1]
//   0x124  recordParam[3][2]
//   0x128  recordParam[3][3]
//   0x12C  recordParam[3][4]
//   0x130  recordParam[3][5]

void yuugijou_init(void);

extern PiantaParlorWork* yuwp;

void evtYuuWindow(void);
s32 evtYuuWindowEndChk(void);
