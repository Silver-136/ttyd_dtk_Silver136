/* "event" - story event flags
 * Status: Almost complete, need to stub/implement commented out functions,
 * theoretically 1:1
 *
 * Function: all the data for setting story flags based on progress
 */
#include "event.h"
#include "driver/swdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "driver/msgdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "evt/evt_badgeshop.h"
#include "evt/evt_yuugijou.h"
#include "evt/evt_johoya.h"
#include "statuswindow.h"
//TODO: put in /evt?
//funcs sorted backwards aka original source file

s32 yoshiSetColor(void);

#ifdef VERSION_G8ME01
#define NAME_EN(x) , x
#else
#define NAME_EN(x)
#endif

EventData stage0_evt[] = {
    {0, 0, 0, 0, "STG0_00", "プロローグ" NAME_EN("Prologue"), "aaa_00", "prologue", stg0_00_init},
    {0, 0, 0, 1, "STG0_01", "広場" NAME_EN("Plaza"), "aaa_00", "prologue", NULL},
    {0, 0, 0, 2, "STG0_02", "マリオの家" NAME_EN("Mario's House"), "aaa_00", "prologue", NULL},
    {0, 0, 0, 3, "STG0_03", "ゴロツキタウンへ" NAME_EN("To Rogueport"), "sys_01", "prologue", NULL},
    {0, 0, 0, 3, "STG0_03", "ゴロツキタウン到着" NAME_EN("Rogueport (Arrival)"), "gor_00", "", NULL},
    {0, 0, 0, 4, "STG0_04", "クリスチーヌ襲われる" NAME_EN("Goombella Assaulted"), "gor_00", NULL, NULL},
    {0, 0, 0, 5, "STG0_05", "軍団１０００体バトル前" NAME_EN("1000 X-Nauts (Before)"), "gor_00", NULL, stg0_05_init},
    {0, 0, 0, 6, "STG0_06", "軍団１０００体バトル後" NAME_EN("1000 X-Nauts (After)"), "gor_00", NULL, NULL},
    {0, 0, 13, 7, "STG0_07", "クリスチーヌ仲間になる" NAME_EN("Goombella Joins"), "gor_01", "s_bero", NULL},
    {0, 1, 0, 8, "STG0_08", "コインが盗まれる" NAME_EN("Stolen Coins"), "gor_02", "w_bero", stg0_08_init},
    {0, 1, 0, 9, "STG0_09", "クリハカセとの会話" NAME_EN("Prof. Frankly"), "gor_02", "w_bero", NULL},
    {0, 1, 11, 10, "STG0_10", "柵をはずす" NAME_EN("Removing the Fence"), "gor_02", "w_bero", NULL},
    {0, 1, 0, 11, "STG0_10_01", "クリハカセのレクチャー" NAME_EN("Frankly's Tutorial"), "gor_02", "w_bero", NULL},
    {0, 1, 11, 12, "STG0_11", "ゴロツキ三人組" NAME_EN("3 Goomba Goons"), "tik_01", "dokan_2", stg0_11_init},
    {0, 1, 11, 13, "STG0_12", "プニとの遭遇" NAME_EN("Puni Encounter"), "tik_04", "dokan", NULL},
    {0, 1, 11, 14, "STG0_12_01", "スイッチペラ階段" NAME_EN("Paper Stair Switch"), "tik_04", "dokan", NULL},
    {0, 1, 11, 15, "STG0_12_02", "ペラ魔人１" NAME_EN("Paper Mage 1"), "tik_19", "s_bero", stg0_12_02_init},
    {0, 1, 11, 16, "STG0_13", "はじめての飛行" NAME_EN("First Flight"), "tik_04", "n_bero_5", stg0_13_init00},
    {0, 1, 11, 16, "STG0_13", "大きな扉" NAME_EN("Thousand-Year Door"), "tik_05", "e_bero", NULL},
    {0, 1, 11, 17, "STG0_14", "魔方陣の前" NAME_EN("Front of the Pedestal"), "tik_05", "e_bero", NULL},
    {0, 1, 11, 18, "STG0_15", "魔方陣" NAME_EN("The Pedestal"), "tik_05", "e_bero", NULL},
    {0, 1, 0, 19, "STG0_16", "地図解読" NAME_EN("Frankly's House"), "gor_02", NULL, stg0_16_init},
    {0, 1, 0, 20, "STG0_17", "レクチャー（バッジ編）" NAME_EN("Badge Tutorial"), "gor_02", "w_bero", NULL},
    {0, 1, 0, 21, "STG0_19", "ゲッソーとバトル" NAME_EN("Blooper Battle"), "tik_02", "w_bero_1", NULL},
    {0, 1, 0, 22, "STG0_END", "ステージ１へ" NAME_EN("To Stage 1"), "tik_02", "w_bero_1", NULL}
};

EventData stage1_evt[] = {
    {0, 1, 0, 23, "STG1_00", "はじめてきた" NAME_EN("Petal Meadows"), "sys_01", "stg_1", NULL},
    {0, 1, 0, 24, "STG1_00_01", "ごんばばの影" NAME_EN("The Shadow of Hooktail"), "hei_00", "dokan_2", NULL},
    {0, 1, 0, 25, "STG1_01", "スイッチで橋が出る" NAME_EN("The Flip-Bridge"), "hei_01", "w_bero", NULL},
    {0, 1, 0, 26, "STG1_01a", "初めてのノコ村" NAME_EN("Petalburg (1st Time)"), "nok_00", "w_bero", stg1_01a_init},
    {0, 1, 0, 27, "STG1_02", "ノコ村村長と会話" NAME_EN("Talking with Kroop"), "nok_01", "w_bero", NULL},
    {0, 1, 0, 28, "STG1_03", "ノコ村門番" NAME_EN("Petalburg Gatekeeper"), "nok_01", "w_bero", stg1_03_init},
    {0, 1, 0, 29, "STG1_04", "野原しんのすけ" NAME_EN("Bald Cleft"), "hei_03", "w_bero", NULL},
    {0, 1, 0, 30, "STG1_05", "トゲダルマー" NAME_EN("Bristle"), "hei_05", "w_bero", NULL},
    {0, 1, 0, 31, "STG1_06", "クイズ" NAME_EN("Quiz"), "hei_07", "w_bero", NULL},
    {0, 1, 0, 32, "STG1_06_01", "ゴールドチョロボン" NAME_EN("Gold Fuzzy"), "hei_10", "w_bero", stg1_06_01_init},
    {0, 1, 0, 33, "STG1_07", "ノコタロウ仲間" NAME_EN("Koops Joins"), "hei_01", "e_bero", NULL},
    {0, 2, 0, 34, "STG1_08", "ゴンババ入り口開く" NAME_EN("Opening the Path"), "hei_00", "e_bero", stg1_08_init},
    {0, 2, 0, 35, "STG1_09", "初めてのゴンババ城" NAME_EN("Hooktail Castle"), "gon_01", "w_bero_1", stg1_09_init},
    {0, 2, 0, 36, "STG1_09_01", "カギ" NAME_EN("Key"), "gon_01", "w_bero_1", NULL},
    {0, 2, 0, 37, "STG1_09_02", "錠前" NAME_EN("Lock"), "gon_01", "w_bero_1", stg1_09_02_init},
    {0, 2, 0, 38, "STG1_10", "父の手紙" NAME_EN("Father's Letter"), "gon_03", "w_bero", stg1_10_init},
    {0, 2, 0, 39, "STG1_11", "カロン１０００体" NAME_EN("1000 Dry Bones"), "gon_03", "w_bero", NULL},
    {0, 2, 0, 40, "STG1_12", "ペラ階段" NAME_EN("Paper Stairs"), "gon_04", "w_bero_1", NULL},
    {0, 2, 0, 41, "STG1_12_01", "かぎ" NAME_EN("Key"), "gon_04", "w_bero_1", NULL},
    {0, 2, 0, 42, "STG1_12_02", "錠前" NAME_EN("Lock"), "gon_04", "w_bero_1", stg1_12_02_init},
    {0, 2, 0, 43, "STG1_13", "つり天井" NAME_EN("A Suspicious Room"), "gon_07", "w_bero", stg1_13_init},
    {0, 2, 0, 44, "STG1_13_01", "つり天井から脱出" NAME_EN("50 Seconds to Escape"), "gon_06", "e_bero", stg1_13_01_init},
    {0, 2, 0, 45, "STG1_13_01_01", "ペラ魔人２" NAME_EN("Paper Mage 2"), "gon_06", "e_bero", NULL},
    {0, 2, 0, 46, "STG1_13_02", "かぎ２" NAME_EN("Key 2"), "gon_03", "e_bero", stg1_13_02_init},
    {0, 2, 0, 47, "STG1_13_03", "錠前２" NAME_EN("Lock 2"), "gon_05", "w_bero_1", stg1_13_03_init},
    {0, 2, 0, 48, "STG1_14", "ペラ橋（右）" NAME_EN("Paper Bridge (Right)"), "gon_04", "e_bero_3", stg1_14_init},
    {0, 2, 0, 49, "STG1_14_01", "ペラ橋（左）" NAME_EN("Paper Bridge (Left)"), "gon_04", "e_bero_3", NULL},
    {0, 1, 0, 50, "STG1_15", "チュチュリーナ　クリ編" NAME_EN("Ms. Mowz (w/Goombella)"), "gon_12", "e_bero", NULL},
    {0, 2, 0, 50, "STG1_15", "チュチュリーナ　ノコ編" NAME_EN("Ms. Mowz (w/Koops)"), "gon_12", "e_bero", NULL},
    {0, 2, 0, 51, "STG1_15_01", "かぎ３" NAME_EN("Key 3"), "gon_12", "e_bero", NULL},
    {0, 2, 0, 52, "STG1_15_02", "錠前３" NAME_EN("Lock 3"), "gon_08", "w_bero", stg1_15_02_init},
    {0, 2, 0, 53, "STG1_15_03", "かぎ４" NAME_EN("Key 4"), "gon_05", "w_bero_4", stg1_15_03_init},
    {0, 2, 0, 54, "STG1_15_04", "錠前４" NAME_EN("Lock 4"), "gon_05", "w_bero_4", stg1_15_04_init},
    {0, 2, 0, 55, "STG1_16", "ゴンババ遭遇" NAME_EN("Hooktail"), "gon_11", "w_bero", stg1_16_init00},
    {0, 2, 0, 55, "STG1_16", "ゴンババ倒れる" NAME_EN("Hooktail Defeated"), "gon_11", NULL, stg1_16_init01},
    {0, 2, 0, 56, "STG1_17", "父との再会" NAME_EN("Father & Son Reunited"), "gon_11", "w_bero", stg1_17_init},
    {0, 0, 0, 57, "STG1_18", "ピーチ編：つかまったピーチ姫" NAME_EN("Peach: Caught"), "aji_10", NULL, NULL},
    {0, 0, 0, 58, "STG1_18_01", "ピーチ編：扉が開いた" NAME_EN("Peach: An Open Door"), "aji_16", NULL, NULL},
    {0, 0, 0, 59, "STG1_19", "ピーチ編：テックと出会う" NAME_EN("Peach: TEC"), "aji_17", "w_bero", stg1_19_init},
    {0, 0, 0, 60, "STG1_19_01", "クッパ編：クッパ城" NAME_EN("Bowser: Bowser Castle"), "kpa_00", "", NULL},
    {0, 0, 0, 60, "STG1_19_01", "クッパ編：クッパ登場" NAME_EN("Bowser: Entrance"), "kpa_01", "", NULL},
    {0, 0, 0, 61, "STG1_19_02", "クッパ編：クッパ城城内" NAME_EN("Bowser: Castle INT."), "kpa_01", "player", NULL},
    {0, 0, 0, 61, "STG1_19_02", "クッパ編：クッパ出発" NAME_EN("Bowser: Kammy Enters"), "kpa_01", "event", NULL},
    {0, 0, 0, 61, "STG1_19_02", "クッパ編：クッパ飛び立つ" NAME_EN("Bowser: Departure"), "kpa_00", "", NULL},
    {0, 2, 0, 62, "STG1_20", "ノコタロウ旅立ち" NAME_EN("Koops Leaves"), "nok_01", "nokotarou", stg1_20_init},
    {0, 2, 0, 63, "STG1_21", "ピーチからのメール" NAME_EN("A Mail from Peach"), "hei_01", "e_bero", NULL},
    {0, 1, 0, 64, "STG1_30", "ナンシーさん" NAME_EN("Zess T."), "gor_01", "e_bero", stg1_30_init},
    {0, 1, 0, 64, "STG1_30", "大きな扉ステージ１後" NAME_EN("Thousand-Year Door"), "tik_05", "e_bero", stg1_30_init01},
    {0, 1, 0, 65, "STG1_31", "地図解読ステージ１後" NAME_EN("Frankly's House Stg.1"), "gor_02", NULL, NULL},
    {0, 1, 0, 66, "STG1_32", "再度プニとの遭遇" NAME_EN("Puni Encounter 2"), "tik_04", "dokan", NULL},
    {0, 1, 0, 67, "STG1_33", "プニ助けを請う" NAME_EN("A Puni Plea for Help"), "tik_03", "s_bero_3", NULL},
    {0, 1, 0, 68, "STG1_34", "プニ壁開ける" NAME_EN("Puni's Secret Passage"), "tik_03", "s_bero_3", NULL},
    {0, 1, 0, 69, "STG1_35", "プニ仲間になる" NAME_EN("Puni Joins"), "tik_03", "s_bero_3", stg1_35_init01},
    {0, 1, 10, 70, "STG1_END", "ステージ２へ" NAME_EN("To Stage 2"), "tik_03", "a_slit", NULL}
};

