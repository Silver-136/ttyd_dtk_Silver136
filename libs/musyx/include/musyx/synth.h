#ifndef _MUSYX_SYNTH
#define _MUSYX_SYNTH
#include "musyx/musyx.h"

#include "musyx/adsr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  MAC_STATE_RUNNABLE = 0,
  MAC_STATE_YIELDED,
  MAC_STATE_STOPPED,
} MAC_STATE;

typedef struct MSTEP {
  u32 para[2];
} MSTEP;

typedef struct CALLSTACK {
  MSTEP* addr;
  MSTEP* curAddr;
} CALLSTACK;

typedef struct SYNTH_VOICELIST {
  // total size: 0x4
  u8 prev;  // offset 0x0, size 0x1
  u8 next;  // offset 0x1, size 0x1
  u16 user; // offset 0x2, size 0x2
} SYNTH_VOICELIST;
extern SYNTH_VOICELIST voicePrioSortVoices[64];
extern u8 voicePrioSortVoicesRoot[256];
extern SYNTH_VOICELIST voicePrioSortVoices[64];

typedef struct SYNTH_ROOTLIST {
  // total size: 0x4
  u16 next; // offset 0x0, size 0x2
  u16 prev; // offset 0x2, size 0x2
} SYNTH_ROOTLIST;

extern SYNTH_ROOTLIST voicePrioSortRootList[256];

typedef struct synthInfo {
  u32 mixFrq;
  u32 numSamples;
  SND_PLAYBACKINFO pbInfo;
  u8 voiceNum;
  u8 maxMusic;
  u8 maxSFX;
  u8 studioNum;
} SynthInfo;
extern SynthInfo synthInfo;

typedef struct VID_LIST {
  struct VID_LIST* next;
  struct VID_LIST* prev;
  u32 vid;
  u32 root;
} VID_LIST;

typedef struct SYNTH_QUEUE {
  // total size: 0xC
  struct SYNTH_QUEUE* next; // offset 0x0, size 0x4
  struct SYNTH_QUEUE* prev; // offset 0x4, size 0x4
  u8 voice;                 // offset 0x8, size 0x1
  u8 jobTabIndex;           // offset 0x9, size 0x1
} SYNTH_QUEUE;

typedef enum {
  SYNTH_JOBTYPE_LOW = 0,
  SYNTH_JOBTYPE_ZERO = 1,
  SYNTH_JOBTYPE_EVENT = 2,
} SYNTH_JOBTYPE;

typedef struct {
  // total size: 0xC
  SYNTH_QUEUE* lowPrecision; // offset 0x0, size 0x4
  SYNTH_QUEUE* event;        // offset 0x4, size 0x4
  SYNTH_QUEUE* zeroOffset;   // offset 0x8, size 0x4
} SYNTH_JOBTAB;

typedef struct SYNTH_LFO {
  u32 time;
  u32 period;
  s16 value;
  s16 lastValue;
} SYNTH_LFO;

typedef struct SYNTHMasterFader {
  // total size: 0x30
  f32 volume;         // offset 0x0, size 0x4
  f32 target;         // offset 0x4, size 0x4
  f32 start;          // offset 0x8, size 0x4
  f32 time;           // offset 0xC, size 0x4
  f32 deltaTime;      // offset 0x10, size 0x4
  f32 pauseVol;       // offset 0x14, size 0x4
  f32 pauseTarget;    // offset 0x18, size 0x4
  f32 pauseStart;     // offset 0x1C, size 0x4
  f32 pauseTime;      // offset 0x20, size 0x4
  f32 pauseDeltaTime; // offset 0x24, size 0x4
  u32 seqId;          // offset 0x28, size 0x4
  u8 seqMode;         // offset 0x2C, size 0x1
  u8 type;            // offset 0x2D, size 0x1
} SYNTHMasterFader;

