#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_kemu_test.h"
#include "eff/eff_nozle.h"

typedef struct EffNozleWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 sx;         // 0x10
    f32 sy;         // 0x14
    f32 u0;         // 0x18
    f32 v0;         // 0x1C
    f32 u1;         // 0x20
    f32 v1;         // 0x24
    f32 scale;      // 0x28
    s32 life;       // 0x2C
    s32 timer;      // 0x30
    s32 alpha;      // 0x34
    s32 flag;       // 0x38
    f32 rotX;       // 0x3C
    f32 rotY;       // 0x40
} EffNozleWork;

void effNozleMain(EffectEntry* effect);
void effNozleDisp(CameraId cameraId, EffectEntry* effect);



static s16 nozle_1_vertex_tbl[] __attribute__((aligned(32))) = {
    -796, 0, 0, 796, 0, 0,
    -1741, 2132, 0, 1741, 2132, 0,
    -2514, 4266, 0, 2514, 4266, 0,
    -3316, 6399, 0, 3316, 6399, 0,
    -3891, 8532, 0, 3891, 8532, 0,
    -4387, 10666, 0, 4387, 10666, 0,
    -5146, 12799, 0, 5146, 12799, 0,
    0, 0, 0, 0, 2132, 0,
    0, 4266, 0, 0, 6399, 0,
    0, 8532, 0, 0, 10666, 0,
    0, 14719, 0, -398, 0, 0,
    -870, 2132, 0, -1257, 4266, 0,
    -1752, 6399, 0, -2039, 8532, 0,
    -2193, 10666, 0, -2573, 14207, 0,
    398, 0, 0, 870, 2132, 0,
    1257, 4266, 0, 1752, 6399, 0,
    2039, 8532, 0, 2193, 10666, 0,
    2573, 14207, 0, -1279, 0, -640,
    1279, 0, -640, -2394, 2132, -640,
    2394, 2132, -640, -3354, 4266, -640,
    3354, 4266, -640, -4157, 6399, -640,
    4157, 6399, -640, -4731, 8532, -640,
    4731, 8532, -640, -5228, 10666, -640,
    5228, 10666, -640, -5986, 15271, -640,
    5986, 15271, -640, 0, 0, -640,
    0, 2132, -640, 0, 4266, -640,
    0, 6399, -640, 0, 8532, -640,
    0, 10666, -640, 0, 17191, -640,
    -639, 0, -640, -1112, 2132, -640,
    -1592, 4266, -640, -2087, 6399, -640,
    -2375, 8532, -640, -2529, 10666, -640,
    -2908, 16679, -640, 639, 0, -640,
    1112, 2132, -640, 1592, 4266, -640,
    2087, 6399, -640, 2375, 8532, -640,
    2529, 10666, -640, 2908, 16679, -640,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0,
};
static s16 nozle_1_texcoord0_tbl[] __attribute__((aligned(32))) = {
    0, 13653, 4096, 13653, 4096, 16384, 0, 16384,
    0, 10922, 4096, 10922, 0, 8192, 4096, 8192,
    0, 5461, 4096, 5461, 0, 2730, 4096, 2730,
    0, 0, 4096, 0, 8192, 13653, 12288, 13653,
    12288, 16384, 8192, 16384, 12288, 10922, 8192, 10922,
    12288, 8192, 8192, 8192, 12288, 5461, 8192, 5461,
    12288, 2730, 8192, 2730, 12288, 0, 8192, 0,
    16384, 13653, 16384, 16384, 16384, 10922, 16384, 8192,
    16384, 5461, 16384, 2730, 16384, 0, 0, -2730,
    4096, -2730, 0, -5461, 4096, -5461, 0, -8192,
    4096, -8192, 12288, -2730, 8192, -2730, 12288, -5461,
    8192, -5461, 12288, -8192, 8192, -8192, 16384, -2730,
    16384, -5461, 16384, -8192, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
static s16 nozle_1_texcoord1_tbl[] __attribute__((aligned(32))) = {
    0, 13653, 4096, 13653, 4096, 16384, 0, 16384,
    0, 10922, 4096, 10922, 0, 8192, 4096, 8192,
    0, 5461, 4096, 5461, 0, 2730, 4096, 2730,
    0, 0, 4096, 0, 8192, 13653, 12288, 13653,
    12288, 16384, 8192, 16384, 12288, 10922, 8192, 10922,
    12288, 8192, 8192, 8192, 12288, 5461, 8192, 5461,
    12288, 2730, 8192, 2730, 12288, 0, 8192, 0,
    16384, 13653, 16384, 16384, 16384, 10922, 16384, 8192,
    16384, 5461, 16384, 2730, 16384, 0, 0, -2730,
    4096, -2730, 0, -5461, 4096, -5461, 0, -8192,
    4096, -8192, 12288, -2730, 8192, -2730, 12288, -5461,
    8192, -5461, 12288, -8192, 8192, -8192, 16384, -2730,
    16384, -5461, 16384, -8192, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
static u32 nozle_1_dl_0_0[] __attribute__((aligned(32))) = {0x98000A0D, 0x00042222, 0x0B000221, 0x21220004, 0x1A1A2100, 0x03181814, 0x00041B1B, 0x13000519, 0x191B0004, 0x0D0D1A00, 0x030B0B0A, 0x00020A0A, 0x08000008, 0x08000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_0_1[] __attribute__((aligned(32))) = {0x98000A0B, 0x00022121, 0x09000020, 0x20210003, 0x18182000, 0x00161613, 0x00051919, 0x12000017, 0x171A0003, 0x0B0B1900, 0x00090908, 0x00000808, 0x06000006, 0x06000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_0_2[] __attribute__((aligned(32))) = {0x98000A09, 0x00002020, 0x0700001F, 0x1F200000, 0x16161F00, 0x00141412, 0x00001717, 0x11000015, 0x15190000, 0x09091800, 0x00070706, 0x00000606, 0x04000004, 0x04000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_0_3[] __attribute__((aligned(32))) = {0x98000A07, 0x00001F1F, 0x0500001E, 0x1E1F0000, 0x14141E00, 0x00121211, 0x00001515, 0x10000013, 0x13180000, 0x07071700, 0x00050504, 0x00000404, 0x02000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_0_4[] __attribute__((aligned(32))) = {0x98000A05, 0x00001E1E, 0x0300001C, 0x1C1E0000, 0x12121D00, 0x000F0F10, 0x00001313, 0x0F00000E, 0x0E170000, 0x05051600, 0x00010102, 0x00000000, 0x00000103, 0x03000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_1_0[] __attribute__((aligned(32))) = {0x98000A30, 0x00043131, 0x2E000230, 0x30450004, 0x2D2D4400, 0x032B2B37, 0x00042E2E, 0x3600052C, 0x2C3E0004, 0x28283D00, 0x0326262D, 0x00022525, 0x2B000023, 0x23000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_1_1[] __attribute__((aligned(32))) = {0x98000A2E, 0x00023030, 0x2C00002F, 0x2F440003, 0x2B2B4300, 0x00292936, 0x00052C2C, 0x3500002A, 0x2A3D0003, 0x26263C00, 0x0024242B, 0x00002323, 0x2900000C, 0x0C000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_1_2[] __attribute__((aligned(32))) = {0x98000A2C, 0x00002F2F, 0x2A000022, 0x22430000, 0x29294200, 0x001A1A35, 0x00002A2A, 0x3400001B, 0x1B3C0000, 0x24243B00, 0x000D0D29, 0x00000C0C, 0x2700000A, 0x0A000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_1_3[] __attribute__((aligned(32))) = {0x98000A2A, 0x00002222, 0x28000021, 0x21420000, 0x1A1A4100, 0x00181834, 0x00001B1B, 0x33000019, 0x193B0000, 0x0D0D3A00, 0x000B0B27, 0x00000A0A, 0x25000008, 0x08000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_1_4[] __attribute__((aligned(32))) = {0x98000A28, 0x00002121, 0x26000020, 0x20410000, 0x18184000, 0x00161633, 0x00001919, 0x32000017, 0x173A0000, 0x0B0B3900, 0x00090925, 0x00000808, 0x23000106, 0x06000000, 0x00000000, 0x00000000};
static s16 nozle_3_vertex_tbl[] __attribute__((aligned(32))) = {
    -1120, 0, 0, -495, 0, 0,
    0, 0, 0, 495, 0, 0,
    1120, 0, 0, -3040, 2133, 0,
    -1520, 2133, 0, 0, 2133, 0,
    1520, 2133, 0, 3040, 2133, 0,
    -4191, 4266, 0, -2032, 4266, 0,
    0, 4266, 0, 2032, 4266, 0,
    4191, 4266, 0, -4832, 6400, 0,
    -2287, 6400, 0, 0, 6400, 0,
    2287, 6400, 0, 4831, 6400, 0,
    -5215, 8533, 0, -2544, 8533, 0,
    0, 8533, 0, 2544, 8533, 0,
    5215, 8533, 0, -5471, 10666, 0,
    -2672, 10666, 0, 0, 10666, 0,
    2672, 10666, 0, 5471, 10666, 0,
    0, 12800, 0, 2800, 12800, 0,
    5600, 12800, 0, -5600, 12800, 0,
    -5325, 13788, 0, -4530, 15192, 0,
    -3291, 16284, 0, -1730, 17123, 0,
    0, 17664, 0, 1730, 17123, 0,
    3291, 16284, 0, 4530, 15192, 0,
    5325, 13788, 0, -2800, 12800, 0,
    -2662, 13294, 0, -2265, 14252, 0,
    -1645, 14990, 0, -865, 15601, 0,
    0, 15936, 0, 865, 15601, 0,
    1645, 14990, 0, 2265, 14252, 0,
    2662, 13294, 0, -1280, 0, -640,
    -566, 0, -640, 0, 0, -640,
    566, 0, -640, 1280, 0, -640,
    -3474, 2240, -640, -1737, 2240, -640,
    0, 2240, -640, 1737, 2240, -640,
    3474, 2240, -640, -4790, 4480, -640,
    -2322, 4480, -640, 0, 4480, -640,
    2322, 4480, -640, 4790, 4480, -640,
    -5522, 6720, -640, -2614, 6720, -640,
    0, 6720, -640, 2614, 6720, -640,
    5522, 6720, -640, -5961, 8960, -640,
    -2907, 8960, -640, 0, 8960, -640,
    2907, 8960, -640, 5961, 8960, -640,
    -6253, 11200, -640, -3053, 11200, -640,
    0, 11200, -640, 3053, 11200, -640,
    6253, 11200, -640, 0, 13440, -640,
    3200, 13440, -640, 6400, 13440, -640,
    -6400, 13440, -640, -6086, 14478, -640,
    -5177, 15952, -640, -3761, 17099, -640,
    -1977, 17979, -640, 0, 18547, -640,
    1977, 17979, -640, 3761, 17099, -640,
    5177, 15952, -640, 6086, 14478, -640,
    -3200, 13440, -640, -3043, 13959, -640,
    -2588, 14965, -640, -1880, 15739, -640,
    -988, 16381, -640, 0, 16732, -640,
    988, 16381, -640, 1880, 15739, -640,
    2588, 14965, -640, 3043, 13959, -640,
    0, 0,
};
static s16 nozle_3_texcoord0_tbl[] __attribute__((aligned(32))) = {
    0, 13653, 4096, 13653, 4096, 16384, 0, 16384,
    8192, 13653, 8192, 16384, 12288, 13653, 12288, 16384,
    16384, 13653, 16384, 16384, 0, 10922, 4096, 10922,
    8192, 10922, 12288, 10922, 16384, 10922, 0, 8192,
    4096, 8192, 8192, 8192, 12288, 8192, 16384, 8192,
    0, 5461, 4096, 5461, 8192, 5461, 12288, 5461,
    16384, 5461, 0, 2730, 4096, 2730, 8192, 2730,
    12288, 2730, 16384, 2730, 0, 0, 4096, 0,
    8192, 0, 12288, 0, 16384, 0, 4096, -819,
    0, -819, 4096, -1638, 0, -1638, 4096, -2457,
    0, -2457, 4096, -3276, 0, -3276, 4096, -4096,
    0, -4096, 12288, -3276, 12288, -4096, 16384, -4096,
    16384, -3276, 12288, -2457, 16384, -2457, 12288, -1638,
    16384, -1638, 12288, -819, 16384, -819, 0, -2730,
    4096, -2730, 8192, -2730, 12288, -2730, 16384, -2730,
    0, -5461, 4096, -5461, 8192, -5461, 12288, -5461,
    16384, -5461, 0, -8192, 4096, -8192, 8192, -8192,
    12288, -8192, 16384, -8192, 4096, -9011, 0, -9011,
    4096, -9830, 0, -9830, 4096, -10649, 0, -10649,
    4096, -11468, 0, -11468, 4096, -12288, 0, -12288,
    12288, -11468, 12288, -12288, 16384, -12288, 16384, -11468,
    12288, -10649, 16384, -10649, 12288, -9830, 16384, -9830,
    12288, -9011, 16384, -9011, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
static s16 nozle_3_texcoord1_tbl[] __attribute__((aligned(32))) = {
    0, 13653, 4096, 13653, 4096, 16384, 0, 16384,
    8192, 13653, 8192, 16384, 12288, 13653, 12288, 16384,
    16384, 13653, 16384, 16384, 0, 10922, 4096, 10922,
    8192, 10922, 12288, 10922, 16384, 10922, 0, 8192,
    4096, 8192, 8192, 8192, 12288, 8192, 16384, 8192,
    0, 5461, 4096, 5461, 8192, 5461, 12288, 5461,
    16384, 5461, 0, 2730, 4096, 2730, 8192, 2730,
    12288, 2730, 16384, 2730, 0, 0, 4096, 0,
    8192, 0, 12288, 0, 16384, 0, 4096, -819,
    0, -819, 4096, -1638, 0, -1638, 4096, -2457,
    0, -2457, 4096, -3276, 0, -3276, 4096, -4096,
    0, -4096, 12288, -3276, 12288, -4096, 16384, -4096,
    16384, -3276, 12288, -2457, 16384, -2457, 12288, -1638,
    16384, -1638, 12288, -819, 16384, -819, 0, -2730,
    4096, -2730, 8192, -2730, 12288, -2730, 16384, -2730,
    0, -5461, 4096, -5461, 8192, -5461, 12288, -5461,
    16384, -5461, 0, -8192, 4096, -8192, 8192, -8192,
    12288, -8192, 16384, -8192, 4096, -9011, 0, -9011,
    4096, -9830, 0, -9830, 4096, -10649, 0, -10649,
    4096, -11468, 0, -11468, 4096, -12288, 0, -12288,
    12288, -11468, 12288, -12288, 16384, -12288, 16384, -11468,
    12288, -10649, 16384, -10649, 12288, -9830, 16384, -9830,
    12288, -9011, 16384, -9011, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
static GXColor nozle_3_color0_tbl[] __attribute__((aligned(32))) = {
    {255, 203, 23, 204}, {255, 240, 182, 51}, {255, 177, 23, 204}, {255, 124, 33, 204},
    {227, 81, 21, 204}, {214, 76, 20, 204}, {170, 61, 16, 102}, {180, 64, 17, 102},
    {188, 67, 18, 102}, {180, 64, 17, 76}, {170, 61, 16, 76}, {180, 64, 17, 51},
    {170, 61, 16, 51}, {180, 64, 17, 25}, {170, 61, 16, 25}, {180, 64, 17, 0},
    {170, 61, 16, 0}, {183, 146, 16, 204}, {186, 176, 133, 51}, {178, 123, 16, 204},
    {178, 87, 23, 204}, {170, 61, 16, 204}, {126, 45, 12, 204}, {95, 34, 9, 102},
    {95, 34, 9, 76}, {95, 34, 9, 51}, {95, 34, 9, 25}, {95, 34, 9, 0},
    {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
};
static u32 nozle_3_dl_0_0[] __attribute__((aligned(32))) = {0x98001100, 0x04000000, 0x01000900, 0x09000300, 0x00000100, 0x07000700, 0x09000000, 0x00000800, 0x08000800, 0x00000000, 0x06000600, 0x0E000000, 0x02000E00, 0x0E000D00, 0x00000200, 0x0D000D00, 0x13000000, 0x03001300, 0x13001200, 0x00000300, 0x12001200, 0x18000000, 0x04001800, 0x18001700, 0x00000400, 0x17001700, 0x1D000000, 0x05001D00, 0x1D001C00, 0x00000500, 0x1C001C00, 0x20000000, 0x06002200, 0x22001F00, 0x00000700, 0x21002100, 0x34000000, 0x07003500, 0x35001E00, 0x00000800, 0x20002000, 0x33000000, 0x09003300, 0x33000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_1[] __attribute__((aligned(32))) = {0x98001100, 0x02000000, 0x01000500, 0x05000100, 0x00000100, 0x02000200, 0x07000000, 0x00000400, 0x04000600, 0x00000000, 0x01000100, 0x0C000000, 0x02000C00, 0x0C000B00, 0x00000200, 0x0B000B00, 0x11000000, 0x03001100, 0x11001000, 0x00000300, 0x10001000, 0x16000000, 0x04001600, 0x16001500, 0x00000400, 0x15001500, 0x1B000000, 0x05001B00, 0x1B001A00, 0x00000500, 0x1A001A00, 0x1E000000, 0x08002000, 0x20002B00, 0x00000700, 0x1F001F00, 0x2C000000, 0x07002300, 0x23002200, 0x00000600, 0x24002400, 0x23000000, 0x0A002600, 0x26000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_2[] __attribute__((aligned(32))) = {0x98000E00, 0x03000000, 0x01000700, 0x07000200, 0x00000100, 0x05000500, 0x08000000, 0x00000600, 0x06000700, 0x00000000, 0x04000400, 0x0D000000, 0x02000D00, 0x0D000C00, 0x00000200, 0x0C000C00, 0x12000000, 0x03001200, 0x12001100, 0x00000300, 0x11001100, 0x17000000, 0x04001700, 0x17001600, 0x00000400, 0x16001600, 0x1C000000, 0x05001C00, 0x1C001B00, 0x00000500, 0x1B001B00, 0x1F000000, 0x07002100, 0x21001E00, 0x00000800, 0x20002000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_3[] __attribute__((aligned(32))) = {0x98000900, 0x26000000, 0x10002F00, 0x2F002700, 0x00000E00, 0x30003000, 0x30000000, 0x0F002E00, 0x2E003100, 0x00000D00, 0x2D002D00, 0x1E000000, 0x08002000, 0x20003200, 0x00000B00, 0x31003100, 0x33000000, 0x09003300, 0x33002900, 0x00000A00, 0x34003400, 0x2A000000, 0x06003600, 0x36000000};
static u32 nozle_3_dl_0_4[] __attribute__((aligned(32))) = {0x98000600, 0x30000000, 0x0F002B00, 0x2B001E00, 0x00000800, 0x20002000, 0x2F000000, 0x0D002900, 0x29002E00, 0x00000B00, 0x27002700, 0x25000000, 0x0E002A00, 0x2A002400, 0x00000C00, 0x28002800};
static u32 nozle_3_dl_0_5[] __attribute__((aligned(32))) = {0x98000500, 0x29000000, 0x0A003400, 0x34003200, 0x00000B00, 0x31003100, 0x28000000, 0x0C003200, 0x32003100, 0x00000D00, 0x2D002D00, 0x27000000, 0x0E003000, 0x30000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_6[] __attribute__((aligned(32))) = {0x98000500, 0x2E000000, 0x0B002700, 0x27001E00, 0x00000800, 0x20002000, 0x2D000000, 0x09002500, 0x25002C00, 0x00000700, 0x23002300, 0x23000000, 0x0A002600, 0x26000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_7[] __attribute__((aligned(32))) = {0x98000400, 0x23000000, 0x0A002600, 0x26002400, 0x00000C00, 0x28002800, 0x2D000000, 0x09002500, 0x25002E00, 0x00000B00, 0x27002700, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_8[] __attribute__((aligned(32))) = {0x98000400, 0x25000000, 0x0E002A00, 0x2A002600, 0x00001000, 0x2C002C00, 0x2F000000, 0x0D002900, 0x29003000, 0x00000F00, 0x2B002B00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_9[] __attribute__((aligned(32))) = {0x98000400, 0x0B000000, 0x02000B00, 0x0B000600, 0x00000000, 0x01000100, 0x05000000, 0x00000000, 0x00000000, 0x00000100, 0x03000300, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_10[] __attribute__((aligned(32))) = {0x98000400, 0x10000000, 0x03001000, 0x10000B00, 0x00000200, 0x0B000B00, 0x0A000000, 0x02000A00, 0x0A000500, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_11[] __attribute__((aligned(32))) = {0x98000400, 0x15000000, 0x04001500, 0x15001000, 0x00000300, 0x10001000, 0x0F000000, 0x03000F00, 0x0F000A00, 0x00000200, 0x0A000A00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_12[] __attribute__((aligned(32))) = {0x98000400, 0x1A000000, 0x05001A00, 0x1A001500, 0x00000400, 0x15001500, 0x14000000, 0x04001400, 0x14000F00, 0x00000300, 0x0F000F00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_13[] __attribute__((aligned(32))) = {0x98000400, 0x2B000000, 0x07001F00, 0x1F001A00, 0x00000500, 0x1A001A00, 0x19000000, 0x05001900, 0x19001400, 0x00000400, 0x14001400, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_14[] __attribute__((aligned(32))) = {0x98000400, 0x22000000, 0x06002400, 0x24002B00, 0x00000700, 0x1F001F00, 0x21000000, 0x06001E00, 0x1E001900, 0x00000500, 0x19001900, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_15[] __attribute__((aligned(32))) = {0x98000400, 0x20000000, 0x06002200, 0x22003400, 0x00000700, 0x35003500, 0x2A000000, 0x06003600, 0x36003300, 0x00000900, 0x33003300, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_0_16[] __attribute__((aligned(32))) = {0x98000300, 0x06000000, 0x00000100, 0x01000100, 0x00000100, 0x02000200, 0x00000000, 0x01000300, 0x03000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static void* nozle_3_dl_0_tbl[] __attribute__((aligned(32))) = {nozle_3_dl_0_0, nozle_3_dl_0_1, nozle_3_dl_0_2, nozle_3_dl_0_3, nozle_3_dl_0_4, nozle_3_dl_0_5, nozle_3_dl_0_6, nozle_3_dl_0_7, nozle_3_dl_0_8, nozle_3_dl_0_9, nozle_3_dl_0_10, nozle_3_dl_0_11, nozle_3_dl_0_12, nozle_3_dl_0_13, nozle_3_dl_0_14, nozle_3_dl_0_15, nozle_3_dl_0_16};
static u32 nozle_3_dl_1_0[] __attribute__((aligned(32))) = {0x98001100, 0x39000000, 0x12001300, 0x13003800, 0x00001200, 0x12001200, 0x3E000000, 0x11001800, 0x18003D00, 0x00001100, 0x17001700, 0x43000000, 0x13001D00, 0x1D004200, 0x00001300, 0x1C001C00, 0x48000000, 0x14002200, 0x22004700, 0x00001400, 0x21002100, 0x4D000000, 0x15003B00, 0x3B004C00, 0x00001500, 0x3A003A00, 0x52000000, 0x16004000, 0x40005100, 0x00001600, 0x3F003F00, 0x55000000, 0x17004500, 0x45005400, 0x00001700, 0x44004400, 0x69000000, 0x17005800, 0x58005300, 0x00001700, 0x43004300, 0x68000000, 0x18005600, 0x56000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_1[] __attribute__((aligned(32))) = {0x98001100, 0x37000000, 0x12001100, 0x11003600, 0x00001200, 0x10001000, 0x3C000000, 0x11001600, 0x16003B00, 0x00001100, 0x15001500, 0x41000000, 0x13001B00, 0x1B004000, 0x00001300, 0x1A001A00, 0x46000000, 0x14002000, 0x20004500, 0x00001400, 0x1F001F00, 0x4B000000, 0x15003900, 0x39004A00, 0x00001500, 0x38003800, 0x50000000, 0x16003E00, 0x3E004F00, 0x00001600, 0x3D003D00, 0x53000000, 0x17004300, 0x43006000, 0x00001700, 0x42004200, 0x61000000, 0x17004600, 0x46005700, 0x00001700, 0x47004700, 0x58000000, 0x18004900, 0x49000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_2[] __attribute__((aligned(32))) = {0x98000E00, 0x38000000, 0x12001200, 0x12003700, 0x00001200, 0x11001100, 0x3D000000, 0x11001700, 0x17003C00, 0x00001100, 0x16001600, 0x42000000, 0x13001C00, 0x1C004100, 0x00001300, 0x1B001B00, 0x47000000, 0x14002100, 0x21004600, 0x00001400, 0x20002000, 0x4C000000, 0x15003A00, 0x3A004B00, 0x00001500, 0x39003900, 0x51000000, 0x16003F00, 0x3F005000, 0x00001600, 0x3E003E00, 0x54000000, 0x17004400, 0x44005300, 0x00001700, 0x43004300, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_3[] __attribute__((aligned(32))) = {0x98000900, 0x5B000000, 0x1B005200, 0x52005C00, 0x00001A00, 0x53005300, 0x65000000, 0x1B005100, 0x51006600, 0x00001A00, 0x50005000, 0x53000000, 0x17004300, 0x43006700, 0x00001900, 0x54005400, 0x68000000, 0x18005600, 0x56005E00, 0x00001800, 0x57005700, 0x5F000000, 0x17005900, 0x59000000};
static u32 nozle_3_dl_1_4[] __attribute__((aligned(32))) = {0x98000600, 0x65000000, 0x1B004E00, 0x4E005300, 0x00001700, 0x43004300, 0x64000000, 0x1A004C00, 0x4C006300, 0x00001900, 0x4A004A00, 0x5A000000, 0x1A004D00, 0x4D005900, 0x00001900, 0x4B004B00};
static u32 nozle_3_dl_1_5[] __attribute__((aligned(32))) = {0x98000500, 0x5E000000, 0x18005700, 0x57006700, 0x00001900, 0x54005400, 0x5D000000, 0x19005500, 0x55006600, 0x00001A00, 0x50005000, 0x5C000000, 0x1A005300, 0x53000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_6[] __attribute__((aligned(32))) = {0x98000500, 0x63000000, 0x19004A00, 0x4A005300, 0x00001700, 0x43004300, 0x62000000, 0x18004800, 0x48006100, 0x00001700, 0x46004600, 0x58000000, 0x18004900, 0x49000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_7[] __attribute__((aligned(32))) = {0x98000400, 0x58000000, 0x18004900, 0x49005900, 0x00001900, 0x4B004B00, 0x62000000, 0x18004800, 0x48006300, 0x00001900, 0x4A004A00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_8[] __attribute__((aligned(32))) = {0x98000400, 0x5A000000, 0x1A004D00, 0x4D005B00, 0x00001B00, 0x4F004F00, 0x64000000, 0x1A004C00, 0x4C006500, 0x00001B00, 0x4E004E00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_9[] __attribute__((aligned(32))) = {0x98000400, 0x40000000, 0x13001A00, 0x1A003B00, 0x00001100, 0x15001500, 0x3A000000, 0x11001400, 0x14003500, 0x00001200, 0x0F000F00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_10[] __attribute__((aligned(32))) = {0x98000400, 0x45000000, 0x14001F00, 0x1F004000, 0x00001300, 0x1A001A00, 0x3F000000, 0x13001900, 0x19003A00, 0x00001100, 0x14001400, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_11[] __attribute__((aligned(32))) = {0x98000400, 0x4A000000, 0x15003800, 0x38004500, 0x00001400, 0x1F001F00, 0x44000000, 0x14001E00, 0x1E003F00, 0x00001300, 0x19001900, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_12[] __attribute__((aligned(32))) = {0x98000400, 0x4F000000, 0x16003D00, 0x3D004A00, 0x00001500, 0x38003800, 0x49000000, 0x15003700, 0x37004400, 0x00001400, 0x1E001E00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_13[] __attribute__((aligned(32))) = {0x98000400, 0x60000000, 0x17004200, 0x42004F00, 0x00001600, 0x3D003D00, 0x4E000000, 0x16003C00, 0x3C004900, 0x00001500, 0x37003700, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_14[] __attribute__((aligned(32))) = {0x98000400, 0x57000000, 0x17004700, 0x47006000, 0x00001700, 0x42004200, 0x56000000, 0x17004100, 0x41004E00, 0x00001600, 0x3C003C00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_15[] __attribute__((aligned(32))) = {0x98000400, 0x55000000, 0x17004500, 0x45006900, 0x00001700, 0x58005800, 0x5F000000, 0x17005900, 0x59006800, 0x00001800, 0x56005600, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_3_dl_1_16[] __attribute__((aligned(32))) = {0x98000300, 0x3B000000, 0x11001500, 0x15003600, 0x00001200, 0x10001000, 0x35000000, 0x12000F00, 0x0F000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static void* nozle_3_dl_1_tbl[] __attribute__((aligned(32))) = {nozle_3_dl_1_0, nozle_3_dl_1_1, nozle_3_dl_1_2, nozle_3_dl_1_3, nozle_3_dl_1_4, nozle_3_dl_1_5, nozle_3_dl_1_6, nozle_3_dl_1_7, nozle_3_dl_1_8, nozle_3_dl_1_9, nozle_3_dl_1_10, nozle_3_dl_1_11, nozle_3_dl_1_12, nozle_3_dl_1_13, nozle_3_dl_1_14, nozle_3_dl_1_15, nozle_3_dl_1_16};
static s16 nozle_4_vertex_tbl[] __attribute__((aligned(32))) = {
    -23040, 0, 0, -17280, 0, 0,
    -11520, 0, 0, -5760, 0, 0,
    0, 0, 0, 5760, 0, 0,
    11520, 0, 0, 17280, 0, 0,
    23040, 0, 0, -23040, 1280, 0,
    -17280, 1392, 0, -11520, 1280, 0,
    -5760, 1418, 0, 0, 1665, 0,
    5760, 1665, 0, 11520, 1608, 0,
    17280, 1280, 0, 23040, 1280, 0,
    -23040, 2560, 0, -17280, 2812, 0,
    -11520, 2560, 0, -5760, 2698, 0,
    0, 3056, 0, 5760, 3056, 0,
    11520, 2888, 0, 17280, 2560, 0,
    23040, 2560, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
};
static s16 nozle_4_texcoord0_tbl[] __attribute__((aligned(32))) = {
    4096, -22528, 4096, -16384, 0, -16384, 0, -22528,
    4096, -10240, 0, -10240, 4096, -4096, 0, -4096,
    4096, 2048, 0, 2048, 4096, 8192, 0, 8192,
    4096, 14336, 0, 14336, 4096, 20480, 0, 20480,
    4096, 26624, 0, 26624, 8192, -22528, 8192, -16384,
    8192, -10240, 8192, -4096, 8192, 2048, 8192, 8192,
    8192, 14336, 8192, 20480, 8192, 26624, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
static s16 nozle_4_texcoord1_tbl[] __attribute__((aligned(32))) = {
    4096, -12288, 4096, -8192, 0, -8192, 0, -12288,
    4096, -4096, 0, -4096, 4096, 0, 0, 0,
    4096, 4096, 0, 4096, 4096, 8192, 0, 8192,
    4096, 12288, 0, 12288, 4096, 16384, 0, 16384,
    4096, 20480, 0, 20480, 8192, -12288, 8192, -8192,
    8192, -4096, 8192, 0, 8192, 4096, 8192, 8192,
    8192, 12288, 8192, 16384, 8192, 20480, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
static u32 nozle_4_dl_0_0[] __attribute__((aligned(32))) = {0x98000612, 0x00001212, 0x13000013, 0x13090000, 0x00000A00, 0x00010100, 0x00000303, 0x01000002, 0x02000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_4_dl_0_1[] __attribute__((aligned(32))) = {0x98000613, 0x00001313, 0x14000014, 0x140A0000, 0x01010B00, 0x00040401, 0x00000202, 0x02000005, 0x05000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_4_dl_0_2[] __attribute__((aligned(32))) = {0x98000614, 0x00001414, 0x15000015, 0x150B0000, 0x04040C00, 0x00060602, 0x00000505, 0x03000007, 0x07000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_4_dl_0_3[] __attribute__((aligned(32))) = {0x98000615, 0x00001515, 0x16000016, 0x160C0000, 0x06060D00, 0x00080803, 0x00000707, 0x04000009, 0x09000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_4_dl_0_4[] __attribute__((aligned(32))) = {0x98000616, 0x00001616, 0x17000017, 0x170D0000, 0x08080E00, 0x000A0A04, 0x00000909, 0x0500000B, 0x0B000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_4_dl_0_5[] __attribute__((aligned(32))) = {0x98000617, 0x00001717, 0x18000018, 0x180E0000, 0x0A0A0F00, 0x000C0C05, 0x00000B0B, 0x0600000D, 0x0D000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_4_dl_0_6[] __attribute__((aligned(32))) = {0x98000618, 0x00001818, 0x19000119, 0x190F0000, 0x0C0C1000, 0x010E0E06, 0x00000D0D, 0x0700010F, 0x0F000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_4_dl_0_7[] __attribute__((aligned(32))) = {0x98000619, 0x00011919, 0x1A00021A, 0x1A100001, 0x0E0E1100, 0x02101007, 0x00010F0F, 0x08000211, 0x11000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static s8 nozle_1_normal_tbl[] __attribute__((aligned(32))) = {
    0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
static GXColor nozle_1_color0_tbl[] __attribute__((aligned(32))) = {
    {255, 255, 255, 204}, {255, 255, 255, 51}, {255, 255, 255, 76}, {255, 255, 255, 102},
    {255, 255, 255, 0}, {255, 255, 255, 127}, {0, 0, 0, 0}, {0, 0, 0, 0},
};
static u32 nozle_1_dl_0_5[] __attribute__((aligned(32))) = {0x98000401, 0x00011D1D, 0x1C000110, 0x10030000, 0x1C1C1D00, 0x000F0F00, 0x00000000, 0x00000000};
static u32 nozle_1_dl_0_6[] __attribute__((aligned(32))) = {0x9800041C, 0x00011010, 0x0E000111, 0x111D0000, 0x0F0F0F00, 0x000E0E00, 0x00000000, 0x00000000};
static u32 nozle_1_dl_0_7[] __attribute__((aligned(32))) = {0x9800040E, 0x00011111, 0x15000102, 0x020F0000, 0x0E0E1600, 0x00010100, 0x00000000, 0x00000000};
static u32 nozle_1_dl_0_8[] __attribute__((aligned(32))) = {0x9800030C, 0x00040C0C, 0x1B00040D, 0x0D0A0002, 0x0A0A0000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_0_9[] __attribute__((aligned(32))) = {0x98000316, 0x00000101, 0x15000102, 0x02000001, 0x03030000, 0x00000000, 0x00000000, 0x00000000};
static void* nozle_1_dl_0_tbl[] __attribute__((aligned(32))) = {nozle_1_dl_0_0, nozle_1_dl_0_1, nozle_1_dl_0_2, nozle_1_dl_0_3, nozle_1_dl_0_4, nozle_1_dl_0_5, nozle_1_dl_0_6, nozle_1_dl_0_7, nozle_1_dl_0_8, nozle_1_dl_0_9};
static u8 nozle_1_dl_0_size_tbl[] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01};
static u32 nozle_1_dl_1_5[] __attribute__((aligned(32))) = {0x98000424, 0x00011F1F, 0x3F000114, 0x14260000, 0x20204000, 0x00161600, 0x00000000, 0x00000000};
static u32 nozle_1_dl_1_6[] __attribute__((aligned(32))) = {0x9800043F, 0x00011414, 0x31000115, 0x15400000, 0x16163200, 0x00171700, 0x00000000, 0x00000000};
static u32 nozle_1_dl_1_7[] __attribute__((aligned(32))) = {0x98000431, 0x00011515, 0x38000107, 0x07320000, 0x17173900, 0x00090900, 0x00000000, 0x00000000};
static u32 nozle_1_dl_1_8[] __attribute__((aligned(32))) = {0x9800032F, 0x00042727, 0x3E000428, 0x282D0002, 0x25250000, 0x00000000, 0x00000000, 0x00000000};
static u32 nozle_1_dl_1_9[] __attribute__((aligned(32))) = {0x98000339, 0x00000909, 0x38000107, 0x07230001, 0x06060000, 0x00000000, 0x00000000, 0x00000000};
static void* nozle_1_dl_1_tbl[] __attribute__((aligned(32))) = {nozle_1_dl_1_0, nozle_1_dl_1_1, nozle_1_dl_1_2, nozle_1_dl_1_3, nozle_1_dl_1_4, nozle_1_dl_1_5, nozle_1_dl_1_6, nozle_1_dl_1_7, nozle_1_dl_1_8, nozle_1_dl_1_9};
static u8 nozle_1_dl_1_size_tbl[] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01};
static s8 nozle_3_normal_tbl[] __attribute__((aligned(32))) = {
    0, 0, 64, 0,
};
static u8 nozle_3_dl_0_size_tbl[] = {0x06, 0x06, 0x05, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
static u8 nozle_3_dl_1_size_tbl[] = {0x06, 0x06, 0x05, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
static s8 nozle_4_normal_tbl[] __attribute__((aligned(32))) = {
    0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
static GXColor nozle_4_color0_tbl[] __attribute__((aligned(32))) = {
    {255, 255, 255, 127}, {255, 255, 255, 76}, {255, 255, 255, 0},
};
static void* nozle_4_dl_0_tbl[] = {nozle_4_dl_0_0, nozle_4_dl_0_1, nozle_4_dl_0_2, nozle_4_dl_0_3, nozle_4_dl_0_4, nozle_4_dl_0_5, nozle_4_dl_0_6, nozle_4_dl_0_7};
static u8 nozle_4_dl_0_size_tbl[] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
static u8 scale_dt[][2] = {{0, 0}, {11, 13}, {22, 27}, {34, 41}, {46, 55}, {57, 69}, {68, 82}, {79, 94}, {87, 105}, {94, 113}, {100, 120}, {102, 123}, {103, 124}, {101, 123}, {98, 120}, {95, 116}, {90, 111}, {86, 107}, {83, 103}, {81, 101}, {80, 100}, {0, 0}};

EffectEntry* effNozleEntry(s32 type, f32 x, f32 y, f32 z, f32 rx, f32 ry, s32 life) {
    EffectEntry* effect;
    EffNozleWork* work;
    EffNozleWork* p;
    s32 count;
    s32 i;

    if (type == 1) {
        return effKemuTestEntry(3, x, y, z, 1.0f);
    }
    effect = effEntry();
    count = 1;
    switch (type) {
        case 0:
        case 1:
        case 2:
            count = 1;
            break;
        case 3:
            count = 4;
            break;
    }
    effect->type = "Nozle";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffNozleWork));
    effect->userdata = work;
    effect->callback = effNozleMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->sx = type == 3 ? 1.0f : 0.0f;
    work->sy = type == 3 ? 1.0f : 0.0f;
    work->scale = 1.0f;
    work->u1 = work->v1 = work->u0 = work->v0 = 0.0f;
    if (life <= 0) {
        work->life = 1000;
    } else {
        work->life = life;
    }
    work->timer = 0;
    work->alpha = 255;
    work->rotX = rx;
    work->rotY = ry;
    for (i = 1, p = work + 1; i < effect->count; i++, p++) {
        p->position.x = (i & 1 ? -1 : 1) * 400;
        p->position.y = 0.0f;
        p->position.z = 20.0f * i;
        p->sx = (i & 1 ? 1 : -1);
        p->sy = 1.0f;
        p->u1 = p->v1 = p->u0 = p->v0 = 0.0f;
        p->alpha = 255;
        p->flag = 0;
    }
    return effect;
}

void effNozleMain(EffectEntry* effect) {
    EffNozleWork* work = effect->userdata;
    EffNozleWork* p;
    Vec position;
    Vec v = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;

    v.x = work->position.x;
    v.y = work->position.y;
    v.z = work->position.z;
    position = v;
    type = work->type;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->life = 32;
    }
    switch (type) {
        case 0:
        case 2:
            if (work->timer < 21U) {
                work->sx = scale_dt[work->timer][0] / 100.0f;
                work->sy = scale_dt[work->timer][1] / 100.0f;
            }
            if (work->life < 21U) {
                work->sx = scale_dt[work->life][0] / 100.0f;
                work->sy = scale_dt[work->life][1] / 100.0f;
            }
            work->v0 += -0.041666668f;
            work->v1 += -0.083333336f;
            break;
        case 3:
            p = effect->userdata;
            p++;
            for (i = 1; i < effect->count; i++, p++) {
                p->position.x += -p->position.x / 300.0f;
                if (p->flag == 0) {
                    p->sy += (1.3f - p->sy) / 500.0f;
                    if (p->sy > 1.2f) {
                        p->flag = 1;
                    }
                } else {
                    p->sy += (0.8f - p->sy) / 300.0f;
                }
                p->v0 += 0.002f;
                p->u1 += 0.002f;
                p->v1 += 0.004f;
            }
            break;
    }
    work = effect->userdata;
    if (work->life < 1000) {
        work->life--;
    }
    if (work->life < 16) {
        work->alpha = work->life * 16;
    }
    work->timer++;
    if (work->life < 0) {
        effDelete(effect);
    } else if (type == 3) {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 8, (DispCallback)effNozleDisp, effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effNozleDisp, effect);
    }
}

// Indexed vertex arrays of one nozzle model: type = index size, posFrac/texFrac = fixed-point fraction bits.
static inline void nozleVtxSetup(GXAttrType type, u8 posFrac, u8 texFrac, void* vtx, void* nrm, void* clr, void* tex0, void* tex1) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, type);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, posFrac);
    GXSetArray(GX_VA_POS, vtx, 6);
    GXSetVtxDesc(GX_VA_NRM, type);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
    GXSetArray(GX_VA_NRM, nrm, 3);
    GXSetVtxDesc(GX_VA_CLR0, type);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetArray(GX_VA_CLR0, clr, 4);
    GXSetVtxDesc(GX_VA_TEX0, type);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, texFrac);
    GXSetArray(GX_VA_TEX0, tex0, 4);
    GXSetVtxDesc(GX_VA_TEX1, type);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_S16, texFrac);
    GXSetArray(GX_VA_TEX1, tex1, 4);
}

// Call a table of n display lists through the caller's k and the given size/list pointers
#define CALLDL(n, tbl, sz_, S, D)                           \
    {                                                       \
        for (k = 0, S = sz_, D = tbl; k < n; k++, S++, D++) {\
            GXCallDisplayList(*D, *S * 32);                 \
        }                                                   \
    }

void effNozleDisp(CameraId cameraId, EffectEntry* effect) {
    void** dl5;
    u8* sz5;
    u8* sz3;
    void** dl3;
    u8* sz4;
    void** dl4;
    u8* sz2;
    void** dl2;
    void** dl;
    u8* sz;
    u32 k;
    EffNozleWork* w;
    s32 i;
    s32 alpha;
    EffNozleWork* work = effect->userdata;
    Mtx a;
    Mtx b;
    Mtx c;
    Mtx d;
    GXTexObj texObj;
    CameraEntry* cam;
    cam = camGetPtr(cameraId);
    alpha = work->alpha;
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(1, GX_LEQUAL, 0);
    PSMTXTrans(a, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(b, 'y', 0.017453292f * work->rotY);
    PSMTXConcat(a, b, a);
    PSMTXRotRad(b, 'x', 0.017453292f * work->rotX);
    PSMTXConcat(a, b, a);
    PSMTXScale(c, work->scale * work->sx, work->scale * work->sy, work->scale);
    PSMTXConcat(a, c, a);
    PSMTXConcat(cam->view, a, d);
    GXLoadPosMtxImm(d, 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 30, 0, 125);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 33, 0, 125);
    PSMTXTrans(a, work->u0, -work->v0, 0.0f);
    GXLoadTexMtxImm(a, 30, GX_MTX2x4);
    PSMTXTrans(a, work->u1, -work->v1, 0.0f);
    GXLoadTexMtxImm(a, 33, GX_MTX2x4);
    effGetTexObj(60, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    switch (work->type) {
        case 0:
        case 3:
            effGetTexObj(61, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP1);
            break;
        case 2:
            effGetTexObj(62, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP1);
            break;
    }
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    switch (work->type) {
        case 0:
            nozleVtxSetup(GX_INDEX8, 7, 14, nozle_1_vertex_tbl, nozle_1_normal_tbl, nozle_1_color0_tbl, nozle_1_texcoord0_tbl, nozle_1_texcoord1_tbl);
            { for (k = 0, sz = nozle_1_dl_0_size_tbl, dl = nozle_1_dl_0_tbl; k < 10; k++, sz++, dl++) { GXCallDisplayList(*dl, *sz * 32); } }
            nozleVtxSetup(GX_INDEX8, 7, 14, nozle_1_vertex_tbl, nozle_1_normal_tbl, nozle_1_color0_tbl, nozle_1_texcoord0_tbl, nozle_1_texcoord1_tbl);
            { for (k = 0, sz2 = nozle_1_dl_1_size_tbl, dl2 = nozle_1_dl_1_tbl; k < 10; k++, sz2++, dl2++) { GXCallDisplayList(*dl2, *sz2 * 32); } }
            break;
        case 2:
            nozleVtxSetup(GX_INDEX16, 7, 14, nozle_3_vertex_tbl, nozle_3_normal_tbl, nozle_3_color0_tbl, nozle_3_texcoord0_tbl, nozle_3_texcoord1_tbl);
            CALLDL(17, nozle_3_dl_1_tbl, nozle_3_dl_1_size_tbl, sz5, dl5);
            nozleVtxSetup(GX_INDEX16, 7, 14, nozle_3_vertex_tbl, nozle_3_normal_tbl, nozle_3_color0_tbl, nozle_3_texcoord0_tbl, nozle_3_texcoord1_tbl);
            CALLDL(17, nozle_3_dl_0_tbl, nozle_3_dl_0_size_tbl, sz3, dl3);
            break;
        case 3:
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, alpha});
            w = effect->userdata;
            for (i = 1, w++; i < effect->count; i++, w++) {
                PSMTXTrans(a, w->u0, -w->v0, 0.0f);
                GXLoadTexMtxImm(a, 30, GX_MTX2x4);
                PSMTXTrans(a, w->u1, -w->v1, 0.0f);
                GXLoadTexMtxImm(a, 33, GX_MTX2x4);
                PSMTXTrans(a, w->position.x, w->position.y, w->position.z);
                PSMTXScale(c, w->sx, w->sy, 1.0f);
                PSMTXConcat(a, c, c);
                PSMTXConcat(d, c, c);
                GXLoadPosMtxImm(c, 0);
                nozleVtxSetup(GX_INDEX8, 6, 13, nozle_4_vertex_tbl, nozle_4_normal_tbl, nozle_4_color0_tbl, nozle_4_texcoord0_tbl, nozle_4_texcoord1_tbl);
                CALLDL(8, nozle_4_dl_0_tbl, nozle_4_dl_0_size_tbl, sz4, dl4);
            }
            break;
    }
}