EventData stage2_evt[] = {
    {0, 2, 10, 71, "STG2_00", "不思議の森へご案内" NAME_EN("Boggly Woods"), "sys_01", "stg_2", stg2_00_init},
    {0, 2, 10, 72, "STG2_01", "魔女三人組もめる" NAME_EN("A Siren Argument"), "win_00", "w_bero", NULL},
    {0, 2, 10, 73, "STG2_02", "ガイドが大樹へ" NAME_EN("To The Great Tree"), "win_01", "w_bero", NULL},
    {0, 2, 0, 74, "STG2_03", "初めての大樹" NAME_EN("The Great Tree"), "mri_00", "w_bero", NULL},
    {0, 2, 0, 75, "STG2_03_01", "大樹をふさぐもの" NAME_EN("Sealed Entrance"), "mri_00", "w_bero", NULL},
    {0, 2, 0, 76, "STG2_03_02", "大樹をふさぐものその２" NAME_EN("Sealed Entrance 2"), "mri_00", "w_bero", NULL},
    {0, 2, 10, 77, "STG2_03_03", "クラウダ登場" NAME_EN("Flurrie Intro"), "win_03", "", stg2_03_03_init},
    {0, 2, 10, 78, "STG2_03_04", "だいじなものをなくす" NAME_EN("She's Lost Her Precious"), "win_03", "", NULL},
    {0, 2, 10, 79, "STG2_03_05", "ヒミツの入り口を知った後" NAME_EN("To Flurrie's Place"), "win_01", "n_bero", NULL},
    {0, 2, 10, 80, "STG2_04", "のびる飛行機台" NAME_EN("Extending Plane Panel"), "win_02", "w_bero", NULL},
    {0, 2, 10, 80, "STG2_04", "ケモノ道" NAME_EN("The Trail"), "win_03", "w_bero", NULL},
    {0, 2, 10, 81, "STG2_05", "クラウダに会う" NAME_EN("A Shy Flurrie"), "win_04", "w_bero", NULL},
    {0, 2, 10, 82, "STG2_05_01", "クラウダに会う：踊り場" NAME_EN("Flurrie: Living Room"), "win_04", "w_bero", NULL},
    {0, 2, 10, 83, "STG2_05_02", "クラウダに会う：依頼" NAME_EN("Flurrie: Request"), "win_04", "w_bero", NULL},
    {0, 2, 10, 84, "STG2_06", "魔女三人組似顔絵発見" NAME_EN("The Sketch"), "win_00", "", NULL},
    {0, 2, 10, 85, "STG2_07", "マリオＶＳ魔女三人組" NAME_EN("Mario vs. Shadow Sirens"), "win_00", "e_bero", NULL},
    {0, 2, 10, 86, "STG2_08", "負けた魔女三人組" NAME_EN("Shadow Sirens Lose"), "win_00", "", NULL},
    {0, 2, 10, 87, "STG2_09", "クラウダ仲間になる" NAME_EN("Flurrie Joins"), "win_04", "w_bero", stg2_09_init},
    {0, 5, 10, 88, "STG2_10", "大樹への入り口出現" NAME_EN("Secret Entrance Revealed!"), "mri_00", "w_bero", stg2_10_init},
    {0, 5, 10, 89, "STG2_11", "大樹での初バトル" NAME_EN("X-Naut Battle"), "mri_01", "w_bero", stg2_11_init},
    {0, 5, 0, 90, "STG2_12", "ライバル登場" NAME_EN("Rival Arrival"), "mri_01", "", NULL},
    {0, 5, 0, 91, "STG2_12_01", "黒い影" NAME_EN("A Dark Figure"), "mri_04", "", stg2_12_01_init},
    {0, 5, 0, 92, "STG2_12_02", "とらわれ長老" NAME_EN("Puni Elder Imprisoned"), "mri_03", "dokan", NULL},
    {0, 5, 0, 93, "STG2_13", "チュチュリーナ遭遇" NAME_EN("Ms. Mowz"), "mri_04", "e_bero", NULL},
    {0, 5, 0, 94, "STG2_14", "長老救出" NAME_EN("Puni Elder Freed"), "mri_03", "w_bero", stg2_14_init},
    {0, 5, 0, 95, "STG2_15", "ライバル改心" NAME_EN("11 Punies"), "mri_01", "dokan", stg2_15_init},
    {0, 5, 0, 96, "STG2_16", "長老の能力" NAME_EN("The Puni Elder's Powers"), "mri_01", "w_bero", stg2_16_init00},
    {0, 5, 0, 96, "STG2_16", "プニスイッチ" NAME_EN("Puni Switch"), "mri_02", "dokan1", stg2_16_init01},
    {0, 5, 0, 97, "STG2_17", "シャボン玉" NAME_EN("Bubbles"), "mri_05", "dokan3", NULL},
    {0, 5, 0, 97, "STG2_17", "飛行機で？" NAME_EN("By Plane?"), "mri_19", "dokan1", NULL},
    {0, 5, 0, 97, "STG2_17", "トゲ族と出会う" NAME_EN("Jabbi Encounter"), "mri_06", "dokan1", NULL},
    {0, 5, 0, 98, "STG2_18", "ちびトゲ" NAME_EN("Jabble"), "mri_07", "e_bero", NULL},
    {0, 5, 0, 98, "STG2_18", "プニ族ＶＳトゲ族１" NAME_EN("Punies vs. Jabbies"), "mri_08", "w_bero", stg2_18_init01},
    {0, 5, 0, 98, "STG2_18", "青い鍵入手" NAME_EN("Blue Key Obtained"), "mri_09", "w_bero", stg2_18_init02},
    {0, 5, 0, 99, "STG2_19", "青い牢屋の中の様子" NAME_EN("Inside the Blue Cell"), "mri_03", "", stg2_19_init},
    {0, 5, 0, 100, "STG2_19_01", "プニコ再会直前" NAME_EN("Punio & Petuni(Before)"), "mri_03", "dokan", NULL},
    {0, 5, 0, 101, "STG2_20", "プニコ再会" NAME_EN("Punio & Petuni"), "mri_03", "dokan", stg2_20_init},
    {0, 5, 0, 102, "STG2_21", "シャボン玉２" NAME_EN("Bubbles 2"), "mri_05", "dokan3", stg2_21_init00},
    {0, 5, 0, 102, "STG2_21", "プニ族ＶＳトゲ族２" NAME_EN("Punies vs. Jabbies 2"), "mri_11", "e_bero", NULL},
    {0, 5, 0, 102, "STG2_21", "ワナにはまる" NAME_EN("Trapped"), "mri_17", "w_bero", stg2_21_init02},
    {0, 5, 0, 103, "STG2_21_01", "なぞの像" NAME_EN("Mystery Pillar"), "mri_17", "w_bero", stg2_21_01_init},
    {0, 5, 0, 104, "STG2_21_02", "絵合わせ" NAME_EN("Matching the Patterns"), "mri_09", "w_bero", stg2_21_02_init},
    {0, 5, 0, 105, "STG2_21_03", "スーパーブーツ" NAME_EN("Super Boots"), "mri_10", "s_bero", stg2_21_03_init},
    {0, 5, 0, 106, "STG2_21_04", "ワナ脱出" NAME_EN("Escape"), "mri_17", "w_bero", stg2_21_04_init},
    {0, 5, 0, 107, "STG2_21_06", "水の仕掛け" NAME_EN("Water Switch"), "mri_16", "dokan2", stg2_21_06_init00},
    {0, 5, 0, 107, "STG2_21_06", "１０１匹スイッチ" NAME_EN("The 101 Puni Switch"), "mri_13", "dokan", stg2_21_06_init01},
    {0, 5, 0, 108, "STG2_22", "第三勢力幹部出現" NAME_EN("Lord Crump Appears"), "mri_14", "", NULL},
    {0, 5, 0, 109, "STG2_23", "時限爆弾作動" NAME_EN("Time Bomb Activated"), "mri_14", "w_bero", stg2_23_init},
    {0, 5, 0, 110, "STG2_23_01", "第三勢力部下バトル" NAME_EN("X-Naut Battle"), "mri_15", "dokan", stg2_23_01_init},
    {0, 5, 0, 111, "STG2_24", "ボスバトル" NAME_EN("Boss Battle"), "mri_01", "e_bero", NULL},
    {0, 5, 0, 112, "STG2_25", "ボスバトル後" NAME_EN("After the Boss Battle"), "mri_01", "", stg2_25_init},
    {0, 0, 0, 113, "STG2_25_01", "ピーチ編：首領と手下" NAME_EN("Peach: Crump & X-Nauts"), "aji_10", "", NULL},
    {0, 0, 0, 113, "STG2_25_01", "ピーチ編：ドアが開いた２" NAME_EN("Peach: An Open Door 2"), "aji_16", "", NULL},
    {0, 0, 0, 114, "STG2_25_02", "ピーチ編：テックのダンスミニゲーム" NAME_EN("Peach: Shall We Dance?"), "aji_17", "w_bero", stg2_25_02_init},
    {0, 0, 0, 115, "STG2_25_03", "クッパ編：平原" NAME_EN("Bowser: Meadows"), "hei_00", "", NULL},
    {0, 0, 0, 116, "STG2_25_04", "クッパ編：アクションステージ１" NAME_EN("Bowser: Action Stage"), "kpa_02", "", NULL},
    {0, 0, 0, 117, "STG2_25_05", "クッパ編：ノコ村" NAME_EN("Bowser: Petalburg"), "nok_00", "w_bero", NULL},
    {0, 5, 0, 118, "STG2_26", "マリオ激励される" NAME_EN("Praise from the Puni"), "mri_00", "", stg2_26_init},
    {0, 5, 0, 119, "STG2_27", "ピーチからのメール" NAME_EN("A Mail from Peach"), "win_01", "dokan1", NULL},
    {0, 5, 0, 120, "STG2_40", "大きな扉ステージ２後" NAME_EN("Thousand-Year Door"), "tik_05", "e_bero", stg2_40_init},
    {0, 5, 0, 121, "STG2_41", "地図解読ステージ２後" NAME_EN("Frankly's House Stg.2"), "gor_02", NULL, NULL},
    {0, 5, 0, 122, "STG2_44", "ボスに会う方法" NAME_EN("Finding Don Pianta"), "gor_02", "w_bero", NULL},
    {0, 5, 0, 122, "STG2_44", "裏口へ" NAME_EN("Backdoor"), "gor_03", "e_bero", NULL},
    {0, 5, 0, 122, "STG2_44", "マフィアのボス" NAME_EN("Mafia Boss"), "gor_03", "", stg2_44_init},
    {0, 5, 0, 123, "STG2_45", "娘発見" NAME_EN("Daughter Located"), "gor_00", "n_bero", NULL},
    {0, 5, 0, 124, "STG2_46", "飛行船のチケットゲット" NAME_EN("Blimp Ticket"), "gor_03", "", NULL},
    {0, 5, 0, 125, "STG2_47", "チケットゲット直後" NAME_EN("After Getting the Ticket"), "gor_03", "", stg2_47_init},
    {0, 5, 0, 126, "STG2_END", "ウーロン街へ" NAME_EN("To Glitzville"), "gor_04", "s_bero", stg2_end_init}
};