// SYNTHMasterFader.type: which sndVolume group selector reaches the fader (synthVolume).
typedef enum {
  SYNTH_FADER_MUSIC = 0,      // SND_MUSIC_VOLGROUPS; sequence groups (seqVolDef)
  SYNTH_FADER_FX = 1,         // SND_FX_VOLGROUPS
  SYNTH_FADER_USER_MUSIC = 2, // SND_USERMUSIC_VOLGROUPS
  SYNTH_FADER_USER_FX = 3,    // SND_USERFX_VOLGROUPS; FX groups named by sndFXStart callers
  SYNTH_FADER_NONE = 4,       // slot not in use (synthInit default)
} SYNTH_FADER_TYPE;

// synthMasterFader[] slots with a fixed use.
#define SYNTH_VGROUP_MUSIC_MASTER 21 // sndMasterVolume music; scales every non-FX voice
#define SYNTH_VGROUP_FX_MASTER 22    // sndMasterVolume FX; scales every FX voice
#define SYNTH_VGROUP_SEQ_BASE 23     // 8 sequence groups 23..30 (defVGroup = seqId + 23)
#define SYNTH_VGROUP_FX 31           // default group of an FX entry

typedef struct CTRL_SOURCE {
  u8 midiCtrl;
  u8 combine;
  s32 scale;
} CTRL_SOURCE;

typedef struct CTRL_DEST {
  CTRL_SOURCE source[4];
  u16 oldValue;
  u8 numSource;
} CTRL_DEST;

