#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "sfx_name_word.h"

#include "manager/fontmgr.h"
#include "driver/msgdrv.h"
#include "dolphin/mtx.h"
#include "driver/windowdrv.h"
#include "driver/icondrv.h"
#include "driver/camdrv.h"
#include "stdio.h"
#include "manager/evtmgr_cmd.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "string.h"
#include "system.h"
#include "mario/mariost.h"
#include "evt/evt_yuugijou.h"
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include "icon_ids.h"

const GXColor yuuranking_unused_color = {0xFF, 0xFF, 0xFF, 0xFF};
typedef struct YuuRankingWork {
    EventEntry* event;
    u16 flags;
    u32 dispFlags;
    f32 x;
    f32 y;
    s32 type;
    s32 mode;
    s32 score;
    s32 bonus;
    s32 mul;
    s32 rank;
    s32 max;
    s32 phase;
    s32 monte;
    s32 dispScore;
    s32 timer;
} YuuRankingWork;
#ifdef VERSION_G8ME01
// US keeps separate rankings for 60Hz (record rows 0-3) and 50Hz (rows 4-7) play.
#define YURANK(f) ((f) + (gp->framerate == 50 ? 1 : 0) * 4)
#else
#define YURANK(f) (f)
#endif
static s32 snd_id; // was a function-local static; file scope (declared before _wp: .bss is emitted reversed) keeps order _wp, snd_id
static YuuRankingWork* _wp;

static USER_FUNC(evt_yuuranking_main_func);
USER_FUNC(yuuranking_get_monte);
USER_FUNC(yuuranking_wait_button);
USER_FUNC(evt_yuuranking_free);
USER_FUNC(evt_yuuranking_alloc);
static s32 yuu_kesan(s32 type, s32 score, s32 bonus, s32 mul, s32 max);
static void evt_yuuranking_disp_func(CameraId camera, void* param);
void yuuranking_draw_msg_fit(f32 x, f32 y, f32 scale, const char* msg, s32 width);
#ifdef VERSION_G8ME01
void yuuranking_draw_msg_fit_right(f32 x, f32 y, f32 scale, const char* msg, s32 width);
void yuuranking_draw_msg_fit_center(f32 x, f32 y, f32 scale, const char* msg, s32 width);
#endif
static s32 fontlenpitch(const char* str, s32 pitch);

static EvtWord main_evt[4];

static s32 fontlenpitch(const char* str, s32 pitch) {
    s32 i;
    s32 n;

    if (pitch != 0) {
        for (n = 0, i = 0; ((const s8*)str)[i] != 0; i++) {
            if (_ismbblead(((const s8*)str)[i])) {
                n++;
                i++;
            } else {
                n++;
            }
        }
        return n * pitch;
    }
    return FontGetMessageWidth(str);
}

#ifdef VERSION_G8ME01
// US: centered in [x, x + width], squeezed to fit width.
static inline void yuuDrawCenter(f32 x, f32 y, f32 scale, const char* msg, s32 width) {
    Mtx trans;
    Mtx scl;
    Mtx mtx;
    f32 sx;
    s32 w;

    if (FontGetMessageWidth(msg) > width) {
        sx = (width * scale) / FontGetMessageWidth(msg);
    } else {
        sx = scale;
    }
    w = sx * FontGetMessageWidth(msg);
    PSMTXTrans(trans, (width - w) / 2.0f + x, y, 0.0f);
    PSMTXScale(scl, sx, scale, scale);
    PSMTXConcat(trans, scl, mtx);
    FontDrawMessageMtx(mtx, msg);
}
#endif

#ifdef VERSION_G8ME01
// US: left-aligned at x, squeezed to fit width.
static inline void yuuDrawLeft(f32 x, f32 y, f32 scale, const char* msg, s32 width) {
    Mtx trans;
    Mtx scl;
    Mtx mtx;
    f32 sx;

    PSMTXTrans(trans, x, y, 0.0f);
    if (FontGetMessageWidth(msg) > width) {
        sx = (width * scale) / FontGetMessageWidth(msg);
    } else {
        sx = scale;
    }
    PSMTXScale(scl, sx, scale, scale);
    PSMTXConcat(trans, scl, mtx);
    FontDrawMessageMtx(mtx, msg);
}
#endif

#ifdef VERSION_G8ME01
// US: right-aligned at x, squeezed to fit width.
// FAKE: x offset + work pointer instead of x, so wp->x is read after the width test as in the target
static inline void yuuDrawRight(f32 xo, f32 y, f32 scale, const char* msg, s32 width, YuuRankingWork* wp) {
    Mtx trans;
    Mtx scl;
    Mtx mtx;
    f32 sx;

    PSMTXTrans(trans, (xo + wp->x) - (scale * FontGetMessageWidth(msg) > width ? width : scale * FontGetMessageWidth(msg)), y, 0.0f);
    if (FontGetMessageWidth(msg) > width) {
        sx = (width * scale) / FontGetMessageWidth(msg);
    } else {
        sx = scale;
    }
    PSMTXScale(scl, sx, scale, scale);
    PSMTXConcat(trans, scl, mtx);
    FontDrawMessageMtx(mtx, msg);
}
#endif

