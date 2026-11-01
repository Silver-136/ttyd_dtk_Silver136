// Name entry screen: the kana/latin character grid used to name a save file or Yoshi.

#include "version.h"
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
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "sfx_name_word.h"
#include "eff/eff_magic1_n64.h"
#include "dolphin/gx.h"
#include <string.h>

void FontDrawString(f32 x, f32 y, const char* str);
void nameBG(void);
void nameKirinukiGX(f32 x, f32 y, f32 w, f32 h);
void nameWinGX(f32 x, f32 y, f32 w, f32 h, TEXPalette* tpl, s32 type);

s32 nameMain(void);
void nameEntDisp(CameraId cameraId, void* param);

//.data
#if VERSION >= VERSION_US
#define NAME_LANG_PICK(jp, en) ((u32)gp->language == LANGUAGE_JP ? (jp) : (en))
#else
#define NAME_LANG_PICK(jp, en) (jp)
#endif

#define NAMEENT_COLS 14 // characters per row of hiragana_tbl, katakana_tbl and latin_tbl

/**
 * Hiragana grid of the name-entry screen, NAMEENT_COLS characters per row, in the order they are drawn.
 * NULL marks an empty cell and a full-width space a blank key; cell 9 is patched at run time between the "n" kana
 * and a blank, depending on the name type.
 */
const char* hiragana_tbl[] = {
    "あ", "い", "う", "え", "お", "わ",
    "　", "を", "　", "ん", "０", "１",
    "２", "３", "か", "き", "く", "け",
    "こ", "が", "ぎ", "ぐ", "げ", "ご",
    "４", "５", "６", "７", "さ", "し",
    "す", "せ", "そ", "ざ", "じ", "ず",
    "ぜ", "ぞ", "８", "９", "！", "？",
    "た", "ち", "つ", "て", "と", "だ",
    "ぢ", "づ", "で", "ど", "　", "鳩",
    "☆", "♪", "な", "に", "ぬ", "ね",
    "の", "ば", "び", "ぶ", "べ", "ぼ",
    NULL,       NULL,       NULL,       NULL,       "は", "ひ",
    "ふ", "へ", "ほ", "ぱ", "ぴ", "ぷ",
    "ぺ", "ぽ", NULL,       NULL,       NULL,       NULL,
    "ま", "み", "む", "め", "も", "ぁ",
    "ぃ", "ぅ", "ぇ", "ぉ", NULL,       NULL,
    NULL,       NULL,       "や", "　", "ゆ", "　",
    "よ", "ゃ", "ゅ", "ょ", "っ", "鵜",
    NULL,       NULL,       NULL,       NULL,       "ら", "り",
    "る", "れ", "ろ", "－", "～", "・",
    "．", "　", NULL,       NULL,       NULL,       NULL
};

