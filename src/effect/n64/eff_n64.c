// Shared support for the Paper Mario 64 effects: texture loading, the effect set table and vertex/triangle helpers.
// The early demos also store a description and a pattern-name list (per type) for each effect.

#include "version.h"
#include "eff/eff_n64.h"
#include "driver/arcdrv.h"
#include "manager/dvdmgr.h"

#if VERSION >= VERSION_US
#include "mario/mariost.h"
#endif

#include "memory.h"
#include "system.h"
#include <stdlib.h>
#include <string.h>
#include "libc_decls.h"

//.data
#if VERSION <= VERSION_DEMO_JP
static const char* kemuri_patname[] = {"マリオ:KAMECSMALL", "ガレキ:KAMECBIG", "-----", "マリオどっしん:KAMECBIG", "Blockどっしん::KAMECBIG", "サンボ", "マミー", NULL};
static const char* kemuri_dash_patname[] = {"Mairo:KAMECSMALL", "KAMECBIG", "PINKYBOM", NULL};
static const char* stardust_patname[] = {"ぱぁーっとでる", "すくなめ", "おおめ", "らっかすくなめ", "らっかおおめ"};
static const char* magic2_patname[] = {"カメックしゅうちゅう", "カメックわっか", NULL};
static const char* hokori_patname[] = {"トリデほこり", "ほこりその２", "ほこりひとつ", NULL};
static const char* kemu_pinky_patname[] = {"LV1", "LV2", "LV3", "LV1炎", "LV2炎", "LV3炎", NULL};
static const char* small_star_patname[] = {"しょきかいてんなし", "しょきかいてんあり", "らっかあり", "オウギ", "パーティ", NULL};
static const char* pinky_patname[] = {"LV1", "LV2", "LV3", "LV1炎", "LV2炎", "LV3炎", NULL};
static const char* wangflush_patname[] = {"ワンワン", "<みしよう>", NULL};
static const char* hammerkemu_patname[] = {"せいこう", "ノーマル", NULL};
static const char* stardust2_patname[] = {"<みしよう>", "HPかいふく", "HPかいふく2", "HP,FPブロック", "ピーチへんしん", "ねがう", "HP,FPブロック(Z)", NULL};
static const char* ripple_patname[] = {"a", "b", NULL};
static const char* eff_n64_users_unused[] = {"c", "d", "e", "f", "g", "h", "j", "k", NULL};
static const char* dogabon_kemuri_patname[] = {"あるき＆はしり", "ヒップアタック", "サンボ", "くもくもまじん", "新サンボ", "新サンボマミー", "ライト落下", NULL};
static const char* onpu_patname[] = {"じょうしょう", "フラワーさん", "ルイージしんぱいしてる", NULL};
static const char* sweat_patname[] = {"攻撃ミス時", "チビヨッシー", NULL};
static const char* stamp_patname[] = {"ねむらせ", "こんらん", "バトルビビアン３", "そのほか", NULL};
static const char* thruhammer_patname[] = {"かんつうハンマー", "かんつうハンマーしっぱい", NULL};
static const char* radiation_patname[] = {"-----", "つえのひかり", "つえのひかり２", "ノコブロスのわざ", NULL};
static const char* broken_barrier_patname[] = {"つえバリアこわれる", "ピーチへんしんとかれる", "逆再生", NULL};
static const char* flame_patname[] = {"タイトルデモ", "ほのおのてき", "クッパ", "ししょう", NULL};
static const char* sunlight_patname[] = {"右から", "左から", NULL};
static const char* release_patname[] = {"ノーマル", "テスト わっかなし", "テスト ふらっしゅつき", "テスト もどってこない", NULL};
static const char* peach_dust_patname[] = {"てきにあたる トゲノコ", "てきにあたる ハンマーブロス", "てきにあたる ガボン", "----みてい", NULL};
static const char* star_power_patname[] = {"-----", "<デバッグよう>", NULL};
static const char* hit_patname[] = {"つうじょう", "ひ", "かみなり", "れいき", NULL};
static const char* confetti_patname[] = {"bom lv1", "bom lv2", "bom lv3", "level up!", "bom lv1", "bom lv2", "bom lv3", NULL};
static const char* sstar_patname[] = {"none-z,BG_SURF", "none-z,LAST_DRAW", "z,Hit Check", "Small:z,Hit Check", NULL};
static const char* recover_heart_patname[] = {"ハート", "フラワー", "ヘイホーかいふく", NULL};
static const char* thunder_patname[] = {"demo1", "demo2", "demo3", "battle", "battle", "battle", NULL};
static const char* thunderflare_patname[] = {"カメックつえまほう", "ドガボンしんぞうまほう", NULL};
static const char* whirlwind_patname[] = {"左まわり", "右まわり", "あか左まわり", "あか右まわり", "あお左まわり", "あお右まわり", "あかあお左まわり", "あかあお右まわり", "test 0", NULL};
static const char* glass_patname[] = {"ノーマル", "ライトつき", "緑・ノーマル", "緑・ライトつき", NULL};
static const char* kemuri12_patname[] = {"くろこげ", "ようがんあつい", "ドガボンめざめ", NULL};
static const char* balloon_heiho_patname[] = {"赤", "青", "黄", NULL};
static const char* rubble_patname[] = {"rock", "wall", "block", NULL};
static const char* snow_dust_patname[] = {"アイススタンプゆきたま", "アイススタンプ/パラレラーゆきじるし", "マリオふっかつ/キラキラ", "デモ:ほしすいこみ/こていキラキラ", "ほしのねがいわざ/ほしほし", "ミニほしほし", "まどわせの粉", NULL};
static const char* fireworks_patname[] = {"ふつうそとがわ", "ふつううちがわ", "スターがた", NULL};
static const char* vapor_patname[] = {"ボスパックン ゆげ", "どく・アイスパックン いき", "ゆきこんこん れいき", NULL};
static const char* kumokumo_chip_patname[] = {"くもくもまじん", "ぽこぴーのわざ", NULL};
static const char* crystal_patname[] = {"デアール", "ミステール", "ほしのせいのごこう", "デアール ウエーブ", "ホシパワーアップ デアールウエーブ2", "ミステール ウエーブ", "ミステール ウエーブ2", "チャージ", NULL};
static const char* packn_bfire_patname[] = {"いちばん", "にばん", NULL};
static const char* kumokumo_patname[] = {"クラウダ吹き飛ばし", "コルテス吸い込み", "バッテンダー吸い込み", "ブリザード１", "ブリザード２", NULL};
static const char* akari_pose_patname[] = {"AKR_B_1 つうじょう", "AKR_B_2 よろこび", NULL};
static const char* breath_fire_patname[] = {"ファイアブレス", "チョロチョロファイア", "はんようブレス", NULL};
static const char* butterfly_patname[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", NULL};
static const char* cloud_patname[] = {"パレッタ", "カメキ", "カメキ バーニング", "ひょうじテスト", NULL};
static const char* shock_patname[] = {"ビリビリ", "ビリビリ&Wave", "あかりシビレはどう", "Wave", NULL};
static const char* pokopi_pcharge_patname[] = {"ポコピー/ジュゲム", "カメックぶんしんするとき", "クッパつえつかう/サンデール", "ふっかつするとき", "クッパパワーアップちゅう", "<みしよう>", "はんようチャージ", "<みしよう>", "<みしよう>", "かいふく", "<みしよう>", "<みしよう>", "<みしよう>", NULL};
static const char* opuku_jetw_patname[] = {"ゲッソー スミこうげき", "オプク ジェットふんしゃ", NULL};
static const char* pokopi_thunder_patname[] = {"はんようのカミナリ", "クッパつえ", "<つかわないこと>", NULL};
static const char* water_damage_patname[] = {"enemy", "mario", "こわっぱおちる", "ジェットふんしゃ", "落下物（水）", "mario jump", "mario stay", "mario dash", NULL};
static const char* dogabon_attack_patname[] = {"a", "b", NULL};
static const char* opuku_wgun_patname[] = {"a", "b", NULL};
static const char* opuku_wprotect_patname[] = {"a", "b", NULL};
static const char* pturn_patname[] = {"ぴよりターン", "おくちにチャック", NULL};
static const char* pokopi_pipo_patname[] = {"a", "b", NULL};
static const char* str_fireflower[] = {"順方向", "逆方向", NULL};
static const char* str_fall_leaf[] = {"落ち葉64-1", "落ち葉64-2", "落ち葉 GC-1", "落ち葉 GC-2", "落ち葉 GC-3", NULL};
#endif

eff_set eff_set_n64[] = {
#if VERSION <= VERSION_DEMO_JP
    {0, "expbom_n64", "敵がしんじゃうとき", NULL},
    {1, "kemuri1_n64", "土煙着地", kemuri_patname},
    {2, "kemuri2_n64", "土煙ダッシュ", kemuri_dash_patname},
    {3, "kemuri3_n64", "花煙ジャンプ", NULL},
    {4, "kemuri4_n64", "花煙ダッシュ", NULL},
    {5, "kemuri5_n64", "雲煙着地", NULL},
    {6, "kemuri6_n64", "雲煙歩き", NULL},
    {7, "kemuri7_n64", "雪あしあと", NULL},
    {8, "flower_n64", "お花がのぼっていく", NULL},
    {9, "snow_n64", "雪の結晶が降る", NULL},
    {10, "sstar_n64", "流れ星", sstar_patname},
    {11, "stardust_n64", "スターダスト", stardust_patname},
    {12, "magic1_n64", "カメックの魔法１", NULL},
    {13, "@Pymagic2_n64@Pw", "カメックの魔法２", magic2_patname},
    {14, "fall_leaf_n64", "落ち葉", str_fall_leaf},
    {15, "hokori_n64", "ホコリ", hokori_patname},
    {16, "gareki_n64", "がれき", NULL},
    {17, "kemuri8_n64", "けむりピンキーボム用", kemu_pinky_patname},
    {18, "small_star_n64", "細かい星", small_star_patname},
    {19, "bomb_n64", "bom", pinky_patname},
    {20, "wangflush_n64", "ワンワンフラッシュ", wangflush_patname},
    {21, "magic3_n64", "マジック３", NULL},
    {22, "kemuri9_n64", "ハンマー煙", hammerkemu_patname},
    {23, "kemuri10_n64", "カメックほうきけむり", NULL},
    {24, "@Pyfire_n64@Pw", "ほのお", NULL},
    {25, "rappa_n64", "ラッパ草", NULL},
    {26, "stardust2_n64", "スターダスト２", stardust2_patname},
    {27, "ripple_n64", "おぷくの波紋", ripple_patname},
    {28, "@Pyexplosion_n64@Pw", "爆発", NULL},
    {29, "kemuri11_n64", "ドガボンけむり", dogabon_kemuri_patname},
    {30, "onpu_n64", "音符", onpu_patname},
    {31, "kemuri12_n64", "コロコゲ煙", kemuri12_patname},
    {32, "sweat_n64", "汗", sweat_patname},
    {33, "balloon_n64", "ドガボン風船", NULL},
    {34, "cool_leaf_n64", "さぶっ　落葉", NULL},
    {35, "bush_n64", "しげみ", NULL},
    {36, "rothammer_success_n64", "回転ハンマー成功", NULL},
    {37, "rothammer_fail_n64", "回転ハンマー失敗", NULL},
    {38, "guruguru_n64", "ぐるぐる", NULL},
    {39, "ice_n64", "アイスバッチ", NULL},
    {40, "stamp_n64", "ふみつけ", stamp_patname},
    {41, "@Pythruhammer_n64@Pw", "かんつうハンマー", thruhammer_patname},
    {42, "radiation_n64", "集中", radiation_patname},
    {43, "thunder_n64", "かみなり", thunder_patname},
    {44, "breath_fire_n64", "火を吐く", breath_fire_patname},
    {45, "broken_barrier_n64", "バリア破壊", broken_barrier_patname},
    {46, "fire_ring_n64", "オープニング", NULL},
    {47, "fire_dust_n64", "fire_ring が使用する", NULL},
    {48, "@Pyflame_n64@Pw", "flame", flame_patname},
    {49, "blow_n64", "噴水", NULL},
    {50, "bomhei_n64", "バトルの敵ボムヘイ", NULL},
    {51, "fireflower_n64", "ファイヤーフラワー", str_fireflower},
    {52, "recover_heart_n64", "回復ハート", recover_heart_patname},
    {53, "pturn_n64", "行動不能", pturn_patname},
    {54, "hit_n64", "ヒット", hit_patname},
    {55, "confetti_n64", "紙ふぶき", confetti_patname},
    {56, "snowfall_n64", "雪が降る", NULL},
    {57, "whirlwind_n64", "マリオ・敵の踊り回転", whirlwind_patname},
    {58, "thunderflare_n64", "魔法カメック杖", thunderflare_patname},
    {59, "coin_n64", "コイン取ったとき", NULL},
    {60, "shock_n64", "バトル用", shock_patname},
    {61, "balloon_heiho_n64", "バトル・バルーンヘイホー", balloon_heiho_patname},
    {62, "rubble_n64", "バトル・ホラオウ", rubble_patname},
    {63, "magic_horao_n64", "ホラオウバトル", NULL},
    {64, "lensflare_n64", "サンデール・ビッグカンテラ", NULL},
    {65, "crystal_n64", "クリスタル", crystal_patname},
    {66, "butterfly_n64", "ジャングル飛んでる", butterfly_patname},
    {67, "akari_charge_n64", "アカリ・くもくも魔人・ピカリン", NULL},
    {68, "akaro_sflush_n64", "アカリの技", NULL},
    {69, "opuku_wgun_n64", "オプクの技", opuku_wgun_patname},
    {70, "opuku_wprotect_n64", "オプクの技", opuku_wprotect_patname},
    {71, "opuku_wcurtain_n64", "オプクの技", NULL},
    {72, "opuku_jetw_n64", "オプク・ゲッソー", opuku_jetw_patname},
    {73, "opuku_jetwrain_n64", "オプク", NULL},
    {74, "pokopi_thunder_n64", "ポコピー", pokopi_thunder_patname},
    {75, "water_damage_n64", "水ダメージ", water_damage_patname},
    {76, "snowman_n64", "ゆきやこんこん", NULL},
    {77, "holography_n64", "ホログラフ", NULL},
    {78, "timestamp_n64", "バトル", NULL},
    {79, "packn_bfire_n64", "パックンフラワー", packn_bfire_patname},
    {80, "pokopi_pipo_n64", "ポコピーパイポ", pokopi_pipo_patname},
    {81, "cloud_n64", "くも", cloud_patname},
    {82, "dogabon_attack_n64", "ドガボン", dogabon_attack_patname},
    {83, "kameki_tornade_n64", "バトルカメキ", NULL},
    {84, "stone_smoke_n64", "ミニミニ・ストーンぼうし", NULL},
    {85, "pokopi_pcharge_n64", "ポコピーチャージ", pokopi_pcharge_patname},
    {86, "hagetaka_flush_n64", "バトル・イワハゲ", NULL},
    {87, "kumokumo_n64", "くもくも", kumokumo_patname},
    {88, "vapor_n64", "ゆげ", vapor_patname},
    {89, "fire_spark_n64", "バトル・ファイアーパックン", NULL},
    {90, "horao_spell_n64", "バトル・ホラオウ", NULL},
    {91, "snow_dust_n64", "ユキの煙", snow_dust_patname},
    {92, "akari_pose_n64", "あかりポーズ", akari_pose_patname},
    {93, "kumokumo_chip_n64", "くもくも魔人・ポコピー技", kumokumo_chip_patname},
    {94, "fireworks_n64", "花火", fireworks_patname},
    {95, "glass_n64", "ガラス", glass_patname},
    {96, "booboo_n64", "ブーブーさん", NULL},
    {97, "pikkyolo_n64", "ぴっきょろ", NULL},
    {98, "freeze_n64", "フリーズ", NULL},
    {99, "sunlight_n64", "太陽光線", sunlight_patname},
    {100, "release_n64", "解放", release_patname},
    {101, "peach_dust_n64", "ピーチダスト", peach_dust_patname},
    {102, "star_power_n64", "スターパワー", star_power_patname},
    {-1, NULL, NULL, NULL},
#else
    {0, "expbom_n64"},
    {1, "kemuri1_n64"},
    {2, "kemuri2_n64"},
    {3, "kemuri3_n64"},
    {4, "kemuri4_n64"},
    {5, "kemuri5_n64"},
    {6, "kemuri6_n64"},
    {7, "kemuri7_n64"},
    {8, "flower_n64"},
    {9, "snow_n64"},
    {10, "sstar_n64"},
    {11, "stardust_n64"},
    {12, "magic1_n64"},
    {13, "@Pymagic2_n64@Pw"},
    {14, "fall_leaf_n64"},
    {15, "hokori_n64"},
    {16, "gareki_n64"},
    {17, "kemuri8_n64"},
    {18, "small_star_n64"},
    {19, "bomb_n64"},
    {20, "wangflush_n64"},
    {21, "magic3_n64"},
    {22, "kemuri9_n64"},
    {23, "kemuri10_n64"},
    {24, "@Pyfire_n64@Pw"},
    {25, "rappa_n64"},
    {26, "stardust2_n64"},
    {27, "ripple_n64"},
    {28, "@Pyexplosion_n64@Pw"},
    {29, "kemuri11_n64"},
    {30, "onpu_n64"},
    {31, "kemuri12_n64"},
    {32, "sweat_n64"},
    {33, "balloon_n64"},
    {34, "cool_leaf_n64"},
    {35, "bush_n64"},
    {36, "rothammer_success_n64"},
    {37, "rothammer_fail_n64"},
    {38, "guruguru_n64"},
    {39, "ice_n64"},
    {40, "stamp_n64"},
    {41, "@Pythruhammer_n64@Pw"},
    {42, "radiation_n64"},
    {43, "thunder_n64"},
    {44, "breath_fire_n64"},
    {45, "broken_barrier_n64"},
    {46, "fire_ring_n64"},
    {47, "fire_dust_n64"},
    {48, "@Pyflame_n64@Pw"},
    {49, "blow_n64"},
    {50, "bomhei_n64"},
    {51, "fireflower_n64"},
    {52, "recover_heart_n64"},
    {53, "pturn_n64"},
    {54, "hit_n64"},
    {55, "confetti_n64"},
    {56, "snowfall_n64"},
    {57, "whirlwind_n64"},
    {58, "thunderflare_n64"},
    {59, "coin_n64"},
    {60, "shock_n64"},
    {61, "balloon_heiho_n64"},
    {62, "rubble_n64"},
    {63, "magic_horao_n64"},
    {64, "lensflare_n64"},
    {65, "crystal_n64"},
    {66, "butterfly_n64"},
    {67, "akari_charge_n64"},
    {68, "akaro_sflush_n64"},
    {69, "opuku_wgun_n64"},
    {70, "opuku_wprotect_n64"},
    {71, "opuku_wcurtain_n64"},
    {72, "opuku_jetw_n64"},
    {73, "opuku_jetwrain_n64"},
    {74, "pokopi_thunder_n64"},
    {75, "water_damage_n64"},
    {76, "snowman_n64"},
    {77, "holography_n64"},
    {78, "timestamp_n64"},
    {79, "packn_bfire_n64"},
    {80, "pokopi_pipo_n64"},
    {81, "cloud_n64"},
    {82, "dogabon_attack_n64"},
    {83, "kameki_tornade_n64"},
    {84, "stone_smoke_n64"},
    {85, "pokopi_pcharge_n64"},
    {86, "hagetaka_flush_n64"},
    {87, "kumokumo_n64"},
    {88, "vapor_n64"},
    {89, "fire_spark_n64"},
    {90, "horao_spell_n64"},
    {91, "snow_dust_n64"},
    {92, "akari_pose_n64"},
    {93, "kumokumo_chip_n64"},
    {94, "fireworks_n64"},
    {95, "glass_n64"},
    {96, "booboo_n64"},
    {97, "pikkyolo_n64"},
    {98, "freeze_n64"},
    {99, "sunlight_n64"},
    {100, "release_n64"},
    {101, "peach_dust_n64"},
    {102, "star_power_n64"},
    {-1, NULL}
#endif
};

/** Fixed table of 128 random numbers that effTblRandN64 reads for repeatable effect randomness. */
static s32 _rand_table[] = {
    0x100B2AF5, 0x45B59924, 0x35094B45, 0x4ABFA67A,
    0x164F5371, 0x7B195845, 0x58562A56, 0x25733D41,
    0x48008107, 0x0E004F28, 0x6963B8AD, 0x5B82AB71,
    0x6BC1F51B, 0x3D947816, 0x39705175, 0x44409A59,
    0x0ED99067, 0x5F70B6F8, 0x3225AEEE, 0x08B5E97C,
    0x2CDE7594, 0x5E9E5B7D, 0x5B2A2888, 0x107F0F50,
    0x58FBC53B, 0x65F69F19, 0x3AF6CEF3, 0x15C3894F,
    0x4C931A2C, 0x39A897E9, 0x5945BB8C, 0x5C9E4521,
    0x7F5E310D, 0x0E4F06D2, 0x275DEB9C, 0x15AD847F,
    0x09685F17, 0x7FB415F2, 0x3B20C1C0, 0x5168E01E,
    0x0DB4651A, 0x24847A6D, 0x2CEB8B8F, 0x79765A35,
    0x6218F283, 0x665BDD04, 0x3DB6F48F, 0x70F282EB,
    0x45CC93FC, 0x6FDCA37D, 0x79A86C67, 0x72AB0990,
    0x4E7AFEFA, 0x54D294F0, 0x032A18E0, 0x2776C435,
    0x3AC93409, 0x3E20E7D4, 0x3D3A4D85, 0x075C4E35,
    0x77C97FBD, 0x16800911, 0x63FA9357, 0x7727B0CB,
    0x24CF0FE3, 0x0B587EF3, 0x0CD5354A, 0x2E376EFA,
    0x0B0C94E5, 0x47F5F70A, 0x7FA04F19, 0x18C0FA00,
    0x6C7A7178, 0x2C8BDAA8, 0x12375435, 0x4E9363FB,
    0x12E7B7AD, 0x4FEE48C4, 0x3F85E6E6, 0x58B44BA9,
    0x3FCAEC41, 0x392E534E, 0x4B5F553A, 0x0E45EB3C,
    0x0E00E83E, 0x4E896E1A, 0x35BCAF71, 0x48CA1C47,
    0x0CAA55EE, 0x72F6FCF6, 0x50266A7D, 0x0473D5AC,
    0x09770608, 0x3420FDD4, 0x7B9B8677, 0x2E4615EB,
    0x3F797CC7, 0x0870BBC1, 0x5C7D84E6, 0x4A8611AC,
    0x5066B2CB, 0x5C1DD3FF, 0x63470BAC, 0x3CE12443,
    0x08A9AEA7, 0x757E5FE2, 0x0B74883F, 0x1B916654,
    0x456CA8A6, 0x4AFA6F25, 0x7445B1FE, 0x053794E8,
    0x0428C273, 0x3FA50738, 0x137D8024, 0x1229AAB1,
    0x0E2E7552, 0x493A2F95, 0x5AF3C6F9, 0x1AD8CB41,
    0x3C312C8C, 0x2B1A3176, 0x1F4CA0ED, 0x45A83294,
    0x5F3B2F4A, 0x1AE82764, 0x73EE487F, 0x1EB4AC11
};

//.sbss
static eff_n64_work work;

//.sdata
static eff_n64_work* wp = &work;

/** DVD read-complete callback: unpacks the effect texture palette, closes the file and marks the textures loaded. */
static void _callback_tpl(s32 error, DVDFileInfo* info) {
    DVDEntry* entry;

    entry = (DVDEntry*)info->cb.userData;
    UnpackTexPalette(wp->effTexture);
    DVDMgrClose(entry);
    wp->effTextureLoaded = TRUE;
}

/** Clears the effect-texture state so no N64 effect texture is loaded yet. */
void effInit64(void) {
    memset(wp, 0, sizeof(eff_n64_work));
    wp->effTextureLoaded = FALSE;
}

/**
 * Loads effect_n64.tpl from the archive if present,
 * otherwise reads it from the game disc (path by version and language) in the background;
 * textures become available once unpacked.
 */
void effTexSetupN64(void) {
    const char* lang;
    DVDEntry* entry;
    char path[128];
    u32 length;

    wp->effTexture = NULL;
    wp->effTextureLoaded = FALSE;
    wp->effTexture = (TEXPalette*)arcOpen("effect_n64.tpl", 0, 0);
    if (wp->effTexture == NULL) {
#if VERSION <= VERSION_DEMO_JP
        sprintf(path, "%s/eff/jp/effect_n64.tpl", getMarioStDvdRoot());
#elif VERSION >= VERSION_US
        lang = gp->language == LANGUAGE_JP ? "jp" : "us";
        sprintf(path, "%s/e/%s/effect_n64.tpl", getMarioStDvdRoot(), lang);
#else
        sprintf(path, "%s/e/jp/effect_n64.tpl", getMarioStDvdRoot());
#endif
        entry = DVDMgrOpen(path, 2, 0);
        length = OSRoundUp32B(DVDMgrGetLength(entry));
        wp->effTexture = (TEXPalette*)__memAlloc(HEAP_DEFAULT, length);
        entry->info.cb.userData = entry;
        DVDMgrReadAsync(entry, wp->effTexture, length, 0, _callback_tpl);
    } else {
        UnpackTexPalette(wp->effTexture);
        wp->effTextureLoaded = TRUE;
    }
}

/** Fills obj with effect texture id from the loaded palette, or a 1x1 placeholder until loading has finished. */
void effGetTexObjN64(u32 id, GXTexObj* obj) {
#pragma explicit_zero_data on
    static u8 dummy ATTRIBUTE_ALIGN(32) = 0;
#pragma explicit_zero_data reset

    if (!wp->effTextureLoaded) {
        GXInitTexObj(obj, &dummy, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
    } else {
        TEXGetGXTexObjFromPalette(wp->effTexture, obj, id);
    }
}

/** Looks up an N64 effect by its script name in the eff_set_n64 table; returns NULL if there is no such name. */
eff_set* effGetSetN64(const char* name) {
    int i;

    for (i = 0; eff_set_n64[i].id != -1; i++) {
        if (!strcmp(eff_set_n64[i].name, name)) {
            return &eff_set_n64[i];
        }
    }
    return NULL;
}

/** Returns a repeatable pseudo-random value in 0..max taken from a fixed 128-entry table at index (mod 128). */
u32 effTblRandN64(u32 max, s32 index) {
    s32 val = _rand_table[index % 128];
    if (val < 0) {
        val = -val;
    }
    return val % (s32)(max + 1);
}

/**
 * Points GX at an effN64Vtx array and selects 16-bit-indexed position,
 * color and texcoord attributes for the following draws.
 */
void effSetVtxDescN64(effN64Vtx* vtx) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 5);
    GXSetArray(GX_VA_POS, &vtx->x, sizeof(effN64Vtx));
    GXSetArray(GX_VA_CLR0, &vtx->r, sizeof(effN64Vtx));
    GXSetArray(GX_VA_TEX0, &vtx->s, sizeof(effN64Vtx));
}

