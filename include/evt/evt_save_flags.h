#pragma once

/* Saved flags (GSWF) and values (GSW) named from what the scripts do with them and the room's message
 * text; the game carries no names for these. Only values the code uses are listed. */

// set by buy_evt when a 300-point shop prize could not be given (item bag full); shopper_talk_evt hands it out
// (shop_point_item 300) and clears it
#define GSWF_SHOP_POINT_PRIZE_PENDING GSWF(1)
// set by johoyaketi_talk (tik_00) after Wonky's introduction; the same talk tests it
#define GSWF_TIK_00_WONKY_FIRST_TALK_DONE GSWF(3)
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
// read only by mail_evt: while clear, mail_first_hit is printed again on later mails; no code sets it
#define GSWF_MAIL_TUTORIAL_REPEAT_OFF GSWF(38)
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
#define GSWF_TIK_01_VISITED_UNUSED GSWF(254)     /* set by tik_01 init on every entry; no code reads it */
#define GSWF_GON_00_VISITED_UNUSED GSWF(255)     /* set by gon_00 init on every entry; no code reads it */
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
// gonbaba_event (gon_11) tests it to jump past the Hooktail intro and battle; nothing sets it
#define GSWF_GON_11_HOOKTAIL_INTRO_SKIP_UNUSED GSWF(1498)
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
// set by chuchurina_sogu (gon_12) when Koops is the partner at the end of the Ms. Mowz encounter; no code reads it
#define GSWF_GON_12_MOWZ_MET_WITH_KOOPS_UNUSED GSWF(1511)
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
// set by sister_talk (mri_01) when Petuni's guessing game is won (Mystic Egg); sister_talk tests it; cleared on
// leaving through mri_00 out_dokan_1 / tik_03 bero_05_custom_in after STG2_25
#define GSWF_MRI_PETUNI_GAME_WON_THIS_VISIT GSWF(2827)
// set by chibitoge_talk (mri_04) when the baby Jabbi gives a Mushroom; chibitoge_init tests it; cleared on leaving
// through mri_00 out_dokan_1 / tik_03 bero_05_custom_in after STG2_25
#define GSWF_MRI_04_BABY_JABBI_GIFT_THIS_VISIT GSWF(2840)
// set by out_dokan_1 (mri_00) and bero_05_custom_in (tik_03) when Mario leaves the Puni area at STG2_25 or later;
// mri_00 init runs the send-off (mario_see_you) while it is 0; the mri_00..mri_11 NPC inits use their revisit lines
// once set
#define GSWF_MRI_LEFT_AFTER_BOSS_BATTLE GSWF(2884)
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
// set by tensou_ride (aji_07) and moo_04 init before the warp to tik_00; tik_00 tensou_init_event tests and clears it
// to play the arrival
#define GSWF_AJI_TELEPORT_TO_TIK_PENDING GSWF(4175)
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
// 4 bytes (GSW 118..121): indices into badge_bottakuru_table for Charlieton's four offers, rolled by
// badgeShop_bottakuruGeneration; a sold one is set to the last (empty) entry; read by gor_01 talk and winmgr
#define GSW_BADGESHOP_CHARLIETON_STOCK GSW(118)
// hammer hits on hei_13's a_kan_5 (evt_anm_kan); at 10 a Horsetail pops out; hei_13 init resets it to 0
#define GSW_HEI_13_HAMMER_HIT_COUNT GSW(301)
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
// 8 bytes 1445..1452: play time saved by evt_sub_save_playtime when The Food I Want (Bub-ulber, nok_00) is completed;
// no code reads it
#define GSW_NOK_FOOD_I_WANT_DONE_TIME_UNUSED GSW(1445)
