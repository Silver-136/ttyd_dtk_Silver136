#include "nameent.h"
#include "system.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "driver/fadedrv.h"
#include "sdk/texPalette.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "manager/fontmgr.h"
#include "win/win_main.h"
#include "mario/mariost.h"
#include "version.h"
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>

void winTexInit_x2(TEXPalette* texture);
void winTexSet_x2(u32 id1, u32 id2, Vec translate, Vec scale, GXColor color);
void FontDrawString(f32 x, f32 y, const char* str);
void GXGetProjectionv(f32* ptr);
void GXGetViewportv(f32* vp);
void nameBG(void);
void nameKirinukiGX(f32 x, f32 y, f32 w, f32 h);
void nameMaskGX(f32 t);
void nameWinGX(f32 x, f32 y, f32 w, f32 h, TEXPalette* tpl, s32 type);
#include <string.h>

s32 nameMain(void);
double sin(double);
void nameEntDisp(CameraId cameraId, void* param);

//.data
#ifdef VERSION_G8ME01
#define NAME_LANG_PICK(jp, en) ((u32)gp->language == 0 ? (jp) : (en))
#else
#define NAME_LANG_PICK(jp, en) (jp)
#endif

#define NAMEENT_COLS 14 // characters per row of hiragana_tbl, katakana_tbl and latin_tbl

const char* hiragana_tbl[] = {
    "\x82\xA0", "\x82\xA2", "\x82\xA4", "\x82\xA6", "\x82\xA8", "\x82\xED",
    "\x81\x40", "\x82\xF0", "\x81\x40", "\x82\xF1", "\x82\x4F", "\x82\x50",
    "\x82\x51", "\x82\x52", "\x82\xA9", "\x82\xAB", "\x82\xAD", "\x82\xAF",
    "\x82\xB1", "\x82\xAA", "\x82\xAC", "\x82\xAE", "\x82\xB0", "\x82\xB2",
    "\x82\x53", "\x82\x54", "\x82\x55", "\x82\x56", "\x82\xB3", "\x82\xB5",
    "\x82\xB7", "\x82\xB9", "\x82\xBB", "\x82\xB4", "\x82\xB6", "\x82\xB8",
    "\x82\xBA", "\x82\xBC", "\x82\x57", "\x82\x58", "\x81\x49", "\x81\x48",
    "\x82\xBD", "\x82\xBF", "\x82\xC2", "\x82\xC4", "\x82\xC6", "\x82\xBE",
    "\x82\xC0", "\x82\xC3", "\x82\xC5", "\x82\xC7", "\x81\x40", "\x94\xB5",
    "\x81\x99", "\x81\xF4", "\x82\xC8", "\x82\xC9", "\x82\xCA", "\x82\xCB",
    "\x82\xCC", "\x82\xCE", "\x82\xD1", "\x82\xD4", "\x82\xD7", "\x82\xDA",
    NULL,       NULL,       NULL,       NULL,       "\x82\xCD", "\x82\xD0",
    "\x82\xD3", "\x82\xD6", "\x82\xD9", "\x82\xCF", "\x82\xD2", "\x82\xD5",
    "\x82\xD8", "\x82\xDB", NULL,       NULL,       NULL,       NULL,
    "\x82\xDC", "\x82\xDD", "\x82\xDE", "\x82\xDF", "\x82\xE0", "\x82\x9F",
    "\x82\xA1", "\x82\xA3", "\x82\xA5", "\x82\xA7", NULL,       NULL,
    NULL,       NULL,       "\x82\xE2", "\x81\x40", "\x82\xE4", "\x81\x40",
    "\x82\xE6", "\x82\xE1", "\x82\xE3", "\x82\xE5", "\x82\xC1", "\x89\x4C",
    NULL,       NULL,       NULL,       NULL,       "\x82\xE7", "\x82\xE8",
    "\x82\xE9", "\x82\xEA", "\x82\xEB", "\x81\x7C", "\x81\x60", "\x81\x45",
    "\x81\x44", "\x81\x40", NULL,       NULL,       NULL,       NULL
};

