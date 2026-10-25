#pragma defer_codegen on
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "driver/npcdrv.h"
#include "driver/extdrv.h"
#include "string.h"
#include "dolphin/os.h"
#include "mario/mariost.h"
#include "driver/animdrv.h"
#include "stdlib.h"
#include "driver/camdrv.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "memory.h"
#include "driver/dispdrv.h"
#include "pmario_sound.h"
#include "yuu.h"
#include "data_tables.h"
#include <math.h>

typedef struct YuuNpcPose { char name[16]; char anim[8]; u16 field_0x18; f32 frames; f32 jump; s32 start; s32 count; u16 flags; f32 field_0x30; } YuuNpcPose;
typedef struct YuuNpc { u16 id; u16 flags; s16 state; f32 field_0x8; Vec goal; Vec dest; Vec pos; Vec dir; f32 speed; f32 vy; f32 field_0x44; f32 zofs; s32 field_0x4c; s32 field_0x50; s16 poseId; s16 poses[4]; u8 field_0x5e[2]; NpcEntry* npc; f32 animSpeed; u64 time; } YuuNpc;
typedef struct YuuNpcWork { u16 flags; s32 numNpc; s32 slot[3]; YuuNpc* npcs; s32 basePoseNum; s32 numPoses; s32 chusanPose; s16 hit[6 * 72]; s16 hitY[72]; YuuNpcPose pose[32]; } YuuNpcWork;
static YuuNpcWork* yuunpc_wp;
typedef struct YuuNpcData { const char* name; const char* anim; f32 frames; } YuuNpcData;
s32 yuunpc_pose_check(const char* name, const char* anim);
void yuunpc_pose_change(YuuNpc* npc, s16 id, f32 speed, f32 frame);
f32 yuusintbl[450];
s32 chusan_mov_tbl[28];
s32 chusan_act_tbl[8];
f32 yuunpc_pose_sub(ExtdrvPose* ext, YuuNpc* npc);
void ext_yuunpc_minigame4_init_sub(void);
MiniGameWork* yuunpc_yuuminigame4_ptr;
void yuunpc_disp(CameraId camera, void* param);
void yuunpc_pose_minigame1_main(void);
void yuunpc_pose_minigame2_main(void);
void yuunpc_pose_minigame3_main(void);
void yuunpc_pose_minigame4_main(void);

USER_FUNC(evt_ext_entry);
USER_FUNC(evt_npc_delete);
USER_FUNC(evt_npc_entry);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_seq_wait);
void ext_yuunpc_minigame4_main(void);
void ext_yuunpc_minigame3_main(void);
void ext_yuunpc_minigame2_main(void);
void ext_yuunpc_minigame1_main(void);
void ext_yuunpc_dispent(void);
void yuunpc_disp(CameraId camera, void* param);
void yuunpc_pose_minigame4_main(void);
void ext_yuunpc_minigame4_init(void);
void ext_yuunpc_minigame4_init_sub(void);
s32 yuunpc_juni(Vec* pos);
u16 yuunpc_wp_flag_check(u16 flag);
void yuunpc_wp_flag_off(u16 flag);
void yuunpc_wp_flag_on(u16 flag);
void errper_set_rtn(YuuNpc* npc);
s32 goalincheck(YuuNpc* npc);
s32 fallcheck(YuuNpc* npc);
s32 hitcheck(YuuNpc* npc);
void yuunpc_pose_minigame3_main(void);
void ext_yuunpc_minigame3_init(void);
void yuunpc_pose_minigame2_main(void);
void ext_yuunpc_minigame2_init(void);
void yuunpc_pose_minigame1_main(void);
void ext_yuunpc_minigame1_init(void);
f32 yuunpc_pose_sub(ExtdrvPose* ext, YuuNpc* npc);
void yuunpc_set_jump_pose(const char* name, const char* anim, f32 jump);
void yuunpc_pose_change2(YuuNpc* npc, const char* name, const char* anim, f32 speed);
void yuunpc_pose_change(YuuNpc* npc, s16 id, f32 speed, f32 frame);
s32 yuunpc_pose_check(const char* name, const char* anim);
void yuunpc_pose_init(YuuNpcData* data);
void yuunpc_maphit_init(void);

ExtTexInfo ext_yuunpc_minigame1_data[6];
ExtTexInfo ext_yuunpc_minigame2_data[6];
ExtTexInfo ext_yuunpc_minigame3_data[6];
ExtTexInfo ext_yuunpc_minigame4_data[78];
s32 chusan_mov_tbl[28];
s32 chusan_act_tbl[8];
EvtWord evt_yuunpc_minigame1[69];
EvtWord evt_yuunpc_minigame2[69];
EvtWord evt_yuunpc_minigame3[69];
EvtWord evt_yuunpc_minigame4[167];

f32 yuusintbl[] = {
    0.0f, 0.0174519997f, 0.0348990001f, 0.0523359999f, 0.0697560012f, 0.0871559978f, 0.104528002f, 0.121868998f,
    0.139173001f, 0.156434f, 0.173648f, 0.190808997f, 0.207911998f, 0.224950999f, 0.241922006f, 0.258819014f,
    0.275637001f, 0.292371988f, 0.309017003f, 0.325567991f, 0.342020005f, 0.358368009f, 0.374606997f, 0.390731007f,
    0.406737f, 0.422618002f, 0.438371003f, 0.453990012f, 0.469471008f, 0.484809995f, 0.5f, 0.515038013f,
    0.529919028f, 0.544638991f, 0.559193015f, 0.573575974f, 0.587785006f, 0.601814985f, 0.615661025f, 0.629320025f,
    0.642787993f, 0.656059027f, 0.669130981f, 0.681998014f, 0.694657981f, 0.707107008f, 0.719340026f, 0.731353998f,
    0.743144989f, 0.754709005f, 0.766044021f, 0.777145982f, 0.788011014f, 0.798635006f, 0.809017003f, 0.819151998f,
    0.829037011f, 0.838670015f, 0.848047972f, 0.857167006f, 0.866024971f, 0.87462002f, 0.882947028f, 0.891005993f,
    0.898793995f, 0.906307995f, 0.913545012f, 0.920504987f, 0.927183986f, 0.933579981f, 0.939692974f, 0.945518017f,
    0.951056004f, 0.956305027f, 0.961261988f, 0.965925992f, 0.970296025f, 0.974370003f, 0.978147984f, 0.981626987f,
    0.984808028f, 0.987688005f, 0.990267992f, 0.992546022f, 0.994521976f, 0.996195018f, 0.997564018f, 0.998629987f,
    0.999391019f, 0.999848008f, 1.0f, 0.999848008f, 0.999391019f, 0.998629987f, 0.997564018f, 0.996195018f,
    0.994521976f, 0.992546022f, 0.990267992f, 0.987688005f, 0.984808028f, 0.981626987f, 0.978147984f, 0.974370003f,
    0.970296025f, 0.965925992f, 0.961261988f, 0.956305027f, 0.951057017f, 0.94551897f, 0.939692974f, 0.933580995f,
    0.927183986f, 0.920504987f, 0.913546026f, 0.906307995f, 0.898793995f, 0.891007006f, 0.882947981f, 0.87462002f,
    0.866025984f, 0.857168019f, 0.848047972f, 0.838671029f, 0.829038024f, 0.819151998f, 0.809017003f, 0.798636019f,
    0.788011014f, 0.777145982f, 0.766044974f, 0.754710019f, 0.743144989f, 0.731353998f, 0.719340026f, 0.707107008f,
    0.694658995f, 0.681999028f, 0.669130981f, 0.656059027f, 0.642787993f, 0.629320979f, 0.615661979f, 0.601814985f,
    0.587786019f, 0.573576987f, 0.559193015f, 0.544638991f, 0.529919982f, 0.515039027f, 0.5f, 0.484809995f,
    0.469471991f, 0.453990996f, 0.438371986f, 0.422618985f, 0.406737f, 0.39073199f, 0.374606997f, 0.358368993f,
    0.342020988f, 0.325569004f, 0.309017003f, 0.292371988f, 0.275638014f, 0.258819997f, 0.241923004f, 0.224951997f,
    0.207911998f, 0.190808997f, 0.173648998f, 0.156434998f, 0.139173999f, 0.121870004f, 0.104529001f, 0.0871559978f,
    0.0697569996f, 0.0523369983f, 0.0348999985f, 0.0174529999f, 9.99999997e-07f, -0.0174519997f, -0.0348990001f, -0.0523350015f,
    -0.0697560012f, -0.0871549994f, -0.104528002f, -0.121868998f, -0.139173001f, -0.156434f, -0.173648f, -0.190807998f,
    -0.207911f, -0.224950001f, -0.241920993f, -0.258819014f, -0.275637001f, -0.292371005f, -0.309015989f, -0.325567991f,
    -0.342018992f, -0.358366996f, -0.374606013f, -0.390729994f, -0.406735986f, -0.422618002f, -0.438371003f, -0.453990012f,
    -0.469471008f, -0.484809011f, -0.499998987f, -0.515037f, -0.529919028f, -0.544638991f, -0.559192002f, -0.573575974f,
    -0.587785006f, -0.601813972f, -0.615661025f, -0.629320025f, -0.64278698f, -0.656058013f, -0.669130027f, -0.681998014f,
    -0.694657981f, -0.707105994f, -0.719339013f, -0.731352985f, -0.743143976f, -0.754709005f, -0.766044021f, -0.777145028f,
    -0.788010001f, -0.798635006f, -0.809015989f, -0.819151998f, -0.829037011f, -0.838670015f, -0.848047972f, -0.857167006f,
    -0.866024971f, -0.874619007f, -0.882947028f, -0.891005993f, -0.898793995f, -0.906306982f, -0.913545012f, -0.920503974f,
    -0.927182972f, -0.933579981f, -0.93969202f, -0.945518017f, -0.951056004f, -0.956304014f, -0.961260974f, -0.965925992f,
    -0.970295012f, -0.974370003f, -0.97814697f, -0.981626987f, -0.984808028f, -0.987688005f, -0.990267992f, -0.992546022f,
    -0.994521976f, -0.996195018f, -0.997564018f, -0.998628974f, -0.999391019f, -0.999848008f, -1.0f, -0.999848008f,
    -0.999391019f, -0.998629987f, -0.997564018f, -0.996195018f, -0.994521976f, -0.992546022f, -0.990267992f, -0.987688005f,
    -0.984808028f, -0.981626987f, -0.978147984f, -0.974370003f, -0.970296025f, -0.965925992f, -0.961261988f, -0.956305027f,
    -0.951057017f, -0.94551897f, -0.939692974f, -0.933580995f, -0.927183986f, -0.920504987f, -0.913546026f, -0.906307995f,
    -0.898795009f, -0.891007006f, -0.882947981f, -0.87462002f, -0.866025984f, -0.857168019f, -0.848048985f, -0.838671029f,
    -0.829038024f, -0.819153011f, -0.809018016f, -0.798636019f, -0.788011014f, -0.777146995f, -0.766044974f, -0.754710019f,
    -0.743146002f, -0.731353998f, -0.71934098f, -0.707108021f, -0.694658995f, -0.681999028f, -0.669131994f, -0.65605998f,
    -0.642787993f, -0.629320979f, -0.615661979f, -0.601815999f, -0.587786019f, -0.573576987f, -0.559194028f, -0.544640005f,
    -0.529919982f, -0.515039027f, -0.500001013f, -0.484811008f, -0.469473004f, -0.453990996f, -0.438371986f, -0.422618985f,
    -0.406737f, -0.39073199f, -0.37460801f, -0.358368993f, -0.342022002f, -0.325569004f, -0.309017986f, -0.292373002f,
    -0.275638014f, -0.258819997f, -0.241923004f, -0.224951997f, -0.207912996f, -0.190809995f, -0.173648998f, -0.156434998f,
    -0.139174998f, -0.121871002f, -0.104529999f, -0.0871570036f, -0.069757998f, -0.0523369983f, -0.0349010006f, -0.0174540002f,
    -9.99999997e-07f, 0.0174509995f, 0.0348990001f, 0.0523350015f, 0.0697550029f, 0.087154001f, 0.104526997f, 0.121867999f,
    0.139172003f, 0.156433001f, 0.173647001f, 0.190807998f, 0.207910001f, 0.224950001f, 0.241920993f, 0.258818001f,
    0.275635988f, 0.292371005f, 0.309015006f, 0.325567007f, 0.342018992f, 0.358366996f, 0.374605f, 0.390729994f,
    0.406735986f, 0.422616988f, 0.438369989f, 0.453990012f, 0.469471008f, 0.484809011f, 0.499998987f, 0.515037f,
    0.529918015f, 0.544637978f, 0.559190989f, 0.57357502f, 0.587783992f, 0.601813972f, 0.615661025f, 0.629319012f,
    0.64278698f, 0.656058013f, 0.669130027f, 0.681997001f, 0.694657028f, 0.707104981f, 0.719339013f, 0.731352985f,
    0.743143976f, 0.754709005f, 0.766044021f, 0.777145028f, 0.788010001f, 0.798635006f, 0.809015989f, 0.819150984f,
    0.829037011f, 0.838670015f, 0.848047018f, 0.857165992f, 0.866024971f, 0.874619007f, 0.882947028f, 0.891005993f,
    0.898792982f, 0.906306982f, 0.913545012f, 0.920503974f, 0.927182972f, 0.933579981f, 0.93969202f, 0.945518017f,
    0.951056004f, 0.956304014f, 0.961260974f, 0.965924978f, 0.970295012f, 0.974370003f, 0.97814697f, 0.981626987f,
    0.984808028f, 0.987688005f, 0.990267992f, 0.992546022f, 0.994521976f, 0.996195018f, 0.997564018f, 0.998628974f,
    0.999391019f, 0.999848008f,
};

