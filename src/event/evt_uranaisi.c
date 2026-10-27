#include "evt/evt_johoya.h"
#include "driver/msgdrv.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include <string.h>
#include "libc_decls.h"
#include "evt/evt_uranaisi.h"


typedef struct {
    u16 flags;
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
    { "GSWF_STARPIECE_00", GSWF_STARPIECE_00 },
    { "GSWF_STARPIECE_01", GSWF_STARPIECE_01 },
    { "GSWF_STARPIECE_02", GSWF_STARPIECE_02 },
    { "GSWF_STARPIECE_03", GSWF_STARPIECE_03 },
    { "GSWF_STARPIECE_04", GSWF_STARPIECE_04 },
    { "GSWF_STARPIECE_05", GSWF_STARPIECE_05 },
    { "GSWF_STARPIECE_06", GSWF_STARPIECE_06 },
    { "GSWF_STARPIECE_07", GSWF_STARPIECE_07 },
    { "GSWF_STARPIECE_08", GSWF_STARPIECE_08 },
    { "GSWF_STARPIECE_09", GSWF_STARPIECE_09 },
    { "GSWF_STARPIECE_10", GSWF_STARPIECE_10 },
    { "GSWF_STARPIECE_11", GSWF_STARPIECE_11 },
    { "GSWF_STARPIECE_12", GSWF_STARPIECE_12 },
    { "GSWF_STARPIECE_13", GSWF_STARPIECE_13 },
    { "GSWF_STARPIECE_14", GSWF_STARPIECE_14 },
    { "GSWF_STARPIECE_15", GSWF_STARPIECE_15 },
    { "GSWF_STARPIECE_16", GSWF_STARPIECE_16 },
    { "GSWF_STARPIECE_17", GSWF_STARPIECE_17 },
    { "GSWF_STARPIECE_18", GSWF_STARPIECE_18 },
    { "GSWF_STARPIECE_19", GSWF_STARPIECE_19 },
    { "GSWF_STARPIECE_20", GSWF_STARPIECE_20 },
    { "GSWF_STARPIECE_21", GSWF_STARPIECE_21 },
    { "GSWF_STARPIECE_22", GSWF_STARPIECE_22 },
    { "GSWF_STARPIECE_23", GSWF_STARPIECE_23 },
    { "GSWF_STARPIECE_24", GSWF_STARPIECE_24 },
    { "GSWF_STARPIECE_25", GSWF_STARPIECE_25 },
    { "GSWF_STARPIECE_26", GSWF_STARPIECE_26 },
    { "GSWF_STARPIECE_27", GSWF_STARPIECE_27 },
    { "GSWF_STARPIECE_28", GSWF_STARPIECE_28 },
    { "GSWF_STARPIECE_29", GSWF_STARPIECE_29 },
    { "GSWF_STARPIECE_30", GSWF_STARPIECE_30 },
    { "GSWF_STARPIECE_31", GSWF_STARPIECE_31 },
    { "GSWF_STARPIECE_32", GSWF_STARPIECE_32 },
    { "GSWF_STARPIECE_33", GSWF_STARPIECE_33 },
    { "GSWF_STARPIECE_34", GSWF_STARPIECE_34 },
    { "GSWF_STARPIECE_35", GSWF_STARPIECE_35 },
    { "GSWF_STARPIECE_36", GSWF_STARPIECE_36 },
    { "GSWF_STARPIECE_37", GSWF_STARPIECE_37 },
    { "GSWF_STARPIECE_38", GSWF_STARPIECE_38 },
    { "GSWF_STARPIECE_39", GSWF_STARPIECE_39 },
    { "GSWF_STARPIECE_40", GSWF_STARPIECE_40 },
    { "GSWF_STARPIECE_41", GSWF_STARPIECE_41 },
    { "GSWF_STARPIECE_42", GSWF_STARPIECE_42 },
    { "GSWF_STARPIECE_43", GSWF_STARPIECE_43 },
    { "GSWF_STARPIECE_44", GSWF_STARPIECE_44 },
    { "GSWF_STARPIECE_45", GSWF_STARPIECE_45 },
    { "GSWF_STARPIECE_46", GSWF_STARPIECE_46 },
    { "GSWF_STARPIECE_47", GSWF_STARPIECE_47 },
    { "GSWF_STARPIECE_48", GSWF_STARPIECE_48 },
    { "GSWF_STARPIECE_49", GSWF_STARPIECE_49 },
    { "GSWF_STARPIECE_50", GSWF_STARPIECE_50 },
    { "GSWF_STARPIECE_51", GSWF_STARPIECE_51 },
    { "GSWF_STARPIECE_52", GSWF_STARPIECE_52 },
    { "GSWF_STARPIECE_53", GSWF_STARPIECE_53 },
    { "GSWF_STARPIECE_54", GSWF_STARPIECE_54 },
    { "GSWF_STARPIECE_55", GSWF_STARPIECE_55 },
    { "GSWF_STARPIECE_56", GSWF_STARPIECE_56 },
    { "GSWF_STARPIECE_57", GSWF_STARPIECE_57 },
    { "GSWF_STARPIECE_58", GSWF_STARPIECE_58 },
    { "GSWF_STARPIECE_59", GSWF_STARPIECE_59 },
    { "GSWF_STARPIECE_60", GSWF_STARPIECE_60 },
    { "GSWF_STARPIECE_61", GSWF_STARPIECE_61 },
    { "GSWF_STARPIECE_62", GSWF_STARPIECE_62 },
    { "GSWF_STARPIECE_63", GSWF_STARPIECE_63 },
    { "GSWF_STARPIECE_64", GSWF_STARPIECE_64 },
    { "GSWF_STARPIECE_65", GSWF_STARPIECE_65 },
    { "GSWF_STARPIECE_66", GSWF_STARPIECE_66 },
    { "GSWF_STARPIECE_67", GSWF_STARPIECE_67 },
    { "GSWF_STARPIECE_68", GSWF_STARPIECE_68 },
    { "GSWF_STARPIECE_69", GSWF_STARPIECE_69 },
    { "GSWF_STARPIECE_70", GSWF_STARPIECE_70 },
    { "GSWF_STARPIECE_71", GSWF_STARPIECE_71 },
    { "GSWF_STARPIECE_72", GSWF_STARPIECE_72 },
    { "GSWF_STARPIECE_73", GSWF_STARPIECE_73 },
    { "GSWF_STARPIECE_74", GSWF_STARPIECE_74 },
    { "GSWF_STARPIECE_75", GSWF_STARPIECE_75 },
    { "GSWF_STARPIECE_76", GSWF_STARPIECE_76 },
    { "GSWF_STARPIECE_77", GSWF_STARPIECE_77 },
    { "GSWF_STARPIECE_78", GSWF_STARPIECE_78 },
    { "GSWF_STARPIECE_79", GSWF_STARPIECE_79 },
    { "GSWF_STARPIECE_80", GSWF_STARPIECE_80 },
    { "GSWF_STARPIECE_81", GSWF_STARPIECE_81 },
    { "GSWF_STARPIECE_82", GSWF_STARPIECE_82 },
    { "GSWF_STARPIECE_83", GSWF_STARPIECE_83 },
    { "GSWF_STARPIECE_84", GSWF_STARPIECE_84 },
    { "GSWF_STARPIECE_85", GSWF_STARPIECE_85 },
    { "GSWF_STARPIECE_86", GSWF_STARPIECE_86 },
    { "GSWF_STARPIECE_87", GSWF_STARPIECE_87 },
    { "GSWF_STARPIECE_88", GSWF_STARPIECE_88 },
    { "GSWF_STARPIECE_89", GSWF_STARPIECE_89 },
    { "GSWF_STARPIECE_90", GSWF_STARPIECE_90 },
    { "GSWF_STARPIECE_91", GSWF_STARPIECE_91 },
    { "GSWF_STARPIECE_92", GSWF_STARPIECE_92 },
    { "GSWF_STARPIECE_93", GSWF_STARPIECE_93 },
    { "GSWF_STARPIECE_94", GSWF_STARPIECE_94 },
    { "GSWF_STARPIECE_95", GSWF_STARPIECE_95 },
    { "GSWF_STARPIECE_96", GSWF_STARPIECE_96 },
    { "GSWF_STARPIECE_97", GSWF_STARPIECE_97 },
    { "GSWF_STARPIECE_98", GSWF_STARPIECE_98 },
    { "GSWF_STARPIECE_99", GSWF_STARPIECE_99 },
    { NULL, 0 }
};