void yuuranking_draw_msg_fit(f32 x, f32 y, f32 scale, const char* msg, s32 width) {
    Mtx trans;
    Mtx scl;
    Mtx mtx;
    f32 sx;

    PSMTXTrans(trans, x, y, 0.0f);
    if (FontGetMessageWidth(msg) > width) {
        sx = (width * scale) / FontGetMessageWidth(msg);
    } else {
        sx = scale;
    }
    PSMTXScale(scl, sx, scale, scale);
    PSMTXConcat(trans, scl, mtx);
    FontDrawMessageMtx(mtx, msg);
}

#ifdef VERSION_G8ME01
// US: centered text, squeezed to fit width.
void yuuranking_draw_msg_fit_center(f32 x, f32 y, f32 scale, const char* msg, s32 width) {
    Mtx trans;
    Mtx scl;
    Mtx mtx;
    f32 sx;
    s32 w;

    if (FontGetMessageWidth(msg) > width) {
        sx = (width * scale) / FontGetMessageWidth(msg);
    } else {
        sx = scale;
    }
    w = sx * FontGetMessageWidth(msg);
    PSMTXTrans(trans, (width - w) / 2.0f + x, y, 0.0f);
    PSMTXScale(scl, sx, scale, scale);
    PSMTXConcat(trans, scl, mtx);
    FontDrawMessageMtx(mtx, msg);
}

// US: right-aligned text, squeezed to fit width.
void yuuranking_draw_msg_fit_right(f32 x, f32 y, f32 scale, const char* msg, s32 width) {
    Mtx trans;
    Mtx scl;
    Mtx mtx;
    f32 sx;
    s32 w;

    if (FontGetMessageWidth(msg) > width) {
        sx = (width * scale) / FontGetMessageWidth(msg);
    } else {
        sx = scale;
    }
    w = sx * FontGetMessageWidth(msg);
    PSMTXTrans(trans, (x + width) - w, y, 0.0f);
    PSMTXScale(scl, sx, scale, scale);
    PSMTXConcat(trans, scl, mtx);
    FontDrawMessageMtx(mtx, msg);
}
#endif

static EVT_BEGIN(main_evt)
    CALL(evt_yuuranking_main_func)
    RETURN
EVT_END()

static void evt_yuuranking_disp_func(CameraId camera, void* param) {
#ifdef VERSION_G8ME01
    const char* head;
#endif
    YuuRankingWork* wp = param;
    const char* titles[4] = {"menu_title_minigame1", "menu_title_minigame4", "menu_title_minigame3", "menu_title_minigame2"};
    const char* scoreTitles[4] = {"menu_title_score", "menu_title_score", "menu_title_score", "menu_title_score"};
#ifdef VERSION_G8ME01
    const char* ranks[2][6] = {{"\x81\x40\x82\x50", "\x81\x40\x82\x51", "\x81\x40\x82\x52", "\x81\x40\x82\x53", "\x81\x40\x82\x54", "menu_subtitle2_rankgai"}, {VERSION_PICK("1", " 1"), VERSION_PICK("2", " 2"), VERSION_PICK("3", " 3"), VERSION_PICK("4", " 4"), VERSION_PICK("5", " 5"), "menu_subtitle2_rankgai"}};
#else
    const char* ranks[6] = {"\x81\x40\x82\x50", "\x81\x40\x82\x51", "\x81\x40\x82\x52", "\x81\x40\x82\x53", "\x81\x40\x82\x54", "menu_subtitle2_rankgai"};
#endif
    char buf[32];
    const char* str;
    const u8* rk;
    const char* title;
    s32 score;
    s32 sec;
    s32 min;
    s32 rscore;
    s32 rmin;
    s32 rsec;
    s32 i;
#ifdef VERSION_G8ME01
    s32 fr;
#endif
    f32 ofs;
    s32 ix;
    s32 iy;
    Mtx mtx;
    f32 scale;

    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -196.0f + wp->x, 112.0f + wp->y, 392.0f, 224.0f, 20.0f);
    switch (wp->mode) {
        case 0:
            title = msgSearch(scoreTitles[wp->type]);
            break;
        case 1:
        case 2:
            title = msgSearch(titles[wp->type]);
            break;
    }
    scale = 1.0f;
    windowDispGX_Waku_col(0, (GXColor){0x2B, 0xA3, 0xFF, 0xFF}, wp->x + -(scale * FontGetMessageWidth(title) + 50.0f) / 2.0f, scale + (128.0f + wp->y), scale * FontGetMessageWidth(title) + 50.0f, 28.0f, 8.0f);
    FontDrawStart();
    FontDrawScale(1.0f);
    FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    FontDrawMessage(wp->x + scale * -FontGetMessageWidth(title) / 2.0f, 128.0f + wp->y, title);
    if (wp->mode == 0) {
        switch (wp->type) {
            case 0:
                FontDrawStart();
                FontDrawScale(1.0f);
                FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
                yuuranking_draw_msg_fit(-150.4f + wp->x, 56.0f + wp->y, 1.0f, msgSearch("menu_subtitle1_minigame1"), 165);
                if (wp->dispScore >= 0) {
                    sprintf(buf, "%3d%s%02d", wp->dispScore / 1000, msgSearch("menu_decimal_point"), (wp->dispScore % 1000) / 10);
                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 56.0f + wp->y, 18.0f, buf);
                    if (wp->phase >= 4) {
                        FontDrawStart();
                        FontDrawScale(1.0f);
                        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
                        yuuranking_draw_msg_fit(-150.4f + wp->x, 32.0f + wp->y, 1.0f, msgSearch("menu_subtitle4_getmonte"), 165);
                        sprintf(buf, "%4d", wp->bonus);
                            FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 32.0f + wp->y, 18.0f, buf);
                    }
                    if (wp->phase >= 7) {
                        FontDrawStart();
                        FontDrawScale(1.0f);
                        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
                        if (wp->mul > 1) {
                            yuuranking_draw_msg_fit(-150.4f + wp->x, 8.0f + wp->y, 1.0f, msgSearch("menu_subtitle2_minigame1"), 165);
                            if (wp->mul < 0x100) {
                                sprintf(buf, msgSearch("menu_subtitle4_minigame1"), wp->mul);
                            } else {
                                sprintf(buf, "%2d", wp->mul - 0x100);
                            }
                                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 8.0f + wp->y, 18.0f, buf);
                        } else if (wp->mul < 0) {
                            yuuranking_draw_msg_fit(-150.4f + wp->x, 8.0f + wp->y, 1.0f, msgSearch("menu_subtitle5_minigame1"), 165);
                            sprintf(buf, msgSearch("menu_subtitle6_minigame1"), -wp->mul);
                                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 8.0f + wp->y, 18.0f, buf);
                        }
                    }
                } else {
                    FontDrawColor((GXColor){0xFF, 0x3F, 0x3F, 0xFF});
                    sprintf(buf, msgSearch("menu_error_minigame1"));
#ifdef VERSION_G8ME01
                    yuuDrawRight(150.4f, 56.0f + wp->y, 1.0f, buf, 140, wp);
#else
                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 56.0f + wp->y, 18.0f, buf);