EventData stage3_evt[] = {
    {0, 1, 0, 127, "STG3_00", "初めてのウーロン街" NAME_EN("Welcome to Glitzville"), "tou_20", "start", NULL},
    {0, 1, 0, 128, "STG3_01", "初めての闘技場（１Ｆ）" NAME_EN("Arena Intro (1F)"), "tou_03", "s_bero_1", NULL},
    {0, 1, 0, 128, "STG3_01", "初めての闘技場（２Ｆ）" NAME_EN("Arena Intro (2F)"), "tou_03", "s_bero_2", NULL},
    {0, 1, 0, 129, "STG3_02", "ガードマンどいた" NAME_EN("The Guard Backs Off"), "tou_02", "s_bero", NULL},
    {0, 1, 0, 130, "STG3_03", "マリオ選手になる" NAME_EN("Grubba"), "tou_05", "n_bero", NULL},
    {0, 1, 0, 131, "STG3_04", "選手になった" NAME_EN("Gonzales is Born"), "tou_05", "n_bero", NULL},
    {0, 1, 0, 132, "STG3_05", "試合登録レクチャー" NAME_EN("Match"), "tou_10", NULL, NULL},
    {0, 1, 0, 133, "STG3_06", "第１試合" NAME_EN("1st Match"), "tou_03", NULL, stg3_06_init},
    {0, 1, 0, 134, "STG3_07", "２部−９位" NAME_EN("Minors - 9th Pl."), "tou_10", "n_bero", stg3_07_init__9},
    {0, 1, 0, 134, "STG3_07", "２部−８位" NAME_EN("Minors - 8th Pl."), "tou_10", "n_bero", stg3_07_init__8},
    {0, 1, 0, 134, "STG3_07", "２部−７位" NAME_EN("Minors - 7th Pl."), "tou_10", "n_bero", stg3_07_init__7},
    {0, 1, 0, 134, "STG3_07", "２部−６位" NAME_EN("Minors - 6th Pl."), "tou_10", "n_bero", stg3_07_init__6},
    {0, 1, 0, 135, "STG3_10", "２部−５位（たまご）" NAME_EN("Minors - 5th Pl. (Egg)"), "tou_01", "a_door_mon", stg3_10_init__5},
    {0, 1, 0, 136, "STG3_11", "たまご逃げまくり" NAME_EN("Runaway Egg"), "tou_01", "a_door_mon", NULL},
    {0, 1, 0, 137, "STG3_12", "ヨッシーのたまごゲット！" NAME_EN("The Mini-Yoshi Egg"), "tou_01", "a_door_mon", NULL},
    {0, 1, 8, 137, "STG3_12", "２部−４位" NAME_EN("Minors - 4th Pl."), "tou_10", "n_bero", stg3_12_init__4},
    {0, 1, 8, 137, "STG3_12", "２部−３位" NAME_EN("Minors - 3rd Pl."), "tou_10", "n_bero", stg3_12_init__3},
    {0, 1, 8, 137, "STG3_12", "２部−２位" NAME_EN("Minors - 2nd Place"), "tou_10", "n_bero", stg3_12_init__2},
    {0, 1, 8, 137, "STG3_12", "２部−１位（アイアン戦）" NAME_EN("Minors - 1st Pl. (Iron)"), "tou_10", "n_bero", stg3_12_init__1},
    {0, 4, 0, 137, "STG3_12", "２部−１位（アイアン戦２）" NAME_EN("Minors - 1st Pl. (Iron 2)"), "tou_10", "n_bero", stg3_12_2_init},
    {0, 4, 0, 138, "STG3_20", "１部−１０位" NAME_EN("Majors - 10th Place"), "tou_08", "n_bero", stg3_20_init_10},
    {0, 4, 0, 138, "STG3_20", "１部−９位" NAME_EN("Majors - 9th Place"), "tou_08", "n_bero", stg3_20_init_9},
    {0, 4, 0, 139, "STG3_20_00", "１部−８位" NAME_EN("Majors - 8th Place"), "tou_08", "n_bero", stg3_20_init_8},
    {0, 4, 0, 140, "STG3_20_01", "ハンマーゲット" NAME_EN("Super Hammer"), "tou_01", "a_door_mon", stg3_20_01_init},
    {0, 4, 0, 141, "STG3_20_02", "再び２部リーグへ" NAME_EN("Back to the Minors"), "tou_04", "e_bero", stg3_20_02_init},
    {0, 4, 0, 142, "STG3_20_03", "壁破壊" NAME_EN("The Yellow Block"), "tou_10", "n_bero", NULL},
    {0, 4, 0, 143, "STG3_20_04", "キノシコワに没収" NAME_EN("Confiscated"), "tou_10", "n_bero", NULL},
    {0, 4, 0, 143, "STG3_20_04", "１部−７位（乱入・ケーキ）" NAME_EN("Majors - 7th Pl.(Raid/Cake)"), "tou_08", "n_bero", stg3_20_init_7},
    {0, 4, 0, 143, "STG3_20_04", "１部−６位（クッパ乱入）" NAME_EN("Majors - 6th Pl.(Bowser)"), "tou_08", "n_bero", stg3_20_init_6},
    {0, 4, 0, 143, "STG3_20_04", "１部−５位" NAME_EN("Majors - 5th Pl."), "tou_08", "n_bero", stg3_20_init_5},
    {0, 4, 0, 144, "STG3_20_05", "１部−４位" NAME_EN("Majors - 4th Pl."), "tou_08", "n_bero", stg3_20_init_4},
    {0, 4, 0, 145, "STG3_20_06", "ブレッドハート逃げる" NAME_EN("Rawk Hawk's Warning"), "tou_04", "s_bero_6", stg3_20_06_init},
    {0, 4, 0, 146, "STG3_20_07", "カギをゲット" NAME_EN("Storage Key"), "tou_01", "a_door_mon", NULL},
    {0, 4, 0, 147, "STG3_20_07_00", "倉庫を開錠" NAME_EN("Storage Room Unlocked"), "tou_04", "e_bero", stg3_20_07_00_init},
    {0, 4, 0, 148, "STG3_20_08", "チュチュリーナ" NAME_EN("Ms. Mowz"), "tou_06", "n_bero", stg3_20_08_init},
    {0, 5, 0, 149, "STG3_20_09", "コンテナふきとばし" NAME_EN("Blowing Containers"), "tou_06", "n_bero", stg3_20_09_init},
    {0, 4, 0, 150, "STG3_20_10", "階段出現" NAME_EN("Hidden Stairway"), "tou_06", "n_bero", NULL},
    {0, 4, 0, 151, "STG3_20_11", "床破壊" NAME_EN("Hole in the Floor"), "tou_06", "n_bero", NULL},
    {0, 4, 0, 152, "STG3_20_12", "キノシコワ密談" NAME_EN("Jolene's Secret"), "tou_05", "fall", stg3_20_12_init},
    {0, 4, 0, 152, "STG3_20_12", "１部−３位（毒ケーキ）" NAME_EN("Majors - 3rd Pl.(Poison)"), "tou_08", "n_bero", stg3_20_init_3},
    {0, 4, 0, 152, "STG3_20_12", "１部−２位" NAME_EN("Majors - 2nd Pl."), "tou_08", "n_bero", stg3_20_init_2},
    {0, 1, 0, 153, "STG3_20_13", "１部−１位" NAME_EN("Majors - 1st Pl."), "tou_08", "n_bero", stg3_20_init_1},
    {0, 5, 0, 154, "STG3_20_14", "ロビーのポスター" NAME_EN("Lobby Poster"), "tou_02", "n_bero_4", stg3_20_13_init},
    {0, 1, 0, 155, "STG3_20_15", "カギをあける" NAME_EN("Unlocking the Door"), "tou_06", "n_bero", stg3_20_14_init},
    {0, 1, 0, 156, "STG3_20_16", "干物を発見" NAME_EN("Flattened"), "tou_12", "w_bero", stg3_20_15_init},
    {0, 1, 0, 157, "STG3_21", "チャンピョンに初挑戦つづき" NAME_EN("1st Match with Rawk Hawk"), "tou_08", "dokan", stg3_21_init},
    {0, 1, 0, 158, "STG3_30", "マリオチャンピョンになる" NAME_EN("A New Champion"), "tou_08", NULL, stg3_30_init},
    {0, 1, 0, 158, "STG3_30", "マリオチャンピョン" NAME_EN("Champion's Room"), "tou_07", "n_bero", stg3_31_00_init},
    {0, 1, 0, 159, "STG3_30_01", "ダクト入口" NAME_EN("Air Duct Entrance"), "tou_07", "n_bero", NULL},
    {0, 1, 0, 160, "STG3_30_02", "初めてのダクト" NAME_EN("Air Duct (1st Time)"), "tou_13", "e_bero_", NULL},
    {0, 1, 0, 161, "STG3_30_03", "盗み聞き" NAME_EN("Eavesdropping"), "tou_13", "e_bero_", NULL},
    {0, 1, 0, 162, "STG3_30_04", "ダクト出口" NAME_EN("Air Duct Exit"), "tou_13", "e_bero_", NULL},
    {0, 1, 0, 163, "STG3_31", "ガンスの企み" NAME_EN("Grubba's Plans"), "tou_05", "w_bero", stg3_31_init},
    {0, 1, 0, 164, "STG3_32", "ＢＯＳＳ戦" NAME_EN("Boss Battle"), "tou_03", "w_bero", stg3_32_init},
    {0, 0, 0, 165, "STG3_33", "ピーチ編：影三姉妹の謁見" NAME_EN("Peach: The Secret Weapon"), "aji_10", NULL, stg3_33_init},
    {0, 0, 0, 166, "STG3_33_01", "ピーチ編：テックの頼み１" NAME_EN("Peach: TEC's Request"), "aji_17", "w_bero", NULL},
    {0, 0, 0, 167, "STG3_33_02", "ピーチ編：変装" NAME_EN("Peach: Incognito"), "aji_09", "s_bero", NULL},
    {0, 0, 0, 168, "STG3_33_03", "ピーチ編：スパイ大作戦" NAME_EN("Peach: Grodus"), "aji_10", "w_bero", stg3_33_03_init},
    {0, 0, 0, 169, "STG3_33_04", "ピーチ編：世界征服の野望" NAME_EN("Peach: Back to TEC"), "aji_17", "w_bero", stg3_33_04_init},
    {0, 0, 0, 170, "STG3_33_05", "クッパ編：森" NAME_EN("Bowser: The Great Tree"), "mri_01", NULL, NULL},
    {0, 0, 0, 171, "STG3_33_06", "クッパ編：お宝はどこ？" NAME_EN("Bowser: Treasure?"), "mri_01", "player", NULL},
    {0, 0, 0, 171, "STG3_33_06", "クッパ編：長老" NAME_EN("Bowser: Puni Elder"), "mri_01", "event", NULL},
    {0, 1, 0, 172, "STG3_34", "ゴロツキタウンにもどる" NAME_EN("Return to Rogueport"), "tou_02", "after_peach", stg3_34_init},
    {0, 1, 0, 173, "STG3_35", "ピーチからのメール" NAME_EN("A Mail from Peach"), "gor_03", "n_bero", stg3_35_init},
    {0, 5, 0, 174, "STG3_50", "大きな扉ステージ３後" NAME_EN("Thousand-Year Door"), "tik_05", "e_bero", stg3_50_init},
    {0, 5, 0, 175, "STG3_51", "地図解読ステージ３後" NAME_EN("Frankly's House Stg.3"), "gor_02", NULL, NULL},
    {0, 5, 0, 176, "STG3_52", "クラガリさん" NAME_EN("Darkly"), "gor_02", "w_bero", stg3_52_init},
    {0, 5, 0, 177, "STG3_END", "ステージ４へ" NAME_EN("To Stage 4"), "tik_07", "e_bero", stg3_end_init}
};

EventData stage4_evt[] = {
    {0, 1, 0, 178, "STG4_00", "ステージ４へようこそ" NAME_EN("Welcome to Stage 4"), "sys_01", "stg_4", stage4_00_init},
    {0, 1, 0, 179, "STG4_01_00", "門番と話した" NAME_EN("Gatekeeper Conversation"), "usu_01", "w_bero", NULL},
    {0, 1, 0, 180, "STG4_01", "村長がブタに！！" NAME_EN("The Mayor's a Pig!"), "usu_00", "e_bero", NULL},
    {0, 1, 0, 181, "STG4_02", "門番がブタに！！" NAME_EN("Dinge"), "usu_01", "w_bero", NULL},
    {0, 1, 0, 182, "STG4_02_02", "ブタ門番を見て一言" NAME_EN("The Gatekeeper's a Pig!"), "usu_01", "w_bero", NULL},
    {0, 1, 0, 183, "STG4_02_03", "門を開く" NAME_EN("Opening the Gate"), "usu_01", "w_bero", NULL},
    {0, 1, 0, 184, "STG4_03", "倉庫のカギを入手" NAME_EN("Shop Key"), "gra_02", "w_bero_1", NULL},
    {0, 1, 0, 185, "STG4_04", "倉庫を開ける" NAME_EN("Storage Room Unlocked"), "usu_01", "e_bero", stage4_04_init},
    {0, 1, 0, 187, "STG4_04_01", "ペラ魔人（ロール取得）" NAME_EN("Paper Mage (Tube Mod)"), "usu_01", "e_bero", stage4_04_01_init},
    {0, 1, 0, 188, "STG4_05", "わら" NAME_EN("Straw Tepee"), "gra_02", "w_bero_1", stage4_05_init},
    {0, 5, 0, 189, "STG4_06", "木の幹" NAME_EN("Tree Trunk"), "gra_04", "e_bero", NULL},
    {0, 5, 0, 190, "STG4_06_01", "床を吹き飛ばす" NAME_EN("Blow the Floor"), "gra_05", "w_bero", NULL},
    {0, 1, 0, 191, "STG4_06_02", "穴を開ける" NAME_EN("Through the Hole"), "gra_05", "w_bero", NULL},
    {0, 1, 0, 192, "STG4_07", "岩の仕掛け" NAME_EN("Trick Rock"), "gra_05", "w_bero", NULL},
    {0, 1, 0, 193, "STG4_08", "ダイム寺院のカメラ" NAME_EN("Steeple Demo (EXT.)"), "gra_06", "sw_bero", NULL},
    {0, 1, 0, 194, "STG4_09", "ダイム寺院内のカメラ" NAME_EN("Steeple Demo (INT.)"), "jin_00", "w_bero", NULL},
    {0, 1, 0, 195, "STG4_09_01", "台座を押す" NAME_EN("Push the Statue"), "jin_00", "w_bero", NULL},
    {0, 1, 0, 196, "STG4_09_02", "テレサ箱" NAME_EN("Boo Box"), "jin_07", "e_bero", NULL},
    {0, 1, 0, 197, "STG4_09_03", "１０００テレサ" NAME_EN("1000 Boos"), "jin_00", "ana", NULL},
    {0, 1, 0, 198, "STG4_09_04", "かぎ" NAME_EN("Key"), "jin_00", "e_bero_3", NULL},
    {0, 1, 0, 199, "STG4_09_05", "じょうまえ" NAME_EN("Lock"), "jin_00", "e_bero_1", stage4_09_05_init},
    {0, 1, 0, 200, "STG4_10", "ランペル遭遇" NAME_EN("Doopliss Battle"), "jin_04", "s_bero", stage4_10_init},
    {0, 1, 0, 201, "STG4_10_02", "入れ替わったマリオ" NAME_EN("Trading Places"), "jin_04", NULL, NULL},
    {0, 0, 0, 202, "STG4_11", "ｓｔｇ４魔女三人組" NAME_EN("Superbombbomb Lost"), "usu_00", "evt_majo3", stg4_11_init},
    {0, 0, 0, 203, "STG4_11_02", "待伏せランペル１" NAME_EN("Waiting Doopliss"), "gra_00", "e_bero", NULL},
    {0, 0, 0, 204, "STG4_12", "ビビアン" NAME_EN("Vivian"), "usu_00", "e_bero", NULL},
    {0, 0, 0, 205, "STG4_12_01", "イチコロリー玉発見" NAME_EN("Superbombbomb Found"), "usu_00", "e_bero", NULL},
    {0, 0, 0, 206, "STG4_13", "ビビアン仲間" NAME_EN("Vivian Joins"), "usu_00", "e_bero", stg4_13_init},
    {0, 6, 0, 207, "STG4_13_02", "待伏せランペル２" NAME_EN("Waiting Doopliss 2"), "gra_00", "w_bero", stg4_13_02_init},
    {0, 6, 0, 208, "STG4_13_03", "床こわす" NAME_EN("Breaking the Floor"), "jin_11", "s_bero", stg4_13_03_init},
    {0, 6, 0, 209, "STG4_13_04", "「ん」の文字" NAME_EN("The Letter p"), "jin_08", "e_bero_2", NULL},
    {0, 6, 0, 210, "STG4_14", "名前を言い当てる" NAME_EN("Name Guessing"), "gra_00", "e_bero", stg4_14_init},
    {0, 6, 0, 211, "STG4_15", "ランペルと最終バトル" NAME_EN("Doopliss Battle 2"), "jin_04", "s_bero", stg4_15_init},
    {0, 6, 0, 212, "STG4_15_02", "マリオ本当の姿を取り戻す" NAME_EN("Back to Normal"), "jin_04", NULL, NULL},
    {0, 6, 0, 213, "STG4_15_03", "ルビースター取得" NAME_EN("The Ruby Star"), "jin_04", "s_bero", stg4_15_03_init},
    {0, 6, 0, 214, "STG4_15_04", "魔女二人とランペル合流" NAME_EN("Shadow Sirens & Doopliss"), "usu_00", "s_bero", NULL},
    {0, 0, 0, 215, "STG4_15_05", "ピーチ編：首領と手下" NAME_EN("Peach: X-Naut PhD Report"), "aji_10", NULL, NULL},
    {0, 0, 0, 215, "STG4_15_05", "ピーチ編：テックのクイズ" NAME_EN("Peach: TEC's Quiz"), "aji_17", "w_bero", NULL},
    {0, 0, 0, 216, "STG4_15_06", "クッパ編：飛行船乗り場" NAME_EN("Bowser: Blimp Area"), "gor_04", "", NULL},
    {0, 0, 0, 216, "STG4_15_06", "クッパ編：空の上" NAME_EN("Bowser: Flying"), "tou_00", "", NULL},
    {0, 0, 0, 217, "STG4_15_07", "クッパ編：アクションステージ２" NAME_EN("Bowser: Action Stage"), "kpa_03", "", NULL},
    {0, 0, 0, 218, "STG4_15_08", "クッパ編：ゴロツキ港" NAME_EN("Bowser: Rogueport Dock"), "gor_00", "", NULL},
    {0, 6, 0, 219, "STG4_16", "ウスグラタウンを去る" NAME_EN("Leaving Twilight Town"), "usu_00", NULL, stg4_16_init},
    {0, 6, 0, 220, "STG4_17", "ピーチからのメール" NAME_EN("A Mail from Peach"), "tik_07", "n_bero", NULL},
    {0, 6, 0, 221, "STG4_30", "大きな扉ステージ４後" NAME_EN("Thousand-Year Door"), "tik_05", "e_bero", stg4_30_init},
    {0, 6, 0, 222, "STG4_31", "地図解読ステージ４後" NAME_EN("Frankly's House Stg.4"), "gor_02", NULL, NULL},
    {0, 6, 0, 223, "STG4_32", "マルコと意気投合" NAME_EN("Chatting with Flavio"), "gor_01", "e_bero", NULL},
    {0, 6, 0, 224, "STG4_33", "サンダース紹介" NAME_EN("Meet Bobbery"), "gor_00", "n_bero", NULL},
    {0, 6, 0, 225, "STG4_34", "サンダース断られる" NAME_EN("Bobbery Declines"), "gor_02", "w_bero", NULL},
    {0, 6, 0, 226, "STG4_35", "マスターに相談" NAME_EN("Advice from Podley"), "gor_01", "e_bero", stg4_35_init},
    {0, 6, 0, 227, "STG4_36", "サンダースの決意" NAME_EN("Bobbery's Decision"), "gor_02", "w_bero", stg4_36_init},
    {0, 6, 15, 228, "STG4_37", "サンダース港へ" NAME_EN("To the Port"), "gor_02", "w_bero", stg4_37_init},
    {0, 6, 0, 229, "STG4_END", "船の出発" NAME_EN("Off to the Keys"), "gor_00", "n_bero", stg4_end_init}
};

