#include "win/win_main.h"
#include "eff/eff_recovery.h"
#include "data/item_data.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/seqdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/fontmgr.h"
#include "mario/mario.h"
#include "memory.h"
#include "party.h"
#include "system.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include <string.h>
#include <stdarg.h>
#include "evt/evt_yuugijou.h"
#include "koopa.h"
#include "eff/eff_magic1_n64.h"
#include "party/party_motion.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
#include "evt/evt_fade.h"
#include "evt/evt_item.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "win/win_root.h"
#include "driver/itemdrv.h"
#ifdef VERSION_G8ME01
char* strstr(const char* str, const char* sub);
#endif
#include "driver/msgdrv.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "win/win_party.h"
#include "libc_decls.h"

extern ItemData itemDataTable[];

#define HALF_F float_0p5_8041dc98
USER_FUNC(cam_r);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(lecture_key_mask_off);
USER_FUNC(lecture_step_wait);
USER_FUNC(all_party_slit_off);
USER_FUNC(party_color);
USER_FUNC(famicom_check);
USER_FUNC(itemUseFunc2);
EvtWord evt_lecture_msg[];
EvtWord evt_item_use[];
EvtWord evt_diary_read[];

//.data

//.sbss
static s32 cnt;
TEXPalette* winTexTpl;
static WindowWork* wp;

// local prototypes
#include "driver/fadedrv.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "driver/swdrv.h"
#include "driver/effdrv.h"
#include "manager/filemgr.h"
#include "mario/mario_party.h"
#include "pmario_sound.h"
#include "statuswindow.h"
#include "data/party_data.h"
#include "manager/evtmgr_cmd.h"
void FontDrawStringVecPitch(Vec* pos, const char* msg, f32 pitch);
void winLectureKeyMask(void);
void iconGetTexObj(GXTexObj* obj, u16 iconId);
void winDisp(CameraId camId, void* param);
void winDispKoopa(CameraId camId, void* param);


WindowWork* winGetPtr(void) {
    return wp;
}

void winInit(void) {
    wp = __memAlloc(HEAP_DEFAULT, sizeof(WindowWork));
    memset(wp, 0, sizeof(WindowWork));
}

void winReInit(void) {
    memset(wp, 0, sizeof(WindowWork));
}

BOOL winCheck(void) {
    s32 state;

    if (wp == NULL) {
        return FALSE;
    }

    if (wp->flags & WINDOW_FLAG_LECTURE) {
        return FALSE;
    }

    state = wp->menuState;
    if (state != 0 && state < 200) {
        return TRUE;
    }
    return FALSE;
}

void winOpenEnable(void) {
    if (wp) {
        wp->flags &= ~WINDOW_FLAG_OPEN_DISABLE;
    }
}

void winOpenDisable(void) {
    if (wp) {
        wp->flags |= WINDOW_FLAG_OPEN_DISABLE;
    }
}

#ifdef VERSION_G8ME01
typedef struct { const char* s[6]; } WinLangTbl;
#else
typedef struct { const char* s[2]; } WinLangTbl;
#endif
static const WinLangTbl winLangTbl;