#endif
                }
                break;
            case 3:
            case 2:
                FontDrawStart();
                FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
                switch (wp->type) {
                    case 3:
                        str = msgSearch("menu_subtitle1_minigame2");
                        break;
                    case 2:
                        str = msgSearch("menu_subtitle1_minigame3");
                        break;
                }
                yuuranking_draw_msg_fit(-150.4f + wp->x, 56.0f + wp->y, 1.0f, str, 165);
                FontDrawScale(1.0f);
#ifdef VERSION_G8ME01
                if (wp->dispScore >= 0) {
                    sec = wp->dispScore / 1000;
                    min = sec / 60;
                    sec = sec % 60;
                    sprintf(buf, "%1d%s%02d%s%02d", min, msgSearch("menu_colon"), sec, msgSearch("menu_colon"), (wp->dispScore % 1000) / 10);
#else
                score = wp->dispScore;
                if (score >= 0) {
                    sec = score / 1000;
                    min = sec / 60;
                    sec = sec % 60;
                    sprintf(buf, "%1d%s%02d%s%02d", min, msgSearch("menu_colon"), sec, msgSearch("menu_colon"), (score % 1000) / 10);
#endif
                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 56.0f + wp->y, 18.0f, buf);
                    if (wp->phase >= 4) {
                        FontDrawStart();
                        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
                        switch (wp->type) {
                            case 3:
                                str = msgSearch("menu_subtitle4_getmonte");
                                break;
                            case 2:
                                str = msgSearch("menu_subtitle4_getmonte");
                                break;
                        }
                        yuuranking_draw_msg_fit(-150.4f + wp->x, 32.0f + wp->y, 1.0f, str, 165);
                        FontDrawScale(1.0f);
                        sprintf(buf, "%4d", wp->bonus);
                            FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 32.0f + wp->y, 18.0f, buf);
                    }
                    if (wp->phase >= 7) {
                        FontDrawStart();
                        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
                        if (wp->mul > 1) {
                            switch (wp->type) {
                                case 3:
                                    str = msgSearch("menu_subtitle2_minigame2");
                                    break;
                                case 2:
                                    str = msgSearch("menu_subtitle2_minigame3");
                                    break;
                            }
                            yuuranking_draw_msg_fit(-150.4f + wp->x, 8.0f + wp->y, 1.0f, str, 165);
                            FontDrawScale(1.0f);
                            if (wp->mul < 0x100) {
                                sprintf(buf, msgSearch("menu_subtitle4_minigame1"), wp->mul);
                            } else {
                                sprintf(buf, "%2d", wp->mul - 0x100);
                            }
                                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 8.0f + wp->y, 18.0f, buf);
                        } else if (wp->mul < 0) {
                            yuuranking_draw_msg_fit(-150.4f + wp->x, 8.0f + wp->y, 1.0f, msgSearch("menu_subtitle5_minigame1"), 165);
                            FontDrawScale(1.0f);
                            sprintf(buf, msgSearch("menu_subtitle6_minigame1"), -wp->mul);
                                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 8.0f + wp->y, 18.0f, buf);
                        }
                    }
                } else {
                    FontDrawColor((GXColor){0xFF, 0x3F, 0x3F, 0xFF});
                    if (wp->type == 3) {
                        sprintf(buf, msgSearch("menu_error_minigame2"));
                    } else {
                        sprintf(buf, msgSearch("menu_error_minigame3"));
                    }
#ifdef VERSION_G8ME01
                    yuuDrawLeft(37.59999f + wp->x, 56.0f + wp->y, 1.0f, buf, 140);
#else
                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 56.0f + wp->y, 18.0f, buf);