EventData stage5_evt[] = {
    {0, 1, 0, 230, "STG5_00", "航海日誌" NAME_EN("Logbook"), "sys_01", "stg_5", NULL},
    {0, 1, 0, 231, "STG5_10", "怨霊襲撃" NAME_EN("Haunted"), "muj_20", NULL, NULL},
    {0, 1, 0, 232, "STG5_11", "無人島漂着" NAME_EN("Deserted Island"), "muj_00", NULL, NULL},
    {0, 2, 0, 233, "STG5_12", "仲間割れ" NAME_EN("Flavio & Pa-Patch"), "muj_01", "w_bero", NULL},
    {0, 2, 0, 233, "STG5_12", "挙手願おう" NAME_EN("Flavio's Favor"), "muj_01", "", stg5_12_init01},
    {0, 2, 0, 234, "STG5_13", "サンダース孤軍奮闘" NAME_EN("Bobbery & the Bridge"), "muj_04", "w_bero", NULL},
    {0, 2, 0, 235, "STG5_15", "にげる水夫たち" NAME_EN("Runaway Sailors"), "muj_03", "e_bero", NULL},
    {0, 2, 0, 235, "STG5_15", "オドオドした水夫説得" NAME_EN("Flavio's Rescue Plan"), "muj_01", "e_bero1", stg5_15_init01},
    {0, 2, 0, 235, "STG5_15", "サンダース救出" NAME_EN("Saving Bobbery"), "muj_05", "w_bero", stg5_15_init02},
    {0, 4, 0, 236, "STG5_16", "マルコのワイン" NAME_EN("Flavio's Wine"), "muj_01", "e_bero1", NULL},
    {0, 5, 0, 237, "STG5_17", "ワインゲット" NAME_EN("Wine Obtained"), "muj_01", "e_bero1", stg5_17_init},
    {0, 1, 0, 238, "STG5_18", "サンダース救出２" NAME_EN("Bobbery Rescue 2"), "muj_05", "w_bero", stg5_18_init},
    {0, 1, 0, 239, "STG5_18_01", "サンダース仲間" NAME_EN("Bobbery Joins"), "muj_05", "w_bero", NULL},
    {0, 3, 0, 240, "STG5_18_02", "挙手願おうその２" NAME_EN("Flavio's Favor 2"), "muj_01", "e_bero1", stg5_18_init_02},
    {0, 3, 9, 241, "STG5_18_03", "マルコと宝石" NAME_EN("Marco's Jewel"), "muj_05", "w_bero", NULL},
    {0, 3, 0, 242, "STG5_19", "宝石と洞窟と子守唄" NAME_EN("Gems and Jingles"), "muj_05", "w_bero", stg5_19_init00},
    {0, 3, 0, 242, "STG5_19", "ドクロ岩のなぞ" NAME_EN("The Skull Rock"), "muj_05", "w_bero", stg5_19_init01},
    {0, 3, 0, 242, "STG5_19", "ドクロ岩爆破" NAME_EN("Skull Rock Blast"), "muj_05", "w_bero", stg5_19_init02},
    {0, 3, 0, 243, "STG5_19_01", "洞窟突入" NAME_EN("Entering the Grotto"), "dou_00", "w_bero", stg5_19_01_init},
    {0, 3, 0, 244, "STG5_19_02", "コルテスの叫び声" NAME_EN("The Call of Cortez"), "dou_01", "w_bero", NULL},
    {0, 3, 0, 244, "STG5_19_02", "動くトラップ１" NAME_EN("Spikes (Floor)"), "dou_02", "w_bero_1", NULL},
    {0, 3, 0, 245, "STG5_20", "棚の上のスイッチ" NAME_EN("The Blue Block"), "dou_05", "w_bero_1", NULL},
    {0, 3, 0, 246, "STG5_21", "カギツメのかぎ" NAME_EN("Grotto Key"), "dou_04", "e_bero", stg5_21_init00},
    {0, 3, 0, 246, "STG5_21", "動くトラップ２" NAME_EN("Spikes (Wall)"), "dou_10", "n_bero", stg5_21_init01},
    {0, 3, 0, 246, "STG5_21", "爆弾穴" NAME_EN("Ship Hole"), "dou_09", "e_bero_2", NULL},
    {0, 3, 0, 246, "STG5_21", "ペラ魔人４" NAME_EN("Paper Mage 4"), "dou_07", "s_bero", stg5_21_init03},
    {0, 3, 0, 247, "STG5_21_01", "船変形ゲット" NAME_EN("Boat Mod Obtained"), "dou_07", "s_bero", stg5_21_01_init00},
    {1, 3, 0, 248, "STG5_22", "水門ハンドル" NAME_EN("Floodgate Handle"), "dou_02", "e_bero_1", stg5_22_init00},
    {0, 3, 0, 248, "STG5_22", "水門が開く" NAME_EN("Floodgate"), "dou_03", "w_bero_2", stg5_22_init01},
    {1, 3, 0, 249, "STG5_29", "波よけミニゲーム" NAME_EN("Wave-dodging"), "dou_10", "w_bero_1", stg5_29_init},
    {1, 3, 0, 250, "STG5_30", "取り残された水夫たち" NAME_EN("Shipwrecked Sailors"), "dou_11", "w_bero", NULL},
    {0, 3, 0, 250, "STG5_30", "スイッチテスト" NAME_EN("Switch Test"), "dou_11", "e_bero", stg5_30_init01},
    {1, 3, 0, 250, "STG5_30", "水夫お礼" NAME_EN("Thankful Sailors"), "dou_11", "w_bero", stg5_30_init02},
    {0, 3, 0, 251, "STG5_31", "なぞの出現" NAME_EN("Four-Eyes"), "muj_10", "w_bero", NULL},
    {0, 3, 0, 252, "STG5_32", "コルテスの警告" NAME_EN("Cortez's Warning"), "muj_11", "w_bero", NULL},
    {0, 3, 0, 253, "STG5_33", "コルテスとお宝" NAME_EN("Cortez and the Treasure"), "muj_12", "w_bero", NULL},
    {0, 3, 0, 254, "STG5_34", "海賊船追い出される" NAME_EN("Kicked Off"), "muj_10", "", NULL},
    {0, 3, 0, 255, "STG5_35", "脱出口" NAME_EN("Escape Route"), "dou_11", "e_bero", NULL},
    {0, 3, 0, 256, "STG5_36", "脱出口爆破" NAME_EN("Hole in the Wall"), "dou_11", "e_bero", NULL},
    {0, 3, 0, 257, "STG5_40", "第三勢力襲来" NAME_EN("True Identity"), "muj_00", "n_bero_02", NULL},
    {0, 3, 9, 258, "STG5_41", "再び洞窟へ" NAME_EN("Return to the Grotto"), "muj_00", "e_bero", NULL},
    {0, 3, 9, 258, "STG5_41", "マルコ決死の交渉" NAME_EN("Flavio Makes a Deal"), "muj_12", "w_bero", NULL},
    {0, 3, 9, 259, "STG5_41_01", "船員もやってきた" NAME_EN("All Aboard"), "muj_10", "A_b_bero03", NULL},
    {0, 3, 0, 260, "STG5_42", "海賊船ＶＳ第三勢力" NAME_EN("Pirates vs. X-Nauts"), "muj_00", "", NULL},
    {0, 3, 0, 261, "STG5_43", "第三勢力撃退後" NAME_EN("X-Nauts Retreat"), "muj_00", "", stg5_43_init},
    {0, 0, 0, 262, "STG5_44", "ピーチ編：テックの頼み２" NAME_EN("Peach: TEC's Request 2"), "aji_10", "", NULL},
    {0, 0, 0, 263, "STG5_45", "ピーチ編：調合" NAME_EN("Peach: Potion Puzzle"), "aji_06", "s_bero", NULL},
    {0, 0, 0, 264, "STG5_46", "ピーチ編：ディスクを探す" NAME_EN("Peach: Disk Search"), "aji_11", "w_bero", NULL},
    {0, 0, 0, 265, "STG5_47", "ピーチ編：テックと会話" NAME_EN("Peach: TEC Talk"), "aji_17", "", stg5_47_init},
    {0, 0, 0, 266, "STG5_48", "クッパ編：ウスグラタウン到着" NAME_EN("Bowser: Twilight Town"), "usu_00", "", NULL},
    {0, 0, 0, 267, "STG5_49", "クッパ編：ウスグラタウン" NAME_EN("Bowser: Twilight Town 2"), "usu_00", "player", NULL},
    {0, 0, 0, 267, "STG5_49", "クッパ編：クッパVS第三勢力" NAME_EN("Bowser: Bowser vs. X-Nauts"), "usu_00", "event", NULL},
    {0, 3, 0, 268, "STG5_58", "島を去る" NAME_EN("Fair Winds!"), "muj_00", "", stg5_58_init},
    {0, 3, 0, 269, "STG5_59", "ゴロツキタウンへ帰還" NAME_EN("Rogueport Dock"), "gor_00", "", NULL},
    {0, 3, 0, 270, "STG5_59_01", "ピーチからのメール" NAME_EN("A Mail from Peach"), "gor_00", "", stg5_59_01_init},
    {0, 3, 0, 271, "STG5_60", "いつでも無人島へ" NAME_EN("Return to Keelhaul"), "gor_00", "n_bero", stg5_60_init},
    {0, 6, 0, 271, "STG5_60", "大きな扉ステージ５後" NAME_EN("Thousand-Year Door"), "tik_05", "e_bero", NULL},
    {0, 6, 0, 272, "STG5_61", "地図解読ステージ５後" NAME_EN("Frankly's House Stg.5"), "gor_02", NULL, NULL},
    {0, 5, 0, 273, "STG5_62", "手下の依頼" NAME_EN("An Ill Don Pianta"), "gor_03", "", stg5_62_init},
    {0, 5, 0, 274, "STG5_63", "再び無人島へ" NAME_EN("Keelhaul Anytime"), "muj_00", NULL, stg5_63_init},
    {0, 5, 0, 275, "STG5_64", "結婚指輪をなくす" NAME_EN("Lost Wedding Ring"), "muj_02", "w_bero", stg5_64_init},
    {0, 5, 0, 276, "STG5_64_01", "結婚指輪をとどける" NAME_EN("Wedding Ring Delivery"), "muj_02", "w_bero", stg5_64_01_init},
    {0, 5, 0, 277, "STG5_64_02", "モニー＆ピートン乗船" NAME_EN("F & F Sail Off"), "muj_00", "e_bero", stg5_64_02_init},
    {0, 5, 0, 278, "STG5_64_03", "モニー＆ピートン帰還" NAME_EN("F & F Arrive"), "gor_00", "", stg5_64_03_init},
    {0, 5, 0, 279, "STG5_65", "列車のチケットゲット" NAME_EN("Train Ticket"), "gor_03", "", stg5_65_init},
    {0, 6, 0, 280, "STG5_66", "マジョリンに知られる" NAME_EN("Beldam Finds Out"), "gor_03", "", stg5_66_init},
    {0, 6, 0, 281, "STG5_END", "列車発車" NAME_EN("The Train Leaves"), "gor_04", "s_bero", NULL}
};

