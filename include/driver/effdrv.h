#pragma once

#include <dolphin/dvd.h>
#include "manager/filemgr.h"
#include "sdk/texPalette.h"

#include <dolphin/mtx.h>
#include "eff/eff_maya_data.h"
#include "version.h"

// Effect texture id in each version's effect texture file: the US ids of the textures that use it are 0x1C lower
#define EFF_TEX(id) ((id) - VERSION_PICK(0, 0x1C))

/* Maya effect animation data (the same layout in effdrv and the effects that play it: eff_damage_star, eff_ice,
 * eff_recovery): MayaAnimData -> MayaKeyList (one entry per animated polygon group) -> MayaPoly (its keys and
 * polygon strips) -> MayaHermitKey. */
typedef struct MayaPoly {
    s32 keyCount;        // 0x0
    MayaHermitKey* keys; // 0x4
    s32 count;           // 0x8, polygon strip count
    s16** strips;        // 0xC
} MayaPoly;

typedef struct MayaKeyList {
    s32 count;       // 0x0
    MayaPoly* polys; // 0x4
} MayaKeyList;

typedef struct MayaAnimData {
    u8 pad_00[4];        // 0x0, never accessed
    f32 endTime;         // 0x4
    MayaKeyList* lists;  // 0x8
} MayaAnimData;

typedef struct MayaJoint {
    Mtx mtx;   // 0x0
    f32 alpha; // 0x30
} MayaJoint;

typedef struct MayaAnim {
    u16 flags;             // 0x0
    f32 time;              // 0x4
    OSTime startTime;      // 0x8
    MayaAnimData* data;    // 0x10
    MayaJoint joints[1];   // 0x14, one per MayaKeyList entry (effMayaAnimAlloc)
} MayaAnim;

typedef void (*EffectCallback)(struct EffectEntry* effect);

// EffectEntry.flags (effEntry, effMain, effSoftDelete)
typedef enum EffectFlags {
    EFFECT_FLAG_ACTIVE = (1 << 0),         // 0x1, slot in use; effMain runs the callback
    EFFECT_FLAG_SOFT_DELETABLE = (1 << 1), // 0x2, effSoftDelete only requests the end (the effect fades out itself)
    EFFECT_FLAG_SOFT_DELETE = (1 << 2),    // 0x4, effSoftDelete requested; the effect clears it and starts fading
} EffectFlags;

typedef struct EffectEntry {
    u32 flags;               // 0x0, EffectFlags
    BOOL inBattle;           // 0x4
    s32 count;               // 0x8
    void* userdata;          // 0xC, for effects to store their own data
    EffectCallback callback; // 0x10
    const char* type;        // 0x14
    char name[16];           // 0x18
} EffectEntry;

typedef struct EffectSet {
    s16 id;           // 0x0
    const char* name; // 0x4
} EffectSet;

typedef struct EffectWork {
    s32 count;            // 0x0
    EffectEntry* entries; // 0x4
    TEXPalette* texture;  // 0x8
    BOOL texLoaded;       // 0xC
    FileEntry* handle;    // 0x10
    s32 language;         // 0x14
} EffectWork;

void effInit(void);
void effTexSetup(void);
void effGetTexObj(u32 id, GXTexObj* obj);
void effAutoRelease(BOOL inBattle);
EffectEntry* effEntry(void);
void effSetName(EffectEntry* effect, const char* name);
void effMain(void);
void effDelete(EffectEntry* effect);
void effSoftDelete(EffectEntry* effect);
EffectEntry* effNameToPtr(const char* name);
EffectSet* effGetSet(const char* name);
void effCalcMayaAnim(struct MayaAnim* anim);
void effDeleteMayaAnim(struct MayaAnim* anim);
void effDrawMayaPoly(struct MayaPoly* poly);
struct MayaAnim* effMayaAnimAlloc(struct MayaAnimData* data);
void effPlayMayaAnim(struct MayaAnim* anim);
