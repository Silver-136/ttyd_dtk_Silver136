// Pianta Parlor (yuu, yugijo "amusement hall"): types and data of its minigames (Monte, MonteTable, SlitWork).

#pragma once

#include "version.h"
#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "manager/evtmgr.h"
#include "driver/camdrv.h"
#include "evt/evt_yuugijou.h"

struct EventEntry;

// Pianta Parlor minigame piece (yuu_00..yuu_03)
typedef struct MonteSub {
    s32 unk0;
    Vec pos;
    f32 scale;
    u8 unk14[0x38 - 0x14];
} MonteSub;
typedef struct Monte {
    u8 hidden;
    u8 unk1;
    s16 num;
    u8 state;
    u8 unk5[3];
    MonteSub sub;
    MiniGameWork* wp;
} Monte;

// Spawn bands of the minigame pieces (Monte) in yuu_00 and yuu_01 (montetable): num pieces in the x0..x1,
// y0..y1 box. yuu_02 has its own x/z table (MonteTableXZ).
typedef struct MonteTable {
    f32 x0;
    f32 x1;
    f32 y0;
    f32 y1;
    s32 num;
} MonteTable;

// minigame 4 (yuu_03) work
typedef struct SlitWork {
    u32 flags;
    s32 phase;
    s32 coins;
    s32 result;
    s32 state;
    s32 timer;
    f32 rot1;
    f32 rot2;
    s32 monteCount;
#if VERSION >= VERSION_JP
    EventEntry* viewEvent;
#endif
} SlitWork;

extern EvtWord evt_yuunpc_minigame1[];
extern EvtWord evt_yuunpc_minigame2[];
extern EvtWord evt_yuunpc_minigame3[];
extern EvtWord evt_yuunpc_minigame4[];
extern EvtWord yuu_00_init_evt[];
extern EvtWord yuu_01_init_evt[];
extern EvtWord yuu_02_init_evt[];
extern EvtWord yuu_03_init_evt[];
extern MiniGameWork* yuunpc_yuuminigame4_ptr;
extern f32 yuusintbl[];

extern EvtWord minigame02_end[];
extern EvtWord minigame02_start[];
extern EvtWord minigame03_end[];
extern EvtWord minigame03_start[];
extern EvtWord minigame04_end[];
extern EvtWord minigame04_start[];
extern s32 minigame_view_monte[];
extern EvtWord yuu_02_icon_monte_entry_evt[];
extern EvtWord yuu_02_jumpret[];
extern EvtWord yuu_03_icon_monte_entry_evt[];

extern EvtWord minigame01_floor_delete[];
void yuucounter_flag_on(u32 mask);
s32 yuucounter_get_data(void);
void yuuminigame_disp_4slit(CameraId camera, void* param);
void yuuminigame_disp_3roll(CameraId camera, void* param);
void yuuminigame_disp_2fune(CameraId camera, void* param);
void yuuminigame_disp_1hikoki(CameraId camera, void* param);
void ext_yuunpc_minigame4_init_sub(void);
s32 yuunpc_juni(Vec* pos);

// yuunpc_wp_flag_on/off/check
#define YUUNPC_WP_FLAG_WAIT 1 // race NPCs hold their start until yuunpc_go
void yuunpc_wp_flag_off(u16 flag);
void yuuminigame_init_1hikoki(struct EventEntry* event, BOOL isFirstCall);
void yuuminigame_main_1hikoki(struct EventEntry* event, BOOL isFirstCall);
void yuuminigame_init_2fune(struct EventEntry* event, BOOL isFirstCall);
void yuuminigame_main_2fune(struct EventEntry* event, BOOL isFirstCall);
void yuuminigame_init_3roll(struct EventEntry* event, BOOL isFirstCall);
void yuuminigame_main_3roll(struct EventEntry* event, BOOL isFirstCall);
void yuuminigame_init_4slit(struct EventEntry* event, BOOL isFirstCall);
void yuuminigame_main_4slit(struct EventEntry* event, BOOL isFirstCall);

USER_FUNC(eff_call_minigame);
USER_FUNC(evt_yuucounter_alloc);
USER_FUNC(evt_yuucounter_free);
USER_FUNC(evt_yuuminigame_alloc);
USER_FUNC(evt_yuutitle_alloc);
USER_FUNC(evt_yuutitle_free);
USER_FUNC(yuu_atai_init);
USER_FUNC(yuu_atai_pop);
USER_FUNC(yuu_atai_push);
USER_FUNC(yuuminigame_get_dataadr);
USER_FUNC(yuuminigame_set_dataadr);
USER_FUNC(yuuranking_get_monte);