EventData stage6_evt[] = {
    {0, 1, 0, 282, "STG6_00", "スタート" NAME_EN("Start"), "sys_01", "stg_6", stg6_00_init},
    {0, 1, 0, 283, "STG6_01", "車掌と話す" NAME_EN("The Conductor"), "rsh_04_a", "e_bero", NULL},
    {0, 1, 0, 284, "STG6_02", "ランチ紛失事件" NAME_EN("A Case of Lost Stew"), "rsh_03_a", "w_bero", NULL},
    {0, 1, 0, 285, "STG6_03", "何かのあと" NAME_EN("Stained Carpet"), "rsh_03_a", "e_bero", NULL},
    {0, 1, 0, 286, "STG6_04", "グルメキノピオ会話" NAME_EN("Grilling Heff T."), "rsh_02_a", "e_bero", NULL},
    {0, 1, 0, 287, "STG6_05", "チュボーのなべ発見" NAME_EN("Galley Kettle Discovered"), "rsh_02_a", "e_bero", NULL},
    {0, 1, 0, 288, "STG6_06", "ポワン助手" NAME_EN("Pennington's Assistant"), "rsh_04_a", "e_bero", stg6_06_init},
    {0, 1, 0, 289, "STG6_07", "ナリキンママと話す" NAME_EN("Sylvia"), "rsh_04_b", "e_bero", NULL},
    {0, 1, 0, 290, "STG6_08", "コナリキンと話す" NAME_EN("Bub"), "rsh_04_b", "e_bero", NULL},
    {0, 1, 0, 291, "STG6_09", "コナリキン依頼" NAME_EN("Bub's Request"), "rsh_04_b", "e_bero", NULL},
    {0, 1, 0, 292, "STG6_10", "車掌・トロンと話す" NAME_EN("The Conductor/Zip Toad"), "rsh_04_b", "e_bero", NULL},
    {0, 1, 0, 293, "STG6_11", "サインもらう" NAME_EN("Obtain Autograph"), "rsh_00_b", "w_bero", NULL},
    {0, 1, 0, 294, "STG6_12", "サインあげる" NAME_EN("Give Autograph"), "rsh_04_b", "e_bero", stg6_12_init},
    {0, 1, 0, 295, "STG6_13", "車掌と話す" NAME_EN("Talk to the Conductor"), "rsh_04_c", "e_bero", stg6_13_init},
    {0, 1, 0, 296, "STG6_14", "ゆうれい" NAME_EN("Ghost T."), "rsh_02_c", "w_bero", NULL},
    {0, 1, 0, 297, "STG6_15", "ゆうれい依頼" NAME_EN("Ghost Request"), "rsh_02_c", "w_bero", NULL},
    {0, 1, 0, 298, "STG6_16", "貨物室の許可もらう" NAME_EN("The Baggage Car"), "rsh_04_c", "e_bero", NULL},
    {0, 1, 0, 299, "STG6_17", "ゆうれいの日記" NAME_EN("Ghost Diary"), "rsh_05_a", "e_bero", NULL},
    {0, 1, 0, 300, "STG6_18", "毛布とこうかん" NAME_EN("Blanket Exchange"), "rsh_02_c", "w_bero", stg6_18_init},
    {0, 1, 0, 301, "STG6_19", "車掌に毛布" NAME_EN("Give Blanket"), "rsh_04_c", "e_bero", stg6_19_init},
    {0, 1, 0, 302, "STG6_20", "１日目が終了" NAME_EN("End of Day 1"), "rsh_02_c", "w_bero", stg6_20_init},
    {0, 1, 0, 303, "STG6_21", "２日目スタート" NAME_EN("Day 2"), "rsh_02_a", "w_bero", NULL},
    {0, 1, 0, 304, "STG6_22", "書類紛失事件" NAME_EN("Document"), "rsh_04_a", "e_bero", stg6_22_init},
    {0, 1, 0, 305, "STG6_23", "大事そうな紙" NAME_EN("Vital Paper"), "rsh_01_a", "w_bero", NULL},
    {0, 1, 0, 306, "STG6_24", "大事なてがかり" NAME_EN("A Vital Clue"), "rsh_04_a", "e_bero", stg6_24_init},
    {0, 1, 0, 307, "STG6_24_02", "大事そうな紙を調べる" NAME_EN("Another Clue"), "rsh_02_a", "w_bero", NULL},
    {0, 6, 0, 308, "STG6_25", "トロン出現" NAME_EN("Zip Toad Appears"), "rsh_02_a", "w_bero", NULL},
    {0, 1, 0, 309, "STG6_26", "トロンほかく" NAME_EN("Zip Toad Caught"), "rsh_02_a", "w_bero", NULL},
    {0, 1, 0, 310, "STG6_27", "途中駅に到着" NAME_EN("Riverside Station"), "hom_00", "hom_touchaku", stg6_27_init},
    {0, 1, 0, 311, "STG6_28", "駅の鍵もらう" NAME_EN("Station Key Obtained"), "hom_00", "n_bero_1", NULL},
    {0, 1, 0, 312, "STG6_29", "駅の鍵あける" NAME_EN("Station Door Unlocked"), "hom_00", "n_bero_1", stg6_29_init},
    {0, 1, 0, 313, "STG6_30", "折り紙階段" NAME_EN("Origami Stairs"), "eki_01", "w_dero", stg6_30_init},
    {0, 1, 0, 314, "STG6_31", "錠前の鍵ゲット" NAME_EN("Key"), "eki_02", "s_bero", NULL},
    {0, 1, 0, 315, "STG6_32", "錠前をあける" NAME_EN("Unlocking the Door"), "eki_01", "w_dero", stg6_32_init},
    {0, 1, 0, 316, "STG6_33", "ポスター飛ばす" NAME_EN("Blow the Poster"), "eki_03", "w_bero_1", stg6_33_init},
    {0, 1, 0, 317, "STG6_34", "スイッチのはこが開く" NAME_EN("3 Blue Blocks"), "eki_06", "e_bero_1", stg6_34_init},
    {0, 1, 0, 318, "STG6_35", "おりがみ階段" NAME_EN("Origami Stairs"), "eki_06", "e_bero_1", NULL},
    {0, 1, 0, 319, "STG6_36", "じゃばらジャンプ" NAME_EN("Spring Jump"), "eki_05", "w_bero_1", NULL},
    {0, 1, 0, 320, "STG6_37", "エレベータキー" NAME_EN("Elevator Key"), "eki_05", "n_bero", stg6_37_init},
    {0, 1, 0, 321, "STG6_38", "エレベータ動く" NAME_EN("Ride the Elevator"), "eki_00", "e_bero", stg6_38_init},
    {0, 1, 0, 322, "STG6_39", "初めてのモアモア" NAME_EN("Smorg (1st Encounter)"), "eki_00", "e_bero", stg6_39_init},
    {0, 1, 0, 323, "STG6_40", "モアモア追い払う" NAME_EN("Swatting Smorgs"), "eki_00", "e_bero", NULL},
    {0, 1, 0, 324, "STG6_41", "おりがみ階段" NAME_EN("Origami Stairs"), "eki_00", "e_bero", NULL},
    {0, 1, 0, 325, "STG6_42", "鉄橋が降りる" NAME_EN("Drawbridge Down"), "eki_00", "e_bero", NULL},
    {0, 1, 0, 326, "STG6_43", "電車出発" NAME_EN("Conductor Appears"), "hom_00", "n_bero_2", NULL},
    {0, 1, 0, 327, "STG6_44", "２日目終了" NAME_EN("End of Day 2"), "rsh_02_c", "w_bero", stg6_44_init},
    {0, 1, 0, 328, "STG6_45", "３日目スタート" NAME_EN("Day 3 Starts"), "rsh_02_a", "w_bero", NULL},
#ifdef VERSION_G8ME01
    {0, 1, 0, 329, "STG6_46", "運転手" NAME_EN("Engineer"), "rsh_00_a", "w_bero", stg6_47_00_init},
    {0, 1, 0, 330, "STG6_47_00", "倉庫のモアモア" NAME_EN("Baggage Car Smorgs"), "rsh_05_a", "e_bero", NULL},
#else
    {0, 1, 0, 329, "STG6_46", "運転手" NAME_EN("Engineer"), "rsh_00_a", "w_bero", NULL},
    {0, 1, 0, 330, "STG6_47_00", "倉庫のモアモア" NAME_EN("Baggage Car Smorgs"), "rsh_05_a", "e_bero", stg6_47_00_init},
#endif
    {0, 1, 0, 331, "STG6_47", "モアモアから救出" NAME_EN("Escaping the Smorgs"), "rsh_05_a", "e_bero", NULL},
    {0, 1, 0, 332, "STG6_48", "グレートモアモア" NAME_EN("Smorg Miasma"), "rsh_06_a", "w_bero", NULL},
    {0, 1, 0, 334, "STG6_49", "ピカリーヒルズ到着" NAME_EN("Poshley Heights"), "rsh_01_a", "w_bero", NULL},
    {0, 1, 0, 335, "STG6_50", "ポワンが神官" NAME_EN("The Sanctum Manager"), "pik_01", "nw_bero", NULL},
    {0, 1, 0, 336, "STG6_51", "ガーネットスター強奪" NAME_EN("3 Surprise Guests"), "pik_02", "s_bero", NULL},
    {0, 1, 0, 337, "STG6_52", "ドカン出現" NAME_EN("The Hidden Pipe"), "pik_02", "s_bero", NULL},
    {0, 1, 0, 338, "STG6_53", "ガーネットスター本物" NAME_EN("The Real Garnet Star"), "pik_03", "s_bero", NULL},
    {0, 1, 0, 339, "STG6_54", "ただいま" NAME_EN("Welcome Back"), "pik_02", "h_bero", stg6_54_init},
    {0, 0, 0, 340, "STG6_55", "ピーチ編：総統と手下の会話" NAME_EN("Peach: Grodus & X-Naut"), "aji_10", NULL, NULL},
    {0, 0, 0, 341, "STG6_55_01", "ピーチ編：テック消去" NAME_EN("Peach: TEC Deleted"), "aji_17", "w_bero", NULL},
    {0, 0, 0, 342, "STG6_55_02", "クッパ編：ゴロツキタウン" NAME_EN("Bowser: Rogueport"), "gor_01", "", NULL},
    {0, 0, 0, 343, "STG6_55_03", "クッパ編：アクションステージ３" NAME_EN("Bowser: Action Stage"), "kpa_05", "", NULL},
    {0, 0, 0, 344, "STG6_55_04", "クッパ編：ブレッドハート" NAME_EN("Bowser: Rawk Hawk"), "kpa_07", "", NULL},
    {0, 1, 0, 345, "STG6_56", "帰りの電車" NAME_EN("Rogueport Station"), "gor_04", "resha_return", stg6_56_init},
    {0, 1, 0, 346, "STG6_57", "ピーチからのメール" NAME_EN("A Mail from Peach"), "gor_04", "n_bero", NULL},
    {0, 6, 0, 347, "STG6_70", "大きな扉ステージ６後" NAME_EN("Thousand-Year Door"), "tik_05", "e_bero", stg6_70_init},
    {0, 6, 0, 348, "STG6_71", "地図解読ステージ６後" NAME_EN("Frankly's House Stg.6"), "gor_02", NULL, NULL},
    {0, 6, 0, 350, "STG6_72", "地図解読３分後" NAME_EN("3 Minutes Later"), "gor_02", "w_bero", stg6_71_01_init},
    {0, 6, 0, 350, "STG6_72", "月に行く方法" NAME_EN("A Way to the Moon"), "gor_02", "w_bero", stg6_72_init},
    {0, 6, 0, 351, "STG6_END", "ウルトラハンマーゲット" NAME_EN("Ultra Hammer"), "gor_01", "e_bero", NULL},
    {0, 6, 0, 351, "STG6_END", "ストーンブロック破壊" NAME_EN("Stone Block Smash"), "tik_07", "e_bero", stg6_end_init01},
    {0, 6, 0, 351, "STG6_END", "ボム村へ" NAME_EN("To Fahr Outpost"), "tik_07", "e_bero", stg6_end_init02}
};