/** Katakana grid of the name-entry screen: the same layout as hiragana_tbl, cell for cell. */
const char* katakana_tbl[] = {
    "ア", "イ", "ウ", "エ", "オ", "ワ",
    "　", "ヲ", "　", "ン", "０", "１",
    "２", "３", "カ", "キ", "ク", "ケ",
    "コ", "ガ", "ギ", "グ", "ゲ", "ゴ",
    "４", "５", "６", "７", "サ", "シ",
    "ス", "セ", "ソ", "ザ", "ジ", "ズ",
    "ゼ", "ゾ", "８", "９", "！", "？",
    "タ", "チ", "ツ", "テ", "ト", "ダ",
    "ヂ", "ヅ", "デ", "ド", "　", "鳩",
    "☆", "♪", "ナ", "ニ", "ヌ", "ネ",
    "ノ", "バ", "ビ", "ブ", "ベ", "ボ",
    NULL,       NULL,       NULL,       NULL,       "ハ", "ヒ",
    "フ", "ヘ", "ホ", "パ", "ピ", "プ",
    "ペ", "ポ", NULL,       NULL,       NULL,       NULL,
    "マ", "ミ", "ム", "メ", "モ", "ァ",
    "ィ", "ゥ", "ェ", "ォ", NULL,       NULL,
    NULL,       NULL,       "ヤ", "　", "ユ", "　",
    "ヨ", "ャ", "ュ", "ョ", "ッ", "ヴ",
    NULL,       NULL,       NULL,       NULL,       "ラ", "リ",
    "ル", "レ", "ロ", "－", "～", "・",
    "．", "　", NULL,       NULL,       NULL,       NULL
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

/** Clears the name entry state. Called once at boot (marioStInit). */
void nameEntInit(void) {
    memset(wp, 0, sizeof(NameEntryWork));
}

/** Makes the next nameEntMain free the name entry textures and stop drawing. Used when the game over screen resets. */
void nameEntReInit(void) {
    wp->loadState = 3;
}

#if VERSION >= VERSION_US
const char* latin_tbl[] = {
    "A", "B", "C", "D", "E", "a",
    "b", "c", "d", "e", "\xFC", "\xFA",
    "\xF9", "\xFB", "F", "G", "H", "I",
    "J", "f", "g", "h", "i", "j",
    "ﾟ", "-", "/", "&", "K", "L",
    "M", "N", "O", "k", "l", "m",
    "n", "o", "｡", "!", "ｿ", "?",
    "P", "Q", "R", "S", "T", "p",
    "q", "r", "s", "t", ".", ",",
    "0", "1", "U", "V", "W", "X",
    "Y", "u", "v", "w", "x", "y",
    "2", "3", "4", "5", "Z", "ﾄ",
    "ﾁ", "ﾀ", "ﾇ", "z", "\xE4", "\xE1",
    "\xE0", "\xE2", "6", "7", "8", "9",
    "ﾉ", "ﾈ", "ﾍ", "ﾌ", "ﾑ", "\xE7",
    "\xEB", "\xE9", "\xE8", "\xEA", NULL, NULL,
    NULL, NULL, "ﾖ", "ﾓ", "ﾒ", "\x8C",
    "ﾜ", "\xEF", "\xED", "\xEC", "\xEE", "\xF1",
    NULL, NULL, NULL, NULL, "ﾚ", "ﾙ",
    "ﾐ", "ﾞ", " ", "\xF6", "\xF3", "\xF2",
    "\xF4", "\x9C", NULL, NULL, NULL, NULL
};
#endif

// The demos still load the name entry textures from the old window folder.
#if VERSION <= VERSION_DEMO_US
#define NAME_TPL_JP "%s/window/jp/name.tpl"
#else
#define NAME_TPL_JP "%s/w/jp/name.tpl"
#endif

/**
 * Starts loading the name entry textures (w/<language>/name.tpl; the JP builds always use jp,
 * the demo reads window/jp) and returns TRUE once they are loaded. Callers poll it before nameEntOn.
 */
BOOL nameEntPrepare(void) {
#if VERSION >= VERSION_US
    const char* lang[6] = {"jp", "us", "ge", "fr", "sp", "it"};
    return fileAsyncf(FILE_TYPE_TPL, NULL, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]) != 0;
#else
    return fileAsyncf(FILE_TYPE_TPL, NULL, NAME_TPL_JP, getMarioStDvdRoot()) != 0;
#endif
}

/**
 * Opens the name entry screen.
 * type selects the prompt from msg_tbl and the rules:
 * 0 the save file name (seq_load), 1 Yoshi's name (tou_10, blank gives his default name),
 * 2 and 3 guessing Doopliss's name (gra_00) before and after the Letter P is recovered,
 * 4 the password in the X-Naut Fortress (aji_01).
 * Starts with hiragana (latin outside Japanese in the US and PAL releases),
 * an empty 8-character name and the cursor at the top left.
 * Type 2 blanks the grid cell the name needs (hiragana and katakana n, latin p), so it cannot be typed yet.
 */