signed char hitdata[] = "000000000000000000000000001617181911121314151122231102030405060607080910000000000000000000242526272828282828000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000747474747474744949242400000000000000000000000000000000000000000000000000\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\241\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\241\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\241\201\241\201\241\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\241\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\241\201\241\201\241\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\241\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240\201\240";

ExtTexInfo ext_yuunpc_minigame1_data[] = {
    {"c_kinopiko", "KNF_S_1", 0.0f},
    {"c_roten", "S_1", 0.0f},
    {"c_jugemu", "JGM_S_1", 0.0f},
    {"c_kinopio", "KNP_S_1", 0.0f},
    {0},
    {0},
};

ExtTexInfo ext_yuunpc_minigame2_data[] = {
    {"c_kinopiko", "KNF_S_1", 0.0f},
    {"c_roten", "S_1", 0.0f},
    {"c_jugemu", "JGM_S_1", 0.0f},
    {"c_kinopio", "KNP_S_1", 0.0f},
    {0},
    {0},
};

ExtTexInfo ext_yuunpc_minigame3_data[] = {
    {"c_kinopiko", "KNF_S_1", 0.0f},
    {"c_roten", "S_1", 0.0f},
    {"c_jugemu", "JGM_S_1", 0.0f},
    {"c_kinopio", "KNP_S_1", 0.0f},
    {0},
    {0},
};

ExtTexInfo ext_yuunpc_minigame4_data[] = {
    {"c_kinopiko", "KNF_S_1", 0.0f},
    {"c_roten", "S_1", 0.0f},
    {"c_jugemu", "JGM_S_1", 0.0f},
    {"c_kinopio", "KNP_S_1", 0.0f},
    {"c_kinopio", "KNP_R_1", 0.0f},
    {"c_kinopio", "KNP_R_1", 1.0f},
    {"c_kinopio", "KNP_R_1", 2.0f},
    {"c_kinopio", "KNP_R_1", 3.0f},
    {"c_kinopio", "KNP_R_1", 4.0f},
    {"c_kinopio", "KNP_R_1", 5.0f},
    {"c_kinopio", "KNP_R_1", 6.0f},
    {"c_kinopio", "KNP_R_1", 7.0f},
    {"c_kinopio", "KNP_R_1", 8.0f},
    {"c_kinopio", "KNP_R_1", 9.0f},
    {"c_kinopio", "KNP_R_1", 10.0f},
    {"c_kinopio", "KNP_R_1", 11.0f},
    {"c_kinopio", "KNP_R_1", 12.0f},
    {"c_kinopio", "KNP_R_1", 13.0f},
    {"c_kinopio", "KNP_R_1", 14.0f},
    {"c_kinopio", "KNP_R_1", 15.0f},
    {"c_kinopio", "KNP_R_1", 16.0f},
    {"c_kinopio", "KNP_R_1", 17.0f},
    {"c_kinopio", "KNP_R_1", 18.0f},
    {"c_kinopio", "KNP_R_1", 19.0f},
    {"c_kinopio", "KNP_R_1", 20.0f},
    {"c_kinopio", "KNP_R_1", 21.0f},
    {"c_kinopio", "KNP_R_1", 22.0f},
    {"c_kinopio", "KNP_R_1", 23.0f},
    {"c_bom_n", "S_1", 0.0f},
    {"c_bom_n", "R_1", 0.0f},
    {"c_bom_n", "R_1", 1.0f},
    {"c_bom_n", "R_1", 2.0f},
    {"c_bom_n", "R_1", 3.0f},
    {"c_bom_n", "R_1", 4.0f},
    {"c_bom_n", "R_1", 5.0f},
    {"c_bom_n", "R_1", 6.0f},
    {"c_bom_n", "R_1", 7.0f},
    {"c_bom_n", "R_1", 8.0f},
    {"c_bom_n", "R_1", 9.0f},
    {"c_bom_n", "R_1", 10.0f},
    {"c_bom_n", "R_1", 11.0f},
    {"c_bom_n", "R_1", 12.0f},
    {"c_bom_n", "R_1", 13.0f},
    {"c_bom_n", "R_1", 14.0f},
    {"c_bom_n", "R_1", 15.0f},
    {"c_bom_n", "R_1", 16.0f},
    {"c_bom_n", "R_1", 17.0f},
    {"c_bom_n", "R_1", 18.0f},
    {"c_bom_n", "R_1", 19.0f},
    {"c_tyusan", "CHU_S_1", 0.0f},
    {"c_tyusan", "CHU_S_1", 131.0f},
    {"c_tyusan", "CHU_S_1", 132.0f},
    {"c_tyusan", "CHU_S_1", 133.0f},
    {"c_tyusan", "CHU_R_1", 0.0f},
    {"c_tyusan", "CHU_R_1", 1.0f},
    {"c_tyusan", "CHU_R_1", 2.0f},
    {"c_tyusan", "CHU_R_1", 3.0f},
    {"c_tyusan", "CHU_R_1", 4.0f},
    {"c_tyusan", "CHU_R_1", 5.0f},
    {"c_tyusan", "CHU_R_1", 6.0f},
    {"c_tyusan", "CHU_R_1", 7.0f},
    {"c_tyusan", "CHU_R_1", 8.0f},
    {"c_tyusan", "CHU_R_1", 9.0f},
    {"c_tyusan", "CHU_R_1", 10.0f},
    {"c_tyusan", "CHU_R_1", 11.0f},
    {"c_tyusan", "CHU_R_1", 12.0f},
    {"c_tyusan", "CHU_R_1", 13.0f},
    {0},
    {"c_luigi", "S_1", 40.0f},
    {"c_luigi", "W_1", 40.0f},
    {"c_luigi", "R_1", 20.0f},
    {"c_nokorin", "S_1", 40.0f},
    {"c_nokorin", "W_1", 40.0f},
    {"c_nokorin", "R_1", 20.0f},
    {"c_joho_a", "S_1", 60.0f},
    {"c_joho_a", "W_1", 48.0f},
    {"c_joho_a", "R_1", 24.0f},
    {0},
};

s32 chusan_mov_tbl[] = {
    0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 1, 1,
    0, 0, 0, 0,
};

s32 chusan_act_tbl[] = {
    1, 2, 3, 3, 3, 2, 1, 0,
};

void yuunpc_maphit_init(void) {
    signed char key[3] = "\x81\xA1";
    s32 i;
    s32 j;

    for (i = 0; i < 72; i++) {
        yuunpc_wp->hitY[i] = (hitdata[i * 2] - '0') * 10 + (hitdata[i * 2 + 1] - '0');
    }
    for (i = 0; i < 72; i++) {
        s16 h = -((hitdata[0x90 + i * 2] - '0') * 10 + (hitdata[0x90 + i * 2 + 1] - '0'));
        yuunpc_wp->hit[i + 72 * 5] = h;
        yuunpc_wp->hit[i + 72 * 4] = h;
        yuunpc_wp->hit[i + 72 * 3] = h;
        yuunpc_wp->hit[i + 72 * 2] = h;
        yuunpc_wp->hit[i + 72 * 1] = h;
        yuunpc_wp->hit[i] = h;
    }
    for (j = 0; j < 6; j++) {
        for (i = 0; i < 72; i++) {
            if (key[0] == hitdata[0x120 + j * 0x90 + i * 2] && key[1] == hitdata[0x120 + j * 0x90 + i * 2 + 1]) {
                yuunpc_wp->hit[i + j * 72] = -500;
            }
        }
    }
}

