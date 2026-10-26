#pragma once

/* Saved flags (GSWF) and values (GSW) named from what the scripts do with them and the room's message
 * text; the game carries no names for these. Only values the code uses are listed. */

// set by buy_evt when a 300-point shop prize could not be given (item bag full); shopper_talk_evt hands it out
// (shop_point_item 300) and clears it
#define GSWF_SHOP_POINT_PRIZE_PENDING GSWF(1)
// set by johoyaketi_talk (tik_00) after Wonky's introduction; the same talk tests it
#define GSWF_TIK_00_WONKY_FIRST_TALK_DONE GSWF(3)
// set by johoyaperfect_talk (gor_02) after Grifty's introduction (johoyaperfect_00); later talks use the short
// greeting
#define GSWF_GOR_GRIFTY_FIRST_TALK_DONE GSWF(4)
// set by uranaisi_table_talk (tik_00) after Merluvlee's introduction; later visits use the short greeting
#define GSWF_TIK_00_MERLUVLEE_FIRST_TALK_DONE GSWF(5)
// set by uranaisi_table_talk (tik_00) once Mario accepts a reading; later talks start at the reading menu; tik_00
// init clears it
#define GSWF_TIK_00_MERLUVLEE_ASKED_THIS_VISIT GSWF(6)
// set by mail_evt_nok_01 (and stg1_03_init in event.c) when mails 16 (Frankly: E-mail) and 26 (RDM Registration)
// arrive; mail_evt_nok_01 stops once set
#define GSWF_MAIL_FRANKLY_SENT GSWF(7)
// set by mail_evt_gor_03 (and stg2_end_init in event.c) when mail 17 (Koopley) arrives at STG2_47
#define GSWF_MAIL_KOOPLEY_SENT GSWF(8)
// set by mail_evt_gor_02 (and stg3_52_init in event.c) when mail 18 (Koopie Koo) arrives at STG3_51
#define GSWF_MAIL_KOOPIE_KOO_SENT GSWF(9)
// set by mail_evt_gor_01 when mail 19 (Petuni) arrives at STG4_37; the event stops once set
#define GSWF_MAIL_PETUNI_SENT GSWF(10)
#define GSWF_MAIL_BOO_SENT GSWF(11)              /* set by mail_evt_gor_04 when mail 20 (Boo) arrives at STG5_66 */
// set by mail_evt_rsh_03_a when mail 21 (Pa-Patch) arrives from STG6_45 on; the event stops once set
#define GSWF_MAIL_PA_PATCH_SENT GSWF(12)
// set by mail_evt_hom_00 when mail 22 (King K) arrives at STG6_42; the event stops once set
#define GSWF_MAIL_KING_K_SENT GSWF(13)
// set by mail_evt_gor_02_2 when mail 23 (The Real Zip Toad) arrives at STG6_71; the event stops once set
#define GSWF_MAIL_ZIP_TOAD_SENT GSWF(14)
// set by mail_evt_bom_01 when mail 24 (Rawk Hawk) arrives at STG7_02; the event stops once set
#define GSWF_MAIL_RAWK_HAWK_SENT GSWF(15)
// set by mail_evt_tik_00 when mails 25 (Ghost T.) and 33 (RDM Final Issue) arrive at STG7_50; the event stops once
// set
#define GSWF_MAIL_GHOST_T_SENT GSWF(16)
// set by mail_evt_win_01 when mail 27 (RDM Premiere Issue) arrives at STG2_09; the event stops once set
#define GSWF_MAIL_RDM_ISSUE_1_SENT GSWF(17)
// set by mail_evt_tou_01 when mail 28 (RDM Issue 2) arrives at STG3_12; the event stops once set
#define GSWF_MAIL_RDM_ISSUE_2_SENT GSWF(18)
// set by mail_evt_usu_01 when mail 29 (RDM Issue 3) arrives at STG4_04_01; the event stops once set
#define GSWF_MAIL_RDM_ISSUE_3_SENT GSWF(19)
// set by mail_evt_gor_01_2 when mail 30 (RDM Issue 4) arrives at STG5_59_01
#define GSWF_MAIL_RDM_ISSUE_4_SENT GSWF(20)
// set by mail_evt_rsh_03_a_2 when mail 31 (RDM Issue 5) arrives at STG6_21; the event stops once set
#define GSWF_MAIL_RDM_ISSUE_5_SENT GSWF(21)
// set by mail_evt_pik_00 when mail 32 (RDM Issue 6) arrives at STG7_02; the event stops once set
#define GSWF_MAIL_RDM_ISSUE_6_SENT GSWF(22)
// set by mail_evt the first time it prints mail_first_hit (the e-mail explanation)
#define GSWF_MAIL_TUTORIAL_SEEN GSWF(37)
// set by the mail window (win_item) when a mail is opened; mail_evt prints mail_first_hit again on later mails while
// clear
#define GSWF_MAIL_TUTORIAL_REPEAT_OFF GSWF(38)
// set by muj_00 init when Cortez's trip from Rogueport (GSWF(3125)) ends, cleared by gor_00 init when the trip back
// ends; no code reads it
#define GSWF_MUJ_ARRIVED_BY_SHIP_UNUSED GSWF(39)
// set by cooking_evt (gor_01) when Zess T. hands over a cooked item; the journal (win_log) adds its recipe page once
// set
#define GSWF_GOR_ZESS_T_DISH_RECEIVED GSWF(64)
// set by mobj_save_blk_sysevt (evt_mobj.c) on a save from STG0_07 on; christine_nakama_after (gor_01) skips
// Goombella's save reminder when set
#define GSWF_MAIN_SAVED_SINCE_GOOMBELLA_JOINED GSWF(236)
// set by uranaisi_table_talk (tik_00) when Merluvlee's greeting is shown; later talks use a shorter line; tik_00 init
// clears it
#define GSWF_TIK_00_MERLUVLEE_GREETED_THIS_VISIT GSWF(237)
// set by badgeShop_bargainGeneration once the story reaches STG1_16_HOOKTAIL, after adding
// badge_special_table_stage_1_clear to the stock
#define GSWF_BADGESHOP_STAGE_1_STOCK_ADDED GSWF(238)
// set by badgeShop_bargainGeneration from STG2_25_AFTER_THE_BOSS_BATTLE, after adding the stage 2 table
#define GSWF_BADGESHOP_STAGE_2_STOCK_ADDED GSWF(239)
// set by badgeShop_bargainGeneration from STG3_32_BOSS_BATTLE, after adding the stage 3 table
#define GSWF_BADGESHOP_STAGE_3_STOCK_ADDED GSWF(240)
// set by badgeShop_bargainGeneration from STG4_15_02_BACK_TO_NORMAL, after adding the stage 4 table
#define GSWF_BADGESHOP_STAGE_4_STOCK_ADDED GSWF(241)
// set by badgeShop_bargainGeneration from STG5_33_CORTEZ_AND_THE_TREASURE, after adding the stage 5 table
#define GSWF_BADGESHOP_STAGE_5_STOCK_ADDED GSWF(242)
// set by badgeShop_bargainGeneration from STG6_53_THE_REAL_GARNET_STAR, after adding the stage 6 table
#define GSWF_BADGESHOP_STAGE_6_STOCK_ADDED GSWF(243)
// set by badgeShop_bargainGeneration from STG7_20_LORD_CRUMP_DEFEATED; no badges are added and only its own check
// reads it
#define GSWF_BADGESHOP_STAGE_7_STOCK_ADDED GSWF(244)
// set by badgeShop_bargainGeneration from STG8_16_THE_RETURN_OF_THE_QUEEN; no badges are added and only its own check
// reads it
#define GSWF_BADGESHOP_STAGE_8_STOCK_ADDED GSWF(245)
// set by badgeShop_bteresaGeneration once Mario has the Special Card, after adding its prizes to the Pianta Parlor
// (gor_03) stock
#define GSWF_PARLOR_SPECIAL_CARD_PRIZES_ADDED GSWF(246)
// set by badgeShop_bteresaGeneration once Mario has the Silver Card, after adding its prizes to the Pianta Parlor
// stock
#define GSWF_PARLOR_SILVER_CARD_PRIZES_ADDED GSWF(247)
// set by badgeShop_bteresaGeneration once Mario has the Gold Card, after adding its prizes to the Pianta Parlor stock
#define GSWF_PARLOR_GOLD_CARD_PRIZES_ADDED GSWF(248)
// set by badgeShop_bteresaGeneration once Mario has the Platinum Card, after adding its prizes to the Pianta Parlor
// stock
#define GSWF_PARLOR_PLATINUM_CARD_PRIZES_ADDED GSWF(249)
// set by mail_evt_tou_01 (RDM Issue 2); shopper_talk_evt (mri_07 greets with stg2_shop_04_1) and add_shop_point
// (swGet(0xFA)) double points within 15 minutes of GSW 5
#define GSWF_SHOP_MRI_07_DOUBLE_POINTS_SALE GSWF(250)
// set by mail_evt_gor_01_2 (RDM Issue 4); shopper_talk_evt (usu_01 greets with usugurashop_04_1) and add_shop_point
// (swGet(0xFB)) double points within 15 minutes of GSW 14
#define GSWF_SHOP_USU_01_DOUBLE_POINTS_SALE GSWF(251)
// set by mail_evt_pik_00 (RDM Issue 6); shopper_talk_evt (rsh_03 greets with stg6_shop_04_1) and add_shop_point
// (swGet(0xFC)) double points within 15 minutes of GSW 23
#define GSWF_SHOP_RSH_03_DOUBLE_POINTS_SALE GSWF(252)
// set by win_04 init on every entry; win_log's map-dot check reads it for dot type 1, but no tubuDt dot uses type 1
#define GSWF_WIN_04_ENTERED GSWF(253)
// set by tik_01 init on every entry; the journal map (win_log tubuCheck type 2) reads it
#define GSWF_TIK_01_VISITED GSWF(254)
// set by gon_00 init on every entry; the journal map (win_log tubuCheck type 3) reads it
#define GSWF_GON_00_VISITED GSWF(255)
// set by gra_00 init on every entry; win_log shows the type-4 map dots (Twilight Town to gra path) once it is set
#define GSWF_GRA_00_ENTERED GSWF(256)
// set by kuzi_bomb_talk_husei (gor_03) after the lottery-clock-error explanation, cleared when Mario buys a new
// 500-coin number; when set the short lines are used
#define GSWF_GOR_LOTTERY_RESET_EXPLAINED GSWF(257)
// set by luigi_gor_first_talk (gor.c) after Luigi's first greeting (gor_luigi_00/01)
#define GSWF_GOR_LUIGI_FIRST_TALK_DONE GSWF(1175)
// mony_peton_talk (gor_00): 1 if Mario agrees to pretend he never saw the runaway couple (mac_2_083), 0 if he
// refuses; mony_talk/peton_talk pick their lines
#define GSWF_GOR_00_RUNAWAYS_SECRET_KEPT GSWF(1176)
// set by marco_talk (gor_00) at STG4_37 after Flavio's departure speech (mac_4_089/090); later talks go straight to
// the set-sail question
#define GSWF_GOR_FLAVIO_SET_SAIL_TALK_DONE GSWF(1177)
// set by korutesu_talk (muj_00) when Mario accepts Cortez's trip to Rogueport; muj_20 init plays the voyage, gor_00
// init clears it and runs into_gor_ship
#define GSWF_MUJ_SAILING_TO_ROGUEPORT GSWF(1178)
// set by christine_osoware (gor_00) when Mario walks to the back of the plaza; garawaru_regl and suifubomb_b_regl
// stop their patrol loops
#define GSWF_GOR_00_GOOMBELLA_ASSAULT_STARTED GSWF(1179)
// tested and cleared by gor_00 init (from STG5_58): when set Mario is placed beside the ship; set only by the debug
// story jump in event.c
#define GSWF_GOR_00_PLACE_BY_SHIP GSWF(1180)
// set by mony_peton_after (gor_00) after the partner's remark (mac_2_088) at STG2_45; gor_00 init arms that event
// only while clear
#define GSWF_GOR_00_RUNAWAYS_REMARK_DONE GSWF(1181)
// set by mokorim_talk (gor_00) on Lumpy's first pitch (salari_before_000..002); later talks start at
// salari_before_003
#define GSWF_GOR_LUMPY_FIRST_TALK_DONE GSWF(1182)
// set by mokorim_talk (gor_00) from STG6_56 when Lumpy reports striking oil and repays GSW(116)'s investments
#define GSWF_GOR_LUMPY_REPAYMENT_DONE GSWF(1183)
// set by powerupya_table_talk (gor_02) after Merlon's full introduction (powerup_00)
#define GSWF_GOR_MERLON_FIRST_TALK_DONE GSWF(1185)
// set by powerupya_orb_use (gor_02) when Merlon uses the Up Arrow; winmgr raises the partner rank limit from 1 to 2,
// mapball_init/dearu_event/powerupya_party_lank_rtn test it
#define GSWF_GOR_UP_ARROW_USED GSWF(1186)
// set by nancy_init_check (gor_01) when Zess T. blocks the west gate until she gets a contact lens (gor_01_007);
// nancy_init/regl and shoptender_talk test it
#define GSWF_GOR_ZESS_T_BLOCKING_WEST_GATE GSWF(1187)
// set by nancy_talk (gor_01) when Mario gives Zess T. the Contact Lens and the west gate opens; shop stock, doors and
// her later talks (cooking) test it
#define GSWF_GOR_ZESS_T_GOT_CONTACT_LENS GSWF(1188)
// set by shoptender_talk (gor_01) when the lens is ordered before STG1_17; gor_01 init then sets GSWF(1191) once
// STG1_17 is reached
#define GSWF_GOR_CONTACT_LENS_ORDERED_EARLY GSWF(1189)
// set by shoptender_talk (gor_01) when the lens is ordered from STG1_17 on; gor_01 init (after GSW(108) shows 90 s)
// and tik_01 init set GSWF(1191)
#define GSWF_GOR_CONTACT_LENS_ORDERED_LATE GSWF(1190)
// set by gor_01 init or tik_01 init once the ordered lens is in; the shop stocks goods_list2 and the clerk announces
// it (gor_01_015)
#define GSWF_GOR_CONTACT_LENS_ARRIVED GSWF(1191)
// set by marco_talk (gor_01) before STG4_31 when Flavio introduces himself (mac_4_005); at STG4_31 it picks mac_4_007
// or the short mac_4_008
#define GSWF_GOR_FLAVIO_FIRST_TALK_DONE GSWF(1192)
// set by master_talk (gor_01) after Podley's first lines about Bobbery refusing to sail (mac_4_061..063)
#define GSWF_GOR_PODLEY_BOBBERY_TALK_DONE GSWF(1193)
// set by master_talk (gor_01) after Podley tells Bobbery's story (mac_4_066..069); later talks skip to mac_4_071
#define GSWF_GOR_PODLEY_BOBBERY_STORY_TOLD GSWF(1194)
// the gor_01 big chest (evt_mobj_itembox mobj_box00) holding the Ultra Hammer
#define GSWF_GOR_01_ULTRA_HAMMER_CHEST GSWF(1195)
// set by ultrahammer_check (gor_01) when the Ultra Hammer chest falls from the roof; gor_01 init then places the
// chest on the ground
#define GSWF_GOR_01_ULTRA_HAMMER_CHEST_FELL GSWF(1196)
// set by cooking_evt (gor_01) after Zess T. introduces her cooking (gor_01_cook_001)
#define GSWF_GOR_ZESS_T_COOKING_INTRO_DONE GSWF(1197)
// set by thief_talk (gor_01) after the thief returns the stolen coins (GSW(2)/GSW(3)); evt_door_04_close tests it
#define GSWF_GOR_THIEF_FOUND_IN_HIDEOUT GSWF(1198)
// set by evt_door_04_close (gor_01) when Mario leaves the thief's room after GSWF(1198); thief_talk then uses
// mac_0_065_04 instead of _03
#define GSWF_GOR_THIEF_HIDEOUT_LEFT GSWF(1199)
// set by christine_nakama_after (gor_01) after Goombella's save-block reminder (mac_0_061_01); gor_01 init at STG0_07
// spawns it while clear
#define GSWF_GOR_01_SAVE_REMINDER_DONE GSWF(1200)
// the Attack FX B reward from Ms. Mowz's trouble (iri_16_tyutyu_talk, gor_01); given directly or left as item iri_16
// when Mario holds 200 badges
#define GSWF_GOR_01_MS_MOWZ_ATTACK_FX_B GSWF(1201)
// the Honey Shroom reward from Zess T.'s trouble (irai_18, cooking_evt gor_01); given directly or left as item iri_18
// when Mario's items are full
#define GSWF_GOR_01_ZESS_T_HONEY_SHROOM GSWF(1202)
// set by dearu_event (gor_02, from STG5_58) while the Up Arrow is unused, cleared otherwise and after use;
// powerupya_talk/init run Merlon's star-chart hint
#define GSWF_GOR_MERLON_HINT_PENDING GSWF(1203)
// set by powerupya_talk (gor_02) after Merlon's star-chart hint (powerup_ev_01/02); later talks offer the Up Arrow or
// repeat the hint
#define GSWF_GOR_MERLON_STAR_CHART_HINT_DONE GSWF(1204)
// set by bottakuru_talk (gor_01) after Charlieton's first greeting (bottakuru_00)
#define GSWF_GOR_CHARLIETON_FIRST_TALK_DONE GSWF(1205)
// set by bottakuru_talk (gor_01) after a purchase, cleared by bottakuru_init; picks the greeting bottakuru_02 over
// _01
#define GSWF_GOR_CHARLIETON_BOUGHT_THIS_VISIT GSWF(1206)
// set by gor_02_event_01 (exit through the pipe to tik_01); kurihakase_init hides Frankly at STG0_10_01 once set
#define GSWF_GOR_02_TUTORIAL_PIPE_TAKEN GSWF(1207)
// set by phase_event (unit_monban.c) after the gatekeeper's hint to run when Mario has 5 HP or less (gor_02_008_01)
#define GSWF_GOR_GATEKEEPER_RUN_HINT_SHOWN GSWF(1208)
// set by oyabun_talk (gor_02) on Ishnail's first greeting up to STG2_40 (gor_02_000)
#define GSWF_GOR_ISHNAIL_FIRST_TALK_DONE GSWF(1209)
// set by oyabun_talk (gor_02) at STG2_41 after Ishnail's first speech about the Robbo Thieves (mac_2_012..015)
#define GSWF_GOR_ISHNAIL_DON_PIANTA_TALK_DONE GSWF(1210)
// set by oyabun_talk (gor_02) after Mario pays 64 coins and Ishnail explains how to reach Don Pianta (mac_2_038);
// later talks repeat mac_2_040
#define GSWF_GOR_ISHNAIL_PASSWORD_TOLD GSWF(1211)
// set by tozokudan1_talk (gor_02) after the gatekeeper's 10-coin toll speech (gor_02_004)
#define GSWF_GOR_GATEKEEPER_FIRST_TALK_DONE GSWF(1212)
// set by tozokudan1_talk (gor_02) when Mario wins the gatekeeper battle; the gate wall stays open (tozokudan1_init,
// gor_02_hitobj_on)
#define GSWF_GOR_GATEKEEPER_DEFEATED GSWF(1213)
// set by master_talk (gor_01) or kurihakase_talk (gor_02) at STG4_33 when told where Bobbery lives; sanders_talk
// picks its line
#define GSWF_GOR_BOBBERY_HOME_LEARNED GSWF(1214)
// set by sanders_talk (gor_02) when Bobbery denies who he is before GSWF(1214); marco_talk (gor_00) then uses
// mac_4_045_01
#define GSWF_GOR_BOBBERY_FIRST_DENIAL_HEARD GSWF(1215)
// set by sanders_open_door (gor_02) when Bobbery unlocks his door; door events and sanders_init test it
#define GSWF_GOR_BOBBERY_DOOR_UNLOCKED GSWF(1216)
// set by evt_door_03_close (gor_02) at STG4_34 after the partner's remark on leaving Bobbery's house (mac_4_058/059)
#define GSWF_GOR_BOBBERY_DECLINE_REMARK_DONE GSWF(1217)
// set by kurihakase_talk (gor_02) after Frankly tells Mario to look for Darkly (mac_3_009/010); later talks use
// gor_02_020
#define GSWF_GOR_FRANKLY_DARKLY_HINT_DONE GSWF(1218)
// the gor_02 lock mobj_lock00 (evt_mobj_lock, House Key); evt_unlock_npc sets it when the gang member comes out
#define GSWF_GOR_02_HOUSE_KEY_LOCK_UNLOCKED GSWF(1219)
// set by epigraphy_map_after_stage6 (gor_02) when Frankly asks Mario to come back later; cleared by
// evt_door_02_close; kurihakase_talk then prints mac_6_006
#define GSWF_GOR_FRANKLY_STG6_SAID_WAIT GSWF(1220)
// set by evt_cloud (gor_02) when the wall cover S_i04_kabe_msk is blown away (evt_cloud_ent); gor_02 init skips the
// cover once set
#define GSWF_GOR_02_HIDDEN_WALL_BLOWN_AWAY GSWF(1221)
// set by tozokudan3_talk (gor_02) after the gang member's first talk at the Trouble Center's secret way in
// (irai_oku_00)
#define GSWF_GOR_TROUBLE_CENTER_BACKWAY_TALK GSWF(1222)
// set by gor_03_down_evt (exit s_bero to tik_07); tik_07_down_evt tests and clears it to play the drop-in
#define GSWF_GOR_03_DROP_TO_TIK_07_PENDING GSWF(1223)
// set by wall_bomb (gor_03) when Bobbery blows open the cracked wall; wall_bomb_init shows the hole
#define GSWF_GOR_03_WALL_BOMBED GSWF(1224)
// set by exchange_set (gor_03) after the Pianta Changer's first introduction (exchange_mess_00); later uses print
// exchange_mess_01
#define GSWF_GOR_PIANTA_CHANGER_FIRST_USE_DONE GSWF(1225)
// set by bteresa_table_talk (gor_03) after the attendant's first welcome (bunny_teresa_00)
#define GSWF_GOR_PARLOR_ATTENDANT_FIRST_TALK GSWF(1226)
// set by bteresa_table_talk (gor_03) after a greeting, cleared by door_data_02_0_open; when set the short
// bunny_teresa_02 is used
#define GSWF_GOR_PARLOR_GREETED_THIS_VISIT GSWF(1227)
// set by bteresa_talk_sub_keihin (gor_03) after the prize-trade introduction (bunny_teresa_60)
#define GSWF_GOR_PARLOR_PRIZE_INTRO_DONE GSWF(1228)
// set by minigame01..04_end (yuu_00..03) before returning to gor_03; gor_03 init/dokan init/montemedal test it, init
// clears it and runs yuureturn_rtn
#define GSWF_GOR_RETURNING_FROM_MINIGAME GSWF(1229)
// set by uraguchi_event (gor_03) when the clerk accepts the password and opens the back door (mac_2_043_01); door and
// NPC setup test it
#define GSWF_GOR_03_BACK_DOOR_PASSWORD_DONE GSWF(1230)
// set by bteresa_table_talk (gor_03) when Mario shows the Special Card; minigameselect_msg_set lists the Plane Game
#define GSWF_GOR_PLANE_GAME_UNLOCKED GSWF(1232)
// set by bteresa_table_talk (gor_03) when Mario shows the Platinum Card; minigameselect_msg_set lists the Boat Game
#define GSWF_GOR_BOAT_GAME_UNLOCKED GSWF(1233)
// set by bteresa_table_talk (gor_03) when Mario shows the Gold Card; minigameselect_msg_set lists the Tube Game
#define GSWF_GOR_TUBE_GAME_UNLOCKED GSWF(1234)
// set by bteresa_table_talk (gor_03) when Mario shows the Silver Card; minigameselect_msg_set lists the Paper Game
#define GSWF_GOR_PAPER_GAME_UNLOCKED GSWF(1235)
// set by maffiaboss_airship_ticket (gor_03) at STG2_45; maffiaboss_init, maffia1/2_talk and gor_03 init test it
#define GSWF_GOR_DON_PIANTA_TICKET_SCENE_DONE GSWF(1236)
// set by kuzi_bomb_talk_normal (gor_03) on the lottery host's first pitch (kuji_bomb_000)
#define GSWF_GOR_LOTTERY_FIRST_TALK_DONE GSWF(1237)
// set by kuzi_get_keihin (gor_03) on the first Number 1 win: Power Plus the first time, Zess Dynamite after
#define GSWF_GOR_LOTTERY_NUMBER_1_PRIZE_GIVEN GSWF(1238)
// set by kuzi_get_keihin (gor_03) on the first Number 2 win: Lucky Day the first time, Zess Special after
#define GSWF_GOR_LOTTERY_NUMBER_2_PRIZE_GIVEN GSWF(1239)
// set by evt_yuuranking main_evt when a Plane Game score (yuu_00, type 0) takes first place; bteresa_talk counts it
#define GSWF_GOR_PLANE_GAME_TOP_SCORE GSWF(1240)
// set by evt_yuuranking main_evt when a Boat Game score (yuu_01, type 3) takes first place; bteresa_talk counts it
#define GSWF_GOR_BOAT_GAME_TOP_SCORE GSWF(1241)
// set by evt_yuuranking main_evt when a Tube Game score (yuu_02, type 2) takes first place; bteresa_talk counts it
#define GSWF_GOR_TUBE_GAME_TOP_SCORE GSWF(1242)
// set by evt_yuuranking main_evt when a Paper Game time (yuu_03, type 1) takes first place; bteresa_talk counts it
#define GSWF_GOR_PAPER_GAME_TOP_SCORE GSWF(1243)
// set by evt_to_gor and evt_skip (tou_00) when the blimp flies back to gor_04; no code reads it
#define GSWF_TOU_BLIMP_TO_ROGUEPORT_UNUSED GSWF(1244)
// set by epigraphy_map (gor_02) with STG0_16; the tik_04 battle groups (battle_database_tik) switch to their
// altSelect when set
#define GSWF_GOR_FRANKLY_FIRST_MAP_READ GSWF(1246)
#define GSWF_GOR_00_HP_DRAIN_CHEST GSWF(1247)    /* the gor_00 chest (evt_mobj_itembox mobj_box00) holding HP Drain */
// the gor_02 chest (evt_mobj_itembox mobj_box01) holding Double Dip
#define GSWF_GOR_02_DOUBLE_DIP_CHEST GSWF(1248)
// set by kurihakase_talk (gor_02) in the epilogue after gor_02_029..029_02; later talks use gor_02_029_03
#define GSWF_GOR_FRANKLY_EPILOGUE_TALK_DONE GSWF(1249)
// set by powerupya_table_talk (gor_02), cleared by evt_door_01_open; picks the short greeting powerup_00_02
#define GSWF_GOR_MERLON_GREETED_THIS_VISIT GSWF(1250)
// set by starmaniac_talk (tik_00) after Dazzle's introduction; the same talk tests it
#define GSWF_TIK_00_DAZZLE_FIRST_TALK_DONE GSWF(1325)
// set by tensou_event (tik_00) before the teleporter warp to aji_07; aji_07 tensou_init tests and clears it to play
// the arrival
#define GSWF_TIK_TELEPORT_TO_AJI_PENDING GSWF(1326)
// set by evt_door_stg7_close (tik_00) when the door is used from its far side; evt_door_stg7_open says it does not
// open while it is 0
#define GSWF_TIK_00_STG7_DOOR_OPENED GSWF(1327)
// set by tensou_init_event (tik_00) after the partner's first remark on arriving by teleporter (before STG7_26); the
// same event tests it
#define GSWF_TIK_00_TELEPORT_ARRIVAL_TALK_DONE GSWF(1328)
// set by evt_door_04_close (tik_00) after the partner's remark that they are under Rogueport (before STG7_26); the
// same event tests it
#define GSWF_TIK_00_DOOR_04_TALK_DONE GSWF(1329)
// set by hashira_bomb_sub (tik_00) when bom_mae is blown open; hashira_bomb tests it
#define GSWF_TIK_00_PILLAR_BOMBED GSWF(1330)
// set by wall_bomb00 (tik_01) when the wall bom_kabe_1 is blown open; tik_01 init tests it
#define GSWF_TIK_01_WALL_BOMBED GSWF(1331)
// set by wall_bomb01 (tik_01) and wall_bomb (tik_02) when the wall at tik_01's e_bero_2 is blown open; both inits
// test it
#define GSWF_TIK_01_EAST_WALL_BOMBED GSWF(1332)
// state of the tik_03 secret passage: set by puni_switch, toggled by tik03_switch_evt (1 = open, 0 = closed); tik_03
// init tests it
#define GSWF_TIK_03_SECRET_PASSAGE_OPEN GSWF(1334)
// the tik_04 blue switch mobj_switch00 (tik_04_switch_evt) that flips the paper stairs; tik_04 init tests it
#define GSWF_TIK_04_STAIRS_SWITCH_PRESSED GSWF(1335)
// set by tik04_wall_blow_evt (tik_04) when the gust case (evt_cloud_ent) removes win_1; tik_04 init tests it
#define GSWF_TIK_04_WALL_BLOWN_AWAY GSWF(1336)
#define GSWF_TIK_04_BLACK_KEY GSWF(1337)         /* the Black Key (ITEM_BLACK_KEY_0x21) lying in tik_04 */
// the tik_04 block block (evt_mobj_lv_blk, MOBJ_Lv1BigBigBlock); tik_04 init also tests it
#define GSWF_TIK_04_LV1_BIGBIG_BLOCK_BROKEN GSWF(1338)
// set by plane_tobashi_check (tik_04) when the plane lands on a_dai_1 or the block; tik_04 init stops the check once
// set
#define GSWF_TIK_04_PLANE_FLIGHT_LANDED GSWF(1339)
// the tik_07 block mobj_blk01 (evt_mobj_lv_blk, MOBJ_Lv2BigBlock); init hides s_hikari while it is 0
#define GSWF_TIK_07_LV2_BIG_BLOCK_BROKEN GSWF(1340)
// set by oshimodo_event (tik_08) when the pipe rejects Mario (tik_08_01); Frankly (kurihakase_talk,
// epigraphy_map_after_stage3) and Darkly's talk test it
#define GSWF_TIK_08_PIPE_REJECTED GSWF(1341)
// set by powerdownya_table_talk (tik_11) after Chet Rippo's introduction; the same talk tests it
#define GSWF_TIK_11_CHET_RIPPO_FIRST_TALK_DONE GSWF(1342)
// the tik_15 block block00 (evt_mobj_lv_blk, MOBJ_Lv1BigBlock)
#define GSWF_TIK_15_LV1_BIG_BLOCK_1_BROKEN GSWF(1343)
// the tik_15 block block01 (evt_mobj_lv_blk, MOBJ_Lv1BigBlock)
#define GSWF_TIK_15_LV1_BIG_BLOCK_2_BROKEN GSWF(1344)
// the tik_16 tornado switch switch00 (evt_mobj_tornadoswitch_blue) that raises the pipe dokan_1 to mri_00; tik_16
// switch init/after and mri_00 evt_switch_init (shows the pipe end) test it
#define GSWF_TIK_16_MRI_PIPE_RAISED GSWF(1345)
// the tik_16 tornado switch switch01 (evt_mobj_tornadoswitch_blue) that raises the pipe dokan_2 to nok_01; tik_16
// switch init/after and nok_01 evt_switch_init (shows the pipe end) test it
#define GSWF_TIK_16_NOK_PIPE_RAISED GSWF(1346)
// the tik_16 block block (evt_mobj_lv_blk, MOBJ_Lv1BigBigBlock); tik_16 init also tests it
#define GSWF_TIK_16_LV1_BIGBIG_BLOCK_BROKEN GSWF(1347)
// the tik_17 tornado switch switch00 (evt_mobj_tornadoswitch_blue) that raises the pipe dokan_1 to pik_01; tik_17
// switch init/after and pik_01 evt_switch_init (shows the pipe end) test it
#define GSWF_TIK_17_PIK_PIPE_RAISED GSWF(1348)
// the tik_17 tornado switch switch01 (evt_mobj_tornadoswitch_blue) that raises the pipe dokan_2 to muj_02; tik_17
// switch init/after and muj_02 evt_switch_init (shows the pipe end) test it
#define GSWF_TIK_17_MUJ_PIPE_RAISED GSWF(1349)
// the tik_17 block block (evt_mobj_lv_blk, MOBJ_Lv2BigBigBlock); tik_17 init also tests it
#define GSWF_TIK_17_LV2_BIGBIG_BLOCK_BROKEN GSWF(1350)
// set by tik_18_cloud_evt when the gust case (evt_cloud_ent) removes s_win_1; tik_18 init tests it
#define GSWF_TIK_18_WALL_BLOWN_AWAY GSWF(1351)
// set by evt_majin2 (tik_19) when the black chest is opened (paper mage); passed to the chest by tik_19 init
#define GSWF_TIK_19_BLACK_CHEST_OPENED GSWF(1352)
// set by evt_majin (tik_19) after the black chest's first speech; the same event tests it
#define GSWF_TIK_19_BLACK_CHEST_FIRST_TALK_DONE GSWF(1353)
#define GSWF_TIK_00_SOFT_STOMP_CHEST GSWF(1354)  /* the tik_00 chest mobj_box01 holding Soft Stomp */
#define GSWF_TIK_01_DEFEND_PLUS_P_CHEST GSWF(1355) /* the tik_01 chest mobj_box00 holding Defend Plus P */
#define GSWF_TIK_03_DAMAGE_DODGE GSWF(1356)      /* the Damage Dodge lying in tik_03 */
#define GSWF_TIK_04_HAPPY_HEART_P_CHEST GSWF(1357) /* the tik_04 chest mobj_box00 holding Happy Heart P */
// the tik_04 hidden badge block (evt_mobj_brick MOBJ_BRICK_HIDDEN_BADGE) holding Pretty Lucky
#define GSWF_TIK_04_PRETTY_LUCKY_HIDDEN_BLOCK GSWF(1358)
#define GSWF_TIK_07_FLOWER_SAVER_P_CHEST GSWF(1359) /* the tik_07 chest mobj_box00 holding Flower Saver P */
#define GSWF_TIK_10_MUSHROOM_CHEST GSWF(1360)    /* the tik_10 chest mobj_box00 holding a Mushroom */
#define GSWF_TIK_14_MUSHROOM_CHEST GSWF(1361)    /* the tik_14 chest mobj_box00 holding a Mushroom */
// the tik_15 ? block mobj_01 (evt_mobj_badgeblk) holding a Mushroom
#define GSWF_TIK_15_MUSHROOM_BLOCK GSWF(1362)
// the tik_15 ? block mobj_02 (evt_mobj_badgeblk) holding a Fire Flower
#define GSWF_TIK_15_FIRE_FLOWER_BLOCK GSWF(1363)
#define GSWF_TIK_17_FP_PLUS_CHEST GSWF(1364)     /* the tik_17 chest mobj_box01 holding FP Plus */
// the tik_18 ? block mobj_01 (evt_mobj_badgeblk) holding a Slow Shroom
#define GSWF_TIK_18_SLOW_SHROOM_BLOCK GSWF(1365)
// the tik_18 ? block mobj_02 (evt_mobj_badgeblk) holding a Gradual Syrup
#define GSWF_TIK_18_GRADUAL_SYRUP_BLOCK GSWF(1366)
#define GSWF_TIK_20_DEFEND_PLUS GSWF(1367)       /* the Defend Plus lying in tik_20 */
#define GSWF_TIK_21_SPIKE_SHIELD_CHEST GSWF(1368) /* the tik_21 chest mobj_box00 holding Spike Shield */
// set by tik_01 init on the first entry, which fades the music in; later entries start it directly
#define GSWF_TIK_01_ENTERED GSWF(1369)
// set by plane_tobashi_check (tik_04) after the first failed-flight hint; later failures get the second hint
#define GSWF_TIK_04_PLANE_HINT_SEEN GSWF(1370)
// set by powerdownya_table_talk (tik_11) when a greeting is shown; later talks use a short line; tik_11 init clears
// it
#define GSWF_TIK_11_CHET_RIPPO_GREETED_VISIT GSWF(1371)
// set by senkaron_event (gon_03) when the bones push Mario out to gon_01; gon_01 init then runs
// honenoko_oidasare_evt, which clears it
#define GSWF_GON_03_PUSHED_OUT_PENDING GSWF(1476)
// set by senkaron_event (gon_03) the first time Mario is pushed out; a later push sets 1478 instead
#define GSWF_GON_03_PUSHED_OUT_ONCE GSWF(1477)
// set by senkaron_event (gon_03) when Mario is pushed out a second time; honenoko_oidasare_evt (gon_01) tests it for
// the partner's remark
#define GSWF_GON_03_PUSHED_OUT_AGAIN GSWF(1478)
// set by senkaron_event (gon_03) after the partner's complaint about being shoved; the same event tests it
#define GSWF_GON_03_PUSHING_TALK_DONE GSWF(1479)
// set by tegami_event (gon_03) when Mario chooses to have Koops read the letter; no code reads it
#define GSWF_GON_03_LETTER_READ_UNUSED GSWF(1481)
// state of the gon_04 paper stairs, toggled by evt_kaidan_pera (0 = neta_01 shown, 1 = neta_02 shown);
// evt_kaidan_pera_init tests it
#define GSWF_GON_04_PAPER_STAIRS_FLIPPED GSWF(1482)
// the gon_04 blue switch mobj_hashi0 (evt_hashi_pera0) for the right paper bridge
#define GSWF_GON_04_BRIDGE_SWITCH_RIGHT_PRESSED GSWF(1483)
// the gon_04 blue switch mobj_hashi1 (evt_hashi_pera1) for the left paper bridge
#define GSWF_GON_04_BRIDGE_SWITCH_LEFT_PRESSED GSWF(1484)
// the gon_04 block mobj_blk00 (evt_mobj_lv_blk, MOBJ_Lv1BigBlock); evt_gon_04flowblock_updown2 also tests it
#define GSWF_GON_04_LV1_BIG_BLOCK_BROKEN GSWF(1485)
// the gon_04 block mobj_blk01 (evt_mobj_lv_blk, MOBJ_Lv2BigBlock)
#define GSWF_GON_04_LV2_BIG_BLOCK_1_BROKEN GSWF(1486)
// the gon_04 block mobj_blk02 (evt_mobj_lv_blk, MOBJ_Lv2BigBlock)
#define GSWF_GON_04_LV2_BIG_BLOCK_2_BROKEN GSWF(1487)
// the gon_04 lock mobj_lock00 opened with ITEM_CASTLE_KEY_0x0D (only created before STG1_12_02)
#define GSWF_GON_04_DOOR_UNLOCKED GSWF(1488)
// the gon_05 lock mobj_lock00 (y 750) opened with ITEM_CASTLE_KEY_0x0C
#define GSWF_GON_05_LOWER_DOOR_UNLOCKED GSWF(1489)
// the gon_05 lock mobj_lock01 (y 1900) opened with ITEM_CASTLE_KEY_0x0E
#define GSWF_GON_05_UPPER_DOOR_UNLOCKED GSWF(1490)
// the gon_06 red switch mobj_switch1 that opens the grate (evt_ori_open)
#define GSWF_GON_06_SWITCH_PRESSED GSWF(1491)
// set by evt_majin (gon_06) after the black chest's first speech; later touches go to the key check
#define GSWF_GON_06_BLACK_CHEST_FIRST_TALK_DONE GSWF(1492)
// set by evt_majin2 (gon_06) when the black chest is opened (paper mage); gon_06 init forces it on from STG1_13_01_01
// and passes it to the chest
#define GSWF_GON_06_BLACK_CHEST_OPENED GSWF(1493)
// the gon_07 chest gon07treasure holding the Black Key; item_evt sets it, init forces it on from STG1_13
#define GSWF_GON_07_BLACK_KEY_CHEST GSWF(1494)
// set by bomb_wall_evt (gon_07) when the wall san is blown open; gon_07 init tests it
#define GSWF_GON_07_WALL_BOMBED GSWF(1495)
#define GSWF_GON_08_DOOR_UNLOCKED GSWF(1496)     /* the gon_08 lock mobj_lock00 opened with ITEM_CASTLE_KEY_0x0F */
// gonbaba_event (gon_11) tests it to jump past the Hooktail intro and battle; only the debug story jump in event.c
// sets it
#define GSWF_GON_11_HOOKTAIL_INTRO_SKIP GSWF(1498)
// the gon_13 floating blue switch mobj_switch00; switch_evt reveals the chest mobj_box04, init hides the chest while
// it is 0
#define GSWF_GON_13_SWITCH_PRESSED GSWF(1499)
// set by gonbaba_event (gon_11) when Koops was not the active partner and the event swaps him in; the event and
// nokotarou_tabidachi (nok_01) test it to place Goombella and restore the party
#define GSWF_GON_11_KOOPS_SWAPPED_IN GSWF(1500)
#define GSWF_GON_01_HP_PLUS GSWF(1501)           /* the HP Plus lying in gon_01 */
// the gon_02 badge block (evt_mobj_badgeblk kind 1) holding Power Bounce
#define GSWF_GON_02_POWER_BOUNCE_BADGE_BLOCK GSWF(1502)
#define GSWF_GON_05_LAST_STAND_P GSWF(1503)      /* the Last Stand P lying in gon_05 */
#define GSWF_GON_06_ATTACK_FX_R GSWF(1504)       /* the Attack FX R lying in gon_06 */
#define GSWF_GON_08_LIFE_SHROOM GSWF(1505)       /* the Life Shroom lying in gon_08 */
// the gon_12 chest mobj_box00 holding ITEM_CASTLE_KEY_0x0F; init forces it on from STG1_15_01
#define GSWF_GON_12_CASTLE_KEY_CHEST GSWF(1507)
#define GSWF_GON_12_MUSHROOM_CHEST GSWF(1508)    /* the gon_12 chest mobj_box01 holding a Mushroom */
#define GSWF_GON_12_HONEY_SYRUP_CHEST GSWF(1509) /* the gon_12 chest mobj_box02 holding a Honey Syrup */
#define GSWF_GON_13_UP_ARROW_CHEST GSWF(1510)    /* the gon_13 chest mobj_box04 holding an Up Arrow */
// set by chuchurina_sogu (gon_12) when Koops is the partner at the end of the Ms. Mowz encounter; the credits
// (end_00) show picture 53 instead of 9 when set
#define GSWF_GON_12_MOWZ_MET_WITH_KOOPS GSWF(1511)
#define GSWF_NOK_00_VISITED_UNUSED GSWF(1624)    /* set by nok_00 init on entry; only its own guard reads it */
// set by nokorin_talk (nok_01) at Koopie Koo's introduction (before STG1_06_01); the same talk tests it
#define GSWF_NOK_KOOPIE_KOO_FIRST_TALK_DONE GSWF(1625)
// set by nokorin_talk (nok_01) when Koopie Koo lets slip that she has fallen for Koops; the same talk tests it
#define GSWF_NOK_KOOPIE_KOO_CONFESSION_SEEN GSWF(1626)
// set by nokorin_talk (nok_01) when Koopie Koo sends Koops off (Koops in the party, STG1_07..STG1_17); nokorin_init
// tests it for her position
#define GSWF_NOK_KOOPIE_KOO_BLESSING_DONE GSWF(1627)
#define GSWF_NOK_01_MEGA_RUSH_P GSWF(1628)       /* the Mega Rush P lying in nok_01 */
// the Turtley Leaf reward for Listen to Me (boss_noko_talk, nok_01): set when it goes into the bag, else the flag of
// the leaf dropped on the ground (also placed by nok_01 init)
#define GSWF_NOK_01_LISTEN_TO_ME_TURTLEY_LEAF GSWF(1629)
// set by evt_chorobon (hei_12) when the Sun Stone is picked up (Fuzzy ambush); hei_12 init, hei_00 stone slot, hei_01
// Koops request, hei_10 Gold Fuzzy and nok_01 boss_noko_talk test it; hei_00 init forces it on from STG1_08
#define GSWF_HEI_SUN_STONE_TAKEN GSWF(1774)
// set by evt_chorobon (hei_08) when the Moon Stone is picked up (Fuzzy ambush); hei_08 init, hei_00 stone slot,
// hei_01 Koops request, hei_10 Gold Fuzzy and nok_01 boss_noko_talk test it; hei_00 init forces it on from STG1_08
#define GSWF_HEI_MOON_STONE_TAKEN GSWF(1775)
// set by sun_stone_set_event (hei_00) when the Sun Stone is placed; hei_00 init, setting_*_stone and switch_evt test
// it; init forces it on from STG1_08
#define GSWF_HEI_SUN_STONE_PLACED GSWF(1776)
// set by moon_stone_set_event (hei_00) when the Moon Stone is placed; hei_00 init, setting_*_stone and switch_evt
// test it; init forces it on from STG1_08
#define GSWF_HEI_MOON_STONE_PLACED GSWF(1777)
#define GSWF_HEI_00_MUSHROOM_TREE GSWF(1778)     /* the Mushroom in hei_00's tree item_ki02 */
#define GSWF_HEI_00_MUSHROOM_BLOCK GSWF(1780)    /* the hei_00 ? block (evt_mobj_badgeblk) holding a Mushroom */
// set at the end of nokotaro_nakama (hei_01), Koops asking to come along once both stones are taken; the same event
// tests it to skip the scene
#define GSWF_HEI_01_KOOPS_JOIN_REQUEST_SEEN GSWF(1781)
// set at the end of evt_nokorin (hei_01), Koopie Koo's scene after Koops joins; evt_nokorin tests it to skip the
// scene
#define GSWF_HEI_01_KOOPIE_KOO_FAREWELL_SEEN GSWF(1782)
// set by kusa_dokan (hei_01) when the bush anm_kusa_1 reveals the pipe; hei_01 init tests it to show the pipe
#define GSWF_HEI_01_BUSH_PIPE_REVEALED GSWF(1783)
#define GSWF_HEI_01_MYSTERY_BUSH GSWF(1784)      /* the Mystery hidden in hei_01's bush item_kusa_4 */
#define GSWF_HEI_01_FIRE_FLOWER_BLOCK GSWF(1785) /* the hei_01 ? block (evt_mobj_badgeblk) holding a Fire Flower */
#define GSWF_HEI_02_COIN_TREE GSWF(1786)         /* the coin in hei_02's tree item_ki_6 */
#define GSWF_HEI_02_POW_BLOCK_BLOCK GSWF(1787)   /* the hei_02 ? block (evt_mobj_badgeblk) holding a POW Block */
// set with 1789 by either dead event of the linked pillar enemies (hei_03 シンノスケ1/2); hei_03 init and hashira_evt test
// it; the gate opens once both are set; hei_05's copy of hashira_evt also tests it
#define GSWF_HEI_03_PILLAR_ENEMY_1_DEFEATED GSWF(1788)
// set with 1788 by either dead event of the linked pillar enemies (hei_03 シンノスケ1/2); hei_03 init and hashira_evt test
// it; the gate opens once both are set; hei_05's copy of hashira_evt also tests it
#define GSWF_HEI_03_PILLAR_ENEMY_2_DEFEATED GSWF(1789)
#define GSWF_HEI_04_COIN_BUSH GSWF(1790)         /* the coin in hei_04's bush item_kusa_11 */
#define GSWF_HEI_04_COIN_TREE GSWF(1791)         /* the coin in hei_04's tree item_ki_3 */
// set with 1793 by either dead event of the linked pillar enemies (hei_05 シンエモン1/2); hei_05 init tests it; the gate
// opens once both are set
#define GSWF_HEI_05_PILLAR_ENEMY_1_DEFEATED GSWF(1792)
// set with 1792 by either dead event of the linked pillar enemies (hei_05 シンエモン1/2); hei_05 init tests it; the gate
// opens once both are set
#define GSWF_HEI_05_PILLAR_ENEMY_2_DEFEATED GSWF(1793)
#define GSWF_HEI_06_COIN_BUSH GSWF(1794)         /* the coin in hei_06's bush item_kusa_8 */
#define GSWF_HEI_06_FIRE_FLOWER_BLOCK GSWF(1795) /* the hei_06 ? block (evt_mobj_badgeblk) holding a Fire Flower */
// the hei_10 badge block (evt_mobj_badgeblk kind 1) holding Multibounce
#define GSWF_HEI_10_MULTIBOUNCE_BADGE_BLOCK GSWF(1796)
// the hei_13 badge block (evt_mobj_badgeblk kind 1) holding Close Call
#define GSWF_HEI_13_CLOSE_CALL_BADGE_BLOCK GSWF(1797)
// the Happy Heart lying in hei_13 (above the bricks; not placed in the E3 build)
#define GSWF_HEI_13_HAPPY_HEART GSWF(1798)
#define GSWF_HEI_13_TEN_COIN_BRICK GSWF(1799)    /* the hei_13 ten-coin brick (evt_mobj_brick MOBJ_BRICK_TEN_COIN) */
#define GSWF_HEI_01_COIN_BUSH GSWF(1800)         /* the coin in hei_01's bush item_kusa_7 */
#define GSWF_HEI_04_POW_BLOCK_BUSH GSWF(1801)    /* the POW Block in hei_04's bush item_kusa_12 */
#define GSWF_HEI_01_COIN_BUSH_2 GSWF(1802)       /* the coin in hei_01's bush item_kusa_2 */
// cleared by nokotaro_party_lecture (evt_lect_hei) and passed to the tutorial's blue switch, which sets it when
// pressed
#define GSWF_HEI_KOOPS_LECTURE_SWITCH GSWF(1803)
// the hei_00 timer switch pair sw_1/sw_2 (evt_mobj_timerswitch) that runs dokan_evt; only created before STG1_08
#define GSWF_HEI_00_TIMER_SWITCHES_PRESSED GSWF(1804)
// set by christine_hei_01 (hei_01), Goombella's Tattle tip; hei_01 init spawns the event while it is 0 (before
// STG1_01)
#define GSWF_HEI_01_GOOMBELLA_TATTLE_TIP_SEEN GSWF(1805)
#define GSWF_HEI_06_INN_COUPON GSWF(1806)        /* the Inn Coupon lying in hei_06 */
// set by shop_woman_talk (usu_01) when the shop wife asks Mario to cure her husband; lock_item_check,
// mawari_house_delay and usu_01 init (gate pig events) test it
#define GSWF_USU_01_SHOP_WIFE_FIRST_TALK_DONE GSWF(1925)
// the usu_01 storeroom lock (evt_mobj_lock usu_00_lock, Shop Key)
#define GSWF_USU_01_STOREROOM_UNLOCKED GSWF(1926)
// set by aunt_talk (usu_01) when the aunt turns into a pig after the food request; aunt/pig-aunt init and the three
// children's talks test it
#define GSWF_USU_01_AUNT_TURNED_INTO_PIG GSWF(1927)
// set by usu01_crowFG_talk_event (usu_01) after the partner's comment on the crows' talk; skips that comment later
#define GSWF_USU_01_CROWS_FG_PARTNER_TALK_DONE GSWF(1928)
// set by village_chief_talk (usu_00) on the mayor's first line to Mario as a shadow (after the Doopliss battle);
// later talks give the short line
#define GSWF_USU_00_MAYOR_SHADOW_TALK_DONE GSWF(1929)
// set by chief_mario_talk (usu_00) after the mayor thanks the fake Mario; the event returns at once when set
#define GSWF_USU_00_MAYOR_THANKED_FAKE_MARIO GSWF(1930)
// the usu_01 black chest pera_box (evt_mobj_itembox); set by evt_majin2 when the curse is released and by usu_01 init
// from STG4_04_01_PAPER_MAGE_TUBE_MOD on
#define GSWF_USU_01_BLACK_CHEST_OPENED GSWF(1931)
// set by evt_majin (usu_01) after the black chest's first plea; later visits check for the Black Key instead
#define GSWF_USU_01_BLACK_CHEST_FIRST_TALK_DONE GSWF(1932)
#define GSWF_USU_00_COIN_BUSH GSWF(1933)         /* the coin in usu_00's bush item_kusa_06 */
#define GSWF_USU_01_DEFEND_PLUS_CHEST GSWF(1934) /* the usu_01 chest mobj_box00 holding a Defend Plus */
#define GSWF_USU_01_LIFE_SHROOM GSWF(1935)       /* the Life Shroom lying in usu_01 (item_02) */
#define GSWF_USU_01_BOOS_SHEET GSWF(1936)        /* the Boo's Sheet lying in usu_01 (item_03) */
#define GSWF_USU_01_JAMMIN_JELLY GSWF(1937)      /* the Jammin' Jelly lying in usu_01 (item_04) */
// the Meteor Meal reward of trouble 20 (aunt_talk, usu_01); set when given directly, otherwise placed on the table
// and placed again by usu_01 init
#define GSWF_USU_01_METEOR_MEAL GSWF(1938)
#define GSWF_USU_01_INN_COUPON GSWF(1939)        /* the Inn Coupon lying in usu_01 (item_05) */
// set by gra_01_after_ranpel2 (Vivian's comment on the ambush); gra_01 init plays that scene from
// STG4_13_VIVIAN_JOINS while it is clear
#define GSWF_GRA_01_AFTER_DOOPLISS_SCENE_SEEN GSWF(2075)
// set by gra_06_saku_open when the gate chain falls; gra_06 init then leaves the gate open
#define GSWF_GRA_06_GATE_OPENED GSWF(2078)
// set by gra_06_ido (well to jin_09), jin_00_dai (to jin_06) and jin_11_ana (hole to jin_07); the target room's init
// plays the landing and clears it
#define GSWF_JIN_SCRIPTED_ENTRY_PENDING GSWF(2079)
// the gra_05 breaking floor (evt_mobj_breaking_floor breaking_floor)
#define GSWF_GRA_05_BREAKING_FLOOR_BROKEN GSWF(2080)
// set by irekawatta_mario (jin_04, STG4_10_02_TRADING_PLACES), cleared by usu00_vivian_joinup_event (usu_00);
// battle_database_gra's gra_01 groups use their _on formations while set
#define GSWF_JIN_TRADING_PLACES_ACTIVE GSWF(2081)
#define GSWF_GRA_01_COIN_BLOCK GSWF(2082)        /* the gra_01 ? block blk00 (evt_mobj_badgeblk) holding a coin */
#define GSWF_GRA_03_TEN_COIN_BRICK GSWF(2083)    /* the gra_03 ten-coin brick blk00 */
// the gra_03 ? block blk01 (evt_mobj_badgeblk) holding an Earth Quake
#define GSWF_GRA_03_EARTH_QUAKE_BLOCK GSWF(2084)
// the gra_05 hidden ten-coin block blk00 (MOBJ_BRICK_HIDDEN_TEN_COIN)
#define GSWF_GRA_05_HIDDEN_TEN_COIN_BRICK GSWF(2085)
// the gra_04 badge block m (evt_mobj_badgeblk kind 1) holding Hammer Throw
#define GSWF_GRA_04_HAMMER_THROW_BADGE_BLOCK GSWF(2086)
// the gra_01 hidden block blk05 (MOBJ_BRICK_HIDDEN_ITEM) holding a Super Shroom
#define GSWF_GRA_01_SUPER_SHROOM_HIDDEN_BLOCK GSWF(2087)
#define GSWF_GRA_01_TEN_COIN_BRICK GSWF(2088)    /* the gra_01 ten-coin brick blk06 */
#define GSWF_GRA_00_TEN_COIN_BRICK GSWF(2089)    /* the gra_00 ten-coin brick blk02 */
// the gra_06 hidden block mobj_hidden1 (MOBJ_BRICK_HIDDEN_ITEM) holding a coin
#define GSWF_GRA_06_COIN_HIDDEN_BLOCK GSWF(2090)
// the Black Key lying in gra_00; set by its pickup event evt_majin_key, gra_00 init places it while clear
#define GSWF_GRA_00_BLACK_KEY GSWF(2091)
// set by senteresa_evt (jin_00) after the Atomic Boo battle; jin_00 init spawns that event and the Boo NPCs while
// clear; gor_02 C code tests it for a 0x4C result
#define GSWF_JIN_00_ATOMIC_BOO_DEFEATED GSWF(2226)
// set by senteresa_evt (jin_00) when the Boos carry Mario out to gra_06; gra_06 init then runs teresa_oidasare_evt,
// which clears it
#define GSWF_JIN_BOOS_THREW_MARIO_OUT GSWF(2227)
// the jin_00 lock mobj_lock00 (evt_mobj_lock, key ITEM_STEEPLE_KEY_0x16)
#define GSWF_JIN_00_DOOR_UNLOCKED GSWF(2228)
// set by jin_06_wall_open_evt when the gust removes the mekure panel; jin_06 init hides it afterwards
#define GSWF_JIN_06_WALL_PANEL_BLOWN_AWAY GSWF(2229)
// set by evt_open_kagi (jin_08) when the box hako2 is opened and the Steeple Key taken; jin_08 init keeps the lid
// open
#define GSWF_JIN_08_STEEPLE_KEY_TAKEN GSWF(2230)
// the jin_08 lock mobj_lock00 (key ITEM_STEEPLE_KEY_0x17); jin_08 and jin_07 init open the door between them when set
#define GSWF_JIN_08_DOOR_UNLOCKED GSWF(2231)
// set the first time oumu_juyo_talk (jin_08) runs, when the parrot drops its act; usu01_gusun (usu_01 sniffling line)
// stops once set
#define GSWF_JIN_08_PARROT_SECRET_HEARD GSWF(2232)
// the jin_11 breaking floor mobj_bf00 (evt_yuka sets STG4_13_03_BREAKING_THE_FLOOR)
#define GSWF_JIN_11_BREAKING_FLOOR_BROKEN GSWF(2233)
// set the first time a stair switch is hit (evt_kaidan_oku jin_01, evt_kaidan_temae jin_02); skips Mario's and the
// partner's confused reactions later
#define GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN GSWF(2234)
#define GSWF_JIN_05_COOKBOOK_CHEST GSWF(2235)    /* the jin_05 chest mobj_box00 holding the Cookbook */
#define GSWF_JIN_05_ICE_SMASH_CHEST GSWF(2237)   /* the jin_05 chest mobj_box01 holding Ice Smash */
#define GSWF_JIN_06_FLOWER_SAVER_CHEST GSWF(2238) /* the jin_06 chest mobj_box00 holding Flower Saver */
#define GSWF_JIN_08_POWER_PLUS_CHEST GSWF(2239)  /* the jin_08 chest mobj_box01 holding Power Plus */
#define GSWF_JIN_08_MR_SOFTENER_CHEST GSWF(2240) /* the jin_08 chest mobj_box02 holding a Mr. Softener */
// the jin_10 badge block m (evt_mobj_badgeblk kind 1) holding Tornado Jump
#define GSWF_JIN_10_TORNADO_JUMP_BADGE_BLOCK GSWF(2241)
#define GSWF_JIN_00_LUCKY_START GSWF(2242)       /* the Lucky Start the Atomic Boo drops in senteresa_evt (jin_00) */
// set by evt_tougi (tou_03) before the arena tour returns to tou_02; tou_02 init then runs evt_tougi2, which clears
// it (story STG3_01)
#define GSWF_TOU_02_ARENA_TOUR_RETURN GSWF(2374)
// set by evt_sensyu (tou_05) when Grubba walks Mario to tou_04; tou_04 init runs evt_gans, tou_05 init clears it and
// runs evt_sensyu2; tou_04 guards and tou_07 init test it
#define GSWF_TOU_GRUBBA_WALK_TALK_PENDING GSWF(2375)
// set by evt_kinoshikowa (tou_05) after the contract; tou_04 init runs evt_kinoshikowa (rules talk, on to tou_10),
// which clears it; tou_04 guards test it
#define GSWF_TOU_JOLENE_ESCORT_TO_MINORS GSWF(2376)
// set by evt_1st_leagu (tou_05) after the promotion; tou_04 init runs evt_kinoshikowa2 (on to tou_08), which clears
// it
#define GSWF_TOU_JOLENE_ESCORT_TO_MAJORS GSWF(2377)
// set by evt_champion_win (tou_08); tou_04 init runs evt_kinoshikowa3 (on to the champion's room tou_07), which
// clears it
#define GSWF_TOU_JOLENE_ESCORT_TO_CHAMP_ROOM GSWF(2378)
// tou_04 init would run evt_gardman (guard 1 leads Mario to tou_03, story STG3_05) and guard 1/2 events test it;
// nothing sets it to 1 (only cleared)
#define GSWF_TOU_04_GUARD_1_ESCORT_UNUSED GSWF(2379)
// tou_04 init would run evt_gardman2 (guard 3 leads Mario to tou_03); nothing sets it to 1 (only cleared)
#define GSWF_TOU_04_GUARD_3_ESCORT_UNUSED GSWF(2380)
// set by tou_04 init while the title match is booked (GSWF(2383)); runs evt_gardman3 (special match, on to tou_11),
// which clears it; guards 3 and 9 test it
#define GSWF_TOU_04_GUARD_3_ESCORT_TO_TOU_11 GSWF(2381)
// tou_04 init would run evt_gardman4 (guard 6, champion's room, to tou_03); nothing sets it to 1 (only cleared)
#define GSWF_TOU_04_GUARD_6_ESCORT_UNUSED GSWF(2382)
// set by evt_tou_match_make_default_sub (tou.c) when the first match against Rawk Hawk is booked in tou_08 (also
// stg3_21 story jump); cleared by evt_toile (tou_08), the ranking reset and post-chapter matches;
// tou_03/tou_04/tou_11 inits, talk_gardman and evt_champ_lose test it
#define GSWF_GLITZ_PIT_TITLE_MATCH_BOOKED GSWF(2383)
// set by toile_out (tou_09) when Mario leaves through the toilet, cleared by both toilets' toile_in; tou_04 init then
// runs evt_toile (on to tou_08 dokan)
#define GSWF_TOU_09_TOILET_WARP_PENDING GSWF(2384)
// set by toile_out (tou_08) when Mario leaves through the toilet, cleared by both toilets' toile_in; tou_04 init then
// runs evt_toile2 (on to tou_09 dokan)
#define GSWF_TOU_08_TOILET_WARP_PENDING GSWF(2385)
// set by evt_tojikome (tou_11, guard locks Mario in before the title match); the event returns at once when it is set
#define GSWF_TOU_11_LOCKED_IN_SEEN GSWF(2386)
// set by evt_postar (tou_11) when the poster pos_1 is blown off (evt_cloud_ent); tou_11 init hides it when set
#define GSWF_TOU_11_POSTER_BLOWN_AWAY GSWF(2387)
// set by evt_match (tou_03) after every match (and the stg3_30 story jump); the locker-room inits (tou_07/08/10) run
// the after-match event and clear it
#define GSWF_GLITZ_PIT_MATCH_JUST_ENDED GSWF(2389)
// set by room_out, the enter event of tou_10's door n_bero; no code reads it
#define GSWF_TOU_10_ENTERED_UNUSED GSWF(2390)
// set only by the story-jump init stg3_12_init__4 (event.c); tou_10 init then raises the story to
// STG3_12_THE_MINI_YOSHI_EGG
#define GSWF_TOU_10_JUMP_STORY_TO_EGG GSWF(2391)
// set by tou_10 init when it runs evt_saisyo_after (first match after-event: Grubba, fight money, King K's crew);
// cleared by the ranking reset
#define GSWF_TOU_GOOMBA_BROS_AFTER_MATCH_SEEN GSWF(2392)
// set by evt_1st_leagu (tou_10) when Jolene takes Mario to Grubba's office; tou_05 init runs the promotion
// evt_1st_leagu, tou_08 init clears it
#define GSWF_TOU_PROMOTION_TO_MAJORS_PENDING GSWF(2393)
// set by talk_gardman (tou.c) when the guard takes Mario out of tou_08, and by stg3_20_init_1 (event.c); no code
// reads it
#define GSWF_TOU_08_GUARD_ESCORT_UNUSED GSWF(2394)
// set by evt_match (tou_03) the first time the opponent is the Goomba Bros. (fighter 19): evt_kurikuri_opening
// instead of the default opening
#define GSWF_TOU_GOOMBA_BROS_OPENING_SEEN GSWF(2395)
// set by evt_match (tou_03) on the first win over the Goomba Bros. (evt_kurikuri_win); evt_saisyo_after (tou_10) pays
// 3 coins instead of 1
#define GSWF_TOU_GOOMBA_BROS_FIRST_WIN GSWF(2396)
// set by evt_match (tou_03) the first time the opponent is the Armored Harriers (fighter 10): evt_shinemon_opening;
// match booking (tou.c) and init_poku (tou_10) test it
#define GSWF_TOU_ARMORED_HARRIERS_OPENING_SEEN GSWF(2397)
// set by evt_match (tou_03) on the first win over the Armored Harriers (evt_shinemon_win)
#define GSWF_TOU_ARMORED_HARRIERS_FIRST_WIN GSWF(2398)
// set by evt_match (tou_03) the first time the opponent is Rawk Hawk (fighter 0): evt_champ_opening(_re); cleared by
// the ranking reset; tou_04/tou_08 NPCs and the toilet door use it for the pre-title-match state
#define GSWF_TOU_RAWK_HAWK_OPENING_SEEN GSWF(2399)
// set by evt_match (tou_03) on the first win over Rawk Hawk (evt_champ_win); cleared by the ranking reset
#define GSWF_TOU_RAWK_HAWK_FIRST_WIN GSWF(2400)
// set by evt_dakuto (tou_05) the first time the duct grate falls and the partner comments (stg3_tou_456); also set by
// stg3_32_init (event.c)
#define GSWF_TOU_05_DUCT_GRATE_FELL GSWF(2401)
// set by evt_momikucha (tou_02) the first time after the first match: fans crowd around the champion NPC; also set by
// tou_02 init at STG3_33_06
#define GSWF_TOU_02_FANS_MOB_RAWK_HAWK_SEEN GSWF(2402)
// set by evt_momikucha (tou_02) the first time Mario is rank 0 (champion): fans crowd Mario; cleared by the ranking
// reset
#define GSWF_TOU_02_FANS_MOB_MARIO_SEEN GSWF(2403)
// set by evt_tou_match_make_default_sub (tou.c) when a match against the KP Koopas (fighter 18) is first booked:
// evt_michel_1 (King K)
#define GSWF_TOU_KING_K_PREMATCH_TALK_SEEN GSWF(2405)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 18: evt_michel_2; init_brots (tou_10) tests it
#define GSWF_TOU_RANK_18_KING_K_TALK_DONE GSWF(2406)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 17: evt_michel_3 (King K, Bandy Andy, Jolene)
#define GSWF_TOU_RANK_17_KING_K_TALK_DONE GSWF(2407)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 16: evt_michel_4
#define GSWF_TOU_RANK_16_KING_K_TALK_DONE GSWF(2408)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 15: evt_michel_5
#define GSWF_TOU_RANK_15_KING_K_TALK_DONE GSWF(2409)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 14: evt_michel_6
#define GSWF_TOU_RANK_14_KING_K_TALK_DONE GSWF(2410)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 13: evt_michel_7 (King K thinks about retiring)
#define GSWF_TOU_RANK_13_KING_K_TALK_DONE GSWF(2411)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 12; rankingInit (tou.c/tou2.c) then hides the KP
// Koopas; init_michel (tou_10) tests it
#define GSWF_GLITZ_PIT_KP_KOOPAS_RETIRED GSWF(2412)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 11: evt_michel_8 (Sir Swoop arrives); rankingInit
// then shows the Wings of Night
#define GSWF_GLITZ_PIT_SIR_SWOOP_JOINED GSWF(2413)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 8: evt_mail_1 (mail from X, story STG3_20_00)
#define GSWF_TOU_RANK_8_MAIL_DONE GSWF(2416)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 7: evt_mail_2; while it is 0, evt_match (tou_03)
// runs the evt_random_battle ambush at rank 7
#define GSWF_TOU_RANK_7_MAIL_DONE GSWF(2417)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 6: evt_cake (a fan's cake on the table);
// evt_cake_init tests it
#define GSWF_TOU_RANK_6_CAKE_DONE GSWF(2418)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 5: evt_koopa_after; while it is 0, evt_match
// (tou_03) runs the evt_random_battle2 ambush at rank 5
#define GSWF_TOU_RANK_5_BOWSER_EVENT_DONE GSWF(2419)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 4: evt_mail_3 (mail from X, story STG3_20_05)
#define GSWF_TOU_RANK_4_MAIL_DONE GSWF(2420)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 3: evt_mail_4; rankingInit then hides the Hand-
// It-Overs and shows the Destructors; init_brots (tou_04) tests it
#define GSWF_GLITZ_PIT_DESTRUCTORS_JOINED GSWF(2421)
// set by evt_tou_match_after_default (tou.c) on first reaching rank 2 or 1: evt_mail_5 (remove the lobby posters,
// story STG3_20_13)
#define GSWF_TOU_RANK_1_MAIL_DONE GSWF(2423)
// tou_06 MOBJ_Lv1BigBlock "a" (evt_mobj_lv_blk); also set by stg3_20_12_init (event.c)
#define GSWF_TOU_06_BIG_BLOCK_A_BROKEN GSWF(2425)
// tou_06 MOBJ_Lv1BigBlock "b" (evt_mobj_lv_blk); also set by stg3_20_12_init (event.c)
#define GSWF_TOU_06_BIG_BLOCK_B_BROKEN GSWF(2426)
// tou_06 MOBJ_Lv1BigBlock "c" (evt_mobj_lv_blk); also set by stg3_20_12_init (event.c)
#define GSWF_TOU_06_BIG_BLOCK_C_BROKEN GSWF(2427)
// tou_06 MOBJ_Lv1BigBlock "d" (evt_mobj_lv_blk); also set by stg3_20_12_init (event.c)
#define GSWF_TOU_06_BIG_BLOCK_D_BROKEN GSWF(2428)
// tou_06 MOBJ_Lv1BigBlock "e" (evt_mobj_lv_blk); also set by stg3_20_12_init (event.c)
#define GSWF_TOU_06_BIG_BLOCK_E_BROKEN GSWF(2429)
// tou_06 MOBJ_Lv1BigBlock "f" (evt_mobj_lv_blk); also set by stg3_20_12_init (event.c)
#define GSWF_TOU_06_BIG_BLOCK_F_BROKEN GSWF(2430)
// tou_06 MOBJ_Lv1BigBlock "g" (evt_mobj_lv_blk); also set by stg3_20_12_init (event.c)
#define GSWF_TOU_06_BIG_BLOCK_G_BROKEN GSWF(2431)
// tou_06 MOBJ_Lv1BigBlock "h" (evt_mobj_lv_blk); also set by stg3_20_12_init (event.c)
#define GSWF_TOU_06_BIG_BLOCK_H_BROKEN GSWF(2432)
#define GSWF_TOU_06_BIG_BLOCK_I_BROKEN GSWF(2433) /* tou_06 MOBJ_Lv1BigBlock "i" (evt_mobj_lv_blk) */
#define GSWF_TOU_06_BIG_BLOCK_J_BROKEN GSWF(2434) /* tou_06 MOBJ_Lv1BigBlock "j" (evt_mobj_lv_blk) */
// set by evt_kontena2 (tou_06) when the second container (kontena_2, a_kon_N) is blown away; tou_06 init hides it
// when set
#define GSWF_TOU_06_CONTAINER_2_BLOWN_AWAY GSWF(2435)
#define GSWF_TOU_04_STORAGE_DOOR_UNLOCKED GSWF(2436) /* the tou_04 storage room lock (evt_mobj_lock "lock") */
// the tou_06 lock (evt_mobj_lock "lock", opened with ITEM_STORAGE_KEY_0x13)
#define GSWF_TOU_06_DOOR_UNLOCKED GSWF(2437)
// the tou_06 blue switch "sw" (evt_mobj_switch_blue) that runs evt_kaidan (hidden stairway)
#define GSWF_TOU_06_SWITCH_PRESSED GSWF(2438)
// tou_06 MOBJ_Lv1BigBigBlock "aa" (evt_mobj_lv_blk, evt_block_broken: story STG3_20_11)
#define GSWF_TOU_06_BIG_BIG_BLOCK_BROKEN GSWF(2439)
// tou_10 MOBJ_Lv1BigBigBlock "blk" (evt_mobj_lv_blk, evt_block: story STG3_20_03)
#define GSWF_TOU_10_BIG_BIG_BLOCK_BROKEN GSWF(2440)
#define GSWF_TOU_12_BIG_BIG_BLOCK_BROKEN GSWF(2441) /* tou_12 MOBJ_Lv1BigBigBlock "aa" (evt_mobj_lv_blk) */
// set by evt_tou_set_ranking_win_lose (tou2.c/tou.c) when Mario won the last match, cleared when he lost; fight money
// (evt_tou_match_after_default, evt_1st_leagu) and partner lines test it
#define GSWF_GLITZ_PIT_BEAT_LAST_OPPONENT GSWF(2442)
// set by talk_brots (tou_04) after Bandy Andy tells about the Seven Wonders of the Glitz Pit; changes his greeting
#define GSWF_TOU_04_SEVEN_WONDERS_HEARD GSWF(2444)
// set by evt_souko_talk (tou_04) when Jolene finds Mario at the storage room; evt_souko_talk and init_bijyo (tou_01)
// test it
#define GSWF_TOU_04_STORAGE_ROOM_TALK_SEEN GSWF(2445)
// set by tou_10 init on the first entry with story at STG3_12 or later; first_tamago (partner pep talk) runs then
// only if GSW(505) is 2 or more
#define GSWF_TOU_10_HARRIERS_PEP_TALK_CHECKED GSWF(2446)
// set by evt_cake (tou.c) when the fan's cake is put on the locker-room table, cleared when eaten (evt_eat_cake) or
// by evt_tou_08_cake; evt_cake_init and tou_08 init test it
#define GSWF_TOU_CAKE_ON_TABLE GSWF(2447)
// set by evt_tou_match_make_default_sub (tou.c) when a match is booked at rank 3: evt_doku_cake (Jolene leaves
// another cake); also set by stg3_20_init_2 (event.c)
#define GSWF_TOU_POISON_CAKE_DELIVERED GSWF(2448)
// set by evt_doku_cake (tou.c), cleared by evt_eat_doku_cake; no code reads it
#define GSWF_TOU_POISON_CAKE_ON_TABLE_UNUSED GSWF(2449)
// set by evt_eat_doku_cake (tou.c) when the partners eat the cake and go numb; talk_gardman, the after-match event
// and the tou_03/04/10 inits test it (partners out of the party)
#define GSWF_TOU_POISON_CAKE_EATEN GSWF(2450)
// set by evt_tou_match_after_default (tou.c) after the next match once the cake was delivered; partners who ate it
// rejoin the party; also set by stg3_20_init_2 (event.c)
#define GSWF_TOU_POISON_CAKE_RECOVERED GSWF(2451)
// set by the story-jump init stg3_20_init_7 (event.c), cleared by stg3_20_init_6; tou_08 init clears it and sets the
// story to STG3_20_04_CONFISCATED
#define GSWF_TOU_08_JUMP_STORY_CONFISCATED GSWF(2452)
// set by the story-jump init stg3_20_init_4 (event.c), cleared by stg3_20_06_init; tou_08 init clears it and sets the
// story to STG3_20_05_MAJORS_4TH_PL
#define GSWF_TOU_08_JUMP_STORY_MAJORS_4TH GSWF(2453)
// set by the story-jump init stg3_20_init_1 (event.c), cleared by stg3_20_13_init; tou_08 init clears it and sets the
// story to STG3_20_13_MAJORS_1ST_PL
#define GSWF_TOU_08_JUMP_STORY_MAJORS_1ST GSWF(2454)
// set by evt_champion (tou_03, before the first title match, then on to tou_11 nidome); tou_03 init runs
// evt_champion2 instead once set
#define GSWF_TOU_03_TITLE_MATCH_INTRO_SEEN GSWF(2455)
// set by evt_poster_1 when the tou_02 lobby poster p_m_1 is blown off (evt_cloud_ent); tou_02 init hides it when set
#define GSWF_TOU_02_POSTER_1_REMOVED GSWF(2456)
// set by evt_poster_2 when the tou_02 lobby poster p_m_2 is blown off; tou_02 init hides it when set
#define GSWF_TOU_02_POSTER_2_REMOVED GSWF(2457)
// set by evt_poster_3 when the tou_02 lobby poster p_m_3 is blown off; tou_02 init hides it when set
#define GSWF_TOU_02_POSTER_3_REMOVED GSWF(2458)
// set by evt_poster_4 when the tou_02 lobby poster p_m_4 is blown off; tou_02 init hides it when set
#define GSWF_TOU_02_POSTER_4_REMOVED GSWF(2459)
// set by evt_poster_5 when the tou_02 lobby poster p_m_5 is blown off; tou_02 init hides it when set
#define GSWF_TOU_02_POSTER_5_REMOVED GSWF(2460)
// set by evt_poster_6 when the tou_02 lobby poster p_m_6 is blown off; tou_02 init hides it when set
#define GSWF_TOU_02_POSTER_6_REMOVED GSWF(2461)
// set by evt_poster_7 when the tou_02 lobby poster p_m_7 is blown off; tou_02 init hides it when set
#define GSWF_TOU_02_POSTER_7_REMOVED GSWF(2462)
// set by evt_poster_8 when the tou_02 lobby poster p_m_8 is blown off (at STG3_20_13 it drops the Storage Key);
// tou_02 init hides it and re-places the key
#define GSWF_TOU_02_POSTER_8_REMOVED GSWF(2463)
// set by evt_poster_9 when the tou_02 lobby poster p_m_9 is blown off; tou_02 init hides it when set
#define GSWF_TOU_02_POSTER_9_REMOVED GSWF(2464)
// set by evt_gondora2 (tou_01) the first time Mario leaves by blimp after the chapter (story STG3_34+), which also
// resets the ranking (evt_rankingReset); also set by stg3_35_init; many tou_01/02/04/10 NPCs test it
#define GSWF_TOU_LEFT_AFTER_CHAPTER GSWF(2496)
// set by evt_move_g (tou_02) when the lobby guard steps aside (story STG3_02); tou_02 init and the guards test it
// (after the chapter the doors stay shut until it is set)
#define GSWF_TOU_02_GUARD_STEPPED_ASIDE GSWF(2497)
// set by talk_g_4 (tou_04) after the chapter when the office guard lets Mario in to see Jolene and fight again;
// init_g_4 and tou_04 init test it
#define GSWF_TOU_04_GUARD_4_LET_IN_REMATCH GSWF(2498)
// set by talk_g_4 (tou_04) after the chapter when the office guard lets Mario in for trouble 13 or 23; init_g_4 and
// tou_04 init test it
#define GSWF_TOU_04_GUARD_4_LET_IN_TROUBLE GSWF(2499)
// set by evt_sensyu_reentry (tou_05) when Jolene signs Mario up again as a minor-league fighter after the chapter;
// tou_05 init and many tou_01/02/04 NPCs test it
#define GSWF_TOU_REMATCH_REGISTERED GSWF(2500)
// set by evt_dakuto (tou_13) when the duct exit is hammered open again after the story reached STG3_30_04; tou_05 and
// tou_13 inits test it after the chapter
#define GSWF_TOU_13_DUCT_EXIT_REOPENED GSWF(2501)
// set by evt_dakuto (tou_07) when the vent (win_kabe) is hammered open again after the story reached STG3_30_01;
// tou_07 init tests it after the chapter
#define GSWF_TOU_07_VENT_REOPENED GSWF(2502)
// set by evt_champion_re (tou_07) when Mario wins the title again after the chapter (Champ's Belt); tou_07 init,
// tou_08 fighters and Rawk Hawk's entry_event test it
#define GSWF_TOU_CHAMPION_TITLE_REGAINED GSWF(2503)
// set by evt_1st_leagu_re (tou_10) when Mario is moved to the major league again after the chapter; tou_01/tou_02
// NPCs test it
#define GSWF_TOU_REMATCH_PROMOTED_TO_MAJORS GSWF(2504)
// set by evt_tou_match_after_default (tou.c) after the chapter when Mario's rank falls to 11th; cleared there when
// the champion loses the title; tou_08/tou_10 fighters' talk tests it
#define GSWF_TOU_REMATCH_DEMOTED_TO_MINORS GSWF(2505)
// the tou_01 hidden block hblk01 (evt_mobj_brick, MOBJ_BRICK_HIDDEN_ITEM) holding a coin
#define GSWF_TOU_01_COIN_HIDDEN_BLOCK GSWF(2507)
// set by evt_hiki_open (tou_05) when Mario takes the Star Piece from the desk drawer hikidasi_N (with
// GSWF_STARPIECE_58)
#define GSWF_TOU_05_DRAWER_STAR_PIECE GSWF(2508)
// set by unit_kurikuri phase_event (tou2) after the turn-1 talk about the early attack (stg3_tou_83_*); while it is
// 0, evt_match (tou_03) gives the opponent the first attack
#define GSWF_TOU_GOOMBA_BROS_AMBUSH_TALK_SEEN GSWF(2509)
// set by unit_iron_sinemon(2) phase_event after their first turn-1 talk; later fights print stg3_tou_267 instead of
// stg3_tou_266
#define GSWF_TOU_ARMORED_HARRIERS_INTRO_SEEN GSWF(2510)
// set by unit_iron_sinemon(2) damage_event after they first take damage and complain (stg3_tou_272/273); the talk
// runs only once
#define GSWF_TOU_ARMORED_HARRIERS_WEAK_POINT GSWF(2511)
// set by evt_shinemon_win (tou_03); unit_iron_sinemon(2) phase_event skips the turn-1 talk once set
#define GSWF_TOU_ARMORED_HARRIERS_BEATEN GSWF(2512)
// set by unit_boss_champion entry_event on the first title match (stg3_tou_432..435); later fights use stg3_tou_436;
// also set by stg3_30_init
#define GSWF_TOU_RAWK_HAWK_INTRO_SEEN GSWF(2513)
// set by unit_boss_champion phase_event when Rawk Hawk first drops to 67 % HP (stg3_tou_436_02 before the chapter
// ends); entry_event clears it for rematches
#define GSWF_TOU_RAWK_HAWK_67_HP_TALK GSWF(2514)
// set by unit_boss_champion phase_event when Rawk Hawk first drops to 34 % HP (stg3_tou_436_04); only before STG3_30
#define GSWF_TOU_RAWK_HAWK_34_HP_TALK GSWF(2515)
// set by unit_boss_champion phase_event when Rawk Hawk first drops to 25 % HP (stg3_tou_436_05); only before STG3_30
#define GSWF_TOU_RAWK_HAWK_25_HP_TALK GSWF(2516)
// set by init_g_1 (tou_04) when Mario comes out of the minor-league locker room (s_bero_7) at STG3_20_04 and guard 1
// steps aside; tou_10 fighters' talk tests it; also set by stg3_20_init_7
#define GSWF_TOU_04_CONFISCATED_EXIT_SEEN GSWF(2517)
// set by talk_brots (tou_12) after the flattened Bandy Andy's warning (stg3_tou_307_12); init_brots and talk_brots
// test it
#define GSWF_TOU_12_BANDY_ANDY_WARNING_HEARD GSWF(2518)
// the tou_01 chest mobj_box00 (evt_mobj_itembox) holding a Power Plus P
#define GSWF_TOU_01_POWER_PLUS_P_CHEST GSWF(2519)
#define GSWF_TOU_04_LAST_STAND GSWF(2520)        /* the Last Stand lying in tou_04 (evt_item_entry item_01) */
#define GSWF_TOU_06_HP_PLUS_P GSWF(2521)         /* the HP Plus P lying in tou_06 (evt_item_entry item_01) */
// the tou_01 tornado switch dummy_sw (evt_mobj_tornadoswitch_blue); tou_01 init then shows the changed sign and the
// stairs
#define GSWF_TOU_01_TORNADO_SWITCH_PRESSED GSWF(2522)
#define GSWF_TOU_06_CHARGE_P GSWF(2523)          /* the Charge P lying in tou_06 (evt_item_entry item_02) */
#define GSWF_TOU_08_ICE_STORM GSWF(2524)         /* the Ice Storm lying in tou_08 (evt_item_entry item_01) */
// set while the automatic door sound plays (n_door_open/close in tou_01, s_door_open/close in tou_02), cleared when
// the door finishes; keeps the sound from starting twice
#define GSWF_TOU_AUTO_DOOR_SFX_PLAYING GSWF(2526)
// the Hot Sauce from trouble Security Code (talk_sarary, tou_01): taken at once or left as item iri_24 when the
// inventory is full
#define GSWF_TOU_01_HOT_SAUCE_REWARD GSWF(2528)
// set by rankingInit (tou.c/tou2.c); once set, the fighters' points are loaded from the saved bytes instead of the
// defaults; cleared by the ranking reset
#define GSWF_GLITZ_PIT_RANKING_SAVED GSWF(2529)
// set by rankingInit (tou.c/tou2.c) the first time the Wings of Night appear (GSWF(2413)); they start marked beaten
// (below Mario) only that first time
#define GSWF_GLITZ_PIT_WINGS_OF_NIGHT_PLACED GSWF(2530)
// set by rankingInit (tou.c/tou2.c) the first time the Destructors appear (GSWF(2421)); they start marked beaten
// (below Mario) only that first time
#define GSWF_GLITZ_PIT_DESTRUCTORS_PLACED GSWF(2531)
// set by evt_tou_set_ranking_win_lose (tou.c/tou2.c) with GSW(507); rankingControll then keeps that opponent next to
// Mario; cleared by the ranking reset
#define GSWF_GLITZ_PIT_LAST_OPPONENT_SAVED GSWF(2532)
#define GSWF_TOU_01_INN_COUPON GSWF(2533)        /* the Inn Coupon lying in tou_01 (evt_item_entry item_05) */
// the Necklace the Shadow Sirens drop (witchtrio_lose, win_00); win_00 init places it at STG2_08 and adds the キザ野郎
// NPC once it is taken
#define GSWF_WIN_00_NECKLACE GSWF(2675)
#define GSWF_WIN_00_HONEY_SYRUP GSWF(2676)       /* the Honey Syrup lying in win_00 */
// the win_02 badge block mobj_badge_gat (evt_mobj_badgeblk kind 1) holding Quake Hammer
#define GSWF_WIN_02_QUAKE_HAMMER_BADGE_BLOCK GSWF(2677)
// the win_02 blue switch mobj_hashi0; evt_hikou_nobiru raises the plane panel, win_02 init restores it when set
#define GSWF_WIN_02_PLANE_PANEL_SWITCH_PRESSED GSWF(2678)
#define GSWF_WIN_03_VOLT_SHROOM GSWF(2679)       /* the Volt Shroom lying in win_03 */
// no code sets it; battle_database_win's win_02 groups would use their _on formations when set
#define GSWF_WIN_02_ALT_BATTLES_UNUSED GSWF(2680)
// set by clouda_irai (win_04, STG2_05_02_FLURRIE_REQUEST); battle_database_win's win_03 group uses its _on formation
// when set
#define GSWF_WIN_FLURRIE_REQUEST_ACCEPTED GSWF(2681)
// set by clouda_nakama (win_04) when Flurrie joins; battle_database_win's win_00 groups use their _on formations when
// set
#define GSWF_WIN_FLURRIE_JOINED GSWF(2682)
// the win_02 hidden badge block mobj_box01 (MOBJ_BRICK_HIDDEN_BADGE) holding P-Down D-Up P
#define GSWF_WIN_02_P_DOWN_D_UP_P_HIDDEN_BLOCK GSWF(2683)
#define GSWF_WIN_05_SUPER_APPEAL_P_CHEST GSWF(2684) /* the win_05 chest mobj_box00 holding Super Appeal P */
#define GSWF_WIN_00_SLEEPY_SHEEP_TREE GSWF(2685) /* the Sleepy Sheep in win_00's tree item_ki01 */
#define GSWF_WIN_01_COIN_TREE GSWF(2686)         /* the coin in win_01's tree item_ki03 */
#define GSWF_WIN_01_INN_COUPON GSWF(2687)        /* the Inn Coupon lying in win_01 */
#define GSWF_MRI_00_FP_PLUS GSWF(2825)           /* the FP Plus lying in mri_00 */
// set by out_ne_bero (mri_00) the first time Mario leaves with Punio, who then follows him; guide_init and mri_00
// init (dancers) test it
#define GSWF_MRI_00_PUNIO_LEFT_WITH_MARIO GSWF(2826)
// set by sister_talk (mri_01) when Petuni's guessing game is won (Mystic Egg); sister_talk tests it; cleared on
// leaving through mri_00 out_dokan_1 / tik_03 bero_05_custom_in after STG2_25
#define GSWF_MRI_PETUNI_GAME_WON_THIS_VISIT GSWF(2827)
#define GSWF_MRI_01_COIN_PLANT GSWF(2828)        /* the coin in mri_01's plant item_ueki_03 */
// set by elder_kaisetsu (mri_01) after the Puni Elder's tip about lost Punies; mri_01 init spawns it at STG2_15_11
// while clear, elder_talk tests it
#define GSWF_MRI_01_ELDER_TIP_SEEN GSWF(2829)
// set/cleared by punipuni_ball00 (mri_02) when the Puni Orb is placed on or taken off its stand; the weight floor and
// pipe event test it
#define GSWF_MRI_02_PUNI_ORB_PLACED GSWF(2830)
#define GSWF_MRI_02_POWER_PUNCH_PLANT GSWF(2831) /* the Power Punch in mri_02's plant item_ueki_04 */
// the mri_03 red cell lock mobj_lock00 (Red Key); jail events and elder_talk test it
#define GSWF_MRI_03_RED_JAIL_UNLOCKED GSWF(2832)
// set by red_jail_open (mri_03) when the red cell door opens (Puni Elder rescue); mri_03 init then shows it open
#define GSWF_MRI_03_RED_JAIL_OPENED GSWF(2833)
#define GSWF_MRI_03_BLUE_JAIL_UNLOCKED GSWF(2834) /* the mri_03 blue cell lock mobj_lock01 (Blue Key) */
// set by blue_jail_open (mri_03) when the blue cell door opens; mri_03 init then shows it open
#define GSWF_MRI_03_BLUE_JAIL_OPENED GSWF(2835)
// set by guide_sister_before (mri_03) when Punio waits at the cell, cleared by guide_sister; puni_return,
// mri_03/mri_07 init and mri_puni.c leave Punio out of the group while set
#define GSWF_MRI_PUNIO_WAITING_AT_JAIL GSWF(2836)
#define GSWF_MRI_04_ULTRA_SHROOM_CHEST GSWF(2837) /* the mri_04 chest mobj_box00 holding an Ultra Shroom */
#define GSWF_MRI_04_COIN_PLANT GSWF(2838)        /* the coin in mri_04's plant item_ueki_02 */
// the Red Key the X-Naut drops after the battle in chuchurina_sogu (mri_04)
#define GSWF_MRI_04_RED_KEY GSWF(2839)
// set by chibitoge_talk (mri_04) when the baby Jabbi gives a Mushroom; chibitoge_init tests it; cleared on leaving
// through mri_00 out_dokan_1 / tik_03 bero_05_custom_in after STG2_25
#define GSWF_MRI_04_BABY_JABBI_GIFT_THIS_VISIT GSWF(2840)
// toggled by door_switch (mri_05); opens the mri_05 west door and the mri_01 east door
#define GSWF_MRI_05_DOOR_SWITCH_ON GSWF(2841)
// set/cleared by punipuni_ball00 (mri_05) when the Puni Orb is placed on or taken off the first stand
#define GSWF_MRI_05_PUNI_ORB_PLACED_1 GSWF(2842)
// set/cleared by punipuni_ball01 (mri_05) when the Puni Orb is placed on or taken off the second stand
#define GSWF_MRI_05_PUNI_ORB_PLACED_2 GSWF(2843)
#define GSWF_MRI_05_THUNDER_RAGE_PLANT GSWF(2844) /* the Thunder Rage in mri_05's plant item_ueki_04 */
// the mri_06 badge block mobj_badge_gat (evt_mobj_badgeblk kind 1) holding ITEM_DAMAGE_DODGE_P
#define GSWF_MRI_06_DAMAGE_DODGE_P_BADGE_BLOCK GSWF(2845)
// set by cloud_wind_event when the gust removes the fake wall S_nisekabe; mri_06 init hides it afterwards
#define GSWF_MRI_06_FAKE_WALL_BLOWN_AWAY GSWF(2846)
// set by puni_vs_toge (mri_08) after the 10-Jabbi squadron intro; mri_08 init then sets the battle up without the
// intro
#define GSWF_MRI_08_JABBI_SQUAD_INTRO_SEEN GSWF(2847)
// set by puni_lose (mri_08) when the Punies lose to the Jabbies; mri_06 puni_lose shows the regroup line and clears
// it
#define GSWF_MRI_08_JABBI_BATTLE_LOST GSWF(2848)
// set by puni_win (mri_08); mri_08 init and mri_puni.c's remaining-count check test it
#define GSWF_MRI_08_JABBI_SQUAD_DEFEATED GSWF(2849)
// set by wall_roll (mri_09) when the four rotating wall pieces line up; evt_puni_set_maze (mri_puni.c) then uses the
// solved maze data
#define GSWF_MRI_09_WALL_PUZZLE_SOLVED GSWF(2850)
// the mri_09 breaking floor mobj_bf00; breakyuka_init and mri_puni.c's floor height test it
#define GSWF_MRI_09_BREAKING_FLOOR_BROKEN GSWF(2851)
#define GSWF_MRI_09_BLUE_KEY_CHEST GSWF(2852)    /* the mri_09 chest mobj_box00 holding the Blue Key */
#define GSWF_MRI_09_CHARGE GSWF(2853)            /* the Charge lying in mri_09 */
#define GSWF_MRI_10_SUPER_BOOTS_CHEST GSWF(2854) /* the mri_10 big chest mobj_box00 holding the Super Boots */
// set by puni_vs_toge (mri_11) after the Jabbi squadron intro; mri_11 init then skips it
#define GSWF_MRI_11_JABBI_SQUAD_INTRO_SEEN GSWF(2855)
// set by puni_lose (mri_11) when the Punies lose; mri_06 puni_lose shows the regroup line and clears it
#define GSWF_MRI_11_JABBI_BATTLE_LOST GSWF(2856)
// set by puni_win (mri_11); mri_11 init and mri_puni.c's remaining-count check test it
#define GSWF_MRI_11_JABBI_SQUAD_DEFEATED GSWF(2857)
// the mri_12 breaking floor mobj_bf00; breakyuka_init and mri_puni.c's floor height test it
#define GSWF_MRI_12_BREAKING_FLOOR_BROKEN GSWF(2858)
// set/cleared by punipuni_ball00 (mri_12) when the Puni Orb is placed on or taken off its stand
#define GSWF_MRI_12_PUNI_ORB_PLACED GSWF(2859)
#define GSWF_MRI_12_COIN_PLANT GSWF(2860)        /* the coin in mri_12's plant item_ueki_01 */
// set/cleared by punipuni_ball00 (mri_13) when the Puni Orb is placed on or taken off its stand; yuka_event tests it
#define GSWF_MRI_13_PUNI_ORB_PLACED GSWF(2861)
#define GSWF_MRI_13_MUSHROOM_PLANT GSWF(2862)    /* the Mushroom in mri_13's plant item_ueki_01 */
// set/cleared by punipuni_ball00 (mri_14) when the Puni Orb is placed on or taken off its stand
#define GSWF_MRI_14_PUNI_ORB_PLACED GSWF(2863)
// set by dai3_kanbu (mri_14) before it reloads mri_14 at STG2_22_LORD_CRUMP_APPEARS; first_time_w_bero tests and
// clears it to play the entry scene
#define GSWF_MRI_14_CRUMP_REENTRY_PENDING GSWF(2864)
// toggled by door_switch (mri_15); opens the mri_15 west door and the mri_06 east door
#define GSWF_MRI_15_DOOR_SWITCH_ON GSWF(2865)
// set by evt_dokan (mri_15, pipe entry) or by dai3_battle after the X-Naut battle; while clear Lord Crump stands in
// mri_15 and escapes during the battle
#define GSWF_MRI_15_CRUMP_GONE GSWF(2866)
// the mri_16 blue switch mobj00 (water_shikake drains the water); mri_16 init and mri_puni.c's maze data test it
#define GSWF_MRI_16_WATER_SWITCH_PRESSED GSWF(2867)
#define GSWF_MRI_16_RIDE_SWITCH_PRESSED GSWF(2868) /* the mri_16 light-blue ride switch mobj01 (bar_shikake) */
#define GSWF_MRI_16_DIZZY_DIAL GSWF(2869)        /* the Dizzy Dial lying in mri_16 */
// set/cleared by punipuni_ball00 (mri_17) when the Puni Orb is placed on or taken off its stand; trap_check tests it
#define GSWF_MRI_17_PUNI_ORB_PLACED GSWF(2870)
// the mri_17 breaking floor mobj_bf00; breakyuka_init and mri_puni.c's floor height test it
#define GSWF_MRI_17_BREAKING_FLOOR_BROKEN GSWF(2871)
// set by bero_02_into_event (mri_17) when it first spawns strange_statue (STG2_21_01_MYSTERY_PILLAR)
#define GSWF_MRI_17_PILLAR_SCENE_SEEN GSWF(2872)
#define GSWF_MRI_18_COIN_PLANT GSWF(2873)        /* the coin in mri_18's plant item_ueki_04 */
// set by cloud_wind_event when the gust removes S_f2_isk; mri_19 init then enables the plane panel hikou
#define GSWF_MRI_19_PLANE_PANEL_UNCOVERED GSWF(2874)
#define GSWF_MRI_20_POWER_PUNCH_PLANT GSWF(2875) /* the Power Punch in mri_20's plant item_ueki_08 */
// set by meet_rival (mri_01) when Punio joins the Puni group (Puni 1); mri_puni.c tests it for Punio and the total
// count
#define GSWF_MRI_PUNIO_JOINED GSWF(2876)
// set by guide_sister (mri_03) when Petuni joins the Puni group (Puni 2); mri_puni.c tests it for Petuni and the
// total count
#define GSWF_MRI_PETUNI_JOINED GSWF(2877)
// set by chibitoge_nakama (mri_07) when Jabble joins (Puni 3); mri_00/mri_01/mri_03/mri_07 events and mri_puni.c test
// it
#define GSWF_MRI_JABBLE_JOINED GSWF(2878)
// set by rival_nakama (mri_01) when Puniper joins the Puni group (Puni 4); mri_puni.c tests it for Puniper and the
// total count
#define GSWF_MRI_PUNIPER_JOINED GSWF(2879)
// set only by the debug stage jump stg2_23_01_init (event.c); mri_countdown (mri.c) starts the 300-second timer and
// clears it
#define GSWF_MRI_COUNTDOWN_START_PENDING GSWF(2880)
// set by seal_bigtree (mri_00) with STG2_03_01_SEALED_ENTRANCE; no code reads it
#define GSWF_MRI_00_SEALED_ENTRANCE_SEEN_UNUSED GSWF(2881)
// set by evt_ueki_03 (mri_01) after the Puni flees from the plant during STG3_33_05_BOWSER_THE_GREAT_TREE; plays once
#define GSWF_MRI_01_BOWSER_PLANT_SCENE_SEEN GSWF(2883)
// set by out_dokan_1 (mri_00) and bero_05_custom_in (tik_03) when Mario leaves the Puni area at STG2_25 or later;
// mri_00 init runs the send-off (mario_see_you) while it is 0; the mri_00..mri_11 NPC inits use their revisit lines
// once set
#define GSWF_MRI_LEFT_AFTER_BOSS_BATTLE GSWF(2884)
#define GSWF_MRI_16_SHRINK_STOMP_CHEST GSWF(2885) /* the mri_16 chest mobj_box00 holding Shrink Stomp */
#define GSWF_DOU_02_GATE_HANDLE GSWF(2975)       /* the Gate Handle lying in dou_02 (evt_item_entry "handle") */
#define GSWF_DOU_04_GROTTO_KEY GSWF(2977)        /* the Grotto Key lying in dou_04 (evt_item_entry "kagitsume") */
#define GSWF_DOU_05_BIG_DOOR_SWITCH_PRESSED GSWF(2978) /* the dou_05 blue switch "mobj00" that runs bigdoor_event */
// the dou_05 lock "mobj_lock00" opened with the Grotto Key (evt_mobj_lock); dou_05 init switches the e_bero on by it
#define GSWF_DOU_05_DOOR_UNLOCKED GSWF(2979)
// the dou_07 black chest "mobj_box00" (MOBJ_ITEMBOX_BLACK) whose opening event gives the Boat curse
#define GSWF_DOU_07_BOAT_CURSE_CHEST GSWF(2980)
// set by bombwall_bomb (dou_09) when the wall is blown open; bombwall_init shows it open
#define GSWF_DOU_09_BOMB_WALL_BLOWN GSWF(2981)
// set by torinoko_suifu (dou_11) after the stranded sailors shout for help at STG5_29; dou_11 init spawns it while 0
#define GSWF_DOU_11_SAILORS_SHOUT_SEEN GSWF(2982)
// the dou_11 blue switch "mobj01" (ukishima_switch1_evt); with GSWF(2984) it raises the floating platforms (ukishima
// events)
#define GSWF_DOU_11_SWITCH_1_PRESSED GSWF(2983)
// the dou_11 blue switch "mobj02" (ukishima_switch2_evt); with GSWF(2983) it raises the floating platforms (ukishima
// events)
#define GSWF_DOU_11_SWITCH_2_PRESSED GSWF(2984)
#define GSWF_DOU_07_P_DOWN_D_UP GSWF(2985)       /* the P-Down, D-Up lying in dou_07 (evt_item_entry "item_00") */
// the dou_05 chest "mobj_box00" (MOBJ_ITEMBOX_NORMAL): Defend Plus P in the US build, Damage Dodge P in JP
#define GSWF_DOU_05_BADGE_CHEST GSWF(2986)
// the Black Key dropped by the dou_07 Ember (erumosu_dead) and re-entered by dou_07 init
#define GSWF_DOU_07_BLACK_KEY GSWF(2987)
// set by apper_elmos (dou_07) when the Ember appears at the black chest; dou_07 init respawns it while GSWF(2989) is
// 0
#define GSWF_DOU_07_EMBER_APPEARED GSWF(2988)
// set by erumosu_dead (dou_07); dou_07 init stops setting up the Ember and places the Black Key once set
#define GSWF_DOU_07_EMBER_DEFEATED GSWF(2989)
#define GSWF_DOU_00_RUIN_POWDER GSWF(2990)       /* the Ruin Powder lying in dou_00 (evt_item_entry "item_01") */
// the Couple's Cake given as the trouble reward in dou_04 (kuribo_talk, irai_21) and re-entered by dou_04 init
#define GSWF_DOU_04_COUPLES_CAKE GSWF(2991)
// set by ukishima_watari (dou_11) when the stranded sailors cross the platforms; dou_11 init places them across and
// enables fune_atari
#define GSWF_DOU_11_SAILORS_CROSSED GSWF(2992)
// the dou_05 hidden block (evt_mobj_brick MOBJ_BRICK_HIDDEN_ITEM, "mobj_hidden1") holding a coin
#define GSWF_DOU_05_COIN_HIDDEN_BLOCK GSWF(2993)
// the dou_09 hidden ten-coin block (evt_mobj_brick MOBJ_BRICK_HIDDEN_TEN_COIN, "hblk1")
#define GSWF_DOU_09_HIDDEN_TEN_COIN_BLOCK GSWF(2994)
// set by korutesu_talk (gor_00) when Mario accepts Cortez's trip to Keelhaul Key; muj_20 init plays the voyage,
// muj_00 init clears it and runs into_muj_ship
#define GSWF_GOR_SAILING_TO_KEELHAUL GSWF(3125)
// when 1, nakamaware_event (muj_01) skips the crew argument scene and goes straight to kyoshu_event; set only by the
// debug story jump in event.c
#define GSWF_MUJ_01_CREW_SPLIT_SKIP GSWF(3126)
// set by marco_talk (muj_01) at STG5_13 after Flavio orders Mario to save Bobbery; later talks give the short
// reminder
#define GSWF_MUJ_01_FLAVIO_RESCUE_ORDER_HEARD GSWF(3127)
// set by settoku_event_main (muj_01) when the crew notice Bobbery is missing; muj_01 init and marco_talk run the
// scene while 0
#define GSWF_MUJ_01_BOBBERY_MISSING_SCENE_SEEN GSWF(3129)
// set by muj_03_event_01 (entering muj_03 by e_bero from STG5_13) when it spawns nigeru_suifu01, the two sailors
// running off
#define GSWF_MUJ_03_FLEEING_SAILORS_SEEN GSWF(3131)
#define GSWF_MUJ_03_COIN_BLOCK GSWF(3133)        /* the muj_03 ? block (evt_mobj_badgeblk, "mobj01") holding a coin */
// the muj_03 hidden ten-coin block (evt_mobj_brick MOBJ_BRICK_HIDDEN_TEN_COIN, "mobj02")
#define GSWF_MUJ_03_HIDDEN_TEN_COIN_BLOCK GSWF(3134)
// set by muj_05_first when Bobbery groans on the first muj_05 entry at STG5_13; muj_05 init spawns muj_05_first while
// 0
#define GSWF_MUJ_05_BOBBERY_FIRST_SCENE_SEEN GSWF(3136)
// set by erumosu_a_dead (muj_05) when the Embers guarding Bobbery are beaten; muj_05 init and sanders_kyushutsu skip
// the Embers once set
#define GSWF_MUJ_05_EMBERS_DEFEATED GSWF(3137)
// set by sanders_talk (muj_05) at STG5_18 after the first, silent talk with Bobbery; later talks play the snoring
// scene
#define GSWF_MUJ_05_BOBBERY_FIRST_TALK_DONE GSWF(3138)
// set by dokuroiwa_touch (muj_05) when the Skull Gem is put in the skull rock; the rock hammer/ground-pound events,
// dokuroiwa_init and marco_talk test it
#define GSWF_MUJ_05_SKULL_GEM_PLACED GSWF(3139)
// set by dokuroiwa_touch (muj_05) when the skull rock is examined without the gem from STG5_18_02; marco_talk only
// asks Flavio for help once set
#define GSWF_MUJ_05_SKULL_ROCK_EXAMINED GSWF(3140)
// set by marco_talk (muj_05) at STG5_18_03 when Mario first talks to the humming Flavio; marco_init and
// dokuroiwa_bomb stop the humming only while 0
#define GSWF_MUJ_05_FLAVIO_HUMMING_STOPPED GSWF(3141)
// set by marco_talk (muj_05) when Flavio is asked to lend something (the Skull Gem choice); skips his song on later
// talks
#define GSWF_MUJ_05_FLAVIO_FAVOR_ASKED GSWF(3142)
// the Wedding Ring lying in muj_05 (evt_item_entry "item00", from STG5_63)
#define GSWF_MUJ_05_WEDDING_RING GSWF(3143)
// JP only: the muj_03 brick "bblk00" (evt_mobj_brick MOBJ_BRICK_PLAIN); the US build uses LSWF(3) instead
#define GSWF_MUJ_03_BRICK GSWF(3145)
// the muj_02 badge block (evt_mobj_badgeblk kind 1, "mobj_badge_gat") holding Head Rattle
#define GSWF_MUJ_02_HEAD_RATTLE_BADGE_BLOCK GSWF(3146)
// the muj_03 ? block (evt_mobj_badgeblk, "mobj03") holding a Thunder Rage
#define GSWF_MUJ_03_THUNDER_RAGE_BLOCK GSWF(3147)
#define GSWF_MUJ_04_ICE_POWER GSWF(3148)         /* the Ice Power lying in muj_04 (evt_item_entry "item_00") */
// set by answer_event (muj_05) when the skull rock puzzle is solved and the rock opens; dokuroiwa_init shows it open
#define GSWF_MUJ_05_SKULL_ROCK_OPENED GSWF(3149)
// the muj_02 ? block (evt_mobj_badgeblk, "mobj_hatena") holding a Courage Shell
#define GSWF_MUJ_02_COURAGE_SHELL_BLOCK GSWF(3150)
// set by koburon_talk (muj_00) after Whacka's first greeting, and by koburon_dead; later talks pick the line by
// GSW(721)
#define GSWF_MUJ_WHACKA_FIRST_TALK_DONE GSWF(3151)
// the Keel Mango of the muj_02 tree "item_treeb_01" (evt_treeb_01); muj_02 init clears it on every entry, the US init
// sets it before STG5_43
#define GSWF_MUJ_02_KEEL_MANGO_TREE GSWF(3152)
// the muj_03 hidden block (evt_mobj_brick MOBJ_BRICK_HIDDEN_ITEM, "mobj_hidden2") holding a Jammin' Jelly
#define GSWF_MUJ_03_JAMMIN_JELLY_HIDDEN_BLOCK GSWF(3153)
#define GSWF_MUJ_02_COIN_BUSH GSWF(3154)         /* the coin hidden in muj_02's bush item_kusa_2 */
#define GSWF_MUJ_03_MINI_MR_MINI_BUSH GSWF(3155) /* the Mini Mr.Mini hidden in muj_03's bush item_kusa_5 */
#define GSWF_MUJ_03_COIN_BUSH GSWF(3156)         /* the coin hidden in muj_03's bush item_kusa_7 */
#define GSWF_MUJ_05_SPITE_POUCH_BUSH GSWF(3157)  /* the Spite Pouch hidden in muj_05's bush item_kusa_1 */
#define GSWF_MUJ_04_INN_COUPON GSWF(3158)        /* the Inn Coupon lying in muj_04 (evt_item_entry "item_01") */
// set by talk_madam_ring_return (pik_00) when the Gold Ring is given back; talk_madam changes her lines once set
#define GSWF_PIK_MADAM_GOLD_RING_RETURNED GSWF(3275)
// the pik_00 chest "mobj_badge_gat" (MOBJ_ITEMBOX_NORMAL) holding HP Drain P
#define GSWF_PIK_00_HP_DRAIN_P_CHEST GSWF(3276)
#define GSWF_PIK_03_L_EMBLEM GSWF(3277)          /* the L Emblem lying in pik_03 (evt_item_entry "item01") */
// the Choco Cake given as the trouble reward in pik_01 (miyageya_talk, irai_27) and re-entered by pik_01 init
#define GSWF_PIK_01_CHOCO_CAKE GSWF(3278)
#define GSWF_PIK_04_INN_COUPON GSWF(3279)        /* the Inn Coupon lying in pik_04 (evt_item_entry "item_01") */
// set by talk_konari (rsh_04) after Bub's first hint (the present starts with A); later talks repeat the hint
#define GSWF_RSH_BUB_HINT_GIVEN GSWF(3425)
// set by talk_powan (rsh_04) during Bub's request when Pennington guesses the present; no code reads it
#define GSWF_RSH_PENNINGTON_BUB_TALK_UNUSED GSWF(3426)
// set by talk_toron (rsh_01) on the first talk with Zip Toad before STG6_05
#define GSWF_RSH_ZIP_TOAD_FIRST_TALK_DONE GSWF(3427)
// set by talk_powan (rsh_03) after Pennington suspects Mario in the stew case; rsh_03 cook, waitress and businessman
// talks change once set
#define GSWF_RSH_PENNINGTON_ACCUSED_MARIO GSWF(3428)
// set by talk_madam (rsh_01, pik_00) when the lady first asks for help finding her Gold Ring; later asks are shorter
#define GSWF_RSH_MADAM_RING_REQUEST_HEARD GSWF(3429)
// set by talk_madam (rsh_01, pik_00) when Mario agrees to look for her Gold Ring; ends the request prompt
#define GSWF_RSH_MADAM_RING_REQUEST_ACCEPTED GSWF(3430)
// set by talk_waitless (rsh_03) when the waitress first asks for help (from STG6_06); later asks are shorter
#define GSWF_RSH_WAITRESS_REQUEST_HEARD GSWF(3431)
// set by talk_waitless (rsh_03) when Mario agrees to look for her Shell Earrings; ends the request prompt
#define GSWF_RSH_WAITRESS_REQUEST_ACCEPTED GSWF(3432)
// set by evt_door_msg (rsh_04 w_bero) after the first look at the door that will not quite open; skips the camera pan
// later
#define GSWF_RSH_04_STUCK_DOOR_SEEN GSWF(3433)
// set by talk_gurume (rsh_02) when he asks Mario to check the noisy room next door (STG6_13)
#define GSWF_RSH_GOURMET_NOISE_TALK_DONE GSWF(3434)
// set by evt_rsh_door_open on entering heya_4 in rsh_02 once GSWF(3434) is set; talk_gurume then insists someone is
// there
#define GSWF_RSH_NOISY_ROOM_CHECKED GSWF(3435)
// set by talk_syashou (rsh_04) during Bub's request (engineer dream); talk_konari and the partner hint event test it
#define GSWF_RSH_CONDUCTOR_BUB_HINT_HEARD GSWF(3436)
// set by talk_toron (rsh_01) during Bub's request (autograph); evt_rsh_konarikin_irai_party_talk needs it and
// GSWF(3436)
#define GSWF_RSH_ZIP_TOAD_BUB_HINT_HEARD GSWF(3437)
// set by talk_syashou (rsh_04) after the conductor first tells of the missing blanket (STG6_12); later asks are
// shorter
#define GSWF_RSH_CONDUCTOR_BLANKET_TALK_DONE GSWF(3438)
// set by evt_rsh_door_close in rsh_02_c at STG6_15 when the ghost warns Mario not to read its diary; shown once
#define GSWF_RSH_GHOST_DIARY_WARNING_SEEN GSWF(3439)
// set by talk_powan (rsh_04) when Pennington hears of the ghost; talk_sarary (rsh_04) then gives the ghost line
#define GSWF_RSH_PENNINGTON_GHOST_TALK_DONE GSWF(3440)
// set by talk_madam (rsh_01) on the first chat between STG6_27 and STG6_43 (her Poshley Heights question); later
// talks skip the opening
#define GSWF_RSH_MADAM_POSHLEY_TALK_DONE GSWF(3441)
// set by evt_shop_moamoa_talk (rsh_03) when the clerk comes to after the Smorg attack; rsh_03 init and evt_shop.c
// main_evt block the shop until then (STG6_45..47)
#define GSWF_RSH_03_SHOP_CLERK_REVIVED GSWF(3442)
// set by evt_syasyou_auto_talk (rsh_04) after the conductor's diary line; the auto talk runs once
#define GSWF_RSH_CONDUCTOR_DIARY_TALK_DONE GSWF(3443)
// set before an event map change (pik_00 arrival, door close, day ends, Zip Toad catch); the next rsh_02/rsh_04 init
// enters at a fixed spot and clears it
#define GSWF_RSH_EVENT_ENTRY_PENDING GSWF(3444)
// set by init_yuurei (rsh_02) when the ghost appears in heya_4; door events mute the BGM there while set; cleared by
// evt_1day_end/evt_2day_end
#define GSWF_RSH_GHOST_APPEARED GSWF(3445)
// set by evt_rsh_door_open on entering heya_5 in rsh_04, cleared by evt_rsh_door_close; evt_narikin_room runs the
// family talks while set
#define GSWF_RSH_04_IN_ROOM_5 GSWF(3446)
// set by evt_rsh_door_open on entering heya_3 in rsh_04, cleared by evt_rsh_door_close; evt_powan_room runs the room
// talks while set
#define GSWF_RSH_04_IN_ROOM_3 GSWF(3447)
// set by talk_yuurei (rsh_02) while it waits on the ghost's fade loop, cleared after; init_yuurei's loop holds while
// set
#define GSWF_RSH_GHOST_FADE_HOLD GSWF(3448)
// set by init_yuurei's fade loop (rsh_02) while it is held for talk_yuurei, cleared when released; talk_yuurei waits
// for it
#define GSWF_RSH_GHOST_FADE_HELD GSWF(3449)
// set by evt_moamoa_escape0 (rsh_05) when the waitress is freed from the Smorg; evt_moamoa_init keeps her free and
// talkable
#define GSWF_RSH_SMORG_WAITRESS_FREED GSWF(3450)
// set by evt_moamoa_escape1 (rsh_05) when the conductor is freed from the Smorg; evt_moamoa_init keeps him free and
// talkable
#define GSWF_RSH_SMORG_CONDUCTOR_FREED GSWF(3451)
// set by talk_konari (rsh_04) when Bub repeats his hint; with GSWF(3436) the conductor or Bub talk advances to
// STG6_10
#define GSWF_RSH_BUB_HINT_REPEATED GSWF(3452)
// set (B/C cleared) by the revisit init of rsh_01_a/rsh_04_a; rsh_05_a and rsh_08_a pick their a entrance data by it
#define GSWF_RSH_REVISIT_MAP_A GSWF(3453)
// set (A/C cleared) by the revisit init of rsh_01_b/rsh_04_b; rsh_05_a and rsh_08_a pick their b entrance data by it
#define GSWF_RSH_REVISIT_MAP_B GSWF(3454)
// set (A/B cleared) by the revisit init of rsh_01_c/rsh_04_c; rsh_05_a and rsh_08_a pick their c entrance data by it
#define GSWF_RSH_REVISIT_MAP_C GSWF(3455)
// set by evt_train (gor_04, pik_00) on boarding from STG6_56; rsh_02 init clears it and runs evt_rsh_rnd_npc_init
#define GSWF_RSH_NEW_PASSENGERS_PENDING GSWF(3456)
// set by evt_rsh_rnd_npc_4_8_talk when Mario agrees to hear the expert traveler's three secrets; later talks give the
// follow-up
#define GSWF_RSH_TRAVELER_SECRETS_HEARD GSWF(3457)
// set by evt_rsh_rnd_npc_5_3_talk after the passenger's sudden quiz is answered; later talks give the follow-up
#define GSWF_RSH_QUIZ_PASSENGER_DONE GSWF(3458)
// set by evt_rsh_door_open on entering heya_4 in rsh_02, cleared by evt_rsh_door_close; init_yuurei only counts
// toward the ghost appearing while set
#define GSWF_RSH_02_IN_ROOM_4 GSWF(3459)
// set by rsh_02_a init on arrival at Poshley Heights (stg6_rsh_c_04), cleared on boarding; rsh room inits pick the
// Poshley music and state
#define GSWF_RSH_TRAIN_AT_POSHLEY GSWF(3460)
// set by gor_04 init (entry n_bero) and evt_train2 on arrival at Rogueport, cleared on boarding; rsh room inits pick
// the Rogueport music and state
#define GSWF_RSH_TRAIN_AT_ROGUEPORT GSWF(3461)
// the Dried Shroom in the rsh_02 drawer tana_5 (evt_access2); evt_access_entry sets it from STG6_56
#define GSWF_RSH_02_DRIED_SHROOM_DRAWER GSWF(3462)
// set by evt_carry_up_bridge (hom_00) after the attendant explains the raised drawbridge (stg6_hom_04/05); later
// talks use stg6_hom_10
#define GSWF_HOM_DRAWBRIDGE_PROBLEM_EXPLAINED GSWF(3574)
// the hom_00 lock "lock" (evt_mobj_lock) on the station door, set up before STG6_29; evt_lock sets
// STG6_29_STATION_DOOR_UNLOCKED
#define GSWF_HOM_00_STATION_DOOR_LOCK GSWF(3575)
// set by evt_bridge (eki_00 bridge switch) before the hom_00 drawbridge scene; hom_00 init runs evt_bridge_down,
// eki_00 init runs evt_bridge2 which clears it
#define GSWF_EKI_BRIDGE_LOWERING_IN_PROGRESS GSWF(3576)
// set by evt_train and evt_train_nidome (hom_00) on boarding at Riverside Station; no code reads it
#define GSWF_HOM_BOARDED_AT_RIVERSIDE_UNUSED GSWF(3577)
// train direction: 1 = runs toward Rogueport (boarded at pik_00, or arrived at gor_04), 0 = toward Poshley Heights
// (boarded at gor_04, arrived at pik_00); station, train and rsh scenery events test it
#define GSWF_HOM_TRAIN_BOUND_FOR_ROGUEPORT GSWF(3578)
// set by evt_hom_touchaku_nidome (hom_00) on a repeat ride's Riverside stop, cleared on departure; rsh rooms skip the
// moving-train animation, evt_nidome_bed wakes Mario at hom_10
#define GSWF_HOM_TRAIN_STOPPED_AT_RIVERSIDE GSWF(3579)
// set by pik_00 init and evt_pik_00_arrival, cleared when the train leaves; rsh rooms skip the moving-train
// animation, staff talks print the arrival lines
#define GSWF_HOM_TRAIN_AT_POSHLEY GSWF(3580)
// set by gor_04 init (entry n_bero) and evt_train2 on arrival at Rogueport, cleared when the train leaves; rsh rooms
// and staff talks test it
#define GSWF_HOM_TRAIN_AT_ROGUEPORT GSWF(3581)
// set by every boarding event (evt_train in gor_04/pik_00, evt_train/evt_train_nidome in hom_00); no code reads it
#define GSWF_HOM_TRAIN_BOARDED_UNUSED GSWF(3582)
// set by the hom_10_evt_resha_start*/stop_* events while the train moves, cleared when it stops; the wheel, coupling,
// scenery, star and lamp loops run only while set
#define GSWF_HOM_TRAIN_MOVING GSWF(3583)
// eki_00 blue switch bridge_sw (evt_bridge), placed by evt_origami and at STG6_41
#define GSWF_EKI_00_BRIDGE_SWITCH_PRESSED GSWF(3725)
// eki_01 lock (opens with the Station Key, evt_lock sets STG6_32_UNLOCKING_THE_DOOR)
#define GSWF_EKI_01_DOOR_UNLOCKED GSWF(3726)
// eki_01 blue switch origami_sw (evt_origami unfolds the stairs, STG6_30_ORIGAMI_STAIRS)
#define GSWF_EKI_01_ORIGAMI_SWITCH_PRESSED GSWF(3727)
#define GSWF_EKI_05_ULTRA_BOOTS_CHEST GSWF(3728) /* eki_05 big chest tbox holding the Ultra Boots (evt_boots) */
// set by evt_poster_1 when poster piece kami_0 is blown away (eki_03); init hides it when set
#define GSWF_EKI_03_POSTER_1_BLOWN_AWAY GSWF(3729)
// set by evt_poster_2 when kami_17 is blown away (eki_03); init hides it when set
#define GSWF_EKI_03_POSTER_2_BLOWN_AWAY GSWF(3730)
// set by evt_poster_3 when kami_16 is blown away (eki_03); init hides it when set
#define GSWF_EKI_03_POSTER_3_BLOWN_AWAY GSWF(3731)
// set by evt_poster_4 when kami_4 is blown away (eki_03); init hides it when set
#define GSWF_EKI_03_POSTER_4_BLOWN_AWAY GSWF(3732)
// set by evt_poster_5 (kami_5, eki_03); evt_chk needs posters 5..15 to open the wall (STG6_33_BLOW_THE_POSTER)
#define GSWF_EKI_03_POSTER_5_BLOWN_AWAY GSWF(3733)
// set by evt_poster_6 (kami_6, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_6_BLOWN_AWAY GSWF(3734)
// set by evt_poster_7 (kami_7, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_7_BLOWN_AWAY GSWF(3735)
// set by evt_poster_8 (kami_8, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_8_BLOWN_AWAY GSWF(3736)
// set by evt_poster_9 (kami_9, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_9_BLOWN_AWAY GSWF(3737)
// set by evt_poster_10 (kami_10, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_10_BLOWN_AWAY GSWF(3738)
// set by evt_poster_11 (kami_11, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_11_BLOWN_AWAY GSWF(3739)
// set by evt_poster_12 (kami_12, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_12_BLOWN_AWAY GSWF(3740)
// set by evt_poster_13 (kami_13, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_13_BLOWN_AWAY GSWF(3741)
// set by evt_poster_14 (kami_14, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_14_BLOWN_AWAY GSWF(3742)
// set by evt_poster_15 (kami_15, eki_03); one of the eleven evt_chk tests
#define GSWF_EKI_03_POSTER_15_BLOWN_AWAY GSWF(3743)
#define GSWF_EKI_04_DRIED_SHROOM GSWF(3744)      /* the Dried Shroom lying in eki_04 (item01) */
// the Close Call P on eki_00's stand, knocked down by hitting a_dai_sita from below (evt_head)
#define GSWF_EKI_00_CLOSE_CALL_P GSWF(3745)
#define GSWF_EKI_03_HP_PLUS GSWF(3746)           /* the HP Plus lying in eki_03 */
#define GSWF_EKI_04_P_UP_D_DOWN GSWF(3747)       /* the P-Up, D-Down lying in eki_04 (item02) */
// set by evt_kami_1 when kami_1 is blown away (eki_01); init hides it when set
#define GSWF_EKI_01_PAPER_1_BLOWN_AWAY GSWF(3748)
// set by evt_kami_2 when kami_2 is blown away (eki_01); init hides it when set
#define GSWF_EKI_01_PAPER_2_BLOWN_AWAY GSWF(3749)
// set by evt_kami_3 when kami_3 is blown away (eki_01); init hides it when set
#define GSWF_EKI_01_PAPER_3_BLOWN_AWAY GSWF(3750)
// set by evt_kami_4 when kami_4 is blown away (eki_01); init hides it when set
#define GSWF_EKI_01_PAPER_4_BLOWN_AWAY GSWF(3751)
// set by evt_kami_5 when kami_5 is blown away (eki_01); init hides it when set
#define GSWF_EKI_01_PAPER_5_BLOWN_AWAY GSWF(3752)
// set by evt_kami_6 when kami_6 is blown away (eki_01); init hides it when set
#define GSWF_EKI_01_PAPER_6_BLOWN_AWAY GSWF(3753)
// the eki_03 hidden block blk02 (MOBJ_BRICK_HIDDEN_ITEM) holding a Thunder Rage
#define GSWF_EKI_03_THUNDER_RAGE_HIDDEN_BLOCK GSWF(3754)
// set by talk_papa_taihou_kyoka (pik_00) when Goldbob permits the cannon; with GSWF(3876) gives STG7_03;
// bom_01/bom_02 talks and mail events test it
#define GSWF_PIK_GOLDBOB_PERMISSION_GIVEN GSWF(3874)
// set by evt_white_bed (bom_02) when General White wakes and agrees to ready the cannon; with GSWF(3874) gives
// STG7_03; bom and pik_00 talks test it
#define GSWF_BOM_GENERAL_WHITE_AGREED GSWF(3876)
// set by nokonoko_A_talk (nok_00) and boss_noko_talk (nok_01) when asked about General White at STG7_02 (also
// stg7_03_white_1); garawaru_talk (muj_01) and talk_sakaba (tou_01) give their hint only then
#define GSWF_NOK_GENERAL_WHITE_HINT_HEARD GSWF(3877)
// set by garawaru_talk (muj_01) at STG7_02 when the sailor says General White left for an arena (needs GSWF(3877));
// no code reads it
#define GSWF_MUJ_01_WHITE_HINT_HEARD_UNUSED GSWF(3878)
// set by talk_sakaba (tou_01) when the bar owner says General White went to a huge tree (STG7_02); mri_01 puni_1_talk
// then gives the next hint
#define GSWF_TOU_GENERAL_WHITE_HINT_HEARD GSWF(3879)
// set by villagerD_talk (usu_00) at STG7_02 after the mri hint; bom_02 talk_soncho, init_white and init then place
// General White in bed
#define GSWF_USU_GENERAL_WHITE_HINT_HEARD GSWF(3880)
// set by talk_papa_taihou_kyoka (pik_00) on the first cannon talk with Goldbob; talk_mama and talk_konari (pik_00)
// change lines once set
#define GSWF_PIK_GOLDBOB_CANNON_TALK_DONE GSWF(3881)
// set by first_evt/first_evt2 (bom_00) after the partner's Fahr Outpost arrival remark; bom_00 init and
// kurihakase_talk (gor_02) test it
#define GSWF_BOM_FIRST_ARRIVAL_DONE GSWF(3884)
// set by bom_01 init when Fahr Outpost is entered before STG6_71_01; first_evt then gives the partner's other arrival
// line
#define GSWF_BOM_01_EARLY_VISIT GSWF(3885)
// set by talk_soncho (bom_02) on the first talk with the mayor before STG7_02 (he denies having a cannon); later
// talks are shorter
#define GSWF_BOM_MAYOR_FIRST_TALK_DONE GSWF(3886)
// the bom_04 badge block (evt_mobj_badgeblk kind 1, "mobj_badge_gat") holding HP Plus P
#define GSWF_BOM_04_HP_PLUS_P_BADGE_BLOCK GSWF(3887)
// set by urotuku_white (bom_02) after the mayor wonders where General White went; later he gives the short line
#define GSWF_BOM_WHITE_MISSING_SCENE_SEEN GSWF(3888)
// set by puni_1_talk (mri_01) at STG7_02 after the tou hint; usu_00 villagerD_talk then gives the next hint
#define GSWF_MRI_GENERAL_WHITE_HINT_HEARD GSWF(3889)
// the Snow Bunny given as the trouble reward in bom_02 (talk_murabito_e, irai_30) and re-entered by bom_02 init
#define GSWF_BOM_02_SNOW_BUNNY GSWF(3890)
// the bom_00 hidden badge block (evt_mobj_brick MOBJ_BRICK_HIDDEN_BADGE, "mobj_badge_gat2") holding Double Dip P
#define GSWF_BOM_00_DOUBLE_DIP_P_HIDDEN_BLOCK GSWF(3891)
#define GSWF_BOM_02_INN_COUPON GSWF(3892)        /* the Inn Coupon lying in bom_02 (evt_item_entry "item_01") */
// set by rock_evt0 (moo_00) when Bobbery blows up rock S_iwa_01; moo_00 init hides it
#define GSWF_MOO_00_ROCK_01_BLOWN GSWF(4025)
// set by rock_evt1 (moo_00) when Bobbery blows up rock S_iwa_08; moo_00 init hides it
#define GSWF_MOO_00_ROCK_08_BLOWN GSWF(4026)
// set by rock_evt0 (moo_01) when Bobbery blows up rock S_iwa_01; moo_01 init hides it
#define GSWF_MOO_01_ROCK_01_BLOWN GSWF(4027)
// set by rock_evt1 (moo_01) when Bobbery blows up rock S_iwa_08; moo_01 init hides it
#define GSWF_MOO_01_ROCK_08_BLOWN GSWF(4028)
// set by rock_evt0 (moo_02) when Bobbery blows up rock S_iwa_01; moo_02 init hides it
#define GSWF_MOO_02_ROCK_01_BLOWN GSWF(4029)
// set by rock_evt1 (moo_02) when Bobbery blows up rock S_iwa_07; moo_02 init hides it
#define GSWF_MOO_02_ROCK_07_BLOWN GSWF(4030)
// set by rock_evt0 (moo_05) when Bobbery blows up rock S_iwa_07; moo_05 init hides it
#define GSWF_MOO_05_ROCK_07_BLOWN GSWF(4031)
// set by rock_evt0 (moo_06) when Bobbery blows up rock S_iwa_01; moo_06 init hides it
#define GSWF_MOO_06_ROCK_01_BLOWN GSWF(4032)
// set by rock_evt1 (moo_06) when Bobbery blows up rock S_iwa_09; moo_06 init hides it
#define GSWF_MOO_06_ROCK_09_BLOWN GSWF(4033)
// set by rock_evt0 (moo_07) when Bobbery blows up rock S_iwa_07; moo_07 init hides it
#define GSWF_MOO_07_ROCK_07_BLOWN GSWF(4034)
#define GSWF_MOO_00_STOPWATCH GSWF(4035)         /* the Stopwatch lying in moo_00 (evt_item_entry "item_01") */
#define GSWF_MOO_01_VOLT_SHROOM GSWF(4036)       /* the Volt Shroom lying in moo_01 (evt_item_entry "item_01") */
#define GSWF_MOO_02_POWER_PUNCH GSWF(4037)       /* the Power Punch lying in moo_02 (evt_item_entry "item_01") */
#define GSWF_MOO_06_RUIN_POWDER GSWF(4038)       /* the Ruin Powder lying in moo_06 (evt_item_entry "item_01") */
#define GSWF_MOO_07_COURAGE_SHELL GSWF(4039)     /* the Courage Shell lying in moo_07 (evt_item_entry "item_01") */
// set by tensou_ride (aji_07) and moo_04 init before the warp to tik_00; tik_00 tensou_init_event tests and clears it
// to play the arrival
#define GSWF_AJI_TELEPORT_TO_TIK_PENDING GSWF(4175)
// the aji_02 gray chest "box" (MOBJ_ITEMBOX_GRAY, lit-floor room) holding an Elevator Key
#define GSWF_AJI_02_ELEVATOR_KEY_CHEST GSWF(4176)
// the aji_05 gray chest "box" (MOBJ_ITEMBOX_GRAY, lit-floor room) holding a Card Key (ITEM_CARD_KEY_0x1D)
#define GSWF_AJI_05_CARD_KEY_CHEST GSWF(4178)
// the aji_12 gray chest "box" (MOBJ_ITEMBOX_GRAY, lit-floor room) holding a Card Key (ITEM_CARD_KEY_0x1E)
#define GSWF_AJI_12_CARD_KEY_CHEST GSWF(4179)
// the Card Key (ITEM_CARD_KEY_0x1F) lying in aji_11, set by its pickup event key_evt
#define GSWF_AJI_11_CARD_KEY GSWF(4180)
// the Card Key (ITEM_CARD_KEY_0x20) lying in aji_13, set by its pickup event evt_key_get
#define GSWF_AJI_13_CARD_KEY GSWF(4181)
// set by evt_box (aji_05) once the Card Key is taken; aji_05 init skips the lit-floor setup and the floor C function
// stops
#define GSWF_AJI_05_LIT_FLOOR_DONE GSWF(4182)
// set by evt_box (aji_12) once the Card Key is taken; aji_12 init skips the lit-floor setup and thunder_main stops
#define GSWF_AJI_12_LIT_FLOOR_DONE GSWF(4183)
// the aji_13 blue switch "sw" (evt_sw, raises the stairs S_kaidan)
#define GSWF_AJI_13_STAIRS_SWITCH_PRESSED GSWF(4184)
// the aji_13 blue switch "bcv_sw" (evt_bcv_sw, starts the conveyor belt, STG7_17)
#define GSWF_AJI_13_CONVEYOR_SWITCH_PRESSED GSWF(4185)
// set by aji_17 init at STG7_25_00_1 the first time (heals the party, spawns evt_last); later entries spawn evt_last2
#define GSWF_AJI_17_LAST_SCENE_SEEN GSWF(4186)
// aji_03 floor switch A_sui1 (evt_sw0), pressed first in the order 1-3-2; cleared by evt_sw_reinit on a wrong order;
// aji_03 init sets it once GSWF(4197) is set
#define GSWF_AJI_03_SWITCH_1_PRESSED GSWF(4188)
// aji_03 floor switch A_sui2 (evt_sw1), pressed last in the order 1-3-2; cleared by evt_sw_reinit on a wrong order;
// aji_03 init sets it once GSWF(4197) is set
#define GSWF_AJI_03_SWITCH_2_PRESSED GSWF(4189)
// aji_03 floor switch A_sui3 (evt_sw2), pressed second in the order 1-3-2; cleared by evt_sw_reinit on a wrong order;
// aji_03 init sets it once GSWF(4197) is set
#define GSWF_AJI_03_SWITCH_3_PRESSED GSWF(4190)
// set by evt_machine (aji_18) when ITEM_CARD_KEY_0x1D goes into slot 1; all three slots give STG7_16; aji_18 init
// sets it from STG7_16
#define GSWF_AJI_18_CARD_KEY_SLOT_1_FILLED GSWF(4191)
// set by evt_machine (aji_18) when ITEM_CARD_KEY_0x1E goes into slot 2; all three slots give STG7_16; aji_18 init
// sets it from STG7_16
#define GSWF_AJI_18_CARD_KEY_SLOT_2_FILLED GSWF(4192)
// set by evt_machine (aji_18) when ITEM_CARD_KEY_0x1F goes into slot 3; all three slots give STG7_16; aji_18 init
// sets it from STG7_16
#define GSWF_AJI_18_CARD_KEY_SLOT_3_FILLED GSWF(4193)
#define GSWF_AJI_07_COG GSWF(4194)               /* the Cog lying in aji_07, set by its pickup event evt_haguruma */
// set by aji_10 init on every entry just before it is tested, so the 0 branch (e_bero switched off) never runs
#define GSWF_AJI_10_VISITED GSWF(4195)
// set by evt_haguruma (aji_03) when the Cog is fitted into the cabinet; aji_03 init enables the floor switches once
// set
#define GSWF_AJI_03_COG_PLACED GSWF(4196)
// set by evt_sw (aji_03) once switches 1, 3, 2 are pressed in order; aji_03 init keeps the switches on and enables
// the crane console
#define GSWF_AJI_03_SWITCH_PUZZLE_SOLVED GSWF(4197)
// the Feeling Fine crane prize "keihin04" in aji_03, set by its pickup event badge_evt
#define GSWF_AJI_03_FEELING_FINE GSWF(4198)
// set by aji_15 init on the first entry; aji_16 init at STG1_18 spawns peach_evt_stg1_1 once set
#define GSWF_AJI_15_VISITED GSWF(4199)
// set by peach_evt_stg1_1 (aji_01) at STG1_18_01 after Peach remarks nobody is around; runs once
#define GSWF_AJI_01_PEACH_NOBODY_AROUND_SEEN GSWF(4200)
// set by peach_evt_stg1_2 (aji_01) at STG1_18_01 after a door opens by itself for Peach; runs once
#define GSWF_AJI_01_PEACH_DOOR_OPENS_SEEN GSWF(4201)
// set by peach_evt_stg2 (aji_16); at STG2_25 aji_16 init spawns peach_evt_stg2_1 instead once set
#define GSWF_AJI_16_PEACH_STG2_SCENE_SEEN GSWF(4202)
// set by peach_evt_fuku_g (aji_09) when Peach takes the uniform; the changing booth events and the aji_09 C check
// need it
#define GSWF_AJI_09_UNIFORM_TAKEN GSWF(4203)
// toggled by peach_evt_kou_in (aji_09) each time Peach changes in a booth; aji_01/aji_10 init and the aji_09 exits
// keep the disguise by it
#define GSWF_AJI_PEACH_IN_UNIFORM GSWF(4204)
// set by peach_evt_kou_in (aji_09) at STG3_33_01: 1 = Peach changed in booth S_kou1_ka, 0 = the other booth; tested
// to find her dress
#define GSWF_AJI_09_CHANGED_IN_BOOTH_1 GSWF(4205)
// set by peach_evt_kou_out (aji_09) at STG3_33_03 when Peach is back in her dress; the booth events and the aji_09 C
// check test it
#define GSWF_AJI_09_PEACH_DRESS_BACK_ON GSWF(4206)
// set by peach_evt_stg5 (aji_11) after Peach's line on first entering Grodus's room; shown once
#define GSWF_AJI_11_PEACH_ENTRY_LINE_SEEN GSWF(4207)
// set by peach_evt_tana1 (aji_11) when Peach takes the Data Disk from the shelf; peach_evt_disksys, peach_evt_bero
// and peach_evt_stg5 test it
#define GSWF_AJI_11_DATA_DISK_TAKEN GSWF(4208)
// set by peach_evt_disksys (aji_11) after the Data Disk is played; then the disk goes back on the shelf
// (peach_evt_tana2); peach_evt_bero tests it
#define GSWF_AJI_11_DATA_DISK_PLAYED GSWF(4209)
// set by peach_evt_tadashii_kusuri (aji_06) when the right potion is picked; peach_evt_door opens the door once set
#define GSWF_AJI_06_RIGHT_POTION_CHOSEN GSWF(4210)
// the aji_02 ? block (evt_mobj_badgeblk, "mobj00") holding a Super Shroom
#define GSWF_AJI_02_SUPER_SHROOM_BLOCK GSWF(4211)
// the aji_05 ? block (evt_mobj_badgeblk, "mobj00") holding a Sleepy Sheep
#define GSWF_AJI_05_SLEEPY_SHEEP_BLOCK GSWF(4212)
// the aji_12 ? block (evt_mobj_badgeblk, "mobj00") holding an HP Drain (ITEM_HP_DRAIN_ITEM)
#define GSWF_AJI_12_HP_DRAIN_BLOCK GSWF(4214)
#define GSWF_AJI_13_ULTRA_SHROOM GSWF(4215)      /* the Ultra Shroom lying in aji_13 (evt_item_entry "item00") */
// the Feeling Fine P crane prize "keihin06" in aji_03, set by its pickup event badge_evt2
#define GSWF_AJI_03_FEELING_FINE_P GSWF(4216)
// set by evt_kou2 (aji_09) the first time Mario arrives from the ceiling (tenjo) and turns into dot Mario; then the
// booth plays BGM_EVT_DOT_MARIO1
#define GSWF_AJI_09_DOT_MARIO_SEEN GSWF(4217)
// set by evt_ufo_sousaban (aji_03) after the crane instructions are shown once
#define GSWF_AJI_03_CRANE_HELP_SEEN GSWF(4218)
// set by tec_talk (aji_17) after TEC's first speech; later talks give the short line
#define GSWF_AJI_17_TEC_FIRST_TALK_DONE GSWF(4219)
// set by peach_evt_syawa_main (aji_15) after Peach first looks at the shower; later looks use the other line
#define GSWF_AJI_15_PEACH_SHOWER_EXAMINED GSWF(4220)
// set by las_08 door_matigai_evt (wrong door in the corridor); las_07 door_matigai_evt plays SFX_STG8_DOOR_NG_OPEN1
// once and clears it
#define GSWF_LAS_WRONG_DOOR_SFX_PENDING GSWF(4325)
// set by bomb_wanwan1_evt (Bobbery at A_wan1, removes map object wan3); las_09 init hides it when set
#define GSWF_LAS_09_CHAIN_CHOMP_1_BLOWN_UP GSWF(4326)
// set by bomb_wanwan2_evt (Bobbery at A_wan2, removes map object wan1); las_09 init hides it when set
#define GSWF_LAS_09_CHAIN_CHOMP_2_BLOWN_UP GSWF(4327)
// las_09 blue switch switch1 (switch_1_evt turns platform S_daizadai1); switch_2_evt plays the success jingle when
// this is also set; init restores the platform
#define GSWF_LAS_09_SWITCH_1_PRESSED GSWF(4328)
// las_09 blue switch switch2 (switch_2_evt turns platform S_daizadai2, enables Ship_Change1); switch_1_evt plays the
// success jingle when this is also set
#define GSWF_LAS_09_SWITCH_2_PRESSED GSWF(4329)
// set by las_10 first_evt (partner lines stg8_las_31/32); las_10 init runs first_evt while clear
#define GSWF_LAS_10_FIRST_CUTSCENE_SEEN GSWF(4330)
// set by kagi1_evt when a key is set into slot S_kagi1 (las_10); las_10 init (kagi_8_on) shows the slot turned
#define GSWF_LAS_10_KEY_SLOT_1_FILLED GSWF(4331)
// set by kagi2_evt when a key is set into slot S_kagi2 (las_10); las_10 init shows the slot turned
#define GSWF_LAS_10_KEY_SLOT_2_FILLED GSWF(4332)
// set by kagi3_evt when a key is set into slot S_kagi3 (las_10); las_10 init shows the slot turned
#define GSWF_LAS_10_KEY_SLOT_3_FILLED GSWF(4333)
// set by kagi4_evt when a key is set into slot S_kagi4 (las_10); las_10 init shows the slot turned
#define GSWF_LAS_10_KEY_SLOT_4_FILLED GSWF(4334)
// set by kagi5_evt when a key is set into slot S_kagi5 (las_10); las_10 init shows the slot turned
#define GSWF_LAS_10_KEY_SLOT_5_FILLED GSWF(4335)
// set by kagi6_evt when a key is set into slot S_kagi6 (las_10); las_10 init shows the slot turned
#define GSWF_LAS_10_KEY_SLOT_6_FILLED GSWF(4336)
// set by kagi7_evt when a key is set into slot S_kagi7 (las_10); las_10 init shows the slot turned
#define GSWF_LAS_10_KEY_SLOT_7_FILLED GSWF(4337)
// set by kagi8_evt when a key is set into slot S_kagi8 (las_10); las_10 init shows the slot turned
#define GSWF_LAS_10_KEY_SLOT_8_FILLED GSWF(4338)
// big block at (150,50,0): Lv2 stone1 in las_11 and Lv3 metal1 in las_15 share it (hint "two are one")
#define GSWF_LAS_11_15_BIG_BLOCK_1_BROKEN GSWF(4339)
// big block at (-150,0,75): Lv2 stone2 in las_11 and Lv3 metal2 in las_15 share it
#define GSWF_LAS_11_15_BIG_BLOCK_2_BROKEN GSWF(4340)
// las_11 blue float switch; switch_evt drops the Palace Key chest in, init hides the chest while clear
#define GSWF_LAS_11_SWITCH_PRESSED GSWF(4341)
// las_12 blue float switch; switch_evt drops the Palace Key chest in, init hides the chest while clear
#define GSWF_LAS_12_SWITCH_PRESSED GSWF(4342)
// set by switch_check (las_13) when the two red switches were hit in the right counts; init drops the switches and
// shows the chest
#define GSWF_LAS_13_SWITCH_PUZZLE_SOLVED GSWF(4343)
// set by cloud_evt (las_14) when the captured wall (S_kabe1/S_ten) at A_kura is blown away; init shows the chest then
#define GSWF_LAS_14_WALL_BLOWN_AWAY GSWF(4344)
// big block at (150,0,75): Lv3 metal1 in las_11 and Lv2 stone1 in las_15 share it
#define GSWF_LAS_11_15_BIG_BLOCK_3_BROKEN GSWF(4345)
// big block at (-150,50,0): Lv3 metal2 in las_11 and Lv2 stone2 in las_15 share it
#define GSWF_LAS_11_15_BIG_BLOCK_4_BROKEN GSWF(4346)
// las_15 blue float switch; switch_evt drops the Palace Key chest in, init hides the chest while clear
#define GSWF_LAS_15_SWITCH_PRESSED GSWF(4347)
// set by bomb_wall_evt (Bobbery at A_ana_ana, las_16); init shows the hole when set
#define GSWF_LAS_16_WALL_BOMBED GSWF(4348)
// set by enemy4_dead_evt (las_17) when the order puzzle is solved; no code reads it (GSW 1123 = 4 is tested instead)
#define GSWF_LAS_17_ENEMIES_IN_ORDER_UNUSED GSWF(4350)
// las_17 blue float switch that appears after the order puzzle; init hides the chest while clear
#define GSWF_LAS_17_SWITCH_PRESSED GSWF(4351)
// las_18 hidden blue float switch; init runs vivian_check and hides the chest while clear
#define GSWF_LAS_18_SWITCH_PRESSED GSWF(4352)
// toggled by switch1_evt (las_19); las_19 init picks the switch anim, las_21 init the height of float block 19blk1
#define GSWF_LAS_19_SWITCH_1_ON GSWF(4353)
// toggled by switch2_evt (las_19); las_19 init picks the switch anim, las_21 init the height of float block 19blk2
#define GSWF_LAS_19_SWITCH_2_ON GSWF(4354)
// las_22 lock (opens with a Palace Key, unlock_evt sets STG8_10_LOCK)
#define GSWF_LAS_22_DOOR_UNLOCKED GSWF(4355)
// set by cloud_blk_evt (las_24) when S_hako at A_hako is blown away; init then lets the big block blk take hits
#define GSWF_LAS_24_HAKO_BLOWN_AWAY GSWF(4356)
#define GSWF_LAS_24_BIG_BLOCK_BROKEN GSWF(4357)  /* las_24 Lv2 big-big block blk (evt_mobj_lv_blk) */
// las_24 breaking floor brkflr (break_floor_evt sets STG8_12_LARGE_STONE_BLOCK)
#define GSWF_LAS_24_FLOOR_BROKEN GSWF(4358)
// las_25 lock (opens with a Palace Key, unlock_evt sets STG8_14_LOCK); init keeps e_bero_2 shut while clear
#define GSWF_LAS_25_DOOR_UNLOCKED GSWF(4359)
// set by bomb_wall (Bobbery at A_ana_before1, las_26); init shows the hole when set
#define GSWF_LAS_26_WALL_BOMBED GSWF(4360)
// las_26 chest holding the Star Key, dropped by Gloomtail in bonbaba_evt and placed by init after STG8_04_GLOOMTAIL
#define GSWF_LAS_26_STAR_KEY_CHEST GSWF(4361)
#define GSWF_LAS_11_PALACE_KEY_CHEST GSWF(4362)  /* las_11 chest holding a Palace Key (box_evt) */
#define GSWF_LAS_12_PALACE_KEY_CHEST GSWF(4363)  /* las_12 chest holding a Palace Key (box_evt) */
#define GSWF_LAS_13_PALACE_KEY_CHEST GSWF(4364)  /* las_13 chest holding a Palace Key (box_evt) */
#define GSWF_LAS_14_PALACE_KEY_CHEST GSWF(4365)  /* las_14 chest holding a Palace Key (box_evt) */
#define GSWF_LAS_15_PALACE_KEY_CHEST GSWF(4366)  /* las_15 chest holding a Palace Key (box_evt) */
#define GSWF_LAS_16_PALACE_KEY_CHEST GSWF(4367)  /* las_16 chest holding a Palace Key (box_evt) */
#define GSWF_LAS_17_PALACE_KEY_CHEST GSWF(4368)  /* las_17 chest holding a Palace Key (box_evt) */
#define GSWF_LAS_18_PALACE_KEY_CHEST GSWF(4369)  /* las_18 chest holding a Palace Key (box_evt) */
#define GSWF_LAS_24_PALACE_KEY_CHEST GSWF(4370)  /* las_24 chest holding a Palace Key (box_evt) */
// the las_02 ? block (evt_mobj_badgeblk) holding a Shooting Star
#define GSWF_LAS_02_SHOOTING_STAR_BLOCK GSWF(4371)
#define GSWF_LAS_06_ULTRA_SHROOM_BLOCK GSWF(4372) /* the las_06 ? block (evt_mobj_badgeblk) holding an Ultra Shroom */
#define GSWF_LAS_07_COIN_BLOCK GSWF(4373)        /* the las_07 ? block mobj00 holding a coin */
#define GSWF_LAS_07_JAMMIN_JELLY_BLOCK GSWF(4374) /* the las_07 ? block mobj01 holding a Jammin' Jelly */
// the las_03 badge block (evt_mobj_badgeblk kind 1) holding All or Nothing
#define GSWF_LAS_03_ALL_OR_NOTHING_BADGE_BLOCK GSWF(4375)
#define GSWF_LAS_07_P_UP_D_DOWN_P_BADGE_BLOCK GSWF(4376) /* the las_07 badge block (kind 1) holding P-Up, D-Down P */
#define GSWF_LAS_22_SHOOTING_STAR_CHEST GSWF(4377) /* las_22 chest mobj_box00 holding a Shooting Star */
#define GSWF_LAS_22_LIFE_SHROOM_BLOCK GSWF(4378) /* the las_22 ? block holding a Life Shroom */
#define GSWF_LAS_25_LIFE_SHROOM GSWF(4379)       /* the Life Shroom lying in las_25 (item02) */
// set by las_09 init once the story is at STG8_04_GLOOMTAIL or later; while clear, las_26 keeps the beaten Gloomtail
// (GNB_H_3) in the room
#define GSWF_LAS_09_VISITED_AFTER_GLOOMTAIL GSWF(4380)
// set by kabenuke_check_evt (las_12) the first time Mario gets past the wall (x >= 0), with the success jingle
#define GSWF_LAS_12_PASSED_THROUGH_WALL GSWF(4381)
// set by vivian_check (las_18) the first time the hidden switch fades fully in while Vivian hides, with the success
// jingle
#define GSWF_LAS_18_HIDDEN_SWITCH_SHOWN GSWF(4382)
#define GSWF_LAS_01_STOPWATCH GSWF(4383)         /* the Stopwatch lying in las_01 (item_01) */
// the las_04 hidden block (MOBJ_BRICK_HIDDEN_ITEM) holding a Boo's Sheet
#define GSWF_LAS_04_BOOS_SHEET_HIDDEN_BLOCK GSWF(4384)
#define GSWF_LAS_26_ULTRA_SHROOM_BLOCK GSWF(4385) /* the las_26 ? block mobj_01 holding an Ultra Shroom */
#define GSWF_LAS_26_JAMMIN_JELLY_BLOCK GSWF(4386) /* the las_26 ? block mobj_02 holding a Jammin' Jelly */
#define GSWF_LAS_19_THUNDER_RAGE_BLOCK GSWF(4387) /* the las_19 ? block holding a Thunder Rage */
#define GSWF_LAS_20_REPEL_CAPE_BLOCK GSWF(4388)  /* the las_20 ? block mobj_item00 holding a Repel Cape */
#define GSWF_LAS_20_LIFE_SHROOM_BLOCK GSWF(4389) /* the las_20 ? block mobj_item01 holding a Life Shroom */
#define GSWF_LAS_24_COIN_BLOCK GSWF(4390)        /* the las_24 ? block holding a coin */
// the las_24 hidden block (MOBJ_BRICK_HIDDEN_ITEM) holding a Point Swap
#define GSWF_LAS_24_POINT_SWAP_HIDDEN_BLOCK GSWF(4391)
#define GSWF_LAS_25_SHOOTING_STAR GSWF(4392)     /* the Shooting Star lying in las_25 (item03) */
#define GSWF_LAS_30_ULTRA_SHROOM_CHEST GSWF(4393) /* las_30 chest mobj_box01 holding an Ultra Shroom */
#define GSWF_LAS_30_JAMMIN_JELLY_CHEST GSWF(4394) /* las_30 chest mobj_box02 holding a Jammin' Jelly */
// set by hammer_AB_talk_evt (kpa_01) after the Hammer Bros' first conversation (kpa_stg1_006..011); later talks take
// the other branch
#define GSWF_KPA_01_HAMMER_BROS_FIRST_TALK_DONE GSWF(4775)
// Pit of 100 Trials floor 10 big chest (evt_open_box, treasure_tbl: Sleepy Stomp)
#define GSWF_PIT_10_SLEEPY_STOMP_CHEST GSWF(5075)
#define GSWF_PIT_20_FIRE_DRIVE_CHEST GSWF(5076)  /* Pit of 100 Trials floor 20 big chest (Fire Drive) */
#define GSWF_PIT_30_ZAP_TAP_CHEST GSWF(5077)     /* Pit of 100 Trials floor 30 big chest (Zap Tap) */
#define GSWF_PIT_40_PITY_FLOWER_CHEST GSWF(5078) /* Pit of 100 Trials floor 40 big chest (Pity Flower) */
#define GSWF_PIT_50_STRANGE_SACK_CHEST GSWF(5079) /* Pit of 100 Trials floor 50 big chest (Strange Sack) */
#define GSWF_PIT_60_DOUBLE_DIP_CHEST GSWF(5080)  /* Pit of 100 Trials floor 60 big chest (Double Dip) */
#define GSWF_PIT_70_DOUBLE_DIP_P_CHEST GSWF(5081) /* Pit of 100 Trials floor 70 big chest (Double Dip P) */
#define GSWF_PIT_80_BUMP_ATTACK_CHEST GSWF(5082) /* Pit of 100 Trials floor 80 big chest (Bump Attack) */
#define GSWF_PIT_90_LUCKY_DAY_CHEST GSWF(5083)   /* Pit of 100 Trials floor 90 big chest (Lucky Day) */
// Pit of 100 Trials floor 100 big chest (Return Postage), dropped after Bonetail in zonbaba_first_event
#define GSWF_PIT_100_RETURN_POSTAGE_CHEST GSWF(5084)
// set by enemy_setup when the floor-100 enemy is beaten and by zonbaba_first_event (jon); zonbaba_first_event and
// iri_12_makkino_fall_return test it
#define GSWF_PIT_BONETAIL_DEFEATED GSWF(5085)