void winMain(void) {
    MarioWork* mario;
    s32 i;

    mario = marioGetPtr();
    if (!(wp->flags & WINDOW_FLAG_OPEN_DISABLE) && seqGetSeq() == SEQ_GAME && !seqCheckSeq()) {
        wp->buttonTrg = keyGetButtonTrg(0);
        wp->buttonRep = keyGetButtonRep(0);
        wp->dirTrg = keyGetDirTrg(0);
        wp->dirRep = keyGetDirRep(0);

        if (keyGetButtonTrg(0) & PAD_BUTTON_LEFT) {
            wp->dirTrg |= DEMO_STICK_LEFT;
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_RIGHT) {
            wp->dirTrg |= DEMO_STICK_RIGHT;
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_UP) {
            wp->dirTrg |= DEMO_STICK_UP;
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_DOWN) {
            wp->dirTrg |= DEMO_STICK_DOWN;
        }

        if (keyGetButtonRep(0) & PAD_BUTTON_LEFT) {
            wp->dirRep |= DEMO_STICK_LEFT;
        }
        if (keyGetButtonRep(0) & PAD_BUTTON_RIGHT) {
            wp->dirRep |= DEMO_STICK_RIGHT;
        }
        if (keyGetButtonRep(0) & PAD_BUTTON_UP) {
            wp->dirRep |= DEMO_STICK_UP;
        }
        if (keyGetButtonRep(0) & PAD_BUTTON_DOWN) {
            wp->dirRep |= DEMO_STICK_DOWN;
        }

        winLectureKeyMask();

        switch (wp->menuState) {
            case WIN_MENU_IDLE:
                if (!(wp->flags & WINDOW_FLAG_LECTURE)) {
                    if (marioStGetSystemLevel() != 0) {
                        break;
                    }
                    if (marioCheckMenuDisable()) {
                        break;
                    }
                    if (mobjCheckExec()) {
                        break;
                    }
                    if (!fadeIsFinish()) {
                        break;
                    }
                    if (mario->characterId == CHARACTER_BOWSER) {
                        if (!(wp->buttonTrg & PAD_BUTTON_START)) {
                            break;
                        }
                        wp->menuSel = 0;
                    } else if (wp->buttonTrg & PAD_BUTTON_START) {
                        wp->menuSel = 0;
                    } else {
                        if (keyGetButton(0) & PAD_TRIGGER_Z) {
                            break;
                        }
                        if (strcmp(gp->currentAreaName, "yuu") == 0) {
                            break;
                        }
                        if ((wp->buttonTrg & PAD_BUTTON_LEFT) && (swByteGet(0) >= STG0_07_GOOMBELLA_JOINS || partyChkJoin(PARTNER_GOOMBELLA))) {
                            wp->menuSel = 1;
                        } else if (wp->buttonTrg & PAD_BUTTON_UP) {
                            wp->menuSel = 2;
                        } else if ((wp->buttonTrg & PAD_BUTTON_RIGHT) && (swByteGet(0) >= STG0_17_BADGE_TUTORIAL || pouchGetHaveBadgeCnt())) {
                            wp->menuSel = 3;
                        } else if (wp->buttonTrg & PAD_BUTTON_DOWN) {
                            wp->menuSel = 4;
                        } else {
                            break;
                        }
                    }
                    if (keyGetButton(0) & PAD_TRIGGER_L) {
                        break;
                    }
#ifdef VERSION_G8ME01
                    if ((gp->flags & GW_FLAG_E3) && evtGetValue(NULL, GSW_STORY_PROGRESS) < STG0_07_GOOMBELLA_JOINS) {
                        break;
                    }
#endif
                } else {
                    wp->menuSel = 0;
                }
                camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
                if (gp->flags & GW_FLAG_E3) {
                    wp->menuSel = 1;
                }
                if (!(wp->flags & WINDOW_FLAG_LECTURE)) {
                    marioStSystemLevel(4);
                } else {
                    npcStopForEvent();
                }
                fbatChangeMode(7);
                if (mario->characterId == CHARACTER_BOWSER) {
                    if (!kpaGetStageViewType()) {
                        marioCtrlOff();
                    } else {
                        marioCtrlOff2();
                    }
                } else {
                    marioCtrlOff2();
                }
                partyCtrlOff();
                psndSetFlag(PSND_FLAG_MUTE_SFX);
                if (mario->characterId == CHARACTER_BOWSER) {
                    kpaPauseSE2();
                }
                if (mario->flags & MARIO_FLAG_IS_8_BIT) {
                    wp->flags |= WINDOW_FLAG_8BIT;
                } else {
                    wp->flags &= ~WINDOW_FLAG_8BIT;
                }
                for (i = 0; i < 4; i++) {
                    padRumbleHardOff(i);
                }
#ifdef VERSION_G8ME01
                fileAsyncf(4, 0, "%s/w/%s/mail.tpl", getMarioStDvdRoot(), (u32)gp->language == LANGUAGE_JP ? "jp" : "us");
#else
                fileAsyncf(4, 0, "%s/w/jp/mail.tpl", getMarioStDvdRoot());
#endif
                {
#ifdef VERSION_G8ME01
                    fileAsyncf(4, 0, "%s/w/%s/%s", getMarioStDvdRoot(), (u32)gp->language == LANGUAGE_JP ? "jp" : "us", winGetMapTplName());
#else
                    const char* name = winGetMapTplName();
                    fileAsyncf(4, 0, "%s/w/jp/%s", getMarioStDvdRoot(), name);
#endif
                }
                wp->active = 0;
                if (strncmp(gp->currentMapName, "yuu_", 4) == 0) {
                    wp->menuState = WIN_MENU_YUU;
                } else {
                    wp->menuState++;
                }
                break;

            case WIN_MENU_LOAD: {
                WinLangTbl lang = winLangTbl;
                BOOL ok;

                if (fileAsyncf(4, 0, "%s/w/%s/win.tpl", getMarioStDvdRoot(), lang.s[gp->language]) == 0U) {
                    ok = FALSE;
                } else {
                    wp->res = fileAllocf(4, "%s/w/%s/win.tpl", getMarioStDvdRoot(), lang.s[gp->language]);
                    ok = TRUE;
                }
                if (ok) {
                    wp->menuState++;
                }
                break;
            }

            case WIN_MENU_ROOT:
                if (!marioGetPtr()->characterId) {
                    switch (winRootMain(wp)) {
                        case -1:
                            wp->menuState++;
                            break;
                        case -2:
                            wp->menuState = WIN_MENU_PARTY_LOAD;
                            break;
                        case -3:
                            if (wp->item.useItem == 0x4E) {
                                wp->menuState = WIN_MENU_DIARY;
                            } else {
                                wp->menuState = WIN_MENU_ITEM_USE;
                            }
                            break;
                    }
                } else if (wp->buttonTrg & PAD_BUTTON_START) {
                    kpaPauseSE2();
                    wp->menuState = WIN_MENU_CLOSE;
                }
                break;

            case WIN_MENU_CLOSE:
                camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
                if (mario->characterId == CHARACTER_BOWSER) {
                    if (!kpaGetStageViewType()) {
                        marioCtrlOn();
                    } else {
                        marioCtrlOn2();
                    }
                } else {
                    marioCtrlOn2();
                }
                partyCtrlOn();
                fbatChangeMode(1);
                psndClearFlag(PSND_FLAG_MUTE_SFX);
                statusWinForceUpdate();
                if (!(wp->flags & WINDOW_FLAG_LECTURE)) {
                    marioStSystemLevel(0);
                } else {
                    npcStartForEvent();
                }
                if (wp->res) {
                    fileFree(wp->res);
                }
                wp->res = NULL;
                wp->flags = 0;
                wp->menuState = WIN_MENU_IDLE;
                break;

            case WIN_MENU_PARTY_LOAD:
                if (marioBgmodeChk()) {
                    MarioWork* m = marioGetPtr();
                    m->prevPartyMemberId[0] = wp->party.partyIds[wp->party.cursor];
                    wp->menuState = WIN_MENU_CLOSE;
                } else if (animGroupBaseAsync(partyDataTbl[wp->party.partyIds[wp->party.cursor]].name, 2, 0)) {
                    wp->menuState++;
                }
                break;

            case WIN_MENU_PARTY_OUT:
                marioPartyGoodbye();
                wp->menuState++;
                break;

            case WIN_MENU_PARTY_IN:
                if (!marioGetParty()) {
                    marioPartyHello(wp->party.partyIds[wp->party.cursor]);
                    wp->menuState = WIN_MENU_CLOSE;
                }
                break;

            case WIN_MENU_ITEM_USE: {
#ifdef VERSION_G8ME01
                WindowPartyDt* list[11]; /* FAKE */ // US frame is 96 bytes; only 7 entries used
#else
                WindowPartyDt* list[7];
#endif
                WindowPartyDt* p;
                WindowPartyDt* base;
                s32 partyId;
                s32 n;

                partyId = !marioBgmodeChk() ? marioGetParty() : marioGetPtr()->prevPartyMemberId[0];
                base = winPartyDt;
                n = 0;
                for (i = 0, p = base; i < 7; i++, p++) {
                    if (partyId == p->partyId) {
                        list[n++] = p;
                    }
                }
                for (i = 0; i < 7; i++, base++) {
                    if (partyId != base->partyId) {
                        list[n++] = base;
                    }
                }
                wp->currentEvent = evtEntryType(evt_item_use, 0, 0, 0);
                wp->currentEvent->lwData[10] = wp->item.useItem;
                wp->currentEvent->lwData[11] = wp->item.useTarget == 0 ? 0 : list[wp->item.useTarget - 1]->partyId;
                psndClearFlag(PSND_FLAG_MUTE_SFX);
                wp->menuState++;
                pouchRemoveItemIndex(wp->item.useItem, wp->item.useIdx);
                break;
            }

            case WIN_MENU_ITEM_USE_WAIT:
                if (!evtCheckID(wp->currentEvent->eventId)) {
                    wp->menuState = WIN_MENU_CLOSE;
                }
                break;

            case WIN_MENU_DIARY:
                wp->currentEvent = evtEntryType(evt_diary_read, 0, 0, 0);
                wp->menuState++;
                break;

            case WIN_MENU_DIARY_WAIT:
                if (!evtCheckID(wp->currentEvent->eventId)) {
                    wp->menuState = WIN_MENU_CLOSE;
                }
                break;

            case WIN_MENU_YUU:
                evtYuuWindow();
                wp->menuState++;
                break;

            case WIN_MENU_YUU_WAIT:
                if (evtYuuWindowEndChk()) {
                    wp->menuState = WIN_MENU_CLOSE;
                }
                break;
        }

        if (wp->menuState == WIN_MENU_ROOT) {
            switch (marioGetPtr()->characterId) {
                case 0:
                    if (!(wp->flags & WINDOW_FLAG_LECTURE)) {
                        dispEntry(CAMERA_2D, 250.0f, 0, winDisp, NULL);
                    } else {
                        dispEntry(CAMERA_2D, 500.0f, 0, winDisp, NULL);
                    }
                    break;
                case 2:
                    dispEntry(CAMERA_2D, 250.0f, 0, winDispKoopa, NULL);
                    break;
            }
        }
    }
}

