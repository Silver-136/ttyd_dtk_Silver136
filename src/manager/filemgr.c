// File manager: loads files into the smart heap (synchronously or through dvdmgr), keeps them cached by
// name, and makes the pointers inside texture and animation files absolute after loading.

#include "version.h"
#include "manager/filemgr.h"
#include "dolphin/os.h"
#include "driver/animdrv.h"
#include "driver/arcdrv.h"
#include "memory.h"
#include "sdk/texPalette.h"
#include <stdarg.h>
#include <string.h>
#include "manager/dvdmgr.h"
#include "libc_decls.h"
#include "addr_word.h"

// File-entry pool capacity; unrelated to the filename buffer size.
enum { FILE_ENTRY_CAPACITY = 0x400 };

//.bss
#if VERSION <= VERSION_DEMO_JP
FileManagerWork fileWork;
#endif

char _filename[0x400]; //for fileAllocf/fileAsyncf vsprintf

//.sbss
FileManagerWork fileWork;

//.sdata
FileManagerWork* afp = &fileWork;

// local prototypes
static FileEntry* _fileAlloc(const char* filename, s32 type, void (*callback)(FileEntry*));
s32 fileAsync(const char* filename, s32 type, FileCallback callback);

/** Boot-time setup: allocates the 1024 file entries and chains them all into the free list. */
void fileInit(void) {
    FileEntry* ptr;
    int i;

    afp->entries = __memAlloc(HEAP_DEFAULT, sizeof(FileEntry) * FILE_ENTRY_CAPACITY);
    afp->archiveType = 0;
    afp->firstused = 0;
    afp->lastused = 0;
    memset(afp->entries, 0, sizeof(FileEntry) * FILE_ENTRY_CAPACITY);

    for (ptr = afp->entries, i = 0; i < FILE_ENTRY_CAPACITY; i++, ptr++) {
        ptr->next = ptr + 1;
    }

    afp->firstavailable = afp->entries;
    afp->lastavailable = &afp->entries[FILE_ENTRY_CAPACITY - 1];
    afp->lastavailable->next = NULL;
}

// file types 3 and 6-10 have no C reader; only the pointer words relocated here are known
typedef struct FileReloc48 {  // types 3, 6, 10
    u8 reserved_00[0x48];       // 0x0
    void* ptrs[1];                // 0x48
} FileReloc48;
typedef struct FileReloc7 {
    u8 reserved_00[0x64];       // 0x0
    void* ptrs[3];                // 0x64
} FileReloc7;
typedef struct FileReloc8 {
    u8 reserved_00[0xC8];       // 0x0
    void* ptrs[23];               // 0xC8
} FileReloc8;
typedef struct FileReloc9 {
    u8 reserved_00[0x58];       // 0x0
    void* ptrs[4];                // 0x58
} FileReloc9;

/**
 * Turns the absolute pointers inside a loaded file back into offsets from its start (per file type: animation
 * poses, animation data, TPL textures and the other relocated formats), so the data can be moved;
 * undone by fileGarbageDataAdrSet.
 */