void nameEntOn(s32 type) {
    s32 i;

    wp->flags = 0;
    wp->type = type;
    wp->loadState = 1;
    wp->state = 0;
#if VERSION >= VERSION_US
    if ((u32)gp->language == LANGUAGE_JP) {
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
#if VERSION >= VERSION_US
        strcat(wp->name, (u32)gp->language == LANGUAGE_JP ? "　" : " ");
#else
        strcat(wp->name, "　");
#endif
    }
    wp->maskAlpha = 0.0f;
#if VERSION >= VERSION_US
    if ((u32)gp->language == LANGUAGE_JP) {
        switch (type) {
            case 2:
                hiragana_tbl[9] = "　";
                katakana_tbl[9] = "　";
                break;
            default:
                hiragana_tbl[9] = "ん";
                katakana_tbl[9] = "ン";
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
            hiragana_tbl[9] = "　";
            katakana_tbl[9] = "　";
            break;
        default:
            hiragana_tbl[9] = "ん";
            katakana_tbl[9] = "ン";
            break;
    }
#endif
}

/** Closes the name entry screen: the next nameEntMain frees its textures. */
void nameEntOff(void) {
    wp->loadState = 3;
}

/** Lets the screen take input (NAMEENT_FLAG_INPUT_ON) and returns TRUE while the player has not yet confirmed or cancelled. */
BOOL nameEntWait(void) {
    wp->flags |= NAMEENT_FLAG_INPUT_ON;
    return wp->state != 101;
}

/** TRUE when the player backed out with B on an empty file name (type 0 only). */
BOOL nameEntIsCancel(void) {
    return (wp->flags >> 1) & 1;
}

/**
 * Returns the confirmed name with trailing blanks (full-width spaces,
 * or plain spaces in the latin table) cut off, keeping at least one character;
 * NULL until the player has confirmed.
 */
const char* nameEntGetName(void) {
    s32 j;

    if (wp->state != 101) {
        return NULL;
    }
#if VERSION >= VERSION_US
    if ((u32)gp->language == LANGUAGE_JP) {
        for (j = 7; j >= 1; j--) {
            if (strncmp(&wp->name[j * 2], "　", 2)) {
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
        if (strncmp(&wp->name[j * 2], "　", 2)) {
            break;
        }
        wp->name[j * 2] = 0;
        wp->name[j * 2 + 1] = 0;
    }
#endif
    return wp->name;
}

/** Loads the language's name.tpl into tplFile once the async read has finished; FALSE while it is still loading. */
static inline BOOL nameEntLoad(void) {
#if VERSION >= VERSION_US
    const char* lang[6] = {"jp", "us", "ge", "fr", "sp", "it"};
    if (fileAsyncf(FILE_TYPE_TPL, NULL, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]) != 0U) {
        wp->tplFile = fileAllocf(FILE_TYPE_TPL, "%s/w/%s/name.tpl", getMarioStDvdRoot(), lang[gp->language]);
#else
    if (fileAsyncf(FILE_TYPE_TPL, NULL, NAME_TPL_JP, getMarioStDvdRoot()) != 0U) {
        wp->tplFile = fileAllocf(FILE_TYPE_TPL, NAME_TPL_JP, getMarioStDvdRoot());
#endif
        return TRUE;
    }
    return FALSE;
}

/**
 * Per frame: loads the textures, runs nameMain while the screen is open, or frees them after nameEntOff,
 * and queues nameEntDisp in the 3D camera layer while anything is shown.
 */
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

/** The demo has no cursor travel check, no all-blank name check and no default partner name. */
#if VERSION <= VERSION_DEMO_US
static s32 nameMain(void) {
    s32 off;
    s32 idx;
#else
s32 nameMain(void) {
    s32 len;
    s32 j;
    s32 off;
    s32 idx;
    f32 dist;

    dist = distABf(wp->x2, wp->y2, wp->x1, wp->y1);
#endif
    switch (wp->state) {
        case 0:
            if ((wp->flags & NAMEENT_FLAG_INPUT_ON) && gp->debugFlag == 0) {
                if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                    off = wp->cursorX;
#if VERSION <= VERSION_DEMO_US
                    if (off >= 10 && wp->cursorY >= 4) {
#else
#if VERSION >= VERSION_US
                    // FAKE: no-op (s32) cast; the plain ?: changes the object
                    if (off >= 10 && wp->cursorY >= (s32)((u32)gp->language == LANGUAGE_JP ? 4 : 6)) {
#else
                    if (off >= 10 && wp->cursorY >= 4) {
#endif
#endif
                        switch (wp->cursorY) {
                            case 4:
                                wp->charTable = hiragana_tbl;
                                psndSFXOn(SFX_NAME_WORD(PSND_SFX_FILE_FONT_CHANGE1 | PSND_SFX_BYPASS_MUTE));
                                break;
                            case 5:
                                wp->charTable = katakana_tbl;
                                psndSFXOn(SFX_NAME_WORD(PSND_SFX_FILE_FONT_CHANGE1 | PSND_SFX_BYPASS_MUTE));
                                break;
                            case 6:
                                if (wp->nameLen > 0) {
                                    wp->nameLen--;
                                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_FILE_MOJI_DELETE1 | PSND_SFX_BYPASS_MUTE));
                                }
                                break;
#if VERSION <= VERSION_DEMO_US
                            case 7:
                                wp->confirmSel = 1;
                                wp->state = 100;
                                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
#else
                            case 7: {
                                s32 n;
                                s32 k;
                                s32 o;
#if VERSION >= VERSION_US
                                if ((u32)gp->language == LANGUAGE_JP) {
                                    n = strlen(wp->name) / 2;
                                    for (k = 0, o = 0; k < n; o += 2, k++) {
                                        if (strncmp(&wp->name[o], "　", 2)) {
                                            break;
                                        }
                                    }
                                    if (k < n || wp->type != 0) {
                                        wp->confirmSel = 1;
                                        wp->state = 100;
                                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                                        if (wp->type == 1 && k >= n) {
                                            strcpy(wp->name, msgSearch("name_party3"));
                                            wp->nameLen = strlen(wp->name) / 2;
#if VERSION == VERSION_EU
                                            if (wp->nameLen >= 8) {
                                                wp->nameLen = 7;
                                            }
#endif
                                        }
                                    } else {
                                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_NOT_CHOICE1 | PSND_SFX_BYPASS_MUTE));
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
                                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                                        if (wp->type == 1 && m >= n) {
                                            strcpy(wp->name, msgSearch("name_party3"));
                                            wp->nameLen = strlen(wp->name);
#if VERSION == VERSION_EU
                                            if (wp->nameLen >= 8) {
                                                wp->nameLen = 7;
                                            }
#endif
                                        }
                                    } else {
                                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_NOT_CHOICE1 | PSND_SFX_BYPASS_MUTE));
                                    }
                                }
#else
                                n = strlen(wp->name) / 2;
                                for (k = 0, o = 0; k < n; o += 2, k++) {
                                    if (strncmp(&wp->name[o], "　", 2)) {
                                        break;
                                    }
                                }
                                if (k < n || wp->type != 0) {
                                    wp->confirmSel = 1;
                                    wp->state = 100;
                                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
                                    if (wp->type == 1 && k >= n) {
                                        strcpy(wp->name, msgSearch("name_party3"));
                                        wp->nameLen = strlen(wp->name) / 2;
                                    }
                                } else {
                                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_NOT_CHOICE1 | PSND_SFX_BYPASS_MUTE));
                                }
#endif
                            }
#endif
                                break;
                        }
                    } else if (wp->nameLen < 8) {
#if VERSION <= VERSION_DEMO_US
                        ((u16*)wp->name)[wp->nameLen] = *(const u16*)wp->charTable[wp->cursorY * NAMEENT_COLS + off];
                        wp->nameLen++;
                        if (wp->nameLen >= 8) {
#else
#if VERSION >= VERSION_US
                        if ((u32)gp->language == LANGUAGE_JP) {
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
#endif
                            wp->cursorX = 10;
                            wp->cursorY = 7;
                        }
#if VERSION <= VERSION_DEMO_US
                        psndSFXOn((const char*)0xA);
                    } else if (wp->nameLen == 8) {
                        ((u16*)wp->name)[wp->nameLen - 1] = *(const u16*)wp->charTable[wp->cursorY * NAMEENT_COLS + off];
                        wp->cursorX = 10;
                        wp->cursorY = 7;
                        psndSFXOn((const char*)0xA);
                    }
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_START) {
                    wp->cursorX = 10;
                    wp->cursorY = 7;
                    wp->confirmSel = 1;
                    wp->state = 100;
                    psndSFXOn((const char*)0x12);
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
                    if (wp->nameLen > 0) {
                        wp->nameLen--;
                        ((u16*)wp->name)[wp->nameLen] = *(u16*)"　";
                        psndSFXOn((const char*)0xB);
                    } else if (wp->nameLen == 0 && wp->type == 0) {
                        psndSFXOn((const char*)0x13);
                        wp->state = 101;
                        wp->flags |= NAMEENT_FLAG_CANCELLED;
                    }
                } else if (keyGetDirRep(0) & DEMO_STICK_LEFT) {
                    wp->cursorX--;
                    if (wp->cursorY > 3) {
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
                } else if (keyGetDirRep(0) & DEMO_STICK_RIGHT) {
                    wp->cursorX++;
                    if (wp->cursorY > 3) {
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
                } else if (keyGetDirRep(0) & DEMO_STICK_UP) {
                    wp->cursorY--;
                    if (wp->cursorY < 0) {
                        wp->cursorY = wp->cursorX > 9 ? 7 : 8;
                    }
                    psndSFXOn((const char*)5);
                } else if (keyGetDirRep(0) & DEMO_STICK_DOWN) {
                    wp->cursorY++;
                    if (wp->cursorX > 9) {
                        if (wp->cursorY >= 4) {
                            wp->cursorX = 10;
                        }
                        if (wp->cursorY >= 8) {
                            wp->cursorY = 0;
                        }
                    } else if (wp->cursorY >= 9) {
                        wp->cursorY = 0;
                    }
                    psndSFXOn((const char*)5);
#else
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_FILE_MOJI_SET1));
                    } else if (wp->nameLen == 8) {
                        wp->nameLen = 7;
#if VERSION >= VERSION_US
                        if ((u32)gp->language == LANGUAGE_JP) {
                            ((u16*)wp->name)[wp->nameLen] = *(u16*)wp->charTable[wp->cursorY * NAMEENT_COLS + wp->cursorX];
                        } else {
                            wp->name[wp->nameLen] = *wp->charTable[wp->cursorY * NAMEENT_COLS + wp->cursorX];
                        }
#else
                        ((u16*)wp->name)[wp->nameLen] = *(u16*)wp->charTable[wp->cursorY * NAMEENT_COLS + wp->cursorX];
#endif
                        wp->cursorX = 10;
                        wp->cursorY = 7;
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_FILE_MOJI_SET1));
                    }
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_START) {
#if VERSION >= VERSION_US
                    if ((u32)gp->language == LANGUAGE_JP) {
                        len = strlen(wp->name) / 2;
                        for (j = 0, off = 0; j < len; off += 2, j++) {
                            if (strncmp(&wp->name[off], "　", 2)) {
                                break;
                            }
                        }
                        if (j < len || wp->type != 0) {
                            wp->confirmSel = 1;
                            wp->state = 100;
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1));
                            if (wp->type == 1 && j >= len) {
                                strcpy(wp->name, msgSearch("name_party3"));
                                wp->nameLen = strlen(wp->name) / 2;
#if VERSION == VERSION_EU
                                if (wp->nameLen >= 8) {
                                    wp->nameLen = 7;
                                }
#endif
                            }
                        } else {
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_NOT_CHOICE1 | PSND_SFX_BYPASS_MUTE));
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
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1));
                            if (wp->type == 1 && m >= len) {
                                strcpy(wp->name, msgSearch("name_party3"));
                                wp->nameLen = strlen(wp->name);
#if VERSION == VERSION_EU
                                if (wp->nameLen >= 8) {
                                    wp->nameLen = 7;
                                }
#endif
                            }
                        } else {
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_NOT_CHOICE1 | PSND_SFX_BYPASS_MUTE));
                        }
                    }
