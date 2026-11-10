// NPC kind table: the model and animation names of every NPC type.

#include "version.h"
#include "data/npc_data.h"
#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "npc_event.h"

/**
 * Every NPC and field-enemy kind, keyed by its Japanese name: model, stand/talk/walk/run animations, size,
 * and footstep, jump and landing sounds. npcGetTribe and the map setup look kinds up here when creating NPCs.
 */
NpcTribeInfo npcTribe[] = {
    {"クリスチーヌ", "c_pkr", "PKR_S_1", "PKR_Z_1", "PKR_S_1", "PKR_T_1", "PKR_W_1", "PKR_R_1", "PKR_D_1", "PKR_D_1", 20.0f, 27.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_PARTY_MOVE1L", "SFX_FLD_NPC_PARTY_MOVE1R", 12000, "SFX_FLD_NPC_PARTY_JUMP1", "SFX_FLD_NPC_PARTY_LANDING1"}, // Goombella
    {"ノコノコ", "c_nokonoko", "NOK_S_1", "NOK_Z_1", "NOK_S_1", "NOK_T_1", "NOK_W_1", "NOK_R_1", "NOK_S_1", "NOK_S_1", 30.0f, 35.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_NOKO_MOVE1L", "SFX_FLD_NPC_NOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Koopa Troopa (NPC)
    {"キノピオ", "c_kinopio", "KNP_S_1", "KNP_Z_1", "KNP_S_1", "KNP_T_1", "KNP_W_1", "KNP_R_1", "KNP_S_1", "KNP_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad
    {"キノピオ青", "c_kinopio_b", "KNP_S_1", "KNP_Z_1", "KNP_S_1", "KNP_T_1", "KNP_W_1", "KNP_R_1", "KNP_S_1", "KNP_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad, blue
    {"キノピオ緑", "c_kinopio_g", "KNP_S_1", "KNP_Z_1", "KNP_S_1", "KNP_T_1", "KNP_W_1", "KNP_R_1", "KNP_S_1", "KNP_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad, green
    {"キノピオ女", "c_kinopiko", "KNF_S_1", "KNF_Z_1", "KNF_S_1", "KNF_T_1", "KNF_W_1", "KNF_R_1", "KNF_S_1", "KNF_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad, female
    {"キノピオ女桃", "c_kinopiko_p", "KNF_S_1", "KNF_Z_1", "KNF_S_1", "KNF_T_1", "KNF_W_1", "KNF_R_1", "KNF_S_1", "KNF_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad, female, pink
    {"キノピオ女紫", "c_kinopiko_v", "KNF_S_1", "KNF_Z_1", "KNF_S_1", "KNF_T_1", "KNF_W_1", "KNF_R_1", "KNF_S_1", "KNF_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad, female, purple
    {"キノピオ子", "c_kokinopio", "CKP_S_1", "CKP_Z_1", "CKP_S_1", "CKP_T_1", "CKP_W_1", "CKP_R_1", "CKP_S_1", "CKP_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_S_MOVE1L", "SFX_FLD_NPC_KINOPIO_S_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad child
    {"キノピオ子緑", "c_kokinopio_g", "CKP_S_1", "CKP_Z_1", "CKP_S_1", "CKP_T_1", "CKP_W_1", "CKP_R_1", "CKP_S_1", "CKP_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad child, green
    {"キノピオ子青", "c_kokinopio_b", "CKP_S_1", "CKP_Z_1", "CKP_S_1", "CKP_T_1", "CKP_W_1", "CKP_R_1", "CKP_S_1", "CKP_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad child, blue
    {"キノピオ子黄緑", "c_kokinopio_y", "CKP_S_1", "CKP_Z_1", "CKP_S_1", "CKP_T_1", "CKP_W_1", "CKP_R_1", "CKP_S_1", "CKP_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad child, yellow-green
    {"情開キノピコ", "c_jpiko_p", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Toadette kind (lit. "情開 Toadette"; meaning unclear, no user found)
    {"フラワーさん", "c_flower", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_FLOWER_MOVE1L", "SFX_FLD_NPC_FLOWER_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Bub-ulb
    {"フラワーさん青", "c_flower_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_FLOWER_MOVE1L", "SFX_FLD_NPC_FLOWER_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Bub-ulb, blue
    {"フラワーさん黄", "c_flower_y", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_FLOWER_MOVE1L", "SFX_FLD_NPC_FLOWER_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Bub-ulb, yellow
    {"パワーアップ屋", "c_deale", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_POWER_MOVE1L", "SFX_FLD_NPC_POWER_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Merlon (power-up shop)
    {"パワーダウン屋", "c_levela", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_POWER_MOVE1L", "SFX_FLD_NPC_POWER_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Chet Rippo (power-down shop)
    {"星マニア", "c_mania", "MNA_S_1", "MNA_Z_1", "MNA_S_1", "MNA_T_1", "MNA_W_1", "MNA_R_1", "MNA_Z_1", "MNA_Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STARMANIA_MOVE1L", "SFX_FLD_NPC_STARMANIA_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Dazzle (Star Piece collector)
    {"うらない師", "c_uranai", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_POWER_MOVE1L", "SFX_FLD_NPC_POWER_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Merluvlee (fortune-teller)
    {"まじない師", "c_majinai", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_POWER_MOVE1L", "SFX_FLD_NPC_POWER_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Merlee (charmer)
    {"情報屋ケチ", "c_joho_a", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_INFO_MOVE1L", "SFX_FLD_NPC_INFO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Wonky (stingy informant)
    {"情報屋完璧", "c_joho_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_INFO_MOVE1L", "SFX_FLD_NPC_INFO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Grifty (thorough informant)
    {"ボッタクール", "c_botta", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 40.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_BOTTAKURU_MOVE1L", "SFX_FLD_NPC_BOTTAKURU_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Charlieton
    {"店員男", "c_gshop_m", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad shopkeeper, male (Thriff T., Plenn T.)
    {"店員女", "c_gshop_w", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toad shop clerk, female
    {"バッジ屋店員", "c_tyusan_t", "CHU_S_1", "CHU_Z_1", "CHU_S_1", "CHU_T_1", "CHU_W_1", "CHU_R_1", "CHU_S_1", "CHU_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // badge shop clerk
#if VERSION >= VERSION_US
    {"バニーテレサ", "c_bteresa2", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 25.0f, 38.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_TELESA_MOVE1", NULL, 0, NULL, NULL}, // Lahla
#else
    {"バニーテレサ", "c_bteresa", "BTS_S_1", "BTS_Z_1", "BTS_S_1", "BTS_T_1", "BTS_W_1", "BTS_R_1", "BTS_Z_1", "BTS_Z_1", 25.0f, 38.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_TELESA_MOVE1", NULL, 0, NULL, NULL}, // Lahla
#endif
    {"キザ野郎", "c_kiza", "Z_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 45.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KIZA_MOVE1L", "SFX_FLD_NPC_KIZA_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Dupree
    {"ノコタロウ", "c_pnk", "PNK_S_1", "PNK_Z_1", "PNK_S_1", "PNK_T_1", "PNK_W_1", "PNK_R_1", "PNK_S_1", "PNK_S_1", 30.0f, 35.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PARTY_MOVE1L", "SFX_FLD_NPC_PARTY_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Koops
    {"ノコタロウ父", "c_nokopapa", "NKP_S_1", "NKP_Z_1", "NKP_S_1", "NKP_T_1", "NKP_W_1", "NKP_R_1", "NKP_S_1", "NKP_S_1", 40.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_NOKO_MOVE1L", "SFX_FLD_NPC_NOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Koopley, Koops's father
    {"ノコ村村長", "c_nokoson", "NKS_S_1", "NKS_Z_1", "NKS_S_1", "NKS_T_1", "NKS_W_1", "NKS_R_1", "NKS_S_1", "NKS_S_1", 30.0f, 35.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_NOKO_MOVE1L", "SFX_FLD_NPC_NOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Kroop, mayor of Petalburg
    {"ノコリン", "c_nokorin", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 35.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_NOKO_MOVE1L", "SFX_FLD_NPC_NOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Koopie Koo
    {"ゴンババ", "c_gonbaba", "GNB_S_1", "GNB_Z_1", "GNB_S_1", "GNB_T_1", "GNB_S_1", "GNB_S_1", "GNB_S_1", "GNB_S_1", 450.0f, 20.0f, 0.0f, 0.0f, -50.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Hooktail
    {"ゴンババ２", "c_gonbaba2", "GNB_F_1", "GNB_F_1", "GNB_F_1", "GNB_F_1", "GNB_F_1", "GNB_F_1", "GNB_F_1", "GNB_F_1", 20.0f, 20.0f, 0.0f, 0.0f, -50.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Hooktail (second model)
    {"ゴンババ影", "c_gonbaba3", "GNB_k_1", "GNB_k_1", "GNB_k_1", "GNB_k_1", "GNB_k_1", "GNB_k_1", "GNB_k_1", "GNB_k_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // lit. "Hooktail shadow"
    {"赤ホネノコ", "c_karon_r_h", "KRN_S_1", "KRN_Z_1", "KRN_S_1", "KRN_S_1", "KRN_W_1", "KRN_R_1", "KRN_D_2", "KRN_D_2", 20.0f, 60.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_HONENOKO_MOVE1L", "SFX_FLD_ENM_HONENOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Red Bones
    {"チュチュリーナ", "c_tyutyu", "PCH_S_1", "PCH_Z_1", "PCH_S_1", "PCH_T_1", "PCH_W_1", "PCH_R_1", "PCH_S_1", "PCH_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PARTY_MOVE1L", "SFX_FLD_NPC_PARTY_MOVE1R", 12000, "SFX_FLD_NPC_PARTY_JUMP1", "SFX_FLD_NPC_PARTY_LANDING1"}, // Ms. Mowz
    {"ガジガジ", "c_gazigazi", "GAZ_S_1", "GAZ_Z_1", "GAZ_S_1", "GAZ_A_1", "GAZ_A_1", "GAZ_A_1", "GAZ_A_1", "GAZ_A_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PUKUPUKUI_MOVE1", NULL, 0, NULL, NULL}, // the creature that bites Mario in hazard water
    {"ホネノコ", "c_karon_g_h", "KRN_S_1", "KRN_Z_1", "KRN_S_1", "KRN_S_1", "KRN_W_1", "KRN_R_1", "KRN_D_2", "KRN_D_2", 20.0f, 60.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_HONENOKO_MOVE1L", "SFX_FLD_ENM_HONENOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Dull Bones
    {"ビビアン", "c_vivian", "PTR_S_1", "PTR_Z_1", "PTR_S_1", "PTR_T_1", "PTR_W_1", "PTR_R_1", "PTR_D_1", "PTR_D_1", 30.0f, 38.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PARTY_HANG1", "SFX_FLD_NPC_PARTY_HANG1", 40000, "SFX_EVT_VIVI_JUMP1", NULL}, // Vivian
    {"ビビアンべそ", "c_vivian", "PTR_S_3", "PTR_S_3", "PTR_S_3", "PTR_T_3", "PTR_W_3", "PTR_R_3", "PTR_R_3", "PTR_R_3", 30.0f, 38.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PARTY_HANG1", "SFX_FLD_NPC_PARTY_HANG1", 40000, "SFX_EVT_VIVI_JUMP1", NULL}, // Vivian, crying
    {"ビビアン困る", "c_vivian", "PTR_S_4", "PTR_S_4", "PTR_S_4", "PTR_T_5", "PTR_W_3", "PTR_R_3", "PTR_R_3", "PTR_R_3", 30.0f, 38.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PARTY_HANG1", "SFX_FLD_NPC_PARTY_HANG1", 40000, "SFX_EVT_VIVI_JUMP1", NULL}, // Vivian, troubled
    {"マリリン", "c_maririn", "MRR_S_1", "MRR_Z_1", "MRR_S_1", "MRR_T_1", "MRR_W_1", "MRR_R_1", "MRR_D_1", "MRR_D_1", 30.0f, 53.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MARI_HANG1", "SFX_FLD_NPC_MARI_HANG1", 6000, "SFX_EVT_MARI_JUMP1", NULL}, // Marilyn
    {"マジョリン", "c_majyorin", "MJR_S_1", "MJR_Z_1", "MJR_S_1", "MJR_T_1", "MJR_W_1", "MJR_R_1", "MJR_D_1", "MJR_D_1", 30.0f, 33.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MAJO_HANG1", "SFX_FLD_NPC_MAJO_HANG1", 6000, "SFX_EVT_MAJO_JUMP1", NULL}, // Beldam
    {"クラウダ", "c_windy", "PWD_S_1", "PWD_Z_1", "PWD_S_1", "PWD_T_1", "PWD_W_1", "PWD_R_1", "PWD_S_1", "PWD_S_1", 30.0f, 47.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PARTY_HANG1", "SFX_FLD_NPC_PARTY_HANG1", 40000, NULL, NULL}, // Flurrie
    {"第三勢力総統", "c_shuryo", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 65.0f, 65.0f, 0.0f, 0.0f, -10.0f, 50.0f, "SFX_FLD_NPC_SYURYO_MOVE1L", "SFX_FLD_NPC_SYURYO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Sir Grodus
    {"第三勢力総統（ヒビ）", "c_shuryo", "S_3", "Z_2", "S_3", "T_4", "W_2", "R_2", "D_4", "D_4", 65.0f, 65.0f, 0.0f, 0.0f, -10.0f, 50.0f, "SFX_FLD_NPC_SYURYO_MOVE1L", "SFX_FLD_NPC_SYURYO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Sir Grodus, cracked
    {"第三勢力幹部", "c_kanbu_ibt", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 60.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KANBU_MOVE1L", "SFX_FLD_NPC_KANBU_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Lord Crump
    {"第三勢力部下", "c_zako_n", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 33.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_3RD1_MOVE1L", "SFX_FLD_ENM_3RD1_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // X-Naut
    {"第三勢力研究員", "c_zako_m", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 33.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_3RD1_MOVE1L", "SFX_FLD_ENM_3RD1_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // X-Naut PhD (researcher)
    {"第三勢力エリート", "c_zako_e", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 33.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_3RD1_MOVE1L", "SFX_FLD_ENM_3RD1_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Elite X-Naut
    {"パレッタ", "c_paretta", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 30.0f, "SFXEVT_OPN_PARETTA_MOVE1", "SFXEVT_OPN_PARETTA_MOVE1", 12000, NULL, NULL}, // Parakarry
    {"モコモコ", "c_mokomoko", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 30.0f, "SFX_FLD_NPC_MOKOMOKO_MOVE1L", "SFX_FLD_NPC_MOKOMOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // lit. "fluffy" (モコモコ; no user found)
    {"ペラ魔人", "c_p_majin", "S_1", "Z_1", "S_1", "T_1", "Z_1", "Z_1", "Z_1", "Z_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 30.0f, NULL, NULL, 0, NULL, NULL}, // lit. "paper demon" (used by the lecture scenes)
    {"ルイージ", "c_luigi", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 22.0f, 47.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_LUIGI_MOVE1L", "SFX_FLD_NPC_LUIGI_MOVE1R", 12000, "SFX_FLD_NPC_LUIGI_JUMP1", "SFX_FLD_NPC_LUIGI_LANDING1"}, // Luigi
    {"キック", "c_lp_gesso", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Blooey, Luigi's Blooper friend
    {"チェリー", "c_lp_bom", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Jerry, Luigi's Bob-omb friend
    {"トルク", "c_lp_met", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Torque, Luigi's friend
    {"ラクガン", "c_lp_pansy", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Hayzee, Luigi's Dayzee friend
    {"ス・クリーミ", "c_lp_moa", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Screamy, Luigi's friend
    {"キック２", "c_lp_gesso", "S_2", "Z_2", "S_2", "T_2", "W_2", "R_2", "S_2", "S_2", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Blooey (second kind)
    {"デモピーチ標準", "c_peach", "P_S_1", "P_Z_1", "P_S_1", "P_T_1", "P_W_1", "P_R_1", "P_S_1", "P_S_1", 30.0f, 50.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Peach (demo), standing
    {"デモピーチ標準2", "c_peach", "P_S_7", "P_Z_7", "P_S_7", "P_T_13", "P_W_7", "P_R_7", "P_S_7", "P_S_7", 30.0f, 50.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Peach (demo), standing 2
    {"デモピーチ物持ち", "c_peach", "P_S_8", "P_Z_8", "P_S_8", "P_T_14", "P_W_8", "P_R_8", "P_S_8", "P_S_8", 30.0f, 50.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Peach (demo), holding something
    {"デモピーチ不安", "c_peach", "P_S_4", "P_Z_4", "P_S_4", "P_T_4", "P_W_4", "P_R_4", "P_S_4", "P_S_4", 30.0f, 50.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, NULL, NULL}, // Peach (demo), worried
    {"謎の人", "c_nazo", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_NAZO_MOVE1L", "SFX_FLD_NPC_NAZO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the hooded merchant of the opening story
    {"キノじい", "c_kino_ji", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toadsworth
    {"盗賊", "c_borodo_n", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_THIEF_MOVE1L", "SFX_FLD_NPC_THIEF_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a bandit
    {"盗賊団", "c_monban", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MONBAN_MOVE1L", "SFX_FLD_NPC_MONBAN_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Robbo gang member
    {"盗賊団親分", "c_binbo", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_THIEF_MOVE1L", "SFX_FLD_NPC_THIEF_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Ishnail, leader of the Robbos
    {"マフィア", "c_m_buka", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 35.0f, 55.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MONTE_MOVE1L", "SFX_FLD_NPC_MONTE_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Pianta mobster
    {"マフィアボス", "c_m_boss", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 35.0f, 55.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MONTE_MOVE1L", "SFX_FLD_NPC_MONTE_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Don Pianta
    // Frankie (ピートン) and Francesca (モニー), the Pianta couple; the original symbols call them peton and mony.
    {"ピートン", "c_m_waka", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 35.0f, 55.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MONTE_MOVE1L", "SFX_FLD_NPC_MONTE_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Frankie
    {"ピートン（指輪）", "c_m_waka", "S_2", "Z_2", "S_2", "T_2", "W_2", "R_2", "S_2", "S_2", 35.0f, 55.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MONTE_MOVE1L", "SFX_FLD_NPC_MONTE_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Frankie (ring scene)
    {"モニー", "c_m_ojo", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 35.0f, 55.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MONTE_MOVE1L", "SFX_FLD_NPC_MONTE_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Francesca
    {"モニー（指輪）", "c_m_ojo", "S_2", "Z_2", "S_2", "T_2", "W_2", "R_2", "S_2", "S_2", 35.0f, 55.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MONTE_MOVE1L", "SFX_FLD_NPC_MONTE_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Francesca (ring scene)
    {"クリハカセ", "c_kuri_h", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "d_1", "d_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KURI_MOVE1L", "SFX_FLD_NPC_KURI_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Professor Frankly
    {"マスター", "c_pub_m", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MASTER_MOVE1L", "SFX_FLD_NPC_MASTER_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Podley, the innkeeper
    {"マスター（地下）", "c_barten", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MASTER_MOVE1L", "SFX_FLD_NPC_MASTER_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // lit. "master (underground)", the underground bar owner
    {"クラガリさん", "c_f_mura", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STG4_MOVE1L", "SFX_FLD_NPC_STG4_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Darkly
    {"ナンシー", "c_cook", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Zess T.
    {"飛行船係員", "c_puku_s", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Stewart, the blimp conductor
    {"水夫ボム兵", "c_suifu_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Bob-omb sailor
    {"クリボーゴロ", "c_kuribo", "KUR_S_1", "KUR_Z_2", "KUR_S_1", "KUR_T_1", "KUR_W_1", "KUR_R_1", "KUR_D_1", "KUR_D_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KURI_MOVE1L", "SFX_FLD_NPC_KURI_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // lit. "Goomba, goro" (meaning unclear, no user found)
    {"トゲクリゴロ", "c_kuribo", "TGK_S_1", "TGK_Z_2", "TGK_S_1", "TGK_T_1", "TGK_W_1", "TGK_R_1", "TGK_D_1", "TGK_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KURI_MOVE1L", "SFX_FLD_NPC_KURI_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // lit. "Spiky Goomba, goro" (meaning unclear, no user found)
    {"パタクリゴロ", "c_kuribo", "PTK_S_1", "PTK_Z_2", "PTK_S_1", "PTK_T_1", "PTK_W_1", "PTK_R_1", "PTK_D_1", "PTK_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KURI_MOVE1L", "SFX_FLD_NPC_KURI_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // lit. "Paragoomba, goro" (meaning unclear, no user found)
    {"NPCクリボー", "c_kuribo_n", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KURI_MOVE1L", "SFX_FLD_NPC_KURI_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Goomba (NPC)
    {"NPCボム兵", "c_bom_n", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Bob-omb (NPC)
    {"門番", "c_monban", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MONBAN_MOVE1L", "SFX_FLD_NPC_MONBAN_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a gatekeeper
    {"ゲッソー", "c_gesso4", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", 50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Blooper
    {"ゲッソーのゲソ", "c_gesso4", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", "GSO_Z_1", 20.0f, 10.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Blooper's tentacle
    {"デブボムヘイ", "c_bom_deb", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // lit. "fat Bob-omb"
    {"チューさん", "c_tyusan", "CHU_S_1", "CHU_Z_1", "CHU_S_1", "CHU_T_1", "CHU_W_1", "CHU_R_1", "CHU_S_1", "CHU_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_CHUSAN_MOVE1L", "SFX_FLD_NPC_CHUSAN_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Squeek (mouse townsperson)
    {"町人チュチュリーナ", "c_tyutyu_t", "CHU_S_1", "CHU_Z_1", "CHU_S_1", "CHU_T_1", "CHU_W_1", "CHU_R_1", "CHU_S_1", "CHU_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 30.0f, "SFX_FLD_NPC_PARTY_MOVE1L", "SFX_FLD_NPC_PARTY_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Ms. Mowz, lit. "townsperson Ms. Mowz"
    {"ロテン", "c_roten", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 100.0f, "SFX_FLD_NPC_ROTEN_MOVE1L", "SFX_FLD_NPC_ROTEN_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Doogan
    {"ロテン赤", "c_roten_o", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_ROTEN_MOVE1L", "SFX_FLD_NPC_ROTEN_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Doogan, red
    {"ロテン緑", "c_roten_g", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_ROTEN_MOVE1L", "SFX_FLD_NPC_ROTEN_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Doogan, green
    {"偽ホネノコ", "c_karon_p", "KRN_S_1", "KRN_Z_1", "KRN_S_1", "KRN_S_1", "KRN_W_1", "KRN_R_1", "KRN_D_2", "KRN_D_2", 30.0f, 60.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_HONENOKO_MOVE1L", "SFX_FLD_ENM_HONENOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // lit. "fake Dull Bones"
    {"プニ族", "c_puni", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 24.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // a Puni
    {"トゲ族", "c_toge", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // a Jabbi
    {"ガイド", "c_pguide", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 24.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Punio
    {"ガイド妹", "c_psister", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 24.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Petuni
    {"ライバル", "c_prival", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Puniper
    {"ちびトゲ", "c_tchibi", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Jabble
    {"プニ族長老", "c_psoncho", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // the Puni elder
    {"プニ店員", "c_pshop", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 24.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Pungent, the Puni shopkeeper
    {"プニ情報屋", "c_pjoho", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 24.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Pungry, the Puni informant
    {"ボスロボット", "c_mb_robo_1a", "S_3", "Z_2", "S_3", "T_3", "W_1", "W_1", "S_3", "S_3", 30.0f, 60.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_STG2_BOSS_MOVE1L", "SFX_STG2_BOSS_MOVE1R", 48000, NULL, NULL}, // Magnus von Grapple
    {"ボスロボットバツ", "c_mb_robo_c", "Z_1", "Z_1", "Z_1", "Z_1", "Z_1", "Z_1", "Z_1", "Z_1", 30.0f, 60.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // the X mark on Magnus von Grapple
    {"ファビオ", "c_kinopio", "KNP_S_1", "KNP_Z_1", "KNP_S_1", "KNP_T_1", "KNP_W_1", "KNP_R_1", "KNP_S_1", "KNP_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Doe T.
    {"ブレッドハート", "c_bred_h", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_3", "D_3", 50.0f, 60.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_STG3_BLED_MOVE1L", "SFX_STG3_BLED_MOVE1R", 12000, "SFX_STG3_BLED_JUMP1", NULL}, // Rawk Hawk
    {"ブレッドハートベルト", "c_bred_h", "S_9", "Z_9", "S_9", "T_9", "W_3", "R_2", "D_3", "D_3", 50.0f, 60.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_STG3_BLED_MOVE1L", "SFX_STG3_BLED_MOVE1R", 12000, "SFX_STG3_BLED_JUMP1", NULL}, // Rawk Hawk's Champ's Belt
    {"キノシコワ", "c_ksikowa", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_STG3_KINOSIKOWA_MOVE1_L", "SFX_STG3_KINOSIKOWA_MOVE1R", 8000, "SFX_STG3_KINOSIKOWA_JUMP1", "SFX_STG3_KINOSIKOWA_LANDING1"}, // Jolene
    {"キノシコワ（マイク）", "c_ksikowa", "S_2", "Z_2", "S_2", "T_2", "W_2", "R_2", "Z_2", "Z_2", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_STG3_KINOSIKOWA_MOVE1_L", "SFX_STG3_KINOSIKOWA_MOVE1R", 8000, "SFX_STG3_KINOSIKOWA_JUMP1", "SFX_STG3_KINOSIKOWA_LANDING1"}, // Jolene with a microphone
    {"美女", "c_bijo", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the mysterious beauty (美女; no name in the game)
    {"ガンス", "c_gans", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 40.0f, 50.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_STG3_GANSU_MOVE1L", "SFX_STG3_GANSU_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Grubba
    {"ガンスマイク", "c_gans", "S_2", "Z_2", "S_2", "T_2", "W_2", "R_2", "Z_2", "Z_2", 40.0f, 50.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_STG3_GANSU_MOVE1L", "SFX_STG3_GANSU_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Grubba with a microphone
    {"ガードマン", "c_guard", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 50.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_STG3_GUARDMAN_MOVE1L", "SFX_STG3_GUARDMAN_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Glitz Pit security guard
    {"たまご", "c_babyyoshi", "PYS_E_1", "PYS_E_1", "PYS_E_1", "PYS_E_1", "PYS_E_1", "PYS_E_1", "PYS_E_1", "PYS_E_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, "SFX_STG3_YOSH_EGG_JUMP2", NULL}, // the egg that hatches into Yoshi
    {"ヨッシー", "c_babyyoshi", "PYS_S_1", "PYS_Z_1", "PYS_S_1", "PYS_T_1", "PYS_W_1", "PYS_R_1", "PYS_S_1", "PYS_S_1", 30.0f, 28.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_PARTY_MOVE1L", "SFX_PARTY_MOVE1R", 12000, NULL, NULL}, // Yoshi kid
    {"踊り子", "c_dance_r", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Traveling Sisters 3 dancer, red
    {"踊り子ピンク", "c_dance_p", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Traveling Sisters 3 dancer, pink
    {"踊り子黄緑", "c_dance_k", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Traveling Sisters 3 dancer, yellow-green
    {"ホットドッグ屋", "c_hotdog", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_STG3_HOTDOG_MOVE1L", "SFX_STG3_HOTDOG_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Mr. Hoggle, the hot dog seller
    {"リポーター", "c_report", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a reporter
    {"ファイター", "c_kinopio", "KNP_S_1", "KNP_Z_1", "KNP_S_1", "KNP_T_1", "KNP_W_1", "KNP_R_1", "KNP_S_1", "KNP_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_STG3_FIGHTING1", "SFX_STG3_FIGHTING1", 10000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a fighter
    {"キノシチョフ", "c_k_chof", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Prince Mush, Jolene's brother
    {"ＮＰＣジュゲム", "c_jugem_n", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_JUGEM_MOVE1", NULL, 0, NULL, NULL}, // Lakitu (NPC)
    {"ボロドーキング", "c_borodo_g", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_BORODO_MOVE1L", "SFX_FLD_NPC_BORODO_MOVE1R", 12000, NULL, NULL}, // lit. "Bandit king" (no user found)
    {"カメラマン", "c_jugem_c", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Lakitu cameraman
    {"NPCボム兵２", "c_bomhey", "BOM_S_1", "BOM_Z_1", "BOM_S_1", "BOM_T_1", "BOM_W_1", "BOM_R_1", "BOM_D_1", "BOM_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, NULL, NULL}, // Bob-omb (NPC, second kind)
    {"NPCハイパートゲメット", "c_met_h", "TMT_S_1", "TMT_Z_1", "TMT_S_1", "TMT_T_1", "TMT_W_1", "TMT_R_1", "TMT_D_1", "TMT_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_MET_MOVE1L", "SFX_FLD_NPC_MET_MOVE1R", 12000, "SFX_FLD_ENM_MET_ATTACK1", NULL}, // Hyper Spike Top (NPC)
    {"ウスグラ村人", "c_f_mura", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STG4_MOVE1L", "SFX_FLD_NPC_STG4_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Twilighter
    {"ウスグラ村人音付き", "c_f_mura", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_STG4_NPC_MOVE1L", "SFX_STG4_NPC_MOVE1R", 10000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Twilighter (with sound)
    {"ウスグラおばさん", "c_f_mama", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STG4_MOVE1L", "SFX_FLD_NPC_STG4_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Eve
    {"ウスグラ子供A", "c_f_child_a", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STG4_S_MOVE1L", "SFX_FLD_NPC_STG4_S_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Twilighter child A
    {"ウスグラ子供B", "c_f_child_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STG4_S_MOVE1L", "SFX_FLD_NPC_STG4_S_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Twilighter child B
    {"ウスグラ子供C", "c_f_child_c", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STG4_S_MOVE1L", "SFX_FLD_NPC_STG4_S_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Twilighter child C
    {"ウスグラ村長", "c_f_son", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STG4_MOVE1L", "SFX_FLD_NPC_STG4_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Mayor Dour of Twilight Town
    {"ウスグラ村長音付き", "c_f_son", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_STG4_NPC_MOVE1L", "SFX_STG4_NPC_MOVE1R", 10000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Mayor Dour of Twilight Town (with sound)
    {"ウスグラ店おじさん", "c_f_oji", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STG4_MOVE1L", "SFX_FLD_NPC_STG4_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Twilight Shop manager
    {"ウスグラ店おばさん", "c_f_oba", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_STG4_MOVE1L", "SFX_FLD_NPC_STG4_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Twilight Shop manager's wife
    {"にせマリオ", "c_mario", "EM_S_1", "EM_Z_1", "EM_S_1", "EM_T_1", "EM_W_1", "EM_R_1", "EM_D_1", "EM_D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_STG4_LANPEL_M_MOVE1L", "SFX_STG4_LANPEL_M_MOVE1R", 10000, "SFX_STG4_LANPEL_M_JUMP1", "STG4_LANPEL_M_LANDING1"}, // fake Mario (Doopliss)
#if VERSION >= VERSION_DEMO_EU
    {"にせマリオ２", "a_mario", "M_S_1", "M_Z_1", "M_S_1", "M_I_3", "M_W_1", "M_R_1", "M_D_1", "M_D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // fake Mario (Doopliss), second model
#else
    {"にせマリオ２", "a_mario", "M_S_1", "M_Z_1", "M_S_1", "M_I_2", "M_W_1", "M_R_1", "M_D_1", "M_D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // fake Mario (Doopliss), second model
#endif
    {"カラス", "c_f_karas", "S_1", "Z_1", "S_1", "T_1", "A_1", "A_1", "A_1", "A_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // a crow
    {"テレサ", "c_teresa", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_TELESA_MOVE1", NULL, 0, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Boo
    {"パープルテレサ", "c_teresa_p", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_TELESA_MOVE1", NULL, 0, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // lit. "purple Boo"
    {"アトミックテレサ", "c_atmic_trs", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, -15.0f, 20.0f, "SFX_FLD_ENM_TELESA_MOVE1", NULL, 0, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Atomic Boo
    {"ランペル", "c_ranpel", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_STG4_LANPEL_MOVE1L", "SFX_STG4_LANPEL_MOVE1R", 10000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Doopliss
    {"ランペル音付き", "c_ranpel", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_STG4_LANPEL_MOVE1L", "SFX_STG4_LANPEL_MOVE1R", 10000, "SFX_STG4_LANPEL_JUMP1", NULL}, // Doopliss (with sound)
    {"オウム", "c_f_owm", "S_1", "Z_1", "S_1", "T_1", "A_1", "A_1", "A_1", "A_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Doopliss's parrot
    {"ブタ", "c_bubusan", "S_1", "Z_1", "S_1", "T_1", "W_1", "W_1", "W_1", "W_1", 30.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // a pig (cursed Twilighter)
    {"ガラの悪い水夫", "c_gara", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 40.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_STG5_SAILER_MOVE1", "SFX_STG5_SAILER_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Pa-Patch
    {"マルコ", "c_marco", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 50.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 12000, "SFX_STG5_MARCO_LANDING1", "SFX_STG5_MARCO_JUMP1"}, // Flavio
    {"めがね水夫", "c_megane", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 60.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Four-Eyes
    {"オドオド水夫", "c_odo_k", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the timid Toad sailor
    {"水夫", "c_suifu_k", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Toad sailor
    {"ボロ水夫", "c_boro_k", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a shipwrecked Toad sailor
#if VERSION <= VERSION_DEMO_JP
    {"エルモス", "c_elmos", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_BUBBLE_MOVE1", NULL, 0, NULL, NULL}, // Ember
#else
    {"エルモス", "c_elmos", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_BUBBLE_MOVE1", NULL, 0, NULL, NULL}, // Ember
#endif
    {"サンダース", "c_bomt_n", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "D_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PARTY_MOVE1L", "SFX_FLD_NPC_PARTY_MOVE1R", 12000, "SFX_FLD_NPC_PARTY_JUMP1", "SFX_FLD_NPC_PARTY_LANDING1"}, // Admiral Bobbery
    {"サンダースへろへろ", "c_bomt_n", "D_2", "D_2", "D_2", "T_1", "W_3", "R_3", "D_1", "D_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PARTY_MOVE1L", "SFX_FLD_NPC_PARTY_MOVE1R", 12000, "SFX_FLD_NPC_PARTY_JUMP1", "SFX_FLD_NPC_PARTY_LANDING1"}, // Admiral Bobbery, exhausted
    {"サンダースねむる", "c_bomt_n", "N_1", "N_1", "N_1", "T_1", "W_3", "R_3", "D_1", "D_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_NPC_PARTY_MOVE1L", "SFX_FLD_NPC_PARTY_MOVE1R", 12000, "SFX_FLD_NPC_PARTY_JUMP1", "SFX_FLD_NPC_PARTY_LANDING1"}, // Admiral Bobbery, asleep
    {"コルテス", "c_korutesu3", "KRT_S_1", "KRT_Z_1", "KRT_S_1", "KRT_T_2", "KRT_W_1", "KRT_R_1", "KRT_D_1", "KRT_D_1", 60.0f, 60.0f, 0.0f, 0.0f, -15.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Cortez
    {"クリチェロ", "c_kuribo_n", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Goom Goom
#if VERSION <= VERSION_DEMO_JP
    {"コブロン", "c_koburon", "S_1", "Z_1", "S_1", "S_1", "S_1", "S_1", "D_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // the Whacka
#else
    {"コブロン", "c_koburon", "S_1", "Z_1", "S_1", "T_1", "S_1", "S_1", "D_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // the Whacka
#endif
    {"運転手", "c_unten", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the engineer of the Excess Express
    {"マダム", "c_rose", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toodles
    {"グルメキノピオ", "c_kinodebu", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Heff T.
    {"ポワン探偵", "c_powan", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Pennington
    {"ゆうれい", "c_ghost_k", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Ghost T.
    {"トロン", "c_haiyu", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "C_1", "C_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Zip Toad
    {"とほほトロン", "c_haiyu", "D_1", "Z_1", "D_1", "T_5", "W_1", "R_1", "C_1", "C_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Zip Toad, dejected
    {"サラリーマン", "c_smoko", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the Ratooey businessman
    {"電車店員", "c_hanbai", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // train sales clerk
    {"車掌", "c_shasho_r", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the conductor of the Excess Express
    {"車掌悲", "c_shasho_r", "O_1", "Z_1", "O_1", "T_3", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the conductor, sad
    {"コック", "c_puku_c", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Chef Shimi
    {"ウェイトレス", "c_waitress", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 999.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the waitress of the Excess Express
    {"ウェイトレス悲", "c_waitress", "P_1", "Z_1", "P_1", "T_5", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 999.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the waitress, sad
    {"ナリキンパパ", "c_papab", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Goldbob
    {"ナリキンママ", "c_mamab", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Sylvia
    {"コナリキン", "c_konari", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 15.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Bub
    {"駅員", "c_shasho_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a station worker
    {"土産屋", "c_kinopiko_g", "KNF_S_1", "KNF_Z_1", "KNF_S_1", "KNF_T_1", "KNF_W_1", "KNF_R_1", "KNF_S_1", "KNF_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Toadia
    {"ターくん", "c_ta-kun", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_KINOPIO_MOVE1L", "SFX_FLD_NPC_KINOPIO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Poshley Heights resident (lit. "Ta-kun")
    {"ヨッシー大", "c_yoshi", "YSH_S_1", "YSH_Z_1", "YSH_S_1", "YSH_T_1", "YSH_W_1", "YSH_R_1", "YSH_S_1", "YSH_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_YOSHI_MOVE1L", "SFX_FLD_NPC_YOSHI_MOVE1R", 0, NULL, NULL}, // lit. "big Yoshi"
    {"レサレサ", "c_resaresa", "S_1", "Z_1", "S_1", "T_1", "S_1", "S_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Bow
    {"セバスチャン", "c_sebaschan", "S_1", "Z_1", "S_1", "T_1", "S_1", "S_1", "S_1", "S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Bootler
    {"ボム村村長", "c_bomson", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // the mayor of Fahr Outpost
    {"ニトロ", "c_hito_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Nob, a Fahr Outpost Bob-omb
    {"ペントリット", "c_hito_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Rob, a Fahr Outpost Bob-omb
    {"スラリー", "c_hito_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Fred, a Fahr Outpost Bob-omb
    {"ボム村村人", "c_hito_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // a Fahr Outpost Bob-omb
    {"ホワイト将軍", "c_bwhite", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_BOMB_MOVE1", "SFX_FLD_NPC_BOMB_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // General White
    {"バッテンダー２", "c_mb_robo_2a", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "Z_1", "Z_1", 80.0f, 150.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 48000, "SFX_STG7_BOSS_JUMP1", "SFX_STG7_BOSS_LANDING1"}, // Magnus von Grapple 2.0
    {"カロン", "c_karon_h", "KRN_S_1", "KRN_Z_1", "KRN_S_1", "KRN_S_1", "KRN_W_1", "KRN_R_1", "KRN_D_1", "KRN_K_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_HONENOKO_MOVE1L", "SFX_FLD_ENM_HONENOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Dry Bones
    {"黒カロン", "c_karon_b_h", "KRN_S_1", "KRN_Z_1", "KRN_S_1", "KRN_S_1", "KRN_W_1", "KRN_R_1", "KRN_D_2", "KRN_D_2", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_HONENOKO_MOVE1L", "SFX_FLD_ENM_HONENOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Dark Bones
    {"ブンババ", "c_gonbaba_b", "GNB_S_1", "GNB_Z_1", "GNB_S_1", "GNB_T_1", "GNB_S_1", "GNB_S_1", "GNB_S_1", "GNB_S_1", 450.0f, 20.0f, 0.0f, 0.0f, -50.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Gloomtail
    {"クッパ", "c_koopa", "KPA_S_1", "KPA_Z_1", "KPA_S_1", "KPA_T_1", "KPA_W_1", "KPA_R_1", "KPA_D_1", "KPA_D_1", 60.0f, 75.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_KUPPA_MOVE1", "SFX_KUPPA_MOVE2", 12000, "SFX_KUPPA_JUMP1", "SFX_KUPPA_LANDING1"}, // Bowser
    {"ピーチ姫", "c_peach", "P_S_1", "P_Z_1", "P_S_1", "P_T_1", "P_W_1", "P_R_1", "P_S_1", "P_S_1", 20.0f, 47.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_PEACH_MOVE1L", "SFX_PEACH_MOVE1R", 12000, NULL, "SFX_PEACH_LANDING1"}, // Princess Peach
    {"ピーチ姫（普通）", "c_peach", "P_S_7", "P_Z_7", "P_S_7", "P_T_13", "P_W_7", "P_R_7", "P_S_7", "P_S_7", 20.0f, 47.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_PEACH_MOVE1L", "SFX_PEACH_MOVE1R", 12000, NULL, "SFX_PEACH_LANDING1"}, // Princess Peach (normal)
    {"ピーチ姫（ラスト用）", "c_peach_f", "P_S_9F", "P_Z_9F", "P_S_9F", "P_T_9F", "P_W_9F", "P_R_9F", "P_S_9F", "P_S_9F", 20.0f, 47.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_PEACH_MOVE1L", "SFX_PEACH_MOVE1R", 12000, NULL, "SFX_PEACH_LANDING1"}, // Princess Peach (final chapter)
    {"ブラックピーチ", "c_b_peach_b", "KRT_S_1", "KRT_Z_1", "KRT_S_1", "KRT_T_1", "KRT_W_1", "KRT_W_1", "KRT_S_1", "KRT_S_1", 20.0f, 47.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // the Shadow Queen in Peach's body
    {"影の女王", "c_q_kage", "KRT_S_1", "KRT_Z_1", "KRT_S_1", "KRT_T_1", "KRT_S_1", "KRT_S_1", "KRT_S_1", "KRT_S_1", 20.0f, 47.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // the Shadow Queen
#if VERSION <= VERSION_DEMO_JP
    {"トゲノコ", "c_togenoko", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_NOKO_MOVE3L", "SFX_FLD_ENM_NOKO_MOVE3R", 12000, "SFX_FLD_ENM_NOKO_JUMP1", NULL}, // Koopatrol
    {"NPCハンマーブロス", "c_burosu_h", "BRO_S_1", "BRO_Z_1", "BRO_S_1", "BRO_T_1", "BRO_W_1", "BRO_R_1", "BRO_D_1", "BRO_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_NOKO_MOVE1L", "SFX_FLD_NPC_NOKO_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Hammer Bro (NPC)
#else
#if VERSION >= VERSION_US
    {"トゲノコ", "c_togenoko", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_NOKO_MOVE3L", "SFX_FLD_ENM_NOKO_MOVE3R", 12000, "SFX_FLD_ENM_NOKO_JUMP1", NULL}, // Koopatrol
#else
    {"トゲノコ", "c_togenoko", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_NOKO_MOVE3L", "SFX_FLD_ENM_NOKO_MOVE3R", 12000, "SFX_FLD_ENM_NOKO_JUMP1", NULL}, // Koopatrol
#endif
    {"NPCハンマーブロス", "c_burosu_h", "BRO_S_1", "BRO_Z_1", "BRO_S_1", "BRO_T_1", "BRO_W_1", "BRO_R_1", "BRO_D_1", "BRO_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_NPC_NOKO_MOVE1L", "SFX_FLD_NPC_NOKO_MOVE1R", 6000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Hammer Bro (NPC)
#endif
    {"カメックババ", "c_kamek_be", "S_2", "Z_2", "S_2", "T_9", "W_2", "R_2", "S_2", "S_2", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 70.0f, NULL, NULL, 0, NULL, NULL}, // Kammy Koopa
    {"カメックババ（地上）", "c_kamek_be", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "S_1", "S_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 70.0f, "SFX_KUPPA_KAMEBABA_MOVE1L", "SFX_KUPPA_KAMEBABA_MOVE1R", 12000, "SFX_KUPPA_KAMEBABA_JUMP1", "SFX_KUPPA_KAMEBABA_LANDING1"}, // Kammy Koopa, on the ground
    {"カメックババ（息切れ）", "c_kamek_be", "I_1", "Z_1", "I_1", "T_3", "I_2", "I_2", "I_1", "I_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 70.0f, "SFX_KUPPA_KAMEBABA_MOVE1L", "SFX_KUPPA_KAMEBABA_MOVE1R", 12000, "SFX_KUPPA_KAMEBABA_JUMP1", "SFX_KUPPA_KAMEBABA_LANDING1"}, // Kammy Koopa, out of breath
    {"カメックババ（風船）", "c_kamek_be", "S_3", "Z_3", "S_3", "T_11", "W_3", "W_3", "S_3", "S_3", 30.0f, 50.0f, 0.0f, 0.0f, 0.0f, 70.0f, NULL, NULL, 0, NULL, NULL}, // Kammy Koopa, balloon
#if VERSION >= VERSION_US
    {"カメック", "c_kamek", "KMK_S_1", "KMK_Z_1", "KMK_S_1", "KMK_T_1", "KMK_W_1", "KMK_R_1", "KMK_D_1", "KMK_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Magikoopa
#else
    {"カメック", "c_kamek", "KMK_S_1", "KMK_Z_1", "KMK_S_1", "KMK_T_1", "KMK_W_1", "KMK_R_1", "KMK_S_1", "KMK_S_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Magikoopa
#endif
    {"飛行船クッパ", "c_hkoopa", "HKPA_S_1", "HKPA_Z_1", "HKPA_S_1", "HKPA_T_1", "HKPA_W_1", "HKPA_R_1", "HKPA_S_1", "HKPA_S_1", 40.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Bowser in his airship
    {"パタクリNPC", "c_kuribo", "PTK_S_1", "PTK_Z_2", "PTK_S_1", "PTK_T_1", "PTK_W_1", "PTK_R_1", "PTK_D_1", "PTK_D_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_PATAKURI_MOVE1", "SFX_FLD_ENM_PATAKURI_MOVE1", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Paragoomba (NPC)
    {"クリボー", "c_kuribo", "KUR_S_1", "KUR_Z_2", "KUR_S_1", "KUR_T_1", "KUR_W_1", "KUR_R_1", "KUR_D_1", "KUR_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KURI_MOVE1L", "SFX_FLD_ENM_KURI_MOVE1R", 12000, NULL, NULL}, // Goomba
    {"トゲクリ", "c_kuribo", "TGK_S_1", "TGK_Z_2", "TGK_S_1", "TGK_T_1", "TGK_W_1", "TGK_R_1", "TGK_D_1", "TGK_K_1", 20.0f, 28.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KURI_MOVE1L", "SFX_FLD_ENM_KURI_MOVE1R", 12000, NULL, NULL}, // Spiky Goomba
    {"パタクリ", "c_kuribo", "PTK_S_1", "PTK_Z_2", "PTK_S_1", "PTK_T_1", "PTK_W_1", "PTK_R_1", "PTK_D_1", "PTK_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_PATAKURI_MOVE1", "SFX_FLD_ENM_PATAKURI_MOVE1", 12000, NULL, NULL}, // Paragoomba
    {"ハイパークリボー", "c_kuribo_h", "KUR_S_1", "KUR_Z_2", "KUR_S_1", "KUR_T_1", "KUR_W_1", "KUR_R_1", "KUR_D_1", "KUR_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KURI_MOVE1L", "SFX_FLD_ENM_KURI_MOVE1R", 12000, NULL, NULL}, // Hyper Goomba
    {"ハイパートゲクリ", "c_kuribo_h", "TGK_S_1", "TGK_Z_2", "TGK_S_1", "TGK_T_1", "TGK_W_1", "TGK_R_1", "TGK_D_1", "TGK_K_1", 20.0f, 28.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KURI_MOVE1L", "SFX_FLD_ENM_KURI_MOVE1R", 12000, NULL, NULL}, // Hyper Spiky Goomba
    {"ハイパーパタクリ", "c_kuribo_h", "PTK_S_1", "PTK_Z_2", "PTK_S_1", "PTK_T_1", "PTK_W_1", "PTK_R_1", "PTK_D_1", "PTK_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_PATAKURI_MOVE1", "SFX_FLD_ENM_PATAKURI_MOVE1", 12000, NULL, NULL}, // Hyper Paragoomba
    {"ヤミクリボー", "c_kuribo_y", "KUR_S_1", "KUR_Z_2", "KUR_S_1", "KUR_T_1", "KUR_W_1", "KUR_R_1", "KUR_D_1", "KUR_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KURI_MOVE1L", "SFX_FLD_ENM_KURI_MOVE1R", 12000, NULL, NULL}, // Gloomba
    {"ヤミトゲクリ", "c_kuribo_y", "TGK_S_1", "TGK_Z_2", "TGK_S_1", "TGK_T_1", "TGK_W_1", "TGK_R_1", "TGK_D_1", "TGK_K_1", 20.0f, 28.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KURI_MOVE1L", "SFX_FLD_ENM_KURI_MOVE1R", 12000, NULL, NULL}, // Spiky Gloomba
    {"ヤミパタクリ", "c_kuribo_y", "PTK_S_1", "PTK_Z_2", "PTK_S_1", "PTK_T_1", "PTK_W_1", "PTK_R_1", "PTK_D_1", "PTK_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_PATAKURI_MOVE1", "SFX_FLD_ENM_PATAKURI_MOVE1", 12000, NULL, NULL}, // Paragloomba
    {"ホネノコ敵", "c_karon_g_h", "KRN_S_1", "KRN_Z_1", "KRN_S_1", "KRN_T_1", "KRN_W_1", "KRN_R_1", "KRN_D_1", "KRN_K_1", 24.0f, 36.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_HONENOKO_MOVE1L", "SFX_FLD_ENM_HONENOKO_MOVE1R", 12000, NULL, NULL}, // Dull Bones (enemy)
    {"ボム兵", "c_bomhey", "BOM_S_1", "BOM_Z_1", "BOM_S_1", "BOM_T_1", "BOM_W_1", "BOM_R_1", "BOM_D_1", "BOM_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_BOMB_MOVE1", "SFX_FLD_ENM_BOMB_MOVE1", 12000, NULL, NULL}, // Bob-omb
#if VERSION <= VERSION_DEMO_JP
    {"メット", "c_met", "MET_S_1", "MET_Z_1", "MET_S_1", "MET_T_1", "MET_W_1", "MET_R_1", "MET_D_1", "MET_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_MET_MOVE1L", "SFX_FLD_ENM_MET_MOVE1R", 12000, "SFX_FLD_ENM_MET_ATTACK1", NULL}, // Buzzy Beetle
    {"トゲメット", "c_met", "TMT_S_1", "TMT_Z_1", "TMT_S_1", "TMT_T_1", "TMT_W_1", "TMT_R_1", "TMT_D_1", "TMT_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_MET_MOVE1L", "SFX_FLD_ENM_MET_MOVE1R", 12000, "SFX_FLD_ENM_MET_ATTACK1", NULL}, // Spike Top
    {"パタトゲメット", "c_met", "PTM_S_1", "PTM_Z_1", "PTM_S_1", "PTM_T_1", "PTM_W_1", "PTM_R_1", "PTM_D_1", "PTM_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_PATAMET_MOVE1", "SFX_FLD_ENM_PATAMET_MOVE1", 12000, NULL, NULL}, // Spiky Parabuzzy
    {"パタメット", "c_met", "PMT_S_1", "PMT_Z_1", "PMT_S_1", "PMT_T_1", "PMT_W_1", "PMT_R_1", "PMT_D_1", "PMT_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_PATAMET_MOVE1", "SFX_FLD_ENM_PATAMET_MOVE1", 12000, NULL, NULL}, // Parabuzzy
    {"ハイパーメット", "c_met_h", "MET_S_1", "MET_Z_1", "MET_S_1", "MET_T_1", "MET_W_1", "MET_R_1", "MET_D_1", "MET_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_MET_MOVE1L", "SFX_FLD_ENM_MET_MOVE1R", 12000, "SFX_FLD_ENM_MET_ATTACK1", NULL}, // Hyper Buzzy Beetle
    {"ハイパートゲメット", "c_met_h", "TMT_S_1", "TMT_Z_1", "TMT_S_1", "TMT_T_1", "TMT_W_1", "TMT_R_1", "TMT_D_1", "TMT_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_MET_MOVE1L", "SFX_FLD_ENM_MET_MOVE1R", 12000, "SFX_FLD_ENM_MET_ATTACK1", NULL}, // Hyper Spike Top
    {"ハイパーパタトゲメット", "c_met_h", "PTM_S_1", "PTM_Z_1", "PTM_S_1", "PTM_T_1", "PTM_W_1", "PTM_R_1", "PTM_D_1", "PTM_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_PATAMET_MOVE1", "SFX_FLD_ENM_PATAMET_MOVE1", 12000, NULL, NULL}, // Hyper Spiky Parabuzzy
    {"ハイパーパタメット", "c_met_h", "PMT_S_1", "PMT_Z_1", "PMT_S_1", "PMT_T_1", "PMT_W_1", "PMT_R_1", "PMT_D_1", "PMT_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_PATAMET_MOVE1", "SFX_FLD_ENM_PATAMET_MOVE1", 12000, NULL, NULL}, // Hyper Parabuzzy
#else
    {"メット", "c_met", "MET_S_1", "MET_Z_1", "MET_S_1", "MET_T_1", "MET_W_1", "MET_R_1", "MET_D_1", "MET_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_MET_MOVE1L", "SFX_FLD_ENM_MET_MOVE1R", 12000, "SFX_FLD_ENM_MET_ATTACK1", NULL}, // Buzzy Beetle
    {"トゲメット", "c_met", "TMT_S_1", "TMT_Z_1", "TMT_S_1", "TMT_T_1", "TMT_W_1", "TMT_R_1", "TMT_D_1", "TMT_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_MET_MOVE1L", "SFX_FLD_ENM_MET_MOVE1R", 12000, "SFX_FLD_ENM_MET_ATTACK1", NULL}, // Spike Top
    {"パタトゲメット", "c_met", "PTM_S_1", "PTM_Z_1", "PTM_S_1", "PTM_T_1", "PTM_W_1", "PTM_R_1", "PTM_D_1", "PTM_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_PATAMET_MOVE1", "SFX_FLD_ENM_PATAMET_MOVE1", 12000, NULL, NULL}, // Spiky Parabuzzy
    {"パタメット", "c_met", "PMT_S_1", "PMT_Z_1", "PMT_S_1", "PMT_T_1", "PMT_W_1", "PMT_R_1", "PMT_D_1", "PMT_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_PATAMET_MOVE1", "SFX_FLD_ENM_PATAMET_MOVE1", 12000, NULL, NULL}, // Parabuzzy
    {"ハイパーメット", "c_met_h", "MET_S_1", "MET_Z_1", "MET_S_1", "MET_T_1", "MET_W_1", "MET_R_1", "MET_D_1", "MET_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_MET_MOVE1L", "SFX_FLD_ENM_MET_MOVE1R", 12000, "SFX_FLD_ENM_MET_ATTACK1", NULL}, // Hyper Buzzy Beetle
    {"ハイパートゲメット", "c_met_h", "TMT_S_1", "TMT_Z_1", "TMT_S_1", "TMT_T_1", "TMT_W_1", "TMT_R_1", "TMT_D_1", "TMT_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_MET_MOVE1L", "SFX_FLD_ENM_MET_MOVE1R", 12000, "SFX_FLD_ENM_MET_ATTACK1", NULL}, // Hyper Spike Top
    {"ハイパーパタトゲメット", "c_met_h", "PTM_S_1", "PTM_Z_1", "PTM_S_1", "PTM_T_1", "PTM_W_1", "PTM_R_1", "PTM_D_1", "PTM_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_PATAMET_MOVE1", "SFX_FLD_ENM_PATAMET_MOVE1", 12000, NULL, NULL}, // Hyper Spiky Parabuzzy
    {"ハイパーパタメット", "c_met_h", "PMT_S_1", "PMT_Z_1", "PMT_S_1", "PMT_T_1", "PMT_W_1", "PMT_R_1", "PMT_D_1", "PMT_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_PATAMET_MOVE1", "SFX_FLD_ENM_PATAMET_MOVE1", 12000, NULL, NULL}, // Hyper Parabuzzy
#endif
    {"サンボ", "c_sanbo", "SNB_S_1", "SNB_Z_1", "SNB_S_1", "SNB_T_1", "SNB_W_1", "SNB_W_1", "SNB_D_1", "SNB_K_1", 20.0f, 80.0f, 0.0f, 0.0f, 0.0f, 999.0f, "SFX_FLD_ENM_SAMBO_MOVE1", NULL, 20000, NULL, NULL}, // Pokey
    {"サンボマミー", "c_sanbo_m", "SNB_S_1", "SNB_Z_1", "SNB_S_1", "SNB_T_1", "SNB_W_1", "SNB_W_1", "SNB_D_1", "SNB_K_1", 20.0f, 80.0f, 0.0f, 0.0f, 0.0f, 999.0f, "SFX_FLD_ENM_SAMBO_MOVE1", NULL, 20000, NULL, NULL}, // lit. "Pokey mummy"
    {"シンエモン", "c_sinemon", "SIN_S_1", "SIN_Z_1", "SIN_S_1", "SIN_T_1", "SIN_W_1", "SIN_R_1", "SIN_D_1", "SIN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_EMON_MOVE1L", "SFX_FLD_ENM_EMON_MOVE1R", 12000, NULL, NULL}, // Cleft
    {"ハイパーシンエモン", "c_sinemon_h", "SIN_S_1", "SIN_Z_1", "SIN_S_1", "SIN_T_1", "SIN_W_1", "SIN_R_1", "SIN_D_1", "SIN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_EMON_MOVE1L", "SFX_FLD_ENM_EMON_MOVE1R", 12000, NULL, NULL}, // Hyper Cleft
    {"モノクロシンエモン", "c_sinemon_w", "SIN_S_1", "SIN_Z_1", "SIN_S_1", "SIN_T_1", "SIN_W_1", "SIN_R_1", "SIN_D_1", "SIN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_EMON_MOVE1L", "SFX_FLD_ENM_EMON_MOVE1R", 12000, NULL, NULL}, // Cleft, monochrome
    {"シンノスケ", "c_sinnosuke", "SIS_S_1", "SIS_Z_1", "SIS_S_1", "SIS_T_1", "SIS_W_1", "SIS_R_1", "SIS_D_1", "SIS_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_EMON_MOVE1L", "SFX_FLD_ENM_EMON_MOVE1R", 12000, NULL, NULL}, // Bald Cleft
    {"バサバサ", "c_basabasa", "BSA_S_1", "BSA_S_1", "BSA_S_1", "BSA_T_1", "BSA_S_1", "BSA_S_1", "BSA_D_1", "BSA_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_BASA_MOVE1", "SFX_FLD_ENM_BASA_MOVE1", 12000, NULL, NULL}, // Swooper
    {"バサバサチュルル", "c_cyuru", "CYU_S_1", "CYU_S_1", "CYU_S_1", "CYU_T_1", "CYU_S_1", "CYU_S_1", "CYU_D_1", "CYU_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_BASA_MOVE1", "SFX_FLD_ENM_BASA_MOVE1", 12000, NULL, NULL}, // Swoopula
#if VERSION <= VERSION_DEMO_JP
    {"バサバサグリーン", "c_basabasa_g", "CYU_S_1", "CYU_S_1", "CYU_S_1", "CYU_T_1", "CYU_S_1", "CYU_S_1", "CYU_D_1", "CYU_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_BASA_MOVE1", "SFX_FLD_ENM_BASA_MOVE1", 12000, NULL, NULL}, // Swampire
#else
    {"バサバサグリーン", "c_basabasa_g", "BSA_S_1", "BSA_S_1", "BSA_S_1", "BSA_T_1", "BSA_S_1", "BSA_S_1", "BSA_D_1", "BSA_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_BASA_MOVE1", "SFX_FLD_ENM_BASA_MOVE1", 12000, NULL, NULL}, // Swampire
#endif
    {"ノコノコ敵", "c_nokoteki", "NKT_S_1", "NKT_Z_1", "NKT_S_1", "NKT_T_1", "NKT_W_1", "NKT_R_1", "NKT_D_1", "NKT_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_NOKO_MOVE3L", "SFX_FLD_ENM_NOKO_MOVE3R", 12000, "SFX_FLD_ENM_NOKO_JUMP1", NULL}, // Koopa Troopa (enemy)
    {"パタパタ", "c_nokoteki", "PTP_S_1", "PTP_Z_1", "PTP_S_1", "PTP_T_1", "PTP_W_1", "PTP_R_1", "PTP_D_1", "PTP_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 30.0f, "SFX_FLD_ENM_PATA_MOVE1", "SFX_FLD_ENM_PATA_MOVE1", 12000, NULL, NULL}, // Paratroopa
    {"ヤミノコノコ", "c_yaminoko", "NKT_S_1", "NKT_Z_1", "NKT_S_1", "NKT_T_1", "NKT_W_1", "NKT_R_1", "NKT_D_1", "NKT_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_NOKO_MOVE3L", "SFX_FLD_ENM_NOKO_MOVE3R", 12000, "SFX_FLD_ENM_NOKO_JUMP1", NULL}, // Dark Koopa
    {"ヤミパタパタ", "c_yaminoko", "PTP_S_1", "PTP_Z_1", "PTP_S_1", "PTP_T_1", "PTP_W_1", "PTP_R_1", "PTP_D_1", "PTP_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 30.0f, "SFX_FLD_ENM_PATA_MOVE1", "SFX_FLD_ENM_PATA_MOVE1", 12000, NULL, NULL}, // Dark Paratroopa
    {"ノコファイター", "c_touginoko", "NKT_S_1", "NKT_Z_1", "NKT_S_1", "NKT_T_1", "NKT_W_1", "NKT_R_1", "NKT_D_1", "NKT_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_NOKO_MOVE3L", "SFX_FLD_ENM_NOKO_MOVE3R", 12000, "SFX_FLD_ENM_NOKO_JUMP1", NULL}, // Koopa fighter (Glitz Pit)
    {"パタファイター", "c_touginoko", "PTP_S_1", "PTP_Z_1", "PTP_S_1", "PTP_T_1", "PTP_W_1", "PTP_R_1", "PTP_D_1", "PTP_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 30.0f, "SFX_FLD_ENM_PATA_MOVE1", "SFX_FLD_ENM_PATA_MOVE1", 12000, NULL, NULL}, // Paratroopa fighter (Glitz Pit)
    {"チョロボン", "c_chorobon", "CBN_S_1", "CBN_Z_1", "CBN_S_1", "CBN_T_1", "CBN_W_1", "CBN_R_1", "CBN_D_1", "CBN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, "SFX_FLD_ENM_CHORO1_MOVE1", NULL}, // Fuzzy
    {"グリーンチョロボン", "c_chorobon_g", "CBN_S_1", "CBN_Z_1", "CBN_S_1", "CBN_T_1", "CBN_W_1", "CBN_R_1", "CBN_D_1", "CBN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, "SFX_FLD_ENM_CHORO1_MOVE1", NULL}, // Green Fuzzy
    {"フラワーチョロボン", "c_chorobon_f", "CBN_S_1", "CBN_Z_1", "CBN_S_1", "CBN_T_1", "CBN_W_1", "CBN_R_1", "CBN_D_1", "CBN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, "SFX_FLD_ENM_CHORO1_MOVE1", NULL}, // Flower Fuzzy
    {"ゴールドチョロボン", "c_chorobon_k", "CBN_S_1", "CBN_Z_1", "CBN_S_1", "CBN_T_1", "CBN_W_1", "CBN_R_1", "CBN_D_1", "CBN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, "SFX_FLD_ENM_CHORO1_MOVE1", NULL}, // Gold Fuzzy
    {"パンジーさん", "c_pansy", "PAN_S_1", "PAN_Z_1", "PAN_S_1", "PAN_T_1", "PAN_W_1", "PAN_R_1", "PAN_D_1", "PAN_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_PANZI_MOVE1L", "SFX_FLD_ENM_PANZI_MOVE1R", 12000, NULL, NULL}, // Crazee Dayzee
    {"きらめくパンジーさん", "c_kpansy", "PAN_S_1_K", "PAN_Z_1_K", "PAN_S_1_K", "PAN_T_1_K", "PAN_W_1_K", "PAN_R_1_K", "PAN_D_1_K", "PAN_K_1_K", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_PANZI_MOVE1L", "SFX_FLD_ENM_PANZI_MOVE1R", 12000, NULL, NULL}, // Amazy Dayzee
#if VERSION <= VERSION_DEMO_JP
    {"キラー大砲", "c_killtai", "KTA_S_1", "KTA_Z_1", "KTA_S_1", "KTA_T_1", "KTA_S_1", "KTA_S_1", "KTA_D_1", "KTA_K_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Bill Blaster
    {"キラー", "c_kilr", "KIL_S_1", "KIL_Z_1", "KIL_S_1", "KIL_T_1", "KIL_W_1", "KIL_R_1", "KIL_D_1", "KIL_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Bullet Bill
    {"スーパーキラー大砲", "c_killtai_g", "KTA_S_1", "KTA_Z_1", "KTA_S_1", "KTA_T_1", "KTA_S_1", "KTA_S_1", "KTA_D_1", "KTA_K_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Bombshell Bill Blaster
#else
    {"キラー大砲", "c_killtai", "KTA_S_1", "KTA_Z_1", "KTA_S_1", "KTA_T_1", "KTA_S_1", "KTA_S_1", "KTA_D_1", "KTA_K_1", 45.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Bill Blaster
    {"キラー", "c_kilr", "KIL_S_1", "KIL_Z_1", "KIL_S_1", "KIL_T_1", "KIL_W_1", "KIL_R_1", "KIL_D_1", "KIL_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Bullet Bill
    {"スーパーキラー大砲", "c_killtai_g", "KTA_S_1", "KTA_Z_1", "KTA_S_1", "KTA_T_1", "KTA_S_1", "KTA_S_1", "KTA_D_1", "KTA_K_1", 45.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Bombshell Bill Blaster
#endif
    {"スーパーキラー", "c_kilr_g", "KIL_S_1", "KIL_Z_1", "KIL_S_1", "KIL_T_1", "KIL_W_1", "KIL_R_1", "KIL_D_1", "KIL_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Bombshell Bill
    {"トゲダルマー", "c_togedaruma", "TGD_S_1", "TGD_Z_1", "TGD_S_1", "TGD_T_1", "TGD_S_1", "TGD_S_1", "TGD_D_1", "TGD_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Bristle
    {"ヤミトゲダルマー", "c_togedaruma_b", "TGD_S_1", "TGD_Z_1", "TGD_S_1", "TGD_T_1", "TGD_S_1", "TGD_S_1", "TGD_D_1", "TGD_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Dark Bristle
    {"パックンフラワー", "c_pakflwr", "PKF_S_1", "PKF_Z_1", "PKF_S_1", "PKF_T_1", "PKF_W_1", "PKF_Z_1", "PKF_D_1", "PKF_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Pale Piranha
    {"パックンフラワー（灰）", "c_pakflwr_t", "PKF_S_1", "PKF_Z_1", "PKF_S_1", "PKF_T_1", "PKF_W_1", "PKF_Z_1", "PKF_D_1", "PKF_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Pale Piranha, grey
    {"ポイズンパックン", "c_pakflwr_p", "PKF_S_1", "PKF_Z_1", "PKF_S_1", "PKF_T_1", "PKF_W_1", "PKF_Z_1", "PKF_D_1", "PKF_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Putrid Piranha
    {"アイスパックン", "c_pakflwr_a", "PKF_S_1", "PKF_Z_1", "PKF_S_1", "PKF_T_1", "PKF_W_1", "PKF_Z_1", "PKF_D_1", "PKF_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Frost Piranha
    {"シンエモン（灰）", "c_sinemon_w", "SIN_S_1", "SIN_Z_1", "SIN_S_1", "SIN_S_1", "SIN_W_1", "SIN_R_1", "SIN_D_1", "SIN_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_EMON_MOVE1L", "SFX_FLD_ENM_EMON_MOVE1R", 12000, NULL, NULL}, // Cleft, grey
    {"ドクガッスン", "c_kmoon_g", "S_1", "Z_1", "S_1", "T_1", "W_1", "Z_1", "D_1", "K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KUMO_MOVE1", NULL, 0, NULL, NULL}, // Poison Puff
    {"パイダース", "c_paid", "PAI_S_1", "PAI_Z_1", "PAI_S_1", "PAI_Z_1", "PAI_W_1", "PAI_Z_1", "PAI_D_1", "PAI_K_1", 30.0f, 60.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Pider
    {"チュランタラー", "c_paid_b", "PAI_S_1", "PAI_Z_1", "PAI_S_1", "PAI_Z_1", "PAI_W_1", "PAI_Z_1", "PAI_D_1", "PAI_K_1", 30.0f, 60.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Arantula
    {"バリアーン", "c_baria", "S_1", "Z_1", "S_1", "Z_1", "W_1", "R_1", "D_1", "K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Yux
    {"バリアーンZ", "c_baria_z", "S_1", "Z_1", "S_1", "Z_1", "W_1", "R_1", "D_1", "K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Z-Yux
    {"バリアーンカスタム", "c_baria_c", "S_1", "Z_1", "S_1", "Z_1", "W_1", "R_1", "D_1", "K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // X-Yux
    {"軍団ザコ敵", "c_zako_n", "Z_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 30.0f, 33.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_3RD1_MOVE1L", "SFX_FLD_ENM_3RD1_MOVE1R", 12000, NULL, NULL}, // X-Naut (enemy)
    {"軍団ザコ敵エリート", "c_zako_e", "Z_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 30.0f, 33.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_3RD1_MOVE1L", "SFX_FLD_ENM_3RD1_MOVE1R", 12000, NULL, NULL}, // Elite X-Naut (enemy)
    {"軍団ザコ敵研究員", "c_zako_m", "Z_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 30.0f, 33.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_3RD1_MOVE1L", "SFX_FLD_ENM_3RD1_MOVE1R", 12000, NULL, NULL}, // X-Naut PhD (enemy)
    {"ボロドー", "c_borodo", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_BORODO_MOVE1L", "SFX_FLD_ENM_BORODO_MOVE1R", 12000, NULL, NULL}, // Bandit
    {"バッジボロドー", "c_borodo_t", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_BORODO_MOVE1L", "SFX_FLD_ENM_BORODO_MOVE1R", 12000, NULL, NULL}, // Badge Bandit
    {"２Ｄクリボー", "c_kuri_kn", "S_1", "Z_1", "S_1", "D_2", "W_1", "R_1", "D_1", "K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 60.0f, NULL, NULL, 0, NULL, NULL}, // Goomba (2D)
    {"２Ｄ軍団ザコ", "c_zako_kn", "S_1", "Z_1", "S_1", "D_2", "W_1", "R_1", "D_1", "K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 60.0f, NULL, NULL, 0, NULL, NULL}, // X-Naut (2D)
    {"２Ｄ軍団ザコ研究員", "c_zako_km", "S_1", "Z_1", "S_1", "D_2", "S_1", "S_1", "D_1", "K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 60.0f, NULL, NULL, 0, NULL, NULL}, // X-Naut PhD (2D)
    {"２Ｄゲッソー", "c_geso_kn", "S_1", "Z_1", "S_1", "Z_1", "A_1", "A_1", "D_1", "K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 60.0f, NULL, NULL, 0, NULL, NULL}, // Blooper (2D)
    {"ジュゲム", "c_jugemu", "JGM_S_1", "JGM_Z_1", "JGM_S_1", "JGM_T_1", "JGM_W_1", "JGM_R_1", "JGM_D_1", "JGM_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_JUGEM_MOVE1", NULL, 0, NULL, NULL}, // Lakitu
    {"ハイパージュゲム", "c_jugemu_b", "JGM_S_1", "JGM_Z_1", "JGM_S_1", "JGM_T_1", "JGM_W_1", "JGM_R_1", "JGM_D_1", "JGM_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_JUGEM_MOVE1", NULL, 0, NULL, NULL}, // Dark Lakitu
    {"ウラノコ", "c_uranoko", "NKT_S_1", "NKT_Z_1", "NKT_S_1", "NKT_T_1", "NKT_W_1", "NKT_R_1", "NKT_D_1", "NKT_K_1", 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_NOKO_MOVE3L", "SFX_FLD_ENM_NOKO_MOVE3R", 12000, "SFX_FLD_ENM_NOKO_JUMP1", NULL}, // Shady Koopa
    {"ボムへい", "c_bomhey", "BOM_S_1", "BOM_Z_1", "BOM_S_1", "BOM_T_1", "BOM_W_1", "BOM_R_1", "BOM_D_1", "BOM_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_BOMB_MOVE1", "SFX_FLD_ENM_BOMB_MOVE1", 12000, NULL, NULL}, // Bob-omb
    {"クロクモーン", "c_kmoon", "S_1", "Z_1", "S_1", "Z_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_KUMO_MOVE1", NULL, 0, NULL, NULL}, // Dark Puff
    {"ブリザード", "c_kmoon_b", "S_1", "Z_1", "S_1", "Z_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_KUMO_MOVE1", NULL, 0, NULL, NULL}, // Ice Puff
    {"モノクロクロクモーン", "c_kmoon_wb", "S_1", "Z_1", "S_1", "Z_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_KUMO_MOVE1", NULL, 0, NULL, NULL}, // Dark Puff, monochrome
    {"トゲゾー", "c_togezo", "TGZ_S_1", "TGZ_Z_1", "TGZ_S_1", "TGZ_S_1", "TGZ_W_1", "TGZ_R_1", "TGZ_D_1", "TGZ_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Spiny
    {"ハイパーシンノスケ", "c_sinnosuke_h", "SIS_S_1", "SIS_Z_1", "SIS_S_1", "SIS_T_1", "SIS_W_1", "SIS_R_1", "SIS_D_1", "SIS_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_EMON_MOVE1L", "SFX_FLD_ENM_EMON_MOVE1R", 12000, NULL, NULL}, // Hyper Bald Cleft
    {"アイアンシンエモン", "c_sinemon_a", "SIN_S_1", "SIN_Z_1", "SIN_S_1", "SIN_T_1", "SIN_W_1", "SIN_R_1", "SIN_D_1", "SIN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_EMON_MOVE1L", "SFX_FLD_ENM_EMON_MOVE1R", 12000, "SFX_STG3_ISIN_JUMP1", NULL}, // Iron Cleft
    {"アイアンシンエモン2", "c_sinemon_m", "SIN_S_1", "SIN_Z_1", "SIN_S_1", "SIN_T_1", "SIN_W_1", "SIN_R_1", "SIN_D_1", "SIN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_EMON_MOVE1L", "SFX_FLD_ENM_EMON_MOVE1R", 12000, "SFX_STG3_ISIN_JUMP1", NULL}, // Iron Cleft 2
    {"ウラパタ", "c_uranoko", "PTP_S_1", "PTP_Z_1", "PTP_S_1", "PTP_T_1", "PTP_W_1", "PTP_R_1", "PTP_D_1", "PTP_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_PATA_MOVE1", "SFX_FLD_ENM_PATA_MOVE1", 12000, NULL, NULL}, // Shady Paratroopa
    {"ハンマーブロス", "c_burosu_h", "BRO_S_1", "BRO_Z_1", "BRO_S_1", "BRO_T_1", "BRO_W_1", "BRO_R_1", "BRO_D_1", "BRO_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_BROS_MOVE1L", "SFX_FLD_ENM_BROS_MOVE1R", 12000, "SFX_FLD_NPC_JUMP1", "SFX_FLD_NPC_LANDING1"}, // Hammer Bro
    {"ファイアブロス", "c_burosu_f", "BRO_S_1", "BRO_Z_1", "BRO_S_1", "BRO_T_1", "BRO_W_1", "BRO_R_1", "BRO_D_1", "BRO_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_BROS_MOVE1L", "SFX_FLD_ENM_BROS_MOVE1R", 12000, NULL, NULL}, // Fire Bro
    {"ブーメランブロス", "c_burosu_b", "BRO_S_1", "BRO_Z_1", "BRO_S_1", "BRO_T_1", "BRO_W_1", "BRO_R_1", "BRO_D_1", "BRO_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_BROS_MOVE1L", "SFX_FLD_ENM_BROS_MOVE1R", 12000, NULL, NULL}, // Boomerang Bro
    {"マホーン", "c_maho", "MAH_S_1", "MAH_Z_1", "MAH_S_1", "MAH_S_1", "MAH_W_1", "MAH_R_1", "MAH_D_1", "MAH_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_MAHO_MOVE1", NULL, 0, NULL, NULL}, // Wizzerd
    {"スーパーマホーン", "c_maho_b", "MAH_S_1", "MAH_Z_1", "MAH_S_1", "MAH_S_1", "MAH_W_1", "MAH_R_1", "MAH_D_1", "MAH_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_MAHO_MOVE1", NULL, 0, NULL, NULL}, // Dark Wizzerd
    {"マホーンカスタム", "c_maho_w", "MAH_S_1", "MAH_Z_1", "MAH_S_1", "MAH_S_1", "MAH_W_1", "MAH_R_1", "MAH_D_1", "MAH_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_FLD_ENM_MAHO_MOVE1", NULL, 0, NULL, NULL}, // Elite Wizzerd
    {"もんばん", "c_monban", "S_1", "Z_1", "S_1", "Z_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, "SFX_PARTY_MOVE1L", "SFX_PARTY_MOVE1R", 12000, "SFX_PARTY_JUMP1", "SFX_PARTY_LANDING1"}, // Gus (lit. "gatekeeper")
    {"チャンピオン", "c_bred_h", "Z_1", "Z_1", "Z_1", "Z_1", "Z_1", "Z_1", "Z_1", "Z_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Rawk Hawk (champion)
    {"マッチョガンス", "c_m_gansu", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Macho Grubba
    {"ワンワン", "c_wanwan", "WAN_S_1A", "WAN_Z_1", "WAN_S_1A", "WAN_A_1", "WAN_S_1A", "WAN_S_1A", "WAN_D_1", "WAN_K_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Chain-Chomp
#if VERSION <= VERSION_DEMO_JP
    {"バブル", "c_bubble", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_BUBBLE_MOVE1", NULL, 0, NULL, NULL}, // Lava Bubble
    {"ファントム", "c_phantom", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_BUBBLE_MOVE1", NULL, 0, NULL, NULL}, // Phantom Ember
    {"ヘビーボム", "c_heavy", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 40.0f, 60.0f, 0.0f, 0.0f, -10.0f, 20.0f, "SFX_FLD_ENM_BOMB_MOVE1", "SFX_FLD_ENM_BOMB_MOVE1", 12000, NULL, NULL}, // Bulky Bob-omb
    {"ジャイアントボム", "c_giant", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 40.0f, 60.0f, 0.0f, 0.0f, -10.0f, 20.0f, "SFX_FLD_ENM_BOMB_MOVE1", "SFX_FLD_ENM_BOMB_MOVE1", 12000, NULL, NULL}, // Bob-ulk
#else
    {"バブル", "c_bubble", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_BUBBLE_MOVE1", NULL, 0, NULL, NULL}, // Lava Bubble
    {"ファントム", "c_phantom", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 55.0f, "SFX_FLD_ENM_BUBBLE_MOVE1", NULL, 0, NULL, NULL}, // Phantom Ember
    {"ヘビーボム", "c_heavy", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 40.0f, 60.0f, 0.0f, 0.0f, -10.0f, 22.0f, "SFX_FLD_ENM_BOMB_MOVE1", "SFX_FLD_ENM_BOMB_MOVE1", 12000, NULL, NULL}, // Bulky Bob-omb
    {"ジャイアントボム", "c_giant", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 40.0f, 60.0f, 0.0f, 0.0f, -10.0f, 22.0f, "SFX_FLD_ENM_BOMB_MOVE1", "SFX_FLD_ENM_BOMB_MOVE1", 12000, NULL, NULL}, // Bob-ulk
#endif
    {"バーストワンワン", "c_wanwan_a", "WAN_S_1A", "WAN_Z_1", "WAN_S_1A", "WAN_A_1", "WAN_S_1A", "WAN_S_1A", "WAN_D_1", "WAN_K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Red Chomp
    {"トゲノコエース", "c_togenoko_b", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_NOKO_MOVE3L", "SFX_FLD_ENM_NOKO_MOVE3R", 12000, "SFX_FLD_ENM_NOKO_JUMP1", NULL}, // Dark Koopatrol
    {"ダークキーパー", "c_monban_t", "S_1", "Z_1", "S_1", "T_1", "W_1", "R_1", "D_1", "K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Dark Craw
    {"ハンニャー", "c_hannya", "S_1", "Z_1", "S_1", "S_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Spania
    {"ヒンニャー", "c_hannya_n", "S_1", "Z_1", "S_1", "S_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Spinia
    {"ヘンニャー", "c_hannya_t", "S_1", "Z_1", "S_1", "S_1", "W_1", "R_1", "D_1", "K_1", 20.0f, 20.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Spunia
    {"カメック（飛行）", "c_kamek", "KMK_S_2", "KMK_Z_2", "KMK_S_2", "KMK_T_2", "KMK_W_2", "KMK_R_2", "KMK_D_2", "KMK_K_2", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Magikoopa, flying
    {"カメック（地上）", "c_kamek", "KMK_S_1", "KMK_Z_1", "KMK_S_1", "KMK_T_1", "KMK_W_1", "KMK_R_1", "KMK_D_1", "KMK_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KAMEKU_MOVE1L", "SFX_FLD_ENM_KAMEKU_MOVE1R", 12000, NULL, NULL}, // Magikoopa, on the ground
    {"カメック赤", "c_kamek_r", "KMR_S_1", "KMR_Z_1", "KMR_S_1", "KMR_T_1", "KMR_W_1", "KMR_R_1", "KMR_D_1", "KMR_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KAMEKU_MOVE1L", "SFX_FLD_ENM_KAMEKU_MOVE1R", 12000, NULL, NULL}, // Red Magikoopa
    {"カメック白", "c_kamek_w", "KMW_S_1", "KMW_Z_1", "KMW_S_1", "KMW_T_1", "KMW_W_1", "KMW_R_1", "KMW_D_1", "KMW_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KAMEKU_MOVE1L", "SFX_FLD_ENM_KAMEKU_MOVE1R", 12000, NULL, NULL}, // White Magikoopa
    {"カメック緑", "c_kamek_g", "KMG_S_1", "KMG_Z_1", "KMG_S_1", "KMG_T_1", "KMG_W_1", "KMG_R_1", "KMG_D_1", "KMG_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KAMEKU_MOVE1L", "SFX_FLD_ENM_KAMEKU_MOVE1R", 12000, NULL, NULL}, // Green Magikoopa
    {"カメック青", "c_kamek", "KMK_S_1", "KMK_Z_1", "KMK_S_1", "KMK_T_1", "KMK_W_1", "KMK_R_1", "KMK_D_1", "KMK_K_1", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, "SFX_FLD_ENM_KAMEKU_MOVE1L", "SFX_FLD_ENM_KAMEKU_MOVE1R", 12000, NULL, NULL}, // Magikoopa, blue
    {"カメック赤空", "c_kamek_r", "KMR_S_2", "KMR_Z_2", "KMR_S_2", "KMR_T_2", "KMR_W_2", "KMR_R_2", "KMR_D_2", "KMR_K_2", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Red Magikoopa, flying
    {"カメック白空", "c_kamek_w", "KMW_S_2", "KMW_Z_2", "KMW_S_2", "KMW_T_2", "KMW_W_2", "KMW_R_2", "KMW_D_2", "KMW_K_2", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // White Magikoopa, flying
    {"カメック緑空", "c_kamek_g", "KMG_S_2", "KMG_Z_2", "KMG_S_2", "KMG_T_2", "KMG_W_2", "KMG_R_2", "KMG_D_2", "KMG_K_2", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Green Magikoopa, flying
    {"カメック青空", "c_kamek", "KMK_S_2", "KMK_Z_2", "KMK_S_2", "KMK_T_2", "KMK_W_2", "KMK_R_2", "KMK_D_2", "KMK_K_2", 30.0f, 40.0f, 0.0f, 0.0f, 0.0f, 20.0f, NULL, NULL, 0, NULL, NULL}, // Magikoopa, blue, flying
#if VERSION >= VERSION_DEMO_EU
    {"ダイジェストマリオ", "a_mario", "M_S_1", "M_Z_1", "M_S_1", "M_I_3", "M_W_1", "M_R_1", "M_D_1", "M_D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Mario (digest)
#else
    {"ダイジェストマリオ", "a_mario", "M_S_1", "M_Z_1", "M_S_1", "M_I_2", "M_W_1", "M_R_1", "M_D_1", "M_D_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Mario (digest)
#endif
    {"ダイジェストピーチ", "c_peach", "P_S_1", "P_Z_1", "P_S_1", "P_T_1", "P_W_1", "P_R_1", "P_S_1", "P_S_1", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Peach (digest)
    {"ダイジェストピーチ２", "c_peach", "P_S_2", "P_Z_2", "P_S_2", "P_T_2", "P_W_2", "P_R_2", "P_S_2", "P_S_2", 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Peach (digest) 2
    {"ゾンババ", "c_gonbaba_z", "GNB_S_1", "GNB_Z_1", "GNB_S_1", "GNB_T_1", "GNB_S_1", "GNB_S_1", "GNB_S_1", "GNB_S_1", 450.0f, 20.0f, 0.0f, 0.0f, -50.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Bonetail
    {"パラシュートボム", "c_bom_sen", "S_2", "S_2", "S_2", "S_2", "S_2", "S_2", "S_2", "S_2", 450.0f, 20.0f, 0.0f, 0.0f, -50.0f, 50.0f, NULL, NULL, 0, NULL, NULL}, // Bob-omb with a parachute
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, NULL, NULL, 0, NULL, NULL}
};

/**
 * Movement types a map's setup file can give a field NPC (Japanese names: none, standing, test enemy,
 * the field enemy kinds, the 2D kinds, wandering): flags and the init, move, defeat, spotted, lost, return
 * and blow-away scripts.
 */
NpcAiInfo npc_ai_type_table[] = {
    {"なし", 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, // none
    {"不動キャラ汎用", 0, npc_init_evt, NULL, NULL, NULL, NULL, NULL, NULL}, // lit. "stationary character, general"
    {"イベント用不動壁当たりなし", NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, // lit. "stationary, for events, no wall collision"
    {"不動テスト敵キャラ汎用", 0, testenemynpc_init_event, testenemynpc_move_event, enemy_common_dead_event, NULL, NULL, NULL, enemy_common_blow_event}, // lit. "stationary test enemy, general"
    {"クリボー", 0, kuriboo_init_event, kuriboo_move_event, enemy_common_dead_event, kuriboo_find_event, kuriboo_lost_event, kuriboo_return_event, enemy_common_blow_event}, // Goomba
    {"トゲクリ", 0, kuriboo_init_event, kuriboo_move_event, enemy_common_dead_event, kuriboo_find_event, kuriboo_lost_event, kuriboo_return_event, enemy_common_blow_event}, // Spiky Goomba
    {"パタクリ", 0, patakuri_init_event, patakuri_move_event, enemy_common_dead_event, patakuri_find_event, patakuri_lost_event, patakuri_return_event, enemy_common_blow_event}, // Paragoomba
    {"ノコノコ", 0, nokonoko_init_event, nokonoko_move_event, enemy_common_dead_event, nokonoko_find_event, nokonoko_lost_event, nokonoko_return_event, enemy_common_blow_event}, // Koopa Troopa
    {"パタパタ", 0, patapata_init_event, patapata_move_event, enemy_common_dead_event, patapata_find_event, patapata_lost_event, patapata_return_event, enemy_common_blow_event}, // Paratroopa
    {"メット", 0, met_init_event, met_move_event, enemy_common_dead_event, met_find_event, met_lost_event, met_return_event, enemy_common_blow_event}, // Buzzy Beetle
    {"天井メット", 0, met_init_event_ceil, met_move_event, enemy_common_dead_event, met_find_event, met_lost_event, met_return_event, enemy_common_blow_event}, // Buzzy Beetle on the ceiling
    {"トゲメット", 0, togemet_init_event, met_move_event, enemy_common_dead_event, met_find_event, met_lost_event, met_return_event, enemy_common_blow_event}, // Spike Top
    {"天井トゲメット", 0, togemet_init_event_ceil, met_move_event, enemy_common_dead_event, met_find_event, met_lost_event, met_return_event, enemy_common_blow_event}, // Spike Top on the ceiling
    {"パタメット", 0, patamet_init_event, patamet_move_event, enemy_common_dead_event, patamet_find_event, patamet_lost_event, patamet_return_event, enemy_common_blow_event}, // Parabuzzy
    {"ホネノコ(歩行)", 0, honenoko2_init_event, honenoko2_move_event, enemy_common_dead_event, honenoko2_find_event, honenoko2_lost_event, honenoko2_return_event, enemy_common_blow_event}, // Dull Bones (walking)
    {"カロン(骨投げ)", 0, karon_init_event, karon_move_event, karon_dead_event, karon_find_event, karon_lost_event, karon_return_event, karon_blow_event}, // Dry Bones (bone throwing)
    {"チョロボン", 0, chorobon_init_event, chorobon_move_event, enemy_common_dead_event, chorobon_find_event, chorobon_lost_event, chorobon_return_event, enemy_common_blow_event}, // Fuzzy
    {"ゴールドチョロボン", 0, NULL, NULL, enemy_common_dead_event, NULL, NULL, NULL, NULL}, // Gold Fuzzy
    {"キラー大砲", 0, killer_cannon_init_event, killer_cannon_move_event, enemy_common_dead_event, NULL, NULL, NULL, enemy_common_blow_event}, // Bill Blaster
    {"キラー", 0, killer_init_event, NULL, killer_dead_event, NULL, NULL, NULL, enemy_common_blow_event}, // Bullet Bill
    {"サンボ", 0, sambo_init_event, sambo_move_event, enemy_common_dead_event, sambo_find_event, sambo_lost_event, sambo_return_event, enemy_common_blow_event}, // Pokey
    {"シンノスケ", 0, sinnosuke_init_event, sinnosuke_move_event, enemy_common_dead_event, sinnosuke_find_event, sinnosuke_lost_event, sinnosuke_return_event, enemy_common_blow_event}, // Bald Cleft
    {"シンエモン", 0, sinemon_init_event, sinemon_move_event, enemy_common_dead_event, sinemon_find_event, sinemon_lost_event, sinemon_return_event, enemy_common_blow_event}, // Cleft
    {"トゲダルマー", 0, togedaruma_init_event, togedaruma_move_event, enemy_common_dead_event, togedaruma_find_event, togedaruma_lost_event, togedaruma_return_event, enemy_common_blow_event}, // Bristle
    {"軍団ザコ", 0, kuriboo_init_event, kuriboo_move_event, enemy_common_dead_event, kuriboo_find_event, kuriboo_lost_event, kuriboo_return_event, enemy_common_blow_event}, // X-Naut
    {"バリアーン", 0, barriern_init_event, barriern_move_event, barriern_dead_event, barriern_find_event, barriern_lost_event, barriern_return_event, barriern_blow_event}, // Yux
    {"バリアーンカスタム", 0, barriern_init_event, barriern2_move_event, barriern_dead_event, barriern_find_event, barriern_lost_event, barriern_return_event, barriern_blow_event}, // X-Yux
    {"パイダース", 0, piders_init_event, piders_move_event, enemy_common_dead_event, piders_find_event, NULL, NULL, piders_blow_event}, // Pider
    {"パックンフラワー", 0, pakkun_init_event, pakkun_move_event, enemy_common_dead_event, pakkun_find_event, NULL, pakkun_return_event, enemy_common_blow_event}, // Piranha Plant
    {"ドクガッスン", 0, dokugassun_init_event, dokugassun_move_event, enemy_common_dead_event, dokugassun_find_event, dokugassun_lost_event, dokugassun_return_event, enemy_common_blow_event}, // Poison Puff
    {"ボロドー", 0, kuriboo_init_event, kuriboo_move_event, enemy_common_dead_event, kuriboo_find_event, kuriboo_lost_event, kuriboo_return_event, enemy_common_blow_event}, // Bandit
    {"バサバサ（天井）", 0, basabasa_init_event, basabasa_move_event, basabasa_dead_event, basabasa_find_event, basabasa_lost_event, basabasa_return_event, enemy_common_blow_event}, // Swooper (on the ceiling)
    {"バサバサ（空中）", 0, basabasa2_init_event, basabasa2_move_event, basabasa2_dead_event, basabasa2_find_event, basabasa2_lost_event, basabasa2_return_event, enemy_common_blow_event}, // Swooper (in the air)
    {"テレサ", 0, teresa_init_event, teresa_move_event, teresa_dead_event, teresa_find_event, teresa_lost_event, teresa_return_event, enemy_common_blow_event}, // Boo
    {"パンジーさん", 0, pansy_init_event, pansy_move_event, enemy_common_dead_event, pansy_find_event, pansy_lost_event, pansy_return_event, enemy_common_blow_event}, // Crazee Dayzee
    {"きらめくパンジーさん", 0, twinkling_pansy_init_event, twinkling_pansy_move_event, enemy_common_dead_event, twinkling_pansy_find_event, twinkling_pansy_lost_event, twinkling_pansy_return_event, enemy_common_blow_event}, // Amazy Dayzee
    {"バブル", 0, bubble_init_event, bubble_move_event, enemy_common_dead_event, bubble_find_event, bubble_lost_event, bubble_return_event, enemy_common_blow_event}, // Lava Bubble
    {"ヘビーボム", 0, hbom_init_event, hbom_move_event, enemy_common_dead_event, hbom_find_event, hbom_lost_event, hbom_return_event, enemy_common_blow_event}, // Bulky Bob-omb
    {"マホーン", 0, mahoon_init_event, mahoon_move_event, mahoon_dead_event, mahoon_find_event, mahoon_lost_event, mahoon_return_event, enemy_common_blow_event}, // Wizzerd
    {"軍団ザコ魔法使い", 0, zakowiz_init_event, zakowiz_move_event, zakowiz_dead_event, zakowiz_find_event, zakowiz_lost_event, zakowiz_return_event, zakowiz_blow_event}, // X-Naut PhD (lit. "wizard")
    {"ハンニャー", 0, hannya_init_event, hannya_move_event, enemy_common_dead_event, hannya_find_event, hannya_lost_event, hannya_return_event, enemy_common_blow_event}, // Spania
    {"カメック（空中）", 0, kamec_init_event, kamec_move_event, kamec_dead_event, kamec_find_event, kamec_lost_event, kamec_return_event, kamec_blow_event}, // Magikoopa (in the air)
    {"カメック（地上）", 0, kamec2_init_event, kamec2_move_event, kamec2_dead_event, kamec2_find_event, kamec2_lost_event, kamec2_return_event, kamec2_blow_event}, // Magikoopa (on the ground)
    {"ハンマーブロス", 0, hbross_init_event, hbross_move_event, hbross_dead_event, hbross_find_event, hbross_lost_event, hbross_return_event, hbross_blow_event}, // Hammer Bro
    {"ワンワン", 0, wanwan_init_event, wanwan_move_event, enemy_common_dead_event, wanwan_find_event, NULL, NULL, enemy_common_blow_event}, // Chain-Chomp
    {"トゲノコ", 0, togenoko_init_event, togenoko_move_event, enemy_common_dead_event, togenoko_find_event, togenoko_lost_event, togenoko_return_event, enemy_common_blow_event}, // Koopatrol
    {"2D不動キャラ汎用", NPC_FLAG_2D, testenemynpc_init_event, testenemynpc_move_event, enemy_2d_common_dead_event, NULL, NULL, NULL, NULL}, // lit. "2D stationary character, general"
    {"2Dクリボー", NPC_FLAG_2D, kuriboo2D_init_event, kuriboo2D_move_event, enemy_2d_common_dead_event, kuriboo2D_find_event, NULL, NULL, NULL}, // 2D Goomba
    {"2D軍団ザコ", NPC_FLAG_2D, zako2D_init_event, zako2D_move_event, enemy_2d_common_dead_event, zako2D_find_event, NULL, NULL, NULL}, // 2D X-Naut
    {"2D軍団ザコ土管", NPC_FLAG_2D, zako2D_init_event, zako2D_move_event, enemy_2d_common_dead_event, zako2D_find_event, NULL, NULL, NULL}, // 2D X-Naut from a pipe
    {"2D軍団がでるザコ土管", NPC_FLAG_2D, dokan2D_init_event, dokan2D_regl_event, NULL, NULL, NULL, NULL, NULL}, // 2D pipe that releases X-Nauts
    {"２Ｄ軍団ザコ降下", NPC_FLAG_2D, zako2D_init_event, zako2D_move_event, enemy_2d_common_dead_event, zako2D_find_event, NULL, NULL, NULL}, // 2D X-Naut dropping in
    {"２Ｄ軍団ザコ降下地点", NPC_FLAG_2D, fall2D_init_event, fall2D_regl_event, NULL, NULL, NULL, NULL, NULL}, // 2D X-Naut drop point
    {"２Ｄゲッソー", NPC_FLAG_2D, gesso2D_init_event, gesso2D_move_event, enemy_2d_common_dead_event, NULL, NULL, NULL, NULL}, // 2D Blooper
    {"2D軍団ザコ研究員", NPC_FLAG_2D, zakoM2D_init_event, zakoM2D_move_event, zakoM2D_dead_event, zakoM2D_find_event, NULL, NULL, NULL}, // 2D X-Naut PhD
    {"イベント用汎用うろうろ", 0, npc_urouro_init_event, npc_urouro_move_event, NULL, NULL, NULL, NULL, NULL}, // lit. "general wandering, for events"
    {NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
};

/** Territory shapes a map's setup file can name for an NPC (none, circle, square), read by setupDataBase. */
NpcTerritoryInfo npc_define_territory_type[] = {
    {"NPC_TERRITORY_TYPE_NOTHING", NPC_TERRITORY_TYPE_NOTHING},
    {"NPC_TERRITORY_TYPE_CIRCLE", NPC_TERRITORY_TYPE_CIRCLE},
    {"NPC_TERRITORY_TYPE_SQUARE", NPC_TERRITORY_TYPE_SQUARE},
    {0}
};