void winDisp(CameraId camId, void* param) {
    winRootDisp(camId, wp);
}

void winDispKoopa(CameraId camId, void* param) {
    winTexInit(*wp->res->data);
    winTexSet(0xB7, (Vec){ 0.0f, 50.0f, 0.0f }, (Vec){ 1.0f, 1.0f, 1.0f }, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
}

#ifdef VERSION_G8ME01
static const WinLangTbl winLangTbl = { { "jp", "us", "ge", "fr", "sp", "it" } };
#else
static const WinLangTbl winLangTbl = { { "jp", "us" } };
#endif


EVT_BEGIN(kime_pose)
    CALL(evt_party_nokotaro_get_status, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_I_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_I_1"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("I_1"))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING("PYS_I_1"))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("PWD_I_1"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("PTR_I_1"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("PCH_I_1"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kime_pose2)
    CALL(evt_party_nokotaro_get_status, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_Y_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_Y_1"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("Y_1"))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING("PYS_Y_1"))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("PWD_Y_1"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("PTR_Y_1"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("PCH_Y_1"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(win_main_child_evt)
    CALL(evt_party_nokotaro_get_status, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_party_set_breed_pose, 0, 1)
    RETURN
EVT_END()

EVT_BEGIN(kime_pose_famicom)
    CALL(evt_party_nokotaro_get_status, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("R_2"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("R_3"))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING("R_13"))
        CASE_EQUAL(4)
        CALL(party_color, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_party_set_pose, 0, STRING("R_5"))
            CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("R_6"))
            CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("R_7"))
            CASE_EQUAL(3)
            CALL(evt_party_set_pose, 0, STRING("R_8"))
            CASE_EQUAL(4)
            CALL(evt_party_set_pose, 0, STRING("R_9"))
            CASE_EQUAL(5)
            CALL(evt_party_set_pose, 0, STRING("R_10"))
            CASE_EQUAL(6)
            CALL(evt_party_set_pose, 0, STRING("R_11"))
        END_SWITCH
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING("R_4"))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING("R_12"))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING("R_15"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(evt_item_use)
    SET_TYPE(0)
    CALL(evt_mario_normalize)
    CALL(all_party_slit_off)
    WAIT_FRAMES(1)
    CALL(evt_mario_adjust_dir)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL_CHILD(PTR(win_main_child_evt))
    IF_INT_EQUAL(LW(11), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_item_entry, STRING("menu_dummy"), LW(10), LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        ADD(LW(1), 10)
        CALL(evt_eff, 0, STRING("itemget"), 10, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_MARIO_USE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_I_1"))
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("menu_dummy"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_RECOVERY_SHINE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff64, 0, STRING("stardust2_n64"), 4, LW(0), LW(1), LW(2), 40, 40, 30, 120, 0, 0, 0, 0)
        WAIT_MS(1400)
        CALL(evt_mario_set_pose, STRING("M_V_2"))
        CALL(itemUseFunc)
        WAIT_MS(1500)
        CALL(itemUseFunc2)
        CALL(evt_mario_set_normal_pose)
        RETURN
    END_IF
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), LW(11))
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 50)
        CALL(evt_item_entry, STRING("menu_dummy"), LW(10), LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
        ADD(LW(1), 10)
        CALL(evt_eff, 0, STRING("itemget"), 10, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_MARIO_USE1"), LW(0), LW(1), LW(2), 0)
        CALL(famicom_check, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(kime_pose))
            ELSE
            CALL_CHILD(PTR(kime_pose_famicom))
        END_IF
        WAIT_MS(1000)
        CALL(evt_item_delete, STRING("menu_dummy"))
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_RECOVERY_SHINE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff64, 0, STRING("stardust2_n64"), 4, LW(0), LW(1), LW(2), 40, 40, 30, 120, 0, 0, 0, 0)
        WAIT_MS(1400)
        CALL(famicom_check, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(kime_pose2))
            ELSE
            CALL_CHILD(PTR(kime_pose_famicom))
        END_IF
        CALL(itemUseFunc)
        WAIT_MS(1500)
        CALL(itemUseFunc2)
        CALL_CHILD(PTR(win_main_child_evt))
        CALL(evt_mario_set_normal_pose)
        RETURN
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_diary_read)
    SET_TYPE(0)
    CALL(evt_party_cont_onoff, 0, 1)
    WAIT_FRAMES(1)
    CALL(evt_mario_normalize)
    CALL(evt_msg_print, 0, STRING("stg6_rsh_diary_01"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg6_rsh_diary_01_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_continue)
        CALL(evt_party_cont_onoff, 0, 0)
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("stg6_rsh_diary_02"))
    CALL(evt_msg_select, 0, STRING("stg6_rsh_diary_01_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_continue)
        CALL(evt_party_cont_onoff, 0, 0)
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("stg6_rsh_diary_03"))
    CALL(evt_msg_select, 0, STRING("stg6_rsh_diary_01_yn"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_continue)
        CALL(evt_party_cont_onoff, 0, 0)
        RETURN
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_adjust_dir)
    CALL(evt_mario_set_pose, STRING("M_L_2"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_diary_04"), 0, 0)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_npc_check, STRING("\202\344\202\244\202\352\202\242"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_npc_entry, STRING("\202\344\202\244\202\352\202\242"), STRING("c_ghost_k"))
        CALL(evt_npc_set_position, STRING("\202\344\202\244\202\352\202\242"), 0, -1000, 0)
        CALL(evt_npc_set_tribe, STRING("\202\344\202\244\202\352\202\242"), STRING("\202\344\202\244\202\352\202\242"))
        CALL(evt_npc_set_anim, STRING("\202\344\202\244\202\352\202\242"), STRING("O_1"))
        CALL(evt_npc_set_autotalkpose, STRING("\202\344\202\244\202\352\202\242"), STRING("O_2"), STRING("T_2"))
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("\202\344\202\244\202\352\202\242"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_SHADOW)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 64, 1000)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("\202\344\202\244\202\352\202\242"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 64, 180, 1000)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_npc_set_color, STRING("\202\344\202\244\202\352\202\242"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG5_GHOST1"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    ADD(LW(2), 20)
    CALL(evt_npc_set_position, STRING("\202\344\202\244\202\352\202\242"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, 132773, LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_I_O"))
    CALL(evt_msg_print, 0, STRING("stg6_rsh_diary_05"), 0, STRING("\202\344\202\244\202\352\202\242"))
    WAIT_MS(300)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 0)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, 132770, LW(3))
    BEGIN_INLINE_EVENT
        CALL(cam_r)
    END_INLINE_EVENT
    WAIT_MS(4000)
    CALL(evt_fade_set_mapchange_type, 1, -1, 600, -1, -1)
    CALL(evt_seq_set_seq, SEQ_GAME_OVER, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_lecture_msg)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("lecture_badge_00"), 0, 0)
    CALL(lecture_step_wait, 1)
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("lecture_badge_01"), 0, 0)
    CALL(lecture_step_wait, 2)
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("lecture_badge_02"), 0, 0)
#ifdef VERSION_G8ME01
    WAIT_MS(500)
    CALL(lecture_step_wait, 3)
    WAIT_MS(500)
    CALL(lecture_step_wait, 4)
    WAIT_MS(500)
