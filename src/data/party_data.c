#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "data/party_data.h"
#include "party/party_bomhei.h"
#include "party/party_chuchu.h"
#include "party/party_cloud.h"
#include "party/party_kuribo.h"
#include "party/party_nokonoko.h"
#include "party/party_move.h"
#include "party/party_yoshi.h"
#include "party/party_vivian.h"

#ifdef VERSION_G8ME01
// US has no local duplicates of these: use the global functions
#define bomhei_init_800BFBB0 bomhei_init
#define kuribo_init_800C23B4 kuribo_init
#define vivian_init_801809C0 vivian_init
#endif

void bomhei_init_800BFBB0(PartyEntry* party);
void kuribo_init_800C23B4(PartyEntry* party);
void vivian_init_801809C0(PartyEntry* party);


PartyData partyDataTbl[] = {
    {0},
    {"c_pkr", kuribo_init_800C23B4, partyMoveWalk, kuribo_use, kuribo_use_post, kuribo_bye, NULL, NULL, NULL, NULL},
    {"c_pnk", nokonoko_init, nokonoko_move, nokonoko_use, nokonoko_use_post, nokonoko_bye, NULL, NULL, NULL, NULL},
    {"c_bomt_n", bomhei_init_800BFBB0, partyMoveWalk, bomhei_use, bomhei_use_post, bomhei_bye, NULL, NULL, NULL, bomhei_exit},
    {"c_babyyoshi", yoshi_init, partyMoveWalk, yoshi_use, NULL, yoshi_bye, NULL, NULL, NULL, yoshi_exit},
    {"c_windy", cloud_init, cloud_move, cloud_use, NULL, kuribo_bye, NULL, NULL, NULL, cloud_exit},
    {"c_vivian", vivian_init_801809C0, vivian_move, vivian_use, NULL, kuribo_bye, NULL, NULL, NULL, NULL},
    {"c_tyutyu", chuchu_init, chuchu_move, chuchu_use, NULL, kuribo_bye, NULL, NULL, NULL, chuchu_exit},
    {"c_babyyoshi", kuribo_init_800C23B4, partyMoveWalk, kuribo_use, NULL, kuribo_bye, NULL, NULL, NULL, NULL},
    {"c_marco", kuribo_init_800C23B4, partyMoveWalk, kuribo_use, NULL, kuribo_bye, NULL, NULL, NULL, NULL},
    {"c_pguide", kuribo_init_800C23B4, partyMoveWalk, kuribo_use, NULL, kuribo_bye, NULL, NULL, NULL, NULL},
    {"c_kuri_h", kuribo_init_800C23B4, partyMoveWalk, kuribo_use, NULL, kuribo_bye, NULL, NULL, NULL, NULL},
    {"c_monban", kuribo_init_800C23B4, partyMoveWalk, kuribo_use, NULL, kuribo_bye, NULL, NULL, NULL, NULL},
    {"c_pkr", kuribo_init_800C23B4, partyMoveWalk, kuribo_use, NULL, kuribo_bye, NULL, NULL, NULL, NULL},
    {"c_pnk", nokonoko_init, nokonoko_move, nokonoko_use, NULL, nokonoko_bye, NULL, NULL, NULL, NULL},
    {"c_bomt_n", bomhei_init_800BFBB0, partyMoveWalk, bomhei_use, bomhei_use_post, bomhei_bye, NULL, NULL, NULL, NULL},
    {"c_babyyoshi", yoshi_init, partyMoveWalk, yoshi_use, NULL, yoshi_bye, NULL, NULL, NULL, NULL},
    {"c_windy", cloud_init, cloud_move, cloud_use, NULL, kuribo_bye, NULL, NULL, NULL, cloud_exit},
    {"c_vivian", vivian_init_801809C0, vivian_move, vivian_use, NULL, kuribo_bye, NULL, NULL, NULL, NULL},
    {"c_tyutyu", chuchu_init, partyMoveWalk, chuchu_use, NULL, kuribo_bye, NULL, NULL, NULL, NULL},
};