#ifdef VERSION_G8ME01
#endif
const char* katakana_tbl[] = {
    "\x83\x41", "\x83\x43", "\x83\x45", "\x83\x47", "\x83\x49", "\x83\x8F",
    "\x81\x40", "\x83\x92", "\x81\x40", "\x83\x93", "\x82\x4F", "\x82\x50",
    "\x82\x51", "\x82\x52", "\x83\x4A", "\x83\x4C", "\x83\x4E", "\x83\x50",
    "\x83\x52", "\x83\x4B", "\x83\x4D", "\x83\x4F", "\x83\x51", "\x83\x53",
    "\x82\x53", "\x82\x54", "\x82\x55", "\x82\x56", "\x83\x54", "\x83\x56",
    "\x83\x58", "\x83\x5A", "\x83\x5C", "\x83\x55", "\x83\x57", "\x83\x59",
    "\x83\x5B", "\x83\x5D", "\x82\x57", "\x82\x58", "\x81\x49", "\x81\x48",
    "\x83\x5E", "\x83\x60", "\x83\x63", "\x83\x65", "\x83\x67", "\x83\x5F",
    "\x83\x61", "\x83\x64", "\x83\x66", "\x83\x68", "\x81\x40", "\x94\xB5",
    "\x81\x99", "\x81\xF4", "\x83\x69", "\x83\x6A", "\x83\x6B", "\x83\x6C",
    "\x83\x6D", "\x83\x6F", "\x83\x72", "\x83\x75", "\x83\x78", "\x83\x7B",
    NULL,       NULL,       NULL,       NULL,       "\x83\x6E", "\x83\x71",
    "\x83\x74", "\x83\x77", "\x83\x7A", "\x83\x70", "\x83\x73", "\x83\x76",
    "\x83\x79", "\x83\x7C", NULL,       NULL,       NULL,       NULL,
    "\x83\x7D", "\x83\x7E", "\x83\x80", "\x83\x81", "\x83\x82", "\x83\x40",
    "\x83\x42", "\x83\x44", "\x83\x46", "\x83\x48", NULL,       NULL,
    NULL,       NULL,       "\x83\x84", "\x81\x40", "\x83\x86", "\x81\x40",
    "\x83\x88", "\x83\x83", "\x83\x85", "\x83\x87", "\x83\x62", "\x83\x94",
    NULL,       NULL,       NULL,       NULL,       "\x83\x89", "\x83\x8A",
    "\x83\x8B", "\x83\x8C", "\x83\x8D", "\x81\x7C", "\x81\x60", "\x81\x45",
    "\x81\x44", "\x81\x40", NULL,       NULL,       NULL,       NULL
};

//.bss
static NameEntryWork work __attribute__((aligned(8)));

//.sdata
static NameEntryWork* wp = &work;
static const char* msg_tbl[] = {
    "msg_nameent_1",
    "msg_nameent_2",
    "msg_nameent_3",
    "msg_nameent_3",
    "msg_nameent_4"
};

void nameEntInit(void) {
    memset(wp, 0, sizeof(NameEntryWork));
}

void nameEntReInit(void) {
    wp->loadState = 3;
}

#ifdef VERSION_G8ME01
const char* latin_tbl[] = {
    "A", "B", "C", "D", "E", "a",
    "b", "c", "d", "e", "\xFC", "\xFA",
    "\xF9", "\xFB", "F", "G", "H", "I",
    "J", "f", "g", "h", "i", "j",
    "\xDF", "-", "/", "&", "K", "L",
    "M", "N", "O", "k", "l", "m",
    "n", "o", "\xA1", "!", "\xBF", "?",
    "P", "Q", "R", "S", "T", "p",
    "q", "r", "s", "t", ".", ",",
    "0", "1", "U", "V", "W", "X",
    "Y", "u", "v", "w", "x", "y",
    "2", "3", "4", "5", "Z", "\xC4",
    "\xC1", "\xC0", "\xC7", "z", "\xE4", "\xE1",
    "\xE0", "\xE2", "6", "7", "8", "9",
    "\xC9", "\xC8", "\xCD", "\xCC", "\xD1", "\xE7",
    "\xEB", "\xE9", "\xE8", "\xEA", NULL, NULL,
    NULL, NULL, "\xD6", "\xD3", "\xD2", "\x8C",
    "\xDC", "\xEF", "\xED", "\xEC", "\xEE", "\xF1",
    NULL, NULL, NULL, NULL, "\xDA", "\xD9",
    "\xD0", "\xDE", " ", "\xF6", "\xF3", "\xF2",
    "\xF4", "\x9C", NULL, NULL, NULL, NULL
};
#endif

BOOL nameEntPrepare(void) {
#ifdef VERSION_G8ME01
    const char* lang[6] = {"jp", "us", "ge", "fr", "sp", "it"};
    return fileAsyncf(4, NULL, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]) != 0;
#else
    return fileAsyncf(4, NULL, "%s/w/jp/name.tpl", getMarioStDvdRoot()) != 0;
#endif
}