#endif
    CALL(lecture_step_wait, 5)
#ifdef VERSION_G8ME01
    WAIT_MS(500)
#endif
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("lecture_badge_03"), 0, 0)
    CALL(lecture_step_wait, 6)
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("lecture_badge_04"), 0, 0)
    CALL(lecture_step_wait, 7)
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("lecture_badge_05"), 0, 0)
    CALL(lecture_step_wait, 8)
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("lecture_badge_06"), 0, 0)
    CALL(lecture_step_wait, 9)
    CALL(evt_msg_continue)
    CALL(evt_msg_print, 0, STRING("lecture_badge_07"), 0, 0)
    CALL(lecture_step_wait, 10)
    CALL(evt_msg_continue)
#ifdef VERSION_G8ME01
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG0_17_BADGE_TUTORIAL)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#endif
    WAIT_MS(500)
    CALL(lecture_key_mask_off)
    RETURN
#ifdef VERSION_G8ME01
    OPCODE_END_SCRIPT,
    0 // US: the object is 0x1E4 bytes (trailing zero word, the JP gap_05 data word)
};
#else
EVT_END()
#endif


USER_FUNC(itemUseFunc) {
    PartyEntry* party;
    MarioWork* mario;
    ItemData* item;
    EffectEntry* eff;

    item = &itemDataTable[event->lwData[10]];
    mario = marioGetPtr();
    party = partyGetPtr(0);
    if (isFirstCall) {
        cnt = 0;
    }
    switch (cnt) {
        case 0:
            statusWinForceOpen();
            if (event->lwData[11] == 0) {
                pouchSetHP(pouchGetHP() + item->hp_restored);
                pouchSetFP(pouchGetFP() + item->fp_restored);
                pouchSetAP(pouchGetAP() + item->sp_restored);
            } else {
                pouchSetPartyHP(event->lwData[11], pouchGetPartyHP(event->lwData[11]) + item->hp_restored);
                pouchSetFP(pouchGetFP() + item->fp_restored);
                pouchSetAP(pouchGetAP() + item->sp_restored);
            }
            cnt++;
            break;
        case 1:
            if (event->lwData[11]) {
                cnt = 200;
                break;
            }
            if (!item->hp_restored) {
                cnt = 100;
                break;
            }
            effRecoveryEntry(0, item->hp_restored, mario->position.x, mario->position.y + mario->wPlayerCollisionBox.y, mario->position.z);
            cnt++;
            break;
        case 31:
            cnt++;
            break;
        case 60:
            cnt = 100;
            break;
        case 100:
            if (!item->fp_restored) {
                cnt = 999;
                break;
            }
            effRecoveryEntry(1, item->fp_restored, mario->position.x, mario->position.y + mario->wPlayerCollisionBox.y, mario->position.z);
        case 130:
            cnt = 999;
            break;
        case 200:
            if (!item->hp_restored) {
                cnt = 300;
                break;
            }
            eff = effRecoveryEntry(0, item->hp_restored, party->position.x, party->position.y + party->height, party->position.z);
            ((RecoveryWork*)eff->userdata)->baseY = -party->height * HALF_F;
            cnt++;
            break;
        case 230:
            cnt++;
            break;
        case 260:
            cnt = 300;
            break;
        case 300:
            if (!item->fp_restored) {
                cnt = 999;
                break;
            }
            effRecoveryEntry(1, item->fp_restored, party->position.x, party->position.y + party->height, party->position.z);
            cnt++;
            break;
        case 330:
            cnt = 999;
            break;
        case 999:
        case 1000:
            return EVT_RETURN_DONE;
        default:
            cnt++;
            break;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(itemUseFunc2) {
    statusWinForceOff();
    return EVT_RETURN_DONE;
}

USER_FUNC(famicom_check) {
    EvtWord* args = event->args;

    if (wp->flags & WINDOW_FLAG_8BIT) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(party_color) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], pouchGetPartyColor(PARTNER_YOSHI));
    return EVT_RETURN_DONE;
}