static void fileGarbageDataAdrClear(FileEntry* entry) {
    u16 i;
    u32 off;
    TEXPalette* tpl;
    AnimationPoseData* pose;
    AnimData* anim;
    s32 j, n;
    AnimTableEntry* e;
    u8* q;
    TEXHeader* img;
    CLUTHeader* pal;
    void* data = *entry->data;
    switch (entry->type) {
        case FILE_TYPE_POSE_UNUSED:
            pose = data;
            if ((void*)pose->shapes >= data) {
                ADDR_WORD(pose->shapes) -= (AddrWord)data;
                ADDR_WORD(pose->drawCalls) -= (AddrWord)data;
                ADDR_WORD(pose->bufferVtxPos) -= (AddrWord)data;
                ADDR_WORD(pose->posIndex) -= (AddrWord)data;
                ADDR_WORD(pose->nrmVtxBuffer) -= (AddrWord)data;
                ADDR_WORD(pose->nrmIndex) -= (AddrWord)data;
                ADDR_WORD(pose->clr0VtxBuffer) -= (AddrWord)data;
                ADDR_WORD(pose->clr0Index) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[0]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[1]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[2]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[3]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[4]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[5]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[6]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[7]) -= (AddrWord)data;
                ADDR_WORD(pose->texVtxBuffer) -= (AddrWord)data;
                ADDR_WORD(pose->texAnimEntries) -= (AddrWord)data;
                ADDR_WORD(pose->texData) -= (AddrWord)data;
                ADDR_WORD(pose->texBinds) -= (AddrWord)data;
                ADDR_WORD(pose->draws) -= (AddrWord)data;
                ADDR_WORD(pose->groupVisibility) -= (AddrWord)data;
                ADDR_WORD(pose->nodeData) -= (AddrWord)data;
                ADDR_WORD(pose->groups) -= (AddrWord)data;
                ADDR_WORD(pose->anims) -= (AddrWord)data;
            }
            break;
        case FILE_TYPE_ANIM_DATA_UNUSED:
            anim = data;
            if ((void*)anim->loopInfo >= data) {
                ADDR_WORD(anim->loopInfo) -= (AddrWord)data;
                ADDR_WORD(anim->keyframes) -= (AddrWord)data;
                ADDR_WORD(anim->vtxPosDeltas) -= (AddrWord)data;
                ADDR_WORD(anim->vtxNrmDeltas) -= (AddrWord)data;
                ADDR_WORD(anim->texDeltas) -= (AddrWord)data;
                ADDR_WORD(anim->visDeltas) -= (AddrWord)data;
                ADDR_WORD(anim->nodeDeltas) -= (AddrWord)data;
                ADDR_WORD(anim->valueKeys) -= (AddrWord)data;
            }
            break;
        case 3:
            if (((FileReloc48*)data)->ptrs[0] >= data) {
                ADDR_WORD(((FileReloc48*)data)->ptrs[0]) -= (AddrWord)data;
            }
            break;
        case FILE_TYPE_TPL:
            tpl = data;
            if (tpl->versionNumber != PALETTE_VERSION) {
#if VERSION >= VERSION_DEMO_EU
                OSPanic("filemgr.c", 150, "invalid version number for texture palette");
#else
                OSPanic("filemgr.c", VERSION_PICK(147, 153), "invalid version number for texture palette");
#endif
            }
            if ((void*)tpl->descriptorArray >= tpl) {
                // FAKE: no-op (u16) cast on the u16 counter; the plain comparison changes the object
                for (i = 0; (u16)i < tpl->numDescriptors; i++) {
                    off = i * sizeof(TEXDescriptor); // FAKE: descriptor relocation stores retain byte-offset addressing; descriptor field loads use typed indexing
                    img = tpl->descriptorArray[i].textureHeader;
                    if (img) {
                        if (img->unpacked) {
                            img->unpacked = FALSE;
                            img = tpl->descriptorArray[i].textureHeader;
                            img->data -= (AddrWord)tpl;
                        }
                        *(AddrWord*)((u8*)tpl->descriptorArray + off) -= (AddrWord)tpl; // FAKE: byte-offset store (ADDR_WORD(tpl->descriptorArray[i].textureHeader) changes the object)
                    }
                    pal = tpl->descriptorArray[i].CLUTHeader;
                    if (pal) {
                        if (pal->unpacked) {
                            pal->unpacked = FALSE;
                            pal = tpl->descriptorArray[i].CLUTHeader;
                            pal->data -= (AddrWord)tpl;
                        }
                        *(AddrWord*)((u8*)tpl->descriptorArray + off + offsetof(TEXDescriptor, CLUTHeader)) -= (AddrWord)tpl; // FAKE: byte-offset store (ADDR_WORD(tpl->descriptorArray[i].CLUTHeader) changes the object)
                    }
                }
                ADDR_WORD(tpl->descriptorArray) -= (AddrWord)tpl;
            }
            break;
        case FILE_TYPE_ANIM:
            pose = data;
            if ((void*)pose->shapes >= data) {
                n = pose->animCount;
                for (j = 0; j < n; j++) {
                    e = &pose->anims[j];
                    q = (u8*)e->data;
                    e->data = (AnimData*)(q - (AddrWord)data);
                    anim = (AnimData*)q;
                    if ((void*)anim->loopInfo >= q) {
                        ADDR_WORD(anim->loopInfo) -= (AddrWord)q;
                        ADDR_WORD(anim->keyframes) -= (AddrWord)q;
                        ADDR_WORD(anim->vtxPosDeltas) -= (AddrWord)q;
                        ADDR_WORD(anim->vtxNrmDeltas) -= (AddrWord)q;
                        ADDR_WORD(anim->texDeltas) -= (AddrWord)q;
                        ADDR_WORD(anim->visDeltas) -= (AddrWord)q;
                        ADDR_WORD(anim->nodeDeltas) -= (AddrWord)q;
                        ADDR_WORD(anim->valueKeys) -= (AddrWord)q;
                    }
                }
                ADDR_WORD(pose->shapes) -= (AddrWord)data;
                ADDR_WORD(pose->drawCalls) -= (AddrWord)data;
                ADDR_WORD(pose->bufferVtxPos) -= (AddrWord)data;
                ADDR_WORD(pose->posIndex) -= (AddrWord)data;
                ADDR_WORD(pose->nrmVtxBuffer) -= (AddrWord)data;
                ADDR_WORD(pose->nrmIndex) -= (AddrWord)data;
                ADDR_WORD(pose->clr0VtxBuffer) -= (AddrWord)data;
                ADDR_WORD(pose->clr0Index) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[0]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[1]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[2]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[3]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[4]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[5]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[6]) -= (AddrWord)data;
                ADDR_WORD(pose->texIndex[7]) -= (AddrWord)data;
                ADDR_WORD(pose->texVtxBuffer) -= (AddrWord)data;
                ADDR_WORD(pose->texAnimEntries) -= (AddrWord)data;
                ADDR_WORD(pose->texData) -= (AddrWord)data;
                ADDR_WORD(pose->texBinds) -= (AddrWord)data;
                ADDR_WORD(pose->draws) -= (AddrWord)data;
                ADDR_WORD(pose->groupVisibility) -= (AddrWord)data;
                ADDR_WORD(pose->nodeData) -= (AddrWord)data;
                ADDR_WORD(pose->groups) -= (AddrWord)data;
                ADDR_WORD(pose->anims) -= (AddrWord)data;
            }
            break;
        case 6:
            if (((FileReloc48*)data)->ptrs[0] >= data) {
                ADDR_WORD(((FileReloc48*)data)->ptrs[0]) -= (AddrWord)data;
            }
            break;
        case 7:
            if (((FileReloc7*)data)->ptrs[0] >= data) {
                ADDR_WORD(((FileReloc7*)data)->ptrs[0]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc7*)data)->ptrs[1]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc7*)data)->ptrs[2]) -= (AddrWord)data;
            }
            break;
        case 8:
            if (((FileReloc8*)data)->ptrs[0] >= data) {
                ADDR_WORD(((FileReloc8*)data)->ptrs[0]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[1]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[2]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[3]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[4]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[5]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[6]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[7]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[8]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[9]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[10]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[11]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[12]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[13]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[14]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[15]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[16]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[17]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[18]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[19]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[20]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[21]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[22]) -= (AddrWord)data;
            }
            break;
        case 9:
            if (((FileReloc9*)data)->ptrs[0] >= data) {
                ADDR_WORD(((FileReloc9*)data)->ptrs[0]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc9*)data)->ptrs[1]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc9*)data)->ptrs[2]) -= (AddrWord)data;
                ADDR_WORD(((FileReloc9*)data)->ptrs[3]) -= (AddrWord)data;
            }
            break;
        case 10:
            if (((FileReloc48*)data)->ptrs[0] >= data) {
                ADDR_WORD(((FileReloc48*)data)->ptrs[0]) -= (AddrWord)data;
            }
            break;
        case FILE_TYPE_RAW:
        default:
            break;
    }
}

