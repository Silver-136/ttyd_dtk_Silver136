#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/mapdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double);

typedef struct EffBoobooN64Work {
    s32 unk0;         // 0x0
    s32 time;         // 0x4
    u32 flags[11];    // 0x8
    f32 x[11];        // 0x34
    f32 y[11];        // 0x60
    f32 z[11];        // 0x8C
    f32 rot[11];      // 0xB8
    u8 type[11];      // 0xE4
    u8 nextState[11]; // 0xEF
    u8 state[11];     // 0xFA
    u8 anim[11];      // 0x105
    f32 yoff[11];     // 0x110
    s8 frame[11];     // 0x13C
} EffBoobooN64Work;

#ifdef __MWERKS__
#endif

s16 boo_vertex_tbl[] __attribute__((aligned(32))) = {
    6474, 32221, -10223, 6474, 32221, 8093,
    14568, 19169, 10223, 14568, 19169, -12353,
    -6474, 32221, -10223, -6474, 32221, 8093,
    -14568, 19169, -12353, -14568, 19169, 10223,
    -8519, 6116, -12353, -8519, 6116, 10223,
    8519, 6116, 10223, 8519, 6116, -12353,
    4046, 23565, 18743, 7284, 17039, 18743,
    -4046, 23565, 18743, -7284, 17039, 18743,
    -4259, 12642, 18743, 4259, 12642, 18743,
    7284, 17039, 23003, 4046, 23565, 23003,
    -4046, 23565, 23003, -7284, 17039, 23003,
    -4259, 12642, 23003, 4259, 12642, 23003,
    4259, 12642, -18743, -4259, 12642, -18743,
    -7284, 17039, -18743, -4046, 23565, -18743,
    4046, 23565, -18743, 7284, 17039, -18743,
    -8519, 6116, -5537, -5679, -273, -2392,
    -2839, 6116, -12353, 8519, 6116, -5537,
    5679, -273, -2392, 2839, 6116, -12353,
    8519, 6116, 3407, 5679, -273, 1900,
    2839, 6116, 10223, -2839, 6116, 10223,
    -5679, -273, 1900, -8519, 6116, 3407,
    -6389, 27688, 14909, -6389, 25559, 14909,
    -4883, 25559, 16415, -4883, 27688, 16415,
    6389, 27688, 14909, 4883, 27688, 16415,
    4883, 25559, 16415, 6389, 25559, 14909,
    6474, 32221, -10223, 6474, 32221, 8093,
    14568, 19169, 10223, 14568, 19169, -12353,
    -6474, 32221, -10223, -6474, 32221, 8093,
    -14568, 19169, -12353, -14568, 19169, 10223,
    -8519, 6116, -12353, -8519, 6116, 10223,
    8519, 6116, 10223, 8519, 6116, -12353,
    4046, 23565, 18743, 7284, 17039, 18743,
    -4046, 23565, 18743, -7284, 17039, 18743,
    -4259, 12642, 18743, 4259, 12642, 18743,
    7284, 17039, 23003, 4046, 23565, 23003,
    -4046, 23565, 23003, -7284, 17039, 23003,
    -4259, 12642, 23003, 4259, 12642, 23003,
    4259, 12642, -18743, -4259, 12642, -18743,
    -7284, 17039, -18743, -4046, 23565, -18743,
    4046, 23565, -18743, 7284, 17039, -18743,
    -8519, 6116, -5537, -5679, -273, -8945,
    -2839, 6116, -12353, 8519, 6116, -5537,
    5679, -273, -8945, 2839, 6116, -12353,
    8519, 6116, 3407, 5679, -273, 6815,
    2839, 6116, 10223, -2839, 6116, 10223,
    -5679, -273, 6815, -8519, 6116, 3407,
    -6389, 27688, 14909, -6389, 25559, 14909,
    -4883, 25559, 16415, -4883, 27688, 16415,
    6389, 27688, 14909, 4883, 27688, 16415,
    4883, 25559, 16415, 6389, 25559, 14909,
    6474, 32221, -10223, 6474, 32221, 8093,
    14568, 19169, 10223, 14568, 19169, -12353,
    -6474, 32221, -10223, -6474, 32221, 8093,
    -14568, 19169, -12353, -14568, 19169, 10223,
    -8519, 6116, -12353, -8519, 6116, 10223,
    8519, 6116, 10223, 8519, 6116, -12353,
    4046, 23565, 18743, 7284, 17039, 18743,
    -4046, 23565, 18743, -7284, 17039, 18743,
    -4259, 12642, 18743, 4259, 12642, 18743,
    7284, 17039, 23003, 4046, 23565, 23003,
    -4046, 23565, 23003, -7284, 17039, 23003,
    -4259, 12642, 23003, 4259, 12642, 23003,
    4259, 12642, -18743, -4259, 12642, -18743,
    -7284, 17039, -18743, -4046, 23565, -18743,
    4046, 23565, -18743, 7284, 17039, -18743,
    -8519, 6116, -5537, -5679, 3003, -13860,
    -2839, 6116, -12353, 8519, 6116, -5537,
    5679, 3003, -13860, 2839, 6116, -12353,
    8519, 6116, 3407, 5679, 1365, 11730,
    2839, 6116, 10223, -2839, 6116, 10223,
    -5679, 1365, 11730, -8519, 6116, 3407,
    -6389, 27688, 14909, -6389, 25559, 14909,
    -4883, 25559, 16415, -4883, 27688, 16415,
    6389, 27688, 14909, 4883, 27688, 16415,
    4883, 25559, 16415, 6389, 25559, 14909,
};
s8 boo_normal_tbl[] __attribute__((aligned(32))) = {
    58, 1, -25, 60, 1, 21, 31, 51, 21, 39, 39, -31,
    0, 64, 0, -27, 52, 23, -12, 51, -35, -54, 33, 0,
    -62, -6, 13, -57, 4, -27, -58, -26, 0, 37, -37, -36,
    58, -26, 0, 55, -15, 28, 19, 54, 28, -14, 54, 30,
    -59, 0, 23, -38, 30, 40, -48, -22, 35, -17, -38, 48,
    -20, -58, 15, 27, -52, 23, 14, -38, 48, 0, 0, 64,
    30, 0, -56, 17, 25, -55, -12, 26, -56, -28, 3, -57,
    -16, -24, -56, 12, -25, -57, -15, -34, -51, -58, -25, 0,
    -34, -53, 0, 0, -30, -56, 0, -54, -32, 58, -25, 0,
    34, -53, 0, 0, -30, 56, 0, -54, 32, 57, 28, 0,
    63, -4, 0, 33, 54, 0, -33, 54, 0, -57, 28, 0,
    -63, -4, 0, -29, -56, 0, 29, -56, 0, 49, 0, 40,
    49, 0, -40, -49, 0, -40, -49, 0, 40, -45, 0, 45,
    45, 0, 45,
};
s16 boo_texcoord0_tbl[] __attribute__((aligned(32))) = {
    2169, 17000, 11200, 7969, 12958, 19263, 5631, 26590,
    8589, -2472, -441, 6557, 14181, 2857, 15487, 8078,
    13302, -659, 15006, -2363, 15885, 1153, 17191, 6374,
    -812, 17852, 492, 23073, -1691, 14334, -356, 83,
    2285, 3831, 5948, -6221, 5863, 253, 15380, 12308,
    14954, 10604, 14352, 11206, 14778, 12910,
};
/* GX display lists (GX_TRIANGLESTRIP 0x98 with u8 position/normal/texcoord indices, 32-byte padded), drawn with GXCallDisplayList */
u32 boo_dl_0_0[] __attribute__((aligned(32))) = {0x98000A0E, 0x2B020714, 0x2A050B0F, 0x10020615, 0x2C050A10, 0x14020816, 0x2D050911, 0x15020817, 0x2E05090D, 0x0D020612, 0x28050A00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_1[] __attribute__((aligned(32))) = {0x98000902, 0x01000100, 0x03000303, 0x0000001D, 0x18020C18, 0x1D020E1C, 0x19020D1B, 0x1A020D04, 0x06000306, 0x09000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_2[] __attribute__((aligned(32))) = {0x98000803, 0x00000018, 0x1D020E0B, 0x0B010519, 0x1C020E08, 0x1E01051A, 0x1B020C06, 0x0900001B, 0x1A020D00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_3[] __attribute__((aligned(32))) = {0x98000824, 0x24011221, 0x24011026, 0x26010423, 0x22010527, 0x26010420, 0x22010529, 0x2001121E, 0x20011000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_0[] __attribute__((aligned(32))) = {0x98000A40, 0x2B020746, 0x2A050B41, 0x10020647, 0x2C050A42, 0x14020848, 0x2D050943, 0x15020849, 0x2E05093F, 0x0D020644, 0x28050A00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_1[] __attribute__((aligned(32))) = {0x98000934, 0x01000132, 0x03000335, 0x0000004F, 0x18020C4A, 0x1D020E4E, 0x19020D4D, 0x1A020D36, 0x06000338, 0x09000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_2[] __attribute__((aligned(32))) = {0x98000835, 0x0000004A, 0x1D020E3D, 0x0B01054B, 0x1C020E3A, 0x1E01054C, 0x1B020C38, 0x0900004D, 0x1A020D00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_3[] __attribute__((aligned(32))) = {0x98000856, 0x24011253, 0x24011058, 0x26010455, 0x22010559, 0x26010452, 0x2201055B, 0x20011250, 0x20011000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_0[] __attribute__((aligned(32))) = {0x98000A72, 0x2B020778, 0x2A050B73, 0x10020679, 0x2C050A74, 0x1402087A, 0x2D050975, 0x1502087B, 0x2E050971, 0x0D020676, 0x28050A00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_1[] __attribute__((aligned(32))) = {0x98000966, 0x01000164, 0x03000367, 0x00000081, 0x18020C7C, 0x1D020E80, 0x19020D7F, 0x1A020D68, 0x0600036A, 0x09000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_2[] __attribute__((aligned(32))) = {0x98000867, 0x0000007C, 0x1D020E6F, 0x0B01057D, 0x1C020E6C, 0x1E01057E, 0x1B020C6A, 0x0900007F, 0x1A020D00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_3[] __attribute__((aligned(32))) = {0x98000888, 0x24011285, 0x2401108A, 0x26010487, 0x2201058B, 0x26010484, 0x2201058D, 0x20011282, 0x20011000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
GXColor boo_color0_tbl[] __attribute__((aligned(32))) = {
    {255, 255, 255, 255}, {255, 180, 69, 255}, {255, 192, 151, 255}, {255, 160, 96, 255},
    {254, 202, 124, 255}, {255, 191, 96, 255}, {215, 129, 0, 255}, {0, 0, 0, 255},
};
u32 boo_dl_0_4[] __attribute__((aligned(32))) = {0x98000614, 0x17030B13, 0x17030B15, 0x17030A12, 0x17030A16, 0x17030917, 0x17030900, 0x00000000};
u32 boo_dl_0_5[] __attribute__((aligned(32))) = {0x98000604, 0x07000305, 0x05000206, 0x09000007, 0x08000108, 0x0A010509, 0x0A010400, 0x00000000};
u32 boo_dl_0_6[] __attribute__((aligned(32))) = {0x9800050D, 0x0D02060C, 0x0E020702, 0x01000101, 0x02000200, 0x03000300, 0x00000000, 0x00000000};
u32 boo_dl_0_7[] __attribute__((aligned(32))) = {0x9800041A, 0x1B020C19, 0x1C020E1B, 0x1A020D18, 0x1D020E00, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_8[] __attribute__((aligned(32))) = {0x98000402, 0x01000103, 0x0000000A, 0x0C01040B, 0x0B010500, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_9[] __attribute__((aligned(32))) = {0x98000404, 0x0600031C, 0x19020D00, 0x0300031D, 0x18020C00, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_10[] __attribute__((aligned(32))) = {0x98000411, 0x1502080D, 0x0D02060A, 0x16010402, 0x01000100, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_11[] __attribute__((aligned(32))) = {0x98000410, 0x14020811, 0x15020809, 0x1301040A, 0x16010400, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_12[] __attribute__((aligned(32))) = {0x98000409, 0x13010407, 0x08000110, 0x1202080F, 0x10020600, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_13[] __attribute__((aligned(32))) = {0x98000405, 0x05000201, 0x0200020E, 0x0F02070C, 0x0E020700, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_14[] __attribute__((aligned(32))) = {0x98000407, 0x11000105, 0x0500020F, 0x1002060E, 0x0F020700, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_15[] __attribute__((aligned(32))) = {0x98000400, 0x04000301, 0x02000204, 0x06000305, 0x05000200, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_16[] __attribute__((aligned(32))) = {0x9800042F, 0x3407132E, 0x34071630, 0x34071431, 0x34071500, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_17[] __attribute__((aligned(32))) = {0x9800042B, 0x3307152A, 0x3307162C, 0x3307142D, 0x33071300, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_18[] __attribute__((aligned(32))) = {0x98000414, 0x2A050B0E, 0x0F020713, 0x29050B0C, 0x0E020700, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_19[] __attribute__((aligned(32))) = {0x98000413, 0x29050B0C, 0x27020712, 0x28050A0D, 0x27020600, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_20[] __attribute__((aligned(32))) = {0x98000323, 0x32060521, 0x32061022, 0x32060F00, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_21[] __attribute__((aligned(32))) = {0x98000324, 0x31061226, 0x31060425, 0x31061100, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_22[] __attribute__((aligned(32))) = {0x98000327, 0x30060429, 0x30061228, 0x30061100, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_23[] __attribute__((aligned(32))) = {0x9800031E, 0x2F061020, 0x2F06051F, 0x2F060F00, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_24[] __attribute__((aligned(32))) = {0x9800031F, 0x1F040F08, 0x0A01051E, 0x20011000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_25[] __attribute__((aligned(32))) = {0x9800031F, 0x21040F20, 0x22010508, 0x1E010500, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_26[] __attribute__((aligned(32))) = {0x98000322, 0x23040F21, 0x2401100B, 0x0B010500, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_27[] __attribute__((aligned(32))) = {0x98000322, 0x21040F0B, 0x0B010523, 0x22010500, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_28[] __attribute__((aligned(32))) = {0x98000325, 0x2304110A, 0x0C010424, 0x24011200, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_29[] __attribute__((aligned(32))) = {0x98000328, 0x1F041129, 0x20011209, 0x0A010400, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_30[] __attribute__((aligned(32))) = {0x98000328, 0x25041109, 0x13010427, 0x26010400, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_0_31[] __attribute__((aligned(32))) = {0x98000325, 0x25041126, 0x2601040A, 0x16010400, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_4[] __attribute__((aligned(32))) = {0x98000646, 0x17030B45, 0x17030B47, 0x17030A44, 0x17030A48, 0x17030949, 0x17030900, 0x00000000};
u32 boo_dl_1_5[] __attribute__((aligned(32))) = {0x98000636, 0x07000337, 0x05000238, 0x09000039, 0x0800013A, 0x0A01053B, 0x0A010400, 0x00000000};
u32 boo_dl_1_6[] __attribute__((aligned(32))) = {0x9800053F, 0x0D02063E, 0x0E020734, 0x01000133, 0x02000232, 0x03000300, 0x00000000, 0x00000000};
u32 boo_dl_1_7[] __attribute__((aligned(32))) = {0x9800044C, 0x1B020C4B, 0x1C020E4D, 0x1A020D4A, 0x1D020E00, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_8[] __attribute__((aligned(32))) = {0x98000434, 0x01000135, 0x0000003C, 0x0C01043D, 0x0B010500, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_9[] __attribute__((aligned(32))) = {0x98000436, 0x0600034E, 0x19020D32, 0x0300034F, 0x18020C00, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_10[] __attribute__((aligned(32))) = {0x98000443, 0x1502083F, 0x0D02063C, 0x16010434, 0x01000100, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_11[] __attribute__((aligned(32))) = {0x98000442, 0x14020843, 0x1502083B, 0x1301043C, 0x16010400, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_12[] __attribute__((aligned(32))) = {0x9800043B, 0x13010439, 0x08000142, 0x12020841, 0x10020600, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_13[] __attribute__((aligned(32))) = {0x98000437, 0x05000233, 0x02000240, 0x0F02073E, 0x0E020700, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_14[] __attribute__((aligned(32))) = {0x98000439, 0x11000137, 0x05000241, 0x10020640, 0x0F020700, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_15[] __attribute__((aligned(32))) = {0x98000432, 0x04000333, 0x02000236, 0x06000337, 0x05000200, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_16[] __attribute__((aligned(32))) = {0x98000461, 0x34071360, 0x34071662, 0x34071463, 0x34071500, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_17[] __attribute__((aligned(32))) = {0x9800045D, 0x3307155C, 0x3307165E, 0x3307145F, 0x33071300, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_18[] __attribute__((aligned(32))) = {0x98000446, 0x2A050B40, 0x0F020745, 0x29050B3E, 0x0E020700, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_19[] __attribute__((aligned(32))) = {0x98000445, 0x29050B3E, 0x27020744, 0x28050A3F, 0x27020600, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_20[] __attribute__((aligned(32))) = {0x98000355, 0x32060553, 0x32061054, 0x32060F00, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_21[] __attribute__((aligned(32))) = {0x98000356, 0x31061258, 0x31060457, 0x31061100, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_22[] __attribute__((aligned(32))) = {0x98000359, 0x3006045B, 0x3006125A, 0x30061100, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_23[] __attribute__((aligned(32))) = {0x98000350, 0x2F061052, 0x2F060551, 0x2F060F00, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_24[] __attribute__((aligned(32))) = {0x98000351, 0x1F040F3A, 0x0A010550, 0x20011000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_25[] __attribute__((aligned(32))) = {0x98000351, 0x21040F52, 0x2201053A, 0x1E010500, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_26[] __attribute__((aligned(32))) = {0x98000354, 0x23040F53, 0x2401103D, 0x0B010500, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_27[] __attribute__((aligned(32))) = {0x98000354, 0x21040F3D, 0x0B010555, 0x22010500, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_28[] __attribute__((aligned(32))) = {0x98000357, 0x2304113C, 0x0C010456, 0x24011200, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_29[] __attribute__((aligned(32))) = {0x9800035A, 0x1F04115B, 0x2001123B, 0x0A010400, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_30[] __attribute__((aligned(32))) = {0x9800035A, 0x2504113B, 0x13010459, 0x26010400, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_1_31[] __attribute__((aligned(32))) = {0x98000357, 0x25041158, 0x2601043C, 0x16010400, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_4[] __attribute__((aligned(32))) = {0x98000678, 0x17030B77, 0x17030B79, 0x17030A76, 0x17030A7A, 0x1703097B, 0x17030900, 0x00000000};
u32 boo_dl_2_5[] __attribute__((aligned(32))) = {0x98000668, 0x07000369, 0x0500026A, 0x0900006B, 0x0800016C, 0x0A01056D, 0x0A010400, 0x00000000};
u32 boo_dl_2_6[] __attribute__((aligned(32))) = {0x98000571, 0x0D020670, 0x0E020766, 0x01000165, 0x02000264, 0x03000300, 0x00000000, 0x00000000};
u32 boo_dl_2_7[] __attribute__((aligned(32))) = {0x9800047E, 0x1B020C7D, 0x1C020E7F, 0x1A020D7C, 0x1D020E00, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_8[] __attribute__((aligned(32))) = {0x98000466, 0x01000167, 0x0000006E, 0x0C01046F, 0x0B010500, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_9[] __attribute__((aligned(32))) = {0x98000468, 0x06000380, 0x19020D64, 0x03000381, 0x18020C00, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_10[] __attribute__((aligned(32))) = {0x98000475, 0x15020871, 0x0D02066E, 0x16010466, 0x01000100, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_11[] __attribute__((aligned(32))) = {0x98000474, 0x14020875, 0x1502086D, 0x1301046E, 0x16010400, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_12[] __attribute__((aligned(32))) = {0x9800046D, 0x1301046B, 0x08000174, 0x12020873, 0x10020600, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_13[] __attribute__((aligned(32))) = {0x98000469, 0x05000265, 0x02000272, 0x0F020770, 0x0E020700, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_14[] __attribute__((aligned(32))) = {0x9800046B, 0x11000169, 0x05000273, 0x10020672, 0x0F020700, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_15[] __attribute__((aligned(32))) = {0x98000464, 0x04000365, 0x02000268, 0x06000369, 0x05000200, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_16[] __attribute__((aligned(32))) = {0x98000493, 0x34071392, 0x34071694, 0x34071495, 0x34071500, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_17[] __attribute__((aligned(32))) = {0x9800048F, 0x3307158E, 0x33071690, 0x33071491, 0x33071300, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_18[] __attribute__((aligned(32))) = {0x98000478, 0x2A050B72, 0x0F020777, 0x29050B70, 0x0E020700, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_19[] __attribute__((aligned(32))) = {0x98000477, 0x29050B70, 0x27020776, 0x28050A71, 0x27020600, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_20[] __attribute__((aligned(32))) = {0x98000387, 0x32060585, 0x32061086, 0x32060F00, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_21[] __attribute__((aligned(32))) = {0x98000388, 0x3106128A, 0x31060489, 0x31061100, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_22[] __attribute__((aligned(32))) = {0x9800038B, 0x3006048D, 0x3006128C, 0x30061100, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_23[] __attribute__((aligned(32))) = {0x98000382, 0x2F061084, 0x2F060583, 0x2F060F00, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_24[] __attribute__((aligned(32))) = {0x98000383, 0x1F040F6C, 0x0A010582, 0x20011000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_25[] __attribute__((aligned(32))) = {0x98000383, 0x21040F84, 0x2201056C, 0x1E010500, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_26[] __attribute__((aligned(32))) = {0x98000386, 0x23040F85, 0x2401106F, 0x0B010500, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_27[] __attribute__((aligned(32))) = {0x98000386, 0x21040F6F, 0x0B010587, 0x22010500, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_28[] __attribute__((aligned(32))) = {0x98000389, 0x2304116E, 0x0C010488, 0x24011200, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_29[] __attribute__((aligned(32))) = {0x9800038C, 0x1F04118D, 0x2001126D, 0x0A010400, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_30[] __attribute__((aligned(32))) = {0x9800038C, 0x2504116D, 0x1301048B, 0x26010400, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u32 boo_dl_2_31[] __attribute__((aligned(32))) = {0x98000389, 0x2504118A, 0x2601046E, 0x16010400, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
u8 anime_data_walk[] = {1, 1, 2, 2, 1, 1, 0, 0};

#define BOO_ARRAYS()                                                                                       \
    GXClearVtxDesc();                                                                                      \
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);                                                                    \
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 14);                                        \
    GXSetArray(GX_VA_POS, boo_vertex_tbl, 6);                                                              \
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);                                                                    \
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);                                          \
    GXSetArray(GX_VA_NRM, boo_normal_tbl, 3);                                                              \
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);                                                                   \
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);                                     \
    GXSetArray(GX_VA_CLR0, boo_color0_tbl, 4);                                                             \
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);                                                                   \
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 14);                                        \
    GXSetArray(GX_VA_TEX0, boo_texcoord0_tbl, 4);

#define DL(n, sz) GXCallDisplayList(n, sz);
#define DL4(a, b, c, d) DL(a, 0x40) DL(b, 0x40) DL(c, 0x40) DL(d, 0x40)
#define DLS(p, a) DL(p##a, 0x20)
#define DLS_ALL(p)                                                                                         \
    DLS(p, 4) DLS(p, 5) DLS(p, 6) DLS(p, 7) DLS(p, 8) DLS(p, 9) DLS(p, 10) DLS(p, 11) DLS(p, 12)          \
    DLS(p, 13) DLS(p, 14) DLS(p, 15) DLS(p, 16) DLS(p, 17) DLS(p, 18) DLS(p, 19) DLS(p, 20) DLS(p, 21)    \
    DLS(p, 22) DLS(p, 23) DLS(p, 24) DLS(p, 25) DLS(p, 26) DLS(p, 27) DLS(p, 28) DLS(p, 29) DLS(p, 30)    \
    DLS(p, 31)

void boo_polygon_0(void) {
    BOO_ARRAYS()
    DL4(boo_dl_0_0, boo_dl_0_1, boo_dl_0_2, boo_dl_0_3)
    DLS_ALL(boo_dl_0_)
}

void boo_polygon_1(void) {
    BOO_ARRAYS()
    DL4(boo_dl_1_0, boo_dl_1_1, boo_dl_1_2, boo_dl_1_3)
    DLS_ALL(boo_dl_1_)
}

void boo_polygon_2(void) {
    BOO_ARRAYS()
    DL4(boo_dl_2_0, boo_dl_2_1, boo_dl_2_2, boo_dl_2_3)
    DLS_ALL(boo_dl_2_)
}

void effBoobooMain(EffectEntry* effect);
void effBoobooDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effBoobooN64Entry(void) {
    EffectEntry* effect;
    EffBoobooN64Work* work;
    s32 i;

    effect = effEntry();
    effect->type = "BoobooN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffBoobooN64Work));
    effect->userdata = work;
    effect->callback = effBoobooMain;
    work->time = 0;
    work->unk0 = 1000;
    for (i = 0; i < 11; i++) {
        work->x[i] = 0.0f;
        work->y[i] = 0.0f;
        work->z[i] = 0.0f;
        work->rot[i] = 0.0f;
        work->flags[i] = 0;
        work->state[i] = 1;
        work->anim[i] = 1;
        work->yoff[i] = 0.0f;
        work->frame[i] = 0;
    }
    return effect;
}

void effBoobooMain(EffectEntry* effect) {
    EffBoobooN64Work* work = effect->userdata;
    s32 i;
    s32 done;
    s32 frame;
    f32 s;

    work->time++;
    for (i = 0; i < 11; i++) {
        done = 0;
        frame = work->frame[i];
        switch (work->state[i]) {
        case 0:
            done = 1;
            break;
        case 1:
            done = 1;
            work->anim[i] = 1;
            break;
        case 3:
            work->anim[i] = anime_data_walk[frame % 8];
            s = (f32)sin(6.2831855f * (f32)(frame * 10) / 360.0f);
            work->rot[i] += s;
            if (frame >= 36) {
                done = 1;
                work->frame[i] = 0;
            }
            break;
        case 2:
            work->anim[i] = anime_data_walk[frame % 8];
            s = (f32)sin(6.2831855f * (f32)(frame * 20) / 360.0f);
            done = 1;
            work->yoff[i] = 2.0f * s;
            if (frame >= 9) {
                work->yoff[i] = 0.0f;
                done = 1;
                work->frame[i] = 0;
            }
            break;
        case 4:
            work->anim[i] = anime_data_walk[frame % 8];
            s = (f32)sin(6.2831855f * (f32)(frame * 20) / 360.0f);
            work->yoff[i] = 4.0f * s;
            if (frame >= 9) {
                work->yoff[i] = 0.0f;
                done = 1;
                work->frame[i] = 0;
            }
            break;
        case 5:
        default:
            work->anim[i] = anime_data_walk[frame % 8];
            s = (f32)sin(6.2831855f * (f32)(frame * 5) / 360.0f);
            work->yoff[i] = 12.0f * s;
            if (frame >= 36) {
                work->yoff[i] = 0.0f;
                done = 1;
                work->frame[i] = 0;
            }
            break;
        }
        work->frame[i]++;
        if (done && (work->flags[i] & 2)) {
            work->flags[i] &= ~2;
            work->state[i] = work->nextState[i];
            work->frame[i] = 0;
        }
    }
    dispEntry(CAMERA_3D, 0.0f, 0, (DispCallback)effBoobooDisp, effect);
}

#define TEX_COMMON(id, cin_d)                                                                              \
    effGetTexObjN64(id, &texObj);                                                                          \
    GXLoadTexObj(&texObj, GX_TEXMAP0);                                                                     \
    GXSetNumTexGens(1);                                                                                    \
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);       \
    GXSetNumTevStages(1);                                                                                  \
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);                                    \
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);             \
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);             \
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, cin_d);                              \
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);

BOOL texture_dl(EffectEntry* effect, s32 idx, u32 type) {
    EffBoobooN64Work* work = effect->userdata;
    GXTexObj texObj;
    GXColor blend;
    GXColor color;

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    switch (type) {
    case 0:
        TEX_COMMON(0x38, GX_CC_ZERO)
        break;
    case 1:
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
        break;
    case 2:
        TEX_COMMON(0x39, GX_CC_ZERO)
        break;
    case 3:
        TEX_COMMON(0x3A, GX_CC_ZERO)
        break;
    case 4:
        TEX_COMMON(0x3B, GX_CC_ZERO)
        break;
    case 5:
        TEX_COMMON(0x3C, GX_CC_ZERO)
        break;
    case 6:
        TEX_COMMON(0x3D, GX_CC_ZERO)
        break;
    case 7:
        TEX_COMMON(0x3E, GX_CC_ZERO)
        break;
    case 8:
        TEX_COMMON(0x3F, GX_CC_C0)
        break;
    case 9:
        TEX_COMMON(0x40, GX_CC_C0)
        break;
    }
    if ((mapGetWork()->entries[0].flags & 2) && (work->flags[idx] & 8)) {
        mapGetBlend(&blend);
        if (!blend.r && !blend.g && !blend.b) {
            return TRUE;
        }
        color = blend;
        GXSetTevColor(GX_TEVREG0, color);
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C0, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    }
    return FALSE;
}

void effBoobooDisp(CameraId cameraId, EffectEntry* effect) {
    Mtx mtx;
    Mtx scl;
    Mtx rot;
    CameraEntry* camera;
    s32 animPhase[2]; // FAKE: register-promoted array (anim id + time*30); separate scalars differ
    EffBoobooN64Work* work;
    s32 i;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    animPhase[1] = work->time * 30;
    for (i = 0; i < 11; i++) {
        if (!(work->flags[i] & 1)) {
            continue;
        }
        PSMTXTrans(mtx, work->x[i], work->y[i] + work->yoff[i], work->z[i]);
        PSMTXRotRad(rot, 'y', 0.017453292f * (180.0f - work->rot[i]));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXScale(scl, 10.0f, 10.0f, 10.0f);
        PSMTXConcat(mtx, scl, mtx);
        PSMTXConcat(camera->view, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        if (texture_dl(effect, i, work->type[i])) {
            continue;
        }
        if (work->type[i] == 8 || work->type[i] == 9) {
            f32 s = (f32)sin(6.2831855f * (f32)(animPhase[1] + i * 20) / 360.0f);
            s32 v = (s32)(63.0f * s + 63.0f);
            GXSetTevColor(GX_TEVREG0, (GXColor){v, v, v});
        }
        animPhase[0] = work->anim[i];
        GXSetCullMode(GX_CULL_BACK);
        switch (animPhase[0]) {
        case 0:
            boo_polygon_0();
            break;
        case 1:
            boo_polygon_1();
            break;
        case 2:
            boo_polygon_2();
            break;
        }
    }
}