USER_FUNC(all_party_slit_off) {
    allPartyForceSlitOff();
    return EVT_RETURN_DONE;
}

USER_FUNC(cam_r) {
    CameraEntry* cam;
    Vec v;
    f32 s;

    cam = camGetPtr(CAMERA_3D);
    cam->rollAngle += 3.0f;
    s = (f32)sin((f32)(u32)gp->retraceCount / 10.0f);
    PSVECSubtract(&cam->cameraAt, &cam->cameraPos, &v);
    PSVECScale(&v, &v, 0.2f * s);
    cam->viewOffset.x = v.x;
    cam->viewOffset.y = v.y;
    cam->viewOffset.z = v.z;
    return EVT_RETURN_BLOCK;
}

BOOL winGhostDiaryChk(void) {
    if (wp->menuState == WIN_MENU_DIARY_WAIT) {
        return TRUE;
    }
    return seqGetSeq() == SEQ_GAME_OVER;
}

const char* winMainStrippedTpl(void) {
    return "%s/w/%s/win.tpl";
}

void winFontInit(void) {
    FontDrawStart();
}

void winFontSet(Vec pos, Vec scale, GXColor color, const char* fmt, ...) {
#ifdef VERSION_G8ME01
    char buf[0x1000];
    const char* msg = fmt;
#else
    char buf[0x400];
#endif
    Mtx trans;
    Mtx scl;
    va_list args;

#ifdef VERSION_G8ME01
    if (strstr(fmt, "%")) {
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        msg = buf;
    }
#else
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
#endif
    PSMTXTrans(trans, pos.x, pos.y, pos.z);
    PSMTXScale(scl, scale.x, scale.y, scale.z);
    PSMTXConcat(trans, scl, trans);
    FontDrawColor(color);
#ifdef VERSION_G8ME01
    FontDrawMessageMtx(trans, msg);
}

