// Effect driver: the effect entry pool, the effect textures and the Maya-exported effect animations.

#include "version.h"
#include "driver/effdrv.h"
#include "hwaccess.h"
#include <dolphin/os/OSFastCast.h>
#include "driver/arcdrv.h"
#include "eff/eff_n64.h"
#include "manager/dvdmgr.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <string.h>
#include "libc_decls.h"

// The demos still load the effect textures from the old eff folder.
#if VERSION <= VERSION_DEMO_US
#define EFF_TPL_PATH "%s/eff/%s/effect_%s.tpl"
#define EFF_TPL_JP "%s/eff/jp/effect.tpl"
#else
#define EFF_TPL_PATH "%s/e/%s/effect_%s.tpl"
#define EFF_TPL_JP "%s/e/jp/effect.tpl"
#endif

//.data
#if VERSION <= VERSION_DEMO_US
static const char* str_kemuri[] = {"着地", "敵消滅", "重いもの移動用", NULL};
static const char* str_confetti[] = {"type 0", "type 1", "type 2", "type 3", "type 4", "type 5", "type 6", NULL};
static const char* str_fukidashi[] = {"びっくり！", "はてな？", "ぐるぐる", "数字", "てんてん", "もじゃもじゃ", "チュチュリーナ", NULL};
static const char* str_butterfly[] = {"type 0", "type 1", "type 2", "type 3", "type 4", "type 5", "type 6", "type 7", "type 8", "type 9", "type 10", NULL};
static const char* str_hit[] = {"通常", "ほのお", "かみなり", "れいき", NULL};
static const char* str_damage[] = {"通常", "フラワー", NULL};
static const char* str_small_star[] = {"初期回転なし", "初期回転あり", "落下あり", "扇", "パーティ", NULL};
static const char* str_mario_balloon[] = {"びっくり！", "てんてん", NULL};
static const char* str_stardust[] = {"type 0", "コインゲット", "ＨＰかいふく", "type 3", "type 4", "type 5", "type 6", NULL};
static const char* str_recovery[] = {"ハート", "フラワー", "生ハート", "生フラワー", "ハートMAX", "フラワーMAX", "バトルクリスチーヌ2", "ハート（星屑なし）", "フラワー（星屑なし）", NULL};
static const char* str_nice[] = {"ナイス", "グット", "グレイト", "ワンダフル", "エクセレント", "-----", "ラッキー", "アクロバット", NULL};
static const char* str_breath_fire[] = {"type 0", "type 1", "テスト", NULL};
static const char* str_kemuri_test[] = {"ノーマル", "ゆげ", "ＭＯＢＪ破壊", "ノズル爆発", "敵を倒した", "ノコタロウ３（待機）", "マグマのけむり", "ぶた変身", "テレサ", "バトルチュチュリーナ１", "暖炉に落ちた", "大砲発射", "大砲発射２", "クスリ調合", "月面", "バトルノコタロウ２", "軍団ザコ１（ほのおのビン）", "キラー", "キラー大砲", "首領・魔法不発", "軍団ザコ魔法使い系１", "マリオのハンマーの煙（地面）", "マリオのハンマーの煙（壁）", NULL};
static const char* str_starstone[] = {"ダイアモンドスター", "エメラルドスター", "ゴールドスター", "ルビースター", "サファイアスター", "ガーネットスター", "クリスタルスター", "生ダイアモンドスター", "生エメラルドスター", "生ゴールドスター", "生ルビースター", "生サファイアスター", "生ガーネットスター", "生クリスタルスター", NULL};
static const char* str_itemget[] = {"スターストーン", "パーティ", "パーティパワーアップ", "重要アイテム", "スーパーコイン", "バッジ", "キーアイテム", "消費アイテム", "ほしのかけら", "スーパーアクションコマンド（バトル）", "アイテム使用時", NULL};
static const char* str_status[] = {"グルグル", "アレルギー", NULL};
static const char* str_fall[] = {"けむし", "けむし大", "フォーク", "水", "たらい", "ばけつ", "クッパ", "いんせき", "ライト", NULL};
static const char* str_mobj_broken[] = {"ブロック茶", "ブロック灰", "ブロック黄", "ブロック茶・大", "ブロック灰・大", "ブロック黄・大", "ドカン", "さんご", NULL};
static const char* str_mahojin[] = {"出現", "★1", "★2", "★3", "★4", "★5", "★6", "★7", "★★", NULL};
static const char* str_nozle[] = {"けむり", "爆発", "炎", "ドライアイス", NULL};
static const char* str_minigame[] = {"スタート", "ゴール", "リタイア", NULL};
static const char* str_splash[] = {"はまった・水", "とびでる・水", "はまった・炎", "ゲッソー登場", "ゲッソー登場・背景", NULL};
static const char* str_toge_flush[] = {"ノーマル", "赤", "青", "ランペル", "影の女王・目玉が光る", NULL};
static const char* str_charge[] = {"チャージ1", "チャージ2", "影の女王・チャージ", NULL};
static const char* str_mizutama[] = {"泳ぐ lv0", "泳ぐ lv1", "泳ぐ lv2", "泳ぐ lv3", "とびこんだ lv0", "とびこんだ lv1", "とびこんだ lv2", "とびこんだ lv3", NULL};
static const char* str_fire[] = {"火", "ノーマル炎（実験）", "ノーマル炎２（実験）", "ローソク炎（グレアが丸い）", "ローソク炎（グレアが十字）", "炎・赤（実験）", "炎・青（実験）", "炎・緑（実験）", "ローソク炎（グレアが丸い）（黒）", NULL};
static const char* str_kiss[] = {"イベント・チュチュリーナ", "バトル・クリスチーヌ１", "バトル・クラウダ２", "バトル・チビヨッシー２", "バトル・ビビアン３", "バトル・ビビアン３（敵）", NULL};
static const char* str_stageclear[] = {"ステージクリア！", "ゴールイン！", NULL};
static const char* str_sandars[] = {"フィールド Lv0", "フィールド Lv1", "フィールド Lv2", "バトル AC 成功", "バトル AC 失敗", "噴出す蒸気", NULL};
static const char* str_mist[] = {"霧", "ヤミの吐息", "困惑の吐息", "毒の息", "白い息 bom", "伝説の扉", "影の女王黒い煙", NULL};
static const char* str_ripple[] = {"普通の波紋", "ビビアンの波紋", "大砲の衝撃波", "影の女王・蒸発する黒い煙", NULL};
static const char* str_toiki[] = {"ヤミの吐息", "困惑の吐息", "毒の息", NULL};
static const char* str_mist2[] = {"湧き出す黒いカタマリ", "落下する黒いカタマリ", NULL};
static const char* str_particle[] = {"火花", NULL};
static const char* str_updown[] = {"攻撃力", "防御力", "攻撃力（静止）", "防御力（静止）", NULL};
static const char* str_indirect[] = {"スター", "わっか", NULL};
static const char* str_batten[] = {"エックス", "照準", "照準ロックオン", "ロケット", NULL};
static const char* str_syuryou[] = {"首領の杖", "バリア", NULL};
static const char* str_uranoko[] = {"黒いオーラ", NULL};
static const char* str_hibashira[] = {"ファイアナグーリ", "首領・炎", "バトルサンダース４", "ファイアナグーリ・衝撃波", "首領・炎・衝撃波", "バトルサンダース４・衝撃波", NULL};
static const char* str_number[] = {"ＨＰ回復", "ＦＰ回復", "ダメージ", "ノーマル", NULL};
static const char* str_energy[] = {"こんらんエネルギー", "ふにゃふにゃエネルギー", "つらぬきエネルギー", "ねむらせエネルギー", "ミニミニエネルギー", NULL};
static const char* str_biribirikinoko[] = {"左向き", "右向き", NULL};
static const char* str_nokotarou[] = {"バトルノコタロウ３", NULL};
static const char* str_teresa[] = {"テレサ", "影の女王・衝撃波", NULL};
static const char* str_gonbaba_breath[] = {"通常", "ポイズン", "メガ", "コールド", "混乱", "眠り", "ミニミニ", "クッパ", NULL};
static const char* str_ultra_hammer[] = {"左回り", "右回り", NULL};
#endif
EffectSet eff_set_table[] = {
#if VERSION <= VERSION_DEMO_US
    {0, "kemuri", "けむり", str_kemuri},
    {1, "confetti", "紙ふぶき", str_confetti},
    {2, "fukidashi", "ふきだし", str_fukidashi},
    {3, "butterfly", "ちょうちょ", str_butterfly},
    {4, "hit", "ヒット", str_hit},
    {5, "damage_star", "バトルダメージ", str_damage},
    {6, "small_star", "敵と接触", str_small_star},
    {7, "mario_balloon", "マリオ吹き出し", str_mario_balloon},
    {8, "mugi", "むぎばたけ", NULL},
    {9, "stardust", "ほしくず", str_stardust},
    {10, "recovery", "回復ハート", str_recovery},
    {11, "miss_star", "攻撃ミス", NULL},
    {12, "nice", "ナイス", str_nice},
    {13, "breath_fire", "クッパが火を吐く", str_breath_fire},
    {14, "torch", "たいまつ", NULL},
    {15, "confusion", "バトル・混乱", NULL},
    {16, "spark", "バトル・スパーク", NULL},
    {17, "updown", "ステータスアップ・ダウン", str_updown},
    {18, "charge", "バトル・チャージ", str_charge},
    {19, "toge_flush", "トゲフラッシュ", str_toge_flush},
    {20, "ice", "アイス", NULL},
    {21, "fire", "ファイアー", str_fire},
    {22, "bomb", "ボム", NULL},
    {23, "ripple", "波紋", str_ripple},
    {24, "coin_fukidashi", "コイン吹き出し", NULL},
    {25, "star_point", "スターポイント", NULL},
    {26, "puni_balloon", "ぷにバルーン", NULL},
    {27, "p_fukidashi", "ピーチ編のふきだし", NULL},
    {28, "kemuri_test", "けむりテスト", str_kemuri_test},
    {29, "starstone", "スターストーン", str_starstone},
    {30, "itemget", "アイテム取得", str_itemget},
    {31, "status", "ステータス異常", str_status},
    {32, "pointget", "ポイントゲット", NULL},
    {33, "levelup", "レベルアップ", NULL},
    {34, "stageclear", "ステージクリア", str_stageclear},
    {35, "fall", "落下物", str_fall},
    {36, "fpdamage", "ＦＰダメージ", NULL},
    {37, "break", "ブレイク", NULL},
    {38, "mobj_broken", "ＭＯＢＪ破壊", str_mobj_broken},
    {39, "mahojin", "魔方陣", str_mahojin},
    {40, "nozle", "ノズル", str_nozle},
    {41, "mizutama", "水中の泡", str_mizutama},
    {42, "minigame", "ミニゲーム", str_minigame},
    {43, "splash", "水しぶき", str_splash},
    {44, "treasure_map", "宝の地図", NULL},
    {45, "kiss", "チュチュリーナキス", str_kiss},
    {46, "funemizu", "船のしぶき", NULL},
    {47, "mist", "霧", str_mist},
    {48, "teresa", "テレサ１０００", str_teresa},
    {49, "batten", "バッテンダーエックス", str_batten},
    {50, "sheep", "ねむれよいこ", NULL},
    {51, "naniga", "なにがおこるかな", NULL},
    {52, "sandars", "サンダース爆発", str_sandars},
    {53, "boomerang", "ブーメラン", NULL},
    {54, "irekae", "いれかえーる", NULL},
    {55, "rankup", "ランクアップ", NULL},
    {56, "scanning", "スキャニング", NULL},
    {57, "toiki", "影の女王・吐息", str_toiki},
    {58, "mist2", "黒い霧", str_mist2},
    {59, "particle", "パーティクル", str_particle},
    {60, "ibuki", "こおりのいぶき", NULL},
    {61, "mahorn", "マホーン", NULL},
    {62, "spirit", "コルテス魂", NULL},
    {63, "indirect", "インダイレクト", str_indirect},
    {64, "syuryou", "首領", str_syuryou},
    {65, "uranoko", "ウラノコ", str_uranoko},
    {66, "hibashira", "火柱", str_hibashira},
    {67, "number", "汎用数字", str_number},
    {68, "laser", "レーザー（地図イベント）", NULL},
    {69, "machingegun", "バッテンダー・マシンガン２", NULL},
    {70, "las_mon", "伝説の扉", NULL},
    {71, "sleep", "眠る", NULL},
    {72, "energy", "エネルギー", str_energy},
    {73, "biribirikinoko", "びりびりキノコ", str_biribirikinoko},
    {74, "nokotarou", "ノコタロウ", str_nokotarou},
    {75, "queen", "影の女王・渦", NULL},
    {76, "queen2", "影の女王・黒い衝撃波", NULL},
    {77, "gonbaba_breath", "ファイアブレス", str_gonbaba_breath},
    {78, "majinai", "呪い師", NULL},
    {79, "mahorn2", "マホーン", NULL},
    {80, "ultra_hammer", "ウルトラハンマー", str_ultra_hammer},
    {-1, NULL, NULL, NULL},
#else
    {0, "kemuri"},
    {1, "confetti"},
    {2, "fukidashi"},
    {3, "butterfly"},
    {4, "hit"},
    {5, "damage_star"},
    {6, "small_star"},
    {7, "mario_balloon"},
    {8, "mugi"},
    {9, "stardust"},
    {10, "recovery"},
    {11, "miss_star"},
    {12, "nice"},
    {13, "breath_fire"},
    {14, "torch"},
    {15, "confusion"},
    {16, "spark"},
    {17, "updown"},
    {18, "charge"},
    {19, "toge_flush"},
    {20, "ice"},
    {21, "fire"},
    {22, "bomb"},
    {23, "ripple"},
    {24, "coin_fukidashi"},
    {25, "star_point"},
    {26, "puni_balloon"},
    {27, "p_fukidashi"},
    {28, "kemuri_test"},
    {29, "starstone"},
    {30, "itemget"},
    {31, "status"},
    {32, "pointget"},
    {33, "levelup"},
    {34, "stageclear"},
    {35, "fall"},
    {36, "fpdamage"},
    {37, "break"},
    {38, "mobj_broken"},
    {39, "mahojin"},
    {40, "nozle"},
    {41, "mizutama"},
    {42, "minigame"},
    {43, "splash"},
    {44, "treasure_map"},
    {45, "kiss"},
    {46, "funemizu"},
    {47, "mist"},
    {48, "teresa"},
    {49, "batten"},
    {50, "sheep"},
    {51, "naniga"},
    {52, "sandars"},
    {53, "boomerang"},
    {54, "irekae"},
    {55, "rankup"},
    {56, "scanning"},
    {57, "toiki"},
    {58, "mist2"},
    {59, "particle"},
    {60, "ibuki"},
    {61, "mahorn"},
    {62, "spirit"},
    {63, "indirect"},
    {64, "syuryou"},
    {65, "uranoko"},
    {66, "hibashira"},
    {67, "number"},
    {68, "laser"},
    {69, "machingegun"},
    {70, "las_mon"},
    {71, "sleep"},
    {72, "energy"},
    {73, "biribirikinoko"},
    {74, "nokotarou"},
    {75, "queen"},
    {76, "queen2"},
    {77, "gonbaba_breath"},
    {78, "majinai"},
    {79, "mahorn2"},
    {80, "ultra_hammer"},
    {-1, NULL}
#endif
};

