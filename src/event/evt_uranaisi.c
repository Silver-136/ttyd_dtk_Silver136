#include "evt/evt_johoya.h"
#include "driver/msgdrv.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include <string.h>
#include "libc_decls.h"

s16 johoya_get(u8* table, s16 id);
s16 johoya_set(u8* table, s16 id, s16 val);

typedef struct {
    u16 unk0;
    s16 labelId;
    s16 idx;
} UranaiInfo;

typedef struct {
    UranaiInfo* info;   // 0x0
    s32 count;          // 0x4
    const char* prefix; // 0x8
    char* buf;          // 0xC
    s32 cur;            // 0x10
    s32 mode;           // 0x14
    u8 starpiece[0xD];  // 0x18
    u8 supercoin[0xD];  // 0x25
    s32 next;           // 0x34
} UranaiWork;

typedef struct {
    const char* name;
    s32 id;
} UranaiEntry;

UranaiWork _udt = {0};

UranaiEntry uranai_table_starpiece[] = {
    { "GSWF_STARPIECE_00", 0xF840713F },
    { "GSWF_STARPIECE_01", 0xF8407140 },
    { "GSWF_STARPIECE_02", 0xF8407141 },
    { "GSWF_STARPIECE_03", 0xF8407142 },
    { "GSWF_STARPIECE_04", 0xF8407143 },
    { "GSWF_STARPIECE_05", 0xF8407144 },
    { "GSWF_STARPIECE_06", 0xF8407145 },
    { "GSWF_STARPIECE_07", 0xF8407146 },
    { "GSWF_STARPIECE_08", 0xF8407147 },
    { "GSWF_STARPIECE_09", 0xF8407148 },
    { "GSWF_STARPIECE_10", 0xF8407149 },
    { "GSWF_STARPIECE_11", 0xF840714A },
    { "GSWF_STARPIECE_12", 0xF840714B },
    { "GSWF_STARPIECE_13", 0xF840714C },
    { "GSWF_STARPIECE_14", 0xF840714D },
    { "GSWF_STARPIECE_15", 0xF840714E },
    { "GSWF_STARPIECE_16", 0xF840714F },
    { "GSWF_STARPIECE_17", 0xF8407150 },
    { "GSWF_STARPIECE_18", 0xF8407151 },
    { "GSWF_STARPIECE_19", 0xF8407152 },
    { "GSWF_STARPIECE_20", 0xF8407153 },
    { "GSWF_STARPIECE_21", 0xF8407154 },
    { "GSWF_STARPIECE_22", 0xF8407155 },
    { "GSWF_STARPIECE_23", 0xF8407156 },
    { "GSWF_STARPIECE_24", 0xF8407157 },
    { "GSWF_STARPIECE_25", 0xF8407158 },
    { "GSWF_STARPIECE_26", 0xF8407159 },
    { "GSWF_STARPIECE_27", 0xF840715A },
    { "GSWF_STARPIECE_28", 0xF840715B },
    { "GSWF_STARPIECE_29", 0xF840715C },
    { "GSWF_STARPIECE_30", 0xF840715D },
    { "GSWF_STARPIECE_31", 0xF840715E },
    { "GSWF_STARPIECE_32", 0xF840715F },
    { "GSWF_STARPIECE_33", 0xF8407160 },
    { "GSWF_STARPIECE_34", 0xF8407161 },
    { "GSWF_STARPIECE_35", 0xF8407162 },
    { "GSWF_STARPIECE_36", 0xF8407163 },
    { "GSWF_STARPIECE_37", 0xF8407164 },
    { "GSWF_STARPIECE_38", 0xF8407165 },
    { "GSWF_STARPIECE_39", 0xF8407166 },
    { "GSWF_STARPIECE_40", 0xF8407167 },
    { "GSWF_STARPIECE_41", 0xF8407168 },
    { "GSWF_STARPIECE_42", 0xF8407169 },
    { "GSWF_STARPIECE_43", 0xF840716A },
    { "GSWF_STARPIECE_44", 0xF840716B },
    { "GSWF_STARPIECE_45", 0xF840716C },
    { "GSWF_STARPIECE_46", 0xF840716D },
    { "GSWF_STARPIECE_47", 0xF840716E },
    { "GSWF_STARPIECE_48", 0xF840716F },
    { "GSWF_STARPIECE_49", 0xF8407170 },
    { "GSWF_STARPIECE_50", 0xF8407171 },
    { "GSWF_STARPIECE_51", 0xF8407172 },
    { "GSWF_STARPIECE_52", 0xF8407173 },
    { "GSWF_STARPIECE_53", 0xF8407174 },
    { "GSWF_STARPIECE_54", 0xF8407175 },
    { "GSWF_STARPIECE_55", 0xF8407176 },
    { "GSWF_STARPIECE_56", 0xF8407177 },
    { "GSWF_STARPIECE_57", 0xF8407178 },
    { "GSWF_STARPIECE_58", 0xF8407179 },
    { "GSWF_STARPIECE_59", 0xF840717A },
    { "GSWF_STARPIECE_60", 0xF840717B },
    { "GSWF_STARPIECE_61", 0xF840717C },
    { "GSWF_STARPIECE_62", 0xF840717D },
    { "GSWF_STARPIECE_63", 0xF840717E },
    { "GSWF_STARPIECE_64", 0xF840717F },
    { "GSWF_STARPIECE_65", 0xF8407180 },
    { "GSWF_STARPIECE_66", 0xF8407181 },
    { "GSWF_STARPIECE_67", 0xF8407182 },
    { "GSWF_STARPIECE_68", 0xF8407183 },
    { "GSWF_STARPIECE_69", 0xF8407184 },
    { "GSWF_STARPIECE_70", 0xF8407185 },
    { "GSWF_STARPIECE_71", 0xF8407186 },
    { "GSWF_STARPIECE_72", 0xF8407187 },
    { "GSWF_STARPIECE_73", 0xF8407188 },
    { "GSWF_STARPIECE_74", 0xF8407189 },
    { "GSWF_STARPIECE_75", 0xF840718A },
    { "GSWF_STARPIECE_76", 0xF840718B },
    { "GSWF_STARPIECE_77", 0xF840718C },
    { "GSWF_STARPIECE_78", 0xF840718D },
    { "GSWF_STARPIECE_79", 0xF840718E },
    { "GSWF_STARPIECE_80", 0xF840718F },
    { "GSWF_STARPIECE_81", 0xF8407190 },
    { "GSWF_STARPIECE_82", 0xF8407191 },
    { "GSWF_STARPIECE_83", 0xF8407192 },
    { "GSWF_STARPIECE_84", 0xF8407193 },
    { "GSWF_STARPIECE_85", 0xF8407194 },
    { "GSWF_STARPIECE_86", 0xF8407195 },
    { "GSWF_STARPIECE_87", 0xF8407196 },
    { "GSWF_STARPIECE_88", 0xF8407197 },
    { "GSWF_STARPIECE_89", 0xF8407198 },
    { "GSWF_STARPIECE_90", 0xF8407199 },
    { "GSWF_STARPIECE_91", 0xF840719A },
    { "GSWF_STARPIECE_92", 0xF840719B },
    { "GSWF_STARPIECE_93", 0xF840719C },
    { "GSWF_STARPIECE_94", 0xF840719D },
    { "GSWF_STARPIECE_95", 0xF840719E },
    { "GSWF_STARPIECE_96", 0xF840719F },
    { "GSWF_STARPIECE_97", 0xF84071A0 },
    { "GSWF_STARPIECE_98", 0xF84071A1 },
    { "GSWF_STARPIECE_99", 0xF84071A2 },
    { NULL, 0 }
};

