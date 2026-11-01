// Shared support for the Paper Mario 64 effects: texture loading, the effect set table and vertex/triangle helpers.
// The demo build also stores a name, description and user list for each effect.

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
#if VERSION <= VERSION_DEMO_US
static const char* kemuri_patname[] = {"\203}\203\212\203I:KAMECSMALL", "\203K\203\214\203L:KAMECBIG", "-----", "\203}\203\212\203I\202\307\202\301\202\265\202\361:KAMECBIG", "Block\202\307\202\301\202\265\202\361::KAMECBIG", "\203T\203\223\203{", "\203}\203~\201[", NULL};
static const char* kemuri_dash_patname[] = {"Mairo:KAMECSMALL", "KAMECBIG", "PINKYBOM", NULL};
static const char* stardust_patname[] = {"\202\317\202\237\201[\202\301\202\306\202\305\202\351", "\202\267\202\255\202\310\202\337", "\202\250\202\250\202\337", "\202\347\202\301\202\251\202\267\202\255\202\310\202\337", "\202\347\202\301\202\251\202\250\202\250\202\337"};
static const char* magic2_patname[] = {"\203J\203\201\203b\203N\202\265\202\343\202\244\202\277\202\343\202\244", "\203J\203\201\203b\203N\202\355\202\301\202\251", NULL};
static const char* hokori_patname[] = {"\203g\203\212\203f\202\331\202\261\202\350", "\202\331\202\261\202\350\202\273\202\314\202Q", "\202\331\202\261\202\350\202\320\202\306\202\302", NULL};
static const char* kemu_pinky_patname[] = {"LV1", "LV2", "LV3", "LV1\211\212", "LV2\211\212", "LV3\211\212", NULL};
static const char* small_star_patname[] = {"\202\265\202\345\202\253\202\251\202\242\202\304\202\361\202\310\202\265", "\202\265\202\345\202\253\202\251\202\242\202\304\202\361\202\240\202\350", "\202\347\202\301\202\251\202\240\202\350", "\203I\203E\203M", "\203p\201[\203e\203B", NULL};
static const char* pinky_patname[] = {"LV1", "LV2", "LV3", "LV1\211\212", "LV2\211\212", "LV3\211\212", NULL};
static const char* wangflush_patname[] = {"\203\217\203\223\203\217\203\223", "<\202\335\202\265\202\346\202\244>", NULL};
static const char* hammerkemu_patname[] = {"\202\271\202\242\202\261\202\244", "\203m\201[\203}\203\213", NULL};
static const char* stardust2_patname[] = {"<\202\335\202\265\202\346\202\244>", "HP\202\251\202\242\202\323\202\255", "HP\202\251\202\242\202\323\202\2552", "HP,FP\203u\203\215\203b\203N", "\203s\201[\203`\202\326\202\361\202\265\202\361", "\202\313\202\252\202\244", "HP,FP\203u\203\215\203b\203N(Z)", NULL};
static const char* ripple_patname[] = {"a", "b", NULL};
static const char* eff_n64_users_unused[] = {"c", "d", "e", "f", "g", "h", "j", "k", NULL};
static const char* dogabon_kemuri_patname[] = {"\202\240\202\351\202\253\201\225\202\315\202\265\202\350", "\203q\203b\203v\203A\203^\203b\203N", "\203T\203\223\203{", "\202\255\202\340\202\255\202\340\202\334\202\266\202\361", "\220V\203T\203\223\203{", "\220V\203T\203\223\203{\203}\203~\201[", "\203\211\203C\203g\227\216\211\272", NULL};
static const char* onpu_patname[] = {"\202\266\202\345\202\244\202\265\202\345\202\244", "\203t\203\211\203\217\201[\202\263\202\361", "\203\213\203C\201[\203W\202\265\202\361\202\317\202\242\202\265\202\304\202\351", NULL};
static const char* sweat_patname[] = {"\215U\214\202\203~\203X\216\236", "\203`\203r\203\210\203b\203V\201[", NULL};
static const char* stamp_patname[] = {"\202\313\202\336\202\347\202\271", "\202\261\202\361\202\347\202\361", "\203o\203g\203\213\203r\203r\203A\203\223\202R", "\202\273\202\314\202\331\202\251", NULL};
static const char* thruhammer_patname[] = {"\202\251\202\361\202\302\202\244\203n\203\223\203}\201[", "\202\251\202\361\202\302\202\244\203n\203\223\203}\201[\202\265\202\301\202\317\202\242", NULL};
static const char* radiation_patname[] = {"-----", "\202\302\202\246\202\314\202\320\202\251\202\350", "\202\302\202\246\202\314\202\320\202\251\202\350\202Q", "\203m\203R\203u\203\215\203X\202\314\202\355\202\264", NULL};
static const char* broken_barrier_patname[] = {"\202\302\202\246\203o\203\212\203A\202\261\202\355\202\352\202\351", "\203s\201[\203`\202\326\202\361\202\265\202\361\202\306\202\251\202\352\202\351", "\213t\215\304\220\266", NULL};
static const char* flame_patname[] = {"\203^\203C\203g\203\213\203f\203\202", "\202\331\202\314\202\250\202\314\202\304\202\253", "\203N\203b\203p", "\202\265\202\265\202\345\202\244", NULL};
static const char* sunlight_patname[] = {"\211E\202\251\202\347", "\215\266\202\251\202\347", NULL};
static const char* release_patname[] = {"\203m\201[\203}\203\213", "\203e\203X\203g \202\355\202\301\202\251\202\310\202\265", "\203e\203X\203g \202\323\202\347\202\301\202\265\202\343\202\302\202\253", "\203e\203X\203g \202\340\202\307\202\301\202\304\202\261\202\310\202\242", NULL};
static const char* peach_dust_patname[] = {"\202\304\202\253\202\311\202\240\202\275\202\351 \203g\203Q\203m\203R", "\202\304\202\253\202\311\202\240\202\275\202\351 \203n\203\223\203}\201[\203u\203\215\203X", "\202\304\202\253\202\311\202\240\202\275\202\351 \203K\203{\203\223", "----\202\335\202\304\202\242", NULL};
static const char* star_power_patname[] = {"-----", "<\203f\203o\203b\203O\202\346\202\244>", NULL};
static const char* hit_patname[] = {"\202\302\202\244\202\266\202\345\202\244", "\202\320", "\202\251\202\335\202\310\202\350", "\202\352\202\242\202\253", NULL};
static const char* confetti_patname[] = {"bom lv1", "bom lv2", "bom lv3", "level up!", "bom lv1", "bom lv2", "bom lv3", NULL};
static const char* sstar_patname[] = {"none-z,BG_SURF", "none-z,LAST_DRAW", "z,Hit Check", "Small:z,Hit Check", NULL};
static const char* recover_heart_patname[] = {"\203n\201[\203g", "\203t\203\211\203\217\201[", "\203w\203C\203z\201[\202\251\202\242\202\323\202\255", NULL};
static const char* thunder_patname[] = {"demo1", "demo2", "demo3", "battle", "battle", "battle", NULL};
static const char* thunderflare_patname[] = {"\203J\203\201\203b\203N\202\302\202\246\202\334\202\331\202\244", "\203h\203K\203{\203\223\202\265\202\361\202\274\202\244\202\334\202\331\202\244", NULL};
static const char* whirlwind_patname[] = {"\215\266\202\334\202\355\202\350", "\211E\202\334\202\355\202\350", "\202\240\202\251\215\266\202\334\202\355\202\350", "\202\240\202\251\211E\202\334\202\355\202\350", "\202\240\202\250\215\266\202\334\202\355\202\350", "\202\240\202\250\211E\202\334\202\355\202\350", "\202\240\202\251\202\240\202\250\215\266\202\334\202\355\202\350", "\202\240\202\251\202\240\202\250\211E\202\334\202\355\202\350", "test 0", NULL};
static const char* glass_patname[] = {"\203m\201[\203}\203\213", "\203\211\203C\203g\202\302\202\253", "\227\316\201E\203m\201[\203}\203\213", "\227\316\201E\203\211\203C\203g\202\302\202\253", NULL};
static const char* kemuri12_patname[] = {"\202\255\202\353\202\261\202\260", "\202\346\202\244\202\252\202\361\202\240\202\302\202\242", "\203h\203K\203{\203\223\202\337\202\264\202\337", NULL};
static const char* balloon_heiho_patname[] = {"\220\324", "\220\302", "\211\251", NULL};
static const char* rubble_patname[] = {"rock", "wall", "block", NULL};
static const char* snow_dust_patname[] = {"\203A\203C\203X\203X\203^\203\223\203v\202\344\202\253\202\275\202\334", "\203A\203C\203X\203X\203^\203\223\203v/\203p\203\211\203\214\203\211\201[\202\344\202\253\202\266\202\351\202\265", "\203}\203\212\203I\202\323\202\301\202\251\202\302/\203L\203\211\203L\203\211", "\203f\203\202:\202\331\202\265\202\267\202\242\202\261\202\335/\202\261\202\304\202\242\203L\203\211\203L\203\211", "\202\331\202\265\202\314\202\313\202\252\202\242\202\355\202\264/\202\331\202\265\202\331\202\265", "\203~\203j\202\331\202\265\202\331\202\265", "\202\334\202\307\202\355\202\271\202\314\225\262", NULL};
static const char* fireworks_patname[] = {"\202\323\202\302\202\244\202\273\202\306\202\252\202\355", "\202\323\202\302\202\244\202\244\202\277\202\252\202\355", "\203X\203^\201[\202\252\202\275", NULL};
static const char* vapor_patname[] = {"\203{\203X\203p\203b\203N\203\223 \202\344\202\260", "\202\307\202\255\201E\203A\203C\203X\203p\203b\203N\203\223 \202\242\202\253", "\202\344\202\253\202\261\202\361\202\261\202\361 \202\352\202\242\202\253", NULL};
static const char* kumokumo_chip_patname[] = {"\202\255\202\340\202\255\202\340\202\334\202\266\202\361", "\202\333\202\261\202\322\201[\202\314\202\355\202\264", NULL};
static const char* crystal_patname[] = {"\203f\203A\201[\203\213", "\203~\203X\203e\201[\203\213", "\202\331\202\265\202\314\202\271\202\242\202\314\202\262\202\261\202\244", "\203f\203A\201[\203\213 \203E\203G\201[\203u", "\203z\203V\203p\203\217\201[\203A\203b\203v \203f\203A\201[\203\213\203E\203G\201[\203u2", "\203~\203X\203e\201[\203\213 \203E\203G\201[\203u", "\203~\203X\203e\201[\203\213 \203E\203G\201[\203u2", "\203`\203\203\201[\203W", NULL};
static const char* packn_bfire_patname[] = {"\202\242\202\277\202\316\202\361", "\202\311\202\316\202\361", NULL};
static const char* kumokumo_patname[] = {"\203N\203\211\203E\203_\220\201\202\253\224\362\202\316\202\265", "\203R\203\213\203e\203X\213z\202\242\215\236\202\335", "\203o\203b\203e\203\223\203_\201[\213z\202\242\215\236\202\335", "\203u\203\212\203U\201[\203h\202P", "\203u\203\212\203U\201[\203h\202Q", NULL};
static const char* akari_pose_patname[] = {"AKR_B_1 \202\302\202\244\202\266\202\345\202\244", "AKR_B_2 \202\346\202\353\202\261\202\321", NULL};
static const char* breath_fire_patname[] = {"\203t\203@\203C\203A\203u\203\214\203X", "\203`\203\207\203\215\203`\203\207\203\215\203t\203@\203C\203A", "\202\315\202\361\202\346\202\244\203u\203\214\203X", NULL};
static const char* butterfly_patname[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", NULL};
static const char* cloud_patname[] = {"\203p\203\214\203b\203^", "\203J\203\201\203L", "\203J\203\201\203L \203o\201[\203j\203\223\203O", "\202\320\202\345\202\244\202\266\203e\203X\203g", NULL};
static const char* shock_patname[] = {"\203r\203\212\203r\203\212", "\203r\203\212\203r\203\212&Wave", "\202\240\202\251\202\350\203V\203r\203\214\202\315\202\307\202\244", "Wave", NULL};
static const char* pokopi_pcharge_patname[] = {"\203|\203R\203s\201[/\203W\203\205\203Q\203\200", "\203J\203\201\203b\203N\202\324\202\361\202\265\202\361\202\267\202\351\202\306\202\253", "\203N\203b\203p\202\302\202\246\202\302\202\251\202\244/\203T\203\223\203f\201[\203\213", "\202\323\202\301\202\251\202\302\202\267\202\351\202\306\202\253", "\203N\203b\203p\203p\203\217\201[\203A\203b\203v\202\277\202\343\202\244", "<\202\335\202\265\202\346\202\244>", "\202\315\202\361\202\346\202\244\203`\203\203\201[\203W", "<\202\335\202\265\202\346\202\244>", "<\202\335\202\265\202\346\202\244>", "\202\251\202\242\202\323\202\255", "<\202\335\202\265\202\346\202\244>", "<\202\335\202\265\202\346\202\244>", "<\202\335\202\265\202\346\202\244>", NULL};
static const char* opuku_jetw_patname[] = {"\203Q\203b\203\\\201[ \203X\203~\202\261\202\244\202\260\202\253", "\203I\203v\203N \203W\203F\203b\203g\202\323\202\361\202\265\202\341", NULL};
static const char* pokopi_thunder_patname[] = {"\202\315\202\361\202\346\202\244\202\314\203J\203~\203i\203\212", "\203N\203b\203p\202\302\202\246", "<\202\302\202\251\202\355\202\310\202\242\202\261\202\306>", NULL};
static const char* water_damage_patname[] = {"enemy", "mario", "\202\261\202\355\202\301\202\317\202\250\202\277\202\351", "\203W\203F\203b\203g\202\323\202\361\202\265\202\341", "\227\216\211\272\225\250\201i\220\205\201j", "mario jump", "mario stay", "mario dash", NULL};
static const char* dogabon_attack_patname[] = {"a", "b", NULL};
static const char* opuku_wgun_patname[] = {"a", "b", NULL};
static const char* opuku_wprotect_patname[] = {"a", "b", NULL};
static const char* pturn_patname[] = {"\202\322\202\346\202\350\203^\201[\203\223", "\202\250\202\255\202\277\202\311\203`\203\203\203b\203N", NULL};
static const char* pokopi_pipo_patname[] = {"a", "b", NULL};
static const char* str_fireflower[] = {"\217\207\225\373\214\374", "\213t\225\373\214\374", NULL};
static const char* str_fall_leaf[] = {"\227\216\202\277\227t64-1", "\227\216\202\277\227t64-2", "\227\216\202\277\227t GC-1", "\227\216\202\277\227t GC-2", "\227\216\202\277\227t GC-3", NULL};
#endif
eff_set eff_set_n64[] = {
#if VERSION <= VERSION_DEMO_US
    {0, "expbom_n64", "\223G\202\252\202\265\202\361\202\266\202\341\202\244\202\306\202\253", NULL},
    {1, "kemuri1_n64", "\223y\211\214\222\205\222n", kemuri_patname},
    {2, "kemuri2_n64", "\223y\211\214\203_\203b\203V\203\205", kemuri_dash_patname},
    {3, "kemuri3_n64", "\211\324\211\214\203W\203\203\203\223\203v", NULL},
    {4, "kemuri4_n64", "\211\324\211\214\203_\203b\203V\203\205", NULL},
    {5, "kemuri5_n64", "\211_\211\214\222\205\222n", NULL},
    {6, "kemuri6_n64", "\211_\211\214\225\340\202\253", NULL},
    {7, "kemuri7_n64", "\220\341\202\240\202\265\202\240\202\306", NULL},
    {8, "flower_n64", "\202\250\211\324\202\252\202\314\202\332\202\301\202\304\202\242\202\255", NULL},
    {9, "snow_n64", "\220\341\202\314\214\213\217\273\202\252\215~\202\351", NULL},
    {10, "sstar_n64", "\227\254\202\352\220\257", sstar_patname},
    {11, "stardust_n64", "\203X\203^\201[\203_\203X\203g", stardust_patname},
    {12, "magic1_n64", "\203J\203\201\203b\203N\202\314\226\202\226@\202P", NULL},
    {13, "@Pymagic2_n64@Pw", "\203J\203\201\203b\203N\202\314\226\202\226@\202Q", magic2_patname},
    {14, "fall_leaf_n64", "\227\216\202\277\227t", str_fall_leaf},
    {15, "hokori_n64", "\203z\203R\203\212", hokori_patname},
    {16, "gareki_n64", "\202\252\202\352\202\253", NULL},
    {17, "kemuri8_n64", "\202\257\202\336\202\350\203s\203\223\203L\201[\203{\203\200\227p", kemu_pinky_patname},
    {18, "small_star_n64", "\215\327\202\251\202\242\220\257", small_star_patname},
    {19, "bomb_n64", "bom", pinky_patname},
    {20, "wangflush_n64", "\203\217\203\223\203\217\203\223\203t\203\211\203b\203V\203\205", wangflush_patname},
    {21, "magic3_n64", "\203}\203W\203b\203N\202R", NULL},
    {22, "kemuri9_n64", "\203n\203\223\203}\201[\211\214", hammerkemu_patname},
    {23, "kemuri10_n64", "\203J\203\201\203b\203N\202\331\202\244\202\253\202\257\202\336\202\350", NULL},
    {24, "@Pyfire_n64@Pw", "\202\331\202\314\202\250", NULL},
    {25, "rappa_n64", "\203\211\203b\203p\221\220", NULL},
    {26, "stardust2_n64", "\203X\203^\201[\203_\203X\203g\202Q", stardust2_patname},
    {27, "ripple_n64", "\202\250\202\325\202\255\202\314\224g\226\344", ripple_patname},
    {28, "@Pyexplosion_n64@Pw", "\224\232\224\255", NULL},
    {29, "kemuri11_n64", "\203h\203K\203{\203\223\202\257\202\336\202\350", dogabon_kemuri_patname},
    {30, "onpu_n64", "\211\271\225\204", onpu_patname},
    {31, "kemuri12_n64", "\203R\203\215\203R\203Q\211\214", kemuri12_patname},
    {32, "sweat_n64", "\212\276", sweat_patname},
    {33, "balloon_n64", "\203h\203K\203{\203\223\225\227\221D", NULL},
    {34, "cool_leaf_n64", "\202\263\202\324\202\301\201@\227\216\227t", NULL},
    {35, "bush_n64", "\202\265\202\260\202\335", NULL},
    {36, "rothammer_success_n64", "\211\361\223]\203n\203\223\203}\201[\220\254\214\367", NULL},
    {37, "rothammer_fail_n64", "\211\361\223]\203n\203\223\203}\201[\216\270\224s", NULL},
    {38, "guruguru_n64", "\202\256\202\351\202\256\202\351", NULL},
    {39, "ice_n64", "\203A\203C\203X\203o\203b\203`", NULL},
    {40, "stamp_n64", "\202\323\202\335\202\302\202\257", stamp_patname},
    {41, "@Pythruhammer_n64@Pw", "\202\251\202\361\202\302\202\244\203n\203\223\203}\201[", thruhammer_patname},
    {42, "radiation_n64", "\217W\222\206", radiation_patname},
    {43, "thunder_n64", "\202\251\202\335\202\310\202\350", thunder_patname},
    {44, "breath_fire_n64", "\211\316\202\360\223f\202\255", breath_fire_patname},
    {45, "broken_barrier_n64", "\203o\203\212\203A\224j\211\363", broken_barrier_patname},
    {46, "fire_ring_n64", "\203I\201[\203v\203j\203\223\203O", NULL},
    {47, "fire_dust_n64", "fire_ring \202\252\216g\227p\202\267\202\351", NULL},
    {48, "@Pyflame_n64@Pw", "flame", flame_patname},
    {49, "blow_n64", "\225\254\220\205", NULL},
    {50, "bomhei_n64", "\203o\203g\203\213\202\314\223G\203{\203\200\203w\203C", NULL},
    {51, "fireflower_n64", "\203t\203@\203C\203\204\201[\203t\203\211\203\217\201[", str_fireflower},
    {52, "recover_heart_n64", "\211\361\225\234\203n\201[\203g", recover_heart_patname},
    {53, "pturn_n64", "\215s\223\256\225s\224\\", pturn_patname},
    {54, "hit_n64", "\203q\203b\203g", hit_patname},
    {55, "confetti_n64", "\216\206\202\323\202\324\202\253", confetti_patname},
    {56, "snowfall_n64", "\220\341\202\252\215~\202\351", NULL},
    {57, "whirlwind_n64", "\203}\203\212\203I\201E\223G\202\314\227x\202\350\211\361\223]", whirlwind_patname},
    {58, "thunderflare_n64", "\226\202\226@\203J\203\201\203b\203N\217\361", thunderflare_patname},
    {59, "coin_n64", "\203R\203C\203\223\216\346\202\301\202\275\202\306\202\253", NULL},
    {60, "shock_n64", "\203o\203g\203\213\227p", shock_patname},
    {61, "balloon_heiho_n64", "\203o\203g\203\213\201E\203o\203\213\201[\203\223\203w\203C\203z\201[", balloon_heiho_patname},
    {62, "rubble_n64", "\203o\203g\203\213\201E\203z\203\211\203I\203E", rubble_patname},
    {63, "magic_horao_n64", "\203z\203\211\203I\203E\203o\203g\203\213", NULL},
    {64, "lensflare_n64", "\203T\203\223\203f\201[\203\213\201E\203r\203b\203O\203J\203\223\203e\203\211", NULL},
    {65, "crystal_n64", "\203N\203\212\203X\203^\203\213", crystal_patname},
    {66, "butterfly_n64", "\203W\203\203\203\223\203O\203\213\224\362\202\361\202\305\202\351", butterfly_patname},
    {67, "akari_charge_n64", "\203A\203J\203\212\201E\202\255\202\340\202\255\202\340\226\202\220l\201E\203s\203J\203\212\203\223", NULL},
    {68, "akaro_sflush_n64", "\203A\203J\203\212\202\314\213Z", NULL},
    {69, "opuku_wgun_n64", "\203I\203v\203N\202\314\213Z", opuku_wgun_patname},
    {70, "opuku_wprotect_n64", "\203I\203v\203N\202\314\213Z", opuku_wprotect_patname},
    {71, "opuku_wcurtain_n64", "\203I\203v\203N\202\314\213Z", NULL},
    {72, "opuku_jetw_n64", "\203I\203v\203N\201E\203Q\203b\203\\\201[", opuku_jetw_patname},
    {73, "opuku_jetwrain_n64", "\203I\203v\203N", NULL},
    {74, "pokopi_thunder_n64", "\203|\203R\203s\201[", pokopi_thunder_patname},
    {75, "water_damage_n64", "\220\205\203_\203\201\201[\203W", water_damage_patname},
    {76, "snowman_n64", "\202\344\202\253\202\342\202\261\202\361\202\261\202\361", NULL},
    {77, "holography_n64", "\203z\203\215\203O\203\211\203t", NULL},
    {78, "timestamp_n64", "\203o\203g\203\213", NULL},
    {79, "packn_bfire_n64", "\203p\203b\203N\203\223\203t\203\211\203\217\201[", packn_bfire_patname},
    {80, "pokopi_pipo_n64", "\203|\203R\203s\201[\203p\203C\203|", pokopi_pipo_patname},
    {81, "cloud_n64", "\202\255\202\340", cloud_patname},
    {82, "dogabon_attack_n64", "\203h\203K\203{\203\223", dogabon_attack_patname},
    {83, "kameki_tornade_n64", "\203o\203g\203\213\203J\203\201\203L", NULL},
    {84, "stone_smoke_n64", "\203~\203j\203~\203j\201E\203X\203g\201[\203\223\202\332\202\244\202\265", NULL},
    {85, "pokopi_pcharge_n64", "\203|\203R\203s\201[\203`\203\203\201[\203W", pokopi_pcharge_patname},
    {86, "hagetaka_flush_n64", "\203o\203g\203\213\201E\203C\203\217\203n\203Q", NULL},
    {87, "kumokumo_n64", "\202\255\202\340\202\255\202\340", kumokumo_patname},
    {88, "vapor_n64", "\202\344\202\260", vapor_patname},
    {89, "fire_spark_n64", "\203o\203g\203\213\201E\203t\203@\203C\203A\201[\203p\203b\203N\203\223", NULL},
    {90, "horao_spell_n64", "\203o\203g\203\213\201E\203z\203\211\203I\203E", NULL},
    {91, "snow_dust_n64", "\203\206\203L\202\314\211\214", snow_dust_patname},
    {92, "akari_pose_n64", "\202\240\202\251\202\350\203|\201[\203Y", akari_pose_patname},
    {93, "kumokumo_chip_n64", "\202\255\202\340\202\255\202\340\226\202\220l\201E\203|\203R\203s\201[\213Z", kumokumo_chip_patname},
    {94, "fireworks_n64", "\211\324\211\316", fireworks_patname},
    {95, "glass_n64", "\203K\203\211\203X", glass_patname},
    {96, "booboo_n64", "\203u\201[\203u\201[\202\263\202\361", NULL},
    {97, "pikkyolo_n64", "\202\322\202\301\202\253\202\345\202\353", NULL},
    {98, "freeze_n64", "\203t\203\212\201[\203Y", NULL},
    {99, "sunlight_n64", "\221\276\227z\214\365\220\374", sunlight_patname},
    {100, "release_n64", "\211\360\225\372", release_patname},
    {101, "peach_dust_n64", "\203s\201[\203`\203_\203X\203g", peach_dust_patname},
    {102, "star_power_n64", "\203X\203^\201[\203p\203\217\201[", star_power_patname},
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
s32 _rand_table[] = {
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
#if VERSION <= VERSION_DEMO_US
        sprintf(path, "%s/eff/jp/effect_n64.tpl", getMarioStDvdRoot());
#else
#if VERSION >= VERSION_US
        lang = gp->language == LANGUAGE_JP ? "jp" : "us";
        sprintf(path, "%s/e/%s/effect_n64.tpl", getMarioStDvdRoot(), lang);
#else
        sprintf(path, "%s/e/jp/effect_n64.tpl", getMarioStDvdRoot());
#endif
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

/** Returns a repeatable pseudo-random value in 0..rand taken from a fixed 128-entry table at index (mod 128). */
u32 effTblRandN64(u32 rand, s32 index) {
    s32 val = _rand_table[index % 128];
    if (val < 0) {
        val = -val;
    }
    return val % (s32)(rand + 1);
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