#pragma push
#pragma pack(4)
typedef struct SYNTH_VOICE {
  // total size: 0x404
  SYNTH_QUEUE lowPrecisionJob;            // offset 0x0, size 0xC
  SYNTH_QUEUE zeroOffsetJob;              // offset 0xC, size 0xC
  SYNTH_QUEUE eventJob;                   // offset 0x18, size 0xC
  u64 lastLowCallTime;                    // offset 0x24, size 0x8
  u64 lastZeroCallTime;                   // offset 0x2C, size 0x8
  MSTEP* addr;                            // offset 0x34, size 0x4
  MSTEP* curAddr;                         // offset 0x38, size 0x4
  struct SYNTH_VOICE* nextMacActive;      // offset 0x3C, size 0x4
  struct SYNTH_VOICE* prevMacActive;      // offset 0x40, size 0x4
  struct SYNTH_VOICE* nextTimeQueueMacro; // offset 0x44, size 0x4
  struct SYNTH_VOICE* prevTimeQueueMacro; // offset 0x48, size 0x4
  MAC_STATE macState;                     // offset 0x4C, size 0x4
  MSTEP* trapEventAddr[3];                // offset 0x50, size 0xC
  MSTEP* trapEventCurAddr[3];             // offset 0x5C, size 0xC
  u8 trapEventAny;                        // offset 0x68, size 0x1
  CALLSTACK callStack[4];                 // offset 0x6C, size 0x20
  u8 callStackEntryNum;                   // offset 0x8C, size 0x1
  u8 callStackIndex;                      // offset 0x8D, size 0x1
  u64 macStartTime;                       // offset 0x90, size 0x8
  u64 wait;                               // offset 0x98, size 0x8
  u64 waitTime;                           // offset 0xA0, size 0x8
  u8 timeUsedByInput;                     // offset 0xA8, size 0x1
  u16 loop;                               // offset 0xAA, size 0x2
  s32 local_vars[16];                     // offset 0xAC, size 0x40
  u32 child;                              // offset 0xEC, size 0x4
  u32 parent;                             // offset 0xF0, size 0x4
  u32 id;                                 // offset 0xF4, size 0x4
  VID_LIST* vidList;                      // offset 0xF8, size 0x4
  VID_LIST* vidMasterList;                // offset 0xFC, size 0x4
#if MUSY_VERSION >= MUSY_VERSION_CHECK(1, 5, 0) && MUSY_VERSION <= MUSY_VERSION_CHECK(2, 0, 0)
  u16 allocId; // offset 0x100, size 0x2
#else
  u32 allocId;
#endif
  u16 macroId;  // offset 0x102, size 0x2
  u8 keyGroup;  // offset 0x104, size 0x1
  u32 lastVID;  // offset 0x108, size 0x4
  u8 prio;      // offset 0x10C, size 0x1
  u16 ageSpeed; // offset 0x10E, size 0x2
  u32 age;      // offset 0x110, size 0x4
  u64 cFlags;   // offset 0x114, size 0x8
  u8 block;     // offset 0x11C, size 0x1
  u8 fxFlag;    // offset 0x11D, size 0x1
  u8 vGroup;    // offset 0x11E, size 0x1
  u8 studio;    // offset 0x11F, size 0x1
  u8 track;     // offset 0x120, size 0x1
  u8 midi;      // offset 0x121, size 0x1
  u8 midiSet;   // offset 0x122, size 0x1
  u8 section;   // offset 0x123, size 0x1
#if MUSY_VERSION <= MUSY_VERSION_CHECK(1, 5, 0)
  void* sAddr;
#endif
  u32 sInfo;   // offset 0x124, size 0x4
  u32 playFrq; // offset 0x128, size 0x4
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
  u16 sampleId;
#endif
  u16 curNote;             // offset 0x12C, size 0x2
  s8 curDetune;            // offset 0x12E, size 0x1
  u8 orgNote;              // offset 0x12F, size 0x1
  u8 lastNote;             // offset 0x130, size 0x1
  u8 portType;             // offset 0x131, size 0x1
  u16 portLastCtrlState;   // offset 0x132, size 0x2
  u32 portDuration;        // offset 0x134, size 0x4
  u32 portCurPitch;        // offset 0x138, size 0x4
  u32 portTime;            // offset 0x13C, size 0x4
  u8 vibKeyRange;          // offset 0x140, size 0x1
  u8 vibCentRange;         // offset 0x141, size 0x1
  u32 vibPeriod;           // offset 0x144, size 0x4
  u32 vibCurTime;          // offset 0x148, size 0x4
  s32 vibCurOffset;        // offset 0x14C, size 0x4
  s16 vibModAddScale;      // offset 0x150, size 0x2
  u32 volume;              // offset 0x154, size 0x4
  u32 orgVolume;           // offset 0x158, size 0x4
  float lastVolFaderScale; // offset 0x15C, size 0x4
  u32 lastPan;             // offset 0x160, size 0x4
  u32 lastSPan;            // offset 0x164, size 0x4
  float treCurScale;       // offset 0x168, size 0x4
  u16 treScale;            // offset 0x16C, size 0x2
  u16 treModAddScale;      // offset 0x16E, size 0x2
  u32 panning[2];          // offset 0x170, size 0x8
  s32 panDelta[2];         // offset 0x178, size 0x8
  u32 panTarget[2];        // offset 0x180, size 0x8
  u32 panTime[2];          // offset 0x188, size 0x8
  u8 revVolScale;          // offset 0x190, size 0x1
  u8 revVolOffset;         // offset 0x191, size 0x1
  u8 volTable;             // offset 0x192, size 0x1
  u8 itdMode;              // offset 0x193, size 0x1
  s32 envDelta;            // offset 0x194, size 0x4
  u32 envTarget;           // offset 0x198, size 0x4
  u32 envCurrent;          // offset 0x19C, size 0x4
  u32 sweepOff[2];         // offset 0x1A0, size 0x8
  s32 sweepAdd[2];         // offset 0x1A8, size 0x8
  s32 sweepCnt[2];         // offset 0x1B0, size 0x8
  u8 sweepNum[2];          // offset 0x1B8, size 0x2
  SYNTH_LFO lfo[2];        // offset 0x1BC, size 0x18
  u8 lfoUsedByInput[2];    // offset 0x1D4, size 0x2
  u8 pbLowerKeyRange;      // offset 0x1D6, size 0x1
  u8 pbUpperKeyRange;      // offset 0x1D7, size 0x1
  u16 pbLast;              // offset 0x1D8, size 0x2
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
  u32 lpfLowerFrqBoundary;
  u32 lpfUpperFrqBoundary;
#endif
  ADSR_VARS pitchADSR; // offset 0x1DC, size 0x28
  s16 pitchADSRRange;  // offset 0x204, size 0x2
  u16 curPitch;        // offset 0x206, size 0x2
  struct setup {
    // total size: 0x9
    u8 vol;                     // offset 0x0, size 0x1
    u8 pan;                     // offset 0x1, size 0x1
    u8 midi;                    // offset 0x2, size 0x1
    u8 midiSet;                 // offset 0x3, size 0x1
    u8 section;                 // offset 0x4, size 0x1
    u8 track;                   // offset 0x5, size 0x1
    u8 vGroup;                  // offset 0x6, size 0x1
    u8 studio;                  // offset 0x7, size 0x1
    u8 itdMode;                 // offset 0x8, size 0x1
  } setup;                      // offset 0x208, size 0x9
  u32 midiDirtyFlags;           // offset 0x214, size 0x4
  CTRL_DEST inpVolume;          // offset 0x218, size 0x24
  CTRL_DEST inpPanning;         // offset 0x23C, size 0x24
  CTRL_DEST inpSurroundPanning; // offset 0x260, size 0x24
  CTRL_DEST inpPitchBend;       // offset 0x284, size 0x24
  CTRL_DEST inpDoppler;         // offset 0x2A8, size 0x24
  CTRL_DEST inpModulation;      // offset 0x2CC, size 0x24
  CTRL_DEST inpPedal;           // offset 0x2F0, size 0x24
  CTRL_DEST inpPortamento;      // offset 0x314, size 0x24
  CTRL_DEST inpPreAuxA;         // offset 0x338, size 0x24
  CTRL_DEST inpReverb;          // offset 0x35C, size 0x24
  CTRL_DEST inpPreAuxB;         // offset 0x380, size 0x24
  CTRL_DEST inpPostAuxB;        // offset 0x3A4, size 0x24
  CTRL_DEST inpTremolo;         // offset 0x3C8, size 0x24
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
  CTRL_DEST inpFilterSwitch;    // offset 0x3F8, size 0x24
  CTRL_DEST inpFilterParameter; // offset 0x41C, size 0x24
#endif
  u8 mesgNum;          // offset 0x3EC, size 0x1
  u8 mesgRead;         // offset 0x3ED, size 0x1
  u8 mesgWrite;        // offset 0x3EE, size 0x1
  s32 mesgQueue[4];    // offset 0x3F0, size 0x10
  u16 curOutputVolume; // offset 0x400, size 0x2
} SYNTH_VOICE;