/**
 * Relocates a freshly loaded or moved file: adds the data address to every offset field its type stores,
 * unless the fields are already absolute. The textures' image and palette headers are fixed up here too.
 */
static void fileGarbageDataAdrSet(void* data, s32 type) {
    u16 i;
    u32 off;
    u32 off2;
    TEXPalette* tpl;
    AnimationPoseData* pose;
    AnimData* anim;
    u8* q;
    s32 j, n;
    TEXHeader* img;
    CLUTHeader* pal;
    TEXHeader* img2;
    CLUTHeader* pal2;
    switch (type) {
        case FILE_TYPE_POSE_UNUSED:
            pose = data;
            if ((void*)pose->shapes < data) {
                ADDR_WORD(pose->shapes) += (AddrWord)data;
                ADDR_WORD(pose->drawCalls) += (AddrWord)data;
                ADDR_WORD(pose->bufferVtxPos) += (AddrWord)data;
                ADDR_WORD(pose->posIndex) += (AddrWord)data;
                ADDR_WORD(pose->nrmVtxBuffer) += (AddrWord)data;
                ADDR_WORD(pose->nrmIndex) += (AddrWord)data;
                ADDR_WORD(pose->clr0VtxBuffer) += (AddrWord)data;
                ADDR_WORD(pose->clr0Index) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[0]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[1]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[2]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[3]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[4]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[5]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[6]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[7]) += (AddrWord)data;
                ADDR_WORD(pose->texVtxBuffer) += (AddrWord)data;
                ADDR_WORD(pose->texAnimEntries) += (AddrWord)data;
                ADDR_WORD(pose->texData) += (AddrWord)data;
                ADDR_WORD(pose->texBinds) += (AddrWord)data;
                ADDR_WORD(pose->draws) += (AddrWord)data;
                ADDR_WORD(pose->groupVisibility) += (AddrWord)data;
                ADDR_WORD(pose->nodeData) += (AddrWord)data;
                ADDR_WORD(pose->groups) += (AddrWord)data;
                ADDR_WORD(pose->anims) += (AddrWord)data;
            }
            break;
        case FILE_TYPE_ANIM_DATA_UNUSED:
            anim = data;
            if ((void*)anim->loopInfo < data) {
                ADDR_WORD(anim->loopInfo) += (AddrWord)data;
                ADDR_WORD(anim->keyframes) += (AddrWord)data;
                ADDR_WORD(anim->vtxPosDeltas) += (AddrWord)data;
                ADDR_WORD(anim->vtxNrmDeltas) += (AddrWord)data;
                ADDR_WORD(anim->texDeltas) += (AddrWord)data;
                ADDR_WORD(anim->visDeltas) += (AddrWord)data;
                ADDR_WORD(anim->nodeDeltas) += (AddrWord)data;
                ADDR_WORD(anim->valueKeys) += (AddrWord)data;
            }
            break;
        case 3:
            if (((FileReloc48*)data)->ptrs[0] < data) {
                ADDR_WORD(((FileReloc48*)data)->ptrs[0]) += (AddrWord)data;
            }
            break;
        case FILE_TYPE_TPL:
            tpl = data;
            if (tpl->versionNumber != PALETTE_VERSION) {
#if VERSION >= VERSION_DEMO_EU
                OSPanic("filemgr.c", 109, "invalid version number for texture palette");
#else
                OSPanic("filemgr.c", VERSION_PICK(106, 112), "invalid version number for texture palette");
#endif
            }
            if ((void*)tpl->descriptorArray < tpl) {
                ADDR_WORD(tpl->descriptorArray) += (AddrWord)tpl;

                // FAKE: no-op (u16) cast on the u16 counter; the plain comparison changes the object
                for (i = 0; (u16)i < tpl->numDescriptors; i++) {
                    // FAKE: byte-offset descriptor access as in fileGarbageDataAdrClear; typed indexing changes the object
                    off = i * sizeof(TEXDescriptor);
                    img = *(TEXHeader**)((u8*)tpl->descriptorArray + off);
                    if (img) {
                        *(TEXHeader**)((u8*)tpl->descriptorArray + off) = (TEXHeader*)((u8*)tpl + (AddrWord)img);
                        img2 = *(TEXHeader**)((u8*)tpl->descriptorArray + off);
                        if (!img2->unpacked) {
                            img2->data = (char*)tpl + (AddrWord)img2->data;
                            (*(TEXHeader**)((u8*)tpl->descriptorArray + off))->unpacked = TRUE;
                        }
                    }
                    off2 = off + offsetof(TEXDescriptor, CLUTHeader);
                    pal = *(CLUTHeader**)((u8*)tpl->descriptorArray + off2);
                    if (pal) {
                        *(CLUTHeader**)((u8*)tpl->descriptorArray + off2) = (CLUTHeader*)((u8*)tpl + (AddrWord)pal);
                        pal2 = *(CLUTHeader**)((u8*)tpl->descriptorArray + off2);
                        if (!pal2->unpacked) {
                            pal2->data = (char*)tpl + (AddrWord)pal2->data;
                            (*(CLUTHeader**)((u8*)tpl->descriptorArray + off2))->unpacked = TRUE;
                        }
                    }
                }
            }
            break;
        case FILE_TYPE_ANIM:
            pose = data;
            if ((void*)pose->shapes < data) {
                ADDR_WORD(pose->shapes) += (AddrWord)data;
                ADDR_WORD(pose->drawCalls) += (AddrWord)data;
                ADDR_WORD(pose->bufferVtxPos) += (AddrWord)data;
                ADDR_WORD(pose->posIndex) += (AddrWord)data;
                ADDR_WORD(pose->nrmVtxBuffer) += (AddrWord)data;
                ADDR_WORD(pose->nrmIndex) += (AddrWord)data;
                ADDR_WORD(pose->clr0VtxBuffer) += (AddrWord)data;
                ADDR_WORD(pose->clr0Index) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[0]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[1]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[2]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[3]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[4]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[5]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[6]) += (AddrWord)data;
                ADDR_WORD(pose->texIndex[7]) += (AddrWord)data;
                ADDR_WORD(pose->texVtxBuffer) += (AddrWord)data;
                ADDR_WORD(pose->texAnimEntries) += (AddrWord)data;
                ADDR_WORD(pose->texData) += (AddrWord)data;
                ADDR_WORD(pose->texBinds) += (AddrWord)data;
                ADDR_WORD(pose->draws) += (AddrWord)data;
                ADDR_WORD(pose->groupVisibility) += (AddrWord)data;
                ADDR_WORD(pose->nodeData) += (AddrWord)data;
                ADDR_WORD(pose->groups) += (AddrWord)data;
                ADDR_WORD(pose->anims) += (AddrWord)data;
                n = pose->animCount;
                for (j = 0; j < n; j++) {
                    ADDR_WORD(pose->anims[j].data) += (AddrWord)data;
                    q = (u8*)pose->anims[j].data;
                    anim = (AnimData*)q;
                    if ((void*)anim->loopInfo < q) {
                        ADDR_WORD(anim->loopInfo) += (AddrWord)q;
                        ADDR_WORD(anim->keyframes) += (AddrWord)q;
                        ADDR_WORD(anim->vtxPosDeltas) += (AddrWord)q;
                        ADDR_WORD(anim->vtxNrmDeltas) += (AddrWord)q;
                        ADDR_WORD(anim->texDeltas) += (AddrWord)q;
                        ADDR_WORD(anim->visDeltas) += (AddrWord)q;
                        ADDR_WORD(anim->nodeDeltas) += (AddrWord)q;
                        ADDR_WORD(anim->valueKeys) += (AddrWord)q;
                    }
                }
            }
            break;
        case 6:
            if (((FileReloc48*)data)->ptrs[0] < data) {
                ADDR_WORD(((FileReloc48*)data)->ptrs[0]) += (AddrWord)data;
            }
            break;
        case 7:
            if (((FileReloc7*)data)->ptrs[0] < data) {
                ADDR_WORD(((FileReloc7*)data)->ptrs[0]) += (AddrWord)data;
                ADDR_WORD(((FileReloc7*)data)->ptrs[1]) += (AddrWord)data;
                ADDR_WORD(((FileReloc7*)data)->ptrs[2]) += (AddrWord)data;
            }
            break;
        case 8:
            if (((FileReloc8*)data)->ptrs[0] < data) {
                ADDR_WORD(((FileReloc8*)data)->ptrs[0]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[1]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[2]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[3]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[4]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[5]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[6]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[7]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[8]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[9]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[10]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[11]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[12]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[13]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[14]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[15]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[16]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[17]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[18]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[19]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[20]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[21]) += (AddrWord)data;
                ADDR_WORD(((FileReloc8*)data)->ptrs[22]) += (AddrWord)data;
            }
            break;
        case 9:
            if (((FileReloc9*)data)->ptrs[0] < data) {
                ADDR_WORD(((FileReloc9*)data)->ptrs[0]) += (AddrWord)data;
                ADDR_WORD(((FileReloc9*)data)->ptrs[1]) += (AddrWord)data;
                ADDR_WORD(((FileReloc9*)data)->ptrs[2]) += (AddrWord)data;
                ADDR_WORD(((FileReloc9*)data)->ptrs[3]) += (AddrWord)data;
            }
            break;
        case 10:
            if (((FileReloc48*)data)->ptrs[0] < data) {
                ADDR_WORD(((FileReloc48*)data)->ptrs[0]) += (AddrWord)data;
            }
            break;
        case FILE_TYPE_RAW:
        default:
            break;
    }
}