#endif
                }
                break;
            case 1:
                FontDrawStart();
                FontDrawScale(1.0f);
                FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
                yuuranking_draw_msg_fit(-150.4f + wp->x, 56.0f + wp->y, 1.0f, msgSearch("menu_subtitle1_minigame4"), 165);
                if (wp->dispScore < wp->max) {
                    sprintf(buf, "%4d", wp->dispScore + 1);
                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 56.0f + wp->y, 18.0f, buf);
                    if (wp->phase >= 7) {
                        FontDrawStart();
                        FontDrawScale(1.0f);
                        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
                        if (wp->mul > 1) {
                            yuuranking_draw_msg_fit(-150.4f + wp->x, 8.0f + wp->y, 1.0f, msgSearch("menu_subtitle2_minigame4"), 165);
                            if (wp->mul < 0x100) {
                                sprintf(buf, msgSearch("menu_subtitle4_minigame1"), wp->mul);
                            } else {
                                sprintf(buf, "%2d", wp->mul - 0x100);
                            }
                                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 8.0f + wp->y, 18.0f, buf);
                        } else if (wp->mul < 0) {
                            yuuranking_draw_msg_fit(-150.4f + wp->x, 8.0f + wp->y, 1.0f, msgSearch("menu_subtitle5_minigame1"), 165);
                            sprintf(buf, msgSearch("menu_subtitle6_minigame1"), -wp->mul);
                                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 8.0f + wp->y, 18.0f, buf);
                        }
                    }
                } else {
                    FontDrawColor((GXColor){0xFF, 0x3F, 0x3F, 0xFF});
                    sprintf(buf, msgSearch("menu_error_minigame4"));
#ifdef VERSION_G8ME01
                    yuuDrawLeft(37.59999f + wp->x, 56.0f + wp->y, 1.0f, buf, 140);
#else
                    FontDrawStringPitch(wp->x + (150.4f - fontlenpitch(buf, 18)), 56.0f + wp->y, 18.0f, buf);
#endif
                }
                break;
        }
        FontDrawStart();
        FontDrawScale(1.0f);
        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
        yuuranking_draw_msg_fit(-150.4f + wp->x, -40.0f + wp->y, 1.0f, msgSearch("menu_subtitle3_totalmonte"), 165);
        ix = 70.0f + (10.399994f + wp->x);
        iy = (-40.0f + wp->y) - 24.0f;
        PSMTXTrans(mtx, ix - 10, iy + 2, 0.0f);
        iconDispGxCol(mtx, 0x10, ICON_ITEM_PIANTA_TOKEN, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        PSMTXTrans(mtx, ix + 60, iy + 2, 0.0f);
        iconNumberDispGx(mtx, wp->monte, FALSE, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    } else {
        FontDrawStart();
        FontDrawScale(1.0f);
#ifdef VERSION_G8ME01
        yuuDrawLeft(-150.4f + wp->x, 80.0f + wp->y, 1.0f, msgSearch("menu_subtitle5_rank"), 60);
        yuuDrawLeft(75.19998f + wp->x, 80.0f + wp->y, 1.0f, msgSearch("menu_subtitle1_monte"), 100);
        switch (wp->type) {
            case 0:
                head = msgSearch("menu_subtitle1_minigame1");
                break;
            case 3:
                head = msgSearch("menu_subtitle1_minigame2");
                break;
            case 2:
                head = msgSearch("menu_subtitle1_minigame3");
                break;
            case 1:
                head = msgSearch("menu_subtitle1_minigame4");
                break;
        }
        yuuDrawCenter(-75.200005f + wp->x, 80.0f + wp->y, 1.0f, head, 130);
#else
        FontDrawMessage(-150.4f + wp->x, 80.0f + wp->y, msgSearch("menu_subtitle5_rank"));
        FontDrawMessage(75.19998f + wp->x, 80.0f + wp->y, msgSearch("menu_subtitle1_monte"));
        FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
        switch (wp->type) {
            case 0:
                FontDrawMessage(-75.200005f + wp->x, 80.0f + wp->y, msgSearch("menu_subtitle1_minigame1"));
                break;
            case 3:
                FontDrawMessage(-75.200005f + wp->x, 80.0f + wp->y, msgSearch("menu_subtitle1_minigame2"));
                break;
            case 2:
                FontDrawMessage(-75.200005f + wp->x, 80.0f + wp->y, msgSearch("menu_subtitle1_minigame3"));
                break;
            case 1:
                FontDrawMessage(-75.200005f + wp->x, 80.0f + wp->y, msgSearch("menu_subtitle1_minigame4"));
                break;
        }
#endif
        for (i = 0; i < 6; i++) {
            if (YURANK(yuwp->recordOn)[wp->type][i] == 0) {
                continue;
            }
            ofs = 2.0f + i;
            if (i == 5) {
                ofs += 0.5f;
            }
            FontDrawStart();
            FontDrawScale(1.0f);
            if (i != wp->rank) {
                FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
            } else {
                FontDrawColor((GXColor){0xFF, 0x3F, 0x3F, 0xFF});
            }
#ifdef VERSION_G8ME01
            rk = (const u8*)ranks[gp->language != 0][i];
#else
            rk = (const u8*)ranks[i];
#endif
            if ((s32)rk[0] == 'm' && (s32)rk[1] == 'e' && (s32)rk[2] == 'n' && (s32)rk[3] == 'u' && (s32)rk[4] == '_') {
                rk = (const u8*)msgSearch((const char*)rk);
            }
            if (i == 5) {
#ifdef VERSION_G8ME01
                yuuDrawLeft(18.8f + (-188.0f + wp->x), wp->y + (104.0f - 24.0f * ofs), 1.0f, (const char*)rk, 80);
#else
                FontDrawMessage(18.8f + (-188.0f + wp->x), wp->y + (104.0f - 24.0f * ofs), (const char*)rk);
#endif
            } else {
                FontDrawStringPitch(-150.4f + wp->x, wp->y + (104.0f - 24.0f * ofs), 18.0f, (const char*)rk);
            }
#ifdef VERSION_G8ME01
            FontDrawStart();
            FontDrawScale(1.0f);
            if (i != wp->rank) {
                FontDrawColor((GXColor){0x00, 0x00, 0x00, 0xFF});
            } else {
                FontDrawColor((GXColor){0xFF, 0x3F, 0x3F, 0xFF});
            }
#endif
            switch (wp->type) {
                case 0:
#ifdef VERSION_G8ME01
                    if (YURANK(yuwp->recordScore)[wp->type][i] >= 0) {
                        sprintf(buf, "%3d%s%02d", YURANK(yuwp->recordScore)[wp->type][i] / 1000, msgSearch("menu_decimal_point"), (YURANK(yuwp->recordScore)[wp->type][i] % 1000) / 10);
#else
                    rscore = YURANK(yuwp->recordScore)[wp->type][i];
                    if (rscore >= 0) {
                        sprintf(buf, "%3d%s%02d", YURANK(yuwp->recordScore)[wp->type][i] / 1000, msgSearch("menu_decimal_point"), (rscore % 1000) / 10);
#endif
                        FontDrawStringPitch(-75.200005f + wp->x, wp->y + (104.0f - 24.0f * ofs), 18.0f, buf);
                        sprintf(buf, "%4d", YURANK(yuwp->recordParam)[wp->type][i]);
                        FontDrawStringPitch(75.19998f + wp->x, wp->y + (104.0f - 24.0f * ofs), 18.0f, buf);
                    } else {
#ifdef VERSION_G8ME01
                        yuuDrawLeft(-75.200005f + wp->x, wp->y + (104.0f - 24.0f * ofs), 1.0f, msgSearch("menu_error_minigame1"), 160);
#else
                        sprintf(buf, msgSearch("menu_error_minigame1"));
                        FontDrawMessage(-75.200005f + wp->x, wp->y + (104.0f - 24.0f * ofs), buf);
#endif
                    }
                    break;
                case 3:
                case 2:
#ifdef VERSION_G8ME01
                    if (YURANK(yuwp->recordScore)[wp->type][i] >= 0) {
                        rsec = YURANK(yuwp->recordScore)[wp->type][i] / 1000;
                        fr = gp->framerate == 50 ? 1 : 0;
#else
                    rscore = YURANK(yuwp->recordScore)[wp->type][i];
                    if (rscore >= 0) {
                        rsec = rscore / 1000;
#endif
                        rmin = rsec / 60;
                        rsec = rsec % 60;
#ifdef VERSION_G8ME01
                        sprintf(buf, "%1d%s%02d%s%02d", rmin, msgSearch("menu_colon"), rsec, msgSearch("menu_colon"), ((yuwp->recordScore + fr * 4)[wp->type][i] % 1000) / 10);
#else
                        sprintf(buf, "%1d%s%02d%s%02d", rmin, msgSearch("menu_colon"), rsec, msgSearch("menu_colon"), (rscore % 1000) / 10);
#endif
                        FontDrawStringPitch(-75.200005f + wp->x, wp->y + (104.0f - 24.0f * ofs), 18.0f, buf);
                        sprintf(buf, "%4d", YURANK(yuwp->recordParam)[wp->type][i]);
                        FontDrawStringPitch(75.19998f + wp->x, wp->y + (104.0f - 24.0f * ofs), 18.0f, buf);
                    } else {
                        if (wp->type == 3) {
                            sprintf(buf, msgSearch("menu_error_minigame2"));
                        } else {
                            sprintf(buf, msgSearch("menu_error_minigame3"));
                        }
#ifdef VERSION_G8ME01
                        yuuDrawLeft(-75.200005f + wp->x, wp->y + (104.0f - 24.0f * ofs), 1.0f, buf, 160);
#else
                        FontDrawMessage(-75.200005f + wp->x, wp->y + (104.0f - 24.0f * ofs), buf);
#endif
                    }
                    break;
                case 1:
                    if (YURANK(yuwp->recordScore)[wp->type][i] < wp->max) {
                        sprintf(buf, "%4d", YURANK(yuwp->recordScore)[wp->type][i] + 1);
#ifdef VERSION_G8ME01
                        FontDrawStringPitch(-75.200005f + wp->x, wp->y + (104.0f - 24.0f * ofs), 18.0f, buf);
#else
                        FontDrawStringPitch(-37.600006f + wp->x, wp->y + (104.0f - 24.0f * ofs), 18.0f, buf);
#endif
                        sprintf(buf, "%4d", YURANK(yuwp->recordParam)[wp->type][i]);
                        FontDrawStringPitch(75.19998f + wp->x, wp->y + (104.0f - 24.0f * ofs), 18.0f, buf);
                    } else {
#ifdef VERSION_G8ME01
                        yuuDrawLeft(-75.200005f + wp->x, wp->y + (104.0f - 24.0f * ofs), 1.0f, msgSearch("menu_error_minigame4"), 160);
#else
                        sprintf(buf, msgSearch("menu_error_minigame4"));
                        FontDrawMessage(-75.200005f + wp->x, wp->y + (104.0f - 24.0f * ofs), buf);
#endif
                    }
                    break;
            }
        }
    }
}

static USER_FUNC(evt_yuuranking_main_func) {
    YuuRankingWork* wp = (YuuRankingWork*)event->lwData[0];

    if (wp->flags & 1) {
        wp->flags |= 2;
        return EVT_RETURN_DONE;
    }
    dispEntry(CAMERA_2D, 1000.0f, 0, evt_yuuranking_disp_func, wp);
    if (wp->mode == 0) {
        switch (wp->phase) {
            case 0:
                wp->timer = 30;
                wp->phase++;
                break;
            case 1:
                if (wp->timer != 0) {
                    wp->timer--;
                    break;
                }
                wp->phase++;
                if (wp->score != 0) {
                    snd_id = psndSFXOn(SFX_NAME_WORD(PSND_SFX_EVT_GAME_RESULT_COUNT1));
                }
                break;
            case 2:
                switch (wp->type) {
                    case 0:
                        wp->dispScore += 5590;
                        if (wp->dispScore > wp->score) {
                            wp->dispScore = wp->score;
                        }
                        break;
                    case 3:
                        wp->dispScore += 1090;
                        if (wp->dispScore > wp->score) {
                            wp->dispScore = wp->score;
                        }
                        break;
                    case 2:
                        wp->dispScore += 1090;
                        if (wp->dispScore > wp->score) {
                            wp->dispScore = wp->score;
                        }
                        break;
                    case 1:
                        wp->dispScore -= 11;
                        if (wp->dispScore < wp->score) {
                            wp->dispScore = wp->score;
                        }
                        break;
                }
                wp->monte = yuu_kesan(wp->type, wp->dispScore, 0, 0, wp->max);
                if (wp->dispScore == wp->score) {
                    if (wp->score != 0) {
                        psndSFXOff(snd_id);
                    }
                    switch (wp->type) {
                        case 0:
                        case 3:
                        case 2:
                            if (wp->score > 0) {
                                wp->timer = 48;
                                wp->phase++;
                            } else {
                                wp->timer = 30;
                                wp->phase = 9;
                            }
                            break;
                        case 1:
                            if (wp->mul > 1 || wp->mul < 0) {
                                if (wp->score < wp->max) {
                                    wp->timer = 48;
                                    wp->phase = 6;
                                } else {
                                    wp->timer = 30;
                                    wp->phase = 9;
                                }
                            } else {
                                wp->timer = 30;
                                wp->phase = 9;
                            }
                            break;
                    }
                }
                break;
            case 3:
                if (wp->timer != 0) {
                    wp->timer--;
                    break;
                }
                wp->timer = 48;
                wp->phase++;
                break;
            case 4:
                if (wp->timer != 0) {
                    wp->timer--;
                    break;
                }
                wp->phase++;
                break;
            case 5: {
                s32 target;

                switch (wp->type) {
                    case 0:
                    case 3:
                    case 2:
                        target = yuu_kesan(wp->type, wp->dispScore, wp->bonus, 0, wp->max);
                        if (wp->monte < target) {
                            wp->monte++;
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_EVT_GAME_MONTE_COUNT1));
                        } else if (wp->monte > target) {
                            wp->monte--;
                        } else if (wp->score > 0 && (wp->mul > 1 || wp->mul < 0)) {
                            wp->timer = 48;
                            wp->phase++;
                        } else {
                            wp->timer = 30;
                            wp->phase = 9;
                        }
                        break;
                }
                break;
            }
            case 6:
                if (wp->timer != 0) {
                    wp->timer--;
                    break;
                }
                wp->timer = 48;
                wp->phase++;
                break;
            case 7:
                if (wp->timer != 0) {
                    wp->timer--;
                    break;
                }
                wp->phase++;
                break;
            case 8: {
                s32 target;

                switch (wp->type) {
                    case 0:
                    case 3:
                    case 2:
                    case 1:
                        target = yuu_kesan(wp->type, wp->dispScore, wp->bonus, wp->mul, wp->max);
                        if (wp->monte < target) {
                            wp->monte++;
                            psndSFXOn(SFX_NAME_WORD(PSND_SFX_EVT_GAME_MONTE_COUNT1));
                        } else if (wp->monte > target) {
                            wp->monte--;
                        } else {
                            wp->timer = 30;
                            wp->phase++;
                        }
                        break;
                }
                break;
            }
            case 9:
                wp->dispFlags &= ~4;
                break;
        }
    }
    return EVT_RETURN_BLOCK;
}