#else
                    len = strlen(wp->name) / 2;
                    for (j = 0, off = 0; j < len; off += 2, j++) {
                        if (strncmp(&wp->name[off], "　", 2)) {
                            break;
                        }
                    }
                    if (j < len || wp->type != 0) {
                        wp->confirmSel = 1;
                        wp->state = 100;
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1));
                        if (wp->type == 1 && j >= len) {
                            strcpy(wp->name, msgSearch("name_party3"));
                            wp->nameLen = strlen(wp->name) / 2;
                        }
                    } else {
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_NOT_CHOICE1 | PSND_SFX_BYPASS_MUTE));
                    }
#endif
                } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
#if VERSION >= VERSION_US
                    if ((u32)gp->language == LANGUAGE_JP) {
                        if (wp->nameLen == 0 && wp->type == 0 && !strncmp(wp->name, "　", 2)) {
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1));
                            wp->state = 101;
                            wp->flags |= 2;
                        } else {
                            ((u16*)wp->name)[wp->nameLen] = *(u16*)"　";
                            wp->nameLen--;
                            if (wp->nameLen < 0) {
                                wp->nameLen = 0;
                            }
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_FILE_MOJI_DELETE1));
                        }
                    } else {
                        if (wp->nameLen == 0 && wp->type == 0 && !strncmp(wp->name, " ", 1)) {
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1));
                            wp->state = 101;
                            wp->flags |= 2;
                        } else {
                            wp->name[wp->nameLen] = *" ";
                            wp->nameLen--;
                            if (wp->nameLen < 0) {
                                wp->nameLen = 0;
                            }
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_FILE_MOJI_DELETE1));
                        }
                    }