/**
 * Moves a file's data to data during smart-heap compaction (memory.c),
 * unrelocating it first and relocating it at the new address.
 * A file still being read from disc only gets its DVD read target moved.
 */
void fileGarbageMoveMem(void* data, FileEntry* file) {
    if (file->state == FILE_STATE_PRELOADED) {
        if (!file->entry) {
            fileGarbageDataAdrClear(file);
        }
    } else {
        fileGarbageDataAdrClear(file);
    }

    // Smart-backed files store &smart->address in data; address is the first smartEntry field.
    memmove(data, ((smartEntry*)file->data)->address, ((smartEntry*)file->data)->size);
    *file->data = data;
    if (file->state == FILE_STATE_PRELOADED) {
        if (file->entry) {
            file->entry->address = data;
        } else {
            fileGarbageDataAdrSet(*file->data, file->type);
        }
    } else {
        fileGarbageDataAdrSet(*file->data, file->type);
    }
}

/**
 * Releases cached files so their entries and memory can be reused;
 * memory.c and the allocators call it when they run out.
 * With agePreloadedEntries 1, ages preloaded files (state 3) and drops them after two passes;
 * with 0, drops every preloaded file at once, waiting out reads in flight.
 * Then all released files (state 2) are freed and their entries returned to the free list.
 */
