// Animation driver: loads the paper animation files (.bin) and their textures, evaluates the
// keyframes and draws the animation poses of Mario, the partners, NPCs and items. Many helpers here
// are inlined into their callers.

#include "version.h"
#include "driver/animdrv.h"
#include <dolphin/os/OSFastCast.h>
#include "driver/dispdrv.h"
#include "driver/mapdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include "addr_word.h"
#include "dolphin/os.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "himo.h"

#include <stdio.h>

#include <string.h>
#include "driver/camdrv.h"
#include "manager/filemgr.h"
#include "libc_decls.h"
#include "sdk/texPalette.h"
#include "sdk/DEMOInit.h"

#define TESTHEAP_SIZE (u32)(1.5 * 1024 * 1024)

//.data
u8 _vivihimoData[3][0x400] ATTRIBUTE_ALIGN(32) = {
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x4C, 0xCC, 0xCD, 0x3F, 0x99, 0x99, 0x9A, 0x3F, 0x4C, 0xCC, 0xCD,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x4C, 0xCC, 0xCD, 0x3F, 0x33, 0x33, 0x33,
    0x3F, 0x00, 0x00, 0x00, 0x3F, 0x33, 0x33, 0x33, 0x3E, 0xCC, 0xCC, 0xCD, 0x3F, 0x33, 0x33, 0x33,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x4C, 0xCC, 0xCD, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x4C, 0xCC, 0xCD, 0x3F, 0x4C, 0xCC, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x4C, 0xCC, 0xCD,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x4C, 0xCC, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x4C, 0xCC, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x4C, 0xCC, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x4C, 0xCC, 0xCD,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x4C, 0xCC, 0xCD, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x4C, 0xCC, 0xCD, 0x3F, 0x66, 0x66, 0x66, 0x3F, 0x33, 0x33, 0x33,
    0x3F, 0x19, 0x99, 0x9A, 0x3F, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
};

#if VERSION <= VERSION_DEMO_US
s32 vivihimoTailGroupNo[4] ATTRIBUTE_ALIGN(32) = {0, 2, -1, -1};
void* vivihimoData[4] = {
    NULL,
    _vivihimoData[0],
    _vivihimoData[1],
    _vivihimoData[2],
};
#endif
const s32 g_moamoa_chain_num = 0x2F;
MoaChain g_moamoa_chain_tbl[47] = {
    { "joint_00", "jointdai1_A01" },
    { "jointdai1_A01", "jointdai1_A02" },
    { "jointdai1_A02", "jointdai1_A03" },
    { "jointdai1_A03", "jointdai1_A04" },
    { "jointdai1_A04", "jointdai1_A05" },
    { "joint_00", "jointdai1_B01" },
    { "jointdai1_B01", "jointdai1_B02" },
    { "jointdai1_B02", "jointdai1_B03" },
    { "jointdai1_B03", "jointdai1_B04" },
    { "jointdai1_B04", "jointdai1_B05" },
    { "joint_00", "jointdai1_C01" },
    { "jointdai1_C01", "jointdai1_C02" },
    { "jointdai1_C02", "jointdai1_C03" },
    { "jointdai1_C03", "jointdai1_C04" },
    { "jointdai1_C04", "jointdai1_C05" },
    { "joint_00", "jointdai2_01" },
    { "jointdai2_01", "jointdai2_02" },
    { "jointdai2_02", "jointdai2_03" },
    { "jointdai2_03", "jointdai2_04" },
    { "jointdai2_04", "jointdai2_05" },
    { "jointdai2_05", "jointdai2_06" },
    { "jointdai2_06", "jointdai2_07" },
    { "jointdai2_07", "jointdai2_upper01" },
    { "jointdai2_upper01", "jointdai2_upper02" },
    { "jointdai2_upper02", "jointdai2_upper03" },
    { "jointdai2_upper03", "jointdai2_upper04" },
    { "jointdai2_07", "jointdai2_lower01" },
    { "jointdai2_lower01", "jointdai2_lower02" },
    { "jointdai2_lower02", "jointdai2_lower03" },
    { "jointdai2_lower03", "jointdai2_lower04" },
    { "joint_00", "jointasi_A01" },
    { "jointasi_A01", "jointasi_A02" },
    { "jointasi_A02", "jointasi_A03" },
    { "jointasi_A03", "jointasi_A04" },
    { "joint_00", "jointasi_B01" },
    { "jointasi_B01", "jointasi_B02" },
    { "jointasi_B02", "jointasi_B03" },
    { "jointasi_B03", "jointasi_B04" },
    { "joint_00", "jointasi_C01" },
    { "jointasi_C01", "jointasi_C02" },
    { "jointasi_C02", "jointasi_C03" },
    { "jointasi_C03", "jointasi_C04" },
    { "joint_00", "jointasi_D01" },
    { "jointasi_D01", "jointasi_D02" },
    { "jointasi_D02", "jointasi_D03" },
    { "jointasi_D03", "jointasi_D04" },
    { "joint_00", "joint_core" },
};

//.bss
extern Mtx g_modeling_mtx_stack[32];
extern AnimationWork work;
#if VERSION <= VERSION_DEMO_US
f32 tanfTbl[0xB2];
#else
extern f32 tanfTbl[0xB2];
#endif

//.sdata
#if VERSION > VERSION_DEMO_US
s32 vivihimoTailGroupNo[4] ATTRIBUTE_ALIGN(32) = { 0, 2, -1, -1 };
#endif
/**
 * Vivian's hair (himo) swing data for each type:
 * entry 0 none, 1-3 the three blocks of _vivihimoData (two floats per animation),
 * filled from a/vivian.bin when the driver starts.
 */
#if VERSION > VERSION_DEMO_US
void* vivihimoData[4] = {
    NULL,
    _vivihimoData[0],
    _vivihimoData[1],
    _vivihimoData[2],
};
#endif
/** Points at the animation driver's work (files, textures, pose pool, scratch buffers). */
#if VERSION <= VERSION_DEMO_US
AnimationWork work;
Mtx g_modeling_mtx_stack[32];
#endif
static AnimationWork* wp = &work;

//.sbss
s32 anim_unused_sbss;
//.sbss
Mtx* g_modeling_mtx;
s32 g_modeling_mtx_lv;

// local declarations
void animPoseRefresh(void);
void animPaperPoseDisp(CameraId camId, void* param);
void animPaperPoseDispSub(s32 unused, AnimationPose* pose);
void evalProc(s32 parentId, s32 groupId);

/** Returns the animation driver's work (files, textures, pose pool, scratch buffers). */
AnimationWork* animGetPtr(void) {
    return wp;
}

/**
 * Current time in milliseconds on the battle render clock (globalTime TRUE) or the field one;
 * pose timing runs on it, so field poses pause while a battle runs.
 */
OSTime animTimeGetTime(BOOL globalTime) {
    if (globalTime == FALSE) {
        return OSTicksToMilliseconds(gp->renderFieldTime);
    }
    return OSTicksToMilliseconds(gp->renderTime);
}

/** Allocates the 1.5 MB per-frame vertex heap on first use and rewinds it. */
void initTestHeap(void) {
    if (!wp->testHeap) { // allocate 1.5MiB test heap
        wp->testHeap = __memAlloc(HEAP_DEFAULT, TESTHEAP_SIZE);
    }
    wp->testAlloc = wp->testHeap;
}

/** heavily inlined */
void* testAlloc(u32 size) {
    void* alloc = wp->testAlloc;

    if (size & 31) {
        size += 32 - (size & 31);
    }
    wp->testAlloc += size;
    if (wp->testAlloc >= (u8*)wp->testHeap + TESTHEAP_SIZE) { // heap full: reset it, reallocate every pose buffer, retry
        animPoseRefresh();
        alloc = testAlloc(size);
    }
    return alloc;
}

/**
 * Boot setup: allocates the pools (64 animation files, 64 texture files, 256 poses,
 * a 1024-vector scratch buffer), the per-frame vertex heap and the tangent table used to bend paper,
 * and loads a/vivian.bin (the hair and tail chain data of Vivian and her sisters)
 * and a/ag2tg.bin (the animation-group to texture-group map).
 */
void animInit(void) {
    int i;
    FileEntry* file;

    wp->totalFiles = 64;
    wp->totalTextures = 64;
    wp->totalPoses = 256;
    wp->floatScratchRP = 1024;
    wp->floatScratchWP = 0;
    wp->files = __memAlloc(HEAP_DEFAULT, sizeof(AnimationPoseFile) * wp->totalFiles);
    wp->textures = __memAlloc(HEAP_DEFAULT, sizeof(AnimationTextureFile) * wp->totalTextures);
    wp->poses = __memAlloc(HEAP_DEFAULT, sizeof(AnimationPose) * wp->totalPoses);
    wp->mbUseFloatScratch = 0;
    wp->floatScratch = __memAlloc(HEAP_DEFAULT, sizeof(Vec) * wp->floatScratchRP);

    wp->texObjs[0] = NULL;
    wp->texObjs[1] = NULL;
    wp->curTexObj = NULL;

    wp->texDatas[0] = NULL;
    wp->texDatas[1] = NULL;
    wp->curTexData = NULL;

    wp->paperMtxPtr[0] = NULL;
    wp->paperMtxPtr[1] = NULL;
    wp->paperMtxPtr[2] = NULL;

    wp->paperTexMode0 = 1;
    wp->paperTexMode1 = 1;

    wp->testHeap = NULL;
    wp->inBattle = FALSE;
    initTestHeap();

    for (i = 0; i < 0xB2; i++) {
        tanfTbl[i] = (f32)tan((i - 0x59) * 0.017453289f);
    }
    file = fileAllocf(FILE_TYPE_RAW, "a/vivian.bin");
    if (file) {
        memcpy_as4(vivihimoData[1], *file->data, sizeof(_vivihimoData));
        memcpy_as4(vivihimoTailGroupNo, (u8*)*file->data + sizeof(_vivihimoData), sizeof(vivihimoTailGroupNo));
        fileFree(file);
    }
    fileSetCurrentArchiveType(0);
    wp->mp_ag2tg = fileAllocf(FILE_TYPE_RAW, "a/ag2tg.bin");
}

/** Once per frame: queues the offscreen pass that renders the paper (effect overlay) poses. */
void animMain(void) {
    dispEntry(CAMERA_OFFSCREEN, 0.0f, 1, animPaperPoseDisp, NULL);
}

/** The animation file data of pose pose. */
inline AnimationPoseData* animPoseGetBaseData(AnimationPose* pose) {
    return *wp->files[pose->fileId].handle->data;
}

// FAKE: macro twin of the animPoseGetBaseData inline above, used by every caller below; calling the inline instead changes the object
#define animPoseGetBaseData(pose) (*(AnimationPoseData**)(wp->files[(pose)->fileId].handle->data))

/**
 * Allocates the pose's vertex buffers.
 * animPoseGetBaseData is animPoseGetAnimBaseDataPtr for a pose pointer;
 * the original inlines that lookup at every use.
 */
void animPose_AllocBuffer(AnimationPose* pose) {
    pose->bufferVtxPos = testAlloc(sizeof(Vec) * animPoseGetBaseData(pose)->bufferPosNum);
    pose->vtxArrayPos = testAlloc(sizeof(Vec) * animPoseGetBaseData(pose)->bufferPosNum);

    pose->bufferVtxNrm = testAlloc(sizeof(Vec) * animPoseGetBaseData(pose)->bufferNrmNum);
    pose->vtxArrayNrm = testAlloc(sizeof(Vec) * animPoseGetBaseData(pose)->bufferNrmNum);

    pose->bufferGroupVisibility = testAlloc(sizeof(s8) * animPoseGetBaseData(pose)->bufferVisibilityNum);
    pose->groupVisibility = pose->bufferGroupVisibility;

    pose->bufferNode = testAlloc(sizeof(*pose->nodes) * animPoseGetBaseData(pose)->bufferNodeNum);
    pose->nodes = testAlloc(sizeof(*pose->nodes) * animPoseGetBaseData(pose)->bufferNodeNum);

    pose->bufferTexAnimEntries = testAlloc(sizeof(AnimTexMtx) * animPoseGetBaseData(pose)->bufferTexAnimNum);
    pose->texAnimEntries = pose->bufferTexAnimEntries;

    pose->lastFrameZero = -1;
}

/** heavily inlined */
void animPoseRefresh(void) {
    int i;

    initTestHeap();
    for (i = 0; i < wp->totalPoses; i++) {
        AnimationPose* pose = &wp->poses[i];
        if (pose->flags) {
            if ((!wp->inBattle || pose->type != 0) && (wp->inBattle || pose->type != 1)) {
                animPose_AllocBuffer(pose);
                pose->lastFrameZero = -1;
            }
        }
    }
}

/** Switches the driver to battle mode and reallocates the vertex buffers of the battle and shared poses. */
void animPoseBattleInit(void) {
    wp->inBattle = TRUE;
    animPoseRefresh();
}

// FAKE: free-file search as an inline: its local p gets a definition-time @ label (numbered above the call-site @ temps), as the target colouring needs; the result goes through q (hit copies p, not-found sets q = NULL before id = -1) so the not-found exit emits `li ptr,0` before `li idx,-1` and the hit keeps its `bne; b` pair
static inline AnimationPoseFile* apeFreeFile(s32* id) {
    AnimationPoseFile* p;
    AnimationPoseFile* q;
    for (id[0] = 0; id[0] < wp->totalFiles; id[0]++) {
        p = &wp->files[id[0]];
        if (!p->active) {
            q = p;
            goto found;
        }
    }
    q = NULL;
    id[0] = -1;
found:
    return q;
}
/**
 * Creates a pose for animation file animName (e.g. "c_kuribo") and returns its id,
 * or -2 if it could not be loaded. type picks the archive and clock: 0 field, 1 battle, 2 shared.
 * Loads the model file and its texture file (sharing ones already loaded),
 * allocates the vertex buffers and resets time, material and turning state;
 * Vivian, Marilyn and Beldam get their hair and tail chain data.
 */
s32 animPoseEntry(const char *animName, s32 type) {
    static s32 _dummy = -1;
    // FAKE: counters and found-slot temps are one-element s32 arrays (IRO scalarizes them into @ window temps, giving the
    // target's `li c,0; mr off,c` loop starts and @ numbering). pc is the pose-loop counter, copied into poseId on a hit
    // (poseId then numbers below pose), and reused as both texture loops' counter (its second web gets a late @ label).
    AnimationPose *t[1];
    AnimationTextureFile *tt[1];
    AnimationPoseFile *file;
    AnimationTextureFile *tex[1];
    AnimationPoseData *data;
    FileEntry *handle;
    s32 poseId[1];
    s32 pc[1];
    s32 fileId[1];
    s32 count;
    s32 gz;
    AnimationPose *pose;
    char textureName[256];
    switch (type) {
        case 0:
            fileSetCurrentArchiveType(1);
            break;

        case 1:
            fileSetCurrentArchiveType(2);
            break;

        case 2:
            fileSetCurrentArchiveType(0);
            break;

    }

    for (pc[0] = 0; pc[0] < wp->totalPoses; pc[0]++) {
        t[0] = &wp->poses[pc[0]];
        if (!t[0]->flags) {
            pose = t[0];
            poseId[0] = pc[0];
            goto poseFound;
        }
    }
    // With no free pose slot, pose stays NULL and is written through below.
    pose = NULL;
    poseId[0] = -1;
poseFound:
    pose->flags |= ANIM_POSE_FLAG_ACTIVE;

    pose->references = pose->typeFlags = fileId[0] = 0;
    count = wp->totalFiles;
    for (; fileId[0] < count; fileId[0]++) {
        file = &wp->files[fileId[0]];
        if (file->active) {
            data = *file->handle->data;
            if (!strcmp(data->fileName, animName)) {
                file->references++;
                goto fileReady;
            }
        }
    }

    file = apeFreeFile(fileId);
    file->active = 1;

    file->references++;
    file->handle = fileAllocf(FILE_TYPE_ANIM, "%s/%s", "a", animName);
    handle = file->handle;
    if (!file->handle) {
        fileId[0] = -2;
        file->active = 0;
        file->references--;
    } else {
        data = *file->handle->data;
        if (!data) {
            fileFree(handle);
            fileId[0] = -2;
            file->active = 0;
            file->references--;
        } else {
            sprintf(textureName, "%s/%s-", "a", data->textureName);
            count = wp->totalTextures;
            for (pc[0] = 0; pc[0] < count; pc[0]++) {
                tex[0] = &wp->textures[pc[0]];
                if (*((u32*)(&tex[0]->active))) {
                    if (tex[0]->handle) {
                        if (!strcmp(tex[0]->handle->filename, textureName)) {
                            // FAKE: dead int guard (COLOURING W), deleted after colouring; its division temps add the texture pointer/count neighbours
                            gz = 0;
                            if (gz != 0) pc[0] = (((s32)(AddrWord)tex[0] / count) / (count / (s32)(AddrWord)tex[0])) / ((pc[0] / count) / (count / pc[0]));
                            tex[0]->references++;
                            goto textureReady;
                        }
                    }
                }
            }

            for (pc[0] = 0; pc[0] < wp->totalTextures; pc[0]++) {
                tt[0] = &wp->textures[pc[0]];
                if (!(*((u32*)(&tt[0]->active)))) {
                    tex[0] = tt[0];
                    goto textureSlotFound;
                }
            }
            tex[0] = NULL;
            pc[0] = -1;
textureSlotFound:
            tex[0]->active = 1;

            tex[0]->references++;
            tex[0]->handle = fileAlloc(textureName, FILE_TYPE_TPL);
textureReady:
            file->textureId = pc[0];
        }
    }
fileReady:
    pose->fileId = fileId[0];

    if (pose->fileId == -2) {
        pose->flags = 0;
        return -2;
    }
    pose->animId = 0;
    pose->localTime = animTimeGetTime(type);
    pose->lastFrameZero = -1;
    pose->lastFrameTime = 0.0f;
    pose->curKeyValue = 0;
    pose->loopTime = 0.0f;
    pose->peraRotY = 0.0f;
    pose->rotationY = 0.0f;
    pose->peraTargetRotY = 0.0f;
    pose->type = type;
    pose->paperId = -1;
    animPose_AllocBuffer(pose);
    pose->gxCallback = NULL;
    pose->disableDraw = 0;
    pose->materialFlag = 0;
    pose->materialLightFlag = 0;
    pose->materialColor = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    pose->materialColor2 = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    pose->paperScaleXZ = (pose->paperScaleY = 1.0f);
    pose->paperOfsY = 0.0f;
    pose->localTimeRate = 1.0f;
    PSMTXIdentity(pose->matrix);
    if (!strcmp(animName, "tst_vivi")) {
        pose->vivianType = 1;
        pose->vivianGroupNo = &vivihimoTailGroupNo[0];
    } else if (!strcmp(animName, "c_vivian")) {
        pose->vivianType = 1;
        pose->vivianGroupNo = &vivihimoTailGroupNo[1];
    } else if (!strcmp(animName, "c_maririn")) {
        pose->vivianType = 2;
        pose->vivianGroupNo = &vivihimoTailGroupNo[2];
    } else if (!strcmp(animName, "c_majyorin")) {
        pose->vivianType = 3;
        pose->vivianGroupNo = &vivihimoTailGroupNo[3];
    } else {
        pose->vivianType = 0;
        pose->vivianGroupNo = &_dummy;
    }
    pose->dispCallback = NULL;
    pose->dispUserDataCallback = NULL;
    return poseId[0];
}

/**
 * Returns the shared paper pose for animName in group, creating it if needed and counting another user;
 * -2 if it could not be loaded.
 */