void winFontSetWidth(Vec pos, Vec scale, GXColor color, f32 width, const char* fmt, ...) {
    char buf[0x1000];
    const char* msg = fmt;
    va_list args;
    f32 w;

    if (strstr(fmt, "%")) {
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        msg = buf;
    }
    w = scale.x * FontGetMessageWidth(msg);
    if (w > width) {
        scale.x *= width / w;
    }
    winFontSet(pos, scale, color, msg);
#else
    FontDrawMessageMtx(trans, buf);
#endif
}

void winFontSetR(Vec pos, Vec scale, GXColor color, const char* fmt, ...) {
#ifdef VERSION_G8ME01
    char buf[0x1000];
    const char* msg = fmt;
#else
    char buf[0x400];
#endif
    Mtx trans;
    Mtx scl;
    va_list args;
#ifdef VERSION_G8ME01
    s32 ref;

    if (strstr(fmt, "%")) {
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        msg = buf;
    }
    if ((u32)gp->language != LANGUAGE_JP) {
        ref = FontGetMessageWidth("999");
    } else {
        ref = FontGetMessageWidth("９９９");
    }
    { s32 w = ref - FontGetMessageWidth(msg); PSMTXTrans(trans, pos.x + scale.x * w, pos.y, pos.z); }
#else

    va_start(args, fmt);
    do { vsprintf(buf, fmt, args); } while (0);
    { s32 w = FontGetMessageWidth("９９９") - FontGetMessageWidth(buf); PSMTXTrans(trans, pos.x + scale.x * w, pos.y, pos.z); }
#endif
    PSMTXScale(scl, scale.x, scale.y, scale.z);
    PSMTXConcat(trans, scl, trans);
    FontDrawColor(color);
#ifdef VERSION_G8ME01
    FontDrawStringMtx(trans, msg);
#else
    FontDrawStringMtx(trans, buf);
#endif
}

void winFontSetEdge(Vec pos, Vec scale, GXColor color, const char* fmt, ...) {
#ifdef VERSION_G8ME01
    char buf[0x1000];
    const char* msg = fmt;
#else
    char buf[0x400];
#endif
    Mtx trans;
    Mtx scl;
    va_list args;

#ifdef VERSION_G8ME01
    if (strstr(fmt, "%")) {
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        msg = buf;
    }
#else
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
#endif
    PSMTXTrans(trans, pos.x, pos.y, pos.z);
    PSMTXScale(scl, scale.x, scale.y, scale.z);
    PSMTXConcat(trans, scl, trans);
    FontDrawColor(color);
    FontDrawEdge();
#ifdef VERSION_G8ME01
    FontDrawStringMtx(trans, msg);
    FontDrawEdgeOff();
}