void _fileGarbage(BOOL agePreloadedEntries) {
    FileEntry* f;
    FileEntry* prev;
    FileEntry* first;
    FileEntry* next;

    if (agePreloadedEntries == 0) {
        for (f = afp->firstused; f; f = f->next) {
            if (f->state == FILE_STATE_PRELOADED) {
                if (f->entry) {
                    if (OSGetCurrentThread() == &dvdmgr_thread) {
                        f->entry->status = DVDMGR_CLOSED;
                    } else {
                        while (f->entry) {
                            OSYieldThread();
                        }
                    }
                }
                f->state = FILE_STATE_RELEASED;
            }
        }
    } else if (agePreloadedEntries == 1) {
        for (f = afp->firstused; f; f = f->next) {
            if (f->state == FILE_STATE_PRELOADED) {
                if (f->entry) {
                    if (OSGetCurrentThread() == &dvdmgr_thread) {
                        continue;
                    }
                    while (f->entry) {
                        OSYieldThread();
                    }
                } else {
                    f->references--;
                    if (f->references <= -2) {
                        f->state = FILE_STATE_RELEASED;
                    }
                }
            }
        }
    }

    prev = NULL;
    first = NULL;
    for (f = afp->firstused; f; f = next) {
        next = f->next;
        if (f->state == FILE_STATE_RELEASED) {
            f->state = FILE_STATE_FREE;
            if (&f->dataSlot != f->data) {
                smartFree((smartEntry*)f->data);
            }
            if (prev) {
                prev->next = f->next;
            }
            if (!afp->firstavailable) {
                afp->firstavailable = f;
            } else {
                afp->lastavailable->next = f;
            }
            f->next = NULL;
            afp->lastavailable = f;
        } else {
            if (!prev) {
                first = f;
            }
            prev = f;
        }
    }
    afp->firstused = first;
    afp->lastused = prev;
}