s32 animPaperPoseEntry(const char* animName, s32 group) {
    s32 i;
    AnimationPose* pose;

    i = animPaperPoseGetId(animName, group);
    if (i >= 0) {
        pose = animPoseGetAnimPosePtr(i);
        pose->references++;
        return i;
    }
    i = animPoseEntry(animName, group);
    if (i == -2) {
        return i;
    }
    pose = animPoseGetAnimPosePtr(i);
    pose->typeFlags = ANIM_POSE_TYPE_SUB | ANIM_POSE_TYPE_PAPER;
    pose->references = 0;
    pose->flags |= ANIM_POSE_FLAG_ACTIVE;
    return i;
}

const char* animGetTexName(const char* name);

/**
 * Starts loading animation file animName and its textures in group's archive in the background;
 * TRUE once both are loaded (or missing), FALSE while still loading.
 */
BOOL animEffectAsync(const char* animName, u32 group) {
    s32 model, anim;
    BOOL ret;

    switch (group) {
        case 0:
            fileSetCurrentArchiveType(1);
            break;
        case 1:
            fileSetCurrentArchiveType(2);
            break;
        case 2:
            fileSetCurrentArchiveType(0);
            break;
    }
    model = fileAsyncf(FILE_TYPE_ANIM, NULL, "%s/%s", "a", animName);
    anim = fileAsyncf(FILE_TYPE_TPL, NULL, "%s/%s-", "a", animGetTexName(animName));
    // FAKE: (u32) casts on the zero tests (unsigned compares); plain model == 0 / anim == 0 change the object
    if (model == -1) {
        ret = TRUE;
    } else if ((u32)model == 0) {
        ret = FALSE;
    } else if (anim == -1) {
        ret = TRUE;
    } else if ((u32)anim == 0) {
        ret = FALSE;
    } else {
        ret = TRUE;
    }
    return ret;
}

/** Turns on pose poseId's paper turning (the flat character flips to face its new direction). */
void animPosePeraOn(s32 poseId) {
    wp->poses[poseId].flags |= ANIM_POSE_FLAG_PERA;
}

/** Turns off pose poseId's paper turning. */
void animPosePeraOff(s32 poseId) {
    wp->poses[poseId].flags &= ~ANIM_POSE_FLAG_PERA;
}

/** Turns on paper turning for pose poseId's paper overlay pose, copying the pose's current turn angles to it. */
void animPosePaperPeraOn(s32 poseId) {
    AnimationPose* dst = &wp->poses[wp->poses[poseId].paperId];
    AnimationPose* src = &wp->poses[poseId];

    dst->flags |= ANIM_POSE_FLAG_PERA;
    dst->peraRotY = src->peraRotY;
    dst->rotationY = src->rotationY;
    dst->peraTargetRotY = src->peraTargetRotY;
}

/** Turns off paper turning for pose poseId's paper overlay pose. */
void animPosePaperPeraOff(s32 poseId) {
    wp->poses[wp->poses[poseId].paperId].flags &= ~ANIM_POSE_FLAG_PERA;
}

/** Sets how fast pose poseId's animation plays (1.0 normal speed). */
void animPoseSetLocalTimeRate(s32 poseId, f32 localTimeRate) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->localTimeRate = localTimeRate;
}

/**
 * Moves pose poseId's animation clock to localTimeFrames frames in (at 60 per second),
 * and its own paper overlay's too.
 */
void animPoseSetLocalTime(s32 poseId, f32 localTimeFrames) {
    AnimationPose *pose, *pera;

    pose = &wp->poses[poseId];
    pose->localTime = animTimeGetTime(pose->type) - (u64)(16.666666f * localTimeFrames);
    pose->localTimeInit = pose->localTime;
    if (pose->paperId != -1) {
        pera = &wp->poses[pose->paperId];
        if (!(pera->typeFlags & ANIM_POSE_TYPE_PAPER)) {
            pera->localTime = pose->localTime;
        }
    }
}

/** Sets the time pose poseId's animation counts from, in animTimeGetTime milliseconds. */
void animPoseSetStartTime(s32 poseId, OSTime startTime) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->localTime = startTime;
}

/* An anim name parameter holds a name (an address in main memory, negative as
 * an s32) or an anim index (non-negative). */
#define ANIM_NAME_INDEX(name) ((s32)(AddrWord)(name))
#define ANIM_INDEX_NAME(idx) ((const char*)(AddrWord)(idx))

/**
 * Looks up an animation in data: by name, or directly by index when name holds a non-negative index.
 * Unknown names and indices give 0.
 */
inline s32 animPoseGetAnimIdx(AnimationPoseData* data, const char* name) {
    s32 i = ANIM_NAME_INDEX(name);
    s32 count = data->animCount;

    if (i < 0) {
        for (i = 0; i < count; i++) {
            if (!strcmp(data->anims[i].name, name)) {
                return i;
            }
        }
    } else if (i < count) {
        return i;
    }
    return 0;
}

/**
 * Switches pose poseId to animation animName (e.g. "S_1", "W_1"), restarting it from the beginning;
 * with reset FALSE nothing happens if it already plays that animation.
 */
void animPoseSetAnim(s32 poseId, const char* animName, BOOL reset) {
    AnimationPose* pose;
    s32 animId;

    pose = &wp->poses[poseId];
    animId = animPoseGetAnimIdx(animPoseGetBaseData(pose), animName);
    if (reset || animId != pose->animId) {
        pose->animId = animId;
        if (pose->paperId != -1 && pose->flags & ANIM_POSE_FLAG_PAPER_GROUP) {
            pose->flags |= ANIM_POSE_FLAG_PAPER_ANIM_CHANGED;
        }
        pose->localTime = animTimeGetTime(pose->type);
        pose->lastFrameZero = -1;
        pose->lastFrameTime = 0.0f;
        pose->curKeyValue = 0;
        pose->loopTime = 0.0f;
    }
}

/** Returns the id of the shared paper pose loaded from animName in group, or -1. */
s32 animPaperPoseGetId(const char* animName, s32 group) {
    AnimationPose* pose;
    s32 i;

    for (i = 0; i < wp->totalPoses; i++) {
        pose = &wp->poses[i];
        if (pose->flags & ANIM_POSE_FLAG_ACTIVE && pose->typeFlags & ANIM_POSE_TYPE_SUB && pose->typeFlags & ANIM_POSE_TYPE_PAPER && pose->type == group &&
            !strcmp(animPoseGetBaseData(pose)->fileName, animName)) {
            break;
        }
    }
    if (i == wp->totalPoses) {
        i = -1;
    }
    return i;
}

/**
 * Attaches the shared paper pose animName (from the pose's group, or the shared group) to pose poseId as its
 * paper overlay; NULL detaches it. someflag turns on caching of the overlay's screen rectangle.
 */
void animPoseSetPaperAnimGroup(s32 poseId, const char* animName, BOOL someflag) {
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);

    if (!animName) {
        pose->paperId = -1;
        animPoseSetMaterialLightFlagOff(poseId, MATERIAL_LIGHT_PAPER);
        return;
    }
    pose->paperId = animPaperPoseGetId(animName, pose->type);
    if (pose->paperId < 0) {
        pose->paperId = animPaperPoseGetId(animName, 2);
    }
    pose->paperAnimIdx = -1;
    pose->localTimeInit = animTimeGetTime(pose->type);
    if (someflag) {
        pose->flags |= ANIM_POSE_FLAG_PAPER_GROUP;
        pose->flags |= ANIM_POSE_FLAG_PAPER_ANIM_CHANGED;
    } else {
        pose->flags &= ~ANIM_POSE_FLAG_PAPER_GROUP;
        pose->flags &= ~ANIM_POSE_FLAG_PAPER_ANIM_CHANGED;
    }
    animPoseSetMaterialLightFlagOn(poseId, MATERIAL_LIGHT_PAPER);
}

/**
 * Plays animation animName of pose poseId's paper overlay, restarting its clock;
 * an overlay of its own (not shared) is evaluated at once.
 */
void animPoseSetPaperAnim(s32 poseId, const char* animName, BOOL reset) {
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);
    AnimationPoseData* data;
    s32 id;
    s32 idx;

    data = animPoseGetAnimBaseDataPtr(pose->paperId);
    idx = animPoseGetAnimIdx(data, animName);
    pose->paperAnimIdx = idx;
    pose->localTimeInit = animTimeGetTime(pose->type);
    id = pose->paperId;
    if (id != -1 && !(*(u32*)((u8*)wp->poses + id * sizeof(AnimationPose) + offsetof(AnimationPose, typeFlags)) & ANIM_POSE_TYPE_PAPER)) { // FAKE: byte-offset load so the typeFlags offset folds into the index (mulli/addi/lwzx), as in the target
        animPoseMain(pose->paperId);
    }
}

void animPoseFileRelease(s32 fileId);

/** Creates a paper overlay pose owned by one pose (not shared), returning its id or -2. */
inline s32 animEffectPoseEntry(const char* animName, s32 group) {
    s32 i;
    AnimationPose* pose;

    i = animPoseEntry(animName, group);
    if (i == -2) {
        return i;
    }
    pose = animPoseGetAnimPosePtr(i);
    pose->typeFlags = ANIM_POSE_TYPE_SUB;
    pose->references = 0;
    pose->flags |= ANIM_POSE_FLAG_ACTIVE;
    return i;
}

/**
 * Gives pose poseId its own paper overlay loaded from animName (e.g. a status or transformation effect),
 * releasing the previous one; NULL removes it.
 * Does nothing if that overlay is already attached with the same someflag (screen rectangle caching).
 */
void animPoseSetEffect(s32 poseId, const char* animName, BOOL someflag) {
    AnimationPose* paper;
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);

    if (pose->paperId != -1) {
        // FAKE: the paper pose's file entry found by byte offset (files + fileId * 16); animPoseGetBaseData(animPoseGetAnimPosePtr(pose->paperId)) changes the object
        if (animName && !strcmp((*(AnimationPoseData**)(((AnimationPoseFile*)((u8*)wp->files + animPoseGetAnimPosePtr(pose->paperId)->fileId * 16))->handle->data))->fileName, animName) &&
            (s32)((pose->flags >> 1) & 1) == someflag) {
            return;
        }
        paper = &wp->poses[pose->paperId];
        paper->references--;
        if (paper->references < 0) {
            animPoseFileRelease(paper->fileId);
            paper->flags = 0;
        }
    }
    if (!animName) {
        pose->paperId = -1;
        animPoseSetMaterialLightFlagOff(poseId, MATERIAL_LIGHT_PAPER);
        return;
    }
    pose->paperId = animEffectPoseEntry(animName, pose->type);
    pose->paperAnimIdx = -1;
    pose->localTimeInit = animTimeGetTime(pose->type);
    if (someflag) {
        pose->flags |= ANIM_POSE_FLAG_PAPER_GROUP;
        pose->flags |= ANIM_POSE_FLAG_PAPER_ANIM_CHANGED;
    } else {
        pose->flags &= ~ANIM_POSE_FLAG_PAPER_GROUP;
    }
    animPoseSetMaterialLightFlagOn(poseId, MATERIAL_LIGHT_PAPER);
}

/** Plays animation animName on pose poseId's paper overlay, if it has one. */
void animPoseSetEffectAnim(s32 poseId, const char* animName, BOOL reset) {
    AnimationPose* pose = &wp->poses[poseId];

    if (pose->paperId != -1) {
        animPoseSetAnim(pose->paperId, animName, reset);
    }
}

/**
 * Sets a GX callback that runs before each of pose poseId's draw stages (to change TEV or blend setup);
 * disableDraw skips the normal drawing.
 */
void animPoseSetGXFunc(s32 poseId, void (*callback)(s32 wXluStage), BOOL disableDraw) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->gxCallback = callback;
    pose->disableDraw = disableDraw;
}

/** Pose poseId's radius from its animation file, in world units. */
f32 animPoseGetRadius(s32 poseId) {
    return 2.0f * animPoseGetAnimBaseDataPtr(poseId)->radius;
}

/** Pose poseId's height from its animation file, in world units. */
f32 animPoseGetHeight(s32 poseId) {
    return 2.0f * animPoseGetAnimBaseDataPtr(poseId)->height;
}

/** How many times pose poseId's current animation has looped (fractional). */
f32 animPoseGetLoopTimes(s32 id) {
    AnimationPose* pose = &wp->poses[id];
    return pose->loopTime;
}

/**
 * Tells whether pose poseId has finished its paper turn (within 10 degrees of the target),
 * TRUE also when turning is off.
 */
BOOL animPoseGetPeraEnd(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];

    if (!(pose->flags & ANIM_POSE_FLAG_PERA)) {
        return TRUE;
    }
    if (__fabs(pose->peraTargetRotY - pose->rotationY) < 10.0) {
        return TRUE;
    }
    return FALSE;
}

/** Sets material flag bits mask on pose poseId (draw modifiers such as the event colour or blending). */
void animPoseSetMaterialFlagOn(s32 poseId, u32 mask) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialFlag |= mask;
}

/** Clears material flag bits mask on pose poseId. */
void animPoseSetMaterialFlagOff(s32 poseId, u32 mask) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialFlag &= ~mask;
}

/** Sets material light flag bits mask on pose poseId (lighting and paper overlay modes). */
void animPoseSetMaterialLightFlagOn(s32 poseId, u32 mask) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialLightFlag |= mask;
}

/** Clears material light flag bits mask on pose poseId. */
void animPoseSetMaterialLightFlagOff(s32 poseId, u32 mask) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialLightFlag &= ~mask;
}

void animPoseSetMaterialEvtColor(s32 poseId, GXColor color) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialColor = color;
}

/** Pose poseId's material flags. */
u32 animPoseGetMaterialFlag(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    return pose->materialFlag;
}

/** Pose poseId's material light flags. */
u32 animPoseGetMaterialLightFlag(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    return pose->materialLightFlag;
}

/** Pose poseId's event tint colour. */
GXColor animPoseGetMaterialEvtColor(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    return pose->materialColor;
}

// FAKE: t3 = t * t2 through an inline so t3 is an inline return temp (numbered above the LICM temp of t2),
// giving the target FPR order (t3 f7, t2 phaseCount). Behaviour-identical.
static inline f32 apmMul(f32 a, f32 b) {
    return a * b;
}
#if VERSION <= VERSION_DEMO_US
/**
 * The demo copies the frame buffers through small inline helpers.
 * Kept: writing the copies out as `if (n != 0) memcpy_as4(...)` changes the demo animPoseMain's registers,
 * and calling the helpers from the later versions' animPoseMain changes theirs.
 */
static inline void apmCopy4(void* src, void* dst, s32 count, s32 size) {
    if (count != 0)
        memcpy_as4(dst, src, count * size);
}
/** Copies count bytes of a frame buffer when there are any (demo animPoseMain helper). */
static inline void apmCopy(void* src, void* dst, s32 count) {
    if (count != 0)
        memcpy(dst, src, count);
}
/**
 * Copies count elements of size bytes from src to dst, 4 bytes at a time,
 * when there are any (demo animPoseMain helper).
 */