// low byte of the coins the thief takes in steel_coin (gor_02); thief_talk (gor_01) adds it back to Mario's coins;
// the tou2.c swByteSet/Get(2*id+519+j) hits are a computed index (519+), not this value
#define GSW_GOR_STOLEN_COINS_LOW GSW(2)
// high byte (x256) of the coins the thief takes in steel_coin (gor_02); thief_talk (gor_01) adds it back when Mario
// finds him
#define GSW_GOR_STOLEN_COINS_HIGH GSW(3)
// set by evt_memcard_save from its LW(0): 0 = save block (resume at the saved map and position), 23 = new file's
// first save, other values = scripted save prompts; seq_load evt_continue picks the map to restart in, seq_mapchange
// the position
#define GSW_SAVE_RESUME_POINT GSW(4)
// 8 bytes (GSW 5..12): play-time ticks saved by mail_evt_tou_01 (RDM Issue 2); add_shop_point doubles mri_07 shop
// points for 15 minutes after it
#define GSW_SHOP_MRI_07_SALE_START_TIME GSW(5)
// 8 bytes (GSW 14..21): play-time ticks saved by mail_evt_gor_01_2 (RDM Issue 4); add_shop_point doubles usu_01
// (Twilight Shop) points for 15 minutes after it
#define GSW_SHOP_USU_01_SALE_START_TIME GSW(14)
// 8 bytes (GSW 23..30): play-time ticks saved by mail_evt_pik_00 (RDM Issue 6); add_shop_point doubles rsh_03 shop
// points for 15 minutes after it
#define GSW_SHOP_RSH_03_SALE_START_TIME GSW(23)
// low byte of the Power Bounce record (record = GSW(32) + 256 * GSW(35), max 9999), raised by
// marioAttackEvent_RenzokuJump (battle_mario); tik_06's sign prints it
#define GSW_MAIN_POWER_BOUNCE_RECORD_LO GSW(32)
// deepest Pit of 100 Trials level reached, raised by jon_init_evt (jon); tik_06's sign (evt_kanban2) prints it
#define GSW_JON_DEEPEST_LEVEL GSW(33)
// shown by tik_06's sign (evt_kanban2) as Star Pieces found out of 100; nothing in the code writes it
#define GSW_TIK_STAR_PIECES_FOUND_COUNT GSW(34)
// high byte of the Power Bounce record (see GSW(32)), raised by marioAttackEvent_RenzokuJump (battle_mario); tik_06's
// sign prints it
#define GSW_MAIN_POWER_BOUNCE_RECORD_HI GSW(35)
// 8-byte play-time stamp (GSW 100..107) saved by epigraphy_map_after_stage6 (gor_02); kurihakase_talk loads the
// elapsed ms at STG6_71 to pick a wait line or run kurihakase_after3minutes after 180000 ms
#define GSW_GOR_FRANKLY_STG6_START_TIME GSW(100)
// 8-byte play-time stamp (GSW 108..115) saved by shoptender_talk (gor_01) when the lens is ordered; gor_01 init sets
// GSWF(1191) once 90000 ms have passed
#define GSW_GOR_CONTACT_LENS_ORDER_TIME GSW(108)
// 100-coin investments given to Lumpy in mokorim_talk (gor_00), 0..3 (3 = he takes no more); from STG6_56 he repays 1
// = 300, 2 = 600 (JP 500), 3 = 999 coins
#define GSW_GOR_LUMPY_INVESTMENT_COUNT GSW(116)
// bteresa_talk (gor_03): 0..5, raised by one per talk while not above the count of minigame top-score flags
// GSWF(1240..1243); picks bunny_teresa_11 .. _11_4
#define GSW_GOR_PARLOR_ATTENDANT_TALK_STAGE GSW(117)
// 4 bytes (GSW 118..121): indices into badge_bottakuru_table for Charlieton's four offers, rolled by
// badgeShop_bottakuruGeneration; a sold one is set to the last (empty) entry; read by gor_01 talk and winmgr
#define GSW_BADGESHOP_CHARLIETON_STOCK GSW(118)
// hammer hits on hei_13's a_kan_5 (evt_anm_kan); at 10 a Horsetail pops out; hei_13 init resets it to 0
#define GSW_HEI_13_HAMMER_HIT_COUNT GSW(301)
// first parameter of the match condition (NpcBattleInfo battleRule[1], printed in the rule message); set by
// evt_tou_match_make_default_sub (tou.c) from evt_tou_get_rule_info, 1 in tou_10 evt_match_make_sub and the
// stg3_06/stg3_21 story-jump inits (event.c); evt_match (tou_03) passes it to evt_npc_set_battle_rule
#define GSW_GLITZ_PIT_CONDITION_PARAM GSW(503)
// second parameter of the match condition (battleRule[2], which the battle code never reads); set and passed like
// GSW(503)
#define GSW_GLITZ_PIT_CONDITION_PARAM2 GSW(504)
// losses to the Armored Harriers (fighter 10), added by evt_match (tou_03) up to 100; tou_10 init runs the
// first_tamago partner pep talk once when it is 2 or more
#define GSW_TOU_ARMORED_HARRIERS_LOSS_COUNT GSW(505)
// Mario's rank when the match starts (tou_03 init, from evt_tou_get_ranking); evt_tou_ranking_msg and
// evt_tou_match_after_default (tou.c) compare the new rank with it (moved up/down, demotion/promotion lines)
#define GSW_GLITZ_PIT_RANK_BEFORE_MATCH GSW(506)
// fighter id of the last match's opponent, saved by evt_tou_set_ranking_win_lose (tou2.c/tou.c) and the story-jump
// inits; rankingControll keeps that fighter next to Mario, evt_tou_ranking_msg reads its rank
#define GSW_GLITZ_PIT_LAST_OPPONENT GSW(507)
// 8 bytes (GSW 508-515): play time when the egg joins, saved by talk_tamago (tou_01) and stg3_12_init__4 (event.c);
// setYoshiColor (tou_10) picks Yoshi's color from the minutes since
#define GSW_TOU_EGG_RECEIVED_TIME GSW(508)
// ordinary Punies (index 5 on) in the group: 0 at start, 9 set by rival_nakama (mri_01), 98 by guide_sister (mri_03);
// mri_puni.c uses it for which Punies exist and the total count
#define GSW_MRI_PUNI_FOLLOWER_COUNT GSW(651)
// Great Tree room number + 1, written by evt_puni_set_yuka (mri_00 = 1 .. mri_20 = 21); evt_puni_setup stops early at
// 1, mri_01 init tests 2
#define GSW_MRI_PUNI_ROOM GSW(665)
// no code sets it (always 0); mri_01 init would start puni_mario_wait (all Punies placed around Mario) when it is 1
// and GSW(665) is 2
#define GSW_MRI_01_PUNI_WAIT_UNUSED GSW(666)
// times Whacka (muj_00) has been hit: koburon_dead adds 1 per hit; koburon_talk picks his line by it (0-7); at 7 he
// gives his last line; at 8 koburon_evt deletes him
#define GSW_MUJ_WHACKA_HIT_COUNT GSW(721)
// packed random passenger picks for rsh_01 (bits 0-2 group 0, 3-4 group 1, bit 5 group 2); cleared and filled by
// evt_rsh_rnd_npc_init, read by evt_rsh_get_rnd_npc
#define GSW_RSH_01_RANDOM_NPCS GSW(821)
// packed random passenger picks for rsh_03 (bits 0-3 group 0, 4-6 group 1); cleared and filled by
// evt_rsh_rnd_npc_init, read by evt_rsh_get_rnd_npc
#define GSW_RSH_03_RANDOM_NPCS GSW(822)
// packed random passenger picks for rsh_04 (bits 0, 1, 2 groups 0-2, bits 3-4 group 3); cleared and filled by
// evt_rsh_rnd_npc_init, read by evt_rsh_get_rnd_npc
#define GSW_RSH_04_RANDOM_NPCS GSW(823)
// set when a train leg starts: 0 = gor_04 evt_train (from Rogueport), 1/3 = hom_00 evt_train_nidome or rsh_02
// evt_nidome_bed (from Riverside, toward Poshley/Rogueport), 2 = pik_00 evt_train (from Poshley); the journal map
// (win_log) reads it for Mario's place on the train
#define GSW_HOM_TRAIN_LEG GSW(870)
// tries at the locked aji_01 doors/elevators at STG7_25 (last_key_lock, last_elv_lock): counts 0-3; at 3 the partner
// comments, then it stops counting
#define GSW_AJI_01_LOCKED_EXIT_TRIES GSW(1071)
// las_08 endless corridor (story STG8_01_02_LOCK): door_seikai_evt adds 1 per correct door, 7 sets
// STG8_02_FOLLOW_THE_LIT_TORCHES; door_matigai_evt resets it to 0; mugen_kairou_init picks the lit door from it
#define GSW_LAS_08_CORRECT_DOOR_COUNT GSW(1121)
// number of keys set into las_10's eight slots (kagi1_evt..kagi8_evt add 1); at 8 the slot event runs tenkyugi_evt
#define GSW_LAS_10_PALACE_KEYS_SET GSW(1122)
// las_17 defeat-order puzzle: 0 = none, 1..3 = Goomba, Spiky Goomba, Koopa Troopa beaten in that order, 4 = Bald
// Cleft last (switch appears); las_17 init resets values below 4 to 0
#define GSW_LAS_17_ENEMY_ORDER_STATE GSW(1123)
// set by peach_change_evt (unit_boss_black_peach) to the Shadow Queen's HP after the change; no code reads it
#define GSW_LAS_SHADOW_QUEEN_HP_UNUSED GSW(1124)
// Bowser side-scroller (kpa): set to 1 by kpa_init on a fresh start, kpa_dead adds 1 per death (max 99); shown on the
// status bar and picks the kpa_start_NNN line
#define GSW_KPA_TRY_COUNT GSW(1221)
// Bowser side-scroller time left in seconds: 180 at kpa_init, written by kpa_set_time each tick; kpa_bgm_change
// switches music at 30 or less
#define GSW_KPA_TIME_LEFT GSW(1222)
// Bowser's power level carried between kpa rooms: saved by jumpstand_evt (kpa_03/kpa_05, evt_koopa_get_level),
// restored by kpa_04/kpa_06 init (evt_koopa_set_level)
#define GSW_KPA_BOWSER_LEVEL GSW(1223)
// 8 bytes 1421..1428: play time saved by evt_sub_save_playtime when Try to Find Me (Koopook, gon_08) is completed; no
// code reads it
#define GSW_GON_TRY_TO_FIND_ME_DONE_TIME_UNUSED GSW(1421)
// 8 bytes 1429..1436: play time saved by evt_sub_save_playtime when Help My Daddy (Pine T. Jr., tik_06) is completed;
// no code reads it
#define GSW_TIK_HELP_MY_DADDY_DONE_TIME_UNUSED GSW(1429)
// 8 bytes (GSW 1437-1444): play time saved by talk_kinoshikowa (tou_05) when trouble Help Wanted is finished;
// irai_mail_check (evt_sub.c) sends mail 0x27 two hours later
#define GSW_TOU_HELP_WANTED_DONE_TIME GSW(1437)
// 8 bytes 1445..1452: play time saved by evt_sub_save_playtime when The Food I Want (Bub-ulber, nok_00) is completed;
// no code reads it
#define GSW_NOK_FOOD_I_WANT_DONE_TIME_UNUSED GSW(1445)
// 8 bytes (1453..1460): evt_sub_save_playtime in aunt_talk (usu_01) when trouble 20 is solved; irai_mail_check sends
// mail 0x29 two hours later
#define GSW_USU_TROUBLE_20_DONE_TIME GSW(1453)
// first of 8 bytes (1461-1468) where evt_sub_save_playtime stores the play time when the I Can't Speak trouble ends
// (bom_02 talk_murabito_h); no code reads it
#define GSW_BOM_I_CANT_SPEAK_TIME_UNUSED GSW(1461)