#else
                    if (wp->nameLen == 0 && wp->type == 0 && !strncmp(wp->name, "　", 2)) {
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1));
                        wp->state = 101;
                        wp->flags |= NAMEENT_FLAG_CANCELLED;
                    } else {
                        ((u16*)wp->name)[wp->nameLen] = *(u16*)"　";
                        wp->nameLen--;
                        if (wp->nameLen < 0) {
                            wp->nameLen = 0;
                        }
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_FILE_MOJI_DELETE1));
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
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
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
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
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
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
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
                        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
                    }
#endif
                }
            }
            idx = wp->cursorX;
#if VERSION <= VERSION_DEMO_US
            if (idx > 9 && wp->cursorY > 3) {
#else
#if VERSION >= VERSION_US
            if (idx > 9 && wp->cursorY > ((u32)gp->language == LANGUAGE_JP ? 3 : 5)) {
#else
            if (idx > 9 && wp->cursorY > 3) {
#endif
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
#if VERSION <= VERSION_DEMO_US
                    psndSFXOn((const char*)0x12);
                } else {
                    wp->state = 0;
                    psndSFXOn((const char*)0x13);
#else
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1));
                } else {
                    wp->state = 0;
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1));
#endif
                }
            } else if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
                wp->state = 0;
#if VERSION <= VERSION_DEMO_US
                psndSFXOn((const char*)0x13);
            } else if (keyGetDirRep(0) & (DEMO_STICK_UP | DEMO_STICK_DOWN)) {
                wp->confirmSel = 1 - wp->confirmSel;
                psndSFXOn((const char*)5);
#else
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1));
            } else if (keyGetDirRep(0) & (DEMO_STICK_UP | DEMO_STICK_DOWN)) {
                wp->confirmSel = 1 - wp->confirmSel;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
#endif
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

/**
 * Draws the name entry screen in 2D: background, the name boxes with the 8 typed characters, the character grid,
 * the side buttons, the caret, the prompt window for this type and, while confirming,
 * the yes/no window over a darkening mask. Restores the camera, projection and viewport afterwards.
 */
#if VERSION <= VERSION_DEMO_US
static void nameEntDisp(CameraId cameraId, void* param) {
#else
void nameEntDisp(CameraId cameraId, void* param) {
#endif
    GXColor white2;
    GXColor white;
    GXColor whiteInit = {0xFF, 0xFF, 0xFF, 0xFF};
    const char* str;
    s32 n;
    s32 j;
    s32 i;
    CameraEntry saved;
    f32 proj[GX_PROJECTION_SZ];
    f32 vp[GX_VIEWPORT_SZ];
    char buf[16];
    CameraEntry* cam;
    const char* ok;
    const char* yes;
    const char* no;
    f32 w;

    // FAKE: opaque white copied through whiteInit, white and white2; one initialized local changes the object
    white = whiteInit;
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
#if VERSION <= VERSION_DEMO_US
                FontDrawString((f32)(n * 24 - 190 + (n / 5) * 30), (f32)(112 - j * 28), wp->charTable[j * NAMEENT_COLS + n]);
#else
                FontDrawString((f32)(n * 24 - 190 + (n / 5) * 30 + (24 - FontGetMessageWidth(wp->charTable[j * NAMEENT_COLS + n])) / 2), (f32)(112 - j * 28), wp->charTable[j * NAMEENT_COLS + n]);
#endif
            }
        }
    }