UranaiEntry uranai_table_supercoin[] = {
    { "GSWF_SHINE_00", GSWF_SHINE_00 },
    { "GSWF_SHINE_01", GSWF_SHINE_01 },
    { "GSWF_SHINE_02", GSWF_SHINE_02 },
    { "GSWF_SHINE_03", GSWF_SHINE_03 },
    { "GSWF_SHINE_04", GSWF_SHINE_04 },
    { "GSWF_SHINE_05", GSWF_SHINE_05 },
    { "GSWF_SHINE_06", GSWF_SHINE_06 },
    { "GSWF_SHINE_07", GSWF_SHINE_07 },
    { "GSWF_SHINE_08", GSWF_SHINE_08 },
    { "GSWF_SHINE_09", GSWF_SHINE_09 },
    { "GSWF_SHINE_10", GSWF_SHINE_10 },
    { "GSWF_SHINE_11", GSWF_SHINE_11 },
    { "GSWF_SHINE_12", GSWF_SHINE_12 },
    { "GSWF_SHINE_13", GSWF_SHINE_13 },
    { "GSWF_SHINE_14", GSWF_SHINE_14 },
    { "GSWF_SHINE_15", GSWF_SHINE_15 },
    { "GSWF_SHINE_16", GSWF_SHINE_16 },
    { "GSWF_SHINE_17", GSWF_SHINE_17 },
    { "GSWF_SHINE_18", GSWF_SHINE_18 },
    { "GSWF_SHINE_19", GSWF_SHINE_19 },
    { "GSWF_SHINE_20", GSWF_SHINE_20 },
    { "GSWF_SHINE_21", GSWF_SHINE_21 },
    { "GSWF_SHINE_22", GSWF_SHINE_22 },
    { "GSWF_SHINE_23", GSWF_SHINE_23 },
    { "GSWF_SHINE_24", GSWF_SHINE_24 },
    { "GSWF_SHINE_25", GSWF_SHINE_25 },
    { "GSWF_SHINE_26", GSWF_SHINE_26 },
    { "GSWF_SHINE_27", GSWF_SHINE_27 },
    { "GSWF_SHINE_28", GSWF_SHINE_28 },
    { "GSWF_SHINE_29", GSWF_SHINE_29 },
    { "GSWF_SHINE_30", GSWF_SHINE_30 },
    { "GSWF_SHINE_31", GSWF_SHINE_31 },
    { "GSWF_SHINE_32", GSWF_SHINE_32 },
    { "GSWF_SHINE_33", GSWF_SHINE_33 },
    { "GSWF_SHINE_34", GSWF_SHINE_34 },
    { "GSWF_SHINE_35", GSWF_SHINE_35 },
    { "GSWF_SHINE_36", GSWF_SHINE_36 },
    { "GSWF_SHINE_37", GSWF_SHINE_37 },
    { "GSWF_SHINE_38", GSWF_SHINE_38 },
    { "GSWF_SHINE_39", GSWF_SHINE_39 },
    { "GSWF_SHINE_40", GSWF_SHINE_40 },
    { "GSWF_SHINE_41", GSWF_SHINE_41 },
    { NULL, 0 }
};