UranaiEntry uranai_table_supercoin[] = {
    { "GSWF_SHINE_00", 0xF8407115 },
    { "GSWF_SHINE_01", 0xF8407116 },
    { "GSWF_SHINE_02", 0xF8407117 },
    { "GSWF_SHINE_03", 0xF8407118 },
    { "GSWF_SHINE_04", 0xF8407119 },
    { "GSWF_SHINE_05", 0xF840711A },
    { "GSWF_SHINE_06", 0xF840711B },
    { "GSWF_SHINE_07", 0xF840711C },
    { "GSWF_SHINE_08", 0xF840711D },
    { "GSWF_SHINE_09", 0xF840711E },
    { "GSWF_SHINE_10", 0xF840711F },
    { "GSWF_SHINE_11", 0xF8407120 },
    { "GSWF_SHINE_12", 0xF8407121 },
    { "GSWF_SHINE_13", 0xF8407122 },
    { "GSWF_SHINE_14", 0xF8407123 },
    { "GSWF_SHINE_15", 0xF8407124 },
    { "GSWF_SHINE_16", 0xF8407125 },
    { "GSWF_SHINE_17", 0xF8407126 },
    { "GSWF_SHINE_18", 0xF8407127 },
    { "GSWF_SHINE_19", 0xF8407128 },
    { "GSWF_SHINE_20", 0xF8407129 },
    { "GSWF_SHINE_21", 0xF840712A },
    { "GSWF_SHINE_22", 0xF840712B },
    { "GSWF_SHINE_23", 0xF840712C },
    { "GSWF_SHINE_24", 0xF840712D },
    { "GSWF_SHINE_25", 0xF840712E },
    { "GSWF_SHINE_26", 0xF840712F },
    { "GSWF_SHINE_27", 0xF8407130 },
    { "GSWF_SHINE_28", 0xF8407131 },
    { "GSWF_SHINE_29", 0xF8407132 },
    { "GSWF_SHINE_30", 0xF8407133 },
    { "GSWF_SHINE_31", 0xF8407134 },
    { "GSWF_SHINE_32", 0xF8407135 },
    { "GSWF_SHINE_33", 0xF8407136 },
    { "GSWF_SHINE_34", 0xF8407137 },
    { "GSWF_SHINE_35", 0xF8407138 },
    { "GSWF_SHINE_36", 0xF8407139 },
    { "GSWF_SHINE_37", 0xF840713A },
    { "GSWF_SHINE_38", 0xF840713B },
    { "GSWF_SHINE_39", 0xF840713C },
    { "GSWF_SHINE_40", 0xF840713D },
    { "GSWF_SHINE_41", 0xF840713E },
    { NULL, 0 }
};

