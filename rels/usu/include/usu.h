#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"
#include "evt/evt_cmd.h"

// booboo_name tables (usu_00, usu_01): the Twilight Town villagers whose models get the material light flag
typedef struct BoobooName {
    const char* name; // NPC name
    u8 field_0x4;     // never read
    u8 field_0x5;     // never read
    u8 pad_0x6[2];
} BoobooName;

extern EvtWord all_party_lecture[];
extern EvtWord evt_usu_kagemario_party_kill[];
extern EvtWord roll_majin_lecture[];
extern EvtWord roll_majin_lecture_init[];
extern EvtWord usu_00_init_evt[];
extern EvtWord usu_01_init_evt[];
USER_FUNC(evt_usu_vivian_pose_set);

// Area-local saved values (USU; swReInit clears them on every area change), named from the area's
// scripts
// loop_msg via mise_kiza_msg (usu_01, STG4_10..STG4_15_05): 1 while Mario's talk target is not the NPC named kiza
// yarou in the script (chk_fuki), letting his bubble stg4_usu_61_3 show; 0 closes it
#define LSWF_USU_01_KIZAYAROU_BUBBLE_ON LSWF(2)
// as LSWF(2) for the shop owner (mise no shujin in the script) and his bubble stg4_usu_61_4
#define LSWF_USU_01_SHOPKEEPER_BUBBLE_ON LSWF(3)