static inline void apmCopyTo(void* dst, void* src, s32 count, s32 size) {
    if (count != 0)
        memcpy_as4(dst, src, count * size);
}
#endif
/** the demo converts the s8 deltas with plain casts (no fast-cast setup) and walks the keyframes and deltas by index */
void animPoseMain(s32 poseId) {
    s32 frame;
    s32 nA;
    s32 nB;
    s32 nD;
    s32 nextFrame;
#if VERSION <= VERSION_DEMO_US
    s32 k;
#endif
    AnimKeyframe* kf;
    AnimKeyframe* kfEnd;
    s32 i;
#if VERSION <= VERSION_DEMO_US
    s32 j;
    s32 n;
#endif
    AnimationPoseData* file;
    AnimData* anim;
    AnimationPose* poses;
    u8* grpB;
    u8* grpA;
    u8* q;
    f32 key;
    u8* v;
#if VERSION <= VERSION_DEMO_US
    u8* d;
    AnimTexDelta* td;
#else
    f32 x;
    f32 y;
#endif
    AnimationPose* pose;
    f32 z;
    s32 loop;
    s32 cnt;
    s32 b;
    s32 nC;
    s32 nE;
    f32 t;
    f32 f;
    f32 cur;
    f32 start;
    f32 len;
    f32 tm;
    f32 dt;
    f32 s;
    f32 t2;
    f32 t3;
    u8* p;
    s64 ms;

    poses = wp->poses;
    pose = &poses[poseId];
    file = animPoseGetBaseData(pose);
    anim = file->anims[pose->animId].data;
#if VERSION > VERSION_DEMO_US
    OSInitFastCast();
    // FAKE: (s32) cast on the u32 flag (signed compare); the plain test changes the object
#endif
    if ((s32)wp->mbUseFloatScratch != 0) {
        grpA = (u8*)wp->vtxArrayPos;
        grpB = (u8*)wp->vtxArrayNrm;
    } else {
        grpA = (u8*)pose->vtxArrayPos;
        grpB = (u8*)pose->vtxArrayNrm;
    }
    if (pose->paperId != -1 && !(((AnimationPose*)((u8*)poses + pose->paperId * sizeof(AnimationPose)))->typeFlags & ANIM_POSE_TYPE_PAPER)) { // FAKE: the paper pose found by byte offset (base + id * size) so the typeFlags offset folds into the index (mulli/addi/lwzx), as in the target
        animPoseMain(pose->paperId);
    }
    if (anim == NULL) {
        return;
    }
    loop = anim->loopInfo->loop;
    if (pose->type == 0) {
        ms = OSTicksToMilliseconds(gp->renderFieldTime);
    } else {
        ms = OSTicksToMilliseconds(gp->renderTime);
    }
    tm = (f32)(u64)(ms - pose->localTime);
    pose->rawFrame = 60.0f * tm / 1000.0f;
    pose->rawFrame *= pose->localTimeRate;
    cur = pose->rawFrame;
    f = anim->loopInfo->end;
    start = anim->loopInfo->start;
    len = f - start;
    if (0.0f == len) {
        pose->loopTime = cur - start;
    } else if (loop != 0) {
        tm = cur - start;
        if (tm < 0.0f) {
            pose->loopTime = 0.0f;
        } else {
            pose->loopTime = tm / len;
            start = cur - (f32)(s32)pose->loopTime * len;
        }
    } else if (cur < start) {
        pose->loopTime = 0.0f;
    } else if (cur > f) {
        pose->loopTime = (cur - start) / len;
        start = f;
    } else {
        pose->loopTime = (cur - start) / len;
        start = cur;
    }
    pose->localFrame = start;
    key = pose->localFrame;
    cnt = anim->keyframeCount;
    q = (u8*)anim->keyframes;
    for (i = 0; i < cnt; q += sizeof(AnimKeyframe), i++) {
        if (*(f32*)q > key) {
            break;
        }
    }
    pose->keyframeIdx = i - 1;
    pose->nextKeyframeIdx = pose->keyframeIdx + 1;
    key = pose->localFrame;
    q = (u8*)anim->valueKeys;
    cnt = anim->valueKeyCount;
    for (i = 0; i < cnt; q += sizeof(AnimValueKey), i++) {
        if (*(f32*)q > key) {
            break;
        }
    }
    pose->valueKeyIdx = i - 1;
    pose->nextValueKeyIdx = pose->valueKeyIdx + 1;
    if (loop == 0) {
        if (pose->nextKeyframeIdx >= anim->keyframeCount) {
            pose->nextKeyframeIdx = pose->keyframeIdx;
        }
        if (pose->nextValueKeyIdx >= anim->valueKeyCount) {
            pose->nextValueKeyIdx = pose->valueKeyIdx;
        }
    }
    pose->curKeyValue = anim->valueKeys[pose->valueKeyIdx].value;
    start = anim->keyframes[pose->keyframeIdx].time;
    tm = pose->localFrame - start;
    if (loop != 0) {
        if (pose->nextKeyframeIdx >= anim->keyframeCount) {
            pose->nextKeyframeIdx = 0;
            len = anim->loopInfo->end - anim->keyframes[pose->keyframeIdx].time;
        } else {
            len = anim->keyframes[pose->nextKeyframeIdx].time - start;
        }
    } else {
        len = anim->keyframes[pose->nextKeyframeIdx].time - start;
    }
    if (0.0f == len) {
        pose->keyframeBlend = 0.0f;
    } else {
        pose->keyframeBlend = tm / len;
    }
    frame = pose->keyframeIdx;
    nextFrame = pose->nextKeyframeIdx;
    t = pose->keyframeBlend;
    nA = file->bufferPosNum;
    nB = file->bufferNrmNum;
    nC = file->bufferVisibilityNum;
    nD = file->bufferNodeNum;
    nE = file->bufferTexAnimNum;
    if (pose->lastFrameZero != frame) {
        if (pose->lastFrameZero == -1 || pose->lastFrameZero > frame) {
#if VERSION <= VERSION_DEMO_US
            apmCopy4((u8*)file->bufferVtxPos, (u8*)pose->bufferVtxPos, nA, sizeof(Vec));
            apmCopy4((u8*)file->nrmVtxBuffer, (u8*)pose->bufferVtxNrm, nB, sizeof(Vec));
            apmCopy(file->groupVisibility, pose->bufferGroupVisibility, nC);
            apmCopy4((u8*)file->nodeData, (u8*)pose->bufferNode, nD, 4);
            apmCopy4((u8*)file->texAnimEntries, (u8*)pose->bufferTexAnimEntries, nE, sizeof(AnimTexMtx));
            if (pose->lastFrameZero == -1)
#else
            kfEnd = &anim->keyframes[frame] + 1;
            kf = anim->keyframes;
            if (nA != 0) {
                memcpy_as4((u8*)pose->bufferVtxPos, (u8*)file->bufferVtxPos, nA * sizeof(Vec));
            }
            if (nB != 0) {
                memcpy_as4((u8*)pose->bufferVtxNrm, (u8*)file->nrmVtxBuffer, nB * sizeof(Vec));
            }
            if (nC != 0) {
                memcpy(pose->bufferGroupVisibility, file->groupVisibility, nC);
            }
            if (nD != 0) {
                memcpy_as4((u8*)pose->bufferNode, (u8*)file->nodeData, nD * 4);
            }
            if (nE != 0) {
                memcpy_as4((u8*)pose->bufferTexAnimEntries, (u8*)file->texAnimEntries, nE * sizeof(AnimTexMtx));
            }
            if (pose->lastFrameZero == -1) {
#endif
                pose->lastFrameZero = 0;
#if VERSION > VERSION_DEMO_US
            }
#endif
            s = 0.0625f;
#if VERSION <= VERSION_DEMO_US
            for (k = 0; k <= frame; k++) {
                kf = &anim->keyframes[k];
                {
                    u8* delta;
                    u8* pos;
                    pos = (u8*)pose->bufferVtxPos;
                    delta = anim->vtxPosDeltas + kf->vtxPosStart * 4;
                    n = kf->vtxPosCount;
                    for (j = 0; j < n; j++, delta += 4) {
                        pos += delta[0] * sizeof(Vec);
                        ((Vec*)pos)->x += s * (f32)(s8)delta[1];
                        ((Vec*)pos)->y += s * (f32)(s8)delta[2];
                        ((Vec*)pos)->z += s * (f32)(s8)delta[3];
#else
            for (; kf < kfEnd; kf++) {
                { u8* d; u8* e;
                v = (u8*)pose->bufferVtxPos;
                d = anim->vtxPosDeltas + kf->vtxPosStart * 4;
                e = d + kf->vtxPosCount * 4;
                for (; d < e; d += 4) {
                    v += d[0] * sizeof(Vec);
                    x = __OSs8tof32((s8*)(d + 1));
                    y = __OSs8tof32((s8*)(d + 2));
                    z = __OSs8tof32((s8*)(d + 3));
                    ((Vec*)v)->x += s * x;
                    ((Vec*)v)->y += s * y;
                    ((Vec*)v)->z += s * z;
#endif
                }
                }
#if VERSION <= VERSION_DEMO_US
                {
                    u8* delta;
                    u8* nrm;
                    nrm = (u8*)pose->bufferVtxNrm;
                    delta = anim->vtxNrmDeltas + kf->vtxNrmStart * 4;
                    n = kf->vtxNrmCount;
                    for (j = 0; j < n; j++, delta += 4) {
                        nrm += delta[0] * sizeof(Vec);
                        ((Vec*)nrm)->x += s * (f32)(s8)delta[1];
                        ((Vec*)nrm)->y += s * (f32)(s8)delta[2];
                        ((Vec*)nrm)->z += s * (f32)(s8)delta[3];
#else
                { u8* d; u8* e;
                v = (u8*)pose->bufferVtxNrm;
                d = anim->vtxNrmDeltas + kf->vtxNrmStart * 4;
                e = d + kf->vtxNrmCount * 4;
                for (; d < e; d += 4) {
                    v += d[0] * sizeof(Vec);
                    x = __OSs8tof32((s8*)(d + 1));
                    y = __OSs8tof32((s8*)(d + 2));
                    z = __OSs8tof32((s8*)(d + 3));
                    ((Vec*)v)->x += s * x;
                    ((Vec*)v)->y += s * y;
                    ((Vec*)v)->z += s * z;
#endif
                }
                }
#if VERSION <= VERSION_DEMO_US
                {
                    u8* delta;
                    u8* v;
                    v = (u8*)pose->bufferGroupVisibility;
                    delta = anim->visDeltas + kf->visStart * 2;
                    n = kf->visCount;
                    for (j = 0; j < n; j++, delta += 2) {
                        v += delta[0];
                        v[0] += delta[1];
#else
                { u8* d; u8* e; s8* v;
                v = pose->bufferGroupVisibility;
                d = anim->visDeltas + kf->visStart * 2;
                e = d + kf->visCount * 2;
                for (; d < e; d += 2) {
                    v += d[0];
                    v[0] += d[1];
#endif
                }
                }
#if VERSION <= VERSION_DEMO_US
                {
                    u8* delta;
                    u8* node;
                    node = (u8*)pose->bufferNode;
                    delta = anim->nodeDeltas + kf->nodeStart * 4;
                    n = kf->nodeCount;
                    for (j = 0; j < n; j++, delta += 4) {
                        node += delta[0] * 4;
                        *(f32*)node += s * (f32)(s8)delta[1];
#else
                { u8* d; u8* e;
                v = (u8*)pose->bufferNode;
                d = anim->nodeDeltas + kf->nodeStart * 4;
                e = d + kf->nodeCount * 4;
                for (; d < e; d += 4) {
                    v += d[0] * 4;
                    x = __OSs8tof32((s8*)(d + 1));
                    *(f32*)v += s * x;
#endif
                }
                }
#if VERSION <= VERSION_DEMO_US
                {
                    u8* tex;
                    tex = (u8*)pose->bufferTexAnimEntries;
                    td = &anim->texDeltas[kf->texStart];
                    n = kf->texCount;
                    for (j = 0; j < n; j++, td++) {
                        tex += td->step * sizeof(AnimTexMtx);
                        tex[0] = tex[0] + td->bindAdd;
                        ((AnimTexMtx*)tex)->transS += td->transS;
                        ((AnimTexMtx*)tex)->transT += td->transT;
#else
                { u8* d; u8* e;
                v = (u8*)pose->bufferTexAnimEntries;
                d = (u8*)&anim->texDeltas[kf->texStart];
                e = d + kf->texCount * sizeof(AnimTexDelta);
                for (; d < e; d += sizeof(AnimTexDelta)) {
                    v += d[0] * sizeof(AnimTexMtx);
                    v[0] += d[1];
                    ((AnimTexMtx*)v)->transS += ((AnimTexDelta*)d)->transS;
                    ((AnimTexMtx*)v)->transT += ((AnimTexDelta*)d)->transT;
#endif
                }
                }
            }
        } else if (pose->lastFrameZero < frame) {
            kf = &anim->keyframes[pose->lastFrameZero + 1];
            kfEnd = &anim->keyframes[frame + 1];
            s = 0.0625f;
#if VERSION <= VERSION_DEMO_US
            for (k = pose->lastFrameZero + 1; k <= frame; k++) {
                kf = &anim->keyframes[k];
                {
                    u8* delta;
                    u8* pos;
                    pos = (u8*)pose->bufferVtxPos;
                    delta = anim->vtxPosDeltas + kf->vtxPosStart * 4;
                    n = kf->vtxPosCount;
                    for (j = 0; j < n; j++, delta += 4) {
                        pos += delta[0] * sizeof(Vec);
                        ((Vec*)pos)->x += s * (f32)(s8)delta[1];
                        ((Vec*)pos)->y += s * (f32)(s8)delta[2];
                        ((Vec*)pos)->z += s * (f32)(s8)delta[3];
#else
            for (; kf < kfEnd; kf++) {
                { u8* d; u8* e;
                v = (u8*)pose->bufferVtxPos;
                d = anim->vtxPosDeltas + kf->vtxPosStart * 4;
                e = d + kf->vtxPosCount * 4;
                for (; d < e; d += 4) {
                    v += d[0] * sizeof(Vec);
                    x = __OSs8tof32((s8*)(d + 1));
                    y = __OSs8tof32((s8*)(d + 2));
                    z = __OSs8tof32((s8*)(d + 3));
                    ((Vec*)v)->x += s * x;
                    ((Vec*)v)->y += s * y;
                    ((Vec*)v)->z += s * z;
#endif
                }
                }
#if VERSION <= VERSION_DEMO_US
                {
                    u8* delta;
                    u8* nrm;
                    nrm = (u8*)pose->bufferVtxNrm;
                    delta = anim->vtxNrmDeltas + kf->vtxNrmStart * 4;
                    n = kf->vtxNrmCount;
                    for (j = 0; j < n; j++, delta += 4) {
                        nrm += delta[0] * sizeof(Vec);
                        ((Vec*)nrm)->x += s * (f32)(s8)delta[1];
                        ((Vec*)nrm)->y += s * (f32)(s8)delta[2];
                        ((Vec*)nrm)->z += s * (f32)(s8)delta[3];
#else
                { u8* d; u8* e;
                v = (u8*)pose->bufferVtxNrm;
                d = anim->vtxNrmDeltas + kf->vtxNrmStart * 4;
                e = d + kf->vtxNrmCount * 4;
                for (; d < e; d += 4) {
                    v += d[0] * sizeof(Vec);
                    x = __OSs8tof32((s8*)(d + 1));
                    y = __OSs8tof32((s8*)(d + 2));
                    z = __OSs8tof32((s8*)(d + 3));
                    ((Vec*)v)->x += s * x;
                    ((Vec*)v)->y += s * y;
                    ((Vec*)v)->z += s * z;
#endif
                }
                }
#if VERSION <= VERSION_DEMO_US
                {
                    u8* delta;
                    u8* v;
                    v = (u8*)pose->bufferGroupVisibility;
                    delta = anim->visDeltas + kf->visStart * 2;
                    n = kf->visCount;
                    for (j = 0; j < n; j++, delta += 2) {
                        v += delta[0];
                        v[0] += delta[1];
#else
                { u8* d; u8* e; s8* v;
                v = pose->bufferGroupVisibility;
                d = anim->visDeltas + kf->visStart * 2;
                e = d + kf->visCount * 2;
                for (; d < e; d += 2) {
                    v += d[0];
                    v[0] += d[1];
#endif
                }
                }
#if VERSION <= VERSION_DEMO_US
                {
                    u8* delta;
                    u8* node;
                    node = (u8*)pose->bufferNode;
                    delta = anim->nodeDeltas + kf->nodeStart * 4;
                    n = kf->nodeCount;
                    for (j = 0; j < n; j++, delta += 4) {
                        node += delta[0] * 4;
                        *(f32*)node += s * (f32)(s8)delta[1];
#else
                { u8* d; u8* e;
                v = (u8*)pose->bufferNode;
                d = anim->nodeDeltas + kf->nodeStart * 4;
                e = d + kf->nodeCount * 4;
                for (; d < e; d += 4) {
                    v += d[0] * 4;
                    x = __OSs8tof32((s8*)(d + 1));
                    *(f32*)v += s * x;
#endif
                }
                }
#if VERSION <= VERSION_DEMO_US
                {
                    u8* tex;
                    tex = (u8*)pose->bufferTexAnimEntries;
                    td = &anim->texDeltas[kf->texStart];
                    n = kf->texCount;
                    for (j = 0; j < n; j++, td++) {
                        tex += td->step * sizeof(AnimTexMtx);
                        tex[0] = tex[0] + td->bindAdd;
                        ((AnimTexMtx*)tex)->transS += td->transS;
                        ((AnimTexMtx*)tex)->transT += td->transT;
#else
                { u8* d; u8* e;
                v = (u8*)pose->bufferTexAnimEntries;
                d = (u8*)&anim->texDeltas[kf->texStart];
                e = d + kf->texCount * sizeof(AnimTexDelta);
                for (; d < e; d += sizeof(AnimTexDelta)) {
                    v += d[0] * sizeof(AnimTexMtx);
                    v[0] += d[1];
                    ((AnimTexMtx*)v)->transS += ((AnimTexDelta*)d)->transS;
                    ((AnimTexMtx*)v)->transT += ((AnimTexDelta*)d)->transT;
#endif
                }
                }
            }
        }
    } else {
        if (pose->lastFrameTime > t) {
            pose->lastFrameTime = 0.0f;
        }
    }
    if (frame == nextFrame) {
#if VERSION <= VERSION_DEMO_US
        apmCopyTo(grpA, (u8*)pose->bufferVtxPos, nA, sizeof(Vec));
        apmCopyTo(grpB, (u8*)pose->bufferVtxNrm, nB, sizeof(Vec));
        apmCopyTo((u8*)pose->nodes, (u8*)pose->bufferNode, nD, 4);
#else
        if (nA != 0) {
            memcpy_as4(grpA, (u8*)pose->bufferVtxPos, nA * sizeof(Vec));
        }
        if (nB != 0) {
            memcpy_as4(grpB, (u8*)pose->bufferVtxNrm, nB * sizeof(Vec));
        }
        if (nD != 0) {
            memcpy_as4((u8*)pose->nodes, (u8*)pose->bufferNode, nD * 4);
        }
#endif
    } else if (frame <= nextFrame) {
#if VERSION > VERSION_DEMO_US
        AnimKeyframe* kf;
#endif
        kf = &anim->keyframes[nextFrame];
#if VERSION > VERSION_DEMO_US
        { u8* d; u8* e;
#endif
        s = 0.0625f * t;
        v = grpA;
        d = anim->vtxPosDeltas + kf->vtxPosStart * 4;
#if VERSION <= VERSION_DEMO_US
        v = (u8*)pose->bufferVtxPos;
        n = kf->vtxPosCount;
        apmCopyTo(grpA, v, nA, sizeof(Vec));
        v = grpA;
        for (j = 0; j < n; j++, d += 4) {
#else
        e = d + kf->vtxPosCount * 4;
        p = (u8*)pose->bufferVtxPos;
        if (nA != 0) {
            memcpy_as4(grpA, p, nA * sizeof(Vec));
        }
        for (; d < e; d += 4) {
#endif
            v += d[0] * sizeof(Vec);
#if VERSION <= VERSION_DEMO_US
            ((Vec*)v)->x += (f32)(s8)d[1] * s;
            ((Vec*)v)->y += (f32)(s8)d[2] * s;
            ((Vec*)v)->z += (f32)(s8)d[3] * s;
#else
            x = __OSs8tof32((s8*)(d + 1));
            y = __OSs8tof32((s8*)(d + 2));
            z = __OSs8tof32((s8*)(d + 3));
            ((Vec*)v)->x += x * s;
            ((Vec*)v)->y += y * s;
            ((Vec*)v)->z += z * s;
#endif
        }
#if VERSION > VERSION_DEMO_US
        }
        { u8* d; u8* e; u8* v;
        v = grpB;
#endif
        d = anim->vtxNrmDeltas + kf->vtxNrmStart * 4;
#if VERSION <= VERSION_DEMO_US
        v = (u8*)pose->bufferVtxNrm;
        n = kf->vtxNrmCount;
        apmCopyTo(grpB, v, nB, sizeof(Vec));
        v = grpB;
        for (j = 0; j < n; j++, d += 4) {
#else
        e = d + kf->vtxNrmCount * 4;
        p = (u8*)pose->bufferVtxNrm;
        if (nB != 0) {
            memcpy_as4(grpB, p, nB * sizeof(Vec));
        }
        for (; d < e; d += 4) {
#endif
            v += d[0] * sizeof(Vec);
#if VERSION <= VERSION_DEMO_US
            ((Vec*)v)->x += (f32)(s8)d[1] * s;
            ((Vec*)v)->y += (f32)(s8)d[2] * s;
            ((Vec*)v)->z += (f32)(s8)d[3] * s;
#else
            x = __OSs8tof32((s8*)(d + 1));
            y = __OSs8tof32((s8*)(d + 2));
            z = __OSs8tof32((s8*)(d + 3));
            ((Vec*)v)->x += x * s;
            ((Vec*)v)->y += y * s;
            ((Vec*)v)->z += z * s;
#endif
        }
#if VERSION > VERSION_DEMO_US
        }
        { u8* d; u8* e; u8* v;
#endif
        d = anim->nodeDeltas + kf->nodeStart * 4;
#if VERSION <= VERSION_DEMO_US
        v = (u8*)pose->bufferNode;
#else
        e = d + kf->nodeCount * 4;
#endif
        dt = anim->keyframes[nextFrame].time - anim->keyframes[frame].time;
#if VERSION <= VERSION_DEMO_US
        n = kf->nodeCount;
        apmCopyTo((u8*)pose->nodes, v, nD, 4);
#else
        p = (u8*)pose->bufferNode;
#endif
        v = (u8*)pose->nodes;
#if VERSION <= VERSION_DEMO_US
        for (j = 0; j < n; j++, d += 4) {
#else
        if (nD != 0) {
            memcpy_as4(v, p, nD * 4);
        }
        for (; d < e; d += 4) {
#endif
            b = (s8)d[3];
            v += d[0] * 4;
            if (b < 0x59 && b > -0x59) {
                f32 hb;
#if VERSION > VERSION_DEMO_US
                x = __OSs8tof32((s8*)(d + 1));
#endif
                t2 = t * t;
                t3 = apmMul(t, t2);
                hb = -2.0f * t3 + 3.0f * t2;
                *(f32*)v = *(f32*)v + ((t + (t3 - 2.0f * t2)) * (dt * tanfTbl[(s8)d[2] + 0x59]) +
#if VERSION <= VERSION_DEMO_US
                                hb * (0.0625f * (f32)(s8)d[1]) +
#else
                                hb * (0.0625f * x) +
#endif
                                (t3 - t2) * (dt * tanfTbl[b + 0x59]));
#if VERSION > VERSION_DEMO_US
            }
#endif
        }
        }
    }
    pose->lastFrameZero = frame;
    pose->lastFrameTime = t;
    if (pose->flags & ANIM_POSE_FLAG_PERA) {
        CameraEntry* cam = camGetPtr(CAMERA_3D);
        f = 57.29579f * (f32)atan2(-(cam->cameraAt.x - cam->cameraPos.x), -(cam->cameraAt.z - cam->cameraPos.z));
#if VERSION <= VERSION_DEMO_US
        while (f < -360.0f)
#else
        while (f < -360.0f) {
#endif
            f += 360.0f;
#if VERSION <= VERSION_DEMO_US
        while (f >= 360.0f)
#else
        }
        while (f >= 360.0f) {
#endif
            f -= 360.0f;
#if VERSION <= VERSION_DEMO_US
        if (f < 0.0f)
#else
        }
        if (f < 0.0f) {
#endif
            f += 360.0f;
#if VERSION > VERSION_DEMO_US
        }
#endif
        f -= pose->rotationY;
#if VERSION <= VERSION_DEMO_US
        while (f < -360.0f)
#else
        while (f < -360.0f) {
#endif
            f += 360.0f;
#if VERSION <= VERSION_DEMO_US
        while (f >= 360.0f)
#else
        }
        while (f >= 360.0f) {
#endif
            f -= 360.0f;
#if VERSION > VERSION_DEMO_US
        }
#endif
        if ((f > 90.0f && f <= 270.0f) || (f < -90.0f && f >= -270.0f)) {
            pose->scaleZFlip = -1.0f;
        } else {
            pose->scaleZFlip = 1.0f;
        }
    }
}

#if VERSION > VERSION_DEMO_US
/** the demo always builds every matrix and concatenates them all */
void pushGXModelMtx_TransformNode__(AnimPoseNode* node) {
    Mtx trans, scale, rot, tmp;
    Mtx sc1, sc2, sc3;
    Mtx rc1, rc2, rc3;
    MtxPtr p;
    u32 flags;
    f32 sx, sy, sz;
    f32 rx, ry, rz;
    f32 rcx, rcy, rcz;
    f32 scx, scy, scz;
    f32 rtx, rty, rtz;
    f32 stx, sty, stz;
    f32 tx, ty, tz;

    flags = 0;
    tx = node->translation.x;
    ty = node->translation.y;
    tz = node->translation.z;
    sx = node->mScale.x;
    sy = node->mScale.y;
    sz = node->mScale.z;
    rx = 2.0f * node->rot1.x;
    ry = 2.0f * node->rot1.y;
    rz = 2.0f * node->rot1.z;
    rcx = node->rotationCenter.x;
    rcy = node->rotationCenter.y;
    rcz = node->rotationCenter.z;
    scx = node->scaleCenter.x;
    scy = node->scaleCenter.y;
    scz = node->scaleCenter.z;
    rtx = node->rotationTranslation.x;
    rty = node->rotationTranslation.y;
    rtz = node->rotationTranslation.z;
    stx = node->scaleTranslation.x;
    sty = node->scaleTranslation.y;
    stz = node->scaleTranslation.z;

    if (tx || ty || tz) {
        PSMTXTrans(trans, tx, ty, tz);
        flags |= 1;
    }
    if (sx != 1.0f || sy != 1.0f || sz != 1.0f) {
        PSMTXScale(scale, sx, sy, sz);
        flags |= 2;
    }
    if (scx != 0.0f || scy != 0.0f || scz != 0.0f) {
        if (flags & 2) {
            PSMTXTrans(sc2, -scx, -scy, -scz);
            PSMTXTrans(sc1, scx, scy, scz);
            PSMTXTrans(sc3, stx, sty, stz);
            PSMTXConcat(sc3, sc1, tmp);
            PSMTXConcat(tmp, scale, tmp);
            PSMTXConcat(tmp, sc2, scale);
        } else if (stx || sty || stz) {
            PSMTXTrans(scale, stx, sty, stz);
            flags |= 2;
        }
    }
    PSMTXIdentity(rot);
    if (rz) {
        PSMTXRotRad(tmp, 'z', 0.017453292f * rz);
        PSMTXConcat(rot, tmp, rot);
        flags |= 4;
    }
    if (ry) {
        PSMTXRotRad(tmp, 'y', 0.017453292f * ry);
        PSMTXConcat(rot, tmp, rot);
        flags |= 4;
    }
    if (rx) {
        PSMTXRotRad(tmp, 'x', 0.017453292f * rx);
        PSMTXConcat(rot, tmp, rot);
        flags |= 4;
    }
    if (rcx != 0.0f || rcy != 0.0f || rcz != 0.0f) {
        if (flags & 4) {
            PSMTXTrans(rc2, -rcx, -rcy, -rcz);
            PSMTXTrans(rc1, rcx, rcy, rcz);
            PSMTXTrans(rc3, rtx, rty, rtz);
            PSMTXConcat(rc3, rc1, tmp);
            PSMTXConcat(tmp, rot, rot);
            PSMTXConcat(rot, rc2, rot);
        } else if (rtx || rty || rtz) {
            PSMTXTrans(rot, rtx, rty, rtz);
            flags |= 4;
        }
    }
    g_modeling_mtx_lv++;
    g_modeling_mtx = &g_modeling_mtx_stack[g_modeling_mtx_lv];
    switch (flags) {
        case 0:
            PSMTXIdentity(tmp);
            p = tmp;
            break;
        case 1:
            p = trans;
            break;
        case 2:
            p = scale;
            break;
        case 3:
            PSMTXConcat(trans, scale, tmp);
            p = tmp;
            break;
        case 4:
            p = rot;
            break;
        case 5:
            PSMTXConcat(trans, rot, tmp);
            p = tmp;
            break;
        case 6:
            PSMTXConcat(rot, scale, tmp);
            p = tmp;
            break;
        case 7:
            PSMTXConcat(trans, rot, tmp);
            PSMTXConcat(tmp, scale, trans);
            p = trans;
            break;
    }
    PSMTXConcat(g_modeling_mtx_stack[g_modeling_mtx_lv - 1], p, g_modeling_mtx_stack[g_modeling_mtx_lv]);
}
#endif

#if VERSION > VERSION_DEMO_US
/**
 * Pushes the modelling matrix of a joint node (translation, two rotations and a scale,
 * cancelling the parent's scale): demo version, which always builds and concatenates every matrix.
 */
void pushGXModelMtx_JointNode__(AnimPoseNode* node, f32 sx, f32 sy, f32 sz) {
    Mtx trans, scale, iscale, rot, tmp;
    MtxPtr p;
    u32 flags;
    f32 tx, ty, tz;
    f32 nsx, nsy, nsz;
    f32 r1x, r1y, r1z;
    f32 r2x, r2y, r2z;

    tx = node->translation.x;
    ty = node->translation.y;
    tz = node->translation.z;
    nsx = node->mScale.x;
    nsy = node->mScale.y;
    nsz = node->mScale.z;
    r1x = 2.0f * node->rot1.x;
    r1y = 2.0f * node->rot1.y;
    r1z = 2.0f * node->rot1.z;
    r2x = node->rot2.x;
    r2y = node->rot2.y;
    r2z = node->rot2.z;
    flags = 0;

    if (tx || ty || tz) {
        PSMTXTrans(trans, tx, ty, tz);
        flags |= 1;
    }
    // BUG: the z test is == where x and y test !=, so a parent scale that differs from 1 only in z is not compensated
    if (1.0f != sx || 1.0f != sy || 1.0f == sz) {
        if (sx && sy && sz) {
            PSMTXScale(iscale, 1.0f / sx, 1.0f / sy, 1.0f / sz);
        } else {
            PSMTXScale(iscale, 1.0f, 1.0f, 1.0f);
        }
        flags |= 2;
    }
    // BUG: same z test as above: a node scale that differs from 1 only in z is not applied
    if (1.0f != nsx || 1.0f != nsy || 1.0f == nsz) {
        PSMTXScale(scale, nsx, nsy, nsz);
        flags |= 4;
    }
    PSMTXIdentity(rot);
    if (r2z) {
        PSMTXRotRad(tmp, 'z', 0.017453292f * r2z);
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (r2y) {
        PSMTXRotRad(tmp, 'y', 0.017453292f * r2y);
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (r2x) {
        PSMTXRotRad(tmp, 'x', 0.017453292f * r2x);
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (r1z) {
        PSMTXRotRad(tmp, 'z', 0.017453292f * r1z);
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (r1y) {
        PSMTXRotRad(tmp, 'y', 0.017453292f * r1y);
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    if (r1x) {
        PSMTXRotRad(tmp, 'x', 0.017453292f * r1x);
        PSMTXConcat(rot, tmp, rot);
        flags |= 8;
    }
    g_modeling_mtx_lv++;
    g_modeling_mtx = &g_modeling_mtx_stack[g_modeling_mtx_lv];
    switch (flags) {
        case 0:
            PSMTXIdentity(tmp);
            p = tmp;
            break;
        case 1:
            p = trans;
            break;
        case 2:
            p = iscale;
            break;
        case 3:
            PSMTXConcat(trans, iscale, tmp);
            p = tmp;
            break;
        case 4:
            p = scale;
            break;
        case 5:
            PSMTXConcat(trans, scale, tmp);
            p = tmp;
            break;
        case 6:
            PSMTXConcat(iscale, scale, tmp);
            p = tmp;
            break;
        case 7:
            PSMTXConcat(trans, iscale, tmp);
            PSMTXConcat(tmp, scale, trans);
            p = trans;
            break;
        case 8:
            p = rot;
            break;
        case 9:
            PSMTXConcat(trans, rot, tmp);
            p = tmp;
            break;
        case 10:
            PSMTXConcat(iscale, rot, tmp);
            p = tmp;
            break;
        case 11:
            PSMTXConcat(trans, iscale, tmp);
            PSMTXConcat(tmp, rot, trans);
            p = trans;
            break;
        case 12:
            PSMTXConcat(rot, scale, tmp);
            p = tmp;
            break;
        case 13:
            PSMTXConcat(trans, rot, tmp);
            PSMTXConcat(tmp, scale, trans);
            p = trans;
            break;
        case 14:
            PSMTXConcat(iscale, rot, tmp);
            PSMTXConcat(tmp, scale, trans);
            p = trans;
            break;
        case 15:
            PSMTXConcat(trans, iscale, tmp);
            PSMTXConcat(tmp, rot, trans);
            PSMTXConcat(trans, scale, tmp);
            p = tmp;
            break;
    }
    PSMTXConcat(g_modeling_mtx_stack[g_modeling_mtx_lv - 1], p, g_modeling_mtx_stack[g_modeling_mtx_lv]);
}
#endif

#if VERSION <= VERSION_DEMO_US
/** the demo always builds every matrix and concatenates them all */
static void pushGXModelMtx_TransformNode__(AnimPoseNode* node) {
    Mtx trans, scale, rot, tmp;
    Mtx sc1, sc2, sc3;
    Mtx rc1, rc2, rc3;
#else
/**
 * the demo sets up each texture object, then builds the full texture matrix
 * unless the animation is the identity.
 * The local declaration order is load-bearing: it decides the register colouring (wrapFlags r30, params
 * r23-r29).
 */
void animSetMaterial_Texture(s32 texCount_, s32 *texIdRemap_, AnimPoseTexEntry *pTexEntries_, AnimTexMtx *animEntries, s32 texBindsCapacity, AnimTexBind *texBinds, AnimationTextureFile *texFile) {
    s32 texCount;
    s32* texIdRemap;
    AnimPoseTexEntry* pTexEntries;
    s32 wrapT;
    s32 gz;
    s32 wrapFlags;
    s32 bindId;
    Mtx rot;
    Mtx trans;
    MtxPtr m;
    TEXHeader *hdr;
    u32 flags;
    AnimTexMtx *anim;
    u32 texId;
    s32 texMtx;
    u32 j;
    TEXDescriptor *desc;
    f32 scaleS;
    f32 scaleT;
    GXTexObj obj;
    GXTlutObj tlut;
    f32 transS;
    s32 i[1];
    Mtx scale;
    Mtx tmp;
    s32 wrapS;
    f32 transT;
    f32 rotate;
    u32 mipmap;
#endif
    // FAKE: texCount/texIdRemap/pTexEntries copied into locals (kept as separate webs by the dead guard before loop 2); the incoming
#if VERSION <= VERSION_DEMO_US
    PSMTXTrans(trans, node->translation.x, node->translation.y, node->translation.z);
    PSMTXScale(scale, node->mScale.x, node->mScale.y, node->mScale.z);
    if (0.0f != node->scaleCenter.x || 0.0f != node->scaleCenter.y || 0.0f != node->scaleCenter.z) {
        PSMTXTrans(sc2, -node->scaleCenter.x, -node->scaleCenter.y, -node->scaleCenter.z);
        PSMTXTrans(sc1, node->scaleCenter.x, node->scaleCenter.y, node->scaleCenter.z);
        PSMTXTrans(sc3, node->scaleTranslation.x, node->scaleTranslation.y, node->scaleTranslation.z);
        PSMTXConcat(sc3, sc1, tmp);
        PSMTXConcat(tmp, scale, tmp);
        PSMTXConcat(tmp, sc2, scale);
    }
    PSMTXIdentity(rot);
    if (2.0f * node->rot1.z) {
        PSMTXRotRad(tmp, 'z', 0.017453292f * (2.0f * node->rot1.z));
        PSMTXConcat(rot, tmp, rot);
    }
    if (2.0f * node->rot1.y) {
        PSMTXRotRad(tmp, 'y', 0.017453292f * (2.0f * node->rot1.y));
        PSMTXConcat(rot, tmp, rot);
    }
    if (2.0f * node->rot1.x) {
        PSMTXRotRad(tmp, 'x', 0.017453292f * (2.0f * node->rot1.x));
        PSMTXConcat(rot, tmp, rot);
    }
    if (0.0f != node->rotationCenter.x || 0.0f != node->rotationCenter.y || 0.0f != node->rotationCenter.z) {
        PSMTXTrans(rc2, -node->rotationCenter.x, -node->rotationCenter.y, -node->rotationCenter.z);
        PSMTXTrans(rc1, node->rotationCenter.x, node->rotationCenter.y, node->rotationCenter.z);
        PSMTXTrans(rc3, node->rotationTranslation.x, node->rotationTranslation.y, node->rotationTranslation.z);
        PSMTXConcat(rc3, rc1, tmp);
        PSMTXConcat(tmp, rot, rot);
        PSMTXConcat(rot, rc2, rot);
    }
    g_modeling_mtx_lv++;
    g_modeling_mtx = &g_modeling_mtx_stack[g_modeling_mtx_lv];
    PSMTXConcat(trans, rot, tmp);
    PSMTXConcat(tmp, scale, trans);
    PSMTXConcat(g_modeling_mtx_stack[g_modeling_mtx_lv - 1], trans, g_modeling_mtx_stack[g_modeling_mtx_lv]);
}
/**
 * Pushes the modelling matrix of a joint node (translation, two rotations and a scale,
 * cancelling the parent's scale): demo version, which always builds and concatenates every matrix.
 */
static void pushGXModelMtx_JointNode__(AnimPoseNode* node, f32 sx, f32 sy, f32 sz) {
    Mtx trans, scale, iscale, rot, tmp;
#endif
    // param nodes then neighbour the other params and m but not the counter, which with the counter as a one-element array
#if VERSION <= VERSION_DEMO_US
    PSMTXTrans(trans, node->translation.x, node->translation.y, node->translation.z);
    if (sx && sy && sz) {
        PSMTXScale(iscale, 1.0f / sx, 1.0f / sy, 1.0f / sz);
    } else {
        PSMTXScale(iscale, 1.0f, 1.0f, 1.0f);
    }
    PSMTXScale(scale, node->mScale.x, node->mScale.y, node->mScale.z);
    PSMTXIdentity(rot);
    if (node->rot2.z) {
        PSMTXRotRad(tmp, 'z', 0.017453292f * node->rot2.z);
        PSMTXConcat(rot, tmp, rot);
    }
    if (node->rot2.y) {
        PSMTXRotRad(tmp, 'y', 0.017453292f * node->rot2.y);
        PSMTXConcat(rot, tmp, rot);
    }
    if (node->rot2.x) {
        PSMTXRotRad(tmp, 'x', 0.017453292f * node->rot2.x);
        PSMTXConcat(rot, tmp, rot);
    }
    if (2.0f * node->rot1.z) {
        PSMTXRotRad(tmp, 'z', 0.017453292f * (2.0f * node->rot1.z));
        PSMTXConcat(rot, tmp, rot);
    }
    if (2.0f * node->rot1.y) {
        PSMTXRotRad(tmp, 'y', 0.017453292f * (2.0f * node->rot1.y));
        PSMTXConcat(rot, tmp, rot);
    }
    if (2.0f * node->rot1.x) {
        PSMTXRotRad(tmp, 'x', 0.017453292f * (2.0f * node->rot1.x));
        PSMTXConcat(rot, tmp, rot);
    }
    g_modeling_mtx_lv++;
    g_modeling_mtx = &g_modeling_mtx_stack[g_modeling_mtx_lv];
    PSMTXConcat(trans, iscale, tmp);
    PSMTXConcat(tmp, rot, tmp);
    PSMTXConcat(tmp, scale, trans);
    PSMTXConcat(g_modeling_mtx_stack[g_modeling_mtx_lv - 1], trans, g_modeling_mtx_stack[g_modeling_mtx_lv]);
}
/**
 * the demo sets up each texture object, then builds the full texture matrix
 * unless the animation is the identity.
 * The local declaration order is load-bearing: it decides the register colouring (wrapFlags r30, params
 * r23-r29).
 */
static void animSetMaterial_Texture(s32 texCount, s32* texIdRemap, AnimPoseTexEntry* pTexEntries, AnimTexMtx* animEntries, s32 texBindsCapacity, AnimTexBind* texBinds, AnimationTextureFile* texFile) {
    u32 mipmap;
    u32 texId;
#else
    // (IRO @ temp, numbered above m) makes the counter colour last (r23) and the 7 params r24..r30 in order, as in the target
    texCount = texCount_;
    texIdRemap = texIdRemap_;
    pTexEntries = pTexEntries_;
    for (i[0] = 0; i[0] < texCount; i[0]++) {
        s32 tex2;
        AnimPoseTexEntry *entry;
#endif
        s32 id;
#if VERSION <= VERSION_DEMO_US
        s32 wrapT;
        TEXDescriptor* desc;
        s32 bindId;
        s32 wrapS;
        s32 i;
        GXTlutObj tlut;
        TEXHeader* hdr;
        GXTexObj obj;
        s32 wrapFlags;
        AnimPoseTexEntry* entry;
        s32 tex2;
        for (i = 0; i < texCount; i++) {
            id = texIdRemap[(u32)((texCount - i) - 1)];
#else
        // FAKE: (u32) index cast; texIdRemap[texCount - i[0] - 1] changes the object
        id = texIdRemap[(u32) ((texCount - i[0]) - 1)];
#endif
        entry = &pTexEntries[id];
        bindId = entry->bindId + animEntries[id].texBindIdx;
        if ((bindId < 0) || (bindId >= texBindsCapacity)) {
            bindId = 0;
            printf("Error: dispSetTevMode: \203x\201[\203X\203\202\203f\203\213%s \203A\203j\203\201 %s \223\340\202\305 \203t\203\214\201[\203\200\203G\203N\203X\203e\203\223\203V\203\207\203\223\202\314\220\335\222\350\202\252\215s\202\355\202\352\202\304\202\242\202\334\202\267\202\252\201A\212Y\223\226\202\267\202\351\203e\203N\203X\203`\203\203\202\252\202\240\202\350\202\334\202\271\202\361 \nframeExt\224\324\215\206 %d? \202\252\226\263\202\242\202\346\202\244\202\305\202\267\201B\203\202\203f\203\213\202\334\202\275\202\315\203A\203j\203\201\202\311\217C\220\263\202\252\225K\227v\202\305\202\267\201BtextureIdx=%d\n",
                   animPoseGetBaseData(wp->curPose)->fileName,
                   animPoseGetBaseData(wp->curPose)->anims[wp->curPose->animId].name,
                   animEntries[id].texBindIdx, entry->bindId); // the two %d are frameExt and textureIdx
        }
        wrapFlags = entry->wrapFlags;
        texId = texBinds[bindId].texId;
        if (texBinds[bindId].texType != 0) {
            continue;
        }
        tex2 = texId; // FAKE: s32 copy after the continue: texId loads into r4 for the first TEXGet, then mr r19,r4 as in the target
        if (TEXGet(*((TEXPalette**)texFile->handle->data), texId)->CLUTHeader) {
            desc = TEXGet(*((TEXPalette**)texFile->handle->data), tex2);
            hdr = desc->textureHeader;
            wrapS = hdr->wrapS;
            wrapT = hdr->wrapT;
            if (wrapFlags >= 0) {
                if (wrapFlags & 4) {
                    wrapS = GX_MIRROR;
                } else {
                    // FAKE: wrapS assigned in two steps; wrapS = wrapFlags & 1 changes the object
                    wrapS = wrapFlags;
                    wrapS = wrapS & 1;
                }
                if (wrapFlags & 8) {
                    wrapT = GX_MIRROR;
                } else {
                    wrapT = (wrapFlags >> 1) & 1;
                }
            }
            mipmap = hdr->minLOD != hdr->maxLOD;
            GXInitTlutObj(&tlut, desc->CLUTHeader->data, desc->CLUTHeader->format, desc->CLUTHeader->numEntries);
            GXInitTexObjCI(&obj, desc->textureHeader->data, desc->textureHeader->width, desc->textureHeader->height, (GXCITexFmt) desc->textureHeader->format, wrapS, wrapT, mipmap, 0);
            hdr = desc->textureHeader;
            GXInitTexObjLOD(&obj, hdr->minFilter, hdr->magFilter, hdr->minLOD, hdr->maxLOD, hdr->LODBias, GX_FALSE, hdr->edgeLODEnable, GX_ANISO_1);
            GXLoadTlut(&tlut, 0);
#if VERSION <= VERSION_DEMO_US
            GXLoadTexObj(&obj, (GXTexMapID)i);
#else
            GXLoadTexObj(&obj, (GXTexMapID) i[0]);
#endif
        } else {
            desc = TEXGet(*((TEXPalette**)texFile->handle->data), tex2);
            hdr = desc->textureHeader;
            wrapS = hdr->wrapS;
            wrapT = hdr->wrapT;
            if (wrapFlags >= 0) {
                if (wrapFlags & 4) {
                    wrapS = GX_MIRROR;
                } else {
                    // FAKE: wrapS assigned in two steps; wrapS = wrapFlags & 1 changes the object
#if VERSION <= VERSION_DEMO_US
                    // plain `wrapFlags & 1` the branch becomes a li/bne select, and the two-step form of the CI path keeps wrapFlags at r22
                    wrapS = (wrapFlags & 1) != 0;
#else
                    wrapS = wrapFlags;
                    wrapS = wrapS & 1;
#endif
                }
                if (wrapFlags & 8) {
                    wrapT = GX_MIRROR;
                } else {
                    wrapT = (wrapFlags >> 1) & 1;
                }
            }
            GXInitTexObj(&obj, hdr->data, hdr->width, hdr->height, (GXTexFmt) hdr->format, wrapS, wrapT, hdr->minLOD != hdr->maxLOD);
            hdr = desc->textureHeader;
#if VERSION <= VERSION_DEMO_US
            GXInitTexObjLOD(&obj, hdr->minFilter, hdr->magFilter, hdr->minLOD, hdr->maxLOD, hdr->LODBias, (GXBool)0, hdr->edgeLODEnable, GX_ANISO_1);
            GXLoadTexObj(&obj, (GXTexMapID)i);
#else
            GXInitTexObjLOD(&obj, hdr->minFilter, hdr->magFilter, hdr->minLOD, hdr->maxLOD, hdr->LODBias, GX_FALSE, hdr->edgeLODEnable, GX_ANISO_1);
            GXLoadTexObj(&obj, (GXTexMapID) i[0]);
#endif
        }
    }
#if VERSION <= VERSION_DEMO_US
        {
#endif

#if VERSION <= VERSION_DEMO_US
    // r22); j is a one-element array (an IRO temp, so j colours r20 and texMtx r21), and the dead gz guard keeps k = 0 from
    // being propagated into j, giving the target's li r22,0 / mr r20,r22 (the guard leaves no code after RA)
    s32 gz;
    s32 texMtx;
    f32 scaleS;
    f32 scaleT;
    f32 transS;
    f32 transT;
    Mtx m;
    AnimTexMtx* anim;
    f32 rotate;
    Mtx tmp;
    s32 j[1];
    s32 k;
    k = 0;
#endif
    // FAKE: dead int guard (no code after RA) keeps the three copies from being propagated away
    gz = 0;
    if (gz != 0) {
#if VERSION <= VERSION_DEMO_US
        k = 0;
#else
        texCount = 0;
        texIdRemap = NULL;
        pTexEntries = NULL;
#endif
    }
#if VERSION <= VERSION_DEMO_US
    for (j[0] = k; k < texCount; k++, j[0] += 3) {
        anim = &animEntries[texIdRemap[(u32)((texCount - k) - 1)]];
#else
    for (i[0] = 0; i[0] < texCount; i[0]++) {
        flags = 0;
        // FAKE: (u32) index cast; texIdRemap[texCount - i[0] - 1] changes the object
        anim = &animEntries[texIdRemap[(u32) ((texCount - i[0]) - 1)]];
        rotate = anim->rotate;
#endif
        scaleS = anim->scaleS;
        scaleT = anim->scaleT;
        transS = anim->transS;
        transT = anim->transT;
#if VERSION <= VERSION_DEMO_US
        rotate = anim->rotate;
        if (scaleS == 1.0f && scaleT == 1.0f && transS == 0.0f && transT == 0.0f && rotate == 0.0f) {
            GXSetTexCoordGen2((GXTexCoordID)k, GX_TG_MTX2x4, (GXTexGenSrc)(GX_TG_TEX0 + k), GX_IDENTITY, (GXBool)0, GX_PTIDENTITY);
#else
        if (rotate != 0.0f) {
            flags |= 1;
            PSMTXTrans(tmp, 0.5f, 0.5f, 0.0f);
            PSMTXRotRad(rot, 'z', 0.017453292f * (-rotate));
            PSMTXConcat(tmp, rot, trans);
            PSMTXTrans(tmp, -0.5f, -0.5f, 0.0f);
            PSMTXConcat(trans, tmp, rot);
        }
        if (((transS != 0.0f) || (transT != 0.0f)) || (scaleT != 0.0f)) {
            flags |= 2;
            PSMTXTrans(trans, transS, (1.0f - transT) - scaleT, 0.0f);
        }
        if ((scaleS != 1.0f) || (scaleT != 1.0f)) {
            flags |= 4;
            PSMTXScale(scale, scaleS, scaleT, 1.0f);
        }
        switch (flags) {
            case 0:
                GXSetTexCoordGen2((GXTexCoordID) i[0], GX_TG_MTX2x4, (GXTexGenSrc) (GX_TG_TEX0 + i[0]), GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
#endif
                continue;
#if VERSION > VERSION_DEMO_US

            case 1:
                m = rot;
                break;

            case 2:
                m = trans;
                break;

            case 3:
                PSMTXConcat(rot, trans, tmp);
                m = tmp;
                break;

            case 4:
                m = scale;
                break;

            case 5:
                PSMTXConcat(rot, scale, tmp);
                m = tmp;
                break;

            case 6:
                PSMTXConcat(trans, scale, tmp);
                m = tmp;
                break;

            case 7:
                PSMTXConcat(rot, trans, tmp);
                PSMTXConcat(tmp, scale, trans);
                m = trans;
                break;
#endif
        }
#if VERSION <= VERSION_DEMO_US
        PSMTXIdentity(m);
        PSMTXTrans(tmp, 0.5f, 0.5f, 0.0f);
        PSMTXConcat(m, tmp, m);
        PSMTXRotRad(tmp, 'z', 0.017453292f * (-rotate));
        PSMTXConcat(m, tmp, m);
        PSMTXTrans(tmp, -0.5f, -0.5f, 0.0f);
        PSMTXConcat(m, tmp, m);
        PSMTXTrans(tmp, transS, (1.0f - transT) - scaleT, 0.0f);
        PSMTXConcat(m, tmp, m);
        PSMTXScale(tmp, scaleS, scaleT, 0.0f);
        PSMTXConcat(m, tmp, m);
        texMtx = GX_TEXMTX0 + j[0];
        GXSetTexCoordGen2((GXTexCoordID)k, GX_TG_MTX2x4, (GXTexGenSrc)(GX_TG_TEX0 + k), texMtx, (GXBool)0, GX_PTIDENTITY);
#else

        j = (u32)i[0] * 3; // FAKE: unsigned multiply (i >= 0, same value) shifts loop-2 colouring toward target
        texMtx = GX_TEXMTX0 + j;
        GXSetTexCoordGen2((GXTexCoordID) i[0], GX_TG_MTX2x4, (GXTexGenSrc) (GX_TG_TEX0 + i[0]), texMtx, GX_FALSE, GX_PTIDENTITY);
#endif
        GXLoadTexMtxImm(m, texMtx, GX_MTX2x4);
    }
}
#if VERSION <= VERSION_DEMO_US
}
#endif

/**
 * Replaces the shape's texture of kind texType with texObj (and texture matrix mtx, if given),
 * for paper overlays and other substituted textures; TRUE if the shape uses such a texture.
 */
static BOOL animSetMaterial_ChangeTexture(AnimationPoseData* data, AnimPoseShapeDraw* draw, AnimationPose* pose, GXTexObj* texObj, void** texData, Mtx mtx, s32 texType) {
    AnimTexBind* bind;
    s32 bindCount;
    AnimTexMtx* texAnim;
    AnimPoseTexEntry* texEntry;
    s32 texCount;
    s32 dataId, bindId, frameExt;
    GXTexObj* smartObj;
    int i;

    bind = data->texBinds;
    bindCount = data->texBindCount;
    texAnim = pose->texAnimEntries;
    texEntry = data->texData;
    texCount = draw->texCount;

    for (i = 0; i < texCount; i++) {
        dataId = texCount - i - 1;
        dataId = draw->texDataIds[dataId];
        frameExt = texAnim[dataId].texBindIdx;
        bindId = texEntry[dataId].bindId + texAnim[dataId].texBindIdx;
        if (bindId < 0 || bindId >= bindCount) {
            bindId = 0;
            //"Error: animSetMaterial_ChangeTexture: テクスチャがありません frameExt=%d\n"
            printf("Error: animSetMaterial_ChangeTexture: "
                   "\x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83\x82\xAA\x82\xA0\x82\xE8\x82\xDC\x82\xB9\x82\xF1 "
                   "frameExt=%d\n",
                   frameExt);
        }
        if (bind[bindId].texType == texType) {
            if (texObj) {
                smartObj = smartTexObj(texObj, texData);
                GXLoadTexObj(smartObj, i);
                if (mtx) {
                    GXLoadTexMtxImm(mtx, GX_TEXMTX0 + i * 3, GX_MTX2x4);
                    GXSetTexCoordGen(i, GX_TG_MTX2x4, GX_TG_TEX0 + i, GX_TEXMTX0 + i * 3);
                } else {
                    GXSetTexCoordGen(i, GX_TG_MTX2x4, GX_TG_TEX0 + i, GX_IDENTITY);
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}

#if VERSION > VERSION_DEMO_US
static s32 tbl[4] = { 2, 1, 3, 0 }; // shape cullMode -> GXCullMode: GX_CULL_BACK, GX_CULL_FRONT, GX_CULL_ALL, GX_CULL_NONE
#else
/** Sets GX culling for a shape's cull mode; the demo map has one static local table both draw functions read. */
static inline void animSetCullMode(s32 cullMode) {
    static s32 tbl[4] = { 2, 1, 3, 0 };
    GXSetCullMode(tbl[cullMode]);
}
#endif

/**
 * Sets up the material of shape shapeId for draw stage xluStage:
 * the matrices, lighting, cull mode, alpha test and blending, the tint colours and textures,
 * including substituted paper textures.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Sets up the material of shape shapeId for draw stage xluStage:
 * the matrices, lighting, cull mode, alpha test and blending, the tint colours and textures,
 * including substituted paper textures.
 */
static void materialProc(s32 shapeId, s32 xluStage) {
#else
/**
 * Sets up the material of shape shapeId for draw stage xluStage:
 * the matrices, lighting, cull mode, alpha test and blending, the tint colours and textures,
 * including substituted paper textures.
 */
void materialProc(s32 shapeId, s32 xluStage) {
#endif
    AnimationPoseData *cd;
    int i;
    s32 drawCount;
    AnimPoseShapeDraw *draw;
    CameraEntry *camera;
    AnimationPose *pose;
    GXTexObj *texObj;
    GXTexObj *currTexObj;
    BOOL texChange1;
    BOOL check1;
    s32 dispMode;
    AnimPoseShape *shape;
    Mtx camMtx;
    Mtx invCamMtx;
    Vec lightPos;
#if VERSION <= VERSION_DEMO_US
    if (shapeId != (-1)) {
#else
    if (shapeId != -1) {
        camera = camGetCurPtr();
        PSMTXConcat(camera->view, *g_modeling_mtx, camMtx);
        GXLoadPosMtxImm(camMtx, GX_PNMTX0);
        PSMTXInvXpose(camMtx, invCamMtx);
        GXLoadNrmMtxImm(invCamMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
#endif
        shape = &wp->curPoseData->shapes[shapeId];
        dispMode = shape->dispMode;
        drawCount = shape->shapeDrawCount;
        draw = &wp->curPoseData->draws[shape->shapeDrawStart];
        if ((dispMode == 0 && wp->currDispMode == 2) || dispMode == wp->currDispMode || wp->curPose->materialColor.a != 0xFF || wp->curPose->materialColor2.a != 0xFF) {
#if VERSION <= VERSION_DEMO_US
            animSetCullMode(shape->cullMode);
#else
            GXSetCullMode(tbl[shape->cullMode]);
#endif
            for (i = 0; i < drawCount; i++, draw++) {
                s32 texCount = draw->texCount;
                // FAKE: (u32) cast on the BOOL (unsigned compare); the plain test changes the object
                if ((u32)wp->curPose->disableDraw == 0) {
                    lightPos = (Vec){0.0f, 0.0f, 0.0f};
                    PSMTXMultVec(*g_modeling_mtx, &lightPos, &lightPos);
                    mapSetMaterialLight(wp->curPose->materialLightFlag, lightPos);
                    animSetMaterial_Texture(draw->texCount, draw->texDataIds, wp->curPoseData->texData, wp->curPose->texAnimEntries, wp->curPoseData->texBindCount, wp->curPoseData->texBinds, &wp->textures[wp->files[wp->curPose->fileId].textureId]);
                    mapSetMaterialLastStageBlend(wp->curPose->materialFlag, wp->curPose->materialColor, wp->curPose->materialColor2);
                    camera = camGetPtr(CAMERA_3D);
                    if (camGetCurPtr() == camera) {
                        mapSetMaterialFog();
                    }
                    mapSetMaterialTev(texCount, draw->drawMode, wp->curPose->materialFlag, *g_modeling_mtx);
                }
                pose = wp->curPose;
                // FAKE: (u32) cast on typeFlags (unsigned compare); the plain test changes the object
                if ((u32)pose->typeFlags != 0) {
                    cd = wp->curPoseData;
                    texChange1 = animSetMaterial_ChangeTexture(cd, draw, pose, wp->texObjs[0], wp->texDatas[0], *wp->paperMtxPtr[0], 1);
                    check1 = texChange1;
                    if (!texChange1) {
                        cd = wp->curPoseData;
                        check1 = animSetMaterial_ChangeTexture(cd, draw, wp->curPose, wp->texObjs[1], wp->texDatas[1], *wp->paperMtxPtr[1], 2);
                    }
                    if (check1 && (!draw->drawMode)) {
                        currTexObj = wp->curTexObj;
                        if (currTexObj) {
                            smartTexObj(currTexObj, wp->curTexData);
                            if (wp->currDispMode != 3) {
                                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                                GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                                GXSetZCompLoc(GX_FALSE);
                                GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
                            }
                            texObj = wp->texObjs[texChange1 == 0];
                            if (texObj) {
                                GXLoadTexObj(texObj, GX_TEXMAP0);
                                GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
                                GXLoadTexMtxImm(*wp->paperMtxPtr[!texChange1], GX_TEXMTX0, GX_MTX2x4);
                            }
                            GXLoadTexObj(currTexObj, GX_TEXMAP1);
                            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
                            GXLoadTexMtxImm(*wp->paperMtxPtr[2], GX_TEXMTX1, GX_MTX2x4);
                            mapSetMaterialTev(2, 12, wp->curPose->materialFlag, *g_modeling_mtx);
                        }
                    }
                }
                { // FAKE: a separate pose2 local for the third ChangeTexture call; reusing pose changes the object
                AnimationPose *pose2;
                pose2 = wp->curPose;
                if (pose2->flags & ANIM_POSE_FLAG_CHANGE_TEXTURE) {
                    texObj = wp->texObjs[0];
                    if (texObj) {
                        if (wp->paperMtxPtr[0]) {
                            cd = wp->curPoseData;
                            animSetMaterial_ChangeTexture(cd, draw, pose2, texObj, wp->texDatas[0], *wp->paperMtxPtr[0], 1);
                        }
                    }
                }
                }
            }
        }
    }
}

// FAKE: renderProc's per-draw pose-data read as an inline; its call-site temp gives the target wp/data register order
static inline AnimationPoseData* rpCurData(void) {
    return wp->curPoseData;
}
/**
 * Draws shape shapeId of the current pose: points GX at the pose's evaluated vertex positions and normals
 * and the file's colours and texture coordinates, then sends its polygons.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Draws shape shapeId of the current pose: points GX at the pose's evaluated vertex positions and normals
 * and the file's colours and texture coordinates, then sends its polygons.
 */
static void renderProc(s32 shapeId, s32 unused) {
#else
/**
 * Draws shape shapeId of the current pose: points GX at the pose's evaluated vertex positions and normals
 * and the file's colours and texture coordinates, then sends its polygons.
 */
void renderProc(s32 shapeId, s32 unused) {
#endif
    AnimationPoseData* cd;
    s32 texIndex;
    int i;
    AnimationPose* pose;
    AnimPoseShape *shapes, *shape;
    AnimPoseShapeDraw *draws, *draw;
    s32 dispMode, drawId;
    s32 posFirstVtx, nrmFirstVtx, clr0FirstVtx;

    BOOL texChange1, check1, texChange2, check2;
    Vec *posVtxPtr, *nrmVtxPtr;

    int j, k, l, k2, jj;

    // stack variables
    s32 clr0VtxSize, posVtxSize, nrmVtxSize;
    s32 drawCount;
    Mtx camMtx, invCamMtx;
    Vec lightPos, lightPos2;

    if (shapeId != -1) {
        GXColor* clr0VtxPtr;
        u16 count;
        AnimationPoseData* data;
        GXTexObj* texObj;
        CameraEntry* camera;
        s32 index;
#if VERSION <= VERSION_DEMO_US
        Mtx* model = g_modeling_mtx;
#endif
        camera = camGetCurPtr();
#if VERSION <= VERSION_DEMO_US
        MTXConcat(camera->view, *model, camMtx);
#else
        MTXConcat(camera->view, *g_modeling_mtx, camMtx);
#endif
        GXLoadPosMtxImm(camMtx, GX_PNMTX0);
#if VERSION <= VERSION_DEMO_US
        MTXInverse(camMtx, invCamMtx);
        MTXTranspose(invCamMtx, camMtx);
        GXLoadNrmMtxImm(camMtx, GX_PNMTX0);
#else
        MTXInvXpose(camMtx, invCamMtx);
        GXLoadNrmMtxImm(invCamMtx, GX_PNMTX0);
#endif
        GXSetCurrentMtx(GX_PNMTX0);

        data = wp->curPoseData;
        shapes = data->shapes;
        draws = data->draws;
        shape = &shapes[shapeId];
        dispMode = shape->dispMode;
        drawId = shape->shapeDrawStart;
        posFirstVtx = shape->vtxPos.first;
        nrmFirstVtx = shape->vtxNrm.first;
        clr0FirstVtx = shape->vtxClr0.first;
        drawCount = shape->shapeDrawCount;
        draw = &draws[drawId];

        if ((dispMode == 0 && wp->currDispMode == 2) || dispMode == wp->currDispMode ||
            wp->curPose->materialColor.a != 0xFF || wp->curPose->materialColor2.a != 0xFF) {
            f32* buffer;
            s32 firstDrawId;
            f32* texVtxIndices[8];
#if VERSION <= VERSION_DEMO_US
            animSetCullMode(shape->cullMode);
#else

            GXSetCullMode(tbl[shape->cullMode]);
#endif
            data = wp->curPoseData;
            clr0VtxSize = sizeof(GXColor) * clr0FirstVtx;
            buffer = data->texVtxBuffer;
            texVtxIndices[0] = &buffer[2 * shape->vtxTex[0].first];
            texVtxIndices[1] = &buffer[2 * shape->vtxTex[1].first];
            texVtxIndices[2] = &buffer[2 * shape->vtxTex[2].first];
            texVtxIndices[3] = &buffer[2 * shape->vtxTex[3].first];
            texVtxIndices[4] = &buffer[2 * shape->vtxTex[4].first];
            texVtxIndices[5] = &buffer[2 * shape->vtxTex[5].first];
            texVtxIndices[6] = &buffer[2 * shape->vtxTex[6].first];
            texVtxIndices[7] = &buffer[2 * shape->vtxTex[7].first];
            posVtxSize = sizeof(Vec) * posFirstVtx;
            nrmVtxSize = sizeof(Vec) * nrmFirstVtx;

            for (i = 0; i < drawCount; i++, draw++) {
                int tc2;
                s32 texCount;
                s8* texIds;
                s32 drawCallCount;
                s32 posFirstId;
                s32 nrmFirstId;
                s32 clr0FirstId;
                s32* firstId;
                AnimDrawCallEntry* drawCall;
                texCount = draw->texCount;
                // FAKE: (u32) cast on the BOOL (unsigned compare); the plain test changes the object
                if ((u32)wp->curPose->disableDraw == 0) {
                    lightPos = (Vec){ 0.0f, 0.0f, 0.0f };
                    MTXMultVec(*g_modeling_mtx, &lightPos, &lightPos);
                    lightPos2 = lightPos;
                    mapSetMaterialLight(wp->curPose->materialLightFlag, lightPos2);
                    animSetMaterial_Texture(draw->texCount, draw->texDataIds, wp->curPoseData->texData,
                                            wp->curPose->texAnimEntries, wp->curPoseData->texBindCount,
                                            wp->curPoseData->texBinds,
                                            &wp->textures[wp->files[wp->curPose->fileId].textureId]);
                    mapSetMaterialLastStageBlend(wp->curPose->materialFlag, wp->curPose->materialColor,
                                                 wp->curPose->materialColor2);
                    camera = camGetPtr(CAMERA_3D);
                    if (camGetCurPtr() == camera) {
                        mapSetMaterialFog();
                    }
                    mapSetMaterialTev(texCount, draw->drawMode, wp->curPose->materialFlag, *g_modeling_mtx);
                }
                pose = wp->curPose;
                // FAKE: (u32) cast on typeFlags (unsigned compare); the plain test changes the object
                if ((u32)pose->typeFlags != 0) {
                    GXTexObj* currTexObj;
                    cd = wp->curPoseData;
                    texChange1 = animSetMaterial_ChangeTexture(cd, draw, pose, wp->texObjs[0], wp->texDatas[0], *wp->paperMtxPtr[0], 1);
                    check1 = texChange1;
                    if (!texChange1) {
                        cd = wp->curPoseData;
                        pose = wp->curPose;
                        texChange2 = animSetMaterial_ChangeTexture(cd, draw, pose, wp->texObjs[1], wp->texDatas[1], *wp->paperMtxPtr[1], 2);
                        check1 = texChange2;
                        check2 = texChange2;
                    } else {
                        check2 = FALSE;
                    }
                    // FAKE: (s32) casts on the u32 paperTexMode fields (signed compares); the plain tests change the object
                    if (check1 && (texChange1 && (s32)wp->paperTexMode0 == 0 || check2 && (s32)wp->paperTexMode1 == 0) && !draw->drawMode) {
                        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
                    }
                    if (check1 && !draw->drawMode) {
                        currTexObj = wp->curTexObj;
                        if (currTexObj) {
                            smartTexObj(currTexObj, wp->curTexData);
                            if (wp->currDispMode != 3) {
                                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                                GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                                GXSetZCompLoc(GX_FALSE);
                                GXSetAlphaCompare(GX_GEQUAL, 0x80u, GX_AOP_OR, GX_NEVER, 0);
                            }
                            texObj = wp->texObjs[texChange1 == 0];
                            if (texObj) {
                                GXLoadTexObj(texObj, GX_TEXMAP0);
                                GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE,
                                                  GX_PTIDENTITY);
                                GXLoadTexMtxImm(*wp->paperMtxPtr[!texChange1], GX_TEXMTX0, GX_MTX2x4);
                            }
                            GXLoadTexObj(currTexObj, GX_TEXMAP1);
                            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE,
                                              GX_PTIDENTITY);
                            GXLoadTexMtxImm(*wp->paperMtxPtr[2], GX_TEXMTX1, GX_MTX2x4);
                            mapSetMaterialTev(2, 12, wp->curPose->materialFlag, *g_modeling_mtx);
                        }
                    }
                }

                { // FAKE: a separate pose2 local for the third ChangeTexture call; reusing pose changes the object
                AnimationPose* pose2;
                pose2 = wp->curPose;
                if (pose2->flags & ANIM_POSE_FLAG_CHANGE_TEXTURE) {
                    texObj = wp->texObjs[0];
                    if (texObj) {
                        buffer = (f32*)wp->paperMtxPtr[0]; // FAKE: the paper matrix pointer held in buffer (an earlier web) so it is a split @ web, not a named mtx local
                        if (buffer) {
                            cd = wp->curPoseData;
                            animSetMaterial_ChangeTexture(cd, draw, pose2, texObj, wp->texDatas[0], *(Mtx*)buffer, 1);
                        }
                    }
                }
                }

                texIds = (s8*)draw->texArrayIds;
                firstId = draw->firstIdTex;
                data = rpCurData();
                drawCall = &data->drawCalls[draw->firstDrawCall];
                tc2 = draw->texCount;
                drawCallCount = draw->drawCallCount;
                clr0VtxPtr = (GXColor*)((u8*)data->clr0VtxBuffer + clr0VtxSize);
                posFirstId = draw->firstIdPos;
                nrmFirstId = draw->firstIdNrm;
                clr0FirstId = draw->firstIdClr0;
                // FAKE: (s32) cast on the u32 flag (signed compare); the plain test changes the object
                if ((s32)wp->mbUseFloatScratch != 0) {
                    posVtxPtr = (Vec*)((u8*)wp->vtxArrayPos + posVtxSize);
                    nrmVtxPtr = (Vec*)((u8*)wp->vtxArrayNrm + nrmVtxSize);
                } else {
                    pose = wp->curPose;
                    posVtxPtr = (Vec*)((u8*)pose->vtxArrayPos + posVtxSize);
                    nrmVtxPtr = (Vec*)((u8*)pose->vtxArrayNrm + nrmVtxSize);
                }
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
                GXSetArray(GX_VA_POS, posVtxPtr, sizeof(Vec));
                GXSetVtxDesc(GX_VA_NRM, GX_INDEX16);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
                GXSetArray(GX_VA_NRM, nrmVtxPtr, sizeof(Vec));
                GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
                GXSetArray(GX_VA_CLR0, clr0VtxPtr, sizeof(GXColor));

                if (tc2 == 1) { // one texture: its coordinates come from texIds[0]
                    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
                    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
                    GXSetArray(GX_VA_TEX0, texVtxIndices[texIds[0]], sizeof(f32) * 2);
                    for (j = 0; j < drawCallCount; j++, drawCall++) {
                        u16 *posIndexPtr, *nrmIndexPtr, *clr0IndexPtr, *texIndexPtr;
                        firstDrawId = drawCall->first;
                        data = wp->curPoseData;
                        count = (u16)drawCall->count;
                        posIndexPtr = (u16*)(data->posIndex + (firstDrawId + posFirstId)) + 1;
                        nrmIndexPtr = (u16*)(data->nrmIndex + (firstDrawId + nrmFirstId)) + 1;
                        clr0IndexPtr = (u16*)(data->clr0Index + (firstDrawId + clr0FirstId)) + 1;
                        texIndexPtr = (u16*)(data->texIndex[texIds[0]] + (firstDrawId + firstId[texIds[0]])) + 1;
                        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, count);
                        for (k = count; k > 0; k--) {
                            GXPosition1x16(*posIndexPtr);
                            posIndexPtr += 2;
                            GXNormal1x16(*nrmIndexPtr);
                            nrmIndexPtr += 2;
                            GXColor1x16(*clr0IndexPtr);
                            clr0IndexPtr += 2;
                            GXTexCoord1x16(*texIndexPtr);
                            texIndexPtr += 2;
                        }
                    }
                } else { // several textures: one coordinate index per texture per vertex
                    for (jj = 0; jj < tc2; jj++) {
                        GXSetVtxDesc(GX_VA_TEX0 + jj, GX_INDEX16);
                        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0 + jj, GX_TEX_ST, GX_F32, 0);
                        GXSetArray(GX_VA_TEX0 + jj, texVtxIndices[texIds[tc2 - jj - 1]], sizeof(f32) * 2);
                    }
                    for (jj = 0; jj < drawCallCount; jj++, drawCall++) {
                        s32 fd2;
                        u16 cnt2;
                        u16 *posIndexPtr, *nrmIndexPtr, *clr0IndexPtr;
                        fd2 = drawCall->first;
                        data = wp->curPoseData;
                        cnt2 = (u16)drawCall->count;
                        posIndexPtr = (u16*)(data->posIndex + (fd2 + posFirstId)) + 1;
                        nrmIndexPtr = (u16*)(data->nrmIndex + (fd2 + nrmFirstId)) + 1;
                        clr0IndexPtr = (u16*)(data->clr0Index + (fd2 + clr0FirstId)) + 1;
                        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, cnt2);
                        for (k2 = 0; k2 < cnt2; k2++, posIndexPtr += 2, nrmIndexPtr += 2, clr0IndexPtr += 2) {
                            GXPosition1x16(*posIndexPtr);
                            GXNormal1x16(*nrmIndexPtr);
                            GXColor1x16(*clr0IndexPtr);
                            for (l = tc2, index = 0; l > 0; l--, index++) {
                                // FAKE: (u32) index cast; texIds[tc2 - index - 1] changes the object
                                texIndex = texIds[(u32)(tc2 - index - 1)];
                                GXTexCoord1x16((&wp->curPoseData->texIndex[texIndex][fd2 + firstId[texIndex]])[k2]);
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * Walks the pose's group tree from groupId (children, then siblings) and draws each visible group:
 * pushes its node matrix, sets up its material and draws its shape.
 * Vivian, Marilyn and Beldam's hair and tail chains are bent towards their movement on the way.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Walks the pose's group tree from groupId (children, then siblings) and draws each visible group:
 * pushes its node matrix, sets up its material and draws its shape.
 * Vivian, Marilyn and Beldam's hair and tail chains are bent towards their movement on the way.
 */
static void dispProc(s32 parentId, s32 groupId, s32 xluStage, s32 nextKeyframe, f32 blend) {
#else
/**
 * Walks the pose's group tree from groupId (children, then siblings) and draws each visible group:
 * pushes its node matrix, sets up its material and draws its shape.
 * Vivian, Marilyn and Beldam's hair and tail chains are bent towards their movement on the way.
 */
void dispProc(s32 parentId, s32 groupId, s32 xluStage, s32 nextKeyframe, f32 blend) {
#endif
    AnimPoseGroup* group;
    AnimPoseGroup* parent;
    AnimPoseNode* node;
    AnimationPose* pose;
    AnimationPoseData* data;
    f32 sx, sy, sz;

    if (groupId == -1) {
        return;
    }
    pose = wp->curPose;
    data = wp->curPoseData;
    group = &data->groups[groupId];
    if (pose->groupVisibility[group->visibilityId]) {
        pose = wp->curPose;
        node = (AnimPoseNode*)(pose->nodes + group->nodeId);
        if (group->useSSC) {
            if (parentId == -1) {
                sx = sy = sz = 1.0f;
            } else {
                parent = &data->groups[parentId];
                if (!parent->useSSC) {
                    sx = sy = sz = 1.0f;
                } else {
                    AnimPoseNode* pnode = (AnimPoseNode*)(pose->nodes + parent->nodeId);
                    sx = pnode->mScale.x;
                    sy = pnode->mScale.y;
                    sz = pnode->mScale.z;
                }
            }
            pushGXModelMtx_JointNode__(node, sx, sy, sz);
        } else {
            pushGXModelMtx_TransformNode__(node);
        }
        if (wp->curPose->flags & ANIM_POSE_FLAG_WORLD_POSITION_EVAL) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = (*g_modeling_mtx)[0][3];
            pos.y = (*g_modeling_mtx)[1][3];
            pos.z = (*g_modeling_mtx)[2][3];
            ((Vec*)wp->curPose->worldPositions->address)[groupId] = pos;
        }
        if (wp->curPose->flags & ANIM_POSE_FLAG_WORLD_MATRIX_EVAL) {
            PSMTXCopy(*g_modeling_mtx, ((Mtx*)wp->curPose->worldMatrices->address)[groupId]);
        }
        pose = wp->curPose;
        if (pose->flags & ANIM_POSE_FLAG_VIVIAN && groupId == pose->vivianGroupNo[0]) {
            Mtx* m;
            MtxPtr mat;
            f32* speed;
            f32 offA, offB;
            int i, j;
            s32 type;

            offB = offA = 0.0f;
            type = pose->vivianType;
            speed = (f32*)vivihimoData[type] + pose->animId * 2;
            m = g_modeling_mtx;
            mat = pose->matrix;
            switch (type) {
                case 1:
                    offA = -2.0f;
#if VERSION <= VERSION_DEMO_US
                    offB = 2.0f;
#else
                    offB = 0.5f;
#endif
                    break;
                case 2:
                    offA = -3.0f;
#if VERSION <= VERSION_DEMO_US
                    offB = 1.0f;
#else
                    offB = 0.5f;
#endif
                    break;
                case 3:
                    offA = -1.0f;
#if VERSION <= VERSION_DEMO_US
                    offB = 1.0f;
#else
                    offB = 0.5f;
#endif
                    break;
            }
            materialProc(group->shapeId, xluStage);
            vivihimo(*m, mat, speed[0], speed[1], offA, offB);
            wp->curPose->flags &= ~ANIM_POSE_FLAG_VIVIAN;
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    mat[i][j] += 0.04f * ((*m)[i][j] - mat[i][j]);
                }
            }
        } else {
            if (pose->dispCallback) {
                pose->dispCallback(pose->dispUserDataCallback, groupId, g_modeling_mtx);
            }
            renderProc(group->shapeId, xluStage);
            dispProc(groupId, group->childGroupId, xluStage, nextKeyframe, blend);
        }
        g_modeling_mtx_lv--;
        g_modeling_mtx = &g_modeling_mtx_stack[g_modeling_mtx_lv];
    }
    dispProc(groupId, group->brotherGroupId, xluStage, nextKeyframe, blend);
}

/** Draws pose poseId at x, y, z turned by rotY degrees and scaled, for draw stage xluMode. */
void animPoseDraw(s32 poseId, f32 x, f32 y, f32 z, f32 rotY, f32 scale, s32 xluMode) {
    Mtx matrix;

    MTXTrans(matrix, x, y, z);
    animPoseDrawMtx(poseId, matrix, xluMode, rotY, scale);
}

// _animPoseDrawMtx
void animPoseMainSub(AnimationPose* pose);

/** Wraps an angle in degrees into 0..360. */
static inline f32 _animClampAngle(f32 a) {
    while (a < -360.0f) {
        a += 360.0f;
    }
    while (a >= 360.0f) {
        a -= 360.0f;
    }
    if (a < 0.0f) {
        a += 360.0f;
    }
    return a;
}

/** Wraps an angle in degrees into -360..360. */
static inline f32 _animClampAngle2(f32 a) {
    while (a < -360.0f) {
        a += 360.0f;
    }
    while (a >= 360.0f) {
        a -= 360.0f;
    }
    return a;
}

/** The 3D camera's yaw in degrees (0..360), from the direction it looks along. */
static inline f32 _animCamAngle(void) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    return _animClampAngle(57.29579f * (f32)atan2(-(cam->cameraAt.x - cam->cameraPos.x), -(cam->cameraAt.z - cam->cameraPos.z)));
}

/**
 * Draws a pose with matrix for draw stage xluMode (skipped when the animation file has nothing for that stage,
 * or for a field pose in battle and a battle pose on the field):
 * applies the event colour and GX callback, turns a paper-turning pose smoothly to face the camera (from behind
 * when seen from the back), scales it, evaluates the animation if needed and draws its group tree.
 */
void _animPoseDrawMtx(AnimationPose* pose, Mtx matrix, s32 xluMode, f32 rotY, f32 scale) {
    Mtx result;
    Mtx tmp2;
    Mtx tmp1;
    Mtx tmp3;
    GXColor col1;
    GXColor col2;
    AnimationPoseData* data;
    AnimData* anim;
    s32 keyframeIdx, nextKeyframeIdx;
    f32 keyframeBlend;
    f32 diff;
    s32 mode;

    data = animPoseGetBaseData(pose);
    keyframeIdx = pose->keyframeIdx;
    nextKeyframeIdx = pose->nextKeyframeIdx;
    keyframeBlend = pose->keyframeBlend;
    anim = data->anims[pose->animId].data;
    if (anim == NULL) {
        return;
    }
    if (wp->inBattle && pose->type == 0) {
        return;
    }
    if (!wp->inBattle && pose->type == 1) {
        return;
    }
    if (!(xluMode == 2 && (data->flags & 1)) && !(data->flags & (1 << xluMode))) {
        return;
    }
    mode = xluMode;
    col2 = pose->materialColor2;
    col1 = pose->materialColor;
    if (mapTestXLU(pose->materialFlag, &col1, &col2)) {
        mode = 3;
    }
    if (pose->gxCallback != NULL) {
        pose->gxCallback(mode);
    } else {
        switch (mode) {
            case 1:
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                GXSetZCompLoc(GX_TRUE);
                GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                break;
            case 0:
            case 2:
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                GXSetZCompLoc(GX_FALSE);
                GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
                break;
            case 3:
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                GXSetZCompLoc(GX_TRUE);
                GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                break;
        }
        wp->currDispMode = mode;
    }
    if (pose->flags & ANIM_POSE_FLAG_PERA) {
        rotY += -90.0f;
        pose->peraRotY = rotY;
        diff = _animClampAngle2(_animCamAngle() - pose->peraRotY);
        if ((diff > 90.0f && diff <= 270.0f) || (diff < -90.0f && diff >= -270.0f)) {
            pose->peraTargetRotY = _animClampAngle2(180.0f + _animCamAngle());
        } else {
            pose->peraTargetRotY = _animCamAngle();
        }
        while (pose->peraTargetRotY - pose->rotationY > 180.0f) {
            pose->rotationY += 360.0f;
        }
        while (pose->peraTargetRotY - pose->rotationY < -180.0f) {
            pose->rotationY -= 360.0f;
        }
        if (pose->flags & ANIM_POSE_FLAG_INSTANT_TURN) {
            pose->rotationY = pose->peraTargetRotY;
        } else {
            pose->rotationY += 0.1f * (pose->peraTargetRotY - pose->rotationY);
        }
        rotY = pose->rotationY;
        if (rotY != 0.0f) {
            PSMTXRotRad(tmp2, 'y', 0.017453292f * rotY);
            PSMTXConcat(matrix, tmp2, result);
            matrix = result;
        }
        if (pose->scaleZFlip == -1.0f) {
            PSMTXScale(tmp2, 1.0f, 1.0f, -1.0f);
            PSMTXConcat(matrix, tmp2, result);
            matrix = result;
        }
    } else if (rotY) {
        PSMTXRotRad(tmp1, 'y', 0.017453292f * rotY);
        PSMTXConcat(matrix, tmp1, result);
        matrix = result;
    }
    if (scale != 1.0f) {
        PSMTXScale(tmp3, scale, scale, scale);
        PSMTXConcat(matrix, tmp3, result);
        matrix = result;
    }
    if (pose->lastFrameZero == -1) {
        animPoseMainSub(pose);
    }
    g_modeling_mtx_lv = 0;
    g_modeling_mtx = &g_modeling_mtx_stack[0];
    PSMTXCopy(matrix, *g_modeling_mtx);
    wp->curPose = pose;
    wp->curAnimData = anim;
    wp->curPoseData = data;
    // FAKE: (s32) cast on the u32 flag (signed compare); the plain test changes the object
    if ((s32)wp->mbUseFloatScratch != 0) {
        DCFlushRange(wp->vtxArrayPos, animPoseGetBaseData(pose)->bufferPosNum * sizeof(Vec));
        DCFlushRange(wp->vtxArrayNrm, animPoseGetBaseData(pose)->bufferNrmNum * sizeof(Vec));
    } else {
        DCFlushRange(pose->vtxArrayPos, animPoseGetBaseData(pose)->bufferPosNum * sizeof(Vec));
        DCFlushRange(pose->vtxArrayNrm, animPoseGetBaseData(pose)->bufferNrmNum * sizeof(Vec));
    }
    GXInvalidateVtxCache();
    dispProc(-1, data->groupCount - 1, keyframeIdx, nextKeyframeIdx, keyframeBlend);
}

/** Stores the three paper texture matrices for the next draw (NULL for none). */
inline void animSetPaperMtx(Mtx m0, Mtx* m1, Mtx* m2) {
    if (m0 == NULL) {
        wp->paperMtxPtr[0] = NULL;
    } else {
        PSMTXCopy(m0, wp->paperMtx[0]);
        wp->paperMtxPtr[0] = &wp->paperMtx[0];
    }
    wp->paperMtxPtr[1] = m1;
    wp->paperMtxPtr[2] = m2;
}

// FAKE: the float-scratch block of animPoseDrawMtx as an inline helper (same code as animPoseMainSub); the sum becomes an inline local, numbered/coloured before posNum as in the target
static inline void apdScratchN(s32 posNum, s32 nrmNum) {
    s32 n = posNum + nrmNum;
    if (n <= wp->floatScratchRP) {
        if (wp->floatScratchWP + n > wp->floatScratchRP) {
            wp->floatScratchWP = 0;
        }
        wp->mbUseFloatScratch = 1;
        wp->vtxArrayPos = &wp->floatScratch[wp->floatScratchWP];
        wp->floatScratchWP += posNum;
        wp->vtxArrayNrm = &wp->floatScratch[wp->floatScratchWP];
        wp->floatScratchWP += nrmNum;
    }
}
// FAKE: the C0 block of animPoseDrawMtx written as an inline helper; its locals become inline temps, which gives the target nonvolatile colours (m/data) and stack slots
static inline void apdEval(AnimationPose* pose, Mtx matrix, f32 rotY, f32 scale) {
    Mtx tmp;
    Mtx *m;
    Mtx rot;
    Mtx scl;
    AnimationPoseData *data;
    data = *((AnimationPoseData**)wp->files[pose->fileId].handle->data);
    m = (Mtx*)matrix;
    if ((!(pose->flags & ANIM_POSE_FLAG_PERA)) && rotY) {
        PSMTXRotRad(rot, 'y', 0.017453292f * rotY);
        PSMTXConcat(matrix, rot, tmp);
        m = &tmp;
    }
    if (1.0f != scale) {
        PSMTXScale(scl, scale, scale, scale);
        PSMTXConcat(*m, scl, tmp);
        m = &tmp;
    }
    g_modeling_mtx_lv = 0;
    g_modeling_mtx = &g_modeling_mtx_stack[0];
    PSMTXCopy(*m, *g_modeling_mtx);
    wp->curPose = pose;
    wp->curPoseData = data;
    evalProc(-1, data->groupCount - 1);
}
#pragma push
#pragma opt_lifetimes off
/**
 * Draws pose poseId with matrix for draw stage xluMode, rotY and scale.
 * A pose with a paper overlay is drawn as that overlay,
 * textured with the pose's own image rendered offscreen and stretched to the overlay's shape.
 */
void animPoseDrawMtx(s32 poseId, Mtx matrix, s32 xluMode, f32 rotY, f32 scale) {
    Mtx m1;
    static Mtx mtx;
    Mtx m2;
    const char *name;
    AnimationPose *paper;
    AnimationPose *pose;
    s32 scratchNum[2];
    AnimationPoseData *pd;
    pose = &wp->poses[poseId];
    if (pose->paperId != -1) {
        if (pose->flags & (ANIM_POSE_FLAG_WORLD_POSITION_EVAL | ANIM_POSE_FLAG_WORLD_MATRIX_EVAL)) {
            apdEval(pose, matrix, rotY, scale);
        }
        paper = &wp->poses[pose->paperId];
        // FAKE: (animPoseGetBaseData) bypasses the macro so the real inline is used; its return temp keeps the paper wp load out of the early simplify pushes (wp r3 / files r4 / poses r5)
        pd = (animPoseGetBaseData)(paper);
        if (((xluMode != 2) || (!(pd->flags & 1))) && (!(pd->flags & (1 << xluMode)))) {
            return;
        }
        if (paper->typeFlags & ANIM_POSE_TYPE_PAPER) {
            scratchNum[1] = pd->bufferPosNum;
            scratchNum[0] = pd->bufferNrmNum;
            apdScratchN(scratchNum[1], scratchNum[0]);
            name = ANIM_INDEX_NAME(pose->paperAnimIdx);
            if (ANIM_NAME_INDEX(name) != -1) {
                // FAKE: the inlined animPoseSetAnim (its animName param re-reads the paper anim id) adds the coalesced param copy the target has (p2 r29 / xluMode r28)
                animPoseSetAnim(pose->paperId, ANIM_INDEX_NAME(pose->paperAnimIdx), FALSE);
            }
            paper->localTime = animTimeGetTime(paper->type);
            paper->lastFrameZero = -1;
            paper->lastFrameTime = 0.0f;
            paper->curKeyValue = 0;
            paper->loopTime = 0.0f;
            paper->localTime = pose->localTimeInit;
            animPoseMain(pose->paperId);
        }
        {
            struct smartEntry* t0 = pose->paperCapture0;
            struct smartEntry* t1 = pose->paperCapture1;
            wp->texObjs[0] = (t0) ? &pose->paperTexObj[0] : NULL;
            wp->texObjs[1] = (t1) ? &pose->paperTexObj[1] : NULL;
            wp->curTexObj = NULL;
            wp->texDatas[0] = t0;
            wp->texDatas[1] = t1;
            wp->curTexData = NULL;
            wp->paperTexMode0 = 1;
            wp->paperTexMode1 = 1;
        }
        paper->materialFlag = pose->materialFlag;
        paper->materialLightFlag = pose->materialLightFlag;
        paper->materialColor = pose->materialColor;
        paper->materialColor2 = pose->materialColor2;
        if (pose->flags & ANIM_POSE_FLAG_PAPER_GROUP) {
            PSMTXScale(m1, pose->paperTexScaleX, pose->paperTexScaleY, 1.0f);
            PSMTXTrans(m2, pose->paperTexOfsX, pose->paperTexOfsY, 0.0f);
            PSMTXConcat(m1, m2, m1);
            animSetPaperMtx(m1, NULL, NULL);
            PSMTXScale(m1, pose->paperScaleXZ, pose->paperScaleY, pose->paperScaleXZ);
            if (wp->inBattle) {
                PSMTXTrans(m2, 0.0f, 2.0f * pose->paperOfsY, 0.0f);
            } else {
                PSMTXTrans(m2, 0.0f, pose->paperOfsY, 0.0f);
            }
            PSMTXConcat(m2, m1, m1);
            PSMTXConcat(matrix, m1, mtx);
            matrix = mtx;
        } else {
            PSMTXScale(m1, pose->paperTexScaleX, pose->paperTexScaleY, 1.0f);
            PSMTXTrans(m2, pose->paperTexOfsX, pose->paperTexOfsY, 0.0f);
            PSMTXConcat(m1, m2, m1);
            PSMTXTrans(m2, 0.5f, 1.0f, 0.0f);
            PSMTXConcat(m1, m2, m1);
            PSMTXScale(m2, 1.0f / pose->paperScaleXZ, 1.0f / pose->paperScaleY, 1.0f);
            PSMTXConcat(m1, m2, m1);
            PSMTXTrans(m2, -0.5f, -1.0f, 0.0f);
            PSMTXConcat(m1, m2, m1);
            animSetPaperMtx(m1, NULL, NULL);
        }
    } else {
        pd = *((AnimationPoseData**)wp->files[pose->fileId].handle->data);
        if (((xluMode != 2) || (!(pd->flags & 1))) && (!(pd->flags & (1 << xluMode)))) {
            return;
        }
    }
    if (pose->paperId != -1) {
        _animPoseDrawMtx(paper, matrix, xluMode, rotY, scale);
        wp->paperMtxPtr[0] = NULL;
        wp->paperMtxPtr[1] = NULL;
        wp->paperMtxPtr[2] = NULL;
        if (paper->typeFlags & ANIM_POSE_TYPE_PAPER) {
            wp->mbUseFloatScratch = 0;
        }
    } else {
        _animPoseDrawMtx(pose, matrix, xluMode, rotY, scale);
    }
}
#pragma pop

u32 anim_ext_tex_size;

/** Sets the textures (and their data) the next paper overlay draw substitutes, and the paper texture mode. */
void animSetPaperTexObj(GXTexObj* obj1, GXTexObj* obj2, GXTexObj* obj3, void* data1, void* data2, void* data3, s32 texMode, s32 unused) {
    wp->texObjs[0] = data1 ? obj1 : NULL;
    wp->texObjs[1] = data2 ? obj2 : NULL;
    wp->curTexObj = data3 ? obj3 : NULL;
    wp->texDatas[0] = data1;
    wp->texDatas[1] = data2;
    wp->curTexData = data3;
    wp->paperTexMode0 = texMode;
    wp->paperTexMode1 = texMode;
}

/**
 * Drops a reference to animation file fileId; the last user frees it
 * and its texture file (when nobody else uses that).
 */
inline void animPoseFileRelease(s32 fileId) {
    AnimationPoseFile* file = &wp->files[fileId];
    AnimationTextureFile* tex;

    file->references--;
    if (file->references <= 0) {
        file->active = 0;
        tex = &wp->textures[file->textureId];
        tex->references--;
        if (tex->references <= 0) {
            fileFree(tex->handle);
            tex->active = FALSE;
        }
        fileFree(file->handle);
    }
}

/**
 * Releases pose poseId: paper overlays drop a reference;
 * normal poses free their world position and matrix buffers and their animation file.
 */
void animPoseRelease(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];

    // FAKE: (u32) cast on typeFlags (unsigned compare); the plain test changes the object
    if ((u32)pose->typeFlags != 0) {
        animPaperPoseRelease(poseId);
    } else {
        if (pose->flags & ANIM_POSE_FLAG_WORLD_POSITION_EVAL) {
            smartFree(pose->worldPositions);
        }
        if (pose->flags & ANIM_POSE_FLAG_WORLD_MATRIX_EVAL) {
            smartFree(pose->worldMatrices);
        }
        animPoseFileRelease(pose->fileId);
        pose->flags = 0;
    }
}

/** Drops a reference to the paper pose poseId and releases it with its file when no user is left. */
void animPaperPoseRelease(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];

    pose->references--;
    if (pose->references < 0) {
        animPoseFileRelease(pose->fileId);
        pose->flags = 0;
    }
}

/**
 * Releases every pose of type type (0 field, 1 battle, 2 shared), e.g. when leaving a map or a battle;
 * releasing the battle poses also leaves battle mode. Then reallocates the vertex buffers of the rest.
 */
void animPoseAutoRelease(s32 type) {
    int i;
    AnimationPose* pose;

    for (i = 0; i < wp->totalPoses; i++) {
        pose = animPoseGetAnimPosePtr(i);
        if (pose->flags & ANIM_POSE_FLAG_ACTIVE && pose->type == type) {
            animPoseRelease(i);
        }
    }
    if (type == 1) {
        wp->inBattle = FALSE;
    }
    animPoseRefresh();
}

/** Offscreen draw callback: renders every pose that has a paper overlay into its own texture. */
void animPaperPoseDisp(CameraId camId, void* param) {
    int i;
    AnimationPose* pose;

    for (i = 0, pose = wp->poses; i < wp->totalPoses; i++, pose++) {
        if (pose->flags & ANIM_POSE_FLAG_ACTIVE && pose->paperId != -1) {
            animPaperPoseDispSub(0, pose);
        }
    }
}

/**
 * Renders one pose with a paper overlay offscreen (unturned) into a texture sized to its screen bounding box,
 * so the overlay can be drawn with the character's image;
 * with rectangle caching on, the rectangle is only remeasured when it changes.
 */
void animPaperPoseDispSub(s32 unused, AnimationPose* arg) {
    AnimationPose* pose;
    AnimData* d;
    Mtx m, save;
    GXColor col1;
    u16 h;
    GXColor col2;
    s32 lightFlag;
    u32 flags;
    s32 matFlag;
    u16 left, top, right, bottom;
    f32 scale, hh;
    s32 t, b, e, a, x1, y0, y1;
    u16 w;

    pose = arg;
    scale = 8.0f;
    if (!(arg->flags & ANIM_POSE_FLAG_ACTIVE)) {
        return;
    }
    // FAKE: dead int-guarded re-definition (COLOURING M). The second reaching def stops IRO pass-0 copy
    // propagation, so 'pose' stays a named local (numbered above d) instead of the low parameter vreg;
    // the arm is folded away (t is reassigned before its next use). Behaviour-identical.
    t = 0;
    if (t != 0) {
        pose = arg;
    }
    d = animPoseGetBaseData(pose)->anims[pose->animId].data;
    scale *= 25.74f / (d->bboxMax.y - d->bboxMin.y);
    hh = 0.75f * DEMOGetRenderModeObj()->efbHeight;
    sysWaitDrawSync();
    GXClearBoundingBox();
    MTXTrans(m, 0.0f, 0.25f * -(f32)DEMOGetRenderModeObj()->efbHeight - d->bboxMin.y * scale, 0.0f);
    flags = pose->flags;
    matFlag = pose->materialFlag;
    lightFlag = pose->materialLightFlag;
    col1 = pose->materialColor;
    col2 = pose->materialColor2;
    pose->flags = flags & ~ANIM_POSE_FLAG_PERA;
    pose->materialColor = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    pose->materialColor2 = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    pose->materialLightFlag = 0;
    if (pose->vivianType) {
        PSMTXCopy(pose->matrix, save);
        PSMTXCopy(m, pose->matrix);
    }
    _animPoseDrawMtx(pose, m, 2, 0.0f, scale);
    pose->flags = flags;
    pose->materialFlag = matFlag;
    pose->materialColor = col1;
    pose->materialColor2 = col2;
    pose->materialLightFlag = lightFlag;
    if (pose->vivianType) {
        PSMTXCopy(save, pose->matrix);
    }
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    if (left > right) {
        left = 0;
        top = 0;
        right = 16;
        bottom = 16;
    }
    if (pose->flags & ANIM_POSE_FLAG_PAPER_GROUP && pose->flags & ANIM_POSE_FLAG_PAPER_ANIM_CHANGED && gp->inBattle) {
        a = d->bboxMin.x * scale + (s32)(DEMOGetRenderModeObj()->fbWidth / 2);
        x1 = d->bboxMax.x * scale + (s32)(DEMOGetRenderModeObj()->fbWidth / 2);
        y0 = hh - scale * (d->bboxMax.y - d->bboxMin.y);
        y1 = hh;
        if (a <= left && x1 >= right && y0 <= top && y1 >= bottom) {
            left = a;
            right = x1;
            top = y0;
            bottom = y1;
        }
    }
    if (left < 2) {
        left = 2;
    }
    if (top < 2) {
        top = 2;
    }
    if (right > DEMOGetRenderModeObj()->fbWidth - 2) {
        right = DEMOGetRenderModeObj()->fbWidth - 2;
    }
    if (bottom > DEMOGetRenderModeObj()->efbHeight - 2) {
        bottom = DEMOGetRenderModeObj()->efbHeight - 2;
    }
    if (pose->flags & ANIM_POSE_FLAG_PAPER_GROUP) {
        if (pose->flags & ANIM_POSE_FLAG_PAPER_ANIM_CHANGED) {
            a = DEMOGetRenderModeObj()->fbWidth / 2 - left;
            t = right - DEMOGetRenderModeObj()->fbWidth / 2;
            b = a;
            if (a < t) {
                b = t;
            }
            a = 0.5f * b;
            e = 0.14881f * (bottom - top);
            pose->paperRectLeft = DEMOGetRenderModeObj()->fbWidth / 2 - b - a;
            pose->paperRectRight = DEMOGetRenderModeObj()->fbWidth / 2 + b + a;
            pose->paperRectTop = top - e;
            pose->paperRectBottom = bottom + e;
            pose->paperScaleXZ = (8.0f / scale) * ((f32)(pose->paperRectRight - pose->paperRectLeft) / 150.0f);
            pose->paperScaleY = (8.0f / scale) * ((f32)(pose->paperRectBottom - pose->paperRectTop) / 215.0f);
            pose->paperOfsY = d->bboxMin.y + (hh - bottom) / scale;
            pose->flags &= ~ANIM_POSE_FLAG_PAPER_ANIM_CHANGED;
        }
    } else {
        s32 a;
        s32 e;
        a = DEMOGetRenderModeObj()->fbWidth / 2 - left;
        t = right - DEMOGetRenderModeObj()->fbWidth / 2;
        b = a;
        if (a < t) {
            b = t;
        }
        a = 0.5f * b;
        e = 0.14881f * (bottom - top);
        pose->paperRectLeft = DEMOGetRenderModeObj()->fbWidth / 2 - b - a;
        pose->paperRectRight = DEMOGetRenderModeObj()->fbWidth / 2 + b + a;
        pose->paperRectTop = top - e;
        pose->paperRectBottom = bottom + e;
        pose->paperScaleXZ = (8.0f / scale) * ((f32)(pose->paperRectRight - pose->paperRectLeft) / 150.0f);
        pose->paperScaleY = (8.0f / scale) * ((f32)(pose->paperRectBottom - pose->paperRectTop) / 215.0f);
        pose->paperOfsY = d->bboxMin.y + (hh - bottom) / scale;
    }
    left -= 2;
    top -= 2;
    right += 2;
    bottom += 2;
    left -= left & 1;
    top -= top & 1;
    right += right & 1;
    bottom += bottom & 1;
    w = right - left;
    h = bottom - top;
    pose->paperTexOfsX = -((f32)left - pose->paperRectLeft) / (f32)(pose->paperRectRight - pose->paperRectLeft);
    pose->paperTexOfsY = -((f32)top - pose->paperRectTop) / (f32)(pose->paperRectBottom - pose->paperRectTop);
    pose->paperTexScaleX = (f32)(pose->paperRectRight - pose->paperRectLeft) / (f32)w;
    pose->paperTexScaleY = (f32)(pose->paperRectBottom - pose->paperRectTop) / (f32)h;
    GXSetTexCopySrc(left, top, w, h);
    GXSetTexCopyDst(w, h, GX_TF_RGB5A3, GX_FALSE);
    pose->paperCapture0 = smartAlloc(GXGetTexBufferSize(w, h, GX_TF_RGB5A3, GX_FALSE, 0), 3);
    GXCopyTex(pose->paperCapture0->address, GX_TRUE);
    GXPixModeSync();
    GXInitTexObj(&pose->paperTexObj[0], pose->paperCapture0->address, w, h, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    pose->paperCapture1 = NULL;
}

/** Returns the id of the first used pose of type type loaded from animation file name, or -1. */
inline s32 animPoseSearchName(const char* name, s32 type) {
    AnimationPose* pose;
    s32 i;

    for (i = 0; i < wp->totalPoses; i++) {
        pose = &wp->poses[i];
        if (pose->flags & ANIM_POSE_FLAG_ACTIVE && pose->type == type && !strcmp(animPoseGetBaseData(pose)->fileName, name)) {
            break;
        }
    }
    if (i == wp->totalPoses) {
        i = -1;
    }
    return i;
}

#pragma push
#pragma opt_lifetimes off
/**
 * Renders frame frame of animation anim of the loaded pose group at scale into the EFB (white material, no paper
 * effect), copies its bounding box to a new RGB5A3 texture (*image, obj)
 * and returns the box relative to the screen centre in x0..y1; the pose is restored afterwards.
 * Used by extdrv for its crowds of flat sprites.
 * The clamp of the bottom edge tests top instead of bottom, as in the original.
 */
void animPoseDisp_MakeExtTexture(const char* group, const char* anim, f32 frame, f32 scale, GXTexObj* obj,
                                 smartEntry** image, s32* x0, s32* y0, s32* x1, s32* y1) {
    AnimationPose* pose;
    AnimationPose* pera;
    AnimationPose backup;
    Mtx m;
    GXColor col1, col2;
    u32 flags;
    s32 matFlag;
    u16 left, top, right, bottom;
    u16 w, h;
    f32 hw, hh;
    s32 i;

    i = animPoseSearchName(group, gp->inBattle != 0);
    if (i < 0) {
        return;
    }
    pose = &wp->poses[i];
    backup = *pose;
    pose->paperId = -1;
    animPoseSetAnim(i, anim, FALSE);
    pose->localTime = animTimeGetTime(pose->type);
    pose->lastFrameZero = -1;
    pose->lastFrameTime = 0.0f;
    pose->curKeyValue = 0;
    pose->loopTime = 0.0f;
    animPoseSetLocalTime(i, frame);
    animPoseMain(i);

    hw = 0.5f * DEMOGetRenderModeObj()->fbWidth;
    hh = 0.75f * DEMOGetRenderModeObj()->efbHeight;
    sysWaitDrawSync();
    GXClearBoundingBox();
    MTXTrans(m, 0.0f, 0.25f * -(f32)DEMOGetRenderModeObj()->efbHeight, 0.0f);
    flags = pose->flags;
    matFlag = pose->materialFlag;
    col1 = pose->materialColor;
    col2 = pose->materialColor2;
    pose->flags = flags & ~ANIM_POSE_FLAG_PERA;
    pose->materialFlag = 0;
    pose->materialColor = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    pose->materialColor2 = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    _animPoseDrawMtx(pose, m, 1, 0.0f, scale);
    _animPoseDrawMtx(pose, m, 2, 0.0f, scale);
    _animPoseDrawMtx(pose, m, 3, 0.0f, scale);
    pose->materialColor = col1;
    pose->materialColor2 = col2;
    pose->flags = flags;
    pose->materialFlag = matFlag;
    sysWaitDrawSync();
    GXReadBoundingBox(&left, &top, &right, &bottom);
    if (left > right) {
        top = left = 0;
        bottom = right = 16;
    }
    if (left < 2) {
        left = 2;
    }
    if (top < 2) {
        top = 2;
    }
    if (right > DEMOGetRenderModeObj()->fbWidth - 2) {
        right = DEMOGetRenderModeObj()->fbWidth - 2;
    }
    // BUG: limits top where animPaperPoseDispSub limits bottom; bottom is never limited to the frame height
    if (top > DEMOGetRenderModeObj()->efbHeight - 2) {
        top = DEMOGetRenderModeObj()->efbHeight - 2;
    }
    left -= left & 1;
    top -= top & 1;
    right += right & 1;
    bottom += bottom & 1;
    w = right - left;
    h = bottom - top;
    GXSetTexCopySrc(left, top, w, h);
    GXSetTexCopyDst(w, h, GX_TF_RGB5A3, GX_FALSE);
    anim_ext_tex_size += GXGetTexBufferSize(w, h, GX_TF_RGB5A3, GX_FALSE, 0);
    *image = smartAlloc(GXGetTexBufferSize(w, h, GX_TF_RGB5A3, GX_FALSE, 0), 1);
    GXCopyTex((*image)->address, GX_TRUE);
    GXPixModeSync();
    GXInitTexObj(obj, (*image)->address, w, h, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    *x0 = left - (s32)hw;
    *y0 = top - (s32)hh;
    *x1 = right - (s32)hw;
    *y1 = bottom - (s32)hh;
    *pose = backup;
}
#pragma pop

/** Copies the three paper texture matrices for the next draw (NULL for none). */
void animSetPaperTexMtx(Mtx mtx1, Mtx mtx2, Mtx mtx3) {
    if (mtx1 == NULL) {
        wp->paperMtxPtr[0] = NULL;
    } else {
        PSMTXCopy(mtx1, wp->paperMtx[0]);
        wp->paperMtxPtr[0] = &wp->paperMtx[0];
    }
    if (mtx2 == NULL) {
        wp->paperMtxPtr[1] = NULL;
    } else {
        PSMTXCopy(mtx2, wp->paperMtx[1]);
        wp->paperMtxPtr[1] = &wp->paperMtx[1];
    }
    if (mtx3 == NULL) {
        wp->paperMtxPtr[2] = NULL;
    } else {
        PSMTXCopy(mtx3, wp->paperMtx[2]);
        wp->paperMtxPtr[2] = &wp->paperMtx[2];
    }
}

/**
 * Starts loading animation file name and its textures in group's archive in the background,
 * with callback for the model; TRUE once both are loaded (or missing), FALSE while still loading.
 */
BOOL animGroupBaseAsync(const char* name, s32 group, void* callback) {
    s32 model, anim;

    switch (group) {
        case 0:
            fileSetCurrentArchiveType(1);
            break;
        case 1:
            fileSetCurrentArchiveType(2);
            break;
        case 2:
            fileSetCurrentArchiveType(0);
            break;
    }
    model = fileAsyncf(FILE_TYPE_ANIM, callback, "%s/%s", "a", name);
    anim = fileAsyncf(FILE_TYPE_TPL, NULL, "%s/%s-", "a", animGetTexName(name));
    if (model == -1) {
        return TRUE;
    }
    // FAKE: (u32) cast (unsigned compare); plain model == 0 changes the object
    if ((u32)model == 0) {
        return FALSE;
    }
    if (anim == -1) {
        return TRUE;
    }
    return anim != 0;
}

/** Returns the texture file name the animation file name uses, from a/ag2tg.bin; "" if it is not listed. */
const char* animGetTexName(const char* name) {
    void** data = wp->mp_ag2tg->data;
    s32 size;
    s32 i;
    char* entry;

    size = ((smartEntry*)data)->size; // file data handle: address, then size
    entry = data[0];

    for (i = 0; i < size; entry += 0x20, i += 0x20) {
        if (!strcmp(entry, name)) {
            return entry + 0x20;
        }
    }
    return "";
}

void animStrippedPool(f32* p, u32 n) {
    p[0] = 0.0f;
    p[1] = 0.017453292f * p[1];
    p[2] = n;
}

/** Returns pose poseId's entry. */
AnimationPose* animPoseGetAnimPosePtr(s32 poseId) {
    return &wp->poses[poseId];
}

/** Returns the data of pose poseId's current animation. */
AnimData* animPoseGetAnimDataPtr(s32 poseId) {
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);
    return animPoseGetAnimBaseDataPtr(poseId)->anims[pose->animId].data;
}

/** Returns the animation file data pose poseId was loaded from. */
AnimationPoseData* animPoseGetAnimBaseDataPtr(s32 poseId) {
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);
    return *wp->files[pose->fileId].handle->data;
}

/** Returns the table entry (name and data) of pose poseId's current animation. */
AnimTableEntry* animPoseGetCurrentAnim(s32 poseId) {
    AnimationPoseData* data = animPoseGetAnimBaseDataPtr(poseId);
    return &data->anims[animPoseGetAnimPosePtr(poseId)->animId];
}

/** Tells whether pose poseId's animation file has translucent parts (file flag 8). */
BOOL animPoseTestXLU(s32 poseId) {
    return (animPoseGetAnimBaseDataPtr(poseId)->flags >> 3) & 1;
}

/**
 * Walks the pose's group tree from groupId and pushes each visible group's node matrix,
 * storing the groups' world positions or matrices when the pose asked for them; it does not draw.
 */
void evalProc(s32 parentId, s32 groupId) {
    AnimPoseGroup* group;
    AnimPoseGroup* parent;
    AnimPoseNode* node;
    AnimationPose* pose;
    AnimationPoseData* data;
    f32 sx, sy, sz;

    if (groupId == -1) {
        return;
    }
    pose = wp->curPose;
    data = wp->curPoseData;
    group = &data->groups[groupId];
    if (pose->groupVisibility[group->visibilityId]) {
        pose = wp->curPose;
        node = (AnimPoseNode*)(pose->nodes + group->nodeId);
        if (group->useSSC) {
            if (parentId == -1) {
                sx = sy = sz = 1.0f;
            } else {
                parent = &data->groups[parentId];
                if (!parent->useSSC) {
                    sx = sy = sz = 1.0f;
                } else {
                    AnimPoseNode* pnode = (AnimPoseNode*)(pose->nodes + parent->nodeId);
                    sx = pnode->mScale.x;
                    sy = pnode->mScale.y;
                    sz = pnode->mScale.z;
                }
            }
            pushGXModelMtx_JointNode__(node, sx, sy, sz);
        } else {
            pushGXModelMtx_TransformNode__(node);
        }
        if (wp->curPose->flags & ANIM_POSE_FLAG_WORLD_POSITION_EVAL) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = (*g_modeling_mtx)[0][3];
            pos.y = (*g_modeling_mtx)[1][3];
            pos.z = (*g_modeling_mtx)[2][3];
            ((Vec*)wp->curPose->worldPositions->address)[groupId] = pos;
        }
        if (wp->curPose->flags & ANIM_POSE_FLAG_WORLD_MATRIX_EVAL) {
            PSMTXCopy(*g_modeling_mtx, ((Mtx*)wp->curPose->worldMatrices->address)[groupId]);
        }
        if (!(wp->curPose->flags & ANIM_POSE_FLAG_VIVIAN) || groupId != wp->curPose->vivianGroupNo[0]) {
            evalProc(groupId, group->childGroupId);
        }
        g_modeling_mtx_lv--;
        g_modeling_mtx = &g_modeling_mtx_stack[g_modeling_mtx_lv];
    }
    evalProc(groupId, group->brotherGroupId);
}

/**
 * Makes pose poseId record the world position of each of its groups
 * when evaluated (for attaching things to body parts), allocating the buffer from the pose's heap.
 */
void animPoseWorldPositionEvalOn(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    AnimationPoseData* data = animPoseGetBaseData(pose);
    u8 heap;

    if (!(pose->flags & ANIM_POSE_FLAG_WORLD_POSITION_EVAL)) {
        pose->flags |= ANIM_POSE_FLAG_WORLD_POSITION_EVAL;
        switch (pose->type) {
            case 0:
                heap = 1;
                break;
            case 1:
                heap = 2;
                break;
            case 2:
                heap = 0;
                break;
        }
        pose->numWorldPositions = data->groupCount;
        pose->worldPositions = smartAlloc(sizeof(Vec) * data->groupCount, heap);
    }
}

/** Stops pose poseId recording group world positions and frees the buffer. */
void animPoseWorldPositionEvalOff(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];

    if (pose->flags & ANIM_POSE_FLAG_WORLD_POSITION_EVAL) {
        pose->flags &= ~ANIM_POSE_FLAG_WORLD_POSITION_EVAL;
        smartFree(pose->worldPositions);
        pose->worldPositions = NULL;
    }
}

/**
 * Makes pose poseId record the world matrix of each of its groups when evaluated,
 * allocating the buffer from the pose's heap.
 */
void animPoseWorldMatrixEvalOn(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    AnimationPoseData* data = animPoseGetBaseData(pose);
    u8 heap;

    if (!(pose->flags & ANIM_POSE_FLAG_WORLD_MATRIX_EVAL)) {
        pose->flags |= ANIM_POSE_FLAG_WORLD_MATRIX_EVAL;
        switch (pose->type) {
            case 0:
                heap = 1;
                break;
            case 1:
                heap = 2;
                break;
            case 2:
                heap = 0;
                break;
        }
        pose->numWorldMatrices = data->groupCount;
        pose->worldMatrices = smartAlloc(sizeof(Mtx) * data->groupCount, heap);
    }
}

/** Updates Vivian's (or Marilyn's or Beldam's) hair and tail chain this frame with the character's position pos. */
void animPoseVivianMain(s32 poseId, Vec* pos) {
    AnimationPose* pose = &wp->poses[poseId];

    pose->flags |= ANIM_POSE_FLAG_VIVIAN;
    pose->matrix[0][3] = pos->x;
    pose->matrix[1][3] = pos->y;
    pose->matrix[2][3] = pos->z;
}

/** Which of the chain-haired characters pose poseId is: 0 none, 1 Vivian, 2 Marilyn, 3 Beldam. */
s32 animPoseGetVivianType(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    return pose->vivianType;
}

/** Sets a callback called with userData before each group of pose poseId is drawn. */
void animPoseSetDispCallBack(s32 poseId, AnimPoseDispCallback callback, void* userData) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->dispCallback = callback;
    pose->dispUserDataCallback = userData;
}

/**
 * Evaluates a pose's animation; shared paper poses evaluate into the float scratch buffer instead of their own
 * vertex buffers.
 */
inline void animPoseMainSub(AnimationPose* pose) {
    s32 id = (u32)((u8*)pose - (u8*)wp->poses) / sizeof(AnimationPose);
    if (pose->typeFlags & ANIM_POSE_TYPE_PAPER) {
        AnimationPoseData* data = animPoseGetBaseData(pose);
        s32 posNum = data->bufferPosNum;
        s32 nrmNum = data->bufferNrmNum;
        if (posNum + nrmNum <= wp->floatScratchRP) {
            if (wp->floatScratchWP + (posNum + nrmNum) > wp->floatScratchRP) {
                wp->floatScratchWP = 0;
            }
            wp->mbUseFloatScratch = 1;
            wp->vtxArrayPos = &wp->floatScratch[wp->floatScratchWP];
            wp->floatScratchWP += posNum;
            wp->vtxArrayNrm = &wp->floatScratch[wp->floatScratchWP];
            wp->floatScratchWP += nrmNum;
        }
        animPoseMain(id);
        wp->mbUseFloatScratch = 0;
    } else {
        animPoseMain(id);
    }
}

/**
 * Draws one shape shapeId of pose poseId on its own (evaluating the pose first if needed),
 * saving and restoring the driver's current draw state around it.
 */
void animPoseDrawShape(s32 poseId, s32 shapeId) {
    AnimationPose* pose;
    GXTexObj* texObj0;
    GXTexObj* texObj1;
    GXTexObj* curTexObj;
    void* texData0;
    void* texData1;
    void* curTexData;
    Mtx* paperMtx0;
    Mtx* paperMtx1;
    Mtx* paperMtx2;
    AnimationPose* curPose;
    AnimData* curAnimData;
    AnimationPoseData* curPoseData;
    u32 texMode0;
    u32 texMode1;

    pose = &wp->poses[poseId];
    curPose = wp->curPose;
    curAnimData = wp->curAnimData;
    curPoseData = wp->curPoseData;
    texObj0 = wp->texObjs[0];
    texObj1 = wp->texObjs[1];
    curTexObj = wp->curTexObj;
    texData0 = wp->texDatas[0];
    texData1 = wp->texDatas[1];
    curTexData = wp->curTexData;
    paperMtx0 = wp->paperMtxPtr[0];
    paperMtx1 = wp->paperMtxPtr[1];
    paperMtx2 = wp->paperMtxPtr[2];
    texMode0 = wp->paperTexMode0;
    texMode1 = wp->paperTexMode1;

    wp->curPose = pose;
    wp->curAnimData = animPoseGetBaseData(pose)->anims[pose->animId].data;
    wp->curPoseData = animPoseGetBaseData(pose);
    wp->texObjs[0] = NULL;
    wp->texObjs[1] = NULL;
    wp->curTexObj = NULL;
    wp->texDatas[0] = NULL;
    wp->texDatas[1] = NULL;
    wp->curTexData = NULL;
    wp->paperMtxPtr[0] = NULL;
    wp->paperMtxPtr[1] = NULL;
    wp->paperMtxPtr[2] = NULL;
    wp->paperTexMode0 = 0;
    wp->paperTexMode1 = 0;

    if (pose->lastFrameZero == -1) {
        animPoseMainSub(pose);
    }
    renderProc(shapeId, pose->keyframeIdx);

    wp->curPose = curPose;
    wp->curAnimData = curAnimData;
    wp->curPoseData = curPoseData;
    wp->texObjs[0] = texObj0;
    wp->texObjs[1] = texObj1;
    wp->curTexObj = curTexObj;
    wp->texDatas[0] = texData0;
    wp->texDatas[1] = texData1;
    wp->curTexData = curTexData;
    wp->paperMtxPtr[0] = paperMtx0;
    wp->paperMtxPtr[1] = paperMtx1;
    wp->paperMtxPtr[2] = paperMtx2;
    wp->paperTexMode0 = texMode0;
    wp->paperTexMode1 = texMode1;
}

/** Returns the index of the shape named name in pose poseId's animation file, or -1. */
s32 animPoseGetShapeIdx(s32 poseId, const char* name) {
    AnimationPoseData* data;
    AnimPoseShape* shape;
    s32 i;

    data = animPoseGetAnimBaseDataPtr(poseId);
    shape = data->shapes;
    for (i = 0; i < data->shapeCount; i++, shape++) {
        if (!strcmp(shape->name, name)) {
            return i;
        }
    }
    return -1;
}

/** Returns the index of the group named name in pose poseId's animation file, or -1. */
s32 animPoseGetGroupIdx(s32 poseId, const char* name) {
    AnimationPoseData* data;
    AnimPoseGroup* group;
    s32 i;

    data = animPoseGetAnimBaseDataPtr(poseId);
    group = data->groups;
    for (i = 0; i < data->groupCount; i++, group++) {
        if (!strcmp(group->name, name)) {
            return i;
        }
    }
    return -1;
}

/** Returns the name of group group of pose poseId's animation file. */
const char* animPoseGetGroupName(s32 poseId, s32 group) {
    return animPoseGetAnimBaseDataPtr(poseId)->groups[group].name;
}

// Uninitialised objects are emitted in reverse definition order, so these end up as g_modeling_mtx_stack, work,
// tanfTbl in .bss.
f32 tanfTbl[0xB2];
AnimationWork work;
Mtx g_modeling_mtx_stack[32];