s32 uranaisi_ryokin_tbl[] = { 5, 5, 10 };

s32 uranaisi_data_make_supercoin(const char* prefix, s32 type);
s32 uranaisi_data_make_starpiece(const char* prefix, s32 type);
s32 uranaisi_data_make_next(const char* prefix, s32 type);

#define URANAI_PARSE_TAG() \
    tagNum = 0; \
    mbbpos = 0; \
    tagdone = FALSE; \
    while (!tagdone) { \
        if (_ismbblead(*tagdata)) { \
            if (tagNum == 0) { \
                tag[mbbpos++] = tagdata[0]; \
                tag[mbbpos++] = tagdata[1]; \
                tagdata += 2; \
            } else { \
                mbbpos += 2; \
                tagdata += 2; \
            } \
        } else { \
            switch (tagdata[0]) { \
                case '>': \
                    tagdone = TRUE; \
                case ',': \
                    tagdata++; \
                    if (tagNum == 0) { \
                        tag[mbbpos] = '\0'; \
                        initEndPos = (u8*)tagdata; \
                    } else { \
                        tagEndPos = (s16)mbbpos; \
                    } \
                    mbbpos = 0; \
                    tagNum++; \
                    break; \
                default: \
                    if (tagNum == 0) { \
                        tag[mbbpos++] = *tagdata++; \
                    } else { \
                        mbbpos++; \
                        tagdata++; \
                    } \
            } \
        } \
    }