#if VERSION >= VERSION_US
    for (j = (u32)gp->language == LANGUAGE_JP ? 0 : 2; j < 4; j++) {
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
#if VERSION >= VERSION_US
        if ((u32)gp->language == LANGUAGE_JP) {
            strncpy(buf, &wp->name[n * 2], 2);
        } else {
            strncpy(buf, &wp->name[n], 1);
        }
#else
        strncpy(buf, &wp->name[n * 2], 2);
#endif
        if (strlen(buf)) {
            FontDrawString((28.0f - FontGetMessageWidth(buf)) / 2.0f + (f32)(n * 32 - 120), 171.0f, buf);
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
        // Both choices are placed with the width of the no string.
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

/**
 * Draws the screen background from name.tpl: a 32-pixel strip on each side (textures 0x22 and 0x23)
 * and the 544-pixel middle (0x21), all 480 high.
 */
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
/** Draws one textured quad from name.tpl texture id at (x, y), w x h, with the texture scaled to fit. */
static inline void nameTexQuad(s32 id, f32 x, f32 y, f32 w, f32 h) {
    GXTexObj obj;
    Mtx i;
    double aw;
    double ah;

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

/**
 * Draws the panel the character grid sits on (kirinuki, a cut-out) from nine name.tpl pieces:
 * 8-pixel corners and edges around the middle.
 */
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
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
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

/** Draws one window piece: texture a scaled to w x h modulated by textures b and c. */
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
    // FAKE: second pair of |w|/|h| locals for the TEXMTX1 scale; reusing aw/ah changes the object
    ah2 = __fabs(h);
    aw2 = __fabs(w);
    PSMTXScale(i, (f32)aw2 / GXGetTexObjWidth(&obj), (f32)ah2 / GXGetTexObjHeight(&obj), 1.0f);
    GXLoadTexMtxImm(i, GX_TEXMTX1, GX_MTX2x4);
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

/**
 * Draws a window frame at (x, y), w x h, from four pieces of the given texture palette starting at texture type
 * (the body and the right, bottom and corner edges, 16 pixels wide). Used for the confirm window.
 */
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
    GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_1);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    nameWinQuad(tpl, type + 4, type + 5, type, x, y, w - 16, h - 16);
    nameWinQuad(tpl, type + 4, type + 7, type + 2, x + w - 16, y, 16, h - 16);
    nameWinQuad(tpl, type + 4, type + 6, type + 1, x, y - h + 16, w - 16, 16);
    nameWinQuad(tpl, type + 4, type + 8, type + 3, x + w - 16, y - h + 16, 16, 16);
}
