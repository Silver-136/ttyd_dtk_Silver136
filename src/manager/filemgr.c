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

//.bss
char _filename[0x400]; //for fileAllocf/fileAsyncf vsprintf

//.sbss
FileManagerWork fileWork;

//.sdata
FileManagerWork* afp = &fileWork;

//local prototypes
void fileGarbageDataAdrClear(FileEntry* entry);
void fileGarbageDataAdrSet(void* data, s32 type);
FileEntry* _fileAlloc(const char* filename, s32 type, void (*callback)(FileEntry*));
void dvdReadDoneCallBack(s32 error, DVDFileInfo* info);
s32 fileAsync(const char* filename, s32 type, FileCallback callback);




void fileInit(void) {
    FileEntry* ptr;
    int i;

    afp->entries = __memAlloc(HEAP_DEFAULT, sizeof(FileEntry) * 0x400);
    afp->archiveType = 0;
    afp->firstused = 0;
    afp->lastused = 0;
    memset(afp->entries, 0, sizeof(FileEntry) * 0x400);

    for (ptr = afp->entries, i = 0; i < 0x400; i++, ptr++) {
        ptr->next = ptr + 1;
    }

    afp->firstavailable = afp->entries;
    afp->lastavailable = &afp->entries[0x3FF];
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
#define FT48(d) ((FileReloc48*)(d))
#define FT7(d) ((FileReloc7*)(d))
#define FT8(d) ((FileReloc8*)(d))
#define FT9(d) ((FileReloc9*)(d))
#define POSE(d) ((AnimationPoseData*)(d))
#define ANIM(d) ((AnimData*)(d))

void fileGarbageDataAdrClear(FileEntry* entry) {
    u16 i;
    u32 off;
    TEXPalette* tpl;
    s32 j, n;
    AnimTableEntry* e;
    u8* q;
    TEXHeader* img;
    CLUTHeader* pal;
    void* data = *entry->data;
    switch (entry->type) {
        case 1:
            if ((void*)POSE(data)->shapes >= data) {
                ADDR_WORD(POSE(data)->shapes) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->drawCalls) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->bufferVtxPos) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->posIndex) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->nrmVtxBuffer) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->nrmIndex) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->clr0VtxBuffer) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->clr0Index) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[0]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[1]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[2]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[3]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[4]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[5]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[6]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[7]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texVtxBuffer) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texAnimEntries) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texData) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texBinds) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->draws) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->groupVisibility) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->nodeData) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->groups) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->mpAnims) -= (AddrWord)data;
            }
            break;
        case 2:
            if ((void*)ANIM(data)->loopInfo >= data) {
                ADDR_WORD(ANIM(data)->loopInfo) -= (AddrWord)data;
                ADDR_WORD(ANIM(data)->keyframes) -= (AddrWord)data;
                ADDR_WORD(ANIM(data)->vtxPosDeltas) -= (AddrWord)data;
                ADDR_WORD(ANIM(data)->vtxNrmDeltas) -= (AddrWord)data;
                ADDR_WORD(ANIM(data)->texDeltas) -= (AddrWord)data;
                ADDR_WORD(ANIM(data)->visDeltas) -= (AddrWord)data;
                ADDR_WORD(ANIM(data)->nodeDeltas) -= (AddrWord)data;
                ADDR_WORD(ANIM(data)->valueKeys) -= (AddrWord)data;
            }
            break;
        case 3:
            if (FT48(data)->ptrs[0] >= data) {
                ADDR_WORD(FT48(data)->ptrs[0]) -= (AddrWord)data;
            }
            break;
        case 4:
            tpl = data;
            if (tpl->versionNumber != 0x0020AF30) {
                OSPanic("filemgr.c", VERSION_PICK(147, 153), "invalid version number for texture palette");
            }
            if ((void*)tpl->descriptorArray >= tpl) {
                for (i = 0; (u16)i < tpl->numDescriptors; i++) {
                    off = i * sizeof(TEXDescriptor); // FAKE: descriptor fields read by byte offset; descriptorArray[i].textureHeader/.CLUTHeader swaps the i and scaled-index registers (99.86%)
                    img = *(TEXHeader**)((u8*)tpl->descriptorArray + off);
                    if (img) {
                        if (img->unpacked) {
                            img->unpacked = FALSE;
                            img = *(TEXHeader**)((u8*)tpl->descriptorArray + off);
                            img->data -= (AddrWord)tpl;
                        }
                        *(AddrWord*)((u8*)tpl->descriptorArray + off) -= (AddrWord)tpl;
                    }
                    pal = *(CLUTHeader**)((u8*)tpl->descriptorArray + off + offsetof(TEXDescriptor, CLUTHeader));
                    if (pal) {
                        if (pal->unpacked) {
                            pal->unpacked = FALSE;
                            pal = *(CLUTHeader**)((u8*)tpl->descriptorArray + off + offsetof(TEXDescriptor, CLUTHeader));
                            pal->data -= (AddrWord)tpl;
                        }
                        *(AddrWord*)((u8*)tpl->descriptorArray + off + offsetof(TEXDescriptor, CLUTHeader)) -= (AddrWord)tpl;
                    }
                }
                ADDR_WORD(tpl->descriptorArray) -= (AddrWord)tpl;
            }
            break;
        case 5:
            if ((void*)POSE(data)->shapes >= data) {
                n = ((AnimationPoseData*)data)->mAnimCount;
                for (j = 0; j < n; j++) {
                    e = &((AnimationPoseData*)data)->mpAnims[j];
                    q = (u8*)e->data;
                    e->data = (AnimData*)(q - (AddrWord)data);
                    if ((void*)ANIM(q)->loopInfo >= q) {
                        ADDR_WORD(ANIM(q)->loopInfo) -= (AddrWord)q;
                        ADDR_WORD(ANIM(q)->keyframes) -= (AddrWord)q;
                        ADDR_WORD(ANIM(q)->vtxPosDeltas) -= (AddrWord)q;
                        ADDR_WORD(ANIM(q)->vtxNrmDeltas) -= (AddrWord)q;
                        ADDR_WORD(ANIM(q)->texDeltas) -= (AddrWord)q;
                        ADDR_WORD(ANIM(q)->visDeltas) -= (AddrWord)q;
                        ADDR_WORD(ANIM(q)->nodeDeltas) -= (AddrWord)q;
                        ADDR_WORD(ANIM(q)->valueKeys) -= (AddrWord)q;
                    }
                }
                ADDR_WORD(POSE(data)->shapes) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->drawCalls) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->bufferVtxPos) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->posIndex) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->nrmVtxBuffer) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->nrmIndex) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->clr0VtxBuffer) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->clr0Index) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[0]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[1]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[2]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[3]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[4]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[5]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[6]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[7]) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texVtxBuffer) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texAnimEntries) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texData) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->texBinds) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->draws) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->groupVisibility) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->nodeData) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->groups) -= (AddrWord)data;
                ADDR_WORD(POSE(data)->mpAnims) -= (AddrWord)data;
            }
            break;
        case 6:
            if (FT48(data)->ptrs[0] >= data) {
                ADDR_WORD(FT48(data)->ptrs[0]) -= (AddrWord)data;
            }
            break;
        case 7:
            if (FT7(data)->ptrs[0] >= data) {
                ADDR_WORD(FT7(data)->ptrs[0]) -= (AddrWord)data;
                ADDR_WORD(FT7(data)->ptrs[1]) -= (AddrWord)data;
                ADDR_WORD(FT7(data)->ptrs[2]) -= (AddrWord)data;
            }
            break;
        case 8:
            if (FT8(data)->ptrs[0] >= data) {
                ADDR_WORD(FT8(data)->ptrs[0]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[1]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[2]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[3]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[4]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[5]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[6]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[7]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[8]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[9]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[10]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[11]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[12]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[13]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[14]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[15]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[16]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[17]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[18]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[19]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[20]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[21]) -= (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[22]) -= (AddrWord)data;
            }
            break;
        case 9:
            if (FT9(data)->ptrs[0] >= data) {
                ADDR_WORD(FT9(data)->ptrs[0]) -= (AddrWord)data;
                ADDR_WORD(FT9(data)->ptrs[1]) -= (AddrWord)data;
                ADDR_WORD(FT9(data)->ptrs[2]) -= (AddrWord)data;
                ADDR_WORD(FT9(data)->ptrs[3]) -= (AddrWord)data;
            }
            break;
        case 10:
            if (FT48(data)->ptrs[0] >= data) {
                ADDR_WORD(FT48(data)->ptrs[0]) -= (AddrWord)data;
            }
            break;
        case 0:
        default:
            break;
    }
}

