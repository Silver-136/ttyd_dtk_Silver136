#include "mail_data.h"

typedef struct MailDataEntry {
    u32 flags;
    s32 id;
    const char* name_key;
    const char* message_key;
    s32 aux;
} MailDataEntry;

void* mailGetPtr(s32 id) {
    extern MailDataEntry mail_data[];
    u8* entry = (u8*)mail_data;
    s32 i;

    for (i = 0; i < 11; i++) {
        if (id == *(s32*)(entry + 4)) {
            break;
        }
        entry += 0x14;
        if (id == *(s32*)(entry + 4)) {
            break;
        }
        entry += 0x14;
        if (id == *(s32*)(entry + 4)) {
            break;
        }
        entry += 0x14;
        if (id == *(s32*)(entry + 4)) {
            break;
        }
        entry += 0x14;
    }
    return entry;
}

/* Target-backed mail message-key catalog. */
const char str_msg_mail_peach_stg1__802ff7c8[] = "msg_mail_peach_stg1_name_00";
const char str_msg_mail_peach_stg1__802ff7e4[] = "msg_mail_peach_stg1_00";
const char str_msg_mail_peach_stg2__802ff7fc[] = "msg_mail_peach_stg2_name_00";
const char str_msg_mail_peach_stg2__802ff818[] = "msg_mail_peach_stg2_00";
const char str_msg_mail_peach_stg3__802ff830[] = "msg_mail_peach_stg3_name_00";
const char str_msg_mail_peach_stg3__802ff84c[] = "msg_mail_peach_stg3_00";
const char str_msg_mail_peach_stg4__802ff864[] = "msg_mail_peach_stg4_name_00";
const char str_msg_mail_peach_stg4__802ff880[] = "msg_mail_peach_stg4_00";
const char str_msg_mail_peach_stg5__802ff898[] = "msg_mail_peach_stg5_name_00";
const char str_msg_mail_peach_stg5__802ff8b4[] = "msg_mail_peach_stg5_00";
const char str_msg_mail_peach_stg6__802ff8cc[] = "msg_mail_peach_stg6_name_00";
const char str_msg_mail_peach_stg6__802ff8e8[] = "msg_mail_peach_stg6_00";
const char str_msg_mail_nazo_stg3_n_802ff900[] = "msg_mail_nazo_stg3_name_00";
const char str_msg_mail_nazo_stg3_0_802ff91c[] = "msg_mail_nazo_stg3_00";
const char str_msg_mail_nazo_stg3_n_802ff934[] = "msg_mail_nazo_stg3_name_01";
const char str_msg_mail_nazo_stg3_0_802ff950[] = "msg_mail_nazo_stg3_01";
const char str_msg_mail_nazo_stg3_n_802ff968[] = "msg_mail_nazo_stg3_name_02";
const char str_msg_mail_nazo_stg3_0_802ff984[] = "msg_mail_nazo_stg3_02";
const char str_msg_mail_nazo_stg3_n_802ff99c[] = "msg_mail_nazo_stg3_name_03";
const char str_msg_mail_nazo_stg3_0_802ff9b8[] = "msg_mail_nazo_stg3_03";
const char str_msg_mail_nazo_stg3_n_802ff9d0[] = "msg_mail_nazo_stg3_name_04";
const char str_msg_mail_nazo_stg3_0_802ff9ec[] = "msg_mail_nazo_stg3_04";
const char str_msg_mail_nazo_stg3_n_802ffa04[] = "msg_mail_nazo_stg3_name_05";
const char str_msg_mail_nazo_stg3_0_802ffa20[] = "msg_mail_nazo_stg3_05";
const char str_msg_mail_nazo_stg3_n_802ffa38[] = "msg_mail_nazo_stg3_name_06";
const char str_msg_mail_nazo_stg3_0_802ffa54[] = "msg_mail_nazo_stg3_06";
const char str_msg_mail_nazo_stg3_n_802ffa6c[] = "msg_mail_nazo_stg3_name_07";
const char str_msg_mail_nazo_stg3_0_802ffa88[] = "msg_mail_nazo_stg3_07";
const char str_msg_mail_nazo_stg3_n_802ffaa0[] = "msg_mail_nazo_stg3_name_08";
const char str_msg_mail_nazo_stg3_0_802ffabc[] = "msg_mail_nazo_stg3_08";
const char str_msg_mail_nazo_stg3_n_802ffad4[] = "msg_mail_nazo_stg3_name_09";
const char str_msg_mail_nazo_stg3_0_802ffaf0[] = "msg_mail_nazo_stg3_09";
const char str_msg_mail_nazo_stg3_n_802ffb08[] = "msg_mail_nazo_stg3_name_10";
const char str_msg_mail_nazo_stg3_1_802ffb24[] = "msg_mail_nazo_stg3_10";
const char str_mail_name_00_802ffb3c[] = "mail_name_00";
const char str_mail_00_802ffb4c[] = "mail_00";
const char str_mail_name_01_802ffb54[] = "mail_name_01";
const char str_mail_01_802ffb64[] = "mail_01";
const char str_mail_name_02_802ffb6c[] = "mail_name_02";
const char str_mail_02_802ffb7c[] = "mail_02";
const char str_mail_name_03_802ffb84[] = "mail_name_03";
const char str_mail_03_802ffb94[] = "mail_03";
const char str_mail_name_04_802ffb9c[] = "mail_name_04";
const char str_mail_04_802ffbac[] = "mail_04";
const char str_mail_name_05_802ffbb4[] = "mail_name_05";
const char str_mail_05_802ffbc4[] = "mail_05";
const char str_mail_name_06_802ffbcc[] = "mail_name_06";
const char str_mail_06_802ffbdc[] = "mail_06";
const char str_mail_name_07_802ffbe4[] = "mail_name_07";
const char str_mail_07_802ffbf4[] = "mail_07";
const char str_mail_name_08_802ffbfc[] = "mail_name_08";
const char str_mail_08_802ffc0c[] = "mail_08";
const char str_mail_name_09_802ffc14[] = "mail_name_09";
const char str_mail_09_802ffc24[] = "mail_09";
const char str_info_mail_name_00_802ffc2c[] = "info_mail_name_00";
const char str_info_mail_00_802ffc40[] = "info_mail_00";
const char str_info_mail_name_01_802ffc50[] = "info_mail_name_01";
const char str_info_mail_01_802ffc64[] = "info_mail_01";
const char str_info_mail_name_02_802ffc74[] = "info_mail_name_02";
const char str_info_mail_02_802ffc88[] = "info_mail_02";
const char str_info_mail_name_03_802ffc98[] = "info_mail_name_03";
const char str_info_mail_03_802ffcac[] = "info_mail_03";
const char str_info_mail_name_04_802ffcbc[] = "info_mail_name_04";
const char str_info_mail_04_802ffcd0[] = "info_mail_04";
const char str_info_mail_name_05_802ffce0[] = "info_mail_name_05";
const char str_info_mail_05_802ffcf4[] = "info_mail_05";
const char str_info_mail_name_06_802ffd04[] = "info_mail_name_06";
const char str_info_mail_06_802ffd18[] = "info_mail_06";
const char str_info_mail_name_07_802ffd28[] = "info_mail_name_07";
const char str_info_mail_07_802ffd3c[] = "info_mail_07";
const char str_free_mail_name_00_802ffd4c[] = "free_mail_name_00";
const char str_free_mail_00_802ffd60[] = "free_mail_00";
const char str_free_mail_name_01_802ffd70[] = "free_mail_name_01";
const char str_free_mail_01_802ffd84[] = "free_mail_01";
const char str_free_mail_name_02_802ffd94[] = "free_mail_name_02";
const char str_free_mail_02_802ffda8[] = "free_mail_02";
const char str_irai_af_mail_00_802ffdb8[] = "irai_af_mail_00";
const char str_irai_mail_00_802ffdc8[] = "irai_mail_00";
const char str_irai_af_mail_01_802ffdd8[] = "irai_af_mail_01";
const char str_irai_mail_01_802ffde8[] = "irai_mail_01";
const char str_irai_af_mail_02_802ffdf8[] = "irai_af_mail_02";
const char str_irai_mail_02_802ffe08[] = "irai_mail_02";
const char str_irai_af_mail_03_802ffe18[] = "irai_af_mail_03";
const char str_irai_mail_03_802ffe28[] = "irai_mail_03";
const char str_irai_af_mail_04_802ffe38[] = "irai_af_mail_04";
const char str_irai_mail_04_802ffe48[] = "irai_mail_04";
const char str_irai_af_mail_05_802ffe58[] = "irai_af_mail_05";
const char str_irai_mail_05_802ffe68[] = "irai_mail_05";

