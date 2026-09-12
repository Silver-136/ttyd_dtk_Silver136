#include "rel/gor/gor_01.h"

typedef struct Vec {
    f32 x;
    f32 y;
    f32 z;
} Vec;

typedef struct ItemData {
    char* mName;
    char* mNameMsg;
    u8 _padding[0x20];
} ItemData;

typedef struct RecipeEntry {
    char* ingredient1;
    char* ingredient2;
    char* result;
} RecipeEntry;

typedef union DoubleConvert {
    f64 value;
    struct {
        u32 hi;
        u32 lo;
    } word;
} DoubleConvert;


extern Vec vec3_gor_000180dc;
extern Vec vec3_gor_000180d0;
extern char str_mahojin_gor_00017764[];
extern char str_SFX_EVT_COOK_EXPLOSI_gor_000183cc[];
extern char str_SFX_EVT_COOK_FIRE1_gor_000183a8[];
extern f64 double_to_int_mask_gor_000183f0;
extern f32 zero_gor_000183f8;
extern f32 float_neg380_gor_000183bc;
extern f32 float_60_gor_000183e4;
extern f32 float_neg170_gor_000183c4;
extern f32 float_2_gor_000183e8;
extern f32 float_25_gor_000183c0;
extern f32 float_1p5_gor_000183c8;
extern f32 float_50_gor_000183ec;
extern void* effNameToPtr(char* name);
extern f32 animPoseGetLoopTimes(s32 poseId);

extern s32 evtGetValue(void* evt, s32 arg);
extern s32 evtSetValue(void* evt, s32 arg, s32 value);
extern void swSet(s32 flag);

extern s32 special_cook_list[];

extern void psndSFXOn_3D(char* name, Vec* pos);
extern void effKemuri12N64Entry(f32 x, f32 y, f32 z, f32 scale, s32 arg4, s32 duration);
extern void effFireEntry(f32 x, f32 y, f32 z, f32 scale, s32 arg4, s32 duration);

extern s32 pouchGetHaveItemCnt(void);
extern s32 pouchHaveItem(s32 index);

extern s32 item_tbl[];

extern s32 itemDataTable;

extern char* msgSearch(char* msg);

extern s32 rand(void);
extern s32 strcmp(char* s1, char* s2);

extern s32 R_mystery_cook_list[];
extern RecipeEntry R_simple_recipe_table[];
extern s32 R_cookbook_recipe_table;

extern void* mapalloc_base_ptr;
extern void* _mapAlloc(void* heap, u32 size);
extern s32* iri_09_item_tbl;
extern void _mapFree(void* heap, void* ptr);