void nameEntOn(s32 type) {
    s32 i;

    wp->flags = 0;
    wp->type = type;
    wp->loadState = 1;
    wp->state = 0;
#ifdef VERSION_G8ME01
    if ((u32)gp->language == 0) {
        wp->charTable = hiragana_tbl;
    } else {
        wp->charTable = latin_tbl;
    }
#else
    wp->charTable = hiragana_tbl;
#endif
    wp->nameLen = 0;
    wp->cursorX = 0;
    wp->cursorY = 0;
    wp->x1 = wp->x2 = (f32)(wp->cursorX * 24 - 190 + (wp->cursorX / 5) * 30);
    wp->y1 = wp->y2 = (f32)(112 - wp->cursorY * 28);
    wp->caretX = wp->caretTargetX = -106.0f;
    wp->caretY = wp->caretTargetY = 135.0f;
    strcpy(wp->name, "");
    for (i = 0; i < 8; i++) {
#ifdef VERSION_G8ME01
        strcat(wp->name, (u32)gp->language == 0 ? "\x81\x40" : " ");
#else
        strcat(wp->name, "\x81\x40");
#endif
    }
    wp->maskAlpha = 0.0f;
#ifdef VERSION_G8ME01
    if ((u32)gp->language == 0) {
        switch (type) {
            case 2:
                hiragana_tbl[9] = "\x81\x40";
                katakana_tbl[9] = "\x81\x40";
                break;
            default:
                hiragana_tbl[9] = "\x82\xF1";
                katakana_tbl[9] = "\x83\x93";
                break;
        }
    } else {
        switch (type) {
            case 2:
                latin_tbl[47] = " ";
                break;
            default:
                latin_tbl[47] = "p";
                break;
        }
    }
#else
    switch (type) {
        case 2:
            hiragana_tbl[9] = "\x81\x40";
            katakana_tbl[9] = "\x81\x40";
            break;
        default:
            hiragana_tbl[9] = "\x82\xF1";
            katakana_tbl[9] = "\x83\x93";
            break;
    }
#endif
}

void nameEntOff(void) {
    wp->loadState = 3;
}

BOOL nameEntWait(void) {
    wp->flags |= 1;
    return wp->state != 0x65;
}

BOOL nameEntIsCancel(void) {
    return (wp->flags >> 1) & 1;
}

const char* nameEntGetName(void) {
    s32 j;

    if (wp->state != 0x65) {
        return NULL;
    }
#ifdef VERSION_G8ME01
    if ((u32)gp->language == 0) {
        for (j = 7; j >= 1; j--) {
            if (strncmp(&wp->name[j * 2], "\x81\x40", 2)) {
                break;
            }
            wp->name[j * 2] = 0;
            wp->name[j * 2 + 1] = 0;
        }
    } else {
        for (j = 7; j >= 1; j--) {
            if (strncmp(&wp->name[j], " ", 1)) {
                break;
            }
            wp->name[j] = 0;
        }
    }
#else
    for (j = 7; j >= 1; j--) {
        if (strncmp(&wp->name[j * 2], "\x81\x40", 2)) {
            break;
        }
        wp->name[j * 2] = 0;
        wp->name[j * 2 + 1] = 0;
    }
#endif
    return wp->name;
}

static inline BOOL nameEntLoad(void) {
#ifdef VERSION_G8ME01
    const char* lang[6] = {"jp", "us", "ge", "fr", "sp", "it"};
#endif
#ifdef VERSION_G8ME01
    if (fileAsyncf(4, NULL, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]) != 0U) {
#else
    if (fileAsyncf(4, NULL, "%s/w/jp/name.tpl", getMarioStDvdRoot()) != 0U) {
#endif
#ifdef VERSION_G8ME01
        wp->tplFile = fileAllocf(4, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]);
#else
        wp->tplFile = fileAllocf(4, "%s/w/jp/name.tpl", getMarioStDvdRoot());
#endif
        return TRUE;
    }
    return FALSE;
}

void nameEntMain(void) {
    switch (wp->loadState) {
        case 0:
            break;
        case 1:
            if (nameEntLoad()) {
                wp->loadState++;
            }
        case 2:
            nameMain();
            break;
        case 3:
            if (wp->tplFile) {
                fileFree(wp->tplFile);
            }
            wp->tplFile = NULL;
            wp->loadState = 0;
            break;
    }
    if (wp->loadState > 1) {
        dispEntry(CAMERA_3D, 0.0f, 7, nameEntDisp, NULL);
    }
}