void yuunpc_pose_init(YuuNpcData* data) {
    char name[32];
    char anim[32];
    s32 n = 0;
    s32 start;
    s32 i;

    strcpy(name, "");
    strcpy(anim, "");
    yuunpc_wp->numPoses = -1;
    do {
        if (strcmp(data->name, name) != 0) {
            strcpy(name, data->name);
            strcpy(anim, "");
        }
        if (strcmp(data->anim, anim) != 0) {
            strcpy(anim, data->anim);
            start = n;
            yuunpc_wp->numPoses++;
        }
        strcpy(yuunpc_wp->pose[yuunpc_wp->numPoses].name, name);
        strcpy(yuunpc_wp->pose[yuunpc_wp->numPoses].anim, anim);
        yuunpc_wp->pose[yuunpc_wp->numPoses].start = start;
        yuunpc_wp->pose[yuunpc_wp->numPoses].count = n - start + 1;
        n++;
        yuunpc_wp->pose[yuunpc_wp->numPoses].frames += 1.0f;
        yuunpc_wp->pose[yuunpc_wp->numPoses].field_0x18 = 1;
        yuunpc_wp->pose[yuunpc_wp->numPoses].jump = 0.0f;
        yuunpc_wp->pose[yuunpc_wp->numPoses].field_0x30 = 2.0f;
        data++;
    } while (data->name != NULL);
    data++;
    yuunpc_wp->numPoses++;
    yuunpc_wp->basePoseNum = yuunpc_wp->numPoses;
    while (data->name != NULL) {
        strcpy(yuunpc_wp->pose[yuunpc_wp->numPoses].name, data->name);
        strcpy(yuunpc_wp->pose[yuunpc_wp->numPoses].anim, data->anim);
        yuunpc_wp->pose[yuunpc_wp->numPoses].start = start;
        yuunpc_wp->pose[yuunpc_wp->numPoses].count = 1;
        yuunpc_wp->pose[yuunpc_wp->numPoses].frames = data->frames;
        data++;
        yuunpc_wp->pose[yuunpc_wp->numPoses].field_0x18 = 1;
        yuunpc_wp->pose[yuunpc_wp->numPoses].jump = 0.0f;
        yuunpc_wp->pose[yuunpc_wp->numPoses].field_0x30 = 1.0f;
        yuunpc_wp->pose[yuunpc_wp->numPoses].flags |= 1;
        yuunpc_wp->numPoses++;
    }
    for (i = 0; i < yuunpc_wp->basePoseNum; i++) {
        if (strcmp(yuunpc_wp->pose[i].name, "c_tyusan") == 0 && strcmp(yuunpc_wp->pose[i].anim, "CHU_S_1") == 0) {
            yuunpc_wp->chusanPose = i;
            yuunpc_wp->pose[i].frames = 224.0f;
        }
    }
}

s32 yuunpc_pose_check(const char* name, const char* anim) {
    YuuNpcPose* pose = yuunpc_wp->pose;
    s32 i;

    for (i = 0; i < yuunpc_wp->numPoses; i++, pose++) {
        if (strcmp(pose->name, name) == 0 && strcmp(pose->anim, anim) == 0) {
            break;
        }
    }
    if (i >= yuunpc_wp->numPoses) {
        return -1;
    }
    return i;
}

static inline void pose_change_inl(YuuNpc* npc, s16 id, f32 speed, f32 frame) {
    YuuNpcPose* cur = &yuunpc_wp->pose[npc->poseId];
    YuuNpcPose* pose = &yuunpc_wp->pose[id];

    if (speed <= 0.0f) {
        npc->flags |= 4;
        npc->poseId = id;
        npc->animSpeed = 1.0f;
        npc->time = OSMillisecondsToTicks(1000.0f * (frame * (pose->frames * npc->animSpeed)) / 60.0f);
    } else {
        u16 flags = npc->flags;
        npc->flags &= ~4;
        if (npc->poseId == id) {
            f32 old = npc->animSpeed;
            if (old != speed) {
                if (flags & 4) {
                    frame = 60.0f * OSTicksToMilliseconds(npc->time) / 1000.0f;
                    frame /= pose->frames * old;
                } else {
                    frame = 60.0f * OSTicksToMilliseconds(gp->renderTime - npc->time) / 1000.0f;
                    frame /= cur->frames * old;
                }
            } else {
                return;
            }
        } else {
            npc->poseId = id;
        }
        npc->animSpeed = speed;
        npc->time = gp->renderTime - OSMillisecondsToTicks(1000.0f * (frame * (pose->frames * npc->animSpeed)) / 60.0f);
    }
    switch (npc->flags & 0x300) {
        case 0:
            break;
        case 0x100:
        case 0x200:
        case 0x300:
            animPoseSetAnim(npc->npc->poseId, pose->anim, TRUE);
            animPoseMain(npc->npc->poseId);
            break;
    }
}

void yuunpc_pose_change(YuuNpc* npc, s16 id, f32 speed, f32 frame) {
    YuuNpcPose* cur = &yuunpc_wp->pose[npc->poseId];
    YuuNpcPose* pose = &yuunpc_wp->pose[id];

    if (speed <= 0.0f) {
        npc->flags |= 4;
        npc->poseId = id;
        npc->animSpeed = 1.0f;
        npc->time = OSMillisecondsToTicks(1000.0f * (frame * (pose->frames * npc->animSpeed)) / 60.0f);
    } else {
        u16 flags = npc->flags;
        npc->flags = flags & ~4;
        if (npc->poseId == id) {
            f32 old = npc->animSpeed;
            if (old != speed) {
                if (flags & 4) {
                    frame = 60.0f * OSTicksToMilliseconds(npc->time) / 1000.0f;
                    frame /= pose->frames * old;
                } else {
                    frame = 60.0f * OSTicksToMilliseconds(gp->renderTime - npc->time) / 1000.0f;
                    frame /= cur->frames * old;
                }
            } else {
                return;
            }
        } else {
            npc->poseId = id;
        }
        npc->animSpeed = speed;
        npc->time = gp->renderTime - OSMillisecondsToTicks(1000.0f * (frame * (pose->frames * npc->animSpeed)) / 60.0f);
    }
    switch (npc->flags & 0x300) {
        case 0:
            break;
        case 0x100:
        case 0x200:
        case 0x300:
            animPoseSetAnim(npc->npc->poseId, pose->anim, TRUE);
            animPoseMain(npc->npc->poseId);
            break;
    }
}

void yuunpc_pose_change2(YuuNpc* npc, const char* name, const char* anim, f32 speed) {
    pose_change_inl(npc, yuunpc_pose_check(name, anim), speed, rand() / 32767.0f);
}

void yuunpc_set_jump_pose(const char* name, const char* anim, f32 jump) {
    s32 id = yuunpc_pose_check(name, anim);

    if (id >= 0) {
        yuunpc_wp->pose[id].jump = jump;
    }
}

f32 yuunpc_pose_sub(ExtdrvPose* ext, YuuNpc* npc) {
    YuuNpcPose* pose = &yuunpc_wp->pose[npc->poseId];
    f32 t;
    f32 a;

    if (!(ext->flags & 2)) {
        f32 c, s;
        a = ext->dir; a = 450.0f - a;
        if (a >= 360.0f) {
            a -= 360.0f;
        }
        s = yuusintbl[(s32)a];
        c = yuusintbl[(s32)a + 90];
        (*ext->mtx)[0][0] = c;
        (*ext->mtx)[2][2] = c;
        (*ext->mtx)[0][2] = s;
        (*ext->mtx)[2][0] = -s;
    }
    if (npc->flags & 4) {
        t = 60.0f * OSTicksToMilliseconds(npc->time) / 1000.0f;
        t /= pose->frames * npc->animSpeed;
    } else {
        OSTime now = gp->renderTime;
        t = 0.5f + 60.0f * OSTicksToMilliseconds(now - npc->time) / 1000.0f;
        t /= pose->frames * npc->animSpeed;
        if (t >= 1.0f) {
            npc->time = now;
            t = 0.0f;
        }
    }
    switch (npc->flags & 0x300) {
        case 0x100:
        case 0x200:
        case 0x300: {
            f32 rot;
            animPoseSetLocalTime(npc->npc->poseId, (s32)(pose->frames * t));
            rot = 3600.0f + (90.0f - ext->dir);
            npc->npc->rotation.y = rot - (s32)(rot / 360.0f) * 360;
            npc->npc->position = npc->pos;
            break;
        }
        case 0:
            if (npc->poseId != yuunpc_wp->chusanPose) {
                ext->texId = pose->start + (s32)(pose->count * t);
            } else {
                s32 k = 224.0f * t;
                ext->texId = pose->start + (chusan_mov_tbl[k >> 3] ? chusan_act_tbl[k & 7] : 0);
            }
            break;
    }
    return t;
}