/** Writes one triangle (three indices, each repeated for the position, color and texcoord arrays) to the GX FIFO. */
void tri1(u16 x, u16 y, u16 z, u16 flag) {
    GXWGFifo.u16 = x;
    GXWGFifo.u16 = x;
    GXWGFifo.u16 = x;
    GXWGFifo.u16 = y;
    GXWGFifo.u16 = y;
    GXWGFifo.u16 = y;
    GXWGFifo.u16 = z;
    GXWGFifo.u16 = z;
    GXWGFifo.u16 = z;
}

/** Writes two triangles (six indices, each repeated for the position, color and texcoord arrays) to the GX FIFO. */
void tri2(s32 x1, s32 y1, s32 z1, s32 flag1, s32 x2, s32 y2, s32 z2, s32 flag2) {
    GXWGFifo.u16 = x1;
    GXWGFifo.u16 = x1;
    GXWGFifo.u16 = x1;
    GXWGFifo.u16 = y1;
    GXWGFifo.u16 = y1;
    GXWGFifo.u16 = y1;
    GXWGFifo.u16 = z1;
    GXWGFifo.u16 = z1;
    GXWGFifo.u16 = z1;
    GXWGFifo.u16 = x2;
    GXWGFifo.u16 = x2;
    GXWGFifo.u16 = x2;
    GXWGFifo.u16 = y2;
    GXWGFifo.u16 = y2;
    GXWGFifo.u16 = y2;
    GXWGFifo.u16 = z2;
    GXWGFifo.u16 = z2;
    GXWGFifo.u16 = z2;
}