EventData stage7_evt[] = {
    {0, 1, 0, 352, "STG7_00", "STG7 スタート" NAME_EN("Start"), "sys_01", "stg_7", NULL},
    {0, 1, 0, 353, "STG7_01", "初めてのボム村" NAME_EN("Fahr Outpost (1st Visit)"), "bom_01", "w_bero", stg7_01_init},
    {0, 3, 0, 354, "STG7_02", "村長と会話" NAME_EN("Mayor Bob"), "bom_02", "w_bero", NULL},
    {0, 1, 0, 355, "STG7_03", "ナリキンパパ" NAME_EN("Goldbob"), "pik_00", "se_bero", NULL},
    {0, 1, 0, 355, "STG7_03", "ホワイト居場所（ノコ村）" NAME_EN("Where's White?(Petalburg)"), "nok_00", "w_bero", stg7_03_white_0},
    {0, 1, 0, 355, "STG7_03", "ホワイト居場所（無人島）" NAME_EN("Where's White?(Keelhaul)"), "muj_01", "w_bero", stg7_03_white_1},
    {0, 1, 0, 355, "STG7_03", "ホワイト居場所（闘技場）" NAME_EN("Where's White?(Glitzville)"), "tou_01", "n_bero", stg7_03_white_2},
    {0, 1, 0, 355, "STG7_03", "ホワイト居場所（大樹）" NAME_EN("Where's White?(Great Tree)"), "mri_01", "w_bero", stg7_03_white_3},
    {0, 1, 0, 355, "STG7_03", "ホワイト居場所（ウスグラ）" NAME_EN("Where's White?(Twilight)"), "usu_00", "e_bero", stg7_03_white_4},
    {0, 1, 0, 355, "STG7_03", "ホワイト将軍目覚める" NAME_EN("Gen. White Awakens"), "bom_02", "w_bero", stg7_03_white_5},
    {0, 1, 0, 356, "STG7_04", "村長認める" NAME_EN("Back Online"), "bom_02", "w_bero", stg7_04_init},
    {0, 1, 0, 357, "STG7_05", "大砲発射" NAME_EN("The Cannon"), "bom_01", "e_bero", NULL},
    {0, 1, 0, 358, "STG7_06", "月に到着" NAME_EN("To the Moon"), "moo_00", NULL, stg7_06_init},
    {0, 3, 0, 359, "STG7_07", "ドカン出現" NAME_EN("A Pipe Appears"), "moo_02", "w_bero", NULL},
    {0, 3, 0, 360, "STG7_07_01", "アジトの外" NAME_EN("Inside the Building"), "aji_19", "w_bero", NULL},
    {0, 1, 0, 361, "STG7_08", "アジト潜入" NAME_EN("Elite X-Nauts"), "aji_00", "w_bero", NULL},
    {0, 1, 0, 362, "STG7_09", "エレベータキーＡ（光る床）" NAME_EN("Elevator Key A(Lit Floor)"), "aji_02", "w_bero", NULL},
    {0, 1, 0, 363, "STG7_10", "エレベータ始動" NAME_EN("Elevator Activated"), "aji_01", "e_bero_1", stg7_10_init},
    {0, 1, 0, 364, "STG7_11", "今日のあいことば" NAME_EN("Password for Today"), "aji_09", "s_bero", stg7_11_init},
    {0, 1, 0, 365, "STG7_12", "合言葉入力" NAME_EN("Password Entry"), "aji_01", "w_bero_2", NULL},
    {0, 1, 0, 366, "STG7_13", "クイズクリア" NAME_EN("Quiz Cleared"), "aji_04", "w_bero", NULL},
    {0, 1, 0, 367, "STG7_14", "エレベータキーＢ" NAME_EN("Elevator Key B"), "aji_04", "w_bero", NULL},
    {0, 1, 0, 368, "STG7_15", "エレベータ始動２" NAME_EN("Elevator Activated 2"), "aji_01", "n_bero_4", stg7_15_init},
    {0, 1, 0, 368, "STG7_15", "ルームキーＡ" NAME_EN("Room Key A"), "aji_05", "e_bero", stg7_15_room_A_init},
    {0, 1, 0, 368, "STG7_15", "ルームキーＢ" NAME_EN("Room Key B"), "aji_12", "e_bero", stg7_15_room_B_init},
    {0, 1, 0, 368, "STG7_15", "ルームキーＣ" NAME_EN("Room Key C"), "aji_10", "w_bero", stg7_15_room_C_init},
    {0, 1, 0, 369, "STG7_16", "工場のドア" NAME_EN("Factory Door"), "aji_18", "w_bero", stg7_16_init},
    {0, 1, 0, 370, "STG7_17", "ベルトコンベア" NAME_EN("Conveyor Belt"), "aji_13", "w_bero", stg7_17_init},
    {0, 1, 0, 371, "STG7_18", "オフスクリーン階段" NAME_EN("Off-screen Stairs"), "aji_13", "w_bero", NULL},
    {0, 1, 0, 372, "STG7_19", "ボス部屋への扉" NAME_EN("The Door to Lord Crump"), "aji_13", "w_bero", stg7_19_init},
    {0, 1, 0, 373, "STG7_20", "ボスを倒す" NAME_EN("Lord Crump Defeated"), "aji_14", "w_bero", stg7_20_init},
    {0, 0, 0, 374, "STG7_25_00", "クッパ編：ピカリーヒルズ" NAME_EN("Bowser: Poshley Heights"), "pik_01", NULL, stg7_25_00_init},
    {0, 0, 0, 375, "STG7_25_00_1", "クッパ編：神殿へ" NAME_EN("Bowser: Poshley Sanctum"), "pik_01", NULL, NULL},
    {0, 0, 0, 375, "STG7_25_00_1", "クッパ編：スターストーンゲット！？" NAME_EN("Bowser: Crystal Star"), "pik_02", NULL, NULL},
    {0, 1, 0, 376, "STG7_25", "最後のメール" NAME_EN("The Last Mail"), "aji_18", "e_bero", NULL},
    {0, 1, 0, 377, "STG7_26", "アジト爆発" NAME_EN("The Hideout Explodes"), "aji_07", "s_bero", NULL},
    {0, 1, 0, 378, "STG7_50", "クリハカセと会う" NAME_EN("A Waiting Prof. Frankly "), "tik_00", NULL, stg7_50_init},
    {0, 6, 0, 379, "STG7_51", "大きな扉ステージ７後" NAME_EN("Thousand-Year Door"), "tik_05", "e_bero", stg7_51_init},
    {0, 6, 0, 380, "STG7_52", "魔方陣" NAME_EN("The Pedestal"), "tik_05", "e_bero", stg7_52_init},
    {0, 6, 0, 381, "STG7_END", "ラストダンジョンへ" NAME_EN("To the Last Dungeon"), "tik_05", "e_bero", NULL}
};

EventData stage8_evt[] = {
    {0, 1, 0, 382, "STG8_00", "スタート" NAME_EN("Start"), "sys_01", "stg_8", NULL},
    {0, 1, 0, 383, "STG8_01", "１０００体カロン" NAME_EN("1000 Dry Bones"), "las_05", "w_bero", NULL},
    {0, 1, 0, 384, "STG8_01_01", "かぎ" NAME_EN("Key"), "las_05", "w_bero", NULL},
    {0, 1, 0, 385, "STG8_01_02", "じょうまえ" NAME_EN("Lock"), "las_05", "w_bero", stg8_01_02_init},
    {0, 1, 0, 386, "STG8_02", "無限回廊" NAME_EN("Follow the Lit Torches"), "las_08", "w_bero_1", stg8_02_init},
    {0, 1, 0, 387, "STG8_03", "女王の庭" NAME_EN("The Palace Garden"), "las_09", "w_bero", NULL},
    {0, 1, 0, 388, "STG8_04", "ボンババ" NAME_EN("Gloomtail"), "las_26", "w_bero", NULL},
    {0, 1, 0, 389, "STG8_05", "鍵穴出現" NAME_EN("Star Key Slot"), "las_10", "s_bero", stg8_05_init},
    {0, 1, 0, 390, "STG8_06", "天球儀起動" NAME_EN("Moving Planets"), "las_10", "tenkyugi_evt", stg8_06_init},
    {0, 1, 0, 391, "STG8_07", "マジョリン御一行" NAME_EN("Beldam Confrontation"), "las_09", "majyorin_evt", NULL},
    {0, 1, 0, 392, "STG8_08", "壁裏通路" NAME_EN("The New Path"), "las_19", "w_bero", NULL},
    {0, 1, 0, 393, "STG8_09", "鍵" NAME_EN("Key"), "las_22", "s_bero", NULL},
    {0, 1, 0, 394, "STG8_10", "錠前" NAME_EN("Lock"), "las_22", "s_bero", stg8_10_init},
    {0, 1, 0, 395, "STG8_11", "星の仕掛け" NAME_EN("Star Puzzle"), "las_20", "e_bero", stg8_11_init},
    {0, 1, 0, 396, "STG8_12", "ブロック" NAME_EN("Large Stone Block"), "las_24", "w_bero", NULL},
    {0, 1, 0, 397, "STG8_13", "鍵" NAME_EN("Key"), "las_24", "s_bero_2", stg8_13_init},
    {0, 1, 0, 398, "STG8_14", "錠前" NAME_EN("Lock"), "las_25", "w_bero", stg8_14_init},
    {0, 1, 0, 399, "STG8_14_01", "玉座の間" NAME_EN("Queen's Chamber"), "las_28", "w_bero", stg8_15_init},
    {0, 1, 0, 400, "STG8_15", "シュリョー" NAME_EN("Grodus"), "las_28", "w_bero", NULL},
    {0, 1, 0, 400, "STG8_15", "クッパ" NAME_EN("Bowser"), "las_28", "koopa_evt", NULL},
    {0, 1, 0, 401, "STG8_16", "女王の墓所" NAME_EN("The Return of the Queen"), "las_29", "w_bero", NULL},
    {0, 1, 0, 401, "STG8_16", "世界が闇に覆われる１" NAME_EN("End of the World 1"), "gor_01", "sekai_yami1", NULL},
    {0, 1, 0, 401, "STG8_16", "カゲの女王" NAME_EN("The Shadow Queen"), "las_29", "sekai_yami1", NULL},
    {0, 1, 0, 401, "STG8_16", "世界が闇に覆われる２" NAME_EN("End of the World 2"), "usu_00", "sekai_yami2", NULL},
    {0, 1, 0, 401, "STG8_16", "ブラックピーチ" NAME_EN("Shadow Peach"), "las_29", "sekai_yami2", NULL},
    {0, 1, 0, 401, "STG8_16", "スターストーンとびだす" NAME_EN("The Crystal Stars"), "las_29", "starstone", NULL},
    {0, 1, 0, 401, "STG8_16", "みんなのこえ１" NAME_EN("Words of Support 1"), "gor_01", "minnnanokoe", NULL},
    {0, 1, 0, 401, "STG8_16", "みんなのこえ２" NAME_EN("Words of Support 2"), "usu_00", "minnnanokoe", NULL},
    {0, 1, 0, 401, "STG8_16", "みんなのこえ３" NAME_EN("Words of Support 3"), "las_29", "minnnanokoe", NULL},
    {0, 1, 0, 402, "STG8_END", "バトル後" NAME_EN("After the Battle"), "las_29", "owari", NULL}
};

EventData stage9_evt[] = {
    {0, 0, 0, 403, "STG9_00", "エピローグ" NAME_EN("Epilogue"), "gor_11", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "ゴロツキ港" NAME_EN("Rogueport Dock"), "gor_00", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "海上" NAME_EN("Sail Away"), "muj_21", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "マリオの家" NAME_EN("Mario's House"), "aaa_00", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "クリハカセ" NAME_EN("Professor Frankly"), "gor_02", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "クリスチーヌ" NAME_EN("Goombella"), "hei_00", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "ノコタロウ" NAME_EN("Koops"), "nok_01", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "チビヨッシー" NAME_EN("Mini-Yoshi"), "tou_08", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "ビビアン" NAME_EN("Vivian"), "usu_00", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "サンダース" NAME_EN("Bobbery"), "muj_20", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "チュチュリーナ" NAME_EN("Ms. Mowz"), "gor_01", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "メガバッテン" NAME_EN("X-Nauts"), "pik_04", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "月" NAME_EN("Moon"), "tik_00", NULL, NULL},
    {0, 0, 0, 403, "STG9_00", "クリスチーヌ２" NAME_EN("Goombella 2"), "gor_00", "epilogue2", NULL},
    {0, 0, 0, 403, "STG9_00", "マリオの家２" NAME_EN("Mario's House 2"), "aaa_00", "epilogue2", NULL},
    {0, 1, 0, 404, "STG9_01", "スタッフロール" NAME_EN("End Credits"), "end_00", "", NULL},
    {0, 1, 0, 405, "STG9_02", "クリア後（最初）" NAME_EN("Return to Rogueport"), "gor_00", "", N_stg9_02_init},
    {0, 1, 0, 406, "STG9_END", "クリア後" NAME_EN("Rogueport"), "gor_00", "n_bero", NULL}
};

EventStage event_dt[] = {
    {"オープニング" NAME_EN("Opening"), stage0_evt, sizeof(stage0_evt) / sizeof(EventData)},
    {"ステージ１" NAME_EN("Stage 1"), stage1_evt, sizeof(stage1_evt) / sizeof(EventData)},
    {"ステージ２" NAME_EN("Stage 2"), stage2_evt, sizeof(stage2_evt) / sizeof(EventData)},
    {"ステージ３" NAME_EN("Stage 3"), stage3_evt, sizeof(stage3_evt) / sizeof(EventData)},
    {"ステージ４" NAME_EN("Stage 4"), stage4_evt, sizeof(stage4_evt) / sizeof(EventData)},
    {"ステージ５" NAME_EN("Stage 5"), stage5_evt, sizeof(stage5_evt) / sizeof(EventData)},
    {"ステージ６" NAME_EN("Stage 6"), stage6_evt, sizeof(stage6_evt) / sizeof(EventData)},
    {"ステージ７" NAME_EN("Stage 7"), stage7_evt, sizeof(stage7_evt) / sizeof(EventData)},
    {"ステージ８" NAME_EN("Stage 8"), stage8_evt, sizeof(stage8_evt) / sizeof(EventData)},
    {"エンディング" NAME_EN("Ending"), stage9_evt, sizeof(stage9_evt) / sizeof(EventData)}
};

// Stage 0 / Prologue -----------------------------------------------------------------
void stg0_00_init(void) {
    pouchGetPtr(); //leftover from development, return value never used for pouchInit
    pouchInit();
    pouchGetItem(ITEM_BOOTS);
    pouchGetItem(ITEM_MAGICAL_MAP);
    pouchGetItem(ITEM_HAMMER);
    pouchSetCoin(100);
    pouchReviseMarioParam();
    badgeShop_init();
    yuugijou_init();
    johoya_init();
    gp->retraceLocalTime = 0;
    marioSetCharMode(0);
    statusWinForceUpdate();
}

void stg0_05_init(void) {
    swSet(0x49B);
}

void stg0_08_init(void) {
    partyJoin(PARTNER_GOOMBELLA);
}

void stg0_11_init(void) {
    swSet(0x4B7);
}

void stg0_12_02_init(void) {
    swSet(0x537);
}

void stg0_13_init00(void) {
    pouchGetItem(ITEM_PLANE_CURSE);
    swSet(0x539);
}

void stg0_16_init(void) {
    pouchGetStarStone(0);
}

// Stage 1 ----------------------------------------------------------------------------
void stg1_01a_init(void) {
    swSet(0x6F7);
}

void stg1_03_init(void) {
    swSet(7);
    pouchReceiveMail(0x10);
    pouchReceiveMail(0x1A);
}

void stg1_06_01_init(void) {
    pouchGetItem(ITEM_MOON_STONE);
    swSet(0x6EF);
    pouchGetItem(ITEM_SUN_STONE);
    swSet(0x6EE);
}

void stg1_08_init(void) {
    partyJoin(PARTNER_KOOPS);
    swSet(0x6F5);
    swSet(0x6F6);
}