s32 nameMain(void) {
    s32 len;
    s32 j;
    s32 off;
    s32 o2;
    s32 idx;
    f32 dist;

    dist = distABf(wp->x2, wp->y2, wp->x1, wp->y1);
    switch (wp->state) {
        case 0:
            if ((wp->flags & 1) && gp->debugFlag == 0) {
                if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                    off = wp->cursorX;
#ifdef VERSION_G8ME01
                    if (off >= 10 && wp->cursorY >= (s32)((u32)gp->language == 0 ? 4 : 6)) {
#else
                    if (off >= 10 && wp->cursorY >= 4) {
#endif
                        switch (wp->cursorY) {
                            case 4:
                                wp->charTable = hiragana_tbl;
                                psndSFXOn((const char*)0x2000C);
                                break;
                            case 5:
                                wp->charTable = katakana_tbl;
                                psndSFXOn((const char*)0x2000C);
                                break;
                            case 6:
                                if (wp->nameLen > 0) {
                                    wp->nameLen--;
                                    psndSFXOn((const char*)0x2000B);
                                                            }
                                break;
                            case 7: {
                                s32 n;
                                s32 k;
                                s32 o;
#ifdef VERSION_G8ME01
                                if ((u32)gp->language == 0) {
                                    n = strlen(wp->name) / 2;
                                    for (k = 0, o = 0; k < n; o += 2, k++) {
                                        if (strncmp(&wp->name[o], "\x81\x40", 2)) {
                                            break;
                                        }
                                    }
                                    if (k < n || wp->type != 0) {
                                        wp->confirmSel = 1;
                                        wp->state = 100;
                                        psndSFXOn((const char*)0x20012);
                                        if (wp->type == 1 && k >= n) {
                                            strcpy(wp->name, msgSearch("name_party3"));
                                            wp->nameLen = strlen(wp->name) / 2;
                                        }
                                    } else {
                                        psndSFXOn((const char*)0x20014);
                                    }
                                } else {
                                    s32 m;

                                    n = strlen(wp->name);
                                    for (m = 0; m < n; m++) {
                                        if (strncmp(&wp->name[m], " ", 1)) {
                                            break;
                                        }
                                    }
                                    if (m < n || wp->type != 0) {
                                        wp->confirmSel = 1;
                                        wp->state = 100;
                                        psndSFXOn((const char*)0x20012);
                                        if (wp->type == 1 && m >= n) {
                                            strcpy(wp->name, msgSearch("name_party3"));
                                            wp->nameLen = strlen(wp->name);
                                        }
                                    } else {
                                        psndSFXOn((const char*)0x20014);
                                    }
                                }
#else
                                n = strlen(wp->name) / 2;
                                for (k = 0, o = 0; k < n; o += 2, k++) {
                                    if (strncmp(&wp->name[o], "\x81\x40", 2)) {
                                        break;
                                    }
                                }
                                if (k < n || wp->type != 0) {
                                    wp->confirmSel = 1;
                                    wp->state = 100;
                                    psndSFXOn((const char*)0x20012);
                                    if (wp->type == 1 && k >= n) {
                                        strcpy(wp->name, msgSearch("name_party3"));
                                        wp->nameLen = strlen(wp->name) / 2;
                                    }
                                } else {
                                    psndSFXOn((const char*)0x20014);
                                }
#endif
}
                                break;
                        }
                    } else if (wp->nameLen < 8) {
#ifdef VERSION_G8ME01
                        if ((u32)gp->language == 0) {
                            ((u16*)wp->name)[wp->nameLen] = *(u16*)wp->charTable[wp->cursorY * NAMEENT_COLS + off];
                        } else {
                            wp->name[wp->nameLen] = *wp->charTable[wp->cursorY * NAMEENT_COLS + off];
                        }
#else
                        ((u16*)wp->name)[wp->nameLen] = *(u16*)wp->charTable[wp->cursorY * NAMEENT_COLS + off];
#endif
                        wp->nameLen++;
                        if (wp->nameLen >= 8) {
                            wp->nameLen = 7;
                            wp->cursorX = 10;
                            wp->cursorY = 7;
                        }
                        psndSFXOn((const char*)0xA);
                    } else if (wp->nameLen == 8) {
                        wp->nameLen = 7;
#ifdef VERSION_G8ME01
                        if ((u32)gp->language == 0) {
                            ((u16*)wp->name)[wp->nameLen] = *(u16*)wp->charTable[wp->cursorY * NAMEENT_COLS + wp->cursorX];
                        } else {
                            wp->name[wp->nameLen] = *wp->charTable[wp->cursorY * NAMEENT_COLS + wp->cursorX];
                        }
#else
                        ((u16*)wp->name)[wp->nameLen] = *(u16*)wp->charTable[wp->cursorY * NAMEENT_COLS + wp->cursorX];
#endif
                        wp->cursorX = 10;
                        wp->cursorY = 7;
                        psndSFXOn((const char*)0xA);
                    }
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_START) {
#ifdef VERSION_G8ME01
                    if ((u32)gp->language == 0) {
                        len = strlen(wp->name) / 2;
                        for (j = 0, off = 0; j < len; off += 2, j++) {
                            if (strncmp(&wp->name[off], "\x81\x40", 2)) {
                                break;
                            }
                        }
                        if (j < len || wp->type != 0) {
                            wp->confirmSel = 1;
                            wp->state = 100;
                            psndSFXOn((const char*)0x12);
                            if (wp->type == 1 && j >= len) {
                                strcpy(wp->name, msgSearch("name_party3"));
                                wp->nameLen = strlen(wp->name) / 2;
                            }
                        } else {
                            psndSFXOn((const char*)0x20014);
                        }
                    } else {
                        s32 m;

                        len = strlen(wp->name);
                        for (m = 0; m < len; m++) {
                            if (strncmp(&wp->name[m], " ", 1)) {
                                break;
                            }
                        }
                        if (m < len || wp->type != 0) {
                            wp->confirmSel = 1;
                            wp->state = 100;
                            psndSFXOn((const char*)0x12);
                            if (wp->type == 1 && m >= len) {
                                strcpy(wp->name, msgSearch("name_party3"));
                                wp->nameLen = strlen(wp->name);
                            }
                        } else {
                            psndSFXOn((const char*)0x20014);
                        }
                    }
#else
                    len = strlen(wp->name) / 2;
                    for (j = 0, off = 0; j < len; off += 2, j++) {
                        if (strncmp(&wp->name[off], "\x81\x40", 2)) {
                            break;
                        }
                    }
                    if (j < len || wp->type != 0) {
                        wp->confirmSel = 1;
                        wp->state = 100;
                        psndSFXOn((const char*)0x12);
                        if (wp->type == 1 && j >= len) {
                            strcpy(wp->name, msgSearch("name_party3"));
                            wp->nameLen = strlen(wp->name) / 2;
                        }
                    } else {
                        psndSFXOn((const char*)0x20014);
                    }
#endif
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
#ifdef VERSION_G8ME01
                    if ((u32)gp->language == 0) {
                        if (wp->nameLen == 0 && wp->type == 0 && !strncmp(wp->name, "\x81\x40", 2)) {
                            psndSFXOn((const char*)0x13);
                            wp->state = 0x65;
                            wp->flags |= 2;
                        } else {
                            ((u16*)wp->name)[wp->nameLen] = *(u16*)"\x81\x40";
                            wp->nameLen--;
                            if (wp->nameLen < 0) {
                                wp->nameLen = 0;
                            }
                            psndSFXOn((const char*)0xB);
                        }
                    } else {
                        if (wp->nameLen == 0 && wp->type == 0 && !strncmp(wp->name, " ", 1)) {
                            psndSFXOn((const char*)0x13);
                            wp->state = 0x65;
                            wp->flags |= 2;
                        } else {
                            wp->name[wp->nameLen] = *" ";
                            wp->nameLen--;
                            if (wp->nameLen < 0) {
                                wp->nameLen = 0;
                            }
                            psndSFXOn((const char*)0xB);
                        }
                    }
#else
                    if (wp->nameLen == 0 && wp->type == 0 && !strncmp(wp->name, "\x81\x40", 2)) {
                        psndSFXOn((const char*)0x13);
                        wp->state = 0x65;
                        wp->flags |= 2;
                    } else {
                        ((u16*)wp->name)[wp->nameLen] = *(u16*)"\x81\x40";
                        wp->nameLen--;
                        if (wp->nameLen < 0) {
                            wp->nameLen = 0;
                        }
                        psndSFXOn((const char*)0xB);
                    }
#endif
                } else if (keyGetDirRep(0) & DEMO_STICK_LEFT) {
                    if (dist < 10.0f) {
                        wp->cursorX--;
                        if (wp->cursorY > NAME_LANG_PICK(3, 5)) {
                            if (wp->cursorX < 0) {
                                wp->cursorX = 10;
                                if (wp->cursorY > 7) {
                                    wp->cursorY = 7;
                                }
                            }
                        } else if (wp->cursorX < 0) {
                            wp->cursorX = 13;
                        }
                        psndSFXOn((const char*)5);
                    }
                } else if (keyGetDirRep(0) & DEMO_STICK_RIGHT) {
                    if (dist < 10.0f) {
                        wp->cursorX++;
                        if (wp->cursorY > NAME_LANG_PICK(3, 5)) {
                            if (wp->cursorX >= 10 && wp->cursorY > 7) {
                                wp->cursorY = 7;
                            }
                            if (wp->cursorX >= 11) {
                                wp->cursorX = 0;
                            }
                        } else if (wp->cursorX >= NAMEENT_COLS) {
                            wp->cursorX = 0;
                        }
                        psndSFXOn((const char*)5);
                    }
                } else if (keyGetDirRep(0) & DEMO_STICK_UP) {
                    if (dist < 10.0f) {
                        wp->cursorY--;
                        if (wp->cursorY < 0) {
                            if (wp->cursorX > 9) {
                                wp->cursorY = 7;
                                wp->cursorX = 10;
                            } else {
                                wp->cursorY = 8;
                            }
                        }
                        psndSFXOn((const char*)5);
                    }
                } else if (keyGetDirRep(0) & DEMO_STICK_DOWN) {
                    if (dist < 10.0f) {
                        wp->cursorY++;
                        if (wp->cursorX > 9) {
                            if (wp->cursorY >= NAME_LANG_PICK(4, 6)) {
                                wp->cursorX = 10;
                            }
                            if (wp->cursorY >= 8) {
                                wp->cursorY = 0;
                            }
                        } else if (wp->cursorY >= 9) {
                            wp->cursorY = 0;
                        }
                        psndSFXOn((const char*)5);
                    }
                }
            }
            idx = wp->cursorX;
#ifdef VERSION_G8ME01
            if (idx > 9 && wp->cursorY > ((u32)gp->language == 0 ? 3 : 5)) {
#else
            if (idx > 9 && wp->cursorY > 3) {
#endif
                wp->x2 = 100.0f;
                wp->y2 = (f32)(-20 - (wp->cursorY - 4) * 34);
                wp->x2 -= 6.0f;
                wp->y2 += 6.0f;
            } else {
                wp->x2 = (f32)(idx * 24 - 190 + (idx / 5) * 30);
                wp->y2 = (f32)(112 - wp->cursorY * 28);
                wp->x2 -= 16.0f;
                wp->y2 -= 12.0f;
            }
            wp->maskAlpha -= 0.05f;
            if (wp->maskAlpha <= 0.0f) {
                wp->maskAlpha = 0.0f;
            }
            break;
        case 100:
            if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                if (wp->confirmSel == 0) {
                    wp->state++;
                    psndSFXOn((const char*)0x12);
                } else {
                    wp->state = 0;
                    psndSFXOn((const char*)0x13);
                }
            } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
                wp->state = 0;
                psndSFXOn((const char*)0x13);
            } else if (keyGetDirRep(0) & (DEMO_STICK_UP | DEMO_STICK_DOWN)) {
                wp->confirmSel = 1 - wp->confirmSel;
                psndSFXOn((const char*)5);
            }
            wp->x2 = -60.0f;
            wp->y2 = -30.0f - 40.0f * (f32)wp->confirmSel;
            wp->maskAlpha += 0.05f;
            if (wp->maskAlpha >= 1.0f) {
                wp->maskAlpha = 1.0f;
            }
            break;
    }
    wp->x1 += (wp->x2 - wp->x1) / FPS_EASE(4.0f);
    wp->y1 += (wp->y2 - wp->y1) / FPS_EASE(4.0f);
    wp->x1 += __fabs(2.0f * (f32)sin(gp->retraceCount / 8.0f));
    idx = wp->nameLen >= 8 ? wp->nameLen - 1 : wp->nameLen;
    wp->caretTargetX = idx * 32 - 106;
    wp->caretTargetY = 135.0f;
    wp->caretX += (wp->caretTargetX - wp->caretX) / FPS_EASE(4.0f);
    wp->caretY += (wp->caretTargetY - wp->caretY) / FPS_EASE(4.0f);
    wp->caretY += __fabs(2.0f * (f32)sin(gp->retraceCount / 8.0f));
    return 0;
}