/** fileAlloc with a printf-style file name (formatted into the shared _filename buffer). */
FileEntry* fileAllocf(s32 type, const char* format, ...) {
    va_list va;

    va_start(va, format);
    vsprintf(_filename, format, va);
    return _fileAlloc(_filename, type, NULL);
}

/**
 * Loads a file synchronously, or reuses the cached copy of it, and takes a reference;
 * returns NULL if it does not exist. type picks how its internal pointers are relocated.
 */
FileEntry* fileAlloc(const char* filename, s32 type) {
    return _fileAlloc(filename, type, NULL);
}

/**
 * Shared body of fileAlloc: returns a cached entry with the same name (waiting for its read,
 * reviving it if released or preloaded), otherwise looks the name up in the open archives and then on disc,
 * reading it into a 32-byte-aligned smart heap block. Frees cached files when no entry is left.
 * callback is unused.
 */
static FileEntry* _fileAlloc(const char* filename, s32 type, void (*callback)(FileEntry*)) {
    void* handle;
    FileEntry* search;
    smartEntry* smart;
    DVDEntry* entry;
    FileEntry *newentry, *temp;
    u32 size, alignedSize;

    for (search = afp->firstused; search; search = search->next) {
        if (search->state && !strcmp(search->filename, filename)) {
#if VERSION >= VERSION_US
            while (search->entry) {
                OSYieldThread();
            }
#else
            // busy-waits until the loader thread clears the entry (US and later yield instead)
            while (search->entry) {
            }
#endif
            if (search->state == FILE_STATE_PRELOADED) {
                search->state = FILE_STATE_LOADED;
                search->references = 0;
            } else if (search->state == FILE_STATE_RELEASED) {
                search->state = FILE_STATE_LOADED;
            }
            search->references++;
            return search;
        }
    }
    newentry = afp->firstavailable;
    if (!newentry) {
        _fileGarbage(1);
        newentry = afp->firstavailable;
    }
    if (!newentry) {
        _fileGarbage(0);
        newentry = afp->firstavailable;
    }

    handle = NULL;
    temp = newentry->next;

    switch (afp->archiveType) {
        case 0:
            handle = arcOpen(filename, NULL, NULL);
            break;
        case 1:
            handle = arcOpen(filename, NULL, NULL);
            if (!handle) {
                handle = arcOpen(filename, NULL, NULL);
            }
            if (!handle) {
                handle = arcOpen(filename, NULL, NULL);
            }
            break;
        case 2:
            handle = arcOpen(filename, NULL, NULL);
            if (!handle) {
                handle = arcOpen(filename, NULL, NULL);
            }
            if (!handle) {
                handle = arcOpen(filename, NULL, NULL);
            }
            break;
    }
    if (handle) {
        fileGarbageDataAdrSet(handle, type);
    }
    if (handle) {
        newentry->state = FILE_STATE_LOADED;
        newentry->data = &newentry->dataSlot;
        *newentry->data = handle;
        newentry->references = 1;
        newentry->type = type;
        newentry->next = NULL;
        strcpy(newentry->filename, filename);
        newentry->entry = NULL;
    } else {
        entry = DVDMgrOpen(filename, 2, 0);
        if (!entry) {
            return NULL;
        }
        size = DVDMgrGetLength(entry);
        alignedSize = size + ((size & 31) ? 32 - (size & 31) : 0);
        if (!alignedSize) {
            DVDMgrClose(entry);
            return NULL;
        }
        if (alignedSize & 31) {
            alignedSize += 32 - (alignedSize & 31);
        }
        smart = smartAlloc(alignedSize, SMART_GROUP_PERMANENT);
        smart->file = newentry;
        DVDMgrRead(entry, smart->address, alignedSize, 0);
        DVDMgrClose(entry);
        newentry->data = &smart->address;
        newentry->state = FILE_STATE_LOADED;
        newentry->references = 1;
        newentry->type = type;
        newentry->next = NULL;
        strcpy(newentry->filename, filename);
        newentry->entry = NULL;
        fileGarbageDataAdrSet(smart->address, type);
    }
    afp->firstavailable = temp;
    if (!afp->firstavailable) {
        afp->lastavailable = NULL;
    }
    if (!afp->firstused) {
        afp->firstused = newentry;
        afp->lastused = newentry;
    } else {
        afp->lastused->next = newentry;
        afp->lastused = newentry;
    }
    return newentry;
}