// SYNTH_VOICE.cFlags bits (u64), named from what the synth code does with each bit.
#define SYNTH_CFLAG_START_BREAK 0x1   // macStart: hw voice still active; macHandleActive breaks it
#define SYNTH_CFLAG_START_PENDING 0x2 // macStart sets; macHandleActive clears after the setup
#define SYNTH_CFLAG_WAIT_KEY_OFF 0x4  // mcmdWait: a key-off resumes the macro
#define SYNTH_CFLAG_KEY_OFF 0x8       // macSetExternalKeyoff: key-off received
#define SYNTH_CFLAG_KEY_ON 0x10       // EventHandler started the hw voice; hw key-off clears it
#define SYNTH_CFLAG_START_SAMPLE 0x20 // mcmdStartSample sets; EventHandler calls hwStart
#define SYNTH_CFLAG_SELECT_FILTER_SWITCH 0x40 // mcmdFilterSwitchSelect: first source set
#define SYNTH_CFLAG_KEY_OFF_PENDING 0x80      // mcmdKeyOff sets; EventHandler sends hwKeyOff
#define SYNTH_CFLAG_ADSR_SET 0x100            // macro set the ADSR: mcmdStartSample keeps it
#define SYNTH_CFLAG_PORTAMENTO 0x400          // portamento on (macro or portamento controller)
#define SYNTH_CFLAG_SELECT_FILTER_PARAM 0x800 // mcmdFilterParameterSelect: first source set
#define SYNTH_CFLAG_PORTAMENTO_CTRL_SEEN 0x1000 // controller read; synthInitPortamento skips glide
#define SYNTH_CFLAG_VIBRATO 0x2000              // mcmdVibrato with a period
#define SYNTH_CFLAG_VIBRATO_MOD_WHEEL 0x4000    // vibrato depth scaled by the modulation input
#define SYNTH_CFLAG_VOL_ENVELOPE 0x8000         // DoEnvelopeCalculation: volume envelope running
#define SYNTH_CFLAG_PITCH_BEND_AFTER_KEY_OFF 0x10000 // pitch bend still read after the key-off
#define SYNTH_CFLAG_LEGATO 0x20000          // do_voice_portamento: voice reused for a new key
#define SYNTH_CFLAG_WAIT_SAMPLE_END 0x40000 // mcmdWait: the sample end resumes the macro
// mcmd*Select: the first control source replaces the input's sources, later ones combine.
#define SYNTH_CFLAG_SELECT_VOLUME 0x80000
#define SYNTH_CFLAG_SELECT_PANNING 0x100000
#define SYNTH_CFLAG_SELECT_PITCH_BEND 0x200000
#define SYNTH_CFLAG_SELECT_MODULATION 0x400000
#define SYNTH_CFLAG_SELECT_REVERB 0x800000
#define SYNTH_CFLAG_SELECT_PORTAMENTO 0x1000000
#define SYNTH_CFLAG_SELECT_PEDAL 0x2000000
#define SYNTH_CFLAG_SELECT_SURROUND_PANNING 0x4000000
#define SYNTH_CFLAG_SELECT_DOPPLER 0x8000000
#define SYNTH_CFLAG_SELECT_TREMOLO 0x10000000
#define SYNTH_CFLAG_SELECT_PRE_AUXA 0x20000000
#define SYNTH_CFLAG_SELECT_PRE_AUXB 0x40000000
#define SYNTH_CFLAG_SELECT_POST_AUXB 0x80000000
#define SYNTH_CFLAG_SELECT_AUXA_0 0x100000000ULL // mcmdAuxAFXSelect, one bit per effect input
#define SYNTH_CFLAG_SELECT_AUXA_1 0x200000000ULL
#define SYNTH_CFLAG_SELECT_AUXA_2 0x400000000ULL
#define SYNTH_CFLAG_SELECT_AUXA_3 0x800000000ULL
#define SYNTH_CFLAG_SELECT_AUXB_0 0x1000000000ULL // mcmdAuxBFXSelect, one bit per effect input
#define SYNTH_CFLAG_SELECT_AUXB_1 0x2000000000ULL
#define SYNTH_CFLAG_SELECT_AUXB_2 0x4000000000ULL
#define SYNTH_CFLAG_SELECT_AUXB_3 0x8000000000ULL
#define SYNTH_CFLAG_PEDAL_HELD 0x10000000000ULL      // macSetPedalState: sustain pedal down
#define SYNTH_CFLAG_PITCH_ADSR 0x20000000000ULL      // mcmdSetPitchADSR: pitch envelope running
#define SYNTH_CFLAG_KEY_OFF_HELD 0x40000000000ULL    // key-off held back until the pedal is up
#define SYNTH_CFLAG_SRC_SET 0x80000000000ULL         // mcmdSRCModeSelect: StartSample keeps it
#define SYNTH_CFLAG_VOLUME_CHANGED 0x100000000000ULL // volume update for the next zero-offset job
#define SYNTH_CFLAG_PAN_CHANGED 0x200000000000ULL    // panning update for the next zero-offset job