s32 uranaisi_ryokin_tbl[] = { 5, 5, 10 };

s32 uranaisi_data_make_supercoin(const char* prefix, s32 type);
s32 uranaisi_data_make_starpiece(const char* prefix, s32 type);
s32 uranaisi_data_make_next(const char* prefix, s32 type);

#define URANAI_PARSE_TAG()                          \
    tagNum = 0;                                     \
    mbbpos = 0;                                     \
    tagdone = FALSE;                                \
    while (!tagdone) {                              \
        if (_ismbblead(*tagdata)) {                 \
            if (tagNum == 0) {                      \
                tag[mbbpos++] = tagdata[0];         \
                tag[mbbpos++] = tagdata[1];         \
                tagdata += 2;                       \
            } else {                                \
                mbbpos += 2;                        \
                tagdata += 2;                       \
            }                                       \
        } else {                                    \
            switch (tagdata[0]) {                   \
                case '>':                           \
                    tagdone = TRUE;                 \
                case ',':                           \
                    tagdata++;                      \
                    if (tagNum == 0) {              \
                        tag[mbbpos] = '\0';         \
                        initEndPos = (u8*)tagdata;  \
                    } else {                        \
                        tagEndPos = (s16)mbbpos;    \
                    }                               \
                    mbbpos = 0;                     \
                    tagNum++;                       \
                    break;                          \
                default:                            \
                    if (tagNum == 0) {              \
                        tag[mbbpos++] = *tagdata++; \
                    } else {                        \
                        mbbpos++;                   \
                        tagdata++;                  \
                    }                               \
            }                                       \
        }                                           \
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