/**
 * Drops a reference to a loaded file; at zero the file is marked released but stays cached
 * until _fileGarbage frees it.
 */
void fileFree(FileEntry* handle) {
    if (handle) {
        if (handle->data) {
            if (handle->state == FILE_STATE_LOADED) {
                handle->references--;
                if (!handle->references) {
                    handle->state = FILE_STATE_RELEASED;
                }
            }
        }
    }
}

/**
 * dvdmgr callback when an asynchronous file read ends:
 * relocates the data and runs the file's callback,
 * or on a canceled read (-3) marks the entry released with type 11.
 */
static void dvdReadDoneCallBack(s32 error, DVDFileInfo* info) {
    DVDEntry* entry;
    FileEntry* f;

    for (f = afp->firstused; f; f = f->next) {
        if (&f->entry->info == info) {
            break;
        }
    }
    entry = f->entry;
    if (error == DVD_RESULT_CANCELED) {
        f->entry = NULL;
        f->state = FILE_STATE_RELEASED;
        f->type = FILE_TYPE_CANCELED;
        DVDMgrClose(entry);
    } else {
        f->entry = NULL;
        fileGarbageDataAdrSet(*f->data, f->type);
        DVDMgrClose(entry);
        if (f->callback) {
            f->callback(f);
        }
    }
}