// SYNTH_VOICE.midiDirtyFlags bits: input changed since its inpGet* call last read it.
#define SYNTH_DIRTY_VOLUME 0x1
#define SYNTH_DIRTY_PANNING 0x2
#define SYNTH_DIRTY_SURROUND_PANNING 0x4
#define SYNTH_DIRTY_PITCH_BEND 0x8
#define SYNTH_DIRTY_DOPPLER 0x10
#define SYNTH_DIRTY_MODULATION 0x20
#define SYNTH_DIRTY_PEDAL 0x40
#define SYNTH_DIRTY_PORTAMENTO 0x80 // set by mcmdPortamentoSelect; no inpGet* reads it
#define SYNTH_DIRTY_PRE_AUXA 0x100
#define SYNTH_DIRTY_REVERB 0x200
#define SYNTH_DIRTY_PRE_AUXB 0x400
#define SYNTH_DIRTY_POST_AUXB 0x800
#define SYNTH_DIRTY_TREMOLO 0x1000
#define SYNTH_DIRTY_FILTER_SWITCH 0x2000 // MusyX 2.0.1 and later
#define SYNTH_DIRTY_FILTER_PARAM 0x4000  // MusyX 2.0.1 and later
#define SYNTH_DIRTY_ALL 0x7fff           // every input (MusyX 2.0.1 and later)
#define SYNTH_DIRTY_ALL_NO_FILTER 0x1fff // every input before the filter inputs (before 2.0.1)