static s32 yuu_kesan(s32 type, s32 score, s32 bonus, s32 mul, s32 max) {
    s32 ret;

    if (score < 0) {
        return 0;
    }
    switch (type) {
        case 0:
            ret = score / 30000;
            break;
        case 3:
            ret = (score - 74000) / 2000;
            break;
        case 2:
            ret = (score - 109000) / 2000;
            break;
        case 1:
            ret = ((max - score) * 15) / max;
            break;
    }
    if (ret < 0) {
        ret = 0;
    }
    ret += bonus;
    if (mul > 0) {
        if (mul < 0x100) {
            ret *= mul;
        } else {
            ret += mul - 0x100;
        }
    } else {
        ret += mul;
        if (ret < 0) {
            ret = 0;
        }
    }
    if (ret < 0) {
        ret = 0;
    }
    return ret;
}

USER_FUNC(evt_yuuranking_alloc) {
    EvtWord* args = event->args;
    YuuRankingWork* wp = _mapAlloc(sizeof(YuuRankingWork));
#ifdef VERSION_G8ME01
    s32 score;
    s32 i;
    s32 j;
#else
    s32 i;
    s32 j;
    s32 score;
#endif

    memset(wp, 0, sizeof(YuuRankingWork));
    _wp = wp;
    wp->x = evtGetFloat(event, args[0]);
    wp->y = evtGetFloat(event, args[1]);
    wp->type = evtGetValue(event, args[2]);
    wp->mode = evtGetValue(event, args[3]);
    wp->score = evtGetValue(event, args[4]);
    wp->bonus = evtGetValue(event, args[5]);
    wp->mul = evtGetValue(event, args[6]);
    switch (wp->type) {
        case 0:
            YURANK(yuwp->recordOn)[wp->type][5] = 0;
            YURANK(yuwp->recordScore)[wp->type][5] = 0;
            YURANK(yuwp->recordParam)[wp->type][5] = 0;
            wp->rank = -1;
            if (wp->mode == 1) {
                score = wp->score;
                if (score >= 0) {
                    for (i = 0; i < 6; i++) {
                        if (YURANK(yuwp->recordOn)[wp->type][i] == 0) {
                            YURANK(yuwp->recordOn)[wp->type][i] = 1;
                            YURANK(yuwp->recordScore)[wp->type][i] = wp->score;
                            YURANK(yuwp->recordParam)[wp->type][i] = yuu_kesan(wp->type, wp->score, wp->bonus, wp->mul, wp->max);
                            wp->rank = i;
                            break;
                        }
                        if (score >= YURANK(yuwp->recordScore)[wp->type][i]) {
                            for (j = 4; j > i; j--) {
                                YURANK(yuwp->recordOn)[wp->type][j] = YURANK(yuwp->recordOn)[wp->type][j - 1];
                                YURANK(yuwp->recordScore)[wp->type][j] = YURANK(yuwp->recordScore)[wp->type][j - 1];
                                YURANK(yuwp->recordParam)[wp->type][j] = YURANK(yuwp->recordParam)[wp->type][j - 1];
                            }
                            YURANK(yuwp->recordOn)[wp->type][i] = 1;
                            YURANK(yuwp->recordScore)[wp->type][i] = wp->score;
                            YURANK(yuwp->recordParam)[wp->type][i] = yuu_kesan(wp->type, wp->score, wp->bonus, wp->mul, wp->max);
                            wp->rank = i;
                            if (i == 0) {
                                evtSetValue(NULL, GSWF(1240), TRUE);
                            }
                            break;
                        }
                    }
                } else {
                    YURANK(yuwp->recordOn)[wp->type][5] = 1;
                    YURANK(yuwp->recordScore)[wp->type][5] = wp->score;
                    YURANK(yuwp->recordParam)[wp->type][5] = 0;
                    wp->rank = 5;
                }
            } else if (wp->mode == 0) {
                wp->dispScore = 0;
                if (wp->score < wp->dispScore) {
                    wp->dispScore = wp->score;
                }
            }
            break;
        case 3:
        case 2:
            YURANK(yuwp->recordOn)[wp->type][5] = 0;
            YURANK(yuwp->recordScore)[wp->type][5] = 0;
            YURANK(yuwp->recordParam)[wp->type][5] = 0;
            wp->rank = -1;
            if (wp->mode == 1) {
                score = wp->score;
                if (score >= 0) {
                    for (i = 0; i < 6; i++) {
                        if (YURANK(yuwp->recordOn)[wp->type][i] == 0) {
                            YURANK(yuwp->recordOn)[wp->type][i] = 1;
                            YURANK(yuwp->recordScore)[wp->type][i] = wp->score;
                            YURANK(yuwp->recordParam)[wp->type][i] = yuu_kesan(wp->type, wp->score, wp->bonus, wp->mul, wp->max);
                            wp->rank = i;
                            break;
                        }
                        if (score >= YURANK(yuwp->recordScore)[wp->type][i]) {
                            for (j = 4; j > i; j--) {
                                YURANK(yuwp->recordOn)[wp->type][j] = YURANK(yuwp->recordOn)[wp->type][j - 1];
                                YURANK(yuwp->recordScore)[wp->type][j] = YURANK(yuwp->recordScore)[wp->type][j - 1];
                                YURANK(yuwp->recordParam)[wp->type][j] = YURANK(yuwp->recordParam)[wp->type][j - 1];
                            }
                            YURANK(yuwp->recordOn)[wp->type][i] = 1;
                            YURANK(yuwp->recordScore)[wp->type][i] = wp->score;
                            YURANK(yuwp->recordParam)[wp->type][i] = yuu_kesan(wp->type, wp->score, wp->bonus, wp->mul, wp->max);
                            wp->rank = i;
                            if (i == 0) {
                                if (wp->type == 3) {
                                    evtSetValue(NULL, GSWF(1241), TRUE);
                                } else {
                                    evtSetValue(NULL, GSWF(1242), TRUE);
                                }
                            }
                            break;
                        }
                    }
                } else {
                    YURANK(yuwp->recordOn)[wp->type][5] = 1;
                    YURANK(yuwp->recordScore)[wp->type][5] = wp->score;
                    YURANK(yuwp->recordParam)[wp->type][5] = 0;
                    wp->rank = 5;
                }
            } else if (wp->mode == 0) {
                wp->dispScore = 0;
                if (wp->score < wp->dispScore) {
                    wp->dispScore = wp->score;
                }
            }
            break;
        case 1:
            wp->max = evtGetValue(event, args[7]);
            YURANK(yuwp->recordOn)[wp->type][5] = 0;
            YURANK(yuwp->recordScore)[wp->type][5] = 0;
            YURANK(yuwp->recordParam)[wp->type][5] = 0;
            wp->rank = -1;
            if (wp->mode == 1) {
                score = wp->score;
                if (score < wp->max) {
                    for (i = 0; i < 6; i++) {
                        if (YURANK(yuwp->recordOn)[wp->type][i] == 0) {
                            YURANK(yuwp->recordOn)[wp->type][i] = 1;
                            YURANK(yuwp->recordScore)[wp->type][i] = wp->score;
                            YURANK(yuwp->recordParam)[wp->type][i] = yuu_kesan(wp->type, wp->score, wp->bonus, wp->mul, wp->max);
                            wp->rank = i;
                            break;
                        }
                        if (score <= YURANK(yuwp->recordScore)[wp->type][i]) {
                            for (j = 4; j > i; j--) {
                                YURANK(yuwp->recordOn)[wp->type][j] = YURANK(yuwp->recordOn)[wp->type][j - 1];
                                YURANK(yuwp->recordScore)[wp->type][j] = YURANK(yuwp->recordScore)[wp->type][j - 1];
                                YURANK(yuwp->recordParam)[wp->type][j] = YURANK(yuwp->recordParam)[wp->type][j - 1];
                            }
                            YURANK(yuwp->recordOn)[wp->type][i] = 1;
                            YURANK(yuwp->recordScore)[wp->type][i] = wp->score;
                            YURANK(yuwp->recordParam)[wp->type][i] = yuu_kesan(wp->type, wp->score, wp->bonus, wp->mul, wp->max);
                            wp->rank = i;
                            if (i == 0) {
                                evtSetValue(NULL, GSWF(1243), TRUE);
                            }
                            break;
                        }
                    }
                } else {
                    YURANK(yuwp->recordOn)[wp->type][5] = 1;
                    YURANK(yuwp->recordScore)[wp->type][5] = wp->score;
                    YURANK(yuwp->recordParam)[wp->type][5] = 0;
                    wp->rank = 5;
                }
            } else {
                if (wp->mode == 2) {
                    wp->max = 10000;
                }
                if (wp->mode == 0) {
                    wp->dispScore = wp->max - 1;
                    if (wp->score > wp->dispScore) {
                        wp->dispScore = wp->score;
                    }
                }
            }
            break;
    }
    if (wp->mode == 0) {
        wp->dispFlags |= 4;
    }
    wp->event = evtEntry(main_evt, 0, 0);
    wp->event->lwData[0] = (EvtWord)wp;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_yuuranking_free) {
    YuuRankingWork* wp = _wp;

    if (wp == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        wp->flags |= 1;
    }
    if (!(wp->flags & 2)) {
        return EVT_RETURN_BLOCK;
    }
    _mapFree(_wp);
    _wp = NULL;
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuranking_wait_button) {
    YuuRankingWork* wp = _wp;

    if (isFirstCall) {
        wp->dispFlags |= 2;
    }
    if (!(wp->dispFlags & 4) && (keyGetButtonTrg(0) & (PAD_BUTTON_A | PAD_BUTTON_B))) {
        wp->dispFlags &= ~2;
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1));
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(yuuranking_get_monte) {
    evtSetValue(event, event->args[0], _wp->monte);
    return EVT_RETURN_DONE;
}