/** fileAsync with a printf-style file name. */
s32 fileAsyncf(s32 type, FileCallback callback, const char* format, ...) {
    va_list va;

    va_start(va, format);
    vsprintf(_filename, format, va);
    return fileAsync(_filename, type, callback);
}

s32 fileAsync(const char* filename, s32 type, FileCallback callback) {
    FileEntry *search, *newentry, *temp;
    smartEntry* smart;
    DVDEntry* entry;
    void* handle;
    u32 size, alignedSize;

    for (search = afp->firstused; search; search = search->next) {
        if (search->state && !strcmp(search->filename, filename)) {
            if (search->entry) {
                return 0;
            }
            if (search->state == FILE_STATE_PRELOADED) {
                return (s32)search;
            }
            if (search->state == FILE_STATE_RELEASED) {
                search->state = FILE_STATE_PRELOADED;
            }
            return (s32)search;
        }
    }

    newentry = afp->firstavailable;
    if (!newentry) {
        _fileGarbage(1);
        newentry = afp->firstavailable;
    }
    if (!newentry) {
        _fileGarbage(0);
        newentry = afp->firstavailable;
    }

    handle = NULL;
    temp = newentry->next;

    switch (afp->archiveType) {
        case 0:
            handle = arcOpen(filename, NULL, NULL);
            break;
        case 1:
            handle = arcOpen(filename, NULL, NULL);
            if (!handle) {
                handle = arcOpen(filename, NULL, NULL);
            }
            if (!handle) {
                handle = arcOpen(filename, NULL, NULL);
            }
            break;
        case 2:
            handle = arcOpen(filename, NULL, NULL);
            if (!handle) {
                handle = arcOpen(filename, NULL, NULL);
            }
            if (!handle) {
                handle = arcOpen(filename, NULL, NULL);
            }
            break;
    }

    if (handle) {
        fileGarbageDataAdrSet(handle, type);
    }

    if (handle) {
        newentry->state = FILE_STATE_PRELOADED;
        newentry->data = &newentry->dataSlot;
        *newentry->data = handle;
        newentry->references = 0;
        newentry->type = type;
        newentry->next = NULL;
        strcpy(newentry->filename, filename);
        newentry->entry = NULL;
        afp->firstavailable = afp->firstavailable->next;
        if (!afp->firstavailable) {
            afp->lastavailable = NULL;
        }
        if (!afp->firstused) {
            afp->firstused = newentry;
            afp->lastused = newentry;
        } else {
            afp->lastused->next = newentry;
            afp->lastused = newentry;
        }
        return (s32)newentry;
    } else {
        entry = DVDMgrOpen(filename, 2, 0);
        if (!entry) {
            return -1;
        }
        size = DVDMgrGetLength(entry);
        alignedSize = size + ((size & 31) ? 32 - (size & 31) : 0);
        if (!alignedSize) {
            DVDMgrClose(entry);
            return 0;
        }
        DVDMgrClose(entry);
        if (alignedSize & 31) {
            alignedSize += 32 - (alignedSize & 31);
        }
        smart = smartAlloc(alignedSize, SMART_GROUP_PERMANENT);
        smart->file = newentry;
        newentry->data = &smart->address;
        newentry->state = FILE_STATE_PRELOADED;
        newentry->references = 0;
        newentry->type = type;
        newentry->next = NULL;
        newentry->callback = callback;
        strcpy(newentry->filename, filename);
        afp->firstavailable = temp;
        if (!afp->firstavailable) {
            afp->lastavailable = NULL;
        }
        if (!afp->firstused) {
            afp->firstused = newentry;
            afp->lastused = newentry;
        } else {
            afp->lastused->next = newentry;
            afp->lastused = newentry;
        }
        newentry->entry = DVDMgrOpen(filename, 2, 0);
        DVDMgrReadAsync(newentry->entry, smart->address, alignedSize, 0, dvdReadDoneCallBack);
        return 0;
    }
}

/** Archive type 0-2: how many times _fileAlloc and fileAsync try arcOpen (0 once, 1 and 2 three times). */
void fileSetCurrentArchiveType(s32 type) {
    afp->archiveType = type;
}