// Synth macro opcodes (low 7 bits of the first command word); named after the mcmd handler each selects.
enum {
  MACRO_OP_END_OF_MACRO = 0x00, // mcmdEndOfMacro
  MACRO_OP_STOP = 0x01, // mcmdStop
  MACRO_OP_IF_KEY = 0x02, // mcmdIfKey
  MACRO_OP_IF_VELOCITY = 0x03, // mcmdIfVelocity
  MACRO_OP_WAIT = 0x04, // mcmdWait
  MACRO_OP_LOOP = 0x05, // mcmdLoop
  MACRO_OP_GOTO = 0x06, // mcmdGoto
  MACRO_OP_WAIT_MS = 0x07, // mcmdWaitMs
  MACRO_OP_PLAY_MACRO = 0x08, // mcmdPlayMacro
  MACRO_OP_SEND_KEY_OFF = 0x09, // mcmdSendKeyOff
  MACRO_OP_IF_MODULATION = 0x0A, // mcmdIfModulation
  MACRO_OP_SET_PIANO_PANNING = 0x0B, // mcmdSetPianoPanning
  MACRO_OP_SET_ADSR = 0x0C, // mcmdSetADSR
  MACRO_OP_SCALE_VOLUME = 0x0D, // mcmdScaleVolume
  MACRO_OP_SET_PANNING = 0x0E, // mcmdSetPanning
  MACRO_OP_ENVELOPE = 0x0F, // mcmdEnvelope
  MACRO_OP_START_SAMPLE = 0x10, // mcmdStartSample
  MACRO_OP_STOP_SAMPLE = 0x11, // mcmdStopSample
  MACRO_OP_KEY_OFF = 0x12, // mcmdKeyOff
  MACRO_OP_IF_RANDOM = 0x13, // mcmdIfRandom
  MACRO_OP_FADE_IN = 0x14, // mcmdFadeIn
  MACRO_OP_SET_SURROUND_PANNING = 0x15, // mcmdSetSurroundPanning
  MACRO_OP_SET_ADSR_FROM_CTRL = 0x16, // mcmdSetADSRFromCtrl
  MACRO_OP_RANDOM_KEY = 0x17, // mcmdRandomKey
  MACRO_OP_ADD_KEY = 0x18, // mcmdAddKey
  MACRO_OP_SET_KEY = 0x19, // mcmdSetKey
  MACRO_OP_LAST_KEY = 0x1A, // mcmdLastKey
  MACRO_OP_PORTAMENTO = 0x1B, // mcmdPortamento
  MACRO_OP_VIBRATO = 0x1C, // mcmdVibrato
  MACRO_OP_PITCH_SWEEP_1 = 0x1D, // mcmdPitchSweep: pitch sweep slot 0
  MACRO_OP_PITCH_SWEEP_2 = 0x1E, // mcmdPitchSweep: pitch sweep slot 1
  MACRO_OP_SET_PITCH = 0x1F, // mcmdSetPitch
  MACRO_OP_SET_PITCH_ADSR = 0x20, // mcmdSetPitchADSR
  MACRO_OP_SCALE_VOLUME_DLS = 0x21, // mcmdScaleVolumeDLS
  MACRO_OP_SET_MOD2_VIBRATO = 0x22, // mcmdSetMod2Vibrato
  MACRO_OP_SETUP_TREMOLO = 0x23, // mcmdSetupTremolo
  MACRO_OP_RETURN = 0x24, // mcmdReturn
  MACRO_OP_GOSUB = 0x25, // mcmdGosub
  MACRO_OP_TRAP_EVENT = 0x28, // mcmdTrapEvent
  MACRO_OP_UNTRAP_EVENT = 0x29, // mcmdUntrapEvent
  MACRO_OP_SEND_MESSAGE = 0x2A, // mcmdSendMessage
  MACRO_OP_GET_MESSAGE = 0x2B, // mcmdGetMessage
  MACRO_OP_GET_VID = 0x2C, // mcmdGetVID
  MACRO_OP_ADD_AGE_COUNTER = 0x30, // mcmdAddAgeCounter
  MACRO_OP_SET_AGE_COUNTER = 0x31, // mcmdSetAgeCounter
  MACRO_OP_SEND_FLAG = 0x32, // mcmdSendFlag
  MACRO_OP_SET_PITCH_WHEEL_RANGE = 0x33, // mcmdSetPitchWheelRange
  MACRO_OP_SCALE_REVERB = 0x34, // mcmdScaleReverb
  MACRO_OP_SET_PITCHBEND_AFTER_KEY_OFF = 0x35, // mcmdSetPitchbendAfterKeyOff
  MACRO_OP_SET_PRIORITY = 0x36, // mcmdSetPriority
  MACRO_OP_ADD_PRIORITY = 0x37, // mcmdAddPriority
  MACRO_OP_SET_AGE_COUNTER_SPEED = 0x38, // mcmdSetAgeCounterSpeed
  MACRO_OP_SET_AGE_COUNTER_BY_VOLUME = 0x39, // mcmdSetAgeCounterByVolume
  MACRO_OP_VOLUME_SELECT = 0x40, // mcmdVolumeSelect
  MACRO_OP_PANNING_SELECT = 0x41, // mcmdPanningSelect
  MACRO_OP_PITCH_WHEEL_SELECT = 0x42, // mcmdPitchWheelSelect
  MACRO_OP_MOD_WHEEL_SELECT = 0x43, // mcmdModWheelSelect
  MACRO_OP_PEDAL_SELECT = 0x44, // mcmdPedalSelect
  MACRO_OP_PORTAMENTO_SELECT = 0x45, // mcmdPortamentoSelect
  MACRO_OP_REVERB_SELECT = 0x46, // mcmdReverbSelect
  MACRO_OP_SURROUND_PANNING_SELECT = 0x47, // mcmdSurroundPanningSelect
  MACRO_OP_DOPPLER_SELECT = 0x48, // mcmdDopplerSelect
  MACRO_OP_TREMOLO_SELECT = 0x49, // mcmdTremoloSelect
  MACRO_OP_PRE_AUX_A_SELECT = 0x4A, // mcmdPreAuxASelect
  MACRO_OP_PRE_AUX_B_SELECT = 0x4B, // mcmdPreAuxBSelect
  MACRO_OP_POST_AUX_B_SELECT = 0x4C, // mcmdPostAuxBSelect
  MACRO_OP_AUX_AFX_SELECT = 0x4D, // mcmdAuxAFXSelect
  MACRO_OP_AUX_BFX_SELECT = 0x4E, // mcmdAuxBFXSelect
  MACRO_OP_SETUP_LFO = 0x50, // mcmdSetupLFO
  MACRO_OP_MODE_SELECT = 0x58, // mcmdModeSelect
  MACRO_OP_SET_KEY_GROUP = 0x59, // mcmdSetKeyGroup
  MACRO_OP_SRC_MODE_SELECT = 0x5A, // mcmdSRCModeSelect
  MACRO_OP_FILTER_SWITCH_SELECT = 0x5E, // mcmdFilterSwitchSelect
  MACRO_OP_FILTER_PARAMETER_SELECT = 0x5F, // mcmdFilterParameterSelect
  MACRO_OP_VAR_ADD = 0x60, // mcmdVarCalculation: case 0 adds
  MACRO_OP_VAR_SUB = 0x61, // mcmdVarCalculation: case 1 subtracts
  MACRO_OP_VAR_MUL = 0x62, // mcmdVarCalculation: case 2 multiplies
  MACRO_OP_VAR_DIV = 0x63, // mcmdVarCalculation: case 3 divides (0 for a zero divisor)
  MACRO_OP_VAR_ADD_ALT = 0x64, // mcmdVarCalculation: op 4 shares the add case
  MACRO_OP_SET_VAR_IMMEDIATE = 0x65, // mcmdSetVarImmediate
  MACRO_OP_IF_VAR_EQUAL = 0x70, // mcmdIfVarCompare: cmp 0: a == b
  MACRO_OP_IF_VAR_LESS = 0x71, // mcmdIfVarCompare: cmp 1: a < b
};

