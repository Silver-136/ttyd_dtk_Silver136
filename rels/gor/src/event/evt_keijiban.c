#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "driver/msgdrv.h"
#include "evt/evt_johoya.h"
#include "stdio.h"
#include "string.h"
#include "memory.h"

typedef struct KeijibanEntry { s16 unk0; s16 evtNo; s16 index; } KeijibanEntry;
typedef struct KeijibanData { KeijibanEntry* entries; s32 count; const char* prefix; char* label; s32 current; s32 maxEvtNo; } KeijibanData;
#define KJDT (_kjdt)

s32 keijiban_data_make(const char* prefix, BOOL fill);
USER_FUNC(keijiban_joho_makelabel);
USER_FUNC(keijiban_joho_check);
USER_FUNC(keijiban_data_free);
USER_FUNC(keijiban_data_alloc);

KeijibanData _kjdt[1];

s32 keijiban_data_make(const char* prefix, BOOL fill) {
    char name[64];
    char buf[64];
    KeijibanData* kj = KJDT;
    s32 gsw = evtGetValue(NULL, GSW_STORY_PROGRESS);
    s32 i;
    s32 len;
    s32 field;
    BOOL end;
    const s8* p;
    s32 count;
    s32 no;

    if (fill & 1) {
        if (kj->maxEvtNo == -1) {
            return 0;
        }
    } else {
        kj->maxEvtNo = -1;
    }
    count = 0;
    for (i = 0; ; i++) {
        sprintf(name, "%s_%04d", prefix, i);
        p = (const s8*)msgSearch(name);
        if (*p++ != '<' || *p++ != '!') {
            break;
        }
        field = 0;
        len = 0;
        end = FALSE;
        while (!end) {
            if (_ismbblead(*p)) {
                if (field == 0) {
                    buf[len++] = (char)*p++;
                    buf[len++] = (char)*p++;
                } else {
                    len += 2;
                    p += 2;
                }
            } else {
                switch (*p) {
                    case '>':
                        end = TRUE;
                    case ',':
                        p++;
                        if (field == 0) {
                            buf[len] = '\0';
                        }
                        len = 0;
                        field++;
                        break;
                    default:
                        if (field == 0) {
                            buf[len++] = (char)*p++;
                        } else {
                            len++;
                            p++;
                        }
                        break;
                }
            }
        }
        if (strcmp(buf, "enddata") == 0) {
            break;
        }
        if (strcmp(buf, "") == 0) {
            continue;
        }
        no = search_evt_no(buf);
        if (no == -1) {
            continue;
        }
        if (fill & 1) {
            if (no != kj->maxEvtNo) {
                continue;
            }
            kj->entries[count].evtNo = no;
            kj->entries[count].index = i;
        } else {
            if (no > gsw || no < kj->maxEvtNo) {
                continue;
            }
            if (no > kj->maxEvtNo) {
                kj->maxEvtNo = no;
                count = 0;
            }
        }
        count++;
    }
    return count;
}

USER_FUNC(keijiban_joho_makelabel) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    KeijibanData* kj = KJDT;

    if (idx >= 0 && idx < kj->count) {
        sprintf(kj->label, "%s_%04d", kj->prefix, kj->entries[idx].index);
        evtSetValue(event, args[1], (EvtWord)kj->label);
        kj->current = idx;
        return EVT_RETURN_DONE;
    }
    kj->label[0] = '\0';
    evtSetValue(event, args[1], (EvtWord)kj->label);
    return EVT_RETURN_DONE;
}

USER_FUNC(keijiban_joho_check) {
    evtSetValue(event, event->args[0], KJDT->count);
    return EVT_RETURN_DONE;
}

USER_FUNC(keijiban_data_free) {
    KeijibanData* kj = KJDT;

    if (kj->entries == NULL) {
        return EVT_RETURN_DONE;
    }
    _mapFree(kj->entries);
    kj->entries = NULL;
    kj->count = 0;
    kj->prefix = NULL;
    kj->label = NULL;
    kj->current = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(keijiban_data_alloc) {
    KJDT->prefix = (const char*)evtGetValue(event, event->args[0]);
    KJDT->count = keijiban_data_make(KJDT->prefix, FALSE);
    KJDT->entries = _mapAlloc(KJDT->count * sizeof(KeijibanEntry) + 0x40);
    memset(KJDT->entries, 0, KJDT->count * sizeof(KeijibanEntry) + 0x40);
    KJDT->label = (char*)&KJDT->entries[KJDT->count];
    keijiban_data_make(KJDT->prefix, TRUE);
    return EVT_RETURN_DONE;
}

KeijibanData _kjdt[] = {
    {0},
};