void stg1_09_init(void) {
    pouchRemoveItem(ITEM_MOON_STONE);
    pouchRemoveItem(ITEM_SUN_STONE);
}

void stg1_09_02_init(void) {
    //stubbed
}

void stg1_10_init(void) {
    //stubbed
}

void stg1_12_02_init(void) {
    pouchGetItem(ITEM_CASTLE_KEY_0x0D);
}

void stg1_13_init(void) {
    pouchRemoveItem(ITEM_CASTLE_KEY_0x0D);
}

void stg1_13_01_init(void) {
    pouchGetItem(ITEM_BLACK_KEY_0x22);
}

void stg1_13_02_init(void) {
    pouchRemoveItem(ITEM_BLACK_KEY_0x22);
    pouchGetItem(ITEM_PAPER_CURSE);
}

void stg1_13_03_init(void) {
    pouchGetItem(ITEM_CASTLE_KEY_0x0C);
}

void stg1_14_init(void) {
    pouchRemoveItem(ITEM_CASTLE_KEY_0x0C);
}

void stg1_15_02_init(void) {
    pouchGetItem(ITEM_CASTLE_KEY_0x0F);
}

void stg1_15_03_init(void) {
    pouchRemoveItem(ITEM_CASTLE_KEY_0x0F);
}

void stg1_15_04_init(void) {
    pouchGetItem(ITEM_CASTLE_KEY_0x0E);
}

void stg1_16_init00(void) {
    pouchRemoveItem(ITEM_CASTLE_KEY_0x0E);
    pouchGetItem(ITEM_ATTACK_FX_R);
    pouchGetItem(ITEM_POWER_BOUNCE);
    pouchGetItem(ITEM_MULTIBOUNCE);
    pouchGetItem(ITEM_POWER_SMASH);
    pouchEquipBadgeID(ITEM_ATTACK_FX_R);
    pouchReviseMarioParam();
}

void stg1_16_init01(void) {
    swSet(0x5DA);
}

void stg1_17_init(void) {
    pouchGetItem(ITEM_DIAMOND_STAR);
    pouchGetStarStone(1);
}

void stg1_19_init(void) {
    swSet(0x1067);
    swSet(0x1068);
    swSet(0x1069);
}

void stg1_20_init(void) {
    marioSetCharMode(0);
}

void stg1_30_init(void) {
    pouchReceiveMail(0);
    pouchOpenMail(0);
}

void stg1_30_init01(void) {
    swSet(0x4A4);
    swSet(0x4A3);
}

void stg1_35_init01(void) {
    swSet(0x536);
}

void stg2_00_init(void) {
    PouchData* pouch;

    pouch = pouchGetPtr();
    pouchSetMaxHP(15);
    pouchSetMaxFP(15);
    pouch->availableBP = 6;
    pouch->maximumBP = 6;
    pouch->level = 3;
}

void stg2_03_03_init(void) {
    //stubbed
}

void stg2_09_init(void) {
    pouchGetItem(ITEM_NECKLACE);
}

void stg2_10_init(void) {
    pouchRemoveItem(ITEM_NECKLACE);
    partyJoin(PARTNER_FLURRIE);
    swSet(0x11);
    pouchReceiveMail(0x1B);
}

void stg2_11_init(void) {
    swSet(0xB0A);
}

void stg2_12_01_init(void) {
    swSet(0xB3C);
    swByteSet(0x28B, 0);
}

void stg2_14_init(void) {
    pouchGetItem(ITEM_RED_KEY);
    swSet(0xB17);
}

void stg2_15_init(void) {
    pouchRemoveItem(ITEM_RED_KEY);
    swSet(0xB10);
    swSet(0xB11);
}

void stg2_16_init00(void) {
    swSet(0xB3F);
    swByteSet(0x28B, 9);
    pouchGetItem(ITEM_PUNI_ORB);
}

void stg2_16_init01(void) {
    swSet(0xB0D);
}

void stg2_18_init01(void) {
    swSet(0xB3E);
}

void stg2_18_init02(void) {
    swSet(0xB1F);
    swSet(0xB21);
}

void stg2_19_init(void) {
    swSet(0xB24);
    pouchGetItem(ITEM_BLUE_KEY);
}

void stg2_20_init(void) {
    swSet(0xB14);
}

void stg2_21_init00(void) {
    pouchRemoveItem(ITEM_BLUE_KEY);
    swSet(0xB12);
    swSet(0xB13);
    swSet(0xB3D);
    swClear(0xB14);
    swByteSet(0x28B, 0x62);
}

void stg2_21_init02(void) {
    swSet(0xB27);
    swSet(0xB29);
}

void stg2_21_01_init(void) {
    swByteSet(0x28C, 0xF6);
    swByteSet(0x28D, 0xFF);
    swByteSet(0x28E, 0xFF);
    swByteSet(0x28F, 0xFF);
    swByteSet(0x290, 0xFF);
    swByteSet(0x291, 0xFF);
    swByteSet(0x292, 0xFF);
    swByteSet(0x293, 0xFF);
    swByteSet(0x294, 0xFF);
    swByteSet(0x295, 0xFF);
    swByteSet(0x296, 0xFF);
    swByteSet(0x297, 0xFF);
    swByteSet(0x298, 0x7F);
}

void stg2_21_02_init(void) {
    swSet(0xB38);
}

void stg2_21_03_init(void) {
    swSet(0xB22);
}

void stg2_21_04_init(void) {
    pouchGetItem(ITEM_SUPER_BOOTS);
}

void stg2_21_06_init00(void) {
    swByteSet(0x28C, 0);
    swByteSet(0x28D, 0);
    swByteSet(0x28E, 0);
    swByteSet(0x28F, 0);
    swByteSet(0x290, 0);
    swByteSet(0x291, 0);
    swByteSet(0x292, 0);
    swByteSet(0x293, 0);
    swByteSet(0x294, 0);
    swByteSet(0x295, 0);
    swByteSet(0x296, 0);
    swByteSet(0x297, 0);
    swByteSet(0x298, 0);
}

void stg2_21_06_init01(void) {
    swSet(0xB33);
}

void stg2_23_init(void) {
    swSet(0xB30);
}

void stg2_23_01_init(void) {
    swClear(0xB30);
    swSet(0xB40);
}

void stg2_25_init(void) {
    swClear(0xB40);
    pouchGetItem(ITEM_EMERALD_STAR);
    pouchGetStarStone(2);
}

void stg2_25_02_init(void) {
    swSet(0x106A);
}

void stg2_26_init(void) {
    marioSetCharMode(0);
}

void stg2_40_init(void) {
    swSet(0xB44);
    pouchReceiveMail(1);
    pouchOpenMail(1);
}

void stg2_44_init(void) {
    swSet(0x4CE);
}

void stg2_47_init(void) {
    pouchGetItem(ITEM_BLIMP_TICKET);
}

void stg2_end_init(void) {
    swSet(8);
    pouchReceiveMail(0x11);
}

void stg3_06_init(void) {
    swSet(0x954);
    swByteSet(0x1F5, 0x13);
    swByteSet(0x1F6, 0x2B);
    swByteSet(0x1F7, 1);
    swByteSet(0x1F8, 1);
}