typedef struct synthITDInfo {
  // total size: 0x2
  u8 music; // offset 0x0, size 0x1
  u8 sfx;   // offset 0x1, size 0x1
} synthITDInfo;

#pragma pop
typedef void (*SYNTH_MESSAGE_CALLBACK)(u32, s32);

extern SND_AUX_CALLBACK synthAuxACallback[8];
extern u8 synthAuxAMIDI[8];
extern u8 synthAuxAMIDISet[8];
extern void* synthAuxAUser[8];
extern SND_AUX_CALLBACK synthAuxBCallback[8];
extern u8 synthAuxBMIDI[8];
extern u8 synthAuxBMIDISet[8];
extern void* synthAuxBUser[8];

extern synthITDInfo synthITDDefault[8];
extern u32 synthFlags;
extern SYNTH_VOICE* synthVoice;
extern u8 sndActive;
extern u8 synthIdleWaitActive;
extern SynthInfo synthInfo;
extern s32 synthGlobalVariable[16];
extern u64 synthRealTime;
extern SYNTH_MESSAGE_CALLBACK synthMessageCallback;

u32 synthGetTicksPerSecond(SYNTH_VOICE* svoice);
void synthKillVoicesByMacroReferences(u16* ref);
void synthKillVoicesBySampleReferences(u16* ref);
void synthExit();
void synthInit(u32 mixFrq, u32 numVoices);
void synthSetBpm(u32 pbm, u8 set, u8 section);
void synthFXCloneMidiSetup(SYNTH_VOICE* dest, SYNTH_VOICE* src);
void synthSetMusicVolumeType(u8 vGroup, u8 type);
void synthInitAllocationAids();
void synthInitPortamento(SYNTH_VOICE* svoice);
void synthStartSynthJobHandling(SYNTH_VOICE* svoice);
void synthForceLowPrecisionUpdate(SYNTH_VOICE* svoice);
void synthKeyStateUpdate(SYNTH_VOICE* svoice);
void synthHandle(u32 deltaTime);
bool synthFXSetCtrl(SND_VOICEID vid, u8 ctrl, u8 value);
bool synthFXSetCtrl14(SND_VOICEID vid, u8 ctrl, u16 value);
bool synthSendKeyOff(SND_VOICEID vid);
SND_VOICEID synthFXStart(u16 fid,
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 2)
                         u8 key,