void winFontSetEdgeWidth(Vec pos, Vec scale, GXColor color, f32 width, const char* fmt, ...) {
    char buf[0x1000];
    const char* msg = fmt;
    va_list args;
    f32 w;

    if (strstr(fmt, "%")) {
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        msg = buf;
    }
    w = scale.x * FontGetMessageWidth(msg);
    if (w > width) {
        scale.x *= width / w;
    }
    winFontSetEdge(pos, scale, color, msg);
#else
    FontDrawStringMtx(trans, buf);
    FontDrawEdgeOff();
#endif
}

void winFontSetLabel(Vec pos, Vec scale, GXColor color, const char* msg) {
    Mtx trans;
    Mtx scl;

    if (msg) {
        PSMTXTrans(trans, pos.x, pos.y, pos.z);
        PSMTXScale(scl, scale.x, scale.y, scale.z);
        PSMTXConcat(trans, scl, trans);
        FontDrawColor(color);
        FontDrawMessageMtx(trans, msgSearch(msg));
    }
}

#ifndef VERSION_G8ME01
void winFontSetPitch(f32 pitch, Vec pos, Vec scale, GXColor color, const char* fmt, ...) {
    char buf[0x400];
    va_list args;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    FontDrawColor(color);
    FontDrawScaleVec(scale);
    FontDrawStringVecPitch(&pos, buf, pitch);
}
#endif