void stg3_07_init__9(void) {
    int i;

    for (i = 0; i < 1; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swClear(0x954);
    swSet(0x958);
    swSet(0x95B);
    swSet(0x95C);
    swSet(0x9CD);
    swSet(0x98B);
}

void stg3_07_init__8(void) {
    int i;

    for (i = 0; i < 2; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swClear(0x954);
    swSet(0x966);
}

void stg3_07_init__7(void) {
    int i;

    for (i = 0; i < 3; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x967);
}

void stg3_07_init__6(void) {
    int i;

    for (i = 0; i < 4; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x968);
}

void stg3_10_init__5(void) {
    int i;

    for (i = 0; i < 5; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x969);
}

void stg3_12_init__4(void) {
    u8 copytobyte[8]; //stack+8/12
    int i;

    for (i = 0; i < 6; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x96A);
    swSet(0x957);
    *(u64*)copytobyte = (u64)gp->retraceLocalTime;
    for (i = 0; i < 8; i++) {
        swByteSet(0x1FC + i, copytobyte[i]);
    }
    swSet(0x12);
    pouchReceiveMail(0x1C);
}

void stg3_12_init__3(void) {
    int i;

    for (i = 0; i < 7; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x96B);
}

void stg3_12_init__2(void) {
    int i;

    for (i = 0; i < 8; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x96C);
}

void stg3_12_init__1(void) {
    int i;

    for (i = 0; i < 9; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x96D);
}

void stg3_12_2_init(void) {
    yoshiSetColor();
    pouchSetYoshiName((char*)msgSearch("name_party3"));
    partyJoin(PARTNER_YOSHI);
    swSet(0x95D);
}

void stg3_20_init_10(void) {
    int i;

    for (i = 0; i < 10; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x96E);
    swSet(0x95E);
    pouchReceiveMail(6);
    pouchOpenMail(6);
}

void stg3_20_init_9(void) {
    int i;

    for (i = 0; i < 11; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x96F);
}

void stg3_20_init_8(void) {
    int i;

    for (i = 0; i < 12; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x970);
}

void stg3_20_01_init(void) {
    pouchReceiveMail(7);
    pouchOpenMail(7);
}

void stg3_20_02_init(void) {
    pouchGetItem(ITEM_SUPER_HAMMER);
    pouchReceiveMail(8);
    pouchOpenMail(8);
}

void stg3_20_init_7(void) {
    int i;

    for (i = 0; i < 13; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x971);
    swSet(0x994);
    swSet(0x9D5);
    pouchReceiveMail(9);
    pouchOpenMail(9);
}

void stg3_20_init_6(void) {
    int i;

    for (i = 0; i < 14; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x972);
    swClear(0x994);
}

void stg3_20_init_5(void) {
    int i;

    for (i = 0; i < 15; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x973);
}

void stg3_20_init_4(void) {
    int i;

    for (i = 0; i < 16; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x974);
    swSet(0x995);
}

void stg3_20_06_init(void) {
    swClear(0x995);
    pouchReceiveMail(10);
    pouchOpenMail(10);
}

void stg3_20_07_00_init(void) {
    pouchGetItem(ITEM_STORAGE_KEY_0x12);
}

void stg3_20_08_init(void) {
    pouchReceiveMail(11);
    pouchOpenMail(11);
}

void stg3_20_09_init(void) {
    pouchReceiveMail(12);
    pouchOpenMail(12);
}

void stg3_20_12_init(void) {
    pouchRemoveItem(ITEM_STORAGE_KEY_0x12);
    swSet(0x979);
    swSet(0x97A);
    swSet(0x97B);
    swSet(0x97C);
    swSet(0x97D);
    swSet(0x97E);
    swSet(0x97F);
    swSet(0x980);
}

void stg3_20_init_3(void) {
    int i;

    for (i = 0; i < 17; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x975);
    pouchReceiveMail(13);
    pouchOpenMail(13);
}

void stg3_20_init_2(void) {
    int i;

    for (i = 0; i < 18; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x976);
    swSet(0x990);
    swSet(0x993);
}

void stg3_20_init_1(void) {
    int i;

    for (i = 0; i < 19; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x977);
    swSet(0x95A);
    swSet(0x996);
    pouchReceiveMail(14);
    pouchOpenMail(14);
}

void stg3_20_13_init(void) {
    swClear(0x996);
}

void stg3_20_14_init(void) {
    pouchGetItem(ITEM_STORAGE_KEY_0x13);
    pouchReceiveMail(15);
    pouchOpenMail(15);
}

void stg3_20_15_init(void) {
    pouchRemoveItem(ITEM_STORAGE_KEY_0x13);
}

void stg3_21_init(void) {
    swSet(0x954);
    swByteSet(0x1F5, 0);
    swSet(0x94F);
    swByteSet(0x1F6, 6);
    swByteSet(0x1F7, 1);
    swByteSet(0x1F8, 1);
}

void stg3_30_init(void) {
    int i;

    for (i = 0; i < 20; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 20 - i);
    swSet(0x978);
    swClear(0x94F);
    swClear(0x954);
    swByteSet(0x1F5, 0);
    swSet(0x955);
    swSet(0x95F);
    swSet(0x960);
    swSet(0x9D1);
    swSet(0x9D2);
    swSet(0x9D3);
    swSet(0x9D4);
}

void stg3_31_00_init(void) {
    swClear(0x955);
    pouchGetItem(ITEM_CHAMPS_BELT);
    pouchReceiveMail(0x2B);
    pouchOpenMail(0x2B);
}

void stg3_31_init(void) {
    swClear(0x955);
}

void stg3_32_init(void) {
    swSet(0x961);
}

void stg3_33_init(void) {
    pouchGetItem(ITEM_GOLD_STAR);
    pouchGetStarStone(3);
}

void stg3_33_03_init(void) {
    swSet(0x106B);
    swSet(0x106C);
}

void stg3_33_04_init(void) {
    swClear(0x106C);
}

void stg3_34_init(void) {
    int i;

    swSet(0x962);
    swSet(0x963);
    marioSetCharMode(0);
    pouchRemoveItem(ITEM_CHAMPS_BELT);
    for (i = 0; i < 20; i++) {
        swClear(i + 0x9A1);
    }
    swClear(0x958);
    swClear(0x95F);
    swClear(0x960);
    swClear(0x94F);
    swClear(0x9E1);
    swClear(0x9E4);
    swClear(0x963);
}

void stg3_35_init(void) {
    swSet(0x9C0);
}

void stg3_50_init(void) {
    pouchReceiveMail(2);
    pouchOpenMail(2);
}

void stg3_52_init(void) {
    swSet(0x53D);
    swSet(9);
    pouchReceiveMail(0x12);
}

void stg3_end_init(void) {
    //stubbed
}

void stage4_00_init(void) {
    //stubbed
}

void stage4_04_init(void) {
    pouchGetItem(ITEM_SHOP_KEY);
}

void stage4_04_01_init(void) {
    pouchRemoveItem(ITEM_SHOP_KEY);
    swSet(0x82B);
    pouchGetItem(ITEM_BLACK_KEY_0x23);
}

void stage4_05_init(void) {
    pouchRemoveItem(ITEM_BLACK_KEY_0x23);
    pouchGetItem(ITEM_TUBE_CURSE);
    swSet(0x13);
    pouchReceiveMail(0x1D);
}

void stage4_09_05_init(void) {
    pouchGetItem(ITEM_STEEPLE_KEY_0x16);
}

void stage4_10_init(void) {
    pouchRemoveItem(ITEM_STEEPLE_KEY_0x16);
}

void stg4_11_init(void) {
    partyLeft(PARTNER_GOOMBELLA);
    partyLeft(PARTNER_KOOPS);
    partyLeft(PARTNER_FLURRIE);
    partyLeft(PARTNER_YOSHI);
}

void stg4_13_init(void) {
    pouchGetItem(ITEM_SUPERBOMBOMB);
}

void stg4_13_02_init(void) {
    pouchRemoveItem(ITEM_SUPERBOMBOMB);
    partyJoin(PARTNER_VIVIAN);
}

void stg4_13_03_init(void) {
    swSet(0x81B);
}

void stg4_14_init(void) {
    pouchGetItem(ITEM_THE_LETTER_P);
    swSet(0x81B);
    swSet(0x8B6);
    swSet(0x8B7);
}

void stg4_15_init(void) {
    pouchRemoveItem(ITEM_THE_LETTER_P);
}

void stg4_15_03_init(void) {
    partyJoin(PARTNER_GOOMBELLA);
    partyJoin(PARTNER_KOOPS);
    partyJoin(PARTNER_FLURRIE);
    partyJoin(PARTNER_YOSHI);
    pouchGetItem(ITEM_RUBY_STAR);
    pouchGetStarStone(4);
}

void stg4_16_init(void) {
    marioSetCharMode(0);
}

void stg4_30_init(void) {
    pouchReceiveMail(3);
    pouchOpenMail(3);
}

void stg4_35_init(void) {
    swSet(0x4C0);
    swSet(0x4C1);
}

void stg4_36_init(void) {
    pouchGetItem(ITEM_OLD_LETTER);
}

void stg4_37_init(void) {
    pouchRemoveItem(ITEM_OLD_LETTER);
}

void stg4_end_init(void) {
    swSet(0xA);
    pouchReceiveMail(0x13);
}

void stg5_12_init01(void) {
    swSet(0xC36);
}

void stg5_15_init01(void) {
    swSet(0xC3B);
}

void stg5_15_init02(void) {
    swSet(0xC39);
}

void stg5_17_init(void) {
    pouchGetItem(ITEM_COCONUT);
}

void stg5_18_init(void) {
    pouchGetItem(ITEM_CHUCKOLA_COLA);
    pouchRemoveItem(ITEM_COCONUT);
}

void stg5_18_init_02(void) {
    pouchRemoveItem(ITEM_CHUCKOLA_COLA);
    partyJoin(PARTNER_BOBBERY);
}

void stg5_19_init00(void) {
    //stubbed
}

void stg5_19_init01(void) {
    pouchGetItem(ITEM_SKULL_GEM);
}

void stg5_19_init02(void) {
    pouchRemoveItem(ITEM_SKULL_GEM);
    swSet(0xC43);
    swSet(0xC4D);
}

void stg5_19_01_init(void) {
    //stubbed
}

void stg5_21_init00(void) {
    swSet(0xBA2);
}

void stg5_21_init01(void) {
    swSet(0xBA1);
    swSet(0xBA3);
}

void stg5_21_init03(void) {
    swSet(0xBA5);
}

void stg5_21_01_init00(void) {
    pouchGetItem(ITEM_BLACK_KEY_0x24);
}

void stg5_22_init00(void) {
    pouchRemoveItem(ITEM_BLACK_KEY_0x24);
    pouchGetItem(ITEM_BOAT_CURSE);
}

void stg5_22_init01(void) {
    pouchGetItem(ITEM_GATE_HANDLE);
}

void stg5_29_init(void) {
    pouchRemoveItem(ITEM_GATE_HANDLE);
}

void stg5_30_init01(void) {
    swSet(0xBA6);
}

void stg5_30_init02(void) {
    swSet(0xBA7);
    swSet(0xBA8);
    swSet(0xBB0);
}

void stg5_43_init(void) {
    pouchGetItem(ITEM_SAPPHIRE_STAR);
    pouchGetStarStone(5);
}

void stg5_47_init(void) {
    swSet(0x106F);
    swSet(0x1070);
    swSet(0x1071);
}

void stg5_58_init(void) {
    marioSetCharMode(0);
}

void stg5_59_01_init(void) {
    swSet(0x49C);
}

void stg5_60_init(void) {
    pouchReceiveMail(4);
    pouchOpenMail(4);
    swSet(0x14);
    pouchReceiveMail(0x1E);
    swClear(0x49C);
}

void stg5_62_init(void) {
    swSet(0x4CF);
}

void stg5_63_init(void) {
    swSet(0xC35);
}

void stg5_64_init(void) {
    swClear(0xC35);
}

void stg5_64_01_init(void) {
    swSet(0xC47);
    pouchGetItem(ITEM_WEDDING_RING);
}

void stg5_64_02_init(void) {
    pouchRemoveItem(ITEM_WEDDING_RING);
}

void stg5_64_03_init(void) {
    swSet(0x49A);
}

void stg5_65_init(void) {
    swClear(0x49A);
    swSet(0x4CF);
}

void stg5_66_init(void) {
    pouchGetItem(ITEM_TRAIN_TICKET);
    swSet(0x4CF);
}

void stg6_00_init(void) {
    swSet(0xB);
    pouchReceiveMail(0x14);
}

void stg6_06_init(void) {
    pouchGetItem(ITEM_GALLEY_POT);
}

void stg6_12_init(void) {
    pouchGetItem(ITEM_AUTOGRAPH);
}

void stg6_13_init(void) {
    pouchRemoveItem(ITEM_AUTOGRAPH);
}

void stg6_18_init(void) {
    pouchGetItem(ITEM_RAGGED_DIARY);
}

void stg6_19_init(void) {
    pouchRemoveItem(ITEM_RAGGED_DIARY);
    pouchGetItem(ITEM_BLANKET);
}

void stg6_20_init(void) {
    pouchRemoveItem(ITEM_BLANKET);
}

void stg6_22_init(void) {
    swSet(0x15);
    pouchReceiveMail(0x1F);
}

void stg6_24_init(void) {
    pouchGetItem(ITEM_VITAL_PAPER);
}

void stg6_27_init(void) {
    pouchRemoveItem(ITEM_VITAL_PAPER);
    pouchGetItem(ITEM_GOLD_RING);
    pouchGetItem(ITEM_SHELL_EARRINGS);
}

void stg6_29_init(void) {
    pouchGetItem(ITEM_STEEPLE_KEY_0x18);
}

void stg6_30_init(void) {
    pouchRemoveItem(ITEM_STEEPLE_KEY_0x18);
}

void stg6_32_init(void) {
    pouchGetItem(ITEM_STEEPLE_KEY_0x19);
}

void stg6_33_init(void) {
    pouchRemoveItem(ITEM_STEEPLE_KEY_0x19);
}

void stg6_34_init(void) {
    swSet(0xE95);
    swSet(0xE96);
    swSet(0xE97);
    swSet(0xE98);
    swSet(0xE99);
    swSet(0xE9A);
    swSet(0xE9B);
    swSet(0xE9C);
    swSet(0xE9D);
    swSet(0xE9E);
    swSet(0xE9F);
}

void stg6_37_init(void) {
    pouchGetItem(ITEM_ULTRA_BOOTS);
}

void stg6_38_init(void) {
    pouchGetItem(ITEM_ELEVATOR_KEY_0x1A);
}

void stg6_39_init(void) {
    pouchRemoveItem(ITEM_ELEVATOR_KEY_0x1A);
}

void stg6_44_init(void) {
    swSet(0xD);
    pouchReceiveMail(0x16);
}

void stg6_47_00_init(void) {
    swSet(0xC);
    pouchReceiveMail(0x15);
}

void stg6_54_init(void) {
    pouchGetItem(ITEM_GARNET_STAR);
    pouchGetStarStone(6);
}

void stg6_56_init(void) {
    marioSetCharMode(0);
}

void stg6_70_init(void) {
    pouchReceiveMail(5);
    pouchOpenMail(5);
}

void stg6_71_01_init(void) {
    swSet(0xE);
    pouchReceiveMail(0x17);
    gp->retraceLocalTime += (OSTime)OS_TIMER_CLOCK * 180;
}

void stg6_72_init(void) {
    //stubbed
}

void stg6_end_init01(void) {
    pouchGetItem(ITEM_ULTRA_HAMMER);
    swSet(0x4AB);
}

void stg6_end_init02(void) {
    swSet(0x53C);
}

void stg7_01_init(void) {
    swSet(0xF2C);
}

void stg7_03_white_0(void) {
    swSet(0x16);
    pouchReceiveMail(0x20);
    swSet(0xF22);
    pouchGetItem(ITEM_GOLDBOB_GUIDE);
}

void stg7_03_white_1(void) {
    swSet(0xF25);
}

void stg7_03_white_2(void) {
    swSet(0xF26);
}

void stg7_03_white_3(void) {
    swSet(0xF27);
}

void stg7_03_white_4(void) {
    swSet(0xF31);
}

void stg7_03_white_5(void) {
    swSet(0xF28);
    swSet(0xF);
    pouchReceiveMail(0x18);
}

void stg7_04_init(void) {
    swSet(0xF24);
}

void stg7_06_init(void) {
    pouchRemoveItem(ITEM_GOLDBOB_GUIDE);
}

void stg7_10_init(void) {
    pouchGetItem(ITEM_ELEVATOR_KEY_0x1B);
}

void stg7_11_init(void) {
    pouchRemoveItem(ITEM_ELEVATOR_KEY_0x1B);
}

void stg7_15_init(void) {
    pouchGetItem(ITEM_ELEVATOR_KEY_0x1C);
}

void stg7_15_room_A_init(void) {
    pouchRemoveItem(ITEM_ELEVATOR_KEY_0x1C);
}

void stg7_15_room_B_init(void) {
    pouchGetItem(ITEM_CARD_KEY_0x1D);
    swSet(0x1056);
}

void stg7_15_room_C_init(void) {
    pouchGetItem(ITEM_CARD_KEY_0x1E);
    swSet(0x1057);
}

void stg7_16_init(void) {
    pouchGetItem(ITEM_CARD_KEY_0x1F);
    swSet(0x1054);
    swSet(0x1063);
}

void stg7_17_init(void) {
    pouchRemoveItem(ITEM_CARD_KEY_0x1D);
    pouchRemoveItem(ITEM_CARD_KEY_0x1E);
    pouchRemoveItem(ITEM_CARD_KEY_0x1F);
    swSet(0x105F);
    swSet(0x1060);
    swSet(0x1061);
}

void stg7_19_init(void) {
    swSet(0x1055);
    pouchGetItem(ITEM_CARD_KEY_0x20);
}

void stg7_20_init(void) {
    pouchRemoveItem(ITEM_CARD_KEY_0x20);
}

void stg7_25_00_init(void) {
    pouchGetItem(ITEM_CRYSTAL_STAR);
    pouchGetStarStone(7);
}

void stg7_50_init(void) {
    swSet(0x104F);
}

void stg7_51_init(void) {
    swSet(0x10);
    pouchReceiveMail(0x19);
    pouchReceiveMail(0x21);
    swClear(0x104F);
    swSet(0x52F);
}

void stg7_52_init(void) {
    //stubbed
}

void stg8_01_02_init(void) {
    pouchGetItem(ITEM_PALACE_KEY_0x2E);
}

void stg8_02_init(void) {
    pouchRemoveItem(ITEM_PALACE_KEY_0x2E);
}

void stg8_05_init(void) {
    swSet(0x111C);
    swSet(0x10E6);
    swSet(0x10E7);
    swSet(0x10E8);
    swSet(0x10E9);
    swSet(0x1109);
    pouchGetItem(ITEM_STAR_KEY);
}

void stg8_06_init(void) {
    pouchRemoveItem(ITEM_STAR_KEY);
    swSet(0x10EA);
    swSet(0x10EB);
    swSet(0x10EC);
    swSet(0x10ED);
    swSet(0x10EE);
    swSet(0x10EF);
    swSet(0x10F0);
    swSet(0x10F1);
    swSet(0x10F2);
    evtSetValue(0, GSW(0x462), 8);
    swSet(0x10F3);
    swSet(0x10F4);
    swSet(0x10F5);
    swSet(0x10F6);
    swSet(0x10F7);
    swSet(0x10F8);
    swSet(0x10F9);
    swSet(0x10FA);
    swSet(0x10FB);
    swSet(0x10FC);
    swSet(0x10FD);
    swSet(0x10FE);
    swSet(0x10FF);
    swSet(0x1100);
    evtSetValue(0, GSW(0x463), 4);
    swSet(0x110A);
    swSet(0x110B);
    swSet(0x110C);
    swSet(0x110D);
    swSet(0x110E);
    swSet(0x110F);
    swSet(0x1110);
    swSet(0x1111);
}

void stg8_10_init(void) {
    pouchGetItem(ITEM_PALACE_KEY_0x2F);
}

void stg8_11_init(void) {
    pouchRemoveItem(ITEM_PALACE_KEY_0x2F);
    swSet(0x1103);
}

void stg8_13_init(void) {
    swSet(0x1104);
    swSet(0x1105);
    swSet(0x1106);
}

void stg8_14_init(void) {
    pouchGetItem(ITEM_PALACE_KEY_0x30);
    swSet(0x1112);
}

void stg8_15_init(void) {
    pouchRemoveItem(ITEM_PALACE_KEY_0x30);
    swSet(0x1107);
}

void N_stg9_02_init(void) {
    swSet(0x150E);
    partyJoin(PARTNER_MSMOWZ);
}

EventStage* eventStgDtPtr(s32 stage) {
    return &event_dt[stage];
}

s32 eventStgNum(void) {
    return sizeof(event_dt) / sizeof(EventStage);
}
