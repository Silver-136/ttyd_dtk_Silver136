#include "event/evt_uranaisi.h"
#include "event/evt_cmd.h"

typedef struct UranaisiData {
    void* table;
    s32 next;
    s32 unk8;
    s32 unkC;
    s32 index;
    s32 type;
    u8 starpieceFlags[0xD];
    u8 supercoinFlags[0x6];
    u8 pad2B[0x38 - 0x2B];
} UranaisiData;

UranaisiData _udt = {0};
s32 uranaisi_ryokin_tbl[3] = {5, 5, 10};
extern void* mapalloc_base_ptr;
extern const char str_PCTs_PCT04d_802fdda0[];
extern const char str_enddata_802fdda8[];
extern int strcmp(const char* s1, const char* s2);
extern char* msgSearch(char* key);
extern s32 _ismbblead(s32 c);
extern int search_evt_no(void* pEvt);

void* _mapAlloc(void* heap, u32 size);
void _mapFree(void* heap, void* ptr);
s32 johoya_get(void* flags, s32 id);
void johoya_set(void* flags, s32 id, s32 value);
void* pouchGetPtr(void);
s32 sprintf(char* dst, const char* fmt, ...);
int uranaisi_data_make_next(s32 param_1, u32 param_2);
int uranaisi_data_make_starpiece(s32 param_1, u32 param_2);
int uranaisi_data_make_supercoin(s32 param_1, u32 param_2);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(uranaisi_data_alloc) {
    UranaisiData* data = &_udt;
    UranaisiData* initData;
    s32* args = event->args;
    s32 count;
    s32 base;

    initData = &_udt;
    initData->type = evtGetValue(event, args[0]);
    initData->unk8 = evtGetValue(event, args[1]);
    switch (initData->type) {
        case 0:
            count = uranaisi_data_make_next(data->unk8, 0);
            break;
        case 1:
            count = uranaisi_data_make_starpiece(data->unk8, 0);
            break;
        case 2:
            count = uranaisi_data_make_supercoin(data->unk8, 0);
            break;
        default:
            count = 0;
            break;
    }

    data->next = count;
    data->table = _mapAlloc(mapalloc_base_ptr, count * 6 + 0x40);
    memset(data->table, 0, data->next * 6 + 0x40);
    data->unkC = (s32)data->table + data->next * 6;
    base = data->unk8;
    switch (data->type) {
        case 0:
            uranaisi_data_make_next(base, 1);
            break;
        case 1:
            uranaisi_data_make_starpiece(base, 1);
            break;
        case 2:
            uranaisi_data_make_supercoin(base, 1);
            break;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(uranaisi_data_free) {
    UranaisiData* data = &_udt;

    if (data->table == 0) {
        return 2;
    }
    _mapFree(mapalloc_base_ptr, data->table);
    data->table = 0;
    data->next = 0;
    data->unk8 = 0;
    data->unkC = 0;
    data->index = 0;
    return 2;
}

USER_FUNC(uranaisi_table_clear) {
    memset(&_udt.starpieceFlags, 0, 0xD);
    memset(&_udt.supercoinFlags, 0, 6);
    return 2;
}

USER_FUNC(uranaisi_next_check) {
    evtSetValue(event, event->args[0], _udt.next);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(uranaisi_next_makelabel) {
    s32* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    UranaisiData* data = &_udt;
    s32 offset = index * 6;
    void* entry = (void*)((s32)data->table + offset);

    sprintf((char*)data->unkC, str_PCTs_PCT04d_802fdda0, data->unk8,
            *(s16*)((s32)entry + 4));
    evtSetValue(event, args[1], data->unkC);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(uranaisi_starpiece_check) {
    UranaisiData* data = &_udt;
    s32 visible = 0;
    s32 i = 0;
    s32 offset = 0;
    s32* args = event->args;

    while (i < data->next) {
        if ((s16)johoya_get(&data->starpieceFlags, *(s16*)((s32)data->table + offset + 4)) == 0) {
            visible++;
        }
        offset += 6;
        i++;
    }
    evtSetValue(event, args[0], visible);
    return 2;
}

USER_FUNC(uranaisi_starpiece_makelabel) {
    s32* args = event->args;
    s32 target = evtGetValue(event, args[0]);
    UranaisiData* data = &_udt;
    s32 visible = 0;
    s32 i = 0;
    s32 offset = 0;

    while (i < data->next) {
        if ((s16)johoya_get(&data->starpieceFlags, *(s16*)((s32)data->table + offset + 4)) == 0) {
            if (target == visible) {
                sprintf((char*)data->unkC, str_PCTs_PCT04d_802fdda0, data->unk8,
                        *(s16*)((s32)data->table + i * 6 + 4));
                evtSetValue(event, args[1], data->unkC);
                data->index = i;
                return 2;
            }
            visible++;
        }
        offset += 6;
        i++;
    }
    *(u8*)data->unkC = 0;
    evtSetValue(event, args[1], data->unkC);
    return 2;
}

USER_FUNC(uranaisi_starpiece_setreadflag) {
    s32 id = *(s16*)((s32)_udt.table + _udt.index * 6 + 4);
    johoya_set(&_udt.starpieceFlags, id, 1);
    return 2;
}

USER_FUNC(uranaisi_supercoin_check) {
    UranaisiData* data = &_udt;
    s32 visible = 0;
    s32 i = 0;
    s32 offset = 0;
    s32* args = event->args;

    while (i < data->next) {
        if ((s16)johoya_get(&data->supercoinFlags, *(s16*)((s32)data->table + offset + 4)) == 0) {
            visible++;
        }
        offset += 6;
        i++;
    }
    evtSetValue(event, args[0], visible);
    return 2;
}

USER_FUNC(uranaisi_supercoin_makelabel) {
    s32* args = event->args;
    s32 target = evtGetValue(event, args[0]);
    UranaisiData* data = &_udt;
    s32 visible = 0;
    s32 i = 0;
    s32 offset = 0;

    while (i < data->next) {
        if ((s16)johoya_get(&data->supercoinFlags, *(s16*)((s32)data->table + offset + 4)) == 0) {
            if (target == visible) {
                sprintf((char*)data->unkC, str_PCTs_PCT04d_802fdda0, data->unk8,
                        *(s16*)((s32)data->table + i * 6 + 4));
                evtSetValue(event, args[1], data->unkC);
                data->index = i;
                return 2;
            }
            visible++;
        }
        offset += 6;
        i++;
    }
    *(u8*)data->unkC = 0;
    evtSetValue(event, args[1], data->unkC);
    return 2;
}

USER_FUNC(uranaisi_supercoin_setreadflag) {
    s32 id = *(s16*)((s32)_udt.table + _udt.index * 6 + 4);
    johoya_set(&_udt.supercoinFlags, id, 1);
    return 2;
}

int uranaisi_data_make_next(s32 param_1, u32 param_2) {
    extern const char str_enddata_802fdda8[];
    extern const char zero_804267c0[];
    char label[64];
    char key[72];
    char* scan;
    UranaisiData* data = &_udt;
    s32 maxLabel;
    s32 count;
    s32 index;
    s32 field;
    s32 pos;
    s32 done;
    s32 labelNo;
    s32* nextBest = (s32*)((s32)data + 0x34);
    u32 write;

    maxLabel = evtGetValue(NULL, GSW(0));
    write = param_2 & 1;

    if (write != 0) {
        if (*nextBest == -1) {
            return 0;
        }
    } else {
        *nextBest = -1;
    }

    count = 0;
    index = 0;
    while (1) {
        sprintf(key, str_PCTs_PCT04d_802fdda0, param_1, index);
        scan = msgSearch(key);
        if ((s8)scan[0] != '<' || (scan += 2, (s8)scan[-1] != '!')) {
            return count;
        }
        field = 0;
        pos = 0;
        done = 0;
        while (!done) {
            if (_ismbblead((s8)*scan) != 0) {
                if (field == 0) {
                    label[pos] = scan[0];
                    label[pos + 1] = scan[1];
                    pos += 2;
                } else {
                    pos += 2;
                }
                scan += 2;
            } else {
                if ((s8)*scan == '>') {
                    done = 1;
                } else if ((s8)*scan != ',') {
                    if (field == 0) {
                        label[pos] = *scan;
                        pos++;
                    } else {
                        pos++;
                    }
                    scan++;
                    continue;
                }
                scan++;
                if (field == 0) {
                    label[pos] = 0;
                }
                pos = 0;
                field++;
            }
        }

        if (strcmp(label, str_enddata_802fdda8) == 0) {
            return count;
        }

        if (strcmp(label, "") != 0) {
            labelNo = search_evt_no(label);
            if (labelNo != -1) {
                if (write != 0) {
                    if (labelNo == *nextBest) {
                        *(s16*)((s32)data->table + count * 6 + 2) = labelNo;
                        *(s16*)((s32)data->table + count * 6 + 4) = index;
                        count++;
                    }
                } else {
                    if (labelNo <= maxLabel && *nextBest <= labelNo) {
                        if (*nextBest < labelNo) {
                            count = 0;
                            *nextBest = labelNo;
                        }
                        count++;
                    }
                }
            }
        }
        index++;
    }
}

int uranaisi_data_make_starpiece(s32 param_1, u32 param_2) {
    extern void* uranai_table_starpiece[];
    char label[64];
    char key[76];
    char* scan;
    char* body;
    s16 bodyLen;
    s32 maxLabel = evtGetValue(NULL, GSW(0));
    s32 count = 0;
    s32 index = 0;
    s32 offset = 0;
    s32 field;
    s32 pos;
    s32 done;
    s32 labelNo;
    s32 found;
    char saved;
    void** table;
    u32 write = param_2 & 1;

    while (1) {
        sprintf(key, str_PCTs_PCT04d_802fdda0, param_1, index);
        scan = msgSearch(key);
        if ((s8)scan[0] != '<' || (scan += 2, (s8)scan[-1] != '!')) {
            goto done_all;
        }
        field = 0;
        pos = 0;
        done = 0;
        while (!done) {
            if (_ismbblead((s8)*scan) != 0) {
                if (field == 0) {
                    label[pos] = scan[0];
                    label[pos + 1] = scan[1];
                    pos += 2;
                } else {
                    pos += 2;
                }
                scan += 2;
            } else {
                if ((s8)*scan == '>') {
                    done = 1;
                } else if ((s8)*scan != ',') {
                    if (field == 0) {
                        label[pos] = *scan;
                        pos++;
                    } else {
                        pos++;
                    }
                    scan++;
                    continue;
                }
                scan++;
                if (field == 0) {
                    label[pos] = 0;
                    body = scan;
                } else {
                    bodyLen = pos;
                }
                pos = 0;
                field++;
            }
        }

        if (strcmp(label, str_enddata_802fdda8) == 0) {
            return count;
        }

        if (strcmp(label, "") != 0) {
            labelNo = search_evt_no(label);
            if (labelNo != -1 && labelNo <= maxLabel) {
                saved = body[bodyLen];
                body[bodyLen] = 0;
                table = uranai_table_starpiece;
                found = 0;
                while (table[0] != 0) {
                    if (strcmp(body, table[0]) == 0) {
                        found = ((s32*)uranai_table_starpiece)[found * 2 + 1];
                        goto found_entry;
                    }
                    table += 2;
                    found++;
                }
                found = -1;
            found_entry:
                body[bodyLen] = saved;

                if (found != -1 && evtGetValue(NULL, found) == 0) {
                    if (write) {
                        *(s16*)((s32)_udt.table + offset + 2) = labelNo;
                        *(s16*)((s32)_udt.table + offset + 4) = index;
                    }
                    offset += 6;
                    count++;
                }
            }
        }
        index++;
    }
done_all:
    return count;
}

int uranaisi_data_make_supercoin(s32 param_1, u32 param_2) {
    extern void* uranai_table_supercoin[];
    char label[64];
    char key[76];
    char* scan;
    char* body;
    s16 bodyLen;
    s32 maxLabel = evtGetValue(NULL, GSW(0));
    s32 count = 0;
    s32 index = 0;
    s32 offset = 0;
    s32 field;
    s32 pos;
    s32 done;
    s32 labelNo;
    s32 found;
    char saved;
    void** table;
    u32 write = param_2 & 1;

    while (1) {
        sprintf(key, str_PCTs_PCT04d_802fdda0, param_1, index);
        scan = msgSearch(key);
        if ((s8)scan[0] != '<' || (scan += 2, (s8)scan[-1] != '!')) {
            goto done_all;
        }
        field = 0;
        pos = 0;
        done = 0;
        while (!done) {
            if (_ismbblead((s8)*scan) != 0) {
                if (field == 0) {
                    label[pos] = scan[0];
                    label[pos + 1] = scan[1];
                    pos += 2;
                } else {
                    pos += 2;
                }
                scan += 2;
            } else {
                if ((s8)*scan == '>') {
                    done = 1;
                } else if ((s8)*scan != ',') {
                    if (field == 0) {
                        label[pos] = *scan;
                        pos++;
                    } else {
                        pos++;
                    }
                    scan++;
                    continue;
                }
                scan++;
                if (field == 0) {
                    label[pos] = 0;
                    body = scan;
                } else {
                    bodyLen = pos;
                }
                pos = 0;
                field++;
            }
        }

        if (strcmp(label, str_enddata_802fdda8) == 0) {
            goto done_all;
        }

        if (strcmp(label, "") != 0) {
            labelNo = search_evt_no(label);
            if (labelNo != -1 && labelNo <= maxLabel) {
                saved = body[bodyLen];
                body[bodyLen] = 0;
                table = uranai_table_supercoin;
                found = 0;
                while (table[0] != 0) {
                    if (strcmp(body, table[0]) == 0) {
                        found = ((s32*)uranai_table_supercoin)[found * 2 + 1];
                        goto found_entry;
                    }
                    table += 2;
                    found++;
                }
                found = -1;
            found_entry:
                body[bodyLen] = saved;

                if (found != -1 && evtGetValue(NULL, found) == 0) {
                    if (write) {
                        *(s16*)((s32)_udt.table + offset + 2) = labelNo;
                        *(s16*)((s32)_udt.table + offset + 4) = index;
                    }
                    offset += 6;
                    count++;
                }
            }
        }
        index++;
    }
done_all:
    return count;
}

s32 uranaisi_ryokin_rtn(s32 type) {
    if (type != 0) {
        return uranaisi_ryokin_tbl[type];
    }
    return *(s16*)((s32)pouchGetPtr() + 0x8A) * 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(uranaisi_ryokin) {
    s32* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 value;

    if (type != 0) {
        value = uranaisi_ryokin_tbl[type];
    } else {
        value = *(s16*)((s32)pouchGetPtr() + 0x8A) * 2;
    }
    evtSetValue(event, args[1], value);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

const char str_GSWF_STARPIECE_00_802fd330[] = "GSWF_STARPIECE_00";
const char str_GSWF_STARPIECE_01_802fd344[] = "GSWF_STARPIECE_01";
const char str_GSWF_STARPIECE_02_802fd358[] = "GSWF_STARPIECE_02";
const char str_GSWF_STARPIECE_03_802fd36c[] = "GSWF_STARPIECE_03";
const char str_GSWF_STARPIECE_04_802fd380[] = "GSWF_STARPIECE_04";
const char str_GSWF_STARPIECE_05_802fd394[] = "GSWF_STARPIECE_05";
const char str_GSWF_STARPIECE_06_802fd3a8[] = "GSWF_STARPIECE_06";
const char str_GSWF_STARPIECE_07_802fd3bc[] = "GSWF_STARPIECE_07";
const char str_GSWF_STARPIECE_08_802fd3d0[] = "GSWF_STARPIECE_08";
const char str_GSWF_STARPIECE_09_802fd3e4[] = "GSWF_STARPIECE_09";
const char str_GSWF_STARPIECE_10_802fd3f8[] = "GSWF_STARPIECE_10";
const char str_GSWF_STARPIECE_11_802fd40c[] = "GSWF_STARPIECE_11";
const char str_GSWF_STARPIECE_12_802fd420[] = "GSWF_STARPIECE_12";
const char str_GSWF_STARPIECE_13_802fd434[] = "GSWF_STARPIECE_13";
const char str_GSWF_STARPIECE_14_802fd448[] = "GSWF_STARPIECE_14";
const char str_GSWF_STARPIECE_15_802fd45c[] = "GSWF_STARPIECE_15";
const char str_GSWF_STARPIECE_16_802fd470[] = "GSWF_STARPIECE_16";
const char str_GSWF_STARPIECE_17_802fd484[] = "GSWF_STARPIECE_17";
const char str_GSWF_STARPIECE_18_802fd498[] = "GSWF_STARPIECE_18";
const char str_GSWF_STARPIECE_19_802fd4ac[] = "GSWF_STARPIECE_19";
const char str_GSWF_STARPIECE_20_802fd4c0[] = "GSWF_STARPIECE_20";
const char str_GSWF_STARPIECE_21_802fd4d4[] = "GSWF_STARPIECE_21";
const char str_GSWF_STARPIECE_22_802fd4e8[] = "GSWF_STARPIECE_22";
const char str_GSWF_STARPIECE_23_802fd4fc[] = "GSWF_STARPIECE_23";
const char str_GSWF_STARPIECE_24_802fd510[] = "GSWF_STARPIECE_24";
const char str_GSWF_STARPIECE_25_802fd524[] = "GSWF_STARPIECE_25";
const char str_GSWF_STARPIECE_26_802fd538[] = "GSWF_STARPIECE_26";
const char str_GSWF_STARPIECE_27_802fd54c[] = "GSWF_STARPIECE_27";
const char str_GSWF_STARPIECE_28_802fd560[] = "GSWF_STARPIECE_28";
const char str_GSWF_STARPIECE_29_802fd574[] = "GSWF_STARPIECE_29";
const char str_GSWF_STARPIECE_30_802fd588[] = "GSWF_STARPIECE_30";
const char str_GSWF_STARPIECE_31_802fd59c[] = "GSWF_STARPIECE_31";
const char str_GSWF_STARPIECE_32_802fd5b0[] = "GSWF_STARPIECE_32";
const char str_GSWF_STARPIECE_33_802fd5c4[] = "GSWF_STARPIECE_33";
const char str_GSWF_STARPIECE_34_802fd5d8[] = "GSWF_STARPIECE_34";
const char str_GSWF_STARPIECE_35_802fd5ec[] = "GSWF_STARPIECE_35";
const char str_GSWF_STARPIECE_36_802fd600[] = "GSWF_STARPIECE_36";
const char str_GSWF_STARPIECE_37_802fd614[] = "GSWF_STARPIECE_37";
const char str_GSWF_STARPIECE_38_802fd628[] = "GSWF_STARPIECE_38";
const char str_GSWF_STARPIECE_39_802fd63c[] = "GSWF_STARPIECE_39";
const char str_GSWF_STARPIECE_40_802fd650[] = "GSWF_STARPIECE_40";
const char str_GSWF_STARPIECE_41_802fd664[] = "GSWF_STARPIECE_41";
const char str_GSWF_STARPIECE_42_802fd678[] = "GSWF_STARPIECE_42";
const char str_GSWF_STARPIECE_43_802fd68c[] = "GSWF_STARPIECE_43";
const char str_GSWF_STARPIECE_44_802fd6a0[] = "GSWF_STARPIECE_44";
const char str_GSWF_STARPIECE_45_802fd6b4[] = "GSWF_STARPIECE_45";
const char str_GSWF_STARPIECE_46_802fd6c8[] = "GSWF_STARPIECE_46";
const char str_GSWF_STARPIECE_47_802fd6dc[] = "GSWF_STARPIECE_47";
const char str_GSWF_STARPIECE_48_802fd6f0[] = "GSWF_STARPIECE_48";
const char str_GSWF_STARPIECE_49_802fd704[] = "GSWF_STARPIECE_49";
const char str_GSWF_STARPIECE_50_802fd718[] = "GSWF_STARPIECE_50";
const char str_GSWF_STARPIECE_51_802fd72c[] = "GSWF_STARPIECE_51";
const char str_GSWF_STARPIECE_52_802fd740[] = "GSWF_STARPIECE_52";
const char str_GSWF_STARPIECE_53_802fd754[] = "GSWF_STARPIECE_53";
const char str_GSWF_STARPIECE_54_802fd768[] = "GSWF_STARPIECE_54";
const char str_GSWF_STARPIECE_55_802fd77c[] = "GSWF_STARPIECE_55";
const char str_GSWF_STARPIECE_56_802fd790[] = "GSWF_STARPIECE_56";
const char str_GSWF_STARPIECE_57_802fd7a4[] = "GSWF_STARPIECE_57";
const char str_GSWF_STARPIECE_58_802fd7b8[] = "GSWF_STARPIECE_58";
const char str_GSWF_STARPIECE_59_802fd7cc[] = "GSWF_STARPIECE_59";
const char str_GSWF_STARPIECE_60_802fd7e0[] = "GSWF_STARPIECE_60";
const char str_GSWF_STARPIECE_61_802fd7f4[] = "GSWF_STARPIECE_61";
const char str_GSWF_STARPIECE_62_802fd808[] = "GSWF_STARPIECE_62";
const char str_GSWF_STARPIECE_63_802fd81c[] = "GSWF_STARPIECE_63";
const char str_GSWF_STARPIECE_64_802fd830[] = "GSWF_STARPIECE_64";
const char str_GSWF_STARPIECE_65_802fd844[] = "GSWF_STARPIECE_65";
const char str_GSWF_STARPIECE_66_802fd858[] = "GSWF_STARPIECE_66";
const char str_GSWF_STARPIECE_67_802fd86c[] = "GSWF_STARPIECE_67";
const char str_GSWF_STARPIECE_68_802fd880[] = "GSWF_STARPIECE_68";
const char str_GSWF_STARPIECE_69_802fd894[] = "GSWF_STARPIECE_69";
const char str_GSWF_STARPIECE_70_802fd8a8[] = "GSWF_STARPIECE_70";
const char str_GSWF_STARPIECE_71_802fd8bc[] = "GSWF_STARPIECE_71";
const char str_GSWF_STARPIECE_72_802fd8d0[] = "GSWF_STARPIECE_72";
const char str_GSWF_STARPIECE_73_802fd8e4[] = "GSWF_STARPIECE_73";
const char str_GSWF_STARPIECE_74_802fd8f8[] = "GSWF_STARPIECE_74";
const char str_GSWF_STARPIECE_75_802fd90c[] = "GSWF_STARPIECE_75";
const char str_GSWF_STARPIECE_76_802fd920[] = "GSWF_STARPIECE_76";
const char str_GSWF_STARPIECE_77_802fd934[] = "GSWF_STARPIECE_77";
const char str_GSWF_STARPIECE_78_802fd948[] = "GSWF_STARPIECE_78";
const char str_GSWF_STARPIECE_79_802fd95c[] = "GSWF_STARPIECE_79";
const char str_GSWF_STARPIECE_80_802fd970[] = "GSWF_STARPIECE_80";
const char str_GSWF_STARPIECE_81_802fd984[] = "GSWF_STARPIECE_81";
const char str_GSWF_STARPIECE_82_802fd998[] = "GSWF_STARPIECE_82";
const char str_GSWF_STARPIECE_83_802fd9ac[] = "GSWF_STARPIECE_83";
const char str_GSWF_STARPIECE_84_802fd9c0[] = "GSWF_STARPIECE_84";
const char str_GSWF_STARPIECE_85_802fd9d4[] = "GSWF_STARPIECE_85";
const char str_GSWF_STARPIECE_86_802fd9e8[] = "GSWF_STARPIECE_86";
const char str_GSWF_STARPIECE_87_802fd9fc[] = "GSWF_STARPIECE_87";
const char str_GSWF_STARPIECE_88_802fda10[] = "GSWF_STARPIECE_88";
const char str_GSWF_STARPIECE_89_802fda24[] = "GSWF_STARPIECE_89";
const char str_GSWF_STARPIECE_90_802fda38[] = "GSWF_STARPIECE_90";
const char str_GSWF_STARPIECE_91_802fda4c[] = "GSWF_STARPIECE_91";
const char str_GSWF_STARPIECE_92_802fda60[] = "GSWF_STARPIECE_92";
const char str_GSWF_STARPIECE_93_802fda74[] = "GSWF_STARPIECE_93";
const char str_GSWF_STARPIECE_94_802fda88[] = "GSWF_STARPIECE_94";
const char str_GSWF_STARPIECE_95_802fda9c[] = "GSWF_STARPIECE_95";
const char str_GSWF_STARPIECE_96_802fdab0[] = "GSWF_STARPIECE_96";
const char str_GSWF_STARPIECE_97_802fdac4[] = "GSWF_STARPIECE_97";
const char str_GSWF_STARPIECE_98_802fdad8[] = "GSWF_STARPIECE_98";
const char str_GSWF_STARPIECE_99_802fdaec[] = "GSWF_STARPIECE_99";
const char str_GSWF_SHINE_00_802fdb00[] = "GSWF_SHINE_00";
const char str_GSWF_SHINE_01_802fdb10[] = "GSWF_SHINE_01";
const char str_GSWF_SHINE_02_802fdb20[] = "GSWF_SHINE_02";
const char str_GSWF_SHINE_03_802fdb30[] = "GSWF_SHINE_03";
const char str_GSWF_SHINE_04_802fdb40[] = "GSWF_SHINE_04";
const char str_GSWF_SHINE_05_802fdb50[] = "GSWF_SHINE_05";
const char str_GSWF_SHINE_06_802fdb60[] = "GSWF_SHINE_06";
const char str_GSWF_SHINE_07_802fdb70[] = "GSWF_SHINE_07";
const char str_GSWF_SHINE_08_802fdb80[] = "GSWF_SHINE_08";
const char str_GSWF_SHINE_09_802fdb90[] = "GSWF_SHINE_09";
const char str_GSWF_SHINE_10_802fdba0[] = "GSWF_SHINE_10";
const char str_GSWF_SHINE_11_802fdbb0[] = "GSWF_SHINE_11";
const char str_GSWF_SHINE_12_802fdbc0[] = "GSWF_SHINE_12";
const char str_GSWF_SHINE_13_802fdbd0[] = "GSWF_SHINE_13";
const char str_GSWF_SHINE_14_802fdbe0[] = "GSWF_SHINE_14";
const char str_GSWF_SHINE_15_802fdbf0[] = "GSWF_SHINE_15";
const char str_GSWF_SHINE_16_802fdc00[] = "GSWF_SHINE_16";
const char str_GSWF_SHINE_17_802fdc10[] = "GSWF_SHINE_17";
const char str_GSWF_SHINE_18_802fdc20[] = "GSWF_SHINE_18";
const char str_GSWF_SHINE_19_802fdc30[] = "GSWF_SHINE_19";
const char str_GSWF_SHINE_20_802fdc40[] = "GSWF_SHINE_20";
const char str_GSWF_SHINE_21_802fdc50[] = "GSWF_SHINE_21";
const char str_GSWF_SHINE_22_802fdc60[] = "GSWF_SHINE_22";
const char str_GSWF_SHINE_23_802fdc70[] = "GSWF_SHINE_23";
const char str_GSWF_SHINE_24_802fdc80[] = "GSWF_SHINE_24";
const char str_GSWF_SHINE_25_802fdc90[] = "GSWF_SHINE_25";
const char str_GSWF_SHINE_26_802fdca0[] = "GSWF_SHINE_26";
const char str_GSWF_SHINE_27_802fdcb0[] = "GSWF_SHINE_27";
const char str_GSWF_SHINE_28_802fdcc0[] = "GSWF_SHINE_28";
const char str_GSWF_SHINE_29_802fdcd0[] = "GSWF_SHINE_29";
const char str_GSWF_SHINE_30_802fdce0[] = "GSWF_SHINE_30";
const char str_GSWF_SHINE_31_802fdcf0[] = "GSWF_SHINE_31";
const char str_GSWF_SHINE_32_802fdd00[] = "GSWF_SHINE_32";
const char str_GSWF_SHINE_33_802fdd10[] = "GSWF_SHINE_33";
const char str_GSWF_SHINE_34_802fdd20[] = "GSWF_SHINE_34";
const char str_GSWF_SHINE_35_802fdd30[] = "GSWF_SHINE_35";
const char str_GSWF_SHINE_36_802fdd40[] = "GSWF_SHINE_36";
const char str_GSWF_SHINE_37_802fdd50[] = "GSWF_SHINE_37";
const char str_GSWF_SHINE_38_802fdd60[] = "GSWF_SHINE_38";
const char str_GSWF_SHINE_39_802fdd70[] = "GSWF_SHINE_39";
const char str_GSWF_SHINE_40_802fdd80[] = "GSWF_SHINE_40";
const char str_GSWF_SHINE_41_802fdd90[] = "GSWF_SHINE_41";
const char str_PCTs_PCT04d_802fdda0[] = "%s_%04d";
const char str_enddata_802fdda8[] = "enddata";

void* uranai_table_starpiece[] = {
    (void*)str_GSWF_STARPIECE_00_802fd330, (void*)GSWF(5567),
    (void*)str_GSWF_STARPIECE_01_802fd344, (void*)GSWF(5568),
    (void*)str_GSWF_STARPIECE_02_802fd358, (void*)GSWF(5569),
    (void*)str_GSWF_STARPIECE_03_802fd36c, (void*)GSWF(5570),
    (void*)str_GSWF_STARPIECE_04_802fd380, (void*)GSWF(5571),
    (void*)str_GSWF_STARPIECE_05_802fd394, (void*)GSWF(5572),
    (void*)str_GSWF_STARPIECE_06_802fd3a8, (void*)GSWF(5573),
    (void*)str_GSWF_STARPIECE_07_802fd3bc, (void*)GSWF(5574),
    (void*)str_GSWF_STARPIECE_08_802fd3d0, (void*)GSWF(5575),
    (void*)str_GSWF_STARPIECE_09_802fd3e4, (void*)GSWF(5576),
    (void*)str_GSWF_STARPIECE_10_802fd3f8, (void*)GSWF(5577),
    (void*)str_GSWF_STARPIECE_11_802fd40c, (void*)GSWF(5578),
    (void*)str_GSWF_STARPIECE_12_802fd420, (void*)GSWF(5579),
    (void*)str_GSWF_STARPIECE_13_802fd434, (void*)GSWF(5580),
    (void*)str_GSWF_STARPIECE_14_802fd448, (void*)GSWF(5581),
    (void*)str_GSWF_STARPIECE_15_802fd45c, (void*)GSWF(5582),
    (void*)str_GSWF_STARPIECE_16_802fd470, (void*)GSWF(5583),
    (void*)str_GSWF_STARPIECE_17_802fd484, (void*)GSWF(5584),
    (void*)str_GSWF_STARPIECE_18_802fd498, (void*)GSWF(5585),
    (void*)str_GSWF_STARPIECE_19_802fd4ac, (void*)GSWF(5586),
    (void*)str_GSWF_STARPIECE_20_802fd4c0, (void*)GSWF(5587),
    (void*)str_GSWF_STARPIECE_21_802fd4d4, (void*)GSWF(5588),
    (void*)str_GSWF_STARPIECE_22_802fd4e8, (void*)GSWF(5589),
    (void*)str_GSWF_STARPIECE_23_802fd4fc, (void*)GSWF(5590),
    (void*)str_GSWF_STARPIECE_24_802fd510, (void*)GSWF(5591),
    (void*)str_GSWF_STARPIECE_25_802fd524, (void*)GSWF(5592),
    (void*)str_GSWF_STARPIECE_26_802fd538, (void*)GSWF(5593),
    (void*)str_GSWF_STARPIECE_27_802fd54c, (void*)GSWF(5594),
    (void*)str_GSWF_STARPIECE_28_802fd560, (void*)GSWF(5595),
    (void*)str_GSWF_STARPIECE_29_802fd574, (void*)GSWF(5596),
    (void*)str_GSWF_STARPIECE_30_802fd588, (void*)GSWF(5597),
    (void*)str_GSWF_STARPIECE_31_802fd59c, (void*)GSWF(5598),
    (void*)str_GSWF_STARPIECE_32_802fd5b0, (void*)GSWF(5599),
    (void*)str_GSWF_STARPIECE_33_802fd5c4, (void*)GSWF(5600),
    (void*)str_GSWF_STARPIECE_34_802fd5d8, (void*)GSWF(5601),
    (void*)str_GSWF_STARPIECE_35_802fd5ec, (void*)GSWF(5602),
    (void*)str_GSWF_STARPIECE_36_802fd600, (void*)GSWF(5603),
    (void*)str_GSWF_STARPIECE_37_802fd614, (void*)GSWF(5604),
    (void*)str_GSWF_STARPIECE_38_802fd628, (void*)GSWF(5605),
    (void*)str_GSWF_STARPIECE_39_802fd63c, (void*)GSWF(5606),
    (void*)str_GSWF_STARPIECE_40_802fd650, (void*)GSWF(5607),
    (void*)str_GSWF_STARPIECE_41_802fd664, (void*)GSWF(5608),
    (void*)str_GSWF_STARPIECE_42_802fd678, (void*)GSWF(5609),
    (void*)str_GSWF_STARPIECE_43_802fd68c, (void*)GSWF(5610),
    (void*)str_GSWF_STARPIECE_44_802fd6a0, (void*)GSWF(5611),
    (void*)str_GSWF_STARPIECE_45_802fd6b4, (void*)GSWF(5612),
    (void*)str_GSWF_STARPIECE_46_802fd6c8, (void*)GSWF(5613),
    (void*)str_GSWF_STARPIECE_47_802fd6dc, (void*)GSWF(5614),
    (void*)str_GSWF_STARPIECE_48_802fd6f0, (void*)GSWF(5615),
    (void*)str_GSWF_STARPIECE_49_802fd704, (void*)GSWF(5616),
    (void*)str_GSWF_STARPIECE_50_802fd718, (void*)GSWF(5617),
    (void*)str_GSWF_STARPIECE_51_802fd72c, (void*)GSWF(5618),
    (void*)str_GSWF_STARPIECE_52_802fd740, (void*)GSWF(5619),
    (void*)str_GSWF_STARPIECE_53_802fd754, (void*)GSWF(5620),
    (void*)str_GSWF_STARPIECE_54_802fd768, (void*)GSWF(5621),
    (void*)str_GSWF_STARPIECE_55_802fd77c, (void*)GSWF(5622),
    (void*)str_GSWF_STARPIECE_56_802fd790, (void*)GSWF(5623),
    (void*)str_GSWF_STARPIECE_57_802fd7a4, (void*)GSWF(5624),
    (void*)str_GSWF_STARPIECE_58_802fd7b8, (void*)GSWF(5625),
    (void*)str_GSWF_STARPIECE_59_802fd7cc, (void*)GSWF(5626),
    (void*)str_GSWF_STARPIECE_60_802fd7e0, (void*)GSWF(5627),
    (void*)str_GSWF_STARPIECE_61_802fd7f4, (void*)GSWF(5628),
    (void*)str_GSWF_STARPIECE_62_802fd808, (void*)GSWF(5629),
    (void*)str_GSWF_STARPIECE_63_802fd81c, (void*)GSWF(5630),
    (void*)str_GSWF_STARPIECE_64_802fd830, (void*)GSWF(5631),
    (void*)str_GSWF_STARPIECE_65_802fd844, (void*)GSWF(5632),
    (void*)str_GSWF_STARPIECE_66_802fd858, (void*)GSWF(5633),
    (void*)str_GSWF_STARPIECE_67_802fd86c, (void*)GSWF(5634),
    (void*)str_GSWF_STARPIECE_68_802fd880, (void*)GSWF(5635),
    (void*)str_GSWF_STARPIECE_69_802fd894, (void*)GSWF(5636),
    (void*)str_GSWF_STARPIECE_70_802fd8a8, (void*)GSWF(5637),
    (void*)str_GSWF_STARPIECE_71_802fd8bc, (void*)GSWF(5638),
    (void*)str_GSWF_STARPIECE_72_802fd8d0, (void*)GSWF(5639),
    (void*)str_GSWF_STARPIECE_73_802fd8e4, (void*)GSWF(5640),
    (void*)str_GSWF_STARPIECE_74_802fd8f8, (void*)GSWF(5641),
    (void*)str_GSWF_STARPIECE_75_802fd90c, (void*)GSWF(5642),
    (void*)str_GSWF_STARPIECE_76_802fd920, (void*)GSWF(5643),
    (void*)str_GSWF_STARPIECE_77_802fd934, (void*)GSWF(5644),
    (void*)str_GSWF_STARPIECE_78_802fd948, (void*)GSWF(5645),
    (void*)str_GSWF_STARPIECE_79_802fd95c, (void*)GSWF(5646),
    (void*)str_GSWF_STARPIECE_80_802fd970, (void*)GSWF(5647),
    (void*)str_GSWF_STARPIECE_81_802fd984, (void*)GSWF(5648),
    (void*)str_GSWF_STARPIECE_82_802fd998, (void*)GSWF(5649),
    (void*)str_GSWF_STARPIECE_83_802fd9ac, (void*)GSWF(5650),
    (void*)str_GSWF_STARPIECE_84_802fd9c0, (void*)GSWF(5651),
    (void*)str_GSWF_STARPIECE_85_802fd9d4, (void*)GSWF(5652),
    (void*)str_GSWF_STARPIECE_86_802fd9e8, (void*)GSWF(5653),
    (void*)str_GSWF_STARPIECE_87_802fd9fc, (void*)GSWF(5654),
    (void*)str_GSWF_STARPIECE_88_802fda10, (void*)GSWF(5655),
    (void*)str_GSWF_STARPIECE_89_802fda24, (void*)GSWF(5656),
    (void*)str_GSWF_STARPIECE_90_802fda38, (void*)GSWF(5657),
    (void*)str_GSWF_STARPIECE_91_802fda4c, (void*)GSWF(5658),
    (void*)str_GSWF_STARPIECE_92_802fda60, (void*)GSWF(5659),
    (void*)str_GSWF_STARPIECE_93_802fda74, (void*)GSWF(5660),
    (void*)str_GSWF_STARPIECE_94_802fda88, (void*)GSWF(5661),
    (void*)str_GSWF_STARPIECE_95_802fda9c, (void*)GSWF(5662),
    (void*)str_GSWF_STARPIECE_96_802fdab0, (void*)GSWF(5663),
    (void*)str_GSWF_STARPIECE_97_802fdac4, (void*)GSWF(5664),
    (void*)str_GSWF_STARPIECE_98_802fdad8, (void*)GSWF(5665),
    (void*)str_GSWF_STARPIECE_99_802fdaec, (void*)GSWF(5666),
    0, 0,
};

void* uranai_table_supercoin[] = {
    (void*)str_GSWF_SHINE_00_802fdb00, (void*)GSWF(5525),
    (void*)str_GSWF_SHINE_01_802fdb10, (void*)GSWF(5526),
    (void*)str_GSWF_SHINE_02_802fdb20, (void*)GSWF(5527),
    (void*)str_GSWF_SHINE_03_802fdb30, (void*)GSWF(5528),
    (void*)str_GSWF_SHINE_04_802fdb40, (void*)GSWF(5529),
    (void*)str_GSWF_SHINE_05_802fdb50, (void*)GSWF(5530),
    (void*)str_GSWF_SHINE_06_802fdb60, (void*)GSWF(5531),
    (void*)str_GSWF_SHINE_07_802fdb70, (void*)GSWF(5532),
    (void*)str_GSWF_SHINE_08_802fdb80, (void*)GSWF(5533),
    (void*)str_GSWF_SHINE_09_802fdb90, (void*)GSWF(5534),
    (void*)str_GSWF_SHINE_10_802fdba0, (void*)GSWF(5535),
    (void*)str_GSWF_SHINE_11_802fdbb0, (void*)GSWF(5536),
    (void*)str_GSWF_SHINE_12_802fdbc0, (void*)GSWF(5537),
    (void*)str_GSWF_SHINE_13_802fdbd0, (void*)GSWF(5538),
    (void*)str_GSWF_SHINE_14_802fdbe0, (void*)GSWF(5539),
    (void*)str_GSWF_SHINE_15_802fdbf0, (void*)GSWF(5540),
    (void*)str_GSWF_SHINE_16_802fdc00, (void*)GSWF(5541),
    (void*)str_GSWF_SHINE_17_802fdc10, (void*)GSWF(5542),
    (void*)str_GSWF_SHINE_18_802fdc20, (void*)GSWF(5543),
    (void*)str_GSWF_SHINE_19_802fdc30, (void*)GSWF(5544),
    (void*)str_GSWF_SHINE_20_802fdc40, (void*)GSWF(5545),
    (void*)str_GSWF_SHINE_21_802fdc50, (void*)GSWF(5546),
    (void*)str_GSWF_SHINE_22_802fdc60, (void*)GSWF(5547),
    (void*)str_GSWF_SHINE_23_802fdc70, (void*)GSWF(5548),
    (void*)str_GSWF_SHINE_24_802fdc80, (void*)GSWF(5549),
    (void*)str_GSWF_SHINE_25_802fdc90, (void*)GSWF(5550),
    (void*)str_GSWF_SHINE_26_802fdca0, (void*)GSWF(5551),
    (void*)str_GSWF_SHINE_27_802fdcb0, (void*)GSWF(5552),
    (void*)str_GSWF_SHINE_28_802fdcc0, (void*)GSWF(5553),
    (void*)str_GSWF_SHINE_29_802fdcd0, (void*)GSWF(5554),
    (void*)str_GSWF_SHINE_30_802fdce0, (void*)GSWF(5555),
    (void*)str_GSWF_SHINE_31_802fdcf0, (void*)GSWF(5556),
    (void*)str_GSWF_SHINE_32_802fdd00, (void*)GSWF(5557),
    (void*)str_GSWF_SHINE_33_802fdd10, (void*)GSWF(5558),
    (void*)str_GSWF_SHINE_34_802fdd20, (void*)GSWF(5559),
    (void*)str_GSWF_SHINE_35_802fdd30, (void*)GSWF(5560),
    (void*)str_GSWF_SHINE_36_802fdd40, (void*)GSWF(5561),
    (void*)str_GSWF_SHINE_37_802fdd50, (void*)GSWF(5562),
    (void*)str_GSWF_SHINE_38_802fdd60, (void*)GSWF(5563),
    (void*)str_GSWF_SHINE_39_802fdd70, (void*)GSWF(5564),
    (void*)str_GSWF_SHINE_40_802fdd80, (void*)GSWF(5565),
    (void*)str_GSWF_SHINE_41_802fdd90, (void*)GSWF(5566),
    0, 0,
};