#endif
                         u8 vol, u8 pan, u8 studio, u32 itd);
void synthVolume(u8 volume, u16 time, u8 vGroup, u8 seqMode, u32 seqId);
u32 synthStartSound(u16 id, u8 prio, u8 max,
#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 1)
                    u32 sourceID,
#endif
                    u8 key, u8 vol, u8 panning, u8 midi, u8 midiSet, u8 section, u16 step,
                    u16 trackid, u8 vGroup, s16 prioOffset, u8 studio, u32 itd);
bool synthIsFadeOutActive(u8 vGroup);
void synthActivateStudio(u8 studio, u32 isMaster, SND_STUDIO_TYPE type);
void synthDeactivateStudio(u8 studio);
u8 synthFXGetMaxVoices(u16 fid);
void synthPauseVolume(u8 volume, u16 time, u8 vGroup);
void synthKillAllVoices(bool8 musiconly);
void synthKeyStateUpdate(SYNTH_VOICE* svoice);
bool synthAddStudioInput(u8 studio, SND_STUDIO_INPUT* in_desc);
bool synthRemoveStudioInput(u8 studio, SND_STUDIO_INPUT* in_desc);
u32 synthGetTicksPerSecond(SYNTH_VOICE* svoice);
u32 adsrSetup(ADSR_VARS* adsr);

#ifdef __cplusplus
}
#endif

#endif // _MUSYX_SYNTH
