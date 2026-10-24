#include "eff/eff_fukidashi.h"
#include "driver/camdrv.h"
#include "evt/evt_word.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/icondrv.h"
#include "driver/mapdrv.h"
#include "driver/npcdrv.h"
#include "manager/fontmgr.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mariost.h"
#include "memory.h"
#include "party.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdio.h>
#include "icon_ids.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

extern s32 psndSFXOn_3D();

EffectEntry* effFukidashiEntry(s32 type, s32 number, NpcEntry* npc, f32 x, f32 y, f32 z, f32 size,
                               f32 angle, s32 duration);
void effFukidashiMain(EffectEntry* effect);
void effFukidashiDisp(CameraId cameraId, EffectEntry* effect);

extern u8 moja_model_dl_0_1[], moja_model_dl_0_2[], moja_model_dl_0_3[], moja_model_dl_0_4[],
    moja_model_dl_0_5[], moja_model_dl_0_6[], moja_model_dl_0_7[], moja_model_dl_0_8[],
    moja_model_dl_0_9[], moja_model_dl_0_10[], moja_model_dl_0_11[], moja_model_dl_0_12[],
    moja_model_dl_0_13[], moja_model_dl_0_14[], moja_model_dl_0_15[], moja_model_dl_0_16[],
    moja_model_dl_0_17[], moja_model_dl_0_18[], moja_model_dl_0_19[], moja_model_dl_0_20[];