char* partyPoseTbl[20][15] = {
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"PKR_Z_1", "PKR_S_1", "PKR_W_1", "PKR_R_1", "PKR_J_1A", "PKR_J_1B", "PKR_N_1", "PKR_T_1", "PKR_D_1", "PKR_D_4", "PKR_S_1", "PKR_S_1", "PKR_D_2", "PKR_W_2", "PKR_R_2"},
    {"PNK_Z_1", "PNK_S_1", "PNK_W_1", "PNK_R_1", "PNK_J_1A", "PNK_J_1B", "PNK_N_1", "PNK_T_1", "PNK_D_1", "PNK_D_4", "PNK_S_1", "PNK_S_1", "PNK_D_2", "PNK_W_2", "PNK_R_2"},
    {"Z_1", "S_1", "W_1", "R_1", "J_1A", "J_1B", "N_1", "T_1", "D_1", "F_1", "S_1", "S_1", "D_2", "W_3", "R_3"},
    {"PYS_Z_1", "PYS_S_1", "PYS_W_1", "PYS_R_1", "PYS_J_1A", "PYS_J_1B", "PYS_N_1", "PYS_T_1", "PYS_D_1", "PYS_D_4", "PYS_S_1", "PYS_S_1", "PYS_D_2", "PYS_W_3", "PYS_R_3"},
    {"PWD_Z_1", "PWD_S_1", "PWD_W_1", "PWD_R_1", "PWD_S_1", "PWD_S_1", "PWD_N_1", "PWD_T_1", "PWD_D_1", "PWD_F_1", "PWD_S_1", "PWD_S_1", "PWD_D_2", "PWD_W_2", "PWD_R_2"},
    {"PTR_Z_1", "PTR_S_1", "PTR_W_1", "PTR_R_1", "PTR_S_1", "PTR_S_1", "PTR_N_1", "PTR_T_1", "PTR_D_1", "PTR_F_1", "PTR_Z_1", "PTR_Z_1", "PTR_D_2", "PTR_W_2", "PTR_R_2"},
    {"PCH_Z_1", "PCH_S_1", "PCH_W_1", "PCH_R_1", "PCH_J_1A", "PCH_J_1B", "PCH_N_1", "PCH_T_1", "PCH_D_1", "PCH_F_1", "PCH_S_1", "PCH_S_1", "PCH_D_2", "PCH_R_2", "PCH_W_2"},
    {"PYS_E_1", "PYS_E_2", "PYS_E_3", "PYS_E_3", "PYS_E_2", "PYS_E_2", "PYS_E_2", "PYS_E_2", "PYS_E_2", "PYS_E_2", "PYS_E_2", "PYS_E_2", "PYS_E_2", "PYS_E_3", "PYS_E_3"},
    {"Z_1", "S_1", "W_1", "R_1", "J_1A", "J_1B", "S_1", "T_1", "D_1", "D_1", "Z_1", "Z_1", "S_1", "W_1", "R_1"},
    {"Z_1", "S_1", "W_1", "R_1", "J_1A", "J_1B", "S_1", "T_1", "Z_1", "Z_1", "S_1", "Z_1", "Z_1", "W_1", "R_1"},
    {"Z_1", "S_1", "W_1", "R_1", "W_1", "W_1", "S_1", "T_1", "Z_1", "Z_1", "S_1", "Z_1", "Z_1", "W_1", "R_1"},
    {"Z_1", "S_1", "W_1", "R_1", "S_1", "S_1", "S_1", "T_1", "S_1", "S_1", "S_1", "S_1", "S_1", "W_1", "R_1"},
    {"PKR_Z_1", "PKR_S_1", "PKR_W_1", "PKR_R_1", "PKR_J_1A", "PKR_J_1B", "PKR_N_1", "PKR_T_1", "PKR_D_1", "PKR_D_4", "PKR_S_1", "PKR_S_1", "PKR_D_2", "PKR_W_2", "PKR_R_2"},
    {"PNK_Z_1", "PNK_S_1", "PNK_W_1", "PNK_R_1", "PNK_J_1A", "PNK_J_1B", "PNK_N_1", "PNK_T_1", "PNK_D_1", "PNK_D_4", "PNK_S_1", "PNK_S_1", "PNK_D_2", "PNK_W_2", "PNK_R_2"},
    {"Z_1", "S_1", "W_1", "R_1", "J_1A", "J_1B", "N_1", "T_1", "D_1", "F_1", "S_1", "S_1", "D_2", "W_3", "R_3"},
    {"PYS_Z_1", "PYS_S_1", "PYS_W_1", "PYS_R_1", "PYS_J_1A", "PYS_J_1B", "PYS_N_1", "PYS_T_1", "PYS_D_1", "PYS_D_4", "PYS_S_1", "PYS_S_1", "PYS_D_2", "PYS_W_2", "PYS_R_2"},
    {"PWD_Z_1", "PWD_S_1", "PWD_W_1", "PWD_R_1", "PWD_S_1", "PWD_S_1", "PWD_N_1", "PWD_T_1", "PWD_D_1", "PWD_F_1", "PWD_S_1", "PWD_S_1", "PWD_D_2", "PWD_W_2", "PWD_R_2"},
    {"PTR_Z_1", "PTR_S_1", "PTR_W_1", "PTR_R_1", "PTR_S_1", "PTR_S_1", "PTR_N_1", "PTR_T_1", "PTR_D_1", "PTR_F_1", "PTR_Z_1", "PTR_Z_1", "PTR_D_2", "PTR_W_2", "PTR_R_2"},
    {"PCH_Z_1", "PCH_S_1", "PCH_W_1", "PCH_R_1", "PCH_J_1A", "PCH_J_1B", "PCH_N_1", "PCH_T_1", "PCH_D_1", "PCH_F_1", "PCH_S_1", "PCH_S_1", "PCH_D_2", "PCH_R_2", "PCH_W_2"},
};

