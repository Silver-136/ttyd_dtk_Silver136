#pragma once

#include <dolphin/types.h>

// MailData.flags: the kind of mail (win_root colours the list entry by 1/2/4) and whether it has a picture
typedef enum MailFlags {
    MAIL_FLAG_PEACH = (1 << 0),      // 0x1, msg_mail_peach_*
    MAIL_FLAG_TROUBLE = (1 << 1),    // 0x2, Trouble Center (irai_*)
    MAIL_FLAG_INFO = (1 << 2),       // 0x4, info_mail_*
    MAIL_FLAG_NAZO = (1 << 3),       // 0x8, msg_mail_nazo_*
    MAIL_FLAG_PICTURE = (1 << 12),   // 0x1000, a picture (icon) next to the title
} MailFlags;

typedef struct MailData {
    u16 flags;         // 0x0, MailFlags
    u16 pad_2;         // 0x2
    s32 id;            // 0x4
    const char* name;  // 0x8, message tag of the mail's title
    const char* body;  // 0xC, message tag of the mail's text
    s32 icon;          // 0x10, texture drawn next to the title with MAIL_FLAG_PICTURE (win_root mail list)
} MailData;

extern MailData mail_data[44];

MailData* mailGetPtr(s32 id);