void fileGarbageDataAdrSet(void* data, s32 type) {
    u16 i;
    u32 off; u32 off2;
    TEXPalette* tpl;
    u8* q;
    s32 j, n;
    TEXHeader* img;
    CLUTHeader* pal;
    TEXHeader* img2;
    CLUTHeader* pal2;
    switch (type) {
        case 1:
            if ((void*)POSE(data)->shapes < data) {
                ADDR_WORD(POSE(data)->shapes) += (AddrWord)data;
                ADDR_WORD(POSE(data)->drawCalls) += (AddrWord)data;
                ADDR_WORD(POSE(data)->bufferVtxPos) += (AddrWord)data;
                ADDR_WORD(POSE(data)->posIndex) += (AddrWord)data;
                ADDR_WORD(POSE(data)->nrmVtxBuffer) += (AddrWord)data;
                ADDR_WORD(POSE(data)->nrmIndex) += (AddrWord)data;
                ADDR_WORD(POSE(data)->clr0VtxBuffer) += (AddrWord)data;
                ADDR_WORD(POSE(data)->clr0Index) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[0]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[1]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[2]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[3]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[4]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[5]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[6]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[7]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texVtxBuffer) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texAnimEntries) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texData) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texBinds) += (AddrWord)data;
                ADDR_WORD(POSE(data)->draws) += (AddrWord)data;
                ADDR_WORD(POSE(data)->groupVisibility) += (AddrWord)data;
                ADDR_WORD(POSE(data)->nodeData) += (AddrWord)data;
                ADDR_WORD(POSE(data)->groups) += (AddrWord)data;
                ADDR_WORD(POSE(data)->mpAnims) += (AddrWord)data;
            }
            break;
        case 2:
            if ((void*)ANIM(data)->loopInfo < data) {
                ADDR_WORD(ANIM(data)->loopInfo) += (AddrWord)data;
                ADDR_WORD(ANIM(data)->keyframes) += (AddrWord)data;
                ADDR_WORD(ANIM(data)->vtxPosDeltas) += (AddrWord)data;
                ADDR_WORD(ANIM(data)->vtxNrmDeltas) += (AddrWord)data;
                ADDR_WORD(ANIM(data)->texDeltas) += (AddrWord)data;
                ADDR_WORD(ANIM(data)->visDeltas) += (AddrWord)data;
                ADDR_WORD(ANIM(data)->nodeDeltas) += (AddrWord)data;
                ADDR_WORD(ANIM(data)->valueKeys) += (AddrWord)data;
            }
            break;
        case 3:
            if (FT48(data)->ptrs[0] < data) {
                ADDR_WORD(FT48(data)->ptrs[0]) += (AddrWord)data;
            }
            break;
        case 4:
            tpl = data;
            if (tpl->versionNumber != 0x0020AF30) {
                OSPanic("filemgr.c", VERSION_PICK(106, 112), "invalid version number for texture palette");
            }
            if ((void*)tpl->descriptorArray < tpl) {
                ADDR_WORD(tpl->descriptorArray) += (AddrWord)tpl;
                for (i = 0; (u16)i < tpl->numDescriptors; i++) {
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
        case 5:
            if ((void*)POSE(data)->shapes < data) {
                ADDR_WORD(POSE(data)->shapes) += (AddrWord)data;
                ADDR_WORD(POSE(data)->drawCalls) += (AddrWord)data;
                ADDR_WORD(POSE(data)->bufferVtxPos) += (AddrWord)data;
                ADDR_WORD(POSE(data)->posIndex) += (AddrWord)data;
                ADDR_WORD(POSE(data)->nrmVtxBuffer) += (AddrWord)data;
                ADDR_WORD(POSE(data)->nrmIndex) += (AddrWord)data;
                ADDR_WORD(POSE(data)->clr0VtxBuffer) += (AddrWord)data;
                ADDR_WORD(POSE(data)->clr0Index) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[0]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[1]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[2]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[3]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[4]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[5]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[6]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texIndex[7]) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texVtxBuffer) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texAnimEntries) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texData) += (AddrWord)data;
                ADDR_WORD(POSE(data)->texBinds) += (AddrWord)data;
                ADDR_WORD(POSE(data)->draws) += (AddrWord)data;
                ADDR_WORD(POSE(data)->groupVisibility) += (AddrWord)data;
                ADDR_WORD(POSE(data)->nodeData) += (AddrWord)data;
                ADDR_WORD(POSE(data)->groups) += (AddrWord)data;
                ADDR_WORD(POSE(data)->mpAnims) += (AddrWord)data;
                n = ((AnimationPoseData*)data)->mAnimCount;
                for (j = 0; j < n; j++) {
                    ADDR_WORD(((AnimationPoseData*)data)->mpAnims[j].data) += (AddrWord)data;
                    q = (u8*)((AnimationPoseData*)data)->mpAnims[j].data;
                    if ((void*)ANIM(q)->loopInfo < q) {
                        ADDR_WORD(ANIM(q)->loopInfo) += (AddrWord)q;
                        ADDR_WORD(ANIM(q)->keyframes) += (AddrWord)q;
                        ADDR_WORD(ANIM(q)->vtxPosDeltas) += (AddrWord)q;
                        ADDR_WORD(ANIM(q)->vtxNrmDeltas) += (AddrWord)q;
                        ADDR_WORD(ANIM(q)->texDeltas) += (AddrWord)q;
                        ADDR_WORD(ANIM(q)->visDeltas) += (AddrWord)q;
                        ADDR_WORD(ANIM(q)->nodeDeltas) += (AddrWord)q;
                        ADDR_WORD(ANIM(q)->valueKeys) += (AddrWord)q;
                    }
                }
            }
            break;
        case 6:
            if (FT48(data)->ptrs[0] < data) {
                ADDR_WORD(FT48(data)->ptrs[0]) += (AddrWord)data;
            }
            break;
        case 7:
            if (FT7(data)->ptrs[0] < data) {
                ADDR_WORD(FT7(data)->ptrs[0]) += (AddrWord)data;
                ADDR_WORD(FT7(data)->ptrs[1]) += (AddrWord)data;
                ADDR_WORD(FT7(data)->ptrs[2]) += (AddrWord)data;
            }
            break;
        case 8:
            if (FT8(data)->ptrs[0] < data) {
                ADDR_WORD(FT8(data)->ptrs[0]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[1]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[2]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[3]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[4]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[5]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[6]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[7]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[8]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[9]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[10]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[11]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[12]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[13]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[14]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[15]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[16]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[17]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[18]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[19]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[20]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[21]) += (AddrWord)data;
                ADDR_WORD(FT8(data)->ptrs[22]) += (AddrWord)data;
            }
            break;
        case 9:
            if (FT9(data)->ptrs[0] < data) {
                ADDR_WORD(FT9(data)->ptrs[0]) += (AddrWord)data;
                ADDR_WORD(FT9(data)->ptrs[1]) += (AddrWord)data;
                ADDR_WORD(FT9(data)->ptrs[2]) += (AddrWord)data;
                ADDR_WORD(FT9(data)->ptrs[3]) += (AddrWord)data;
            }
            break;
        case 10:
            if (FT48(data)->ptrs[0] < data) {
                ADDR_WORD(FT48(data)->ptrs[0]) += (AddrWord)data;
            }
            break;
        case 0:
        default:
            break;
    }
}