void winTexInit(TEXPalette* texture) {
    winTexTpl = texture;
    GXSetCullMode(GX_CULL_NONE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetNumTexGens(1u);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
}

void winTexSet(u32 textureId, Vec translate, Vec scale, GXColor color) {
    Mtx trans;
    Mtx scl;
    Mtx m;
    GXTexObj obj;
    f32 sy;

    if (color.a != 0xFF) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    } else {
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    }
    TEXGetGXTexObjFromPalette(winTexTpl, &obj, textureId);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetTevColor(GX_TEVREG0, color);
    PSMTXTrans(trans, translate.x, translate.y, translate.z);
    sy = scale.y * GXGetTexObjHeight(&obj);
    PSMTXScale(scl, scale.x * GXGetTexObjWidth(&obj), sy, scale.z);
    PSMTXConcat(trans, scl, m);
    PSMTXConcat(camGetPtr(CAMERA_2D)->view, m, m);
    GXLoadPosMtxImm(m, 0);
    GXSetCurrentMtx(0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(HALF_F, -0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(HALF_F, HALF_F, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(-0.5f, HALF_F, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
}

void winTexSetNearest(u32 textureId, Vec translate, Vec scale, GXColor color) {
    Mtx trans;
    Mtx scl;
    Mtx m;
    GXTexObj obj;
    f32 sy;

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    TEXGetGXTexObjFromPalette(winTexTpl, &obj, textureId);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetTevColor(GX_TEVREG0, color);
    PSMTXTrans(trans, translate.x, translate.y, translate.z);
    sy = scale.y * GXGetTexObjHeight(&obj);
    PSMTXScale(scl, scale.x * GXGetTexObjWidth(&obj), sy, scale.z);
    PSMTXConcat(trans, scl, m);
    PSMTXConcat(camGetPtr(CAMERA_2D)->view, m, m);
    GXLoadPosMtxImm(m, 0);
    GXSetCurrentMtx(0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(HALF_F, -0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(HALF_F, HALF_F, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(-0.5f, HALF_F, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
}

void winTexSetRot(u32 textureId, Vec translate, Vec scale, GXColor color, f32 rot) {
    Mtx trans;
    Mtx scl;
    Mtx r;
    Mtx m;
    GXTexObj obj;
    f32 sy;

    if (color.a != 0xFF) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    } else {
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    }
    TEXGetGXTexObjFromPalette(winTexTpl, &obj, textureId);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetTevColor(GX_TEVREG0, color);
    PSMTXTrans(trans, translate.x, translate.y, translate.z);
    PSMTXRotRad(r, 'z', 0.017453292f * rot);
    sy = scale.y * GXGetTexObjHeight(&obj);
    PSMTXScale(scl, scale.x * GXGetTexObjWidth(&obj), sy, scale.z);
    PSMTXConcat(trans, r, m);
    PSMTXConcat(m, scl, m);
    PSMTXConcat(camGetPtr(CAMERA_2D)->view, m, m);
    GXLoadPosMtxImm(m, 0);
    GXSetCurrentMtx(0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(HALF_F, -0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(HALF_F, HALF_F, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(-0.5f, HALF_F, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
}

void winTexInit_x2(TEXPalette* texture) {
    winTexInit(texture);
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
}

void winTexSet_x2(u32 textureId1, u32 textureId2, Vec translate, Vec scale, GXColor color) {
    Mtx trans;
    Mtx scl;
    Mtx m;
    GXTexObj obj;
    f32 sy;

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    TEXGetGXTexObjFromPalette(winTexTpl, &obj, textureId1);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    TEXGetGXTexObjFromPalette(winTexTpl, &obj, textureId2);
    GXInitTexObjLOD(&obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP1);
    GXSetTevColor(GX_TEVREG0, color);
    PSMTXTrans(trans, translate.x, translate.y, translate.z);
    sy = scale.y * GXGetTexObjHeight(&obj);
    PSMTXScale(scl, scale.x * GXGetTexObjWidth(&obj), sy, scale.z);
    PSMTXConcat(trans, scl, m);
    PSMTXConcat(camGetPtr(CAMERA_2D)->view, m, m);
    GXLoadPosMtxImm(m, 0);
    GXSetCurrentMtx(0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(HALF_F, -0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(HALF_F, HALF_F, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(-0.5f, HALF_F, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
}

void winIconInit(void) {
    winTexInit(NULL);}

void winIconGrayInit(void) {
    winTexInit(NULL);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevColor(GX_TEVREG1, (GXColor){ 0x55, 0x55, 0x55, 0xFF });
    GXSetNumTevStages(4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP1);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C1, GX_CC_TEXC, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP2);
    if (!GX_TEVSTAGE2 && !GX_TEVSTAGE2 && !GX_TEVSTAGE2) {
    }
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_C1, GX_CC_TEXC, GX_CC_CPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP3);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
}

void winIconSet(s32 iconId, Vec translate, Vec scale, GXColor color) {
    Mtx trans;
    Mtx scl;
    Mtx m;
    GXTexObj obj;
    f32 sy;

#ifdef VERSION_G8ME01
    if (iconId == 0x11C) {
        iconId = 0x11B;
    }
#else
    if (iconId == 0x115) {
        iconId = 0x114;
    }
#endif
    iconGetTexObj(&obj, iconId);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    if (color.a != 0xFF) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    } else {
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    }
    GXSetTevColor(GX_TEVREG0, color);
    PSMTXTrans(trans, translate.x, translate.y, translate.z);
    sy = scale.y * GXGetTexObjHeight(&obj);
    PSMTXScale(scl, scale.x * GXGetTexObjWidth(&obj), sy, scale.z);
    PSMTXConcat(trans, scl, m);
    PSMTXConcat(camGetPtr(CAMERA_2D)->view, m, m);
    GXLoadPosMtxImm(m, 0);
    GXSetCurrentMtx(0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(HALF_F, -0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(HALF_F, HALF_F, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(-0.5f, HALF_F, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
}

USER_FUNC(lecture_step_wait) {
    s32 n = evtGetValue(event, event->args[0]);

    if (isFirstCall) {
        wp->flags &= ~WINDOW_FLAG_LECTURE_KEY_MASK;
    }
    if (wp->lectureStep < n) {
        return EVT_RETURN_BLOCK;
    }
    wp->flags |= WINDOW_FLAG_LECTURE_KEY_MASK;
    return EVT_RETURN_DONE;
}

USER_FUNC(lecture_key_mask_off) {
    wp->flags &= ~WINDOW_FLAG_LECTURE_KEY_MASK;
    return EVT_RETURN_DONE;
}

void winLectureKeyMask(void) {
    if (wp->flags & WINDOW_FLAG_LECTURE) {
        if (wp->flags & WINDOW_FLAG_LECTURE_KEY_MASK) {
            wp->buttonTrg = 0;
            wp->buttonRep = 0;
            wp->dirTrg = 0;
            wp->dirRep = 0;
            return;
        }
        switch (wp->lectureStep) {
            case 0:
            case 1:
                wp->buttonTrg = 0;
                wp->buttonRep = 0;
                wp->dirTrg = 0;
                wp->dirRep = 0;
                if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                    wp->lectureStep++;
                }
                break;
            case 2:
            case 3:
            case 4:
                wp->buttonTrg = 0;
                wp->buttonRep = 0;
                wp->dirTrg &= 0x8000;
                wp->dirRep &= 0x8000;
                if (wp->dirTrg || wp->dirRep) {
                    wp->lectureStep++;
                }
                break;
            case 5:
            case 6:
            case 7:
            case 8:
                wp->buttonTrg &= PAD_BUTTON_A;
                wp->buttonRep &= PAD_BUTTON_A;
                wp->dirTrg = 0;
                wp->dirRep = 0;
                if (wp->buttonTrg || wp->buttonRep) {
                    wp->lectureStep++;
                }
                break;
            case 9:
                wp->buttonTrg &= PAD_BUTTON_START;
                wp->buttonRep &= PAD_BUTTON_START;
                wp->dirTrg = 0;
                wp->dirRep = 0;
                if (wp->buttonTrg || wp->buttonRep) {
                    wp->lectureStep++;
                }
                break;
        }
    }
}

void winLectureOn(void) {
    wp->flags |= WINDOW_FLAG_LECTURE;
    wp->flags |= WINDOW_FLAG_LECTURE_KEY_MASK;
    wp->lectureStep = 0;
    wp->lectureEvent = evtEntry(evt_lecture_msg, 0, 0);
}

BOOL winLectureCheck(void) {

    if ((wp->flags & WINDOW_FLAG_LECTURE) && evtCheckID(wp->lectureEvent->eventId)) {
        return TRUE;
    }
    wp->flags &= ~WINDOW_FLAG_LECTURE;
    return FALSE;
}


const volatile f32 float_0p5_8041dc98 __attribute__((aligned(8))) = 0.5f;

void winMainStrippedPool(f32* f, s32 i) {
    *f = (f32)i;
}