char* partyDotPoseTbl[8][15] = {
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {"Z_2", "Z_2", "W_2", "R_2", "Z_2", "Z_2", "Z_2", "Z_2", "Z_2", "Z_2", "Z_2", "Z_2", "Z_2", "W_2", "R_2"},
    {"Z_3", "Z_3", "W_3", "R_3", "Z_3", "Z_3", "Z_3", "Z_3", "Z_3", "Z_3", "Z_3", "Z_3", "Z_3", "W_3", "R_3"},
    {"Z_13", "Z_13", "W_13", "R_13", "Z_13", "Z_13", "Z_13", "Z_13", "Z_13", "Z_13", "Z_13", "Z_13", "Z_13", "W_13", "R_13"},
    {"Z_5", "Z_5", "W_5", "R_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "W_5", "R_5"},
    {"Z_4", "Z_4", "W_4", "R_4", "Z_4", "Z_4", "Z_4", "Z_4", "Z_4", "Z_4", "Z_4", "Z_4", "Z_4", "W_4", "R_4"},
    {"Z_12", "Z_12", "W_12", "R_12", "Z_12", "Z_12", "Z_12", "Z_12", "Z_12", "Z_12", "Z_12", "Z_12", "Z_12", "W_12", "R_12"},
    {"Z_14", "Z_14", "W_14", "R_14", "Z_14", "Z_14", "Z_14", "Z_14", "Z_14", "Z_14", "Z_14", "Z_14", "Z_14", "W_14", "R_14"},
};

char* yoshiDotPoseTbl[7][15] = {
    {"Z_5", "Z_5", "W_5", "R_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "Z_5", "W_5", "R_5"},
    {"Z_6", "Z_6", "W_6", "R_6", "Z_6", "Z_6", "Z_6", "Z_6", "Z_6", "Z_6", "Z_6", "Z_6", "Z_6", "W_6", "R_6"},
    {"Z_7", "Z_7", "W_7", "R_7", "Z_7", "Z_7", "Z_7", "Z_7", "Z_7", "Z_7", "Z_7", "Z_7", "Z_7", "W_7", "R_7"},
    {"Z_8", "Z_8", "W_8", "R_8", "Z_8", "Z_8", "Z_8", "Z_8", "Z_8", "Z_8", "Z_8", "Z_8", "Z_8", "W_8", "R_8"},
    {"Z_9", "Z_9", "W_9", "R_9", "Z_9", "Z_9", "Z_9", "Z_9", "Z_9", "Z_9", "Z_9", "Z_9", "Z_9", "W_9", "R_9"},
    {"Z_10", "Z_10", "W_10", "R_10", "Z_10", "Z_10", "Z_10", "Z_10", "Z_10", "Z_10", "Z_10", "Z_10", "Z_10", "W_10", "R_10"},
    {"Z_11", "Z_11", "W_11", "R_11", "Z_11", "Z_11", "Z_11", "Z_11", "Z_11", "Z_11", "Z_11", "Z_11", "Z_11", "W_11", "R_11"},
};

#pragma explicit_zero_data on
#pragma explicit_zero_data reset