static inline s32 uranaiCount(UranaiWork* w) {
    return w->count;
}

USER_FUNC(uranaisi_data_alloc) {
    const char* prefix;
    UranaiWork* wp = &_udt;
    EvtWord* args = event->args;
    s32 count;

    _udt.mode = evtGetValue(event, args[0]);
    _udt.prefix = (const char*)evtGetValue(event, args[1]);
    switch (_udt.mode) {
        case 0:
            count = uranaisi_data_make_next(wp->prefix, 0);
            break;
        case 1:
            count = uranaisi_data_make_starpiece(wp->prefix, 0);
            break;
        case 2:
            count = uranaisi_data_make_supercoin(wp->prefix, 0);
            break;
        default:
            count = 0;
            break;
    }
    wp->count = count;
    wp->info = _mapAlloc(uranaiCount(wp) * sizeof(UranaiInfo) + 0x40);
    memset(wp->info, 0, uranaiCount(wp) * sizeof(UranaiInfo) + 0x40);
    prefix = wp->prefix;
    wp->buf = (char*)&wp->info[uranaiCount(wp)];
    switch (wp->mode) {
        case 0:
            uranaisi_data_make_next(prefix, 1);
            break;
        case 1:
            uranaisi_data_make_starpiece(prefix, 1);
            break;
        case 2:
            uranaisi_data_make_supercoin(prefix, 1);
            break;
    }
    return 2;
}

USER_FUNC(uranaisi_data_free) {
    UranaiWork* wp = &_udt;

    if (wp->info == NULL) {
        return 2;
    }
    _mapFree(wp->info);
    wp->info = NULL;
    wp->count = 0;
    wp->prefix = NULL;
    wp->buf = NULL;
    wp->cur = 0;
    return 2;
}

USER_FUNC(uranaisi_table_clear) {
    memset(_udt.starpiece, 0, 0xD);
    memset(_udt.supercoin, 0, 6);
    return 2;
}

USER_FUNC(uranaisi_next_check) {
    evtSetValue(event, *event->args, _udt.count);
    return 2;
}

USER_FUNC(uranaisi_next_makelabel) {
    EvtWord* args = event->args;
    UranaiWork* wp = &_udt;
    s32 n = evtGetValue(event, args[0]);

    sprintf(wp->buf, "%s_%04d", wp->prefix, wp->info[n].idx);
    evtSetValue(event, args[1], (EvtWord)wp->buf);
    return 2;
}

USER_FUNC(uranaisi_starpiece_check) {
    UranaiWork* wp = &_udt;
    EvtWord* args = event->args;
    s32 i;
    s32 count = 0;

    for (i = 0; i < wp->count; i++) {
        if (johoya_get(wp->starpiece, wp->info[i].idx) == 0) {
            count++;
        }
    }
    evtSetValue(event, args[0], count);
    return 2;
}

USER_FUNC(uranaisi_starpiece_makelabel) {
    UranaiWork* wp;
    s32 target;
    EvtWord* args;
    s32 i;
    s32 n;

    args = event->args;
    target = evtGetValue(event, args[0]);
    wp = &_udt;
    n = 0;

    for (i = 0; i < wp->count; i++) {
        if (johoya_get(wp->starpiece, wp->info[i].idx) == 0) {
            if (target == n) {
                sprintf(wp->buf, "%s_%04d", wp->prefix, wp->info[i].idx);
                evtSetValue(event, args[1], (EvtWord)wp->buf);
                wp->cur = i;
                return 2;
            }
            n++;
        }
    }
    *wp->buf = 0;
    evtSetValue(event, args[1], (EvtWord)wp->buf);
    return 2;
}