__declspec(section ".data") f32 eff_fukidashi_negone_one_a[] = { -1.0f, 1.0f };
__declspec(section ".data") static f32 negone_one_374[] = { -1.0f, 1.0f };
__declspec(section ".data") f32 eff_fukidashi_one_negone_a[] = { 1.0f, -1.0f };
__declspec(section ".data") static f32 one_negone_373[] = { 1.0f, -1.0f };
__declspec(section ".data") f32 eff_fukidashi_negone_one_b[] = { -1.0f, 1.0f };
__declspec(section ".data") static f32 negone_one_360[] = { -1.0f, 1.0f };
__declspec(section ".data") f32 eff_fukidashi_one_negone_b[] = { 1.0f, -1.0f };
__declspec(section ".data") static f32 one_negone_359[] = { 1.0f, -1.0f };
s8 anim_data_tenten[] = { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, -1, 0 };
u8 moja_model_vertex_tbl[] = { 0xF6, 0x01, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x57, 0x03, 0x00, 0x04, 0xFF, 0xFB, 0x00, 0x06, 0x00, 0x08, 0xA9, 0x00, 0x00, 0x09, 0x00, 0x0A, 0x00, 0x05, 0x00, 0x0C, 0x00, 0x08, 0xA9, 0x08, 0xA9, 0x0F, 0x00, 0x05, 0x00, 0x0A, 0x00, 0x12, 0x00, 0x00, 0x00, 0xF6, 0x01, 0x0E, 0x00, 0x00, 0x00, 0xF7, 0x57, 0x11, 0x00, 0x04, 0xFF, 0xFB, 0x00, 0x14, 0x00, 0x08, 0xA9, 0x00, 0x00, 0x17, 0x00, 0x0A, 0x00, 0x05, 0x00, 0x19, 0xFF, 0x08, 0xA9, 0x08, 0xA9, 0x1D, 0x00, 0x05, 0x00, 0x0A, 0x00, 0x20, 0x00, 0x00, 0x00, 0x07, 0xFD, 0x15, 0x00, 0xF8, 0x81, 0x02, 0x80, 0x18, 0x00, 0xF3, 0x03, 0xFB, 0x01, 0x1B, 0x00, 0xF1, 0x01, 0xF3, 0x81, 0x1E, 0x00, 0xF3, 0x03, 0xEE, 0x03, 0x21, 0x00, 0xF8, 0x81, 0x07, 0xFD, 0x23, 0x00, 0xF8, 0x81, 0x02, 0x80, 0x26, 0x00, 0xF3, 0x03, 0xFB, 0x01, 0x29, 0x00, 0xF1, 0x01, 0xF3, 0x81, 0x2C, 0x00, 0xF3, 0x03, 0xEE, 0x03, 0x2F, 0x00, 0xF8, 0x81, 0xEC, 0x01, 0x32, 0x00, 0x00, 0x00, 0xEC, 0x01, 0x24, 0x00, 0x00, 0x00, 0xEF, 0xD2, 0x27, 0x00, 0x0B, 0xC1, 0xEF, 0xD2, 0x35, 0x00, 0x0B, 0xC1, 0xF9, 0xD2, 0x2A, 0x00, 0x13, 0x05, 0x06, 0x2E, 0x2D, 0x00, 0x13, 0x05, 0x10, 0x2E, 0x30, 0x00, 0x0B, 0xC1, 0x14, 0x00, 0x33, 0x00, 0x00, 0x00, 0xF9, 0xD2, 0x38, 0x00, 0x13, 0x05, 0x06, 0x2E, 0x3B, 0x00, 0x13, 0x05, 0x10, 0x2E, 0x3E, 0x00, 0x0B, 0xC1, 0x14, 0x00, 0x41, 0x00, 0x00, 0x00, 0x0F, 0x39, 0x36, 0x00, 0xF1, 0x4F, 0x02, 0xB9, 0x39, 0x00, 0xE8, 0x3A, 0xF3, 0x47, 0x3C, 0x00, 0xE8, 0x3A, 0xE6, 0xC7, 0x3F, 0x00, 0xF1, 0x4F, 0xE2, 0x00, 0x42, 0x00, 0x00, 0x00, 0x0F, 0x39, 0x44, 0x00, 0xF1, 0x4F, 0x02, 0xB9, 0x47, 0x00, 0xE8, 0x3A, 0xF3, 0x47, 0x49, 0xFF, 0xE8, 0x3A, 0xE6, 0xC7, 0x4C, 0xFF, 0xF1, 0x4F, 0xE2, 0x00, 0x50, 0x00, 0x00, 0x00, 0xE7, 0xBB, 0x44, 0xFF, 0x11, 0xA2, 0xF6, 0xBB, 0x47, 0xFF, 0x1C, 0x88, 0x09, 0x45, 0x4B, 0x00, 0x1C, 0x88, 0x18, 0x45, 0x4E, 0x00, 0x11, 0xA2, 0x1E, 0x00, 0x50, 0xFF, 0x00, 0x00, 0xE7, 0xBB, 0x52, 0xFF, 0x11, 0xA2, 0xF6, 0xBB, 0x55, 0xFF, 0x1C, 0x88, 0x09, 0x45, 0x58, 0xFF, 0x1C, 0x88, 0x18, 0x45, 0x5B, 0xFF, 0x11, 0xA2, 0x1E, 0x00, 0x5E, 0xFF, 0x00, 0x00, 0x17, 0x50, 0x53, 0xFF, 0xEB, 0x6E, 0x05, 0xD0, 0x56, 0xFF, 0xDE, 0xB7, 0xF0, 0x30, 0x59, 0xFF, 0xDE, 0xB7, 0xDE, 0xB0, 0x5C, 0xFF, 0xEB, 0x6E, 0xD8, 0x00, 0x5F, 0xFF, 0x00, 0x00, 0x17, 0x50, 0x61, 0xFF, 0xEB, 0x6E, 0x05, 0xD0, 0x64, 0xFF, 0xDE, 0xB7, 0xF0, 0x30, 0x67, 0xFF, 0xDE, 0xB7, 0xDE, 0xB0, 0x6A, 0xFF, 0xEB, 0x6E, 0xD8, 0x00, 0x6D, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_normal_tbl[] = { 0xC3, 0x00, 0x10, 0xC9, 0x00, 0x20, 0xE0, 0x00, 0x37, 0x00, 0x00, 0x40, 0x20, 0x00, 0x37, 0x37, 0x00, 0x20, 0x40, 0x00, 0x00, 0x37, 0x00, 0xE0, 0x20, 0x00, 0xC9, 0x00, 0x00, 0xC0, 0xE0, 0x00, 0xC9, 0xC9, 0x00, 0xE0, 0xC1, 0x00, 0x01, 0xCD, 0x00, 0x25, 0xED, 0x00, 0x3C, 0x13, 0x00, 0x3C, 0x33, 0x00, 0x25, 0x33, 0x00, 0xDB, 0x13, 0x00, 0xC4, 0xED, 0x00, 0xC4, 0xCD, 0x00, 0xDB, 0xC0, 0x00, 0x00, 0xC4, 0x00, 0xED, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_0[] = { 0x98, 0x00, 0x19, 0x18, 0x0C, 0x00, 0x12, 0x0B, 0x00, 0x17, 0x0B, 0x00, 0x11, 0x0A, 0x00, 0x16, 0x0A, 0x00, 0x10, 0x09, 0x00, 0x15, 0x09, 0x00, 0x0F, 0x08, 0x00, 0x14, 0x08, 0x00, 0x0E, 0x07, 0x00, 0x13, 0x07, 0x00, 0x06, 0x06, 0x00, 0x0D, 0x06, 0x00, 0x05, 0x05, 0x00, 0x0C, 0x05, 0x00, 0x04, 0x04, 0x00, 0x0B, 0x04, 0x00, 0x03, 0x03, 0x00, 0x0A, 0x03, 0x00, 0x02, 0x02, 0x00, 0x09, 0x02, 0x00, 0x01, 0x01, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8* moja_model_dl_0_tbl[] = { moja_model_dl_0_0, moja_model_dl_0_1, moja_model_dl_0_2, moja_model_dl_0_3, moja_model_dl_0_4, moja_model_dl_0_5, moja_model_dl_0_6, moja_model_dl_0_7, moja_model_dl_0_8, moja_model_dl_0_9, moja_model_dl_0_10, moja_model_dl_0_11, moja_model_dl_0_12, moja_model_dl_0_13, moja_model_dl_0_14, moja_model_dl_0_15, moja_model_dl_0_16, moja_model_dl_0_17, moja_model_dl_0_18, moja_model_dl_0_19, moja_model_dl_0_20 };
f32 chuchu_dt[] = { 0.0f, 1.0f, 1.0f, -0.169270828f, 0.847395837f, 0.785937548f, -0.368055552f, 0.676388919f, 0.545833349f, -0.5078125f, 0.542187512f, 0.357812494f, -0.5f, 0.5f, 0.300000012f, -0.280888885f, 0.579022229f, 0.41413334f, 0.0839999989f, 0.738399982f, 0.642400026f, 0.489333332f, 0.928266644f, 0.913600028f, 0.829777777f, 1.0987556f, 1.15653336f, 1.0f, 1.20000005f, 1.29999995f, 0.954666674f, 1.2236445f, 1.33137774f, 0.763999999f, 1.20293331f, 1.29813337f, 0.495999992f, 1.15040004f, 1.21920002f, 0.218666673f, 1.07857776f, 1.11351109f, 0.0f, 1.0f, 1.0f };
s8 anim_data_bikkuri[] = { 0, 0, 1, 2, 3, 5, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, -1, 0 };
s8 anim_data_hatena[] = { 0, 0, 2, 1, 3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, -1, 0, 0 };
s8 anim_data_loop[] = { 0, -10, 0, 0 };
s8 anim_data_moja[] = { 0, 0, 1, 1, 2, 2, 3, 3, -10, 0, 0, 0 };
static s8* anim_data_table[] = { anim_data_bikkuri, anim_data_hatena, anim_data_bikkuri, anim_data_loop, anim_data_tenten, anim_data_moja, anim_data_loop };
s8 anim_data_bikkuri_2[] = { 0, 0, 1, 2, 3, 5, 4, 5, -17, -1, 0, 0 };
s8 anim_data_hatena_2[] = { 0, 0, 2, 1, 3, 2, 3, -16, -1, 0, 0, 0 };
s8 anim_data_tenten_2[] = { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, -32, -1, 0, 0 };
s8* anim_data_table_2[] = { anim_data_bikkuri_2, anim_data_hatena_2, anim_data_bikkuri_2, anim_data_loop, anim_data_tenten_2, anim_data_moja, anim_data_loop };
u8 moja_model_color0_tbl[] ATTRIBUTE_ALIGN(32) = { 0x22, 0x4E, 0xB7, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_1[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x05, 0x1B, 0x0D, 0x00, 0x1A, 0x0D, 0x00, 0x18, 0x0C, 0x00, 0x19, 0x0C, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_2[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x1C, 0x0E, 0x00, 0x1A, 0x0D, 0x00, 0x20, 0x0E, 0x00, 0x1B, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_3[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x1D, 0x0F, 0x00, 0x1C, 0x0E, 0x00, 0x21, 0x0F, 0x00, 0x20, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_4[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x1E, 0x10, 0x00, 0x1D, 0x0F, 0x00, 0x22, 0x10, 0x00, 0x21, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_5[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x1F, 0x06, 0x00, 0x1E, 0x10, 0x00, 0x23, 0x06, 0x00, 0x22, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_6[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x24, 0x11, 0x00, 0x1F, 0x06, 0x00, 0x29, 0x11, 0x00, 0x23, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_7[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x25, 0x12, 0x00, 0x24, 0x11, 0x00, 0x2A, 0x12, 0x00, 0x29, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_8[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x26, 0x13, 0x00, 0x25, 0x12, 0x00, 0x2B, 0x13, 0x00, 0x2A, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_9[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x27, 0x14, 0x00, 0x26, 0x13, 0x00, 0x2C, 0x14, 0x00, 0x2B, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_10[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x28, 0x15, 0x00, 0x27, 0x14, 0x00, 0x2D, 0x15, 0x00, 0x2C, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_11[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x2E, 0x0D, 0x00, 0x28, 0x15, 0x00, 0x33, 0x0D, 0x00, 0x2D, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_12[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x2F, 0x0E, 0x00, 0x2E, 0x0D, 0x00, 0x34, 0x0E, 0x00, 0x33, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_13[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x30, 0x0F, 0x00, 0x2F, 0x0E, 0x00, 0x35, 0x0F, 0x00, 0x34, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_14[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x31, 0x10, 0x00, 0x30, 0x0F, 0x00, 0x36, 0x10, 0x00, 0x35, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_15[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x32, 0x06, 0x00, 0x31, 0x10, 0x00, 0x37, 0x06, 0x00, 0x36, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_16[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x38, 0x11, 0x00, 0x32, 0x06, 0x00, 0x3D, 0x11, 0x00, 0x37, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_17[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x39, 0x12, 0x00, 0x38, 0x11, 0x00, 0x3E, 0x12, 0x00, 0x3D, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_18[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x3A, 0x13, 0x00, 0x39, 0x12, 0x00, 0x3F, 0x13, 0x00, 0x3E, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_19[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x3B, 0x14, 0x00, 0x3A, 0x13, 0x00, 0x40, 0x14, 0x00, 0x3F, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_20[] ATTRIBUTE_ALIGN(32) = { 0x98, 0x00, 0x04, 0x3C, 0x16, 0x00, 0x3B, 0x14, 0x00, 0x41, 0x16, 0x00, 0x40, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
u8 moja_model_dl_0_size_tbl[] = { 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 };

#define QUAD(l, t, r, b, u, v)        \
    GXBegin(GX_QUADS, GX_VTXFMT0, 4); \
    GXPosition3f32(l, t, 0.0f);       \
    GXTexCoord2f32(0.0f, 0.0f);       \
    GXPosition3f32(r, t, 0.0f);       \
    GXTexCoord2f32(u, 0.0f);          \
    GXPosition3f32(r, b, 0.0f);       \
    GXTexCoord2f32(u, v);             \
    GXPosition3f32(l, b, 0.0f);       \
    GXTexCoord2f32(0.0f, v);

/* The speaker is an NpcEntry* or one of these script ids (signed compare) */
#define FUKIDASHI_MARIO       -1
#define FUKIDASHI_PARTY       -2
#define FUKIDASHI_EXTRA_PARTY -3
#define SPEAKER(npc)          ((EvtWord)(npc))

#define SETPOS(src)                                                                \
    work->position.x = rot[0][1] * (16.0f + work->size) + ((src)->position.x + work->base.x); \
    work->position.y = rot[1][1] * (16.0f + work->size) + ((src)->position.y + work->base.y); \
    work->position.z = rot[2][1] * (16.0f + work->size) + ((src)->position.z + work->base.z);

void calc_pos(EffFukidashiWork* work, s32 flag) {
    CameraEntry* camera;
    Vec axis;
    Mtx rot;

    camera = camGetPtr(CAMERA_3D);
    axis = (Vec){ (f32)sin(MTXDegToRad(camera->viewYaw)), 0.0f, -(f32)cos(MTXDegToRad(camera->viewYaw)) };
    PSMTXRotAxisRad(rot, &axis, MTXDegToRad(-((work->angle - 20.0f) + (f32)(flag * 20))));
    if (SPEAKER(work->npc) == FUKIDASHI_MARIO) {
        MarioWork* mario = marioGetPtr();
        SETPOS(mario)
    }
    else if (SPEAKER(work->npc) == FUKIDASHI_PARTY) {
        PartyEntry* party = partyGetPtr(marioGetPartyId());
        if (party) {
            SETPOS(party)
        }
    }
    else if (SPEAKER(work->npc) == FUKIDASHI_EXTRA_PARTY) {
        PartyEntry* party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
            SETPOS(party)
        }
    }
    else if (work->npc) {
        NpcEntry* npc = work->npc;
        SETPOS(npc)
    }
    else {
        work->position.x = rot[0][1] * (16.0f + work->size) + work->base.x;
        work->position.y = rot[1][1] * (16.0f + work->size) + work->base.y;
        work->position.z = rot[2][1] * (16.0f + work->size) + work->base.z;
    }
    work->zRot = (work->angle - 20.0f) + (f32)(flag * 20);
    work->scale = 0.5f * (work->size / 12.0f) + 0.5f;
}

EffectEntry* effFukidashiEntry(s32 type, s32 number, NpcEntry* npc, f32 x, f32 y, f32 z, f32 size,
                               f32 angle, s32 duration) {
    EffectEntry* effect;
    EffFukidashiWork* work;

    effect = effEntry();
    effect->type = "Fukidashi";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffFukidashiWork));
    effect->userdata = work;
    effect->callback = effFukidashiMain;
    work->type = type;
    work->number = number;
    if (duration < 0) {
        duration = 10000;
    }
    work->duration = duration;
    work->timer = duration;
    work->frame = 0;
    work->npc = npc;
    work->rot = 0.0f;
    work->alpha = 255;
    work->camera = CAMERA_3D;
    work->base.x = x;
    work->base.y = 3.0f + y;
    work->base.z = z;
    work->angle = angle;
    work->size = size;
    work->stage = 0;
    calc_pos(work, 1);
    if (type == 6) {
        work->rot = -450.0f;
    }
    if (SPEAKER(work->npc) == FUKIDASHI_MARIO) {
        MarioWork* mario = marioGetPtr();
        x += mario->position.x;
        y += mario->position.y;
        z += mario->position.z;
    }
    else if (SPEAKER(work->npc) == FUKIDASHI_PARTY) {
        PartyEntry* party = partyGetPtr(marioGetPartyId());
        if (party) {
            x += party->position.x;
            y += party->position.y;
            z += party->position.z;
        }
    }
    else if (SPEAKER(work->npc) == FUKIDASHI_EXTRA_PARTY) {
        PartyEntry* party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
            x += party->position.x;
            y += party->position.y;
            z += party->position.z;
        }
    }
    else if (work->npc) {
        x = work->npc->position.x;
        y = work->npc->position.y;
        z = work->npc->position.z;
    }
    if (SPEAKER(npc) == FUKIDASHI_PARTY || SPEAKER(npc) == FUKIDASHI_EXTRA_PARTY) {
        switch (type) {
            case 0: {
                Vec pos = { 0.0f, 0.0f, 0.0f };
                pos.x = x;
                pos.y = y;
                pos.z = z;
                psndSFXOn_3D(0x5D, &pos);
                break;
            }
            case 1: {
                Vec pos = { 0.0f, 0.0f, 0.0f };
                pos.x = x;
                pos.y = y;
                pos.z = z;
                psndSFXOn_3D(0x5E, &pos);
                break;
            }
        }
    }
    else if (npc && SPEAKER(npc) != FUKIDASHI_MARIO) {
        switch (type) {
            case 0: {
                Vec pos = { 0.0f, 0.0f, 0.0f };
                pos.x = x;
                pos.y = y;
                pos.z = z;
                psndSFXOn_3D("SFX_EVT_NPC_BIKKURI_MARK1", &pos);
                break;
            }
            case 1: {
                Vec pos = { 0.0f, 0.0f, 0.0f };
                pos.x = x;
                pos.y = y;
                pos.z = z;
                psndSFXOn_3D("SFX_EVT_NPC_QUESTION_MARK1", &pos);
                break;
            }
        }
    }
    return effect;
}

static inline s32 getAnim(EffFukidashiWork* w, s32 type) {
    if (w->duration == 0) {
        return anim_data_table[type][w->frame / 2];
    }
    return anim_data_table_2[type][w->frame / 2];
}

void effFukidashiMain(EffectEntry* effect) {
    EffFukidashiWork* work = effect->userdata;
    Vec position;
    s32 type;
    s32 anim;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    type = work->type;
    anim = getAnim(work, type);
    work->frame++;
    work->animVal = anim;
    if (anim < 0) {
        if (anim == -1) {
            effDelete(effect);
            return;
        }
        work->frame = (-10 - anim) * 2;
        anim = getAnim(work, type);
        work->frame++;
        work->animVal = anim;
    }
    if (type == 6) {
        switch (work->stage) {
            case 0:
                work->rot += 22.5f;
                if (work->rot >= 0.0f) {
                    work->rot = 0.0f;
                    work->stage++;
                }
                break;
            case 120:
                work->rot -= 9.0f;
                if (work->rot <= -90.0f) {
                    effDelete(effect);
                    return;
                }
                break;
            default:
                work->stage++;
                break;
        }
    }
    else if (work->duration > 0) {
        if (work->timer < 10000) {
            work->timer--;
        }
        if (work->timer < 0) {
            effDelete(effect);
            return;
        }
    }
    if (work->npc) {
        calc_pos(work, 1);
    }
    if (work->type == 4) {
        dispEntry(work->camera, dispCalcZ(&position), 2, (DispCallback)effFukidashiDisp, effect);
    }
    else {
        dispEntry(work->camera, dispCalcZ(&position), 8, (DispCallback)effFukidashiDisp, effect);
    }
}

void effFukidashiDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    s32 alphaA;
    s32 alphaB;
    EffFukidashiWork* work = effect->userdata;
    s32 alpha;
    s32 pass;
    s32 passes;
    s32 i;
    u8** dp;
    u32 j;
    u8* sp;
    f32 degToRad;
    Mtx mF;
    Mtx mD;
    Mtx mA;
    Mtx mG;
    Mtx mB;
    Mtx mE;
    Mtx mC;
    Mtx mH;
    Mtx mJ;
    GXLightObj light;
    Mtx mI;
    GXTexObj texObj;
    Vec lp;
    Vec lightPos;
    char text[32];

    camera = camGetPtr(cameraId);
    alpha = work->alpha;
    passes = 1;
    mapSetMaterialFog();
    if (work->rot != 0.0f) {
        passes = 2;
    }
    if (work->type == 2) {
        passes = 2;
    }
    alphaA = alpha * 0xB0 / 0xFF;
    alphaB = alpha * 0xFF / 0xFF;
    degToRad = MTXDegToRad(1.0f);
    for (pass = 0; pass < passes; pass++) {
        PSMTXTrans(mA, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(mB, 'y', degToRad * -camGetPtr(cameraId)->viewYaw);
        PSMTXConcat(mA, mB, mC);
        PSMTXScale(mD, 0.8f, 0.8f, 0.8f);
        PSMTXConcat(mC, mD, mC);
        PSMTXScale(mD, 1.0f, 0.8f, 1.0f);
        PSMTXRotRad(mE, 'z', degToRad * work->zRot);
        PSMTXConcat(mC, mD, mD);
        PSMTXConcat(mD, mE, mF);
        PSMTXConcat(camera->view, mF, mF);
        GXSetCurrentMtx(GX_PNMTX0);
        if (pass == 0) {
            GXSetCullMode(GX_CULL_BACK);
        }
        else {
            GXSetCullMode(GX_CULL_FRONT);
        }
        PSMTXIdentity(mA);
        GXLoadTexMtxImm(mA, GX_TEXMTX0, GX_MTX2x4);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                effGetTexObj(5, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0, 0, 0, alpha });
                PSMTXTrans(mA, 1.3f, -1.3f, 0.0f);
                PSMTXConcat(mF, mA, mA);
                PSMTXRotRad(mG, 'y', degToRad * work->rot);
                PSMTXConcat(mA, mG, mA);
                GXLoadPosMtxImm(mA, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
            }
            else {
                effGetTexObj(4, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                if (pass == 0) {
                    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, alpha });
                }
                else {
                    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xA0, 0xA0, 0xA0, alpha });
                }
                PSMTXTrans(mA, 0.0f, 0.0f, 0.0f);
                PSMTXConcat(mF, mA, mA);
                PSMTXRotRad(mG, 'y', degToRad * work->rot);
                PSMTXConcat(mA, mG, mA);
                GXLoadPosMtxImm(mA, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
            }
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(-16.0f, 13.0f, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(16.0f, 13.0f, 0.0f);
            GXTexCoord2f32(2.0f, 0.0f);
            GXPosition3f32(16.0f, -19.0f, 0.0f);
            GXTexCoord2f32(2.0f, 1.0f);
            GXPosition3f32(-16.0f, -19.0f, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
        PSMTXScale(mH, 1.3f, 1.3f, 1.3f);
        PSMTXConcat(mF, mH, mF);
        switch (work->type) {
            case 0:
            case 1:
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                switch (work->type) {
                    case 0:
                        effGetTexObj(6, &texObj);
                        GXLoadTexObj(&texObj, GX_TEXMAP0);
                        PSMTXScale(mD, 1.0f / 6.0f, 1.0f, 1.0f);
                        PSMTXTrans(mA, work->animVal, 0.0f, 0.0f);
                        PSMTXConcat(mD, mA, mA);
                        GXLoadTexMtxImm(mA, GX_TEXMTX0, GX_MTX2x4);
                        break;
                    case 1:
                        effGetTexObj(7, &texObj);
                        GXLoadTexObj(&texObj, GX_TEXMAP0);
                        PSMTXScale(mD, 0.25f, 1.0f, 1.0f);
                        PSMTXTrans(mA, work->animVal, 0.0f, 0.0f);
                        PSMTXConcat(mD, mA, mA);
                        GXLoadTexMtxImm(mA, GX_TEXMTX0, GX_MTX2x4);
                        break;
                }
                GXSetTevColor(GX_TEVREG0, (GXColor){ 0, 0, 0, alphaA });
                PSMTXTransApply(mF, mH, 1.0f, -1.0f, 0.0f);
                GXLoadPosMtxImm(mH, GX_PNMTX0);
                if (work->type == 0) {
                    QUAD(-4.0f, 8.0f, 4.0f, -8.0f, 1.0f, 1.0f)
                }
                else {
                    QUAD(-8.0f, 16.0f, 8.0f, -8.0f, 1.0f, 1.0f)
                }
                if (pass == 0) {
                    GXSetTevColor(GX_TEVREG0, (GXColor){ 0xFF, 0, 0, alphaB });
                }
                else {
                    GXSetTevColor(GX_TEVREG0, (GXColor){ 0xA0, 0, 0, alphaB });
                }
                GXLoadPosMtxImm(mF, GX_PNMTX0);
                if (work->type == 0) {
                    QUAD(-4.0f, 8.0f, 4.0f, -8.0f, 1.0f, 1.0f)
                }
                else {
                    QUAD(-8.0f, 16.0f, 8.0f, -8.0f, 1.0f, 1.0f)
                }
                break;
            case 2:
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
                GXSetChanAmbColor(GX_COLOR0A0, (GXColor){ 0xE0, 0xE0, 0xE0, alphaB });
                lightPos = (Vec){ 0.0f, 0.0f, 0.0f };
                lightPos.x = 10.0f * (f32)cos(gp->retraceCount / 10.0f) + work->position.x;
                lightPos.y = work->position.y;
                lightPos.z = work->position.z - 10.0f * (f32)sin(gp->retraceCount / 10.0f);
                lp = lightPos;
                PSMTXMultVec(camGetPtr(cameraId)->view, &lp, &lp);
                GXInitLightPos(&light, lp.x, lp.y, lp.z);
                GXInitLightColor(&light, (GXColor){ 0xFF, 0xFF, 0xFF, alphaB });
                GXInitLightAttn(&light, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
                GXLoadLightObjImm(&light, GX_LIGHT0);
                PSMTXTrans(mI, 5.0f, -8.0f, 0.0f);
                PSMTXConcat(mF, mI, mH);
                PSMTXRotRad(mI, 'z', 0.43633232f);
                PSMTXConcat(mH, mI, mH);
                PSMTXRotRad(mI, 'y', degToRad * (f32)(gp->retraceCount * 10));
                PSMTXConcat(mH, mI, mH);
                PSMTXScale(mI, 0.15f, 0.15f, 0.15f);
                PSMTXConcat(mH, mI, mH);
                GXLoadPosMtxImm(mH, GX_PNMTX0);
                PSMTXInvXpose(mH, mJ);
                GXLoadNrmMtxImm(mJ, GX_PNMTX0);
                GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
                GXSetArray(GX_VA_POS, moja_model_vertex_tbl, 6);
                GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
                GXSetArray(GX_VA_NRM, moja_model_normal_tbl, 3);
                GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
                GXSetArray(GX_VA_CLR0, moja_model_color0_tbl, 4);
                dp = moja_model_dl_0_tbl;
                for (j = 0, sp = moja_model_dl_0_size_tbl; j < 21; j++, sp++, dp++) {
                    GXCallDisplayList(*dp, *sp << 5);
                }
                break;
            case 3:
                if (work->number >= 0 && work->number <= 99) {
                    f32 width;
                    sprintf(text, "%d", work->number);
                    width = 10.0f + (f32)FontGetMessageWidth(text);
                    PSMTXRotRad(mG, 'y', degToRad * work->rot);
                    PSMTXScale(mD, 0.5f, 0.5f, 0.5f);
                    PSMTXTrans(mA, -width / 2.0f, -10.0f, 0.0f);
                    PSMTXConcat(mC, mG, mH);
                    PSMTXConcat(mH, mD, mH);
                    PSMTXConcat(mH, mA, mH);
                    if (pass == 0) {
                        iconDispGxCol(mH, 0x400, ICON_18B, (GXColor){ 0xFF, 0xFF, 0xFF, alpha });
                    }
                    else {
                        iconDispGxCol(mH, 0x800, ICON_18B, (GXColor){ 0xA0, 0xA0, 0xA0, alpha });
                    }
                    PSMTXTrans(mA, 10.0f, 22.0f, 0.0f);
                    PSMTXConcat(mH, mA, mH);
                    FontDrawStart_alpha(alpha);
                    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
                    if (pass == 0) {
                        GXSetCullMode(GX_CULL_BACK);
                    }
                    else {
                        GXSetCullMode(GX_CULL_FRONT);
                    }
                    FontDrawStringMtx(mH, text);
                }
                break;
            case 4:
                PSMTXIdentity(mA);
                GXLoadTexMtxImm(mA, GX_TEXMTX0, GX_MTX2x4);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                GXSetTevColor(GX_TEVREG0, (GXColor){ 0, 0, 0, alphaB });
                effGetTexObj(8, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                for (i = 0; i < work->animVal; i++) {
                    PSMTXTransApply(mF, mH, 5.0f * i + -5.0f, 0.0f, 0.0f);
                    GXLoadPosMtxImm(mH, GX_PNMTX0);
                    QUAD(-3.0f, 3.0f, 3.0f, -3.0f, 1.0f, 1.0f)
                }
                break;
            case 5:
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                effGetTexObj(9, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                PSMTXScale(mD, 0.25f, 1.0f, 1.0f);
                PSMTXTrans(mA, work->animVal, 0.0f, 0.0f);
                PSMTXConcat(mD, mA, mD);
                GXLoadTexMtxImm(mD, GX_TEXMTX0, GX_MTX2x4);
                GXSetTevColor(GX_TEVREG0, (GXColor){ 0, 0, 0, alphaB });
                GXLoadPosMtxImm(mF, GX_PNMTX0);
                QUAD(-7.0f, 7.0f, 7.0f, -7.0f, 1.0f, 1.0f)
                break;
            case 6: {
                f32* p;
                u32 idx = gp->retraceCount % 15;
                PSMTXIdentity(mA);
                GXLoadTexMtxImm(mA, GX_TEXMTX0, GX_MTX2x4);
                p = chuchu_dt + idx * 3;
                PSMTXTrans(mA, 0.0f, p[0], 0.0f);
                PSMTXScale(mD, 0.7f * p[1], 0.7f * p[2], 0.7f);
                PSMTXRotRad(mG, 'y', degToRad * work->rot);
                PSMTXConcat(mA, mG, mA);
                PSMTXConcat(mA, mD, mA);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
                effGetTexObj(10, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                if (pass == 0) {
                    GXSetTevColor(GX_TEVREG0, (GXColor){ 0, 0, 0, alphaA });
                    PSMTXTransApply(mF, mH, 1.0f, -1.0f, 0.0f);
                    PSMTXConcat(mH, mA, mH);
                    GXLoadPosMtxImm(mH, GX_PNMTX0);
                    QUAD(-8.0f, 16.0f, 8.0f, -8.0f, 2.0f, 2.0f)
                    if (pass == 0) {
                        GXSetTevColor(GX_TEVREG0, (GXColor){ 0xFF, 0xCD, 0x28, alphaB });
                    }
                    else {
                        GXSetTevColor(GX_TEVREG0, (GXColor){ 0x7F, 0x66, 0x14, alphaB });
                    }
                    PSMTXConcat(mF, mA, mH);
                    GXLoadPosMtxImm(mH, GX_PNMTX0);
                    QUAD(-8.0f, 16.0f, 8.0f, -8.0f, 2.0f, 2.0f)
                }
                break;
            }
        }
    }
}