void fileGarbageMoveMem(void* data, FileEntry* file) {
    if (file->state == 3) {
        if (!file->entry) {
            fileGarbageDataAdrClear(file);
        }
    } else {
        fileGarbageDataAdrClear(file);
    }
    memmove(data, ((smartEntry*)file->data)->address, ((smartEntry*)file->data)->size);
    *file->data = data;
    if (file->state == 3) {
        if (file->entry) {
            file->entry->address = data;
        } else {
            fileGarbageDataAdrSet(*file->data, file->type);
        }
    } else {
        fileGarbageDataAdrSet(*file->data, file->type);
    }
}

void _fileGarbage(BOOL a1) {
    FileEntry* f;
    FileEntry* prev;
    FileEntry* first;
    FileEntry* next;

    if (a1 == 0) {
        for (f = afp->firstused; f; f = f->next) {
            if (f->state == 3) {
                if (f->entry) {
                    if (OSGetCurrentThread() == &dvdmgr_thread) {
                        f->entry->status = 8;
                    } else {
                        while (f->entry) {
                            OSYieldThread();
                        }
                    }
                }
                f->state = 2;
            }
        }
    } else if (a1 == 1) {
        for (f = afp->firstused; f; f = f->next) {
            if (f->state == 3) {
                if (f->entry) {
                    if (OSGetCurrentThread() == &dvdmgr_thread) {
                        continue;
                    }
                    while (f->entry) {
                        OSYieldThread();
                    }
                } else {
                    f->references--;
                    if ((s16)f->references <= -2) {
                        f->state = 2;
                    }
                }
            }
        }
    }

    prev = NULL;
    first = NULL;
    for (f = afp->firstused; f; f = next) {
        next = f->next;
        if (f->state == 2) {
            f->state = 0;
            if ((void**)&f->dataSlot != f->data) {
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

FileEntry* fileAllocf(s32 type, const char* format, ...) {
    va_list va;

    va_start(va, format);
    vsprintf(_filename, format, va);
    return _fileAlloc(_filename, type, NULL);
}

FileEntry* fileAlloc(const char* filename, s32 type) {
    return _fileAlloc(filename, type, NULL);
}

FileEntry* _fileAlloc(const char* filename, s32 type, void (*callback)(FileEntry*)) {
    void* handle;
    FileEntry *search;
    smartEntry* smart;
    DVDEntry* entry;
    FileEntry *newentry, *temp;
    u32 size, test;

    for (search = afp->firstused; search; search = search->next) {
        if (search->state && !strcmp(search->filename, filename)) {
#ifdef VERSION_G8ME01
            while (search->entry) { OSYieldThread(); }
#else
            while (search->entry) {;} // busy-waits until the loader thread clears the entry (US yields instead)
#endif
            if (search->state == 3) {
                search->state = 1;
                search->references = 0;
            } else if (search->state == 2) {
                search->state = 1;
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
        newentry->state = 1;
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
        test = size + ((size & 31) ? 32 - (size & 31) : 0);
        if (!test) {
            DVDMgrClose(entry);
            return NULL;
        }
        if (test & 31) {
            test += 32 - (test & 31);
        }
        smart = smartAlloc(test, 0);
        smart->file = newentry;
        DVDMgrRead(entry, smart->address, test, 0);
        DVDMgrClose(entry);
        newentry->data = &smart->address;
        newentry->state = 1;
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

void fileFree(FileEntry* handle) {
    if (handle) {
        if (handle->data) {
            if (handle->state == 1) {
                handle->references--;
                if (!handle->references) {
                    handle->state = 2;
                }
            }
        }
    }
}

void dvdReadDoneCallBack(s32 error, DVDFileInfo* info) {
    DVDEntry* entry;
    FileEntry* f;

    for (f = afp->firstused; f; f = f->next) {
        if (&f->entry->info == info) {
            break;
        }
    }
    entry = f->entry;
    if (error == -3) {
        f->entry = NULL;
        f->state = 2;
        f->type = 11;
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
    u32 size, test;

    for (search = afp->firstused; search; search = search->next) {
        if (search->state && !strcmp(search->filename, filename)) {
            if (search->entry) return 0;
            if (search->state == 3) return (s32)search;
            if (search->state == 2) {
                search->state = 3;
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
        newentry->state = 3;
        newentry->data = &newentry->dataSlot;
        *newentry->data = handle;
        newentry->references = 0;
        newentry->type = type;
        newentry->next = 0;
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
        test = size + ((size & 31) ? 32 - (size & 31) : 0);
        if (!test) {
            DVDMgrClose(entry);
            return 0;
        }
        DVDMgrClose(entry);
        if (test & 31) {
            test += 32 - (test & 31);
        }
        smart = smartAlloc(test, 0);
        smart->file = newentry;
        newentry->data = &smart->address;
        newentry->state = 3;
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
        newentry->entry = DVDMgrOpen(filename, 2u, 0);
        DVDMgrReadAsync(newentry->entry, smart->address, test, 0, dvdReadDoneCallBack);
        return 0;
    }
}

void fileSetCurrentArchiveType(s32 type) {
    afp->archiveType = type;
}