USER_FUNC(uranaisi_starpiece_setreadflag) {
    UranaiWork* wp = &_udt;
    johoya_set(wp->starpiece, wp->info[wp->cur].idx, 1);
    return 2;
}
USER_FUNC(uranaisi_supercoin_check) {
    UranaiWork* wp = &_udt;
    EvtWord* args = event->args;
    s32 i;
    s32 count = 0;

    for (i = 0; i < wp->count; i++) {
        if (johoya_get(wp->supercoin, wp->info[i].idx) == 0) {
            count++;
        }
    }
    evtSetValue(event, args[0], count);
    return 2;
}

USER_FUNC(uranaisi_supercoin_makelabel) {
    UranaiWork* wp;
    s32 target;
    EvtWord* args;
    s32 i;
    s32 n;

    args = event->args;
    target = evtGetValue(event, args[0]);
    wp = &_udt;
    n = 0;

    for (i = 0; i < wp->count; i++) {
        if (johoya_get(wp->supercoin, wp->info[i].idx) == 0) {
            if (target == n) {
                sprintf(wp->buf, "%s_%04d", wp->prefix, wp->info[i].idx);
                evtSetValue(event, args[1], (EvtWord)wp->buf);
                wp->cur = i;
                return 2;
            }
            n++;
        }
    }
    *wp->buf = 0;
    evtSetValue(event, args[1], (EvtWord)wp->buf);
    return 2;
}

USER_FUNC(uranaisi_supercoin_setreadflag) {
    UranaiWork* wp = &_udt;
    johoya_set(wp->supercoin, wp->info[wp->cur].idx, 1);
    return 2;
}

s32 uranaisi_data_make_next(const char* prefix, s32 type) {
    s32 maxLabel;
    UranaiWork* wp = &_udt;
    char label[64];
    char tag[64];
    s32 storyId;
    s32 mbbpos;
    s32 tagNum;
    BOOL tagdone;
    const s8* tagdata;
    s32 infoId;
    u8* initEndPos;
    s16 tagEndPos;
    const s8* msg;
    s32 labelId;

    maxLabel = evtGetValue(0, -170000000);

    if (type & 1) {
        if (wp->next == -1) {
            return 0;
        }
    } else {
        wp->next = -1;
    }
    infoId = 0;
    storyId = 0;
    while (1) {
        sprintf(label, "%s_%04d", prefix, storyId);
        msg = (const s8*)msgSearch(label);
        if (msg[0] != '<') {
            break;
        }
        tagdata = msg + 2;
        if (msg[1] != '!') {
            break;
        }
        URANAI_PARSE_TAG()

        if (!strcmp(tag, "enddata")) {
            break;
        }

        if (strcmp(tag, "")) {
            labelId = search_evt_no(tag);
            if (labelId != -1) {
                if (type & 1) {
                    if (labelId == wp->next) {
                        wp->info[infoId].labelId = (s16)labelId;
                        wp->info[infoId].idx = (s16)storyId;
                        goto inc;
                    }
                } else if (labelId <= maxLabel && labelId >= wp->next) {
                    if (labelId > wp->next) {
                        wp->next = labelId;
                        infoId = 0;
                    }
inc:
                    infoId++;
                }
            }
        }
        storyId++;
    }
    return infoId;
}

static inline s32 starpiece_search(const char* name) {
    s32 i;
    for (i = 0; uranai_table_starpiece[i].name != NULL; i++) {
        if (!strcmp(name, uranai_table_starpiece[i].name)) {
            return uranai_table_starpiece[i].id;
        }
    }
    return -1;
}