void nameEntDisp(CameraId cameraId, void* param) {
    GXColor white2;
    GXColor white;
    GXColor unused = {0xFF, 0xFF, 0xFF, 0xFF};
    const char* str;
    s32 k;
    s32 n;
    s32 j;
    s32 i;
    CameraEntry saved;
    f32 proj[7];
    f32 vp[6];
    char buf[16];
    CameraEntry* cam;
    const char* ok;
    const char* yes;
    const char* no;
    u16 width;
    f32 w;

    white = unused;
    white2 = white;
    saved = *camGetCurPtr();
    GXGetProjectionv(proj);
    GXGetViewportv(vp);
    cam = camGetPtr(CAMERA_2D);
    *camGetCurPtr() = *cam;
    cam = camGetPtr(CAMERA_2D);
    GXSetProjection(camGetPtr(CAMERA_2D)->projection, cam->projType);
    nameBG();
    winTexInit(*wp->tplFile->data);
    winTexSet(0x2a, (Vec){-130.0f, 160.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    for (i = 0; i < 8; i++) {
        winTexSet(0x29, (Vec){(f32)(i * 32 - 106), 160.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white2);
    }
    winTexSet(0x2b, (Vec){142.0f, 160.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    nameKirinukiGX(-240.0f, 120.0f, 480.0f, 270.0f);
    FontDrawStart();
    for (j = 0; j < 9; j++) {
        for (n = 0; n < NAMEENT_COLS; n++) {
            if (wp->charTable[j * NAMEENT_COLS + n]) {
                FontDrawString((f32)(n * 24 - 190 + (n / 5) * 30 + (24 - FontGetMessageWidth(wp->charTable[j * NAMEENT_COLS + n])) / 2), (f32)(112 - j * 28), wp->charTable[j * NAMEENT_COLS + n]);
            }
        }
    }
#ifdef VERSION_G8ME01
    for (j = (u32)gp->language == 0 ? 0 : 2; j < 4; j++) {
#else
    for (j = 0; j < 4; j++) {
#endif
        if (wp->cursorX >= 10 && wp->cursorY - 4 == j) {
            winTexInit(*wp->tplFile->data);
            winTexSet(VERSION_PICK(0x33, 0x37), (Vec){160.0f, (f32)(-20 - j * 34), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 0x40});
            winTexInit_x2(*wp->tplFile->data);
            winTexSet_x2(j + 0x25, VERSION_PICK(0x33, 0x37), (Vec){154.0f, (f32)(-14 - j * 34), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white2);
        } else {
            winTexInit_x2(*wp->tplFile->data);
            winTexSet_x2(j + 0x25, VERSION_PICK(0x33, 0x37), (Vec){160.0f, (f32)(-20 - j * 34), 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white2);
        }
    }
    winTexInit(*wp->tplFile->data);
    winTexSet(0x24, (Vec){2.0f + wp->caretX, wp->caretY - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 0x80});
    winTexSet(0x24, (Vec){wp->caretX, wp->caretY, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    FontDrawStart();
    for (n = 0; n < 8; n++) {
        memset(buf, 0, sizeof(buf));
#ifdef VERSION_G8ME01
        if ((u32)gp->language == 0) {
            strncpy(buf, &wp->name[n * 2], 2);
        } else {
            strncpy(buf, &wp->name[n], 1);
        }
#else
        strncpy(buf, &wp->name[n * 2], 2);
#endif
        if (strlen(buf)) {
            FontDrawString((28.0f - FontGetMessageWidth(buf)) / 2.0f + (f32)(n * 32 - 120)
, 171.0f, buf);
        }
    }
    str = msg_tbl[wp->type];
    w = FontGetMessageWidth(msgSearch(str));
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -250.0f, -160.0f, 500.0f, 40.0f, 20.0f);
    FontDrawStart();
    FontDrawString((240.0f - w) / 2.0f + -120.0f, -168.0f, msgSearch(str));
    nameMaskGX(wp->maskAlpha);
    if (wp->state == 100) {
        ok = msgSearch("msg_nameent_ok");
        yes = msgSearch("msg_nameent_yes");
        no = msgSearch("msg_nameent_no");
        if (wp->type == 4) {
            ok = msgSearch("msg_nameent_ok_01");
        }
        nameWinGX(-180.0f, 130.0f, 360.0f, 100.0f, *wp->tplFile->data, 1);
        nameWinGX(-80.0f, 10.0f, 160.0f, 120.0f, *wp->tplFile->data, 1);
        FontDrawStart();
        FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        FontDrawEdge();
        FontDrawString(-(f32)FontGetMessageWidth(ok) / 2.0f, 110.0f, ok);
        FontDrawString(-(f32)FontGetMessageWidth(wp->name) / 2.0f, 70.0f, wp->name);
        FontDrawString(-(f32)FontGetMessageWidth(no) / 2.0f, -20.0f, yes);
        FontDrawString(-(f32)FontGetMessageWidth(no) / 2.0f, -60.0f, no);
        FontDrawEdgeOff();
    }
    winTexInit(*wp->tplFile->data);
    winTexSet(0, (Vec){2.0f + wp->x1, wp->y1 - 2.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0, 0, 0, 0x80});
    winTexSet(0, (Vec){wp->x1, wp->y1, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    *camGetCurPtr() = saved;
    GXSetProjectionv(proj);
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
}

void nameBG(void) {
    Mtx m1, m2, m3;
    GXTexObj obj1, obj2, obj3;
    f32 sy;
    double h1, w1, h2, w2, h3, w3;

    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    TEXGetGXTexObjFromPalette(*wp->tplFile->data, &obj1, 0x22);
    GXInitTexObjLOD(&obj1, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj1, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    h1 = __fabs(480.0);
    w1 = __fabs(32.0);
    sy = (f32)h1 / GXGetTexObjHeight(&obj1);
    PSMTXScale(m1, (f32)w1 / GXGetTexObjWidth(&obj1), sy, 1.0f);
    GXLoadTexMtxImm(m1, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-304.0f, 240.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(-304.0f, -240.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(-272.0f, -240.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-272.0f, 240.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);

    TEXGetGXTexObjFromPalette(*wp->tplFile->data, &obj2, 0x21);
    GXInitTexObjLOD(&obj2, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj2, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    h2 = __fabs(480.0);
    w2 = __fabs(544.0);
    sy = (f32)h2 / GXGetTexObjHeight(&obj2);
    PSMTXScale(m2, (f32)w2 / GXGetTexObjWidth(&obj2), sy, 1.0f);
    GXLoadTexMtxImm(m2, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-272.0f, 240.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(-272.0f, -240.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(272.0f, -240.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(272.0f, 240.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);

    TEXGetGXTexObjFromPalette(*wp->tplFile->data, &obj3, 0x23);
    GXInitTexObjLOD(&obj3, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj3, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    h3 = __fabs(480.0);
    w3 = __fabs(32.0);
    sy = (f32)h3 / GXGetTexObjHeight(&obj3);
    PSMTXScale(m3, (f32)w3 / GXGetTexObjWidth(&obj3), sy, 1.0f);
    GXLoadTexMtxImm(m3, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(272.0f, 240.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(272.0f, -240.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(304.0f, -240.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(304.0f, 240.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

#pragma inline_max_total_size(100000)
static inline void nameTexQuad(s32 id, f32 x, f32 y, f32 w, f32 h) {
    GXTexObj obj;
    Mtx i;
    double aw;
    double ah;
    f32 sy;

    TEXGetGXTexObjFromPalette(*wp->tplFile->data, &obj, id);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    ah = __fabs(h);
    aw = __fabs(w);
    PSMTXScale(i, (f32)aw / GXGetTexObjWidth(&obj), (f32)ah / GXGetTexObjHeight(&obj), 1.0f);
    GXLoadTexMtxImm(i, GX_TEXMTX0, GX_MTX2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x, y - h, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(x + w, y - h, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void nameKirinukiGX(f32 x, f32 y, f32 w, f32 h) {
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXLoadPosMtxImm(camGetPtr((CameraId)8)->view, 0);
    GXSetCurrentMtx(0);
    nameTexQuad(0x2F, x, y, 8, 8);
    nameTexQuad(0x2D, x + 8, y, w - 8 - 8, 8);
    nameTexQuad(0x31, x + w - 8, y, 8, 8);
    nameTexQuad(0x2E, x, y - 8, 8, h - 8 - 8);
    nameTexQuad(0x2C, x + 8, y - 8, w - 8, h - 8 - 8);
    nameTexQuad(0x30, x, y - (h - 8), 8, 8);
    nameTexQuad(0x2C, x + 8, y - (h - 8), w - 8 - 8, 8);
    nameTexQuad(0x32, x + w - 8, y - (h - 8), 8, 8);
}

void nameMaskGX(f32 t) {
    Mtx a, b, c;
    GXTexObj obj;
    f32 s;

    s = t * (t * (t * (1000.0f * t * t)));
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 0x80});
    TEXGetGXTexObjFromPalette(fadeGetTpl(), &obj, 1);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    PSMTXTrans(a, 0.5f, 0.5f, 0.0f);
    PSMTXScale(b, s, s, 0.0f);
    PSMTXTrans(c, -0.5f, -0.5f, 0.0f);
    PSMTXConcat(a, b, a);
    PSMTXConcat(a, c, a);
    GXLoadTexMtxImm(a, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(-304, 240, 0);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3s16(304, 240, 0);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3s16(304, -240, 0);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3s16(-304, -240, 0);
    GXTexCoord2f32(0.0f, 1.0f);
}

static inline void nameWinQuad(TEXPalette* tpl, s32 a, s32 b, s32 c, f32 x, f32 y, f32 w, f32 h) {
    GXTexObj obj;
    Mtx i;
    double aw2;
    double ah2;
    double aw;
    double ah;

    GXSetNumTexGens(2);
    TEXGetGXTexObjFromPalette(tpl, &obj, a);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    ah = __fabs(h);
    aw = __fabs(w);
    PSMTXScale(i, (f32)aw / GXGetTexObjWidth(&obj), (f32)ah / GXGetTexObjHeight(&obj), 1.0f);
    GXLoadTexMtxImm(i, GX_TEXMTX0, GX_MTX2x4);
    TEXGetGXTexObjFromPalette(tpl, &obj, b);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP1);
    TEXGetGXTexObjFromPalette(tpl, &obj, c);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP2);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    { ah2 = __fabs(h); aw2 = __fabs(w);
    PSMTXScale(i, (f32)aw2 / GXGetTexObjWidth(&obj), (f32)ah2 / GXGetTexObjHeight(&obj), 1.0f);
    GXLoadTexMtxImm(i, GX_TEXMTX1, GX_MTX2x4); }
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x, y - h, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(x + w, y - h, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
}

void nameWinGX(f32 x, f32 y, f32 w, f32 h, TEXPalette* tpl, s32 type) {
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0, 0, 0, 0x80 });
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_APREV, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevKAlphaSel(GX_TEVSTAGE2, 0);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXLoadPosMtxImm(camGetPtr((CameraId)8)->view, 0);
    GXSetCurrentMtx(0);
    nameWinQuad(tpl, type + 4, type + 5, type, x, y, w - 16, h - 16);
    nameWinQuad(tpl, type + 4, type + 7, type + 2, x + w - 16, y, 16, h - 16);
    nameWinQuad(tpl, type + 4, type + 6, type + 1, x, y - h + 16, w - 16, 16);
    nameWinQuad(tpl, type + 4, type + 8, type + 3, x + w - 16, y - h + 16, 16, 16);
}