u32 chkCookItem(s32 item) {
    return item != 0;
}
u32 chk_cook_eff_type(s32 item) {
    if (item < 0x94) {
        return 1;
    }

    if (item < 0xA1) {
        return 0;
    }

    if (item < 0xB3) {
        if (item < 0xB1) {
            if (item >= 0xA5) {
                goto ret0_mid;
            }

            if (item >= 0xA1) {
                goto ret1_mid;
            }

            goto ret0_mid;
        } else {
            if (item >= 0xB3) {
                goto ret0_mid;
            }

            goto ret1_mid;
        }

ret1_mid:
        return 1;

ret0_mid:
        return 0;
    }

    if (item >= 0xEC) {
        goto ret0_end;
    }

    if (item < 0xCC) {
        if (item == 0xC4) {
            goto ret1_low;
        }

        if (item < 0xC4) {
            goto ret0_low;
        }

        if (item >= 0xCA) {
            goto ret1_low;
        }

        goto ret0_low;
    }

    switch (item) {
    case 0xD1:
        goto ret1_low;
    default:
        goto ret0_low;
}

ret1_low:
    return 1;

ret0_low:
    return 0;

ret0_end:
    return 0;
}
s32 cooking_flag(void* evt) {
    s32* args = *(s32**)((u8*)evt + 0x18);
    s32 item = evtGetValue(evt, args[0]);

    swSet(item - 0x72);
    return 2;
}
s32 chk_special_cook(void* evt) {
    s32* args = *(s32**)((u8*)evt + 0x18);
    s32 item = evtGetValue(evt, args[0]);
    s32 value = 0;
    s32 index = 0;
    s32 entry;

    goto check;

loop:
    if (item == entry) {
        value = 1;
        goto done;
    }

    index++;

check:
    entry = special_cook_list[index];
    if (entry != -1) {
        goto loop;
    }

done:
    evtSetValue(evt, args[1], value);
    return 2;
}
s32 chk_cook_msg(void* evt) {
    s32* args = *(s32**)((u8*)evt + 0x18);
    s32 item = evtGetValue(evt, args[0]);
    s32 value;

    if ((item == 0xC3) || (item == 0xC4)) {
        value = 0;
        goto done;
    }

    if ((((item >= 0xC1) && (item <= 0xC5)) || (item == 0xE8)) || (item == 0xC8)) {
        value = 1;
        goto done;
    }

    if (item == 0xB6) {
        value = 2;
        goto done;
    }

    if (item < 0x94) {
        item = 1;
    } else if (item < 0xA1) {
        item = 0;
    } else if (item < 0xB3) {
        if (item < 0xB1) {
            if (item >= 0xA5) {
                goto flag0_mid;
            }

            if (item >= 0xA1) {
                goto flag1_mid;
            }

            goto flag0_mid;
        }

        if (item >= 0xB3) {
            goto flag0_mid;
        }

        goto flag1_mid;

flag1_mid:
        item = 1;
        goto add3;

flag0_mid:
        item = 0;
        goto add3;
    } else if (item < 0xEC) {
        if (item < 0xCC) {
            if (item == 0xC4) {
                goto flag1_low;
            }

            if (item < 0xC4) {
                goto flag0_low;
            }

            if (item >= 0xCA) {
                goto flag1_low;
            }

            goto flag0_low;
        }

        switch (item) {
            case 0xD1:
                goto flag1_low;
            default:
                goto flag0_low;
        }

flag1_low:
        item = 1;
        goto add3;

flag0_low:
        item = 0;
        goto add3;
    } else {
        item = 0;
    }

add3:
    value = item + 3;

done:
    evtSetValue(evt, args[1], value);
    return 2;
}
s32 cook_effect(void* evt) {
    s32* args = *(s32**)((u8*)evt + 0x18);
    s32 item = evtGetValue(evt, args[0]);
    s32 type;
    Vec fire_pos;
    Vec explosion_pos;

    if (item < 0x94) {
        type = 1;
        goto decide_effect;
    }

    if (item < 0xA1) {
        type = 0;
        goto decide_effect;
    }

    if (item < 0xB3) {
        if (item < 0xB1) {
            if (item >= 0xA5) {
                goto type0_mid;
            }

            if (item >= 0xA1) {
                goto type1_mid;
            }

            goto type0_mid;
        }

        if (item >= 0xB3) {
            goto type0_mid;
        }

        goto type1_mid;

type1_mid:
        type = 1;
        goto decide_effect;

type0_mid:
        type = 0;
        goto decide_effect;
    }

    if (item >= 0xEC) {
        goto type0_end;
    }

    if (item < 0xCC) {
        if (item == 0xC4) {
            goto type1_low;
        }

        if (item < 0xC4) {
            goto type0_low;
        }

        if (item >= 0xCA) {
            goto type1_low;
        }

        goto type0_low;
    }

    switch (item) {
        case 0xD1:
            goto type1_low;
        default:
            goto type0_low;
    }

type1_low:
    type = 1;
    goto decide_effect;

type0_low:
    type = 0;
    goto decide_effect;

type0_end:
    type = 0;

decide_effect:
    switch (type) {
        case 0:
            goto fire_effect;

        case 1:
            goto explosion_effect;

        default:
            goto explosion_effect;
    }

fire_effect:
    fire_pos = vec3_gor_000180d0;

    psndSFXOn_3D(str_SFX_EVT_COOK_FIRE1_gor_000183a8, &fire_pos);
    effFireEntry(
        float_neg380_gor_000183bc,
        float_25_gor_000183c0,
        float_neg170_gor_000183c4,
        float_1p5_gor_000183c8,
        4,
        0x3C
    );

    goto done;

explosion_effect:
    explosion_pos = vec3_gor_000180dc;

    psndSFXOn_3D(str_SFX_EVT_COOK_EXPLOSI_gor_000183cc, &explosion_pos);
    effKemuri12N64Entry(
        float_neg380_gor_000183bc,
        float_60_gor_000183e4,
        float_neg170_gor_000183c4,
        float_2_gor_000183e8,
        0,
        0x3C
    );

done:
    return 2;
}
s32 make_item_tbl(void* evt) {
    s32* args = *(s32**)((u8*)evt + 0x18);
    s32 skip_index = evtGetValue(evt, args[0]);
    s32 item_count = pouchGetHaveItemCnt();
    s32 value = 0;
    s32 i = 0;
    s32 item;

    while (i < item_count) {
        if (skip_index != i) {
            item = pouchHaveItem(i);

            if (item != 0) {
                item_tbl[value] = item;
                value++;
            }
        }

        i++;
    }

    item_tbl[value] = -1;
    evtSetValue(evt, args[1], value);
    return 2;
}
s32 get_item_name(void* evt) {
    s32* args = *(s32**)((u8*)evt + 0x18);
    s32 item = evtGetValue(evt, args[0]);
    s32 offset = item * 0x28;
    char* value = msgSearch(*(char**)((s32)&itemDataTable + offset + 4));

    evtSetValue(evt, args[1], (s32)value);
    return 2;
}
s32 cook_chk(void* evt) {
    s32 item;
    s32 item_offset;
    s32 item_base;
    RecipeEntry* recipe_ptr;
    u32 recipe_index;

    u32 result_item;
    s32 result_table_index;
    char* search_name;

    s32 roll;
    s32 roll_mod;
    u32 scaled;

    s32* args;

    args = *(s32**)((u8*)evt + 0x18);
    item = evtGetValue(evt, args[0]);

    item_offset = item * 0x28;
    item_base = (s32)&itemDataTable;

    if (item == 0xA3) {
        roll = rand();
        roll_mod = roll % 100;

        if (roll_mod < 0x32) {
            scaled = roll_mod * 0x14;
            item_offset = R_mystery_cook_list[(s32)((f32)scaled / float_50_gor_000183ec)];
        } else {
            item_offset = 0xB6;
        }

        evtSetValue(evt, args[1], item_offset);
        evtSetValue(evt, args[2], item_offset);
        return 2;
    }

    recipe_ptr = R_simple_recipe_table;
    recipe_index = 0;

search_recipe:
    if (strcmp(
        *(char**)(item_base + item_offset),
        recipe_ptr->ingredient1
    ) != 0) {
        goto next_recipe;
    }

    result_item = 0;
    result_table_index = result_item;
    search_name = recipe_ptr->ingredient2;

search_ingredient2:
    if (strcmp(search_name, ((char**)&itemDataTable)[result_table_index]) == 0) {
        goto set_ingredient2;
    }

    result_item++;
    result_table_index += 10;

    if ((s32)result_item < 0x153) {
        goto search_ingredient2;
    }

set_ingredient2:
    evtSetValue(
        evt,
        args[1],
        result_item & ~-((s32)result_item >= 0x153)
    );

    result_item = 0;
    result_table_index = result_item;
    search_name = recipe_ptr->result;

search_result:
    if (strcmp(search_name, ((char**)&itemDataTable)[result_table_index]) == 0) {
        goto set_result;
    }

    result_item++;
    result_table_index += 10;

    if ((s32)result_item < 0x153) {
        goto search_result;
    }

set_result:
    evtSetValue(
        evt,
        args[2],
        result_item & ~-((s32)result_item >= 0x153)
    );
    return 2;

next_recipe:
    recipe_index++;
    recipe_ptr++;

    if (recipe_index < 0x1A) {
        goto search_recipe;
    }

    evtSetValue(evt, args[1], 0);
    evtSetValue(evt, args[2], 0);
    return 2;
}
s32 cook_chk2(void* evt) {
    s32* args;
    s32 item1;
    s32 item2;

    u32 recipe_index;
    s32 recipe_ptr;
    s32 item1_offset;
    s32 item_base;
    s32 item2_offset;
    s32 recipe_base;
    s32 recipe_offset;

    s32 result_item;
    s32 result_table_index;
    char* result_name;

    s32 roll;
    s32 roll_mod;
    u32 scaled;

    args = *(s32**)((u8*)evt + 0x18);
    item1 = evtGetValue(evt, args[0]);
    item2 = evtGetValue(evt, args[1]);

    item1_offset = item1 * 0x28;
    item_base = (s32)&itemDataTable;
    item2_offset = item2 * 0x28;

    if ((item1 == 0xA3) || (item2 == 0xA3)) {
        roll = rand();
        roll_mod = roll % 100;

        if (roll_mod < 0x32) {
            scaled = roll_mod * 0x14;
            result_item = R_mystery_cook_list[(s32)((f32)scaled / float_50_gor_000183ec)];
        } else {
            result_item = 0xB6;
        }

        evtSetValue(evt, args[2], result_item);
        return 2;
    }

    recipe_base = (s32)&R_cookbook_recipe_table;
    recipe_index = 0;
    recipe_offset = 0;

loop:
    recipe_ptr = recipe_base + recipe_offset;

    if (strcmp(*(char**)(item_base + item1_offset), *(char**)(recipe_ptr + 0)) == 0) {
        if (strcmp(*(char**)(item_base + item2_offset), *(char**)(recipe_ptr + 4)) == 0) {
            goto found_recipe;
        }
    }

    if (strcmp(*(char**)(item_base + item2_offset), *(char**)(recipe_ptr + 0)) == 0) {
        if (strcmp(*(char**)(item_base + item1_offset), *(char**)(recipe_ptr + 4)) == 0) {
            goto found_recipe;
        }
    }

    goto next_recipe;

found_recipe:
    result_item = 0;
    result_table_index = result_item;
    result_name = *(char**)((s32)&R_cookbook_recipe_table + recipe_offset + 8);

search_result:
    if (strcmp(result_name, ((char**)&itemDataTable)[result_table_index]) == 0) {
        goto set_result;
    }

    result_item++;
    result_table_index += 10;

    if (result_item < 0x153) {
        goto search_result;
    }

set_result:
    evtSetValue(evt, args[2], result_item & ~-((s32)result_item >= 0x153));
    return 2;

next_recipe:
    recipe_index++;
    recipe_offset += 0xC;

    if (recipe_index < 0x124) {
        goto loop;
    }

    evtSetValue(evt, args[2], 0);
    return 2;
}
s32 itemNameToID(char* name) {
    s32 cmp;
    s32 item_base;
    s32 item;
    s32 offset;

    item_base = (s32)&itemDataTable;
    item = 0;
    offset = 0;

    do {
        cmp = strcmp(name, *(char**)(item_base + offset));

        if (cmp == 0) {
            break;
        }

        item++;
        offset += 0x28;
    } while (item < 0x153);

    if (item >= 0x153) {
        return 0;
    }

    return item;
}
s32 naniga_okorukana_cook(void) {
    s32 roll;
    s32 roll_mod;
    u32 scaled;
    s32 result;

    roll = rand();
    roll_mod = roll % 100;

    if (roll_mod < 0x32) {
        scaled = roll_mod * 0x14;
        result = R_mystery_cook_list[(s32)((f32)scaled / float_50_gor_000183ec)];
    } else {
        result = 0xB6;
    }

    return result;
}
s32 set_fook_evt(void* evt) {
    s32* args;
    s32 target;
    s32 value;

    args = *(s32**)((u8*)evt + 0x18);

    target = evtGetValue(evt, args[0]);
    value = evtGetValue(evt, args[1]);

    *(s32*)(target + 4) = value;

    return 2;
}
s32 iri_09_item_tbl_alloc(void* evt) {
    s32* args;
    s32 count;

    args = *(s32**)((u8*)evt + 0x18);
    count = evtGetValue(evt, args[0]);

    iri_09_item_tbl = _mapAlloc(mapalloc_base_ptr, (count + 1) * 4);

    return 2;
}
s32 iri_09_item_tbl_make(void* evt) {
    s32* args;
    s32 count;
    s32 index;
    s32 offset;

    args = *(s32**)((u8*)evt + 0x18);
    count = evtGetValue(evt, args[0]);

    offset = 0;
    index = 0;

    for (; count > 0; count--) {
        index++;
        *(s32*)((s32)iri_09_item_tbl + offset) = 0x8F;
        offset += 4;
    }

    *(s32*)((s32)iri_09_item_tbl + index * 4) = -1;

    evtSetValue(evt, args[1], (s32)iri_09_item_tbl);

    return 2;
}
s32 iri_09_item_tbl_free(void) {
    _mapFree(mapalloc_base_ptr, iri_09_item_tbl);

    iri_09_item_tbl = 0;

    return 2;
}
s32 wait_mahojin(void) {
    void* effect;
    s32 poseId;

    effect = effNameToPtr(str_mahojin_gor_00017764);
    poseId = *(s32*)(*(s32*)((s32)effect + 0xC) + 0x18);

    if (poseId != -1) {
        if (animPoseGetLoopTimes(poseId) != zero_gor_000183f8) {
            return 2;
        }
    }

    return 0;
}