s32 uranaisi_data_make_starpiece(const char* prefix, s32 type) {
    UranaiWork* wp = &_udt;
    char label[64];
    char tag[64];
    s32 mbbpos;
    s32 maxLabel;
    s32 storyId;
    s32 tagNum;
    BOOL tagdone;
    s32 infoId;
    u8* initEndPos;
    s16 tagEndPos;
    const s8* msg;
    const s8* tagdata;
    s32 count;
    s32 labelId;
    s32 flagId;
    u8 saved;

    maxLabel = evtGetValue(0, -170000000);

    count = 0;
    storyId = 0;
    infoId = 0;
    while (1) {
        sprintf(label, "%s_%04d", prefix, storyId);
        msg = (const s8*)msgSearch(label);
        if (msg[0] != '<') {
            break;
        }
        tagdata = msg + 2;
        if (msg[1] != '!') {
            break;
        }
        URANAI_PARSE_TAG()

        if (!strcmp(tag, "enddata")) {
            break;
        }

        if (strcmp(tag, "")) {
            labelId = search_evt_no(tag);
            if (labelId != -1 && labelId <= maxLabel) {
                saved = initEndPos[tagEndPos];
                initEndPos[tagEndPos] = 0;
                flagId = starpiece_search((const char*)initEndPos);
                initEndPos[tagEndPos] = saved;
                if (flagId != -1 && evtGetValue(0, flagId) == 0) {
                    if (type & 1) {
                        wp->info[infoId].labelId = (s16)labelId;
                        wp->info[infoId].idx = (s16)storyId;
                    }
                    infoId++;
                    count++;
                }
            }
        }
        storyId++;
    }
    return count;
}
static inline s32 supercoin_search(const char* name) {
    s32 i;
    for (i = 0; uranai_table_supercoin[i].name != NULL; i++) {
        if (!strcmp(name, uranai_table_supercoin[i].name)) {
            return uranai_table_supercoin[i].id;
        }
    }
    return -1;
}

s32 uranaisi_data_make_supercoin(const char* prefix, s32 type) {
    UranaiWork* wp = &_udt;
    char label[64];
    char tag[64];
    s32 mbbpos;
    s32 maxLabel;
    s32 storyId;
    s32 tagNum;
    BOOL tagdone;
    s32 infoId;
    u8* initEndPos;
    s16 tagEndPos;
    const s8* msg;
    const s8* tagdata;
    s32 count;
    s32 labelId;
    s32 flagId;
    u8 saved;

    maxLabel = evtGetValue(0, -170000000);

    count = 0;
    storyId = 0;
    infoId = 0;
    while (1) {
        sprintf(label, "%s_%04d", prefix, storyId);
        msg = (const s8*)msgSearch(label);
        if (msg[0] != '<') {
            break;
        }
        tagdata = msg + 2;
        if (msg[1] != '!') {
            break;
        }
        URANAI_PARSE_TAG()

        if (!strcmp(tag, "enddata")) {
            break;
        }

        if (strcmp(tag, "")) {
            labelId = search_evt_no(tag);
            if (labelId != -1 && labelId <= maxLabel) {
                saved = initEndPos[tagEndPos];
                initEndPos[tagEndPos] = 0;
                flagId = supercoin_search((const char*)initEndPos);
                initEndPos[tagEndPos] = saved;
                if (flagId != -1 && evtGetValue(0, flagId) == 0) {
                    if (type & 1) {
                        wp->info[infoId].labelId = (s16)labelId;
                        wp->info[infoId].idx = (s16)storyId;
                    }
                    infoId++;
                    count++;
                }
            }
        }
        storyId++;
    }
    return count;
}

s32 uranaisi_ryokin_rtn(s32 mode) {
    if (mode != 0) {
        return uranaisi_ryokin_tbl[mode];
    }
    return pouchGetPtr()->level * 2;
}

USER_FUNC(uranaisi_ryokin) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    s32 price;

    if (mode != 0) {
        price = uranaisi_ryokin_tbl[mode];
    } else {
        price = pouchGetPtr()->level * 2;
    }
    evtSetValue(event, args[1], price);
    return 2;
}