void ext_yuunpc_minigame1_init(void) {
    f32 ytbl[5] = {260.0f, 220.0f, 180.0f, 140.0f, 100.0f};
    f32 ztbl[8] = {-7.0f, -1.0f, 5.0f, -5.0f, 1.0f, 7.0f, -3.0f, 3.0f};
    s32 i;
    ExtdrvPose* ext;
    YuuNpc* npc;

    yuunpc_wp = _mapAlloc(sizeof(YuuNpcWork));
    memset(yuunpc_wp, 0, sizeof(YuuNpcWork));
    yuunpc_wp->numNpc = extGetPoseNum();
    yuunpc_wp->npcs = _mapAlloc(yuunpc_wp->numNpc * sizeof(YuuNpc));
    memset(yuunpc_wp->npcs, 0, yuunpc_wp->numNpc * sizeof(YuuNpc));
    yuunpc_pose_init((YuuNpcData*)ext_yuunpc_minigame1_data);
    ext = extGetPosePtr();
    npc = yuunpc_wp->npcs;
    for (i = 0; i < yuunpc_wp->numNpc; i++, ext++, npc++) {
        s32 row;
        f32 z;
        s16 id;

        npc->poseId = yuunpc_wp->numPoses;
        row = i / 5;
        npc->id = i;
        npc->flags |= 1;
        ext->flags |= 7;
        PSMTXIdentity(*ext->mtx);
        z = 30.0f + (60.0f * (i % 5) + -560.0f);
        (*ext->mtx)[0][3] = 16.666666f * row - 500.0f;
        (*ext->mtx)[1][3] = ytbl[(s32)((z - -560.0f) / 60.0f)];
        (*ext->mtx)[2][3] = z + ztbl[row % 8];
        ext->dir = irand(2) * 180 + 90;
        npc->animSpeed = 1.0f;
        switch (irand(1000) % 4) {
            case 0:
                id = yuunpc_pose_check("c_kinopio", "KNP_S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
            case 1:
                id = yuunpc_pose_check("c_kinopiko", "KNF_S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
            case 2:
                id = yuunpc_pose_check("c_roten", "S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
            case 3:
                id = yuunpc_pose_check("c_jugemu", "JGM_S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
        }
        pose_change_inl(npc, npc->poses[0], 1.0f, rand() / 32767.0f);
    }
}

void yuunpc_pose_minigame1_main(void) {
    s32 i;
    ExtdrvPose* ext;
    f32 right;
    f32 camX = camGetPtr(CAMERA_3D)->cameraAt.x;
    YuuNpc* npc;
    f32 x, y, z;

    ext = extGetPosePtr();
    npc = yuunpc_wp->npcs;
    right = 500.0f + camX;

    for (i = 0; i < yuunpc_wp->numNpc; i++, ext++, npc++) {
        if (npc->flags & 1) {
            x = (*ext->mtx)[0][3];
            if (x > camX) {
                while (TRUE) {
                    if (!(x > right)) break;
                    x -= 1000.0f;
                    (*ext->mtx)[0][3] = x;
                }
            } else {
                while (TRUE) {
                    if (!(x < camX - 500.0f)) break;
                    x += 1000.0f;
                    (*ext->mtx)[0][3] = x;
                }
            }
            z = (*ext->mtx)[2][3];
            if (z < -260.0f) {
                y = (*ext->mtx)[1][3];
                if ((x >= -5000.0f && x <= -1500.0f) || (x >= 1700.0f && x <= 5000.0f) || (x >= 6000.0f && x <= 7000.0f)) {
                    if (y > -500.0f) {
                        y += -1000.0f;
                        (*ext->mtx)[1][3] = y;
                    }
                } else {
                    if (y < -500.0f) {
                        y -= -1000.0f;
                        (*ext->mtx)[1][3] = y;
                    }
                }
            }
            if (irand(150) == 0) {
                if (irand(101) <= 80) {
                    ext->dir = angleABf(camX, 0.0f, x, z);
                } else {
                    ext->dir = angleABf(camX, 0.0f, x - 2.0f * (x - camX), z);
                }
            }
        }
        yuunpc_pose_sub(ext, npc);
    }
}

void ext_yuunpc_minigame2_init(void) {
    f32 ytbl[4] = {170.0f, 130.0f, 90.0f, 50.0f};
    f32 ztbl[8] = {-7.0f, -1.0f, 5.0f, -5.0f, 1.0f, 7.0f, -3.0f, 3.0f};
    s32 i;
    ExtdrvPose* ext;
    YuuNpc* npc;

    yuunpc_wp = _mapAlloc(sizeof(YuuNpcWork));
    memset(yuunpc_wp, 0, sizeof(YuuNpcWork));
    yuunpc_wp->numNpc = extGetPoseNum();
    yuunpc_wp->npcs = _mapAlloc(yuunpc_wp->numNpc * sizeof(YuuNpc));
    memset(yuunpc_wp->npcs, 0, yuunpc_wp->numNpc * sizeof(YuuNpc));
    yuunpc_pose_init((YuuNpcData*)ext_yuunpc_minigame2_data);
    ext = extGetPosePtr();
    npc = yuunpc_wp->npcs;
    for (i = 0; i < yuunpc_wp->numNpc; i++, ext++, npc++) {
        s32 row;
        f32 z;
        s16 id;

        npc->poseId = yuunpc_wp->numPoses;
        row = i / 4;
        npc->id = i;
        npc->flags |= 1;
        ext->flags |= 7;
        PSMTXIdentity(*ext->mtx);
        z = 30.0f + (60.0f * (i % 4) + -655.0f);
        (*ext->mtx)[0][3] = 13.333333f * row - 500.0f;
        (*ext->mtx)[1][3] = ytbl[(s32)((z - -655.0f) / 60.0f)];
        (*ext->mtx)[2][3] = z + ztbl[row % 8];
        ext->dir = irand(2) * 180 + 90;
        npc->animSpeed = 1.0f;
        switch (irand(1000) % 4) {
            case 0:
                id = yuunpc_pose_check("c_kinopio", "KNP_S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
            case 1:
                id = yuunpc_pose_check("c_kinopiko", "KNF_S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
            case 2:
                id = yuunpc_pose_check("c_roten", "S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
            case 3:
                id = yuunpc_pose_check("c_jugemu", "JGM_S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
        }
        pose_change_inl(npc, npc->poses[0], 1.0f, rand() / 32767.0f);
    }
}

void yuunpc_pose_minigame2_main(void) {
    s32 i;
    ExtdrvPose* ext;
    f32 right;
    f32 camX = camGetPtr(CAMERA_3D)->cameraAt.x;
    YuuNpc* npc;
    f32 x, y, z;

    ext = extGetPosePtr();
    npc = yuunpc_wp->npcs;
    right = 500.0f + camX;

    for (i = 0; i < yuunpc_wp->numNpc; i++, ext++, npc++) {
        if (npc->flags & 1) {
            x = (*ext->mtx)[0][3];
            if (x > camX) {
                while (TRUE) {
                    if (!(x > right)) break;
                    x -= 1000.0f;
                    (*ext->mtx)[0][3] = x;
                }
            } else {
                while (TRUE) {
                    if (!(x < camX - 500.0f)) break;
                    x += 1000.0f;
                    (*ext->mtx)[0][3] = x;
                }
            }
            z = (*ext->mtx)[2][3];
            if (z < -415.0f) {
                y = (*ext->mtx)[1][3];
                if ((x >= -5000.0f && x <= -4000.0f) || (x >= 790.0f && x <= 5000.0f)) {
                    if (y > -500.0f) {
                        (*ext->mtx)[1][3] = (y += -1000.0f);
                    }
                } else if (z < -475.0f) {
                    if (x >= -50.0f && x <= 50.0f) {
                        if (y > -500.0f) {
                            (*ext->mtx)[1][3] = (y += -1000.0f);
                        }
                    } else {
                        if (y < -500.0f) {
                            (*ext->mtx)[1][3] = y - -1000.0f;
                        }
                    }
                } else {
                    if (y < -500.0f) {
                        (*ext->mtx)[1][3] = y - -1000.0f;
                    }
                }
            }
            if (irand(150) == 0) {
                if (irand(101) <= 80) {
                    ext->dir = angleABf(camX, 0.0f, x, z);
                } else {
                    ext->dir = angleABf(camX, 0.0f, x - 2.0f * (x - camX), z);
                }
            }
        }
        yuunpc_pose_sub(ext, npc);
    }
}

void ext_yuunpc_minigame3_init(void) {
    f32 ytbl[4] = {200.0f, 160.0f, 120.0f, 80.0f};
    f32 ztbl[8] = {-7.0f, -1.0f, 5.0f, -5.0f, 1.0f, 7.0f, -3.0f, 3.0f};
    s32 i;
    ExtdrvPose* ext;
    YuuNpc* npc;

    yuunpc_wp = _mapAlloc(sizeof(YuuNpcWork));
    memset(yuunpc_wp, 0, sizeof(YuuNpcWork));
    yuunpc_wp->numNpc = extGetPoseNum();
    yuunpc_wp->npcs = _mapAlloc(yuunpc_wp->numNpc * sizeof(YuuNpc));
    memset(yuunpc_wp->npcs, 0, yuunpc_wp->numNpc * sizeof(YuuNpc));
    yuunpc_pose_init((YuuNpcData*)ext_yuunpc_minigame3_data);
    ext = extGetPosePtr();
    npc = yuunpc_wp->npcs;
    for (i = 0; i < yuunpc_wp->numNpc; i++, ext++, npc++) {
        s32 row;
        f32 z;
        s16 id;

        npc->poseId = yuunpc_wp->numPoses;
        row = i / 4;
        npc->id = i;
        npc->flags |= 1;
        ext->flags |= 7;
        PSMTXIdentity(*ext->mtx);
        z = 30.0f + (60.0f * (i % 4) + -750.0f);
        (*ext->mtx)[0][3] = 13.333333f * row - 600.0f;
        (*ext->mtx)[1][3] = ytbl[(s32)((z - -750.0f) / 60.0f)];
        (*ext->mtx)[2][3] = z + ztbl[row % 8];
        ext->dir = irand(2) * 180 + 90;
        npc->animSpeed = 1.0f;
        switch (irand(1000) % 4) {
            case 0:
                id = yuunpc_pose_check("c_kinopio", "KNP_S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
            case 1:
                id = yuunpc_pose_check("c_kinopiko", "KNF_S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
            case 2:
                id = yuunpc_pose_check("c_roten", "S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
            case 3:
                id = yuunpc_pose_check("c_jugemu", "JGM_S_1");
                npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                break;
        }
        pose_change_inl(npc, npc->poses[0], 1.0f, rand() / 32767.0f);
    }
}

void yuunpc_pose_minigame3_main(void) {
    s32 i;
    ExtdrvPose* ext;
    f32 right;
    f32 camX = camGetPtr(CAMERA_3D)->cameraAt.x;
    YuuNpc* npc;
    f32 x, y, z;

    ext = extGetPosePtr();
    npc = yuunpc_wp->npcs;
    right = 600.0f + camX;

    for (i = 0; i < yuunpc_wp->numNpc; i++, ext++, npc++) {
        if (npc->flags & 1) {
            x = (*ext->mtx)[0][3];
            if (x > camX) {
                while (TRUE) {
                    if (!(x > right)) break;
                    x -= 1200.0f;
                    (*ext->mtx)[0][3] = x;
                }
            } else {
                while (TRUE) {
                    if (!(x < camX - 600.0f)) break;
                    x += 1200.0f;
                    (*ext->mtx)[0][3] = x;
                }
            }
            z = (*ext->mtx)[2][3];
            if (z < -570.0f) {
                y = (*ext->mtx)[1][3];
                if ((x >= -1700.0f && x <= -1600.0f) || (x >= -50.0f && x <= 50.0f) || (x >= 1600.0f && x <= 1700.0f)) {
                    if (y > -500.0f) {
                        y += -1000.0f;
                        (*ext->mtx)[1][3] = y;
                    }
                } else {
                    if (y < -500.0f) {
                        y -= -1000.0f;
                        (*ext->mtx)[1][3] = y;
                    }
                }
            }
            if (irand(180) == 0) {
                if (irand(101) <= 80) {
                    ext->dir = angleABf(camX, 0.0f, x, z);
                } else {
                    ext->dir = angleABf(camX, 0.0f, x - 2.0f * (x - camX), z);
                }
            }
        }
        yuunpc_pose_sub(ext, npc);
    }
}

static inline s32 yuunpc_hit_get(f32 x, const f32* zp) {
    s32 out = 0;

    if (x < -1800.0f || x >= 1800.0f || *zp < -150.0f || *zp >= 150.0f) {
        out = 1;
    }
    if (out) {
        return -500;
    }
    return yuunpc_wp->hit[(s32)((*zp - -150.0f) / 50.0f) * 72 + (s32)((x - -1800.0f) / 50.0f)];
}

s32 hitcheck(YuuNpc* npc) {
    Vec v;
    Vec p;
    f32 x, z;
    f32 h;

    if (npc->speed > 0.0f) {
        PSVECScale(&npc->dir, &v, npc->speed);
    } else {
        v = (Vec){0.0f, 0.0f, 0.0f};
    }
    v.z += npc->zofs;
    PSVECAdd(&npc->pos, &v, &p);
    h = yuunpc_hit_get(p.x, &p.z);
    if (npc->pos.y > h) {
        npc->pos = p;
        return -1;
    } else if (npc->pos.y < h) {
        if (v.x > 0.0f) {
            x = (s32)((3600.0f + p.x) / 50.0f) * 50.0f - 3600.0f - 0.001f;
            if (npc->pos.x > x) {
                x = npc->pos.x;
            } else if (npc->pos.y == yuunpc_hit_get(x, &p.z)) {
                npc->pos.x = x;
                npc->pos.z = p.z;
                return 0;
            }
        } else if (v.x < 0.0f) {
            x = ((s32)((3600.0f + p.x) / 50.0f) + 1) * 50.0f - 3600.0f;
            if (npc->pos.x < x) {
                x = npc->pos.x;
            } else if (npc->pos.y == yuunpc_hit_get(x, &p.z)) {
                npc->pos.x = x;
                npc->pos.z = p.z;
                return 0;
            }
        }
        if (v.z > 0.0f) {
            z = (s32)((300.0f + p.z) / 50.0f) * 50.0f - 300.0f - 0.001f;
            if (npc->pos.z > z) {
                z = npc->pos.z;
            } else if (npc->pos.y == yuunpc_hit_get(p.x, &z)) {
                npc->pos.x = p.x;
                npc->pos.z = z;
                return 0;
            }
        } else if (v.z < 0.0f) {
            z = ((s32)((300.0f + p.z) / 50.0f) + 1) * 50.0f - 300.0f;
            if (npc->pos.z < z) {
                z = npc->pos.z;
            } else if (npc->pos.y == yuunpc_hit_get(p.x, &z)) {
                npc->pos.x = p.x;
                npc->pos.z = z;
                return 0;
            }
        }
        if (npc->pos.y == yuunpc_hit_get(x, &z)) {
            npc->pos.x = x;
            npc->pos.z = z;
            return 0;
        }
    } else {
        npc->pos = p;
    }
    return 0;
}

#pragma dont_inline on
s32 fallcheck(YuuNpc* npc) {
    s32 out = 0;
    s32 h;
    f32 y;

    npc->pos.y += npc->vy;
    if (npc->pos.x < -1800.0f || npc->pos.x >= 1800.0f || npc->pos.z < -150.0f || npc->pos.z >= 150.0f) {
        out = 1;
    }
    if (out) {
        h = -500;
    } else {
        h = yuunpc_wp->hit[(s32)((npc->pos.z - -150.0f) / 50.0f) * 72 + (s32)((npc->pos.x - -1800.0f) / 50.0f)];
    }
    y = h;
    if (npc->pos.y <= y) {
        npc->pos.y = y;
        npc->vy = 0.0f;
        return -1;
    }
    npc->vy -= 0.53333336f;
    if (npc->vy < -16.666666f) {
        npc->vy = -16.666666f;
    }
    return 0;
}
#pragma dont_inline reset

s32 goalincheck(YuuNpc* npc) {
    f32 r = npc->speed;
    f32 dz = npc->pos.z - npc->dest.z;
    f32 dx = npc->pos.x - npc->dest.x;

    if (dx * dx + dz * dz <= r * r) {
        if (npc->dest.x == npc->goal.x && npc->dest.z == npc->goal.z) {
            npc->pos.x = npc->dest.x;
            npc->pos.z = npc->dest.z;
            return -1;
        }
        npc->dest = npc->goal;
    }
    return 0;
}

void errper_set_rtn(YuuNpc* npc) {
    switch (npc->state) {
        case 0:
        case 2:
        case 4:
            npc->field_0x8 = -0.25f + 2.0f * (0.25f * (rand() / 32767.0f));
            break;
        case 1:
        case 3:
        case 5:
            npc->field_0x8 = -0.25f + 2.0f * (0.25f * (rand() / 32767.0f));
            break;
        case -1:
        case 6:
            npc->field_0x8 = -0.25f + 2.0f * (0.25f * (rand() / 32767.0f));
            break;
        default:
            npc->field_0x8 = -0.1f + 2.0f * (0.1f * (rand() / 32767.0f));
            break;
    }
}

void yuunpc_wp_flag_on(u16 flag) {
    yuunpc_wp->flags |= flag;
}

void yuunpc_wp_flag_off(u16 flag) {
    yuunpc_wp->flags &= ~flag;
}

u16 yuunpc_wp_flag_check(u16 flag) {
    return yuunpc_wp->flags & flag;
}

s32 yuunpc_juni(Vec* pos) {
    f32 x = pos->x;
    s32 n = 0;
    s32 i;
    YuuNpc* npc = yuunpc_wp->npcs;

    for (i = 0; i < yuunpc_wp->numNpc; i++, npc++) {
        if ((npc->flags & 2) && !(npc->flags & 0x40) && x < npc->pos.x) {
            n++;
        }
    }
    return n;
}

void ext_yuunpc_minigame4_init_sub(void) {
    f32 ytbl[4] = {45.0f, 5.0f, -34.0f, -74.0f};
    f32 ztbl[8] = {-7.0f, -1.0f, 5.0f, -5.0f, 1.0f, 7.0f, -3.0f, 3.0f};
    s32 cnt[9] = {0, 0, 0, 0, 1, 1, 1, 2, 2};
    Vec p0;
    Vec p1;
    Vec p2;
    s32 i;
    ExtdrvPose* ext;
    YuuNpc* npc;
    s32 j;
    s32 n;
    s32 k;
    s32 flag;
    s32 s;
    s16 v;
    s32 type;

    npc = yuunpc_wp->npcs;
    memset(yuunpc_wp, 0, sizeof(YuuNpcWork));
    yuunpc_wp->numNpc = extGetPoseNum();
    yuunpc_wp->npcs = npc;
    memset(yuunpc_wp->npcs, 0, yuunpc_wp->numNpc * sizeof(YuuNpc));
    yuunpc_pose_init((YuuNpcData*)ext_yuunpc_minigame4_data);
    yuunpc_set_jump_pose("c_kinopio", "KNP_R_1", 5.0f);
    yuunpc_set_jump_pose("c_bom_n", "R_1", 5.0f);
    yuunpc_set_jump_pose("c_tyusan", "CHU_R_1", 4.0f);
    yuunpc_set_jump_pose("c_luigi", "R_1", 4.0f);
    yuunpc_set_jump_pose("c_nokorin", "R_1", 5.0f);
    yuunpc_set_jump_pose("c_joho_a", "R_1", 17.0f);
    yuunpc_maphit_init();
    yuunpc_wp->flags |= 1;
    ext = extGetPosePtr();
    yuunpc_wp->slot[0] = yuunpc_wp->slot[1] = yuunpc_wp->slot[2] = -1;
    p0 = (Vec){0.0f, -1000.0f, 0.0f};
    npcNameToPtr("yuusp0")->position = p0;
    p1 = (Vec){0.0f, -1000.0f, 0.0f};
    npcNameToPtr("yuusp1")->position = p1;
    p2 = (Vec){0.0f, -1000.0f, 0.0f};
    npcNameToPtr("yuusp2")->position = p2;
    n = cnt[irand(9)];
    if (irand(10) == 0) {
        k = irand(3);
        if (irand(2) == 0) {
            flag = 0;
        } else {
            flag = 1;
        }
    } else {
        k = -1;
        flag = 1;
    }
    if (flag) {
        for (j = 0; j < n; j++) {
            do {
                s = irand(3);
            } while (yuunpc_wp->slot[s] != -1);
            do {
                v = irand(700) + 200;
            } while (yuunpc_wp->slot[(s + 1) % 3] == v || yuunpc_wp->slot[(s + 2) % 3] == v);
            yuunpc_wp->slot[s] = v;
        }
    }

    npc = yuunpc_wp->npcs;
    for (i = 0; i < yuunpc_wp->numNpc; i++, ext++, npc++) {
        npc->poseId = yuunpc_wp->numPoses;
        npc->id = i;
        if (i < 200) {
            s32 row;
            f32 z;
            s16 id;

            row = i / 4;
            npc->flags |= 1;
            ext->flags |= 7;
            PSMTXIdentity(*ext->mtx);
            z = 30.0f + (60.0f * (i % 4) + -750.0f);
            (*ext->mtx)[0][3] = 20.0f * row - 500.0f;
            (*ext->mtx)[1][3] = ytbl[(s32)((z - -750.0f) / 60.0f)];
            (*ext->mtx)[2][3] = z + ztbl[row % 8];
            ext->dir = irand(2) * 180 + 90;
            npc->animSpeed = 1.0f;
            switch (irand(1000) % 4) {
                case 0:
                    id = yuunpc_pose_check("c_kinopio", "KNP_S_1");
                    npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                    break;
                case 1:
                    id = yuunpc_pose_check("c_kinopiko", "KNF_S_1");
                    npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                    break;
                case 2:
                    id = yuunpc_pose_check("c_roten", "S_1");
                    npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                    break;
                case 3:
                    id = yuunpc_pose_check("c_jugemu", "JGM_S_1");
                    npc->poses[0] = npc->poses[1] = npc->poses[2] = npc->poses[3] = id;
                    break;
            }
            pose_change_inl(npc, npc->poses[0], 1.0f, rand() / 32767.0f);
        } else {
            s32 cols;
            s32 row;
            s32 col;
            s32 m;
            f32 step;
            s32 j2;

            npc->flags |= 2;
            m = i - 200;
            cols = sqrtf(700.0f);
            row = m / cols;
            col = m % cols;
            for (j2 = 0; j2 < 3; j2++) {
                if (yuunpc_wp->slot[j2] == i) {
                    switch (j2) {
                        case 0:
                            npc->flags |= 0x100;
                            break;
                        case 1:
                            npc->flags |= 0x200;
                            break;
                        case 2:
                            npc->flags |= 0x300;
                            break;
                    }
                    break;
                }
            }
            switch (npc->flags & 0x300) {
                case 0:
                    ext->flags |= 5;
                    break;
                case 0x100:
                case 0x200:
                case 0x300:
                    break;
            }
            PSMTXIdentity(*ext->mtx);
            step = 240.0f / (cols - 1);
            (*ext->mtx)[0][3] = -(f32)row * step + -1520.0f - step / 2.0f * (col & 1);
            (*ext->mtx)[1][3] = 0.0f;
            (*ext->mtx)[2][3] = col * step - 120.0f + ztbl[row % 8] + 10.0f * ((npc->id % 101 - 50) / 50.0f);
            npc->dest = (Vec){3300.0f + (*ext->mtx)[0][3], (*ext->mtx)[1][3], (*ext->mtx)[2][3]};
            npc->goal = npc->dest;
            ext->dir = 270.0f;
            npc->animSpeed = 1.0f;
            npc->pos = (Vec){(*ext->mtx)[0][3], (*ext->mtx)[1][3], (*ext->mtx)[2][3]};
            switch (npc->flags & 0x300) {
                case 0x100: {
                    f32 rot;

                    npc->npc = npcNameToPtr("yuusp0");
                    npc->npc->position = npc->pos;
                    rot = 3600.0f + (90.0f - ext->dir);
                    npc->npc->rotation.y = rot - (s32)(rot / 360.0f) * 360;
                    animPosePeraOff(npc->npc->poseId);
                    npc->state = 7;
                    npc->poses[0] = yuunpc_pose_check("c_luigi", "S_1");
                    npc->poses[1] = yuunpc_pose_check("c_luigi", "W_1");
                    npc->poses[2] = yuunpc_pose_check("c_luigi", "R_1");
                    npc->poses[3] = yuunpc_pose_check("c_luigi", "R_1");
                    break;
                }
                case 0x200: {
                    f32 rot;

                    npc->state = 6;
                    npc->npc = npcNameToPtr("yuusp1");
                    npc->npc->position = npc->pos;
                    rot = 3600.0f + (90.0f - ext->dir);
                    npc->npc->rotation.y = rot - (s32)(rot / 360.0f) * 360;
                    animPosePeraOff(npc->npc->poseId);
                    npc->poses[0] = yuunpc_pose_check("c_nokorin", "S_1");
                    npc->poses[1] = yuunpc_pose_check("c_nokorin", "W_1");
                    npc->poses[2] = yuunpc_pose_check("c_nokorin", "R_1");
                    npc->poses[3] = yuunpc_pose_check("c_nokorin", "R_1");
                    break;
                }
                case 0x300: {
                    f32 rot;

                    npc->npc = npcNameToPtr("yuusp2");
                    npc->npc->position = npc->pos;
                    rot = 3600.0f + (90.0f - ext->dir);
                    npc->npc->rotation.y = rot - (s32)(rot / 360.0f) * 360;
                    animPosePeraOff(npc->npc->poseId);
                    npc->state = -1;
                    npc->poses[0] = yuunpc_pose_check("c_joho_a", "S_1");
                    npc->poses[1] = yuunpc_pose_check("c_joho_a", "W_1");
                    npc->poses[2] = yuunpc_pose_check("c_joho_a", "R_1");
                    npc->poses[3] = yuunpc_pose_check("c_joho_a", "R_1");
                    break;
                }
                case 0:
                    if (k < 0) {
                        type = irand(3);
                    } else {
                        type = k;
                        if (flag && irand(35) == 0) {
                            do {
                                type = irand(3);
                            } while (type == k);
                        }
                    }
                    switch (type) {
                        case 0:
                            npc->state = irand(6);
                            npc->poses[0] = yuunpc_pose_check("c_kinopio", "KNP_S_1");
                            npc->poses[1] = yuunpc_pose_check("c_kinopio", "KNP_R_1");
                            npc->poses[2] = yuunpc_pose_check("c_kinopio", "KNP_R_1");
                            npc->poses[3] = yuunpc_pose_check("c_kinopio", "KNP_R_1");
                            break;
                        case 1:
                            npc->state = irand(6);
                            npc->poses[0] = yuunpc_pose_check("c_bom_n", "S_1");
                            npc->poses[1] = yuunpc_pose_check("c_bom_n", "R_1");
                            npc->poses[2] = yuunpc_pose_check("c_bom_n", "R_1");
                            npc->poses[3] = yuunpc_pose_check("c_bom_n", "R_1");
                            break;
                        case 2:
                            npc->state = irand(6);
                            npc->poses[0] = yuunpc_pose_check("c_tyusan", "CHU_S_1");
                            npc->poses[1] = yuunpc_pose_check("c_tyusan", "CHU_R_1");
                            npc->poses[2] = yuunpc_pose_check("c_tyusan", "CHU_R_1");
                            npc->poses[3] = yuunpc_pose_check("c_tyusan", "CHU_R_1");
                            break;
                    }
                    break;
            }
            npc->field_0x4c = 0;
            pose_change_inl(npc, npc->poses[0], 1.0f, rand() / 32767.0f);
            npc->flags |= 0x20;
            errper_set_rtn(npc);
        }
    }
}

void ext_yuunpc_minigame4_init(void) {
    yuunpc_wp = _mapAlloc(sizeof(YuuNpcWork));
    memset(yuunpc_wp, 0, sizeof(YuuNpcWork));
    yuunpc_wp->numNpc = extGetPoseNum();
    yuunpc_wp->npcs = _mapAlloc(yuunpc_wp->numNpc * sizeof(YuuNpc));
    memset(yuunpc_wp->npcs, 0, yuunpc_wp->numNpc * sizeof(YuuNpc));
    ext_yuunpc_minigame4_init_sub();
}

static inline s32 yuunpc_hitY_get(f32 x) {
    s32 out = 0;

    if (x < -1800.0f || x >= 1800.0f) {
        out = 1;
    }
    if (out) {
        return 0;
    }
    return yuunpc_wp->hitY[(s32)((x - -1800.0f) / 50.0f)];
}

#define HG(x, zp) (((x) < -1800.0f || (x) >= 1800.0f || *(zp) < -150.0f || *(zp) >= 150.0f) ? -500 : yuunpc_wp->hit[(s32)((*(zp) - -150.0f) / 50.0f) * 72 + (s32)(((x) - -1800.0f) / 50.0f)])
#define yuunpc_hit_ofs(npc, ofs) ((((npc)->pos.x + (ofs)->x) < -1800.0f || ((npc)->pos.x + (ofs)->x) >= 1800.0f || ((npc)->pos.z + (ofs)->z) < -150.0f || ((npc)->pos.z + (ofs)->z) >= 150.0f) ? -500 : yuunpc_wp->hit[(s32)((((npc)->pos.z + (ofs)->z) - -150.0f) / 50.0f) * 72 + (s32)((((npc)->pos.x + (ofs)->x) - -1800.0f) / 50.0f)])

#define yuunpc_snap(v) (((s32)((3600.0f + (v)) / 50.0f) + 1) * 50.0f - 3600.0f)

static inline f32 yuunpc_rnd10(YuuNpc* npc) /* */ {
    return 10.0f * ((npc->id % 101 - 50) / 50.0f);
}

#define YUU_STOP(npc)                                                              \
    do {                                                                           \
        (npc)->field_0x4c = 0;                                                     \
        yuunpc_pose_change((npc), (npc)->poses[0], 1.0f, rand() / 32767.0f);       \
        (npc)->speed = 0.0f;                                                       \
    } while (0)

#define YUU_JUMP(npc)                                                                                                    \
    do {                                                                                                                 \
        (npc)->field_0x4c = 4;                                                                                           \
        yuunpc_pose_change((npc), (npc)->poses[3], 0.0f, yuunpc_wp->pose[(npc)->poses[3]].jump / yuunpc_wp->pose[(npc)->poses[3]].frames); \
        (npc)->vy = 6.5f;                                                                                                \
        (npc)->dir.x = 1.0f;                                                                                             \
        (npc)->dir.z = 0.0f;                                                                                             \
        if (!(c26 & 0x1f)) {                                                                                             \
            psndSFXOn_3D((const char*)0x8e0, &(npc)->pos);                                                               \
        }                                                                                                                \
        c26++;                                                                                                           \
    } while (0)

#define YUU_RUN(npc)                                                               \
    do {                                                                           \
        (npc)->field_0x4c = 2;                                                     \
        yuunpc_pose_change((npc), (npc)->poses[2], 1.0f, rand() / 32767.0f);       \
        if ((npc)->flags & 0x18) {                                                 \
            (npc)->speed = 1.0f;                                                   \
        } else {                                                                   \
            (npc)->speed = 2.25f;                                                  \
        }                                                                          \
    } while (0)

#define YUU_WALK(npc)                                                                                         \
    do {                                                                                                      \
        (npc)->field_0x4c = 1;                                                                                \
        yuunpc_pose_change((npc), (npc)->poses[1], yuunpc_wp->pose[(npc)->poses[1]].field_0x30, rand() / 32767.0f); \
        if ((npc)->flags & 0x18) {                                                                            \
            (npc)->speed = 0.5f;                                                                              \
        } else {                                                                                              \
            (npc)->speed = 1.0f;                                                                              \
        }                                                                                                     \
    } while (0)

void yuunpc_pose_minigame4_main(void) {
    s32 i;
    ExtdrvPose* ext;
    f32 right;
    f32 camX = camGetPtr(CAMERA_3D)->cameraAt.x;
    f32 y, z, x;
    YuuNpc* npc;
    s32 c28;
    s32 c27;
    s32 c26;
    SlitWork* mg;
    s32 kind;
    s16 h;
    s16 k;
    f32 px;
    f32 off;
    f32 w;
    f32 d;
    f32 dz;
    f32 ha;
    f32 hb;
    f32 hc;
    s32 mode;
    s32 cnt;
    s32 n;
    u32 t;

    c28 = c27 = c26 = 0;
    ext = extGetPosePtr();
    npc = yuunpc_wp->npcs;
    right = 500.0f + camX;

    for (i = 0; i < yuunpc_wp->numNpc; i++, ext++, npc++) {
        if (npc->flags & 1) {
            x = (*ext->mtx)[0][3];
            if (x > camX) {
                while (TRUE) {
                    if (!(x > right)) break;
                    x -= 1000.0f;
                    (*ext->mtx)[0][3] = x;
                }
            } else {
                while (TRUE) {
                    if (!(x < camX - 500.0f)) break;
                    x += 1000.0f;
                    (*ext->mtx)[0][3] = x;
                }
            }
            z = (*ext->mtx)[2][3];
            if (z < -570.0f) {
                y = (*ext->mtx)[1][3];
                if ((x >= -1700.0f && x <= -1600.0f) || (x >= -50.0f && x <= 50.0f) || (x >= 1600.0f && x <= 1700.0f)) {
                    if (y > -500.0f) {
                        y += -1000.0f;
                        (*ext->mtx)[1][3] = y;
                    }
                } else {
                    if (y < -500.0f) {
                        y -= -1000.0f;
                        (*ext->mtx)[1][3] = y;
                    }
                }
            }
            if (irand(100) == 0) {
                if (irand(101) <= 80) {
                    ext->dir = angleABf(camX, 0.0f, x, z);
                } else {
                    ext->dir = angleABf(camX, 0.0f, x - 2.0f * (x - camX), z);
                }
            }
        }
        if (npc->flags & 2) {
            if (yuunpc_yuuminigame4_ptr == NULL || (npc->flags & 0x80)) {
                continue;
            }
            mg = yuunpc_yuuminigame4_ptr->data;
            switch (npc->flags & 0x18) {
            case 0:
                break;
            case 8:
                if (npc->field_0x50 == 0) {
                    npc->flags |= 0x18;
                }
                break;
            case 0x10:
                if (npc->field_0x50 == 0) {
                    npc->flags &= ~0x18;
                    npc->field_0x44 = 0.0f;
                    npc->field_0x4c = 0;
                    yuunpc_pose_change(npc, npc->poses[0], 1.0f, rand() / 32767.0f);
                    npc->speed = 0.0f;
                }
                break;
            case 0x18:
                break;
            }
            switch (npc->field_0x4c) {
            case 0:
            case 1:
            case 2:
                if (!(npc->flags & 0x20)) {
                    npc->field_0x4c = 3;
                    yuunpc_pose_change(npc, npc->poses[3], 0.0f, yuunpc_wp->pose[npc->poses[3]].jump / yuunpc_wp->pose[npc->poses[3]].frames);
                    npc->vy = 0.0f;
                }
                break;
            case 3:
            case 4:
                if (npc->flags & 0x20) {
                    npc->field_0x4c = 0;
                    yuunpc_pose_change(npc, npc->poses[0], 1.0f, rand() / 32767.0f);
                    npc->speed = 0.0f;
                }
                break;
            }
            if (npc->field_0x4c != 3 && npc->field_0x4c != 4) {
                px = npc->pos.x;
                kind = 1;
                h = yuunpc_hitY_get(px);
                k = h;
                switch (h) {
                case 0:
                case 1:
                    npc->dest = npc->goal;
                    if (npc->pos.x < 1500.0f) {
                        f32 dd = npc->pos.z - npc->dest.z;
                        if (dd < 0.0f) {
                            dd = -dd;
                        }
                        if (dd > 16.666666f) {
                            dd = 300.0f * (1.0f - dd / 300.0f);
                            npc->dest.x = npc->pos.x + dd / 2.0f;
                        }
                    }
                    break;
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                    kind = 2;
                    k -= 22;
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    if (k == 6) {
                        off = 0.0f;
                    } else {
                        off = (3 - (k - 2)) * 50.0f;
                    }
                    npc->dest.x = yuunpc_snap(px + off);
                    {
                        f32 sp = ((npc->id % 101 - 50) / 50.0f) * (50.0f * (kind - 1) / 2.0f + 10.0f);
                        npc->dest.z = npc->pos.z >= 0.0f ? 150.0f - 50.0f * kind / 2.0f + sp : -150.0f + 50.0f * kind / 2.0f + sp;
                    }
                    npc->dest.y = npc->pos.y;
                    break;
                case 7:
                case 8:
                case 9:
                case 10:
                    off = (3 - (h - 7)) * 50.0f;
                    npc->dest.x = yuunpc_snap(px + off);
                    npc->dest.z = 0.65f * npc->goal.z;
                    npc->dest.y = npc->pos.y;
                    break;
                case 11:
                    npc->dest.x = yuunpc_snap(50.0f + px);
                    npc->dest.z = npc->pos.z;
                    npc->dest.y = npc->pos.y;
                    break;
                case 12:
                case 13:
                case 14:
                case 15:
                    if ((u16)npc->id & 3) {
                        off = (3 - (h - 12)) * 50.0f; px += off;
                        npc->dest.x = yuunpc_snap(px);
                        npc->dest.z = -(75.0f + yuunpc_rnd10(npc));
                        npc->dest.y = npc->pos.y;
                    } else {
                        npc->dest.x = yuunpc_snap(50.0f + px);
                        npc->dest.z = npc->pos.z;
                        npc->dest.y = npc->pos.y;
                    }
                    break;
                case 16:
                case 17:
                case 18:
                case 19:
                    if ((u16)npc->id & 3) {
                        off = (3 - (h - 16)) * 50.0f; px += off;
                        npc->dest.x = yuunpc_snap(px);
                        npc->dest.z = -25.0f + yuunpc_rnd10(npc);
                        npc->dest.y = npc->pos.y;
                    } else {
                        npc->dest.x = yuunpc_snap(50.0f + px);
                        npc->dest.z = npc->pos.z;
                        npc->dest.y = npc->pos.y;
                    }
                    break;
                case 20:
                case 21:
                case 22:
                case 23:
                    if ((u16)npc->id & 3) {
                        off = (3 - (h - 20)) * 50.0f; px += off;
                        npc->dest.x = yuunpc_snap(px);
                        npc->dest.z = 75.0f + yuunpc_rnd10(npc);
                        npc->dest.y = npc->pos.y;
                    } else {
                        npc->dest.x = yuunpc_snap(50.0f + px);
                        npc->dest.z = npc->pos.z;
                        npc->dest.y = npc->pos.y;
                    }
                    break;
                }
                PSVECSubtract(&npc->dest, &npc->pos, &npc->dir);
                npc->dir.y = 0.0f;
                PSVECNormalize(&npc->dir, &npc->dir);
                w = 150.0f * npc->field_0x8;
                mode = mg->state;
                switch (mode) {
                default:
                    d = 0.0f;
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                    switch (h) {
                    case 0:
                    case 2:
                    case 3:
                    case 4:
                    case 7:
                    case 8:
                    case 9:
                    case 12:
                    case 13:
                    case 14:
                    case 16:
                    case 17:
                    case 18:
                    case 20:
                    case 21:
                    case 22:
                    case 24:
                    case 25:
                    case 26:
                        if (npc->pos.z >= -150.0f && npc->pos.z < 150.0f + w) {
                            d = 150.0f + w - npc->pos.z;
                        } else {
                            d = 0.0f;
                        }
                        break;
                    case 1:
                    case 10:
                        d = 0.0f;
                        break;
                    case 5:
                    case 6:
                    case 27:
                    case 28:
                        z = npc->pos.z;
                        if (z >= -150.0f && z < 150.0f) {
                            f32 tt = 50.0f * kind;
                            f32 a = -150.0f + tt + w;
                            if (z >= a && z < 150.0f - tt) {
                                d = 0.0f;
                            } else if (z > 0.0f) {
                                d = 150.0f + w - z;
                            } else {
                                d = a - z;
                            }
                        } else {
                            d = 0.0f;
                        }
                        break;
                    }
                    break;
                case 5:
                case 6:
                case 7:
                case 8:
                    switch (h) {
                    case 0:
                    case 2:
                    case 3:
                    case 4:
                    case 7:
                    case 8:
                    case 9:
                    case 12:
                    case 13:
                    case 14:
                    case 16:
                    case 17:
                    case 18:
                    case 20:
                    case 21:
                    case 22:
                    case 24:
                    case 25:
                    case 26:
                        if (npc->pos.z >= -150.0f + w && npc->pos.z < 150.0f) {
                            d = -(-150.0f + w) + npc->pos.z;
                        } else {
                            d = 0.0f;
                        }
                        break;
                    case 1:
                    case 10:
                        d = 0.0f;
                        break;
                    case 5:
                    case 6:
                    case 27:
                    case 28:
                        {
                                                if (npc->pos.z >= -150.0f + w && npc->pos.z < 150.0f) {
                            f32 tt = 50.0f * kind;
                            if (npc->pos.z >= -150.0f + tt && npc->pos.z < 150.0f - tt + w) {
                                d = 0.0f;
                            } else if (npc->pos.z > 0.0f) {
                                d = npc->pos.z - (150.0f - tt + w);
                            } else {
                                d = -(-150.0f + w) + npc->pos.z;
                            }
                        } else {
                            d = 0.0f;
                        }
                        }
                        break;
                    }
                    break;
                }
                d -= 17.0f;
                if (d < 0.0f) {
                    d = 0.0f;
                }
                cnt = d / 0.0f;
                switch (npc->flags & 0x18) {
                case 0:
                    if (mg->state == 0) {
                        n = 1;
                    } else {
                        switch (mode) {
                        case 1:
                        case 5:
                            if (cnt >= 60) {
                                cnt += 60;
                            } else {
                                cnt += cnt;
                            }
                            break;
                        case 2:
                        case 6:
                            if (cnt >= mg->timer) {
                                cnt += mg->timer;
                            } else {
                                cnt += cnt;
                            }
                            break;
                        }
                        if (cnt < 30) {
                            n = 0;
                        } else {
                            n = d / 2.9166667f;
                        }
                    }
                    if (!(yuunpc_wp->flags & 1) && npc->goal.x > npc->pos.x && n <= 0) {
                        npc->flags |= 8;
                        npc->field_0x50 = 30;
                        npc->field_0x44 = ext->dir;
                        npc->field_0x4c = 0;
                        yuunpc_pose_change(npc, npc->poses[0], 1.0f, rand() / 32767.0f);
                        npc->speed = 0.0f;
                        errper_set_rtn(npc);
                    }
                    break;
                case 8:
                case 0x10:
                    break;
                case 0x18:
                    if (cnt < 30) {
                        n = 0;
                    } else {
                        cnt = 30;
                        n = d / 2.9166667f;
                    }
                    if (npc->goal.x == npc->pos.x || mg->state == 0 || ((mg->state == 4 || mg->state == 8) && cnt + n >= mg->timer)) {
                        npc->flags &= ~8;
                        npc->field_0x50 = 30;
                        errper_set_rtn(npc);
                    }
                    break;
                }
            }
            switch (npc->flags & 0x18) {
            case 0:
                break;
            case 8:
                npc->field_0x50--;
                ext->dir = npc->field_0x44 + -450.0f * (1.0f - npc->field_0x50 / 30.0f);
                ext->dir = (3600.0f + ext->dir) - (s32)((3600.0f + ext->dir) / 360.0f) * 360;
                break;
            case 0x18:
                t = gp->retraceCount + (u16)npc->id;
                if (t & 0x20) {
                    n = 15 - (t & 0x1f);
                } else {
                    n = (t & 0x1f) - 15;
                }
                ext->dir = -450.0f + npc->field_0x44 + 2.0f * n;
                ext->dir = (3600.0f + ext->dir) - (s32)((3600.0f + ext->dir) / 360.0f) * 360;
                break;
            case 0x10:
                npc->field_0x50--;
                ext->dir = -180.0f + (-450.0f + npc->field_0x44) + -450.0f * (1.0f - npc->field_0x50 / 30.0f);
                ext->dir = (3600.0f + ext->dir) - (s32)((3600.0f + ext->dir) / 360.0f) * 360;
                break;
            }
            npc->zofs = 0.0f;
            if (npc->pos.x < 1500.0f) {
                dz = 0.0f;
                mode = mg->state;
                switch (mode) {
                case 2:
                case 3:
                case 4:
                    switch (npc->flags & 0x18) {
                    case 0x18:
                        dz = 0.0f;
                        break;
                    case 8:
                    case 0x10:
                        dz = 0.0f;
                        break;
                    case 0:
                        dz = 2.9166667f;
                        break;
                    }
                    break;
                case 6:
                case 7:
                case 8:
                    switch (npc->flags & 0x18) {
                    case 0x18:
                        dz = -0.0f;
                        break;
                    case 8:
                    case 0x10:
                        dz = -0.0f;
                        break;
                    case 0:
                        dz = -2.9166667f;
                        break;
                    }
                    break;
                }
                switch (mode) {
                case 2:
                case 6:
                    dz = dz * (60 - mg->timer) / 60.0f;
                    break;
                }
                npc->zofs += dz;
            }
            do {
                kind = 0;
                switch (npc->field_0x4c) {
                case 0:
                    if (goalincheck(npc) == -1) {
                        break;
                    }
                    if (yuunpc_wp->flags & 1) {
                        break;
                    }
                    if (npc->state >= 2 || (npc->state <= 1 && irand(36) == 0)) {
                        px = npc->pos.x;
                        hb = HG(25.0f + px, &npc->pos.z);
                        ha = HG(75.0f + px, &npc->pos.z);
                        if (npc->state <= 3 && irand(2)) {
                            if (hb >= npc->pos.y) {
                                YUU_WALK(npc);
                                kind = 1;
                            }
                        } else if (!(npc->flags & 0x18) || ((npc->flags & 0x18) && (hb >= npc->pos.y || (hb < npc->pos.y && ha < npc->pos.y)))) {
                            YUU_RUN(npc);
                            kind = 1;
                        }
                    }
                    break;
                case 1:
                    if (goalincheck(npc) == -1) {
                        YUU_STOP(npc);
                        break;
                    }
                    if (npc->flags & 0x18) {
                        Vec v1, s1;

                        PSVECSubtract(&npc->dest, &npc->pos, &v1);
                        PSVECNormalize(&v1, &v1);
                        PSVECScale(&v1, &s1, 10.0f);
                        ha = yuunpc_hit_ofs(npc, &s1);
                        PSVECScale(&v1, &s1, 25.0f);
                        hb = yuunpc_hit_ofs(npc, &s1);
                        PSVECScale(&v1, &s1, 75.0f);
                        hc = yuunpc_hit_ofs(npc, &s1);
                        if (ha > npc->pos.y) {
                            YUU_JUMP(npc);
                        } else if (hb < npc->pos.y && hc == npc->pos.y) {
                            YUU_STOP(npc);
                        } else if (irand(60) == 0) {
                            if (npc->state <= 1 && irand(3) == 0) {
                                YUU_STOP(npc);
                            } else if (hb >= npc->pos.y) {
                                YUU_RUN(npc);
                            }
                        }
                    } else {
                        Vec v2, s2;

                        PSVECSubtract(&npc->dest, &npc->pos, &v2);
                        PSVECNormalize(&v2, &v2);
                        PSVECScale(&v2, &s2, 20.0f);
                        ha = yuunpc_hit_ofs(npc, &s2);
                        PSVECScale(&v2, &s2, 25.0f);
                        hb = yuunpc_hit_ofs(npc, &s2);
                        PSVECScale(&v2, &s2, 75.0f);
                        hc = yuunpc_hit_ofs(npc, &s2);
                        if (ha > npc->pos.y) {
                            if (mg->state == 0) {
                                YUU_JUMP(npc);
                            } else {
                                YUU_STOP(npc);
                            }
                        } else if ((hb < npc->pos.y && hc == npc->pos.y) || (npc->state <= 3 && irand(60) == 0)) {
                            if (npc->state <= 1 && irand(3) == 0) {
                                YUU_STOP(npc);
                            } else {
                                YUU_RUN(npc);
                            }
                        }
                    }
                    if (!(c28 & 0x3f)) {
                        t = gp->retraceCount + i;
                        if (!(t & 0xf)) {
                            if (t & 0x10) {
                                psndSFXOn_3D((const char*)0x8de, &npc->pos);
                            } else {
                                psndSFXOn_3D((const char*)0x8df, &npc->pos);
                            }
                        }
                    }
                    c28++;
                    break;
                case 2:
                    if (goalincheck(npc) == -1) {
                        YUU_STOP(npc);
                        break;
                    }
                    if (npc->flags & 0x18) {
                        Vec v3, s3;

                        PSVECSubtract(&npc->dest, &npc->pos, &v3);
                        PSVECNormalize(&v3, &v3);
                        PSVECScale(&v3, &s3, 20.0f);
                        ha = yuunpc_hit_ofs(npc, &s3);
                        PSVECScale(&v3, &s3, 25.0f);
                        hb = yuunpc_hit_ofs(npc, &s3);
                        PSVECScale(&v3, &s3, 75.0f);
                        hc = yuunpc_hit_ofs(npc, &s3);
                        if (ha > npc->pos.y) {
                            YUU_JUMP(npc);
                        } else if (hb < npc->pos.y && hc == npc->pos.y) {
                            YUU_STOP(npc);
                        } else if (npc->state <= 3 && irand(60) == 0) {
                            if (npc->state <= 1 && irand(3) == 0) {
                                YUU_STOP(npc);
                            } else if (hb >= npc->pos.y) {
                                YUU_WALK(npc);
                            }
                        }
                    } else {
                        Vec v4, s4;

                        PSVECSubtract(&npc->dest, &npc->pos, &v4);
                        PSVECNormalize(&v4, &v4);
                        PSVECScale(&v4, &s4, 5.0f);
                        ha = yuunpc_hit_ofs(npc, &s4);
                        PSVECScale(&v4, &s4, 45.0f);
                        hb = yuunpc_hit_ofs(npc, &s4);
                        PSVECScale(&v4, &s4, 55.0f);
                        hc = yuunpc_hit_ofs(npc, &s4);
                        if ((ha < npc->pos.y && hc == npc->pos.y) || hb > npc->pos.y) {
                            if (mg->state == 0) {
                                YUU_JUMP(npc);
                            } else {
                                YUU_STOP(npc);
                            }
                        } else if (npc->state <= 3 && irand(60) == 0) {
                            if (npc->state <= 1 && irand(3) == 0) {
                                YUU_STOP(npc);
                            } else if (yuunpc_hit_get(25.0f + npc->pos.x, &npc->pos.z) >= npc->pos.y) {
                                YUU_WALK(npc);
                            }
                        }
                    }
                    if (!(c27 & 0x3f)) {
                        t = gp->retraceCount + i;
                        if (!(t & 7)) {
                            if (t & 8) {
                                psndSFXOn_3D((const char*)0x8de, &npc->pos);
                            } else {
                                psndSFXOn_3D((const char*)0x8df, &npc->pos);
                            }
                        }
                    }
                    c27++;
                    break;
                case 3:
                case 4:
                    break;
                }
            } while (kind);
            switch (npc->field_0x4c) {
            case 0:
            case 1:
            case 2:
                if (hitcheck(npc) == -1) {
                    npc->flags &= ~0x20;
                } else {
                    npc->flags |= 0x20;
                }
                break;
            case 3:
            case 4:
                hitcheck(npc);
                if (fallcheck(npc) == -1) {
                    npc->flags |= 0x20;
                } else {
                    npc->flags &= ~0x20;
                }
                if (npc->pos.y <= -100.0f) {
                    npc->flags |= 0x40;
                }
                if (npc->pos.y <= -500.0f) {
                    npc->flags |= 0x80;
                    ext->flags &= ~1;
                }
                break;
            }
            (*ext->mtx)[0][3] = npc->pos.x;
            (*ext->mtx)[1][3] = npc->pos.y;
            (*ext->mtx)[2][3] = npc->pos.z;
        }
        yuunpc_pose_sub(ext, npc);
    }
}

void yuunpc_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

void ext_yuunpc_dispent(void) {
    dispEntry(4, 0.0f, 1, yuunpc_disp, NULL);
}

void ext_yuunpc_minigame1_main(void) {
    yuunpc_pose_minigame1_main();
}

void ext_yuunpc_minigame2_main(void) {
    yuunpc_pose_minigame2_main();
}

void ext_yuunpc_minigame3_main(void) {
    yuunpc_pose_minigame3_main();
}

void ext_yuunpc_minigame4_main(void) {
    yuunpc_pose_minigame4_main();
}

EVT_BEGIN(evt_yuunpc_minigame1)
    CALL(evt_npc_entry, STRING("yuunpc0"), STRING("c_kinopiko"))
    CALL(evt_npc_entry, STRING("yuunpc1"), STRING("c_roten"))
    CALL(evt_npc_entry, STRING("yuunpc2"), STRING("c_jugemu"))
    CALL(evt_npc_entry, STRING("yuunpc3"), STRING("c_kinopio"))
    CALL(evt_npc_set_position, STRING("yuunpc0"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc1"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc2"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc3"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc0"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc1"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc2"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc3"), 32)
    CALL(evt_ext_entry, 300, PTR(ext_yuunpc_minigame1_data), PTR(ext_yuunpc_minigame1_init), PTR(ext_yuunpc_minigame1_main), PTR(ext_yuunpc_dispent))
    RETURN
EVT_END()

EVT_BEGIN(evt_yuunpc_minigame2)
    CALL(evt_npc_entry, STRING("yuunpc0"), STRING("c_kinopiko"))
    CALL(evt_npc_entry, STRING("yuunpc1"), STRING("c_roten"))
    CALL(evt_npc_entry, STRING("yuunpc2"), STRING("c_jugemu"))
    CALL(evt_npc_entry, STRING("yuunpc3"), STRING("c_kinopio"))
    CALL(evt_npc_set_position, STRING("yuunpc0"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc1"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc2"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc3"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc0"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc1"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc2"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc3"), 32)
    CALL(evt_ext_entry, 300, PTR(ext_yuunpc_minigame2_data), PTR(ext_yuunpc_minigame2_init), PTR(ext_yuunpc_minigame2_main), PTR(ext_yuunpc_dispent))
    RETURN
EVT_END()

EVT_BEGIN(evt_yuunpc_minigame3)
    CALL(evt_npc_entry, STRING("yuunpc0"), STRING("c_kinopiko"))
    CALL(evt_npc_entry, STRING("yuunpc1"), STRING("c_roten"))
    CALL(evt_npc_entry, STRING("yuunpc2"), STRING("c_jugemu"))
    CALL(evt_npc_entry, STRING("yuunpc3"), STRING("c_kinopio"))
    CALL(evt_npc_set_position, STRING("yuunpc0"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc1"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc2"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc3"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc0"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc1"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc2"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc3"), 32)
    CALL(evt_ext_entry, 360, PTR(ext_yuunpc_minigame3_data), PTR(ext_yuunpc_minigame3_init), PTR(ext_yuunpc_minigame3_main), PTR(ext_yuunpc_dispent))
    RETURN
EVT_END()

EVT_BEGIN(evt_yuunpc_minigame4)
    CALL(evt_npc_entry, STRING("yuunpc0"), STRING("c_kinopiko"))
    CALL(evt_npc_entry, STRING("yuunpc1"), STRING("c_roten"))
    CALL(evt_npc_entry, STRING("yuunpc2"), STRING("c_jugemu"))
    CALL(evt_npc_entry, STRING("yuunpc3"), STRING("c_kinopio"))
    CALL(evt_npc_entry, STRING("yuunpc4"), STRING("c_bom_n"))
    CALL(evt_npc_entry, STRING("yuunpc5"), STRING("c_tyusan"))
    CALL(evt_npc_entry, STRING("yuusp0"), STRING("c_luigi"))
    CALL(evt_npc_entry, STRING("yuusp1"), STRING("c_nokorin"))
    CALL(evt_npc_entry, STRING("yuusp2"), STRING("c_joho_a"))
    CALL(evt_npc_set_position, STRING("yuunpc0"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc1"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc2"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc3"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc4"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuunpc5"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuusp0"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuusp1"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("yuusp2"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc0"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc1"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc2"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc3"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc4"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuunpc5"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuusp0"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuusp1"), 32)
    CALL(evt_npc_flag_onoff, 1, STRING("yuusp2"), 32)
    CALL(evt_ext_entry, 900, PTR(ext_yuunpc_minigame4_data), PTR(ext_yuunpc_minigame4_init), PTR(ext_yuunpc_minigame4_main), PTR(ext_yuunpc_dispent))
    BEGIN_INLINE_EVENT
        CALL(evt_seq_wait, 2)
        CALL(evt_npc_delete, STRING("yuunpc0"))
        CALL(evt_npc_delete, STRING("yuunpc1"))
        CALL(evt_npc_delete, STRING("yuunpc2"))
        CALL(evt_npc_delete, STRING("yuunpc3"))
        CALL(evt_npc_delete, STRING("yuunpc4"))
        CALL(evt_npc_delete, STRING("yuunpc5"))
    END_INLINE_EVENT
    RETURN
EVT_END()