//.sbss
static EffectWork work;

//.sdata
static EffectWork* wp = &work;
const char* prefix_tbl[] = {
    "jp", "us",
#if VERSION >= VERSION_US
    "ge", "fr", "sp", "it", "du"
#endif
};

/**
 * DVD read callback for the asynchronous effect.tpl load:
 * unpacks the texture palette, closes the file and marks the effect textures ready.
 */
static void _callback_tpl(s32 error, DVDFileInfo* info) {
    DVDEntry* entry = info->cb.userData;
    UnpackTexPalette(wp->texture);
    DVDMgrClose(entry);
    wp->texLoaded = TRUE;
}

/**
 * Boot setup: allocates the effect entry pool (512 entries in the US and later releases, 256 before),
 * clears it and the texture state, and initialises the N64-style effects.
 */
void effInit(void) {
#if VERSION >= VERSION_US
    wp->count = 0x200;
#else
    wp->count = 0x100;
#endif
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(EffectEntry));
    memset(wp->entries, 0, wp->count * sizeof(EffectEntry));
    wp->handle = NULL;
    wp->language = -1;
    wp->texture = NULL;
    wp->texLoaded = FALSE;
    effInit64();
}

#if VERSION >= VERSION_US
#define EFF_NTEX 118
#else
#define EFF_NTEX 146
#endif