MailDataEntry mail_data[44] = {
    {0x00010000, 0, str_msg_mail_peach_stg1__802ff7c8, str_msg_mail_peach_stg1__802ff7e4, 0},
    {0x00010000, 1, str_msg_mail_peach_stg2__802ff7fc, str_msg_mail_peach_stg2__802ff818, 0},
    {0x00010000, 2, str_msg_mail_peach_stg3__802ff830, str_msg_mail_peach_stg3__802ff84c, 0},
    {0x00010000, 3, str_msg_mail_peach_stg4__802ff864, str_msg_mail_peach_stg4__802ff880, 0},
    {0x00010000, 4, str_msg_mail_peach_stg5__802ff898, str_msg_mail_peach_stg5__802ff8b4, 0},
    {0x00010000, 5, str_msg_mail_peach_stg6__802ff8cc, str_msg_mail_peach_stg6__802ff8e8, 0},
    {0x00080000, 6, str_msg_mail_nazo_stg3_n_802ff900, str_msg_mail_nazo_stg3_0_802ff91c, 0},
    {0x00080000, 7, str_msg_mail_nazo_stg3_n_802ff934, str_msg_mail_nazo_stg3_0_802ff950, 0},
    {0x00080000, 8, str_msg_mail_nazo_stg3_n_802ff968, str_msg_mail_nazo_stg3_0_802ff984, 0},
    {0x00080000, 9, str_msg_mail_nazo_stg3_n_802ff99c, str_msg_mail_nazo_stg3_0_802ff9b8, 0},
    {0x00080000, 10, str_msg_mail_nazo_stg3_n_802ff9d0, str_msg_mail_nazo_stg3_0_802ff9ec, 0},
    {0x00080000, 11, str_msg_mail_nazo_stg3_n_802ffa04, str_msg_mail_nazo_stg3_0_802ffa20, 0},
    {0x00080000, 12, str_msg_mail_nazo_stg3_n_802ffa38, str_msg_mail_nazo_stg3_0_802ffa54, 0},
    {0x00080000, 13, str_msg_mail_nazo_stg3_n_802ffa6c, str_msg_mail_nazo_stg3_0_802ffa88, 0},
    {0x00080000, 14, str_msg_mail_nazo_stg3_n_802ffaa0, str_msg_mail_nazo_stg3_0_802ffabc, 0},
    {0x00080000, 15, str_msg_mail_nazo_stg3_n_802ffad4, str_msg_mail_nazo_stg3_0_802ffaf0, 0},
    {0x00080000, 43, str_msg_mail_nazo_stg3_n_802ffb08, str_msg_mail_nazo_stg3_1_802ffb24, 0},
    {0x10000000, 16, str_mail_name_00_802ffb3c, str_mail_00_802ffb4c, 0},
    {0x10000000, 17, str_mail_name_01_802ffb54, str_mail_01_802ffb64, 1},
    {0x10000000, 18, str_mail_name_02_802ffb6c, str_mail_02_802ffb7c, 2},
    {0x10000000, 19, str_mail_name_03_802ffb84, str_mail_03_802ffb94, 3},
    {0x10000000, 20, str_mail_name_04_802ffb9c, str_mail_04_802ffbac, 4},
    {0x10000000, 21, str_mail_name_05_802ffbb4, str_mail_05_802ffbc4, 5},
    {0x10000000, 22, str_mail_name_06_802ffbcc, str_mail_06_802ffbdc, 6},
    {0x10000000, 23, str_mail_name_07_802ffbe4, str_mail_07_802ffbf4, 7},
    {0x10000000, 24, str_mail_name_08_802ffbfc, str_mail_08_802ffc0c, 8},
    {0x10000000, 25, str_mail_name_09_802ffc14, str_mail_09_802ffc24, 9},
    {0x00040000, 26, str_info_mail_name_00_802ffc2c, str_info_mail_00_802ffc40, 0},
    {0x00040000, 27, str_info_mail_name_01_802ffc50, str_info_mail_01_802ffc64, 0},
    {0x00040000, 28, str_info_mail_name_02_802ffc74, str_info_mail_02_802ffc88, 0},
    {0x00040000, 29, str_info_mail_name_03_802ffc98, str_info_mail_03_802ffcac, 0},
    {0x00040000, 30, str_info_mail_name_04_802ffcbc, str_info_mail_04_802ffcd0, 0},
    {0x00040000, 31, str_info_mail_name_05_802ffce0, str_info_mail_05_802ffcf4, 0},
    {0x00040000, 32, str_info_mail_name_06_802ffd04, str_info_mail_06_802ffd18, 0},
    {0x00040000, 33, str_info_mail_name_07_802ffd28, str_info_mail_07_802ffd3c, 0},
    {0x00000000, 34, str_free_mail_name_00_802ffd4c, str_free_mail_00_802ffd60, 0},
    {0x00000000, 35, str_free_mail_name_01_802ffd70, str_free_mail_01_802ffd84, 0},
    {0x00000000, 36, str_free_mail_name_02_802ffd94, str_free_mail_02_802ffda8, 0},
    {0x00020000, 37, str_irai_af_mail_00_802ffdb8, str_irai_mail_00_802ffdc8, 0},
    {0x00020000, 38, str_irai_af_mail_01_802ffdd8, str_irai_mail_01_802ffde8, 0},
    {0x00020000, 39, str_irai_af_mail_02_802ffdf8, str_irai_mail_02_802ffe08, 0},
    {0x00020000, 40, str_irai_af_mail_03_802ffe18, str_irai_mail_03_802ffe28, 0},
    {0x00020000, 41, str_irai_af_mail_04_802ffe38, str_irai_mail_04_802ffe48, 0},
    {0x00020000, 42, str_irai_af_mail_05_802ffe58, str_irai_mail_05_802ffe68, 0},
};
