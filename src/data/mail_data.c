#include "data/mail_data.h"

MailData mail_data[44] = {
    {0x1, 0, 0, "msg_mail_peach_stg1_name_00", "msg_mail_peach_stg1_00", 0},
    {0x1, 0, 1, "msg_mail_peach_stg2_name_00", "msg_mail_peach_stg2_00", 0},
    {0x1, 0, 2, "msg_mail_peach_stg3_name_00", "msg_mail_peach_stg3_00", 0},
    {0x1, 0, 3, "msg_mail_peach_stg4_name_00", "msg_mail_peach_stg4_00", 0},
    {0x1, 0, 4, "msg_mail_peach_stg5_name_00", "msg_mail_peach_stg5_00", 0},
    {0x1, 0, 5, "msg_mail_peach_stg6_name_00", "msg_mail_peach_stg6_00", 0},
    {0x8, 0, 6, "msg_mail_nazo_stg3_name_00", "msg_mail_nazo_stg3_00", 0},
    {0x8, 0, 7, "msg_mail_nazo_stg3_name_01", "msg_mail_nazo_stg3_01", 0},
    {0x8, 0, 8, "msg_mail_nazo_stg3_name_02", "msg_mail_nazo_stg3_02", 0},
    {0x8, 0, 9, "msg_mail_nazo_stg3_name_03", "msg_mail_nazo_stg3_03", 0},
    {0x8, 0, 10, "msg_mail_nazo_stg3_name_04", "msg_mail_nazo_stg3_04", 0},
    {0x8, 0, 11, "msg_mail_nazo_stg3_name_05", "msg_mail_nazo_stg3_05", 0},
    {0x8, 0, 12, "msg_mail_nazo_stg3_name_06", "msg_mail_nazo_stg3_06", 0},
    {0x8, 0, 13, "msg_mail_nazo_stg3_name_07", "msg_mail_nazo_stg3_07", 0},
    {0x8, 0, 14, "msg_mail_nazo_stg3_name_08", "msg_mail_nazo_stg3_08", 0},
    {0x8, 0, 15, "msg_mail_nazo_stg3_name_09", "msg_mail_nazo_stg3_09", 0},
    {0x8, 0, 43, "msg_mail_nazo_stg3_name_10", "msg_mail_nazo_stg3_10", 0},
    {0x1000, 0, 16, "mail_name_00", "mail_00", 0},
    {0x1000, 0, 17, "mail_name_01", "mail_01", 1},
    {0x1000, 0, 18, "mail_name_02", "mail_02", 2},
    {0x1000, 0, 19, "mail_name_03", "mail_03", 3},
    {0x1000, 0, 20, "mail_name_04", "mail_04", 4},
    {0x1000, 0, 21, "mail_name_05", "mail_05", 5},
    {0x1000, 0, 22, "mail_name_06", "mail_06", 6},
    {0x1000, 0, 23, "mail_name_07", "mail_07", 7},
    {0x1000, 0, 24, "mail_name_08", "mail_08", 8},
    {0x1000, 0, 25, "mail_name_09", "mail_09", 9},
    {0x4, 0, 26, "info_mail_name_00", "info_mail_00", 0},
    {0x4, 0, 27, "info_mail_name_01", "info_mail_01", 0},
    {0x4, 0, 28, "info_mail_name_02", "info_mail_02", 0},
    {0x4, 0, 29, "info_mail_name_03", "info_mail_03", 0},
    {0x4, 0, 30, "info_mail_name_04", "info_mail_04", 0},
    {0x4, 0, 31, "info_mail_name_05", "info_mail_05", 0},
    {0x4, 0, 32, "info_mail_name_06", "info_mail_06", 0},
    {0x4, 0, 33, "info_mail_name_07", "info_mail_07", 0},
    {0, 0, 34, "free_mail_name_00", "free_mail_00", 0},
    {0, 0, 35, "free_mail_name_01", "free_mail_01", 0},
    {0, 0, 36, "free_mail_name_02", "free_mail_02", 0},
    {0x2, 0, 37, "irai_af_mail_00", "irai_mail_00", 0},
    {0x2, 0, 38, "irai_af_mail_01", "irai_mail_01", 0},
    {0x2, 0, 39, "irai_af_mail_02", "irai_mail_02", 0},
    {0x2, 0, 40, "irai_af_mail_03", "irai_mail_03", 0},
    {0x2, 0, 41, "irai_af_mail_04", "irai_mail_04", 0},
    {0x2, 0, 42, "irai_af_mail_05", "irai_mail_05", 0},
};

MailData* mailGetPtr(s32 id) {
    MailData* mail = mail_data;
    s32 i;

    for (i = 0; i < 44; i++, mail++) {
        if (id == mail->id) {
            break;
        }
    }
    return mail;
}