/**
 * Loads effect.tpl, the shared effect textures:
 * from the startup archive if it is there, otherwise asynchronously from the disc (the jp or us folder by
 * language in the US and later releases). Called from the logo sequence at boot.
 */
void effTexSetup(void) {
    DVDEntry* entry;
    char path[128];
    u32 size;

    wp->texture = NULL;
    wp->texLoaded = FALSE;
    wp->texture = arcOpen("effect.tpl", 0, 0);
    if (!wp->texture) {
#if VERSION >= VERSION_US
        sprintf(path, "%s/e/%s/effect.tpl", getMarioStDvdRoot(), (u32)gp->language == LANGUAGE_JP ? "jp" : "us");
#else
        sprintf(path, EFF_TPL_JP, getMarioStDvdRoot());
#endif
        entry = DVDMgrOpen(path, 2, 0);
        size = OSRoundUp32B(DVDMgrGetLength(entry));
        wp->texture = __memAlloc(HEAP_DEFAULT, size);
        entry->info.cb.userData = entry;
        DVDMgrReadAsync(entry, wp->texture, size, 0, _callback_tpl);
    } else {
        UnpackTexPalette(wp->texture);
        wp->texLoaded = TRUE;
    }
    effTexSetupN64();
}

// explicit_zero_data keeps the zero-initialised `dummy` textures in .sdata instead of .sbss
#pragma explicit_zero_data on
void effGetTexObj(u32 id, GXTexObj* obj) {
    if ((s32)id < EFF_NTEX) { //number of images in effect.tpl
        if (!wp->texLoaded) {
            static char dummy ATTRIBUTE_ALIGN(32) = '\0';
            GXInitTexObj(obj, &dummy, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
        } else {
            TEXGetGXTexObjFromPalette(wp->texture, obj, id);
        }
    } else {
        static char dummy ATTRIBUTE_ALIGN(32) = '\0';
        if (!wp->handle) {
            GXInitTexObj(obj, &dummy, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
        } else {
            TEXGetGXTexObjFromPalette(*wp->handle->data, obj, id - EFF_NTEX);
        }
    }
}
#pragma explicit_zero_data reset

/**
 * Frees every in-use effect that belongs to the battle (inBattle TRUE) or to the field (FALSE);
 * the battle and map change sequences call it when leaving.
 */
void effAutoRelease(BOOL inBattle) {
    int i;
    int count = wp->count;
    EffectEntry* entry = wp->entries;

    for (i = 0; i < count; i++, entry++) {
        if (entry->flags & EFFECT_FLAG_ACTIVE && entry->inBattle == inBattle) {
            effDelete(entry);
        }
    }
}

/**
 * Takes the first free effect entry, marks it in use,
 * tags it as a field or battle effect by the current mode and clears its type, counter, data and callback.
 * The pool has no overflow check.
 */
EffectEntry* effEntry(void) {
    EffectEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if (!entry->flags) {
            break;
        }
    }
    entry->flags = EFFECT_FLAG_ACTIVE;
    entry->inBattle = gp->inBattle != 0;
    entry->type = NULL;
    entry->count = 0;
    entry->userdata = NULL;
    entry->callback = NULL;
    entry->name[0] = '\0';
    return entry;
}

/**
 * Gives an effect the name scripts use to find it again (effNameToPtr).
 * The search for an existing effect with that name has no effect; the name is copied regardless.
 */
void effSetName(EffectEntry* effect, const char* name) {
    int i;
    int count = wp->count;
    EffectEntry* entry = wp->entries;

    for (i = 0; i < count; i++, entry++) {
        if (entry->flags && !strcmp(entry->name, name)) {
            break;
        }
    }
    strcpy(effect->name, name);
}

void effMain(void) {
    const char *dvdroot, *prefix;
    int i;
    int count = wp->count;
    EffectEntry* entry = wp->entries;

    for (i = 0; i < count; i++, entry++) {
        if (gp->inBattle) {
            if (entry->inBattle != 1) {
                continue;
            }
        } else if (entry->inBattle != 0) {
            continue;
        }
        if (entry->flags & EFFECT_FLAG_ACTIVE) {
            if (entry->callback) {
                entry->callback(entry);
            }
        }
    }
    if ((u32)wp->language != (u32)gp->language) {
        if (wp->handle) {
            fileFree(wp->handle);
            wp->handle = NULL;
        }
        prefix = prefix_tbl[gp->language];
        dvdroot = getMarioStDvdRoot();
        if (fileAsyncf(FILE_TYPE_TPL, 0, EFF_TPL_PATH, dvdroot, prefix, prefix) != 0U) {
            prefix = prefix_tbl[gp->language];
            wp->handle = fileAllocf(FILE_TYPE_TPL, EFF_TPL_PATH, getMarioStDvdRoot(), prefix, prefix);
            wp->language = gp->language;
        }
    }
}

/** Frees an effect's data and releases its entry at once; safe on NULL. */
void effDelete(EffectEntry* effect) {
    if (effect && effect->flags) {
        __memFree(HEAP_EFFECT, effect->userdata);
        effect->flags = 0;
    }
}

/**
 * Asks an effect to end: effects that handle their own fade-out (EFFECT_FLAG_SOFT_DELETE) only get a delete
 * request and free themselves later; others are deleted at once.
 */
void effSoftDelete(EffectEntry* effect) {
    if (effect && effect->flags) {
        if (effect->flags & EFFECT_FLAG_SOFT_DELETABLE) {
            effect->flags |= EFFECT_FLAG_SOFT_DELETE;
        } else {
            effDelete(effect);
        }
    }
}

/** Returns the in-use effect with the given name, or NULL. */
EffectEntry* effNameToPtr(const char* name) {
    int i;
    int count = wp->count;
    EffectEntry* entry = wp->entries;

    for (i = 0; i < count; i++, entry++) {
        if (entry->flags && !strcmp(entry->name, name)) {
            break;
        }
    }
    if (i >= count) {
        return NULL;
    } else {
        return entry; //found a match
    }
}

/**
 * Looks an effect type up by its name in eff_set_table (e.g. "kemuri", "confetti");
 * used by the evt_eff script helpers. NULL if unknown.
 */
EffectSet* effGetSet(const char* name) {
    s32 i;

    for (i = 0; eff_set_table[i].id != -1; i++) {
        if (!strcmp(eff_set_table[i].name, name)) {
            return &eff_set_table[i];
        }
    }
    return NULL;
}

/**
 * per-key interpolation and the GQR5 setup were inlines in the original:
 * inline locals take the low registers, last declared lowest (qv0 f9 .. qm1 f12 above dt phaseCount / s f7 / s3 f6 / s2
 * f5; gqr r9 under the key-offset temp r10)
 */
// Hermite interpolation of the 22 channels of two keys into buf (MayaHermitKey: value at +2, in/out tangents at
// +4/+6, step flag at +9 of each 8-byte channel; the s16 values are read through GQR5, see effMayaSetGQR)
// FAKE: channel fields are read through byte offsets; typed channel indexing changes the object
static inline void effMayaKeyInterp(MayaHermitKey* prev, MayaHermitKey* curr, f32 t, f32* buf) {
    s32 i;
    f32* out;
    s8* pb;
    s8* pa;
    pa = (s8*)prev;
    pb = (s8*)curr;
    out = buf;
    for (i = 0; i < 22; i++) {
        f32 qd0;
        f32 qm1;
        f32 u;
        f32 qv1;
        f32 qm0;
        f32 qv0;
        f32 dt;
        f32 s;
        f32 s3;
        f32 s2;
        f32 div;
        qv0 = __OSs16tof32((s16*)(pa + 2));
        qd0 = __OSs16tof32((s16*)(pa + 4));
        qm0 = __OSs16tof32((s16*)(pa + 6));
        qv1 = __OSs16tof32((s16*)(pb + 2));
        qm1 = __OSs16tof32((s16*)(pb + 4));
        qd0 = __OSs16tof32((s16*)(pb + 6));
        if (pa[9] != 0) {
            *out = qv0;
        } else {
            u = t - prev->time;
            dt = curr->time - prev->time;
            div = !dt ? 0.00000001f : dt;
            s = u / div;
            s2 = s * s;
            s3 = s2 * s;
            *out = qv0 * (1.0f + ((2.0f * s3) - (3.0f * s2))) +
                   qm0 * (dt * (s + (s3 - (2.0f * s2)))) +
                   qv1 * ((-2.0f * s3) + (3.0f * s2)) +
                   qm1 * (dt * (s3 - s2));
        }
        pa += 8;
        pb += 8;
        out++;
    }
}

/**
 * Sets GQR5 to load signed 16-bit values scaled by 1/64,
 * the fixed-point format of the Maya key values read by __OSs16tof32.
 */
static inline void effMayaSetGQR(void) {
    register u32 gqr;
    gqr = 0x06070607;
    HW_SET_GQR5(gqr);
}

/**
 * Evaluates one polygon of a Maya effect animation at the animation's current time:
 * interpolates its 22 channels between the surrounding keys
 * and builds its transform matrix (translate, rotate, scale about pivots) and its alpha, clamped to 0..1.
 */
void effCalcMayaAnimMatrix(MayaAnim* anim, MayaPoly* list, f32* mtx, f32* alpha) {
    f32 buf[22];
    Mtx trans;
    Mtx scale;
    Mtx tmp2;
    Mtx pivot2;
    Mtx pivot2Inv;
    Mtx trans3;
    Mtx pivot1;
    Mtx pivot1Inv;
    Mtx trans2;
    Mtx rotX;
    Mtx rotY;
    Mtx rotZ;
    Mtx tmp;

    s32 n = list->keyCount;
    s32 k;
    f32 t;

    for (k = 1; k < n; k++) {
        MayaHermitKey* prev;
        MayaHermitKey* curr;
        curr = &list->keys[k];
        prev = &list->keys[k - 1];
        effMayaSetGQR();
        if (anim->time <= curr->time) {
            t = anim->time;
        } else if (k != n - 1) {
            continue;
        } else {
            t = curr->time;
        }
        effMayaKeyInterp(prev, curr, t, buf);
        PSMTXTrans(trans, buf[0], buf[1], buf[2]);
        PSMTXRotRad(rotX, 'x', buf[3]);
        PSMTXRotRad(rotY, 'y', buf[4]);
        PSMTXRotRad(rotZ, 'z', buf[5]);
        PSMTXScale(scale, buf[6], buf[7], buf[8]);
        PSMTXTrans(pivot1, buf[9], buf[10], buf[11]);
        PSMTXTrans(pivot1Inv, -buf[9], -buf[10], -buf[11]);
        PSMTXTrans(trans2, buf[12], buf[13], buf[14]);
        PSMTXTrans(pivot2, buf[15], buf[16], buf[17]);
        PSMTXTrans(pivot2Inv, -buf[15], -buf[16], -buf[17]);
        PSMTXTrans(trans3, buf[18], buf[19], buf[20]);
        PSMTXConcat(trans3, pivot2, tmp);
        PSMTXConcat(tmp, scale, tmp);
        PSMTXConcat(tmp, pivot2Inv, scale);
        PSMTXConcat(trans2, pivot1, tmp);
        PSMTXConcat(tmp, rotZ, tmp);
        PSMTXConcat(tmp, rotY, tmp);
        PSMTXConcat(tmp, rotX, tmp);
        PSMTXConcat(tmp, pivot1Inv, tmp2);
        PSMTXConcat(trans, tmp2, tmp);
        PSMTXConcat(tmp, scale, (MtxPtr)mtx);
        *alpha = buf[21] > 1.0f ? 1.0f : (buf[21] < 0.0f ? 0.0f : buf[21]);
        return;
    }
}

/**
 * Allocates and clears a playback state for a Maya-exported effect animation, with one joint matrix per polygon;
 * used by the damage star, ice and recovery effects.
 */
MayaAnim* effMayaAnimAlloc(MayaAnimData* data) {
    // 0x18 for the header: 4 bytes more than the offset of joints (0x14)
    s32 size = data->lists->count * sizeof(MayaJoint) + 0x18;
    MayaAnim* anim = __memAlloc(HEAP_EFFECT, size);
    memset(anim, 0, size);
    anim->data = data;
    return anim;
}

/** Frees a playback state from effMayaAnimAlloc. */
void effDeleteMayaAnim(MayaAnim* anim) {
    __memFree(HEAP_EFFECT, anim);
}

/** Starts a Maya effect animation from time 0, using the field or battle render clock. */
void effPlayMayaAnim(MayaAnim* anim) {
    OSTime now = gp->inBattle ? gp->renderTime : gp->renderFieldTime;
    anim->startTime = OSTicksToMilliseconds(now);
    anim->time = 0.0f;
    anim->flags |= MAYA_ANIM_FLAG_PLAYING;
}

/**
 * Advances a playing Maya effect animation from the render clock (field or battle) in frames since it started,
 * stops it at its end time, and recomputes every polygon's matrix and alpha.
 */
void effCalcMayaAnim(MayaAnim* anim) {
    if ((anim->flags & MAYA_ANIM_FLAG_PLAYING) && !(anim->flags & MAYA_ANIM_FLAG_ENDED)) {
        OSTime now = gp->inBattle ? gp->renderTime : gp->renderFieldTime;
        s32 i;
        now = OSTicksToMilliseconds(now);
        anim->time = (f32)(u64)(now - anim->startTime) * gp->framerate / 1000.0f;
        if (anim->time >= anim->data->endTime) {
            anim->time = anim->data->endTime;
            anim->flags |= MAYA_ANIM_FLAG_ENDED;
        }
        for (i = 0; i < anim->data->lists->count; i++) {
            effCalcMayaAnimMatrix(anim, &anim->data->lists->polys[i], (f32*)anim->joints[i].mtx, &anim->joints[i].alpha);
        }
    }
}

/**
 * Draws one polygon of a Maya effect model as triangle fans from its fixed-point (1/512) positions
 * and texture coordinates; the caller sets up the matrix, texture and colour.
 */
void effDrawMayaPoly(MayaPoly* poly) {
    s32 i;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 9);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 9);
    GXSetCullMode(GX_CULL_NONE);
    for (i = 0; i < poly->count; i++) {
        s16* strip = poly->strips[i];
        u16 n = strip[0];
        s16* v;
        s32 j;
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, n);
        v = strip + 1;
        for (j = 0; j < n; j++, v += 5) {
            GXPosition3s16(v[0], v[1], v[2]);
            GXTexCoord2s16(v[3], v[4]);
        }
    }
}
