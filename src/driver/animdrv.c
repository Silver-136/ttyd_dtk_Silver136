/*
 * Warning, inlining hell
 */
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

#include <stdio.h>

#include <string.h>
#include "driver/camdrv.h"
#include "manager/filemgr.h"
#include "libc_decls.h"
#include "sdk/texPalette.h"
#include "sdk/DEMOInit.h"

#define TESTHEAP_SIZE (u32)(1.5 * 1024 * 1024)
#define PI 3.14159265358979323846264338327950288419716939937510f

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

typedef struct MoaChain {
    const char* a;
    const char* b;
} MoaChain;
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
extern f32 tanfTbl[0xB2];

//.sdata
s32 vivihimoTailGroupNo[4] ATTRIBUTE_ALIGN(32) = { 0, 2, -1, -1 };
void* vivihimoData[4] = {
    NULL,
    _vivihimoData[0],
    _vivihimoData[1],
    _vivihimoData[2],
};
static AnimationWork* wp = &work;

//.sbss
s32 anim_unused_sbss;
Mtx* g_modeling_mtx;
s32 g_modeling_mtx_lv;

// local declarations
void animPoseRefresh(void);
void animPaperPoseDisp(CameraId camId, void* param);
void animPaperPoseDispSub(s32 unused, AnimationPose* pose);
void renderProc(s32 shapeId, s32 unused);
void pushGXModelMtx_JointNode__(AnimPoseNode* node, f32 sx, f32 sy, f32 sz);
void pushGXModelMtx_TransformNode__(AnimPoseNode* node);
void evalProc(s32 parentId, s32 groupId);

AnimationWork* animGetPtr(void) {
    return wp;
}

OSTime animTimeGetTime(BOOL globalTime) {
    if (globalTime == FALSE) {
        return OSTicksToMilliseconds(gp->renderFieldTime);
    }
    return OSTicksToMilliseconds(gp->renderTime);
}

void initTestHeap(void) {
    if (!wp->testHeap) { // allocate 1.5MiB test heap
        wp->testHeap = __memAlloc(HEAP_DEFAULT, TESTHEAP_SIZE);
    }
    wp->testAlloc = wp->testHeap;
}

// heavily inlined
void* testAlloc(u32 size) {
    void* alloc = wp->testAlloc;

    if (size & 31) {
        size += 32 - (size & 31);
    }
    wp->testAlloc += size;
    if (wp->testAlloc >= (u8*)wp->testHeap + TESTHEAP_SIZE) { // need to allocate more
        animPoseRefresh();
        alloc = testAlloc(size);
    }
    return alloc;
}

void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter min_filt, GXTexFilter mag_filt,
    f32 min_lod, f32 max_lod, f32 lod_bias, GXBool bias_clamp,
    GXBool do_edge_lod, GXAnisotropy max_aniso);
int __cvt_dbl_usll();
int __cvt_ull_flt();
int __div2i();
void _animPoseDrawMtx(AnimationPose* pose, Mtx matrix, s32 xluMode, f32 rotY, f32 scale);
void animPose_AllocBuffer(AnimationPose* pose);
void animSetMaterial_Texture(s32 texCount, s32 *texIdRemap, AnimPoseTexEntry *pTexEntries, AnimTexMtx *animEntries, s32 texBindsCapacity, AnimTexBind *texBinds, AnimationTextureFile *texFile);
void dispProc(s32 parentId, s32 groupId, s32 xluStage, s32 nextKeyframe, f32 blend);
void materialProc(s32 shapeId, s32 xluStage);
void vivihimo(Mtx a, Mtx b, f32 speed, f32 scale, f32 offA, f32 offB);
void animInit(void) {
    int i;
    FileEntry* file;

    wp->totalFiles = 64; // TODO: #define these?
    wp->totalTextures = 64;
    wp->totalPoses = 256;
    wp->mFloatScratchRP = 1024;
    wp->mFloatScratchWP = 0;
    wp->files = __memAlloc(HEAP_DEFAULT, sizeof(AnimationPoseFile) * wp->totalFiles);
    wp->textures = __memAlloc(HEAP_DEFAULT, sizeof(AnimationTextureFile) * wp->totalTextures);
    wp->poses = __memAlloc(HEAP_DEFAULT, sizeof(AnimationPose) * wp->totalPoses);
    wp->mbUseFloatScratch = 0;
    wp->mpFloatScratch = __memAlloc(HEAP_DEFAULT, sizeof(Vec) * wp->mFloatScratchRP);

    wp->mpTexObjs[0] = NULL;
    wp->mpTexObjs[1] = NULL;
    wp->mpCurTexObj = NULL;

    wp->mpTexDatas[0] = NULL;
    wp->mpTexDatas[1] = NULL;
    wp->mpCurTexData = NULL;

    wp->paperMtxPtr[0] = NULL;
    wp->paperMtxPtr[1] = NULL;
    wp->paperMtxPtr[2] = NULL;

    wp->paperTexMode0 = 1;
    wp->paperTexMode1 = 1;

    wp->testHeap = NULL;
    wp->inBattle = FALSE;
    initTestHeap(); // inlined

    for (i = 0; i < 0xB2; i++) {
        tanfTbl[i] = (f32)tan((i - 0x59) * 0.017453289f);
    }
    file = fileAllocf(0, "a/vivian.bin");
    if (file) {
        memcpy_as4(vivihimoData[1], *file->data, sizeof(_vivihimoData));
        memcpy_as4(vivihimoTailGroupNo, (u8*)*file->data + sizeof(_vivihimoData), sizeof(vivihimoTailGroupNo));
        fileFree(file);
    }
    fileSetCurrentArchiveType(0);
    wp->mp_ag2tg = fileAllocf(0, "a/ag2tg.bin");
}

void animMain(void) {
    dispEntry(CAMERA_OFFSCREEN, 0.0f, 1, animPaperPoseDisp, NULL);
}

inline AnimationPoseData* animPoseGetBaseData(AnimationPose* pose) {
    return *wp->files[pose->fileId].handle->data;
}

#define animPoseGetBaseData(pose) (*(AnimationPoseData**)(wp->files[(pose)->fileId].handle->data))

// heavily inlined, TODO helper function for that AnimPoseData* data, copy of animPoseGetAnimBaseDataPtr with AnimPose*
// arg
void animPose_AllocBuffer(AnimationPose* pose) {
    pose->mpBufferVtxPos = testAlloc(sizeof(Vec) * animPoseGetBaseData(pose)->mBufferPosNum);
    pose->mpVtxArrayPos = testAlloc(sizeof(Vec) * animPoseGetBaseData(pose)->mBufferPosNum);

    pose->mpBufferVtxNrm = testAlloc(sizeof(Vec) * animPoseGetBaseData(pose)->mBufferNrmNum);
    pose->mpVtxArrayNrm = testAlloc(sizeof(Vec) * animPoseGetBaseData(pose)->mBufferNrmNum);

    pose->mpBufferGroupVisibility = testAlloc(sizeof(s8) * animPoseGetBaseData(pose)->mBufferVisibilityNum);
    pose->mpGroupVisibility = pose->mpBufferGroupVisibility;

    pose->mpBufferNode = testAlloc(sizeof(AnimPoseNode*) * animPoseGetBaseData(pose)->mBufferNodeNum);
    pose->mpNodes = testAlloc(sizeof(AnimPoseNode*) * animPoseGetBaseData(pose)->mBufferNodeNum);

    pose->mpBufferTexAnimEntries = testAlloc(sizeof(AnimTexMtx) * animPoseGetBaseData(pose)->mBufferTexAnimNum);
    pose->mpTexAnimEntries = pose->mpBufferTexAnimEntries;

    pose->lastFrameZero = -1;
}

// heavily inlined
void animPoseRefresh(void) {
    int i;

    initTestHeap(); // inline
    for (i = 0; i < wp->totalPoses; i++) {
        AnimationPose* pose = &wp->poses[i]; // inline
        if (pose->flags) {
            if ((!wp->inBattle || pose->type != 0) && (wp->inBattle || pose->type != 1)) {
                animPose_AllocBuffer(pose);
                pose->lastFrameZero = -1;
            }
        }
    }
}

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
  q = (void *) 0;
  id[0] = -1;
found:
  return q;
}
s32 animPoseEntry(const char *animName, s32 type)
{
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
  switch (type)
  {
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

  for (pc[0] = 0; pc[0] < wp->totalPoses; pc[0]++)
  {
    t[0] = &wp->poses[pc[0]];
    if (!t[0]->flags)
    {
      pose = t[0];
      poseId[0] = pc[0];
      goto label_1;
    }
  }
  pose = (void *) 0;
  poseId[0] = -1;
  label_1:
  pose->flags |= 1;

  pose->references = pose->typeFlags = fileId[0] = 0;
  count = wp->totalFiles;
  for (; fileId[0] < count; fileId[0]++)
  {
    file = &wp->files[fileId[0]];
    if (file->active)
    {
      data = *file->handle->data;
      if (!strcmp(data->fileName, animName))
      {
        file->references++;
        goto label_5;
      }
    }
  }

  file = apeFreeFile(fileId);
  file->active = 1;

  file->references++;
  file->handle = fileAllocf(5, "%s/%s", "a", animName);
  handle = file->handle;
  if (!file->handle)
  {
    fileId[0] = -2;
    file->active = 0;
    file->references--;
  }
  else
  {
    data = *file->handle->data;
    if (!data)
    {
      fileFree(handle);
      fileId[0] = -2;
      file->active = 0;
      file->references--;
    }
    else
    {
      sprintf(textureName, "%s/%s-", "a", data->textureName);
      count = wp->totalTextures;
      for (pc[0] = 0; pc[0] < count; pc[0]++)
      {
        tex[0] = &wp->textures[pc[0]];
        if (*((u32 *) (&tex[0]->active)))
        {
          if (tex[0]->handle)
          {
            if (!strcmp(tex[0]->handle->filename, textureName))
            {
              // FAKE: dead int guard (COLOURING W), deleted after colouring; its division temps add the texture pointer/count neighbours
              gz = 0;
              if (gz != 0) pc[0] = (((s32)(AddrWord)tex[0] / count) / (count / (s32)(AddrWord)tex[0])) / ((pc[0] / count) / (count / pc[0]));
              tex[0]->references++;
              goto label_4;
            }
          }
        }
      }

      for (pc[0] = 0; pc[0] < wp->totalTextures; pc[0]++)
      {
        tt[0] = &wp->textures[pc[0]];
        if (!(*((u32 *) (&tt[0]->active))))
        {
          tex[0] = tt[0];
          goto label_3;
        }
      }
      tex[0] = (void *) 0;
      pc[0] = -1;
      label_3:
      tex[0]->active = 1;

      tex[0]->references++;
      tex[0]->handle = fileAlloc(textureName, 4);
      label_4:
      file->textureId = pc[0];
    }
  }
  label_5:
  pose->fileId = fileId[0];

  if (pose->fileId == (-2))
  {
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
  pose->gxCallback = (void *) 0;
  pose->disableDraw = 0;
  pose->materialFlag = 0;
  pose->materialLightFlag = 0;
  pose->materialColor = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
  pose->materialColor2 = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
  pose->paperScaleXZ = (pose->paperScaleY = 1.0f);
  pose->paperOfsY = 0.0f;
  pose->localTimeRate = 1.0f;
  PSMTXIdentity(pose->matrix);
  if (!strcmp(animName, "tst_vivi"))
  {
    pose->vivianType = 1;
    pose->vivianGroupNo = &vivihimoTailGroupNo[0];
  }
  else
    if (!strcmp(animName, "c_vivian"))
  {
    pose->vivianType = 1;
    pose->vivianGroupNo = &vivihimoTailGroupNo[1];
  }
  else
    if (!strcmp(animName, "c_maririn"))
  {
    pose->vivianType = 2;
    pose->vivianGroupNo = &vivihimoTailGroupNo[2];
  }
  else
    if (!strcmp(animName, "c_majyorin"))
  {
    pose->vivianType = 3;
    pose->vivianGroupNo = &vivihimoTailGroupNo[3];
  }
  else
  {
    pose->vivianType = 0;
    pose->vivianGroupNo = &_dummy;
  }
  pose->dispCallback = (void *) 0;
  pose->dispUserDataCallback = (void *) 0;
  return poseId[0];
}

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
    pose->typeFlags = 3;
    pose->references = 0;
    pose->flags |= 1;
    return i;
}

const char* animGetTexName(const char* name);

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
    model = fileAsyncf(5, NULL, "%s/%s", "a", animName);
    anim = fileAsyncf(4, NULL, "%s/%s-", "a", animGetTexName(animName));
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

void animPosePeraOn(s32 poseId) {
    wp->poses[poseId].flags |= 8;
}

void animPosePeraOff(s32 poseId) {
    wp->poses[poseId].flags &= ~8;
}

void animPosePaperPeraOn(s32 poseId) {
    AnimationPose* dst = &wp->poses[wp->poses[poseId].paperId];
    AnimationPose* src = &wp->poses[poseId];

    dst->flags |= 8;
    dst->peraRotY = src->peraRotY;
    dst->rotationY = src->rotationY;
    dst->peraTargetRotY = src->peraTargetRotY;
}

void animPosePaperPeraOff(s32 poseId) {
    wp->poses[wp->poses[poseId].paperId].flags &= ~8;
}

void animPoseSetLocalTimeRate(s32 poseId, f32 localTimeRate) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->localTimeRate = localTimeRate;
}

void animPoseSetLocalTime(s32 poseId, f32 localTimeFrames) {
    AnimationPose *pose, *pera;

    pose = &wp->poses[poseId];
    pose->localTime = animTimeGetTime(pose->type) - (u64)(16.666666f * localTimeFrames);
    pose->mLocalTimeInit = pose->localTime;
    if (pose->paperId != -1) {
        pera = &wp->poses[pose->paperId];
        if (!(pera->typeFlags & 2)) {
            pera->localTime = pose->localTime;
        }
    }
}

void animPoseSetStartTime(s32 poseId, OSTime startTime) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->localTime = startTime;
}

#define animPosePaperAnimId(pose) ((pose)->paperAnimIdx)
/* An anim name parameter holds a name (an address in main memory, negative as
 * an s32) or an anim index (non-negative). */
#define ANIM_NAME_INDEX(name) ((s32)(AddrWord)(name))
#define ANIM_INDEX_NAME(idx) ((const char*)(AddrWord)(idx))
#define animPoseGetBaseDataId(id) (*(AnimationPoseData**)(wp->files[wp->poses[id].fileId].handle->data))

inline s32 animPoseGetAnimIdx(AnimationPoseData* data, const char* name) {
    s32 i = ANIM_NAME_INDEX(name);
    s32 count = data->mAnimCount;

    if (i < 0) {
        for (i = 0; i < count; i++) {
            if (!strcmp(data->mpAnims[i].name, name)) {
                return i;
            }
        }
    }
    else if (i < count) {
        return i;
    }
    return 0;
}

void animPoseSetAnim(s32 poseId, const char* animName, BOOL reset) {
    AnimationPose* pose;
    s32 animId;

    pose = &wp->poses[poseId];
    animId = animPoseGetAnimIdx(animPoseGetBaseData(pose), animName);
    if (reset || animId != pose->animId) {
        pose->animId = animId;
        if (pose->paperId != -1 && pose->flags & 2) {
            pose->flags |= 4;
        }
        pose->localTime = animTimeGetTime(pose->type);
        pose->lastFrameZero = -1;
        pose->lastFrameTime = 0.0f;
        pose->curKeyValue = 0;
        pose->loopTime = 0.0f;
    }
}

s32 animPaperPoseGetId(const char* animName, s32 group) {
    AnimationPose* pose;
    s32 i;

    for (i = 0; i < wp->totalPoses; i++) {
        pose = &wp->poses[i];
        if (pose->flags & 1 && pose->typeFlags & 1 && pose->typeFlags & 2 && pose->type == group &&
            !strcmp(animPoseGetBaseData(pose)->fileName, animName)) {
            break;
        }
    }
    if (i == wp->totalPoses) {
        i = -1;
    }
    return i;
}

void animPoseSetPaperAnimGroup(s32 poseId, const char* animName, BOOL someflag) {
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);

    if (!animName) {
        pose->paperId = -1;
        animPoseSetMaterialLightFlagOff(poseId, 1);
        return;
    }
    pose->paperId = animPaperPoseGetId(animName, pose->type);
    if (pose->paperId < 0) {
        pose->paperId = animPaperPoseGetId(animName, 2);
    }
    animPosePaperAnimId(pose) = -1;
    pose->mLocalTimeInit = animTimeGetTime(pose->type);
    if (someflag) {
        pose->flags |= 2;
        pose->flags |= 4;
    }
    else {
        pose->flags &= ~2;
        pose->flags &= ~4;
    }
    animPoseSetMaterialLightFlagOn(poseId, 1);
}

void animPoseSetPaperAnim(s32 poseId, const char* animName, BOOL reset) {
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);
    AnimationPoseData* data;
    s32 id;
    s32 idx;

    data = animPoseGetAnimBaseDataPtr(pose->paperId);
    idx = animPoseGetAnimIdx(data, animName);
    animPosePaperAnimId(pose) = idx;
    pose->mLocalTimeInit = animTimeGetTime(pose->type);
    id = pose->paperId;
    if (id != -1 && !(*(u32*)((u8*)wp->poses + id * sizeof(AnimationPose) + offsetof(AnimationPose, typeFlags)) & 2)) { // FAKE: byte-offset load so the typeFlags offset folds into the index (mulli/addi/lwzx), as in the target
        animPoseMain(pose->paperId);
    }
}

void animPoseFileRelease(s32 fileId);

inline s32 animEffectPoseEntry(const char* animName, s32 group) {
    s32 i;
    AnimationPose* pose;

    i = animPoseEntry(animName, group);
    if (i == -2) {
        return i;
    }
    pose = animPoseGetAnimPosePtr(i);
    pose->typeFlags = 1;
    pose->references = 0;
    pose->flags |= 1;
    return i;
}

void animPoseSetEffect(s32 poseId, const char* animName, BOOL someflag) {
    AnimationPose* paper;
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);

    if (pose->paperId != -1) {
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
        animPoseSetMaterialLightFlagOff(poseId, 1);
        return;
    }
    pose->paperId = animEffectPoseEntry(animName, pose->type);
    animPosePaperAnimId(pose) = -1;
    pose->mLocalTimeInit = animTimeGetTime(pose->type);
    if (someflag) {
        pose->flags |= 2;
        pose->flags |= 4;
    }
    else {
        pose->flags &= ~2;
    }
    animPoseSetMaterialLightFlagOn(poseId, 1);
}

void animPoseSetEffectAnim(s32 poseId, const char* animName, BOOL reset) {
    AnimationPose* pose = &wp->poses[poseId];

    if (pose->paperId != -1) {
        animPoseSetAnim(pose->paperId, animName, reset);
    }
}

void animPoseSetGXFunc(s32 poseId, void (*callback)(s32 wXluStage), BOOL disableDraw) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->gxCallback = callback;
    pose->disableDraw = disableDraw;
}

f32 animPoseGetRadius(s32 poseId) {
    return 2.0f * animPoseGetAnimBaseDataPtr(poseId)->radius;
}

f32 animPoseGetHeight(s32 poseId) {
    return 2.0f * animPoseGetAnimBaseDataPtr(poseId)->height;
}

f32 animPoseGetLoopTimes(s32 id) {
    AnimationPose* pose = &wp->poses[id];
    return pose->loopTime;
}

BOOL animPoseGetPeraEnd(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];

    if (!(pose->flags & 8)) {
        return TRUE;
    }
    if (__fabs(pose->peraTargetRotY - pose->rotationY) < 10.0) {
        return TRUE;
    }
    return FALSE;
}

void animPoseSetMaterialFlagOn(s32 poseId, u32 mask) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialFlag |= mask;
}

void animPoseSetMaterialFlagOff(s32 poseId, u32 mask) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialFlag &= ~mask;
}

void animPoseSetMaterialLightFlagOn(s32 poseId, u32 mask) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialLightFlag |= mask;
}

void animPoseSetMaterialLightFlagOff(s32 poseId, u32 mask) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialLightFlag &= ~mask;
}

void animPoseSetMaterialEvtColor(s32 poseId, GXColor color) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->materialColor = color;
}

u32 animPoseGetMaterialFlag(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    return pose->materialFlag;
}

u32 animPoseGetMaterialLightFlag(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    return pose->materialLightFlag;
}

GXColor animPoseGetMaterialEvtColor(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    return pose->materialColor;
}

// FAKE: t3 = t * t2 through an inline so t3 is an inline return temp (numbered above the LICM temp of t2),
// giving the target FPR order (t3 f7, t2 f8). Behaviour-identical.
static inline f32 apmMul(f32 a, f32 b) { return a * b; }
void animPoseMain(s32 poseId) {
    s32 frame;
    s32 nA;
    s32 nB;
    s32 nD;
    s32 nextFrame;
    AnimKeyframe* kf;
    AnimKeyframe* kfEnd;
    s32 i;
    AnimationPoseData* file;
    AnimData* anim;
    AnimationPose* poses;
    u8* grpB;
    u8* grpA;
    u8* q;
    f32 key;
    u8* v;
    u8* d;
    f32 x;
    f32 y;
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
    u8* e;
    f32 s;
    f32 t2;
    f32 t3;
    u8* p;
    s64 ms;

    poses = wp->poses;
    pose = &poses[poseId];
    file = animPoseGetBaseData(pose);
    anim = file->mpAnims[pose->animId].data;
    OSInitFastCast();
    if ((s32)wp->mbUseFloatScratch != 0) {
        grpA = (u8*)wp->vtxArrayPos;
        grpB = (u8*)wp->vtxArrayNrm;
    } else {
        grpA = (u8*)pose->mpVtxArrayPos;
        grpB = (u8*)pose->mpVtxArrayNrm;
    }
    if (pose->paperId != -1 && !(((AnimationPose*)((u8*)poses + pose->paperId * sizeof(AnimationPose)))->typeFlags & 2)) { // FAKE: the paper pose found by byte offset (base + id * size) so the typeFlags offset folds into the index (mulli/addi/lwzx), as in the target
        animPoseMain(pose->paperId);
    }
    if (anim == NULL) return;
    loop = *(s32*)anim->loopInfo;
    if (pose->type == 0) {
        ms = OSTicksToMilliseconds(gp->renderFieldTime);
    } else {
        ms = OSTicksToMilliseconds(gp->renderTime);
    }
    tm = (f32)(u64)(ms - pose->localTime);
    pose->rawFrame = 60.0f * tm / 1000.0f;
    pose->rawFrame *= pose->localTimeRate;
    cur = pose->rawFrame;
    f = anim->loopInfo[2];
    start = anim->loopInfo[1];
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
    for (i = 0; i < cnt; q += 0x2c, i++) {
        if (*(f32*)q > key) break;
    }
    pose->keyframeIdx = i - 1;
    pose->nextKeyframeIdx = pose->keyframeIdx + 1;
    key = pose->localFrame;
    q = (u8*)anim->valueKeys;
    cnt = anim->valueKeyCount;
    for (i = 0; i < cnt; q += 8, i++) {
        if (*(f32*)q > key) break;
    }
    pose->valueKeyIdx = i - 1;
    pose->nextValueKeyIdx = pose->valueKeyIdx + 1;
    if (loop == 0) {
        if (pose->nextKeyframeIdx >= anim->keyframeCount) pose->nextKeyframeIdx = pose->keyframeIdx;
        if (pose->nextValueKeyIdx >= anim->valueKeyCount) pose->nextValueKeyIdx = pose->valueKeyIdx;
    }
    pose->curKeyValue = anim->valueKeys[pose->valueKeyIdx].value;
    start = anim->keyframes[pose->keyframeIdx].time;
    tm = pose->localFrame - start;
    if (loop != 0) {
        if (pose->nextKeyframeIdx >= anim->keyframeCount) {
            pose->nextKeyframeIdx = 0;
            len = anim->loopInfo[2] - anim->keyframes[pose->keyframeIdx].time;
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
    nA = file->mBufferPosNum;
    nB = file->mBufferNrmNum;
    nC = file->mBufferVisibilityNum;
    nD = file->mBufferNodeNum;
    nE = file->mBufferTexAnimNum;
    if (pose->lastFrameZero != frame) {
        if (pose->lastFrameZero == -1 || pose->lastFrameZero > frame) {
            kfEnd = &anim->keyframes[frame] + 1;
            kf = anim->keyframes;
            if (nA != 0) memcpy_as4((u8*)pose->mpBufferVtxPos, (u8*)file->bufferVtxPos, nA * sizeof(Vec));
            if (nB != 0) memcpy_as4((u8*)pose->mpBufferVtxNrm, (u8*)file->nrmVtxBuffer, nB * sizeof(Vec));
            if (nC != 0) memcpy(pose->mpBufferGroupVisibility, file->groupVisibility, nC);
            if (nD != 0) memcpy_as4((u8*)pose->mpBufferNode, (u8*)file->nodeData, nD * 4);
            if (nE != 0) memcpy_as4((u8*)pose->mpBufferTexAnimEntries, (u8*)file->texAnimEntries, nE * sizeof(AnimTexMtx));
            if (pose->lastFrameZero == -1) pose->lastFrameZero = 0;
            s = 0.0625f;
            for (; kf < kfEnd; kf++) {
                { u8* d; u8* e;
                v = (u8*)pose->mpBufferVtxPos;
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
                }
                }
                { u8* d; u8* e;
                v = (u8*)pose->mpBufferVtxNrm;
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
                }
                }
                { u8* d; u8* e; s8* v;
                v = pose->mpBufferGroupVisibility;
                d = anim->visDeltas + kf->visStart * 2;
                e = d + kf->visCount * 2;
                for (; d < e; d += 2) {
                    v += d[0];
                    v[0] += d[1];
                }
                }
                { u8* d; u8* e;
                v = (u8*)pose->mpBufferNode;
                d = anim->nodeDeltas + kf->nodeStart * 4;
                e = d + kf->nodeCount * 4;
                for (; d < e; d += 4) {
                    v += d[0] * 4;
                    x = __OSs8tof32((s8*)(d + 1));
                    *(f32*)v += s * x;
                }
                }
                { u8* d; u8* e;
                v = (u8*)pose->mpBufferTexAnimEntries;
                d = (u8*)&anim->texDeltas[kf->texStart];
                e = d + kf->texCount * sizeof(AnimTexDelta);
                for (; d < e; d += sizeof(AnimTexDelta)) {
                    v += d[0] * sizeof(AnimTexMtx);
                    v[0] += d[1];
                    ((AnimTexMtx*)v)->mTransS += ((AnimTexDelta*)d)->transS;
                    ((AnimTexMtx*)v)->mTransT += ((AnimTexDelta*)d)->transT;
                }
                }
            }
        } else if (pose->lastFrameZero < frame) {
            kf = &anim->keyframes[pose->lastFrameZero + 1];
            kfEnd = &anim->keyframes[frame + 1];
            s = 0.0625f;
            for (; kf < kfEnd; kf++) {
                { u8* d; u8* e;
                v = (u8*)pose->mpBufferVtxPos;
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
                }
                }
                { u8* d; u8* e;
                v = (u8*)pose->mpBufferVtxNrm;
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
                }
                }
                { u8* d; u8* e; s8* v;
                v = pose->mpBufferGroupVisibility;
                d = anim->visDeltas + kf->visStart * 2;
                e = d + kf->visCount * 2;
                for (; d < e; d += 2) {
                    v += d[0];
                    v[0] += d[1];
                }
                }
                { u8* d; u8* e;
                v = (u8*)pose->mpBufferNode;
                d = anim->nodeDeltas + kf->nodeStart * 4;
                e = d + kf->nodeCount * 4;
                for (; d < e; d += 4) {
                    v += d[0] * 4;
                    x = __OSs8tof32((s8*)(d + 1));
                    *(f32*)v += s * x;
                }
                }
                { u8* d; u8* e;
                v = (u8*)pose->mpBufferTexAnimEntries;
                d = (u8*)&anim->texDeltas[kf->texStart];
                e = d + kf->texCount * sizeof(AnimTexDelta);
                for (; d < e; d += sizeof(AnimTexDelta)) {
                    v += d[0] * sizeof(AnimTexMtx);
                    v[0] += d[1];
                    ((AnimTexMtx*)v)->mTransS += ((AnimTexDelta*)d)->transS;
                    ((AnimTexMtx*)v)->mTransT += ((AnimTexDelta*)d)->transT;
                }
                }
            }
        }
    } else {
        if (pose->lastFrameTime > t) pose->lastFrameTime = 0.0f;
    }
    if (frame == nextFrame) {
        if (nA != 0) memcpy_as4(grpA, (u8*)pose->mpBufferVtxPos, nA * sizeof(Vec));
        if (nB != 0) memcpy_as4(grpB, (u8*)pose->mpBufferVtxNrm, nB * sizeof(Vec));
        if (nD != 0) memcpy_as4((u8*)pose->mpNodes, (u8*)pose->mpBufferNode, nD * 4);
    } else if (frame <= nextFrame) {
        AnimKeyframe* kf;
        kf = &anim->keyframes[nextFrame];
        { u8* d; u8* e;
        s = 0.0625f * t;
        v = grpA;
        d = anim->vtxPosDeltas + kf->vtxPosStart * 4;
        e = d + kf->vtxPosCount * 4;
        p = (u8*)pose->mpBufferVtxPos;
        if (nA != 0) memcpy_as4(grpA, p, nA * sizeof(Vec));
        for (; d < e; d += 4) {
            v += d[0] * sizeof(Vec);
            x = __OSs8tof32((s8*)(d + 1));
            y = __OSs8tof32((s8*)(d + 2));
            z = __OSs8tof32((s8*)(d + 3));
            ((Vec*)v)->x += x * s;
            ((Vec*)v)->y += y * s;
            ((Vec*)v)->z += z * s;
        }
        }
        { u8* d; u8* e; u8* v;
        v = grpB;
        d = anim->vtxNrmDeltas + kf->vtxNrmStart * 4;
        e = d + kf->vtxNrmCount * 4;
        p = (u8*)pose->mpBufferVtxNrm;
        if (nB != 0) memcpy_as4(grpB, p, nB * sizeof(Vec));
        for (; d < e; d += 4) {
            v += d[0] * sizeof(Vec);
            x = __OSs8tof32((s8*)(d + 1));
            y = __OSs8tof32((s8*)(d + 2));
            z = __OSs8tof32((s8*)(d + 3));
            ((Vec*)v)->x += x * s;
            ((Vec*)v)->y += y * s;
            ((Vec*)v)->z += z * s;
        }
        }
        { u8* d; u8* e; u8* v;
        d = anim->nodeDeltas + kf->nodeStart * 4;
        e = d + kf->nodeCount * 4;
        dt = anim->keyframes[nextFrame].time - anim->keyframes[frame].time;
        p = (u8*)pose->mpBufferNode;
        v = (u8*)pose->mpNodes;
        if (nD != 0) memcpy_as4(v, p, nD * 4);
        for (; d < e; d += 4) {
            b = (s8)d[3];
            v += d[0] * 4;
            if (b < 0x59 && b > -0x59) {
                f32 h;
                x = __OSs8tof32((s8*)(d + 1));
                t2 = t * t;
                t3 = apmMul(t, t2);
                h = -2.0f * t3 + 3.0f * t2;
                *(f32*)v = *(f32*)v + ((t + (t3 - 2.0f * t2)) * (dt * tanfTbl[(s8)d[2] + 0x59]) +
                                h * (0.0625f * x) +
                                (t3 - t2) * (dt * tanfTbl[b + 0x59]));
            }
        }
        }
    }
    pose->lastFrameZero = frame;
    pose->lastFrameTime = t;
    if (pose->flags & 8) {
        CameraEntry* cam = camGetPtr(CAMERA_3D);
        f = 57.29579f * (f32)atan2(-(cam->cameraAt.x - cam->cameraPos.x), -(cam->cameraAt.z - cam->cameraPos.z));
        while (f < -360.0f) f += 360.0f;
        while (f >= 360.0f) f -= 360.0f;
        if (f < 0.0f) f += 360.0f;
        f -= pose->rotationY;
        while (f < -360.0f) f += 360.0f;
        while (f >= 360.0f) f -= 360.0f;
        if ((f > 90.0f && f <= 270.0f) || (f < -90.0f && f >= -270.0f)) {
            pose->scaleZFlip = -1.0f;
        } else {
            pose->scaleZFlip = 1.0f;
        }
    }
}

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
    tx = node->mTranslation.x;
    ty = node->mTranslation.y;
    tz = node->mTranslation.z;
    sx = node->mScale.x;
    sy = node->mScale.y;
    sz = node->mScale.z;
    rx = 2.0f * node->mRot1.x;
    ry = 2.0f * node->mRot1.y;
    rz = 2.0f * node->mRot1.z;
    rcx = node->mRotationCenter.x;
    rcy = node->mRotationCenter.y;
    rcz = node->mRotationCenter.z;
    scx = node->mScaleCenter.x;
    scy = node->mScaleCenter.y;
    scz = node->mScaleCenter.z;
    rtx = node->mRotationTranslation.x;
    rty = node->mRotationTranslation.y;
    rtz = node->mRotationTranslation.z;
    stx = node->mScaleTranslation.x;
    sty = node->mScaleTranslation.y;
    stz = node->mScaleTranslation.z;

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
        }
        else if (stx || sty || stz) {
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
        }
        else if (rtx || rty || rtz) {
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
void pushGXModelMtx_JointNode__(AnimPoseNode* node, f32 sx, f32 sy, f32 sz) {
    Mtx trans, scale, iscale, rot, tmp;
    MtxPtr p;
    u32 flags;
    f32 tx, ty, tz;
    f32 nsx, nsy, nsz;
    f32 r1x, r1y, r1z;
    f32 r2x, r2y, r2z;

    tx = node->mTranslation.x;
    ty = node->mTranslation.y;
    tz = node->mTranslation.z;
    nsx = node->mScale.x;
    nsy = node->mScale.y;
    nsz = node->mScale.z;
    r1x = 2.0f * node->mRot1.x;
    r1y = 2.0f * node->mRot1.y;
    r1z = 2.0f * node->mRot1.z;
    r2x = node->mRot2.x;
    r2y = node->mRot2.y;
    r2z = node->mRot2.z;
    flags = 0;

    if (tx || ty || tz) {
        PSMTXTrans(trans, tx, ty, tz);
        flags |= 1;
    }
    if (1.0f != sx || 1.0f != sy || 1.0f == sz) {
        if (sx && sy && sz) {
            PSMTXScale(iscale, 1.0f / sx, 1.0f / sy, 1.0f / sz);
        } else {
            PSMTXScale(iscale, 1.0f, 1.0f, 1.0f);
        }
        flags |= 2;
    }
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
        case 0: PSMTXIdentity(tmp); p = tmp; break;
        case 1: p = trans; break;
        case 2: p = iscale; break;
        case 3: PSMTXConcat(trans, iscale, tmp); p = tmp; break;
        case 4: p = scale; break;
        case 5: PSMTXConcat(trans, scale, tmp); p = tmp; break;
        case 6: PSMTXConcat(iscale, scale, tmp); p = tmp; break;
        case 7: PSMTXConcat(trans, iscale, tmp); PSMTXConcat(tmp, scale, trans); p = trans; break;
        case 8: p = rot; break;
        case 9: PSMTXConcat(trans, rot, tmp); p = tmp; break;
        case 10: PSMTXConcat(iscale, rot, tmp); p = tmp; break;
        case 11: PSMTXConcat(trans, iscale, tmp); PSMTXConcat(tmp, rot, trans); p = trans; break;
        case 12: PSMTXConcat(rot, scale, tmp); p = tmp; break;
        case 13: PSMTXConcat(trans, rot, tmp); PSMTXConcat(tmp, scale, trans); p = trans; break;
        case 14: PSMTXConcat(iscale, rot, tmp); PSMTXConcat(tmp, scale, trans); p = trans; break;
        case 15: PSMTXConcat(trans, iscale, tmp); PSMTXConcat(tmp, rot, trans); PSMTXConcat(trans, scale, tmp); p = tmp; break;
    }
    PSMTXConcat(g_modeling_mtx_stack[g_modeling_mtx_lv - 1], p, g_modeling_mtx_stack[g_modeling_mtx_lv]);
}

void animSetMaterial_Texture(s32 texCount_, s32 *texIdRemap_, AnimPoseTexEntry *pTexEntries_, AnimTexMtx *animEntries, s32 texBindsCapacity, AnimTexBind *texBinds, AnimationTextureFile *texFile)
{
  s32 texCount;
  s32 * texIdRemap;
  AnimPoseTexEntry * pTexEntries;
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
  // FAKE: texCount/texIdRemap/pTexEntries copied into locals (kept as separate webs by the dead guard before loop 2); the incoming
  // param nodes then neighbour the other params and m but not the counter, which with the counter as a one-element array
  // (IRO @ temp, numbered above m) makes the counter colour last (r23) and the 7 params r24..r30 in order, as in the target
  texCount = texCount_;
  texIdRemap = texIdRemap_;
  pTexEntries = pTexEntries_;
  for (i[0] = 0; i[0] < texCount; i[0]++)
  {
    s32 tex2;
    AnimPoseTexEntry *entry;
    s32 id;
    id = texIdRemap[(u32) ((texCount - i[0]) - 1)];
    entry = &pTexEntries[id];
    bindId = entry->bindId + animEntries[id].mTexBindIdx;
    if ((bindId < 0) || (bindId >= texBindsCapacity))
    {
      bindId = 0;
            printf("Error: dispSetTevMode: \203x\201[\203X\203\202\203f\203\213%s \203A\203j\203\201 %s \223\340\202\305 \203t\203\214\201[\203\200\203G\203N\203X\203e\203\223\203V\203\207\203\223\202\314\220\335\222\350\202\252\215s\202\355\202\352\202\304\202\242\202\334\202\267\202\252\201A\212Y\223\226\202\267\202\351\203e\203N\203X\203`\203\203\202\252\202\240\202\350\202\334\202\271\202\361 \nframeExt\224\324\215\206 %d? \202\252\226\263\202\242\202\346\202\244\202\305\202\267\201B\203\202\203f\203\213\202\334\202\275\202\315\203A\203j\203\201\202\311\217C\220\263\202\252\225K\227v\202\305\202\267\201BtextureIdx=%d\n",
                   animPoseGetBaseData(wp->mpCurPose)->fileName,
                   animPoseGetBaseData(wp->mpCurPose)->mpAnims[wp->mpCurPose->animId].name,
                   animEntries[id].mTexBindIdx, entry->bindId); // correctness: target passes frameExt (r6) and textureIdx (r7) for the two %d
    }
    wrapFlags = entry->wrapFlags;
    texId = texBinds[bindId].texId;
    if (texBinds[bindId].texType != 0)
    {
      continue;
    }
    tex2 = texId; // FAKE: s32 copy after the continue: texId loads into r4 for the first TEXGet, then mr r19,r4 as in the target
    if (TEXGet(*((TEXPalette **) texFile->handle->data), texId)->CLUTHeader)
    {
      desc = TEXGet(*((TEXPalette **) texFile->handle->data), tex2);
      hdr = desc->textureHeader;
      wrapS = hdr->wrapS;
      wrapT = hdr->wrapT;
      if (wrapFlags >= 0)
      {
        if (wrapFlags & 4)
        {
          wrapS = GX_MIRROR;
        }
        else
        {
          wrapS = wrapFlags;
          wrapS = wrapS & 1;
        }
        if (wrapFlags & 8)
        {
          wrapT = GX_MIRROR;
        }
        else
        {
          wrapT = (wrapFlags >> 1) & 1;
        }
      }
      mipmap = hdr->minLOD != hdr->maxLOD;
      GXInitTlutObj(&tlut, desc->CLUTHeader->data, desc->CLUTHeader->format, desc->CLUTHeader->numEntries);
      GXInitTexObjCI(&obj, desc->textureHeader->data, desc->textureHeader->width, desc->textureHeader->height, (GXCITexFmt) desc->textureHeader->format, wrapS, wrapT, mipmap, 0);
      hdr = desc->textureHeader;
      GXInitTexObjLOD(&obj, hdr->minFilter, hdr->magFilter, hdr->minLOD, hdr->maxLOD, hdr->LODBias, (GXBool) 0, hdr->edgeLODEnable, GX_ANISO_1);
      GXLoadTlut(&tlut, 0);
      GXLoadTexObj(&obj, (GXTexMapID) i[0]);
    }
    else
    {
      desc = TEXGet(*((TEXPalette **) texFile->handle->data), tex2);
      hdr = desc->textureHeader;
      wrapS = hdr->wrapS;
      wrapT = hdr->wrapT;
      if (wrapFlags >= 0)
      {
        if (wrapFlags & 4)
        {
          wrapS = GX_MIRROR;
        }
        else
        {
          wrapS = wrapFlags;
          wrapS = wrapS & 1;
        }
        if (wrapFlags & 8)
        {
          wrapT = GX_MIRROR;
        }
        else
        {
          wrapT = (wrapFlags >> 1) & 1;
        }
      }
      GXInitTexObj(&obj, hdr->data, hdr->width, hdr->height, (GXTexFmt) hdr->format, wrapS, wrapT, hdr->minLOD != hdr->maxLOD);
      hdr = desc->textureHeader;
      GXInitTexObjLOD(&obj, hdr->minFilter, hdr->magFilter, hdr->minLOD, hdr->maxLOD, hdr->LODBias, (GXBool) 0, hdr->edgeLODEnable, GX_ANISO_1);
      GXLoadTexObj(&obj, (GXTexMapID) i[0]);
    }
  }

  // FAKE: dead int guard (no code after RA) keeps the three copies from being propagated away
  gz = 0;
  if (gz != 0)
  {
    texCount = 0;
    texIdRemap = 0;
    pTexEntries = 0;
  }
  for (i[0] = 0; i[0] < texCount; i[0]++)
  {
    flags = 0;
    anim = &animEntries[texIdRemap[(u32) ((texCount - i[0]) - 1)]];
    rotate = anim->mRotate;
    scaleS = anim->mScaleS;
    scaleT = anim->mScaleT;
    transS = anim->mTransS;
    transT = anim->mTransT;
    if (rotate != 0.0f)
    {
      flags |= 1;
      PSMTXTrans(tmp, 0.5f, 0.5f, 0.0f);
      PSMTXRotRad(rot, 'z', 0.017453292f * (-rotate));
      PSMTXConcat(tmp, rot, trans);
      PSMTXTrans(tmp, -0.5f, -0.5f, 0.0f);
      PSMTXConcat(trans, tmp, rot);
    }
    if (((transS != 0.0f) || (transT != 0.0f)) || (scaleT != 0.0f))
    {
      flags |= 2;
      PSMTXTrans(trans, transS, (1.0f - transT) - scaleT, 0.0f);
    }
    if ((scaleS != 1.0f) || (scaleT != 1.0f))
    {
      flags |= 4;
      PSMTXScale(scale, scaleS, scaleT, 1.0f);
    }
    switch (flags)
    {
      case 0:
        GXSetTexCoordGen2((GXTexCoordID) i[0], GX_TG_MTX2x4, (GXTexGenSrc) (GX_TG_TEX0 + i[0]), GX_IDENTITY, (GXBool) 0, GX_PTIDENTITY);
        continue;

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

    }

    j = (u32)i[0] * 3; // FAKE: unsigned multiply (i >= 0, same value) shifts loop-2 colouring toward target
    texMtx = GX_TEXMTX0 + j;
    GXSetTexCoordGen2((GXTexCoordID) i[0], GX_TG_MTX2x4, (GXTexGenSrc) (GX_TG_TEX0 + i[0]), texMtx, (GXBool) 0, GX_PTIDENTITY);
    GXLoadTexMtxImm(m, texMtx, GX_MTX2x4);
  }

}

inline BOOL animSetMaterial_ChangeTexture(AnimationPoseData* data, AnimPoseShapeDraw* draw, AnimationPose* pose, GXTexObj* texObj, void** texData, Mtx mtx, s32 texType) {
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
    texAnim = pose->mpTexAnimEntries;
    texEntry = data->texData;
    texCount = draw->texCount;

    for (i = 0; i < texCount; i++) {
        dataId = texCount - i - 1;
        dataId = draw->texDataIds[dataId];
        frameExt = texAnim[dataId].mTexBindIdx;
        bindId = texEntry[dataId].bindId + texAnim[dataId].mTexBindIdx;
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
                    GXLoadTexMtxImm(mtx, GX_TEXMTX0 + (u32)(i * 3), GX_MTX2x4);
                    GXSetTexCoordGen(i, GX_TG_MTX2x4, GX_TG_TEX0 + i, GX_TEXMTX0 + (u32)(i * 3));
                } else {
                    GXSetTexCoordGen(i, GX_TG_MTX2x4, GX_TG_TEX0 + i, GX_IDENTITY);
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}

static s32 tbl[4] = { 2, 1, 3, 0 };  // target g5_tbl_s1109 (g5_tbl_s1109 in the stopgap)

void materialProc(s32 shapeId, s32 xluStage)
{
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
  if (shapeId != (-1))
  {
    camera = camGetCurPtr();
    PSMTXConcat(camera->view, *g_modeling_mtx, camMtx);
    GXLoadPosMtxImm(camMtx, 0);
    PSMTXInvXpose(camMtx, invCamMtx);
    GXLoadNrmMtxImm(invCamMtx, 0);
    GXSetCurrentMtx(0);
    shape = &wp->mpCurPoseData->shapes[shapeId];
    dispMode = shape->dispMode;
    drawCount = shape->shapeDrawCount;
    draw = &wp->mpCurPoseData->draws[shape->shapeDrawStart];
    if ((dispMode == 0 && wp->currDispMode == 2) || dispMode == wp->currDispMode || wp->mpCurPose->materialColor.a != 0xFF || wp->mpCurPose->materialColor2.a != 0xFF)
    {
      GXSetCullMode(tbl[shape->cullMode]);
      for (i = 0; i < drawCount; i++, draw++)
      {
        s32 texCount = draw->texCount;
        if ((u32)wp->mpCurPose->disableDraw == 0)
        {
          lightPos = (Vec){0.0f, 0.0f, 0.0f};
          PSMTXMultVec(*g_modeling_mtx, &lightPos, &lightPos);
          mapSetMaterialLight(wp->mpCurPose->materialLightFlag, lightPos);
          animSetMaterial_Texture(draw->texCount, draw->texDataIds, wp->mpCurPoseData->texData, wp->mpCurPose->mpTexAnimEntries, wp->mpCurPoseData->texBindCount, wp->mpCurPoseData->texBinds, &wp->textures[wp->files[wp->mpCurPose->fileId].textureId]);
          mapSetMaterialLastStageBlend(wp->mpCurPose->materialFlag, wp->mpCurPose->materialColor, wp->mpCurPose->materialColor2);
          camera = camGetPtr(CAMERA_3D);
          if (camGetCurPtr() == camera)
          {
            mapSetMaterialFog();
          }
          mapSetMaterialTev(texCount, draw->drawMode, wp->mpCurPose->materialFlag, *g_modeling_mtx);
        }
        pose = wp->mpCurPose;
        if ((u32)pose->typeFlags != 0)
        {
          cd = wp->mpCurPoseData;
          texChange1 = animSetMaterial_ChangeTexture(cd, draw, pose, wp->mpTexObjs[0], wp->mpTexDatas[0], *wp->paperMtxPtr[0], 1);
          check1 = texChange1;
          if (!texChange1)
          {
            cd = wp->mpCurPoseData;
            check1 = animSetMaterial_ChangeTexture(cd, draw, wp->mpCurPose, wp->mpTexObjs[1], wp->mpTexDatas[1], *wp->paperMtxPtr[1], 2);
          }
          if (check1 && (!draw->drawMode))
          {
            currTexObj = wp->mpCurTexObj;
            if (currTexObj)
            {
              smartTexObj(currTexObj, wp->mpCurTexData);
              if (wp->currDispMode != 3)
              {
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                GXSetZCompLoc(GX_FALSE);
                GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
              }
              texObj = wp->mpTexObjs[texChange1 == 0];
              if (texObj)
              {
                GXLoadTexObj(texObj, GX_TEXMAP0);
                GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
                GXLoadTexMtxImm(*wp->paperMtxPtr[!texChange1], GX_TEXMTX0, GX_MTX2x4);
              }
              GXLoadTexObj(currTexObj, GX_TEXMAP1);
              GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
              GXLoadTexMtxImm(*wp->paperMtxPtr[2], GX_TEXMTX1, GX_MTX2x4);
              mapSetMaterialTev(2, 12, wp->mpCurPose->materialFlag, *g_modeling_mtx);
            }
          }
        }
        { // own pose web for the third ChangeTexture copy (a split web of pose numbers below cd)
        AnimationPose *pose2;
        pose2 = wp->mpCurPose;
        if (pose2->flags & 0x10)
        {
          texObj = wp->mpTexObjs[0];
          if (texObj)
          {
            if (wp->paperMtxPtr[0])
            {
              cd = wp->mpCurPoseData;
              animSetMaterial_ChangeTexture(cd, draw, pose2, texObj, wp->mpTexDatas[0], *wp->paperMtxPtr[0], 1);
            }
          }
        }
        }
      }

    }
  }
}

// note: I detect at least 8 inlined functions here, good luck finding the separation
// FAKE: renderProc's per-draw pose-data read as an inline (the source notes "inlined function here"); its call-site temp gives the target wp/data register order
static inline AnimationPoseData* rpCurData(void) { return wp->mpCurPoseData; }
void renderProc(s32 shapeId, s32 unused) {
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

    u16 *posIndexPtr, *nrmIndexPtr, *clr0IndexPtr, *texIndexPtr;

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
        camera = camGetCurPtr();
        MTXConcat(camera->view, *g_modeling_mtx, camMtx);
        GXLoadPosMtxImm(camMtx, 0);
        MTXInvXpose(camMtx, invCamMtx);
        GXLoadNrmMtxImm(invCamMtx, 0);
        GXSetCurrentMtx(0);

        data = wp->mpCurPoseData;
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
            wp->mpCurPose->materialColor.a != 0xFF || wp->mpCurPose->materialColor2.a != 0xFF) {
                f32* buffer;
                s32 firstDrawId;
                f32* texVtxIndices[8];
            GXSetCullMode(tbl[shape->cullMode]);
            data = wp->mpCurPoseData;
            clr0VtxSize = (s32)sizeof(GXColor) * clr0FirstVtx;
            buffer = data->texVtxBuffer;
            texVtxIndices[0] = &buffer[2 * shape->vtxTex[0].first];
            texVtxIndices[1] = &buffer[2 * shape->vtxTex[1].first];
            texVtxIndices[2] = &buffer[2 * shape->vtxTex[2].first];
            texVtxIndices[3] = &buffer[2 * shape->vtxTex[3].first];
            texVtxIndices[4] = &buffer[2 * shape->vtxTex[4].first];
            texVtxIndices[5] = &buffer[2 * shape->vtxTex[5].first];
            texVtxIndices[6] = &buffer[2 * shape->vtxTex[6].first];
            texVtxIndices[7] = &buffer[2 * shape->vtxTex[7].first];
            posVtxSize = (s32)sizeof(Vec) * posFirstVtx;
            nrmVtxSize = (s32)sizeof(Vec) * nrmFirstVtx;

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
                if ((u32)wp->mpCurPose->disableDraw == 0) {
                    lightPos = (Vec){ 0.0f, 0.0f, 0.0f };
                    MTXMultVec(*g_modeling_mtx, &lightPos, &lightPos);
                    lightPos2 = lightPos;
                    mapSetMaterialLight(wp->mpCurPose->materialLightFlag, lightPos2);
                    animSetMaterial_Texture(draw->texCount, draw->texDataIds, wp->mpCurPoseData->texData,
                                            wp->mpCurPose->mpTexAnimEntries, wp->mpCurPoseData->texBindCount,
                                            wp->mpCurPoseData->texBinds,
                                            &wp->textures[wp->files[wp->mpCurPose->fileId].textureId]);
                    mapSetMaterialLastStageBlend(wp->mpCurPose->materialFlag, wp->mpCurPose->materialColor,
                                                 wp->mpCurPose->materialColor2);
                    camera = camGetPtr(CAMERA_3D);
                    if (camGetCurPtr() == camera) {
                        mapSetMaterialFog();
                    }
                    mapSetMaterialTev(texCount, draw->drawMode, wp->mpCurPose->materialFlag, *g_modeling_mtx);
                }
                pose = wp->mpCurPose;
                if ((u32)pose->typeFlags != 0) {
                    GXTexObj* currTexObj;
                    cd = wp->mpCurPoseData;
                    texChange1 = animSetMaterial_ChangeTexture(cd, draw, pose, wp->mpTexObjs[0], wp->mpTexDatas[0], *wp->paperMtxPtr[0], 1);
                    check1 = texChange1;
                    if (!texChange1) {
                        cd = wp->mpCurPoseData;
                        pose = wp->mpCurPose;
                        texChange2 = animSetMaterial_ChangeTexture(cd, draw, pose, wp->mpTexObjs[1], wp->mpTexDatas[1], *wp->paperMtxPtr[1], 2);
                        check1 = texChange2;
                        check2 = texChange2;
                    } else {
                        check2 = FALSE;
                    }

                    if (check1 && (texChange1 && (s32)wp->paperTexMode0 == 0 || check2 && (s32)wp->paperTexMode1 == 0) && !draw->drawMode) {
                        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
                    }
                    if (check1 && !draw->drawMode) {
                        currTexObj = wp->mpCurTexObj;
                        if (currTexObj) {
                            smartTexObj(currTexObj, wp->mpCurTexData);
                            if (wp->currDispMode != 3) {
                                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
                                GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
                                GXSetZCompLoc(GX_FALSE);
                                GXSetAlphaCompare(GX_GEQUAL, 0x80u, GX_AOP_OR, GX_NEVER, 0);
                            }
                            texObj = wp->mpTexObjs[texChange1 == 0];
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
                            mapSetMaterialTev(2, 12, wp->mpCurPose->materialFlag, *g_modeling_mtx);
                        }
                    }
                }

                {
                AnimationPose* pose2;
                pose2 = wp->mpCurPose;
                if (pose2->flags & 0x10) {
                    texObj = wp->mpTexObjs[0];
                    if (texObj) {
                        buffer = (f32*)wp->paperMtxPtr[0]; // FAKE: the paper matrix pointer held in buffer (an earlier web) so it is a split @ web, not a named mtx local
                        if (buffer) {
                            cd = wp->mpCurPoseData;
                            animSetMaterial_ChangeTexture(cd, draw, pose2, texObj, wp->mpTexDatas[0], *(Mtx*)buffer, 1);
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

                if ((s32)wp->mbUseFloatScratch != 0) {
                    posVtxPtr = (Vec*)((u8*)wp->vtxArrayPos + posVtxSize);
                    nrmVtxPtr = (Vec*)((u8*)wp->vtxArrayNrm + nrmVtxSize);
                } else {
                    pose = wp->mpCurPose;
                    posVtxPtr = (Vec*)((u8*)pose->mpVtxArrayPos + posVtxSize);
                    nrmVtxPtr = (Vec*)((u8*)pose->mpVtxArrayNrm + nrmVtxSize);
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

                if (tc2 == 1) { // just use texIndex[0]
                    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
                    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
                    GXSetArray(GX_VA_TEX0, texVtxIndices[texIds[0]], sizeof(f32) * 2);
                    for (j = 0; j < drawCallCount; j++, drawCall++) {
                        u16 *posIndexPtr, *nrmIndexPtr, *clr0IndexPtr, *texIndexPtr;
                        firstDrawId = drawCall->first;
                        data = wp->mpCurPoseData;
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
                } else { // enumerate texIndex
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
                        data = wp->mpCurPoseData;
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
                                texIndex = texIds[(u32)(tc2 - index - 1)];
                                GXTexCoord1x16((&wp->mpCurPoseData->texIndex[texIndex][fd2 + firstId[texIndex]])[k2]);
                            }
                        }
                    }
                }
            }
        }
    }
}

// dispProc

void dispProc(s32 parentId, s32 groupId, s32 xluStage, s32 nextKeyframe, f32 blend) {
    AnimPoseGroup* group;
    AnimPoseGroup* parent;
    AnimPoseNode* node;
    AnimationPose* pose;
    AnimationPoseData* data;
    f32 sx, sy, sz;

    if (groupId == -1) {
        return;
    }
    pose = wp->mpCurPose;
    data = wp->mpCurPoseData;
    group = &data->groups[groupId];
    if (pose->mpGroupVisibility[group->visibilityId]) {
        pose = wp->mpCurPose;
        node = (AnimPoseNode*)((u32*)pose->mpNodes + group->nodeId);
        if (group->useSSC) {
            if (parentId == -1) {
                sx = sy = sz = 1.0f;
            }
            else {
                parent = &data->groups[parentId];
                if (!parent->useSSC) {
                    sx = sy = sz = 1.0f;
                }
                else {
                    AnimPoseNode* pnode = (AnimPoseNode*)((u32*)pose->mpNodes + parent->nodeId);
                    sx = pnode->mScale.x;
                    sy = pnode->mScale.y;
                    sz = pnode->mScale.z;
                }
            }
            pushGXModelMtx_JointNode__(node, sx, sy, sz);
        }
        else {
            pushGXModelMtx_TransformNode__(node);
        }
        if (wp->mpCurPose->flags & 0x40) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = (*g_modeling_mtx)[0][3];
            pos.y = (*g_modeling_mtx)[1][3];
            pos.z = (*g_modeling_mtx)[2][3];
            (*wp->mpCurPose->worldPositions)[groupId] = pos;
        }
        if (wp->mpCurPose->flags & 0x80) {
            PSMTXCopy(*g_modeling_mtx, (*wp->mpCurPose->worldMatrices)[groupId]);
        }
        pose = wp->mpCurPose;
        if (pose->flags & 0x20 && groupId == pose->vivianGroupNo[0]) {
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
                    offB = 0.5f;
                    break;
                case 2:
                    offA = -3.0f;
                    offB = 0.5f;
                    break;
                case 3:
                    offA = -1.0f;
                    offB = 0.5f;
                    break;
            }
            materialProc(group->shapeId, xluStage);
            vivihimo(*m, mat, speed[0], speed[1], offA, offB);
            wp->mpCurPose->flags &= ~0x20;
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    mat[i][j] += 0.04f * ((*m)[i][j] - mat[i][j]);
                }
            }
        }
        else {
            if (pose->dispCallback) {
                ((void (*)(void*, s32, Mtx*))pose->dispCallback)(pose->dispUserDataCallback, groupId, g_modeling_mtx);
            }
            renderProc(group->shapeId, xluStage);
            dispProc(groupId, group->childGroupId, xluStage, nextKeyframe, blend);
        }
        g_modeling_mtx_lv--;
        g_modeling_mtx = &g_modeling_mtx_stack[g_modeling_mtx_lv];
    }
    dispProc(groupId, group->brotherGroupId, xluStage, nextKeyframe, blend);
}

void animPoseDraw(s32 poseId, f32 x, f32 y, f32 z, f32 rotY, f32 scale, s32 xluMode) {
    Mtx matrix;

    MTXTrans(matrix, x, y, z);
    animPoseDrawMtx(poseId, matrix, xluMode, rotY, scale);
}

// _animPoseDrawMtx
void animPoseMainSub(AnimationPose* pose);

static inline f32 _animClampAngle(f32 a) {
    while (a < -360.0f) a += 360.0f;
    while (a >= 360.0f) a -= 360.0f;
    if (a < 0.0f) a += 360.0f;
    return a;
}

static inline f32 _animClampAngle2(f32 a) {
    while (a < -360.0f) a += 360.0f;
    while (a >= 360.0f) a -= 360.0f;
    return a;
}

static inline f32 _animCamAngle(void) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    return _animClampAngle(57.29579f * (f32)atan2(-(cam->cameraAt.x - cam->cameraPos.x), -(cam->cameraAt.z - cam->cameraPos.z)));
}

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
    anim = data->mpAnims[pose->animId].data;
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
    }
    else {
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
    if (pose->flags & 8) {
        rotY += -90.0f;
        pose->peraRotY = rotY;
        diff = _animClampAngle2(_animCamAngle() - pose->peraRotY);
        if ((diff > 90.0f && diff <= 270.0f) || (diff < -90.0f && diff >= -270.0f)) {
            pose->peraTargetRotY = _animClampAngle2(180.0f + _animCamAngle());
        }
        else {
            pose->peraTargetRotY = _animCamAngle();
        }
        while (pose->peraTargetRotY - pose->rotationY > 180.0f) {
            pose->rotationY += 360.0f;
        }
        while (pose->peraTargetRotY - pose->rotationY < -180.0f) {
            pose->rotationY -= 360.0f;
        }
        if (pose->flags & 0x100) {
            pose->rotationY = pose->peraTargetRotY;
        }
        else {
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
    }
    else if (rotY) {
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
    wp->mpCurPose = pose;
    wp->mpCurAnimData = anim;
    wp->mpCurPoseData = data;
    if ((s32)wp->mbUseFloatScratch != 0) {
        DCFlushRange(wp->vtxArrayPos, animPoseGetBaseData(pose)->mBufferPosNum * sizeof(Vec));
        DCFlushRange(wp->vtxArrayNrm, animPoseGetBaseData(pose)->mBufferNrmNum * sizeof(Vec));
    }
    else {
        DCFlushRange(pose->mpVtxArrayPos, animPoseGetBaseData(pose)->mBufferPosNum * sizeof(Vec));
        DCFlushRange(pose->mpVtxArrayNrm, animPoseGetBaseData(pose)->mBufferNrmNum * sizeof(Vec));
    }
    GXInvalidateVtxCache();
    dispProc(-1, data->groupCount - 1, keyframeIdx, nextKeyframeIdx, keyframeBlend);
}

inline void animSetPaperMtx(Mtx m0, Mtx* m1, Mtx* m2) {
    if (m0 == NULL) {
        wp->paperMtxPtr[0] = NULL;
    }
    else {
        PSMTXCopy(m0, wp->paperMtx[0]);
        wp->paperMtxPtr[0] = &wp->paperMtx[0];
    }
    wp->paperMtxPtr[1] = m1;
    wp->paperMtxPtr[2] = m2;
}

// FAKE: the float-scratch block of animPoseDrawMtx as an inline helper (same code as animPoseMainSub); the sum becomes an inline local, numbered/coloured before posNum as in the target
static inline void apdScratchN(s32 posNum, s32 nrmNum) {
  s32 n = posNum + nrmNum;
  if (n <= wp->mFloatScratchRP) {
    if (wp->mFloatScratchWP + n > wp->mFloatScratchRP) {
      wp->mFloatScratchWP = 0;
    }
    wp->mbUseFloatScratch = 1;
    wp->vtxArrayPos = &wp->mpFloatScratch[wp->mFloatScratchWP];
    wp->mFloatScratchWP += posNum;
    wp->vtxArrayNrm = &wp->mpFloatScratch[wp->mFloatScratchWP];
    wp->mFloatScratchWP += nrmNum;
  }
}
// FAKE: the C0 block of animPoseDrawMtx written as an inline helper; its locals become inline temps, which gives the target nonvolatile colours (m/data) and stack slots
static inline void apdEval(AnimationPose* pose, Mtx matrix, f32 rotY, f32 scale) {
  Mtx tmp;
  Mtx *m;
  Mtx rot;
  Mtx scl;
  AnimationPoseData *data;
  data = *((AnimationPoseData **) wp->files[pose->fileId].handle->data);
  m = (Mtx *) matrix;
  if ((!(pose->flags & 8)) && rotY)
  {
    PSMTXRotRad(rot, 'y', 0.017453292f * rotY);
    PSMTXConcat(matrix, rot, tmp);
    m = &tmp;
  }
  if (1.0f != scale)
  {
    PSMTXScale(scl, scale, scale, scale);
    PSMTXConcat(*m, scl, tmp);
    m = &tmp;
  }
  g_modeling_mtx_lv = 0;
  g_modeling_mtx = &g_modeling_mtx_stack[0];
  PSMTXCopy(*m, *g_modeling_mtx);
  wp->mpCurPose = pose;
  wp->mpCurPoseData = data;
  evalProc(-1, data->groupCount - 1);
}
#pragma push
#pragma opt_lifetimes off
void animPoseDrawMtx(s32 poseId, Mtx matrix, s32 xluMode, f32 rotY, f32 scale)
{
  Mtx m1;
  static Mtx mtx;
  s32 animId;
  Mtx m2;
  const char *name;
  Mtx scl;
  Mtx rot;
  Mtx tmp;
  AnimationPose *paper;
  AnimationPoseData *data;
  AnimationPose *pose;
  AnimationPose *p2;
  s32 scratchNum[2];
  AnimationPoseData *pd;
  pose = &wp->poses[poseId];
  if (pose->paperId != (-1))
  {
    if (pose->flags & 0xC0)
    {
      apdEval(pose, matrix, rotY, scale);
    }
    paper = &wp->poses[pose->paperId];
    // FAKE: (animPoseGetBaseData) bypasses the macro so the real inline is used; its return temp keeps the paper wp load out of the early simplify pushes (wp r3 / files r4 / poses r5)
    pd = (animPoseGetBaseData)(paper);
    if (((xluMode != 2) || (!(pd->flags & 1))) && (!(pd->flags & (1 << xluMode))))
    {
      return;
    }
    if (paper->typeFlags & 2)
    {
      scratchNum[1] = pd->mBufferPosNum;
      scratchNum[0] = pd->mBufferNrmNum;
      apdScratchN(scratchNum[1], scratchNum[0]);
      name = ANIM_INDEX_NAME(pose->paperAnimIdx);
      if (ANIM_NAME_INDEX(name) != -1)
      {
        // FAKE: the inlined animPoseSetAnim (its animName param re-reads the paper anim id) adds the coalesced param copy the target has (p2 r29 / xluMode r28)
        animPoseSetAnim(pose->paperId, ANIM_INDEX_NAME(animPosePaperAnimId(pose)), FALSE);
      }
      paper->localTime = animTimeGetTime(paper->type);
      paper->lastFrameZero = -1;
      paper->lastFrameTime = 0.0f;
      paper->curKeyValue = 0;
      paper->loopTime = 0.0f;
      paper->localTime = pose->mLocalTimeInit;
      animPoseMain(pose->paperId);
    }
    {
      struct smartEntry* t0 = pose->paperCapture0;
      struct smartEntry* t1 = pose->paperCapture1;
      wp->mpTexObjs[0] = (t0) ? &pose->paperTexObj[0] : ((void *) 0);
      wp->mpTexObjs[1] = (t1) ? &pose->paperTexObj[1] : ((void *) 0);
      wp->mpCurTexObj = (void *) 0;
      wp->mpTexDatas[0] = t0;
      wp->mpTexDatas[1] = t1;
      wp->mpCurTexData = (void *) 0;
      wp->paperTexMode0 = 1;
      wp->paperTexMode1 = 1;
    }
    paper->materialFlag = pose->materialFlag;
    paper->materialLightFlag = pose->materialLightFlag;
    paper->materialColor = pose->materialColor;
    paper->materialColor2 = pose->materialColor2;
    if (pose->flags & 2)
    {
      PSMTXScale(m1, pose->paperTexScaleX, pose->paperTexScaleY, 1.0f);
      PSMTXTrans(m2, *((f32 *) (0x104 + ((u8 *) pose))), pose->paperTexOfsY, 0.0f);
      PSMTXConcat(m1, m2, m1);
      animSetPaperMtx(m1, (void *) 0, (void *) 0);
      PSMTXScale(m1, pose->paperScaleXZ, pose->paperScaleY, pose->paperScaleXZ);
      if (wp->inBattle)
      {
        PSMTXTrans(m2, 0.0f, 2.0f * pose->paperOfsY, 0.0f);
      }
      else
      {
        PSMTXTrans(m2, 0.0f, pose->paperOfsY, 0.0f);
      }
      PSMTXConcat(m2, m1, m1);
      PSMTXConcat(matrix, m1, mtx);
      matrix = mtx;
    }
    else
    {
      PSMTXScale(m1, pose->paperTexScaleX, pose->paperTexScaleY, 1.0f);
      PSMTXTrans(m2, pose->paperTexOfsX, pose->paperTexOfsY, 0.0f);
      PSMTXConcat(m1, m2, m1);
      PSMTXTrans(m2, 0.5f, 1.0f, 0.0f);
      PSMTXConcat(m1, m2, m1);
      PSMTXScale(m2, 1.0f / pose->paperScaleXZ, 1.0f / pose->paperScaleY, 1.0f);
      PSMTXConcat(m1, m2, m1);
      PSMTXTrans(m2, -0.5f, -1.0f, 0.0f);
      PSMTXConcat(m1, m2, m1);
      animSetPaperMtx(m1, (void *) 0, (void *) 0);
    }
  }
  else
  {
    pd = *((AnimationPoseData **) wp->files[pose->fileId].handle->data);
    if (((xluMode != 2) || (!(pd->flags & 1))) && (!(pd->flags & (1 << xluMode))))
    {
      return;
    }
  }
  if (pose->paperId != (-1))
  {
    _animPoseDrawMtx(paper, matrix, xluMode, rotY, scale);
    wp->paperMtxPtr[0] = (void *) 0;
    wp->paperMtxPtr[1] = (void *) 0;
    wp->paperMtxPtr[2] = (void *) 0;
    if (paper->typeFlags & 2)
    {
      wp->mbUseFloatScratch = 0;
    }
  }
  else
  {
    _animPoseDrawMtx(pose, matrix, xluMode, rotY, scale);
  }
}
#pragma pop

u32 anim_ext_tex_size;

void animSetPaperTexObj(GXTexObj* obj1, GXTexObj* obj2, GXTexObj* obj3, void* data1, void* data2, void* data3, s32 texMode, s32 unused) {
    wp->mpTexObjs[0] = data1 ? obj1 : NULL;
    wp->mpTexObjs[1] = data2 ? obj2 : NULL;
    wp->mpCurTexObj = data3 ? obj3 : NULL;
    wp->mpTexDatas[0] = data1;
    wp->mpTexDatas[1] = data2;
    wp->mpCurTexData = data3;
    wp->paperTexMode0 = texMode;
    wp->paperTexMode1 = texMode;
}

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

void animPoseRelease(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];

    if ((u32)pose->typeFlags != 0) {
        animPaperPoseRelease(poseId);
    }
    else {
        if (pose->flags & 0x40) {
            smartFree((smartEntry*)pose->worldPositions);
        }
        if (pose->flags & 0x80) {
            smartFree((smartEntry*)pose->worldMatrices);
        }
        animPoseFileRelease(pose->fileId);
        pose->flags = 0;
    }
}

void animPaperPoseRelease(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];

    pose->references--;
    if (pose->references < 0) {
        animPoseFileRelease(pose->fileId);
        pose->flags = 0;
    }
}

void animPoseAutoRelease(s32 type) {
    int i;
    AnimationPose* pose;

    for (i = 0; i < wp->totalPoses; i++) {
        pose = animPoseGetAnimPosePtr(i);
        if (pose->flags & 1 && pose->type == type) {
            animPoseRelease(i);
        }
    }
    if (type == 1) {
        wp->inBattle = FALSE;
    }
    animPoseRefresh();
}

void animPaperPoseDisp(CameraId camId, void* param) {
    int i;
    AnimationPose* pose;

    for (i = 0, pose = wp->poses; i < wp->totalPoses; i++, pose++) {
        if (pose->flags & 1 && pose->paperId != -1) {
            animPaperPoseDispSub(0, pose);
        }
    }
}

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
    if (!(arg->flags & 1)) {
        return;
    }
    // FAKE: dead int-guarded re-definition (COLOURING M). The second reaching def stops IRO pass-0 copy
    // propagation, so 'pose' stays a named local (numbered above d) instead of the low parameter vreg;
    // the arm is folded away (t is reassigned before its next use). Behaviour-identical.
    t = 0;
    if (t != 0) {
        pose = arg;
    }
    d = animPoseGetBaseData(pose)->mpAnims[pose->animId].data;
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
    pose->flags = flags & ~8;
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
    if (pose->flags & 2 && pose->flags & 4 && gp->inBattle) {
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
    if (pose->flags & 2) {
        if (pose->flags & 4) {
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
            pose->flags &= ~4;
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
    pose->paperCapture1 = 0;
}

// target .bss/.sbss pad (uninitialised objects are emitted in reverse order, so this sits last)

inline s32 animPoseSearchName(const char* name, s32 type) {
    AnimationPose* pose;
    s32 i;

    for (i = 0; i < wp->totalPoses; i++) {
        pose = &wp->poses[i];
        if (pose->flags & 1 && pose->type == type && !strcmp(animPoseGetBaseData(pose)->fileName, name)) {
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
    pose->flags = flags & ~8;
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

void animSetPaperTexMtx(Mtx mtx1, Mtx mtx2, Mtx mtx3) {
    if (mtx1 == NULL) {
        wp->paperMtxPtr[0] = NULL;
    }
    else {
        PSMTXCopy(mtx1, wp->paperMtx[0]);
        wp->paperMtxPtr[0] = &wp->paperMtx[0];
    }
    if (mtx2 == NULL) {
        wp->paperMtxPtr[1] = NULL;
    }
    else {
        PSMTXCopy(mtx2, wp->paperMtx[1]);
        wp->paperMtxPtr[1] = &wp->paperMtx[1];
    }
    if (mtx3 == NULL) {
        wp->paperMtxPtr[2] = NULL;
    }
    else {
        PSMTXCopy(mtx3, wp->paperMtx[2]);
        wp->paperMtxPtr[2] = &wp->paperMtx[2];
    }
}

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
    model = fileAsyncf(5, callback, "%s/%s", "a", name);
    anim = fileAsyncf(4, NULL, "%s/%s-", "a", animGetTexName(name));
    if (model == -1) {
        return TRUE;
    }
    if ((u32)model == 0) {
        return FALSE;
    }
    if (anim == -1) {
        return TRUE;
    }
    return anim != 0;
}

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

AnimationPose* animPoseGetAnimPosePtr(s32 poseId) {
    return &wp->poses[poseId];
}

AnimData* animPoseGetAnimDataPtr(s32 poseId) {
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);
    return animPoseGetAnimBaseDataPtr(poseId)->mpAnims[pose->animId].data;
}

AnimationPoseData* animPoseGetAnimBaseDataPtr(s32 poseId) {
    AnimationPose* pose = animPoseGetAnimPosePtr(poseId);
    return *wp->files[pose->fileId].handle->data;
}

AnimTableEntry* animPoseGetCurrentAnim(s32 poseId) {
    AnimationPoseData* data = animPoseGetAnimBaseDataPtr(poseId);
    return &data->mpAnims[animPoseGetAnimPosePtr(poseId)->animId];
}

BOOL animPoseTestXLU(s32 poseId) {
    return (animPoseGetAnimBaseDataPtr(poseId)->flags >> 3) & 1;
}

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
    pose = wp->mpCurPose;
    data = wp->mpCurPoseData;
    group = &data->groups[groupId];
    if (pose->mpGroupVisibility[group->visibilityId]) {
        pose = wp->mpCurPose;
        node = (AnimPoseNode*)((u32*)pose->mpNodes + group->nodeId);
        if (group->useSSC) {
            if (parentId == -1) {
                sx = sy = sz = 1.0f;
            }
            else {
                parent = &data->groups[parentId];
                if (!parent->useSSC) {
                    sx = sy = sz = 1.0f;
                }
                else {
                    AnimPoseNode* pnode = (AnimPoseNode*)((u32*)pose->mpNodes + parent->nodeId);
                    sx = pnode->mScale.x;
                    sy = pnode->mScale.y;
                    sz = pnode->mScale.z;
                }
            }
            pushGXModelMtx_JointNode__(node, sx, sy, sz);
        }
        else {
            pushGXModelMtx_TransformNode__(node);
        }
        if (wp->mpCurPose->flags & 0x40) {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = (*g_modeling_mtx)[0][3];
            pos.y = (*g_modeling_mtx)[1][3];
            pos.z = (*g_modeling_mtx)[2][3];
            (*wp->mpCurPose->worldPositions)[groupId] = pos;
        }
        if (wp->mpCurPose->flags & 0x80) {
            PSMTXCopy(*g_modeling_mtx, (*wp->mpCurPose->worldMatrices)[groupId]);
        }
        if (!(wp->mpCurPose->flags & 0x20) || groupId != wp->mpCurPose->vivianGroupNo[0]) {
            evalProc(groupId, group->childGroupId);
        }
        g_modeling_mtx_lv--;
        g_modeling_mtx = &g_modeling_mtx_stack[g_modeling_mtx_lv];
    }
    evalProc(groupId, group->brotherGroupId);
}

void animPoseWorldPositionEvalOn(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    AnimationPoseData* data = animPoseGetBaseData(pose);
    u8 heap;

    if (!(pose->flags & 0x40)) {
        pose->flags |= 0x40;
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
        pose->worldPositions = (Vec**)smartAlloc(sizeof(Vec) * data->groupCount, heap);
    }
}

void L_animPoseWorldPositionEvalOff(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];

    if (pose->flags & 0x40) {
        pose->flags &= ~0x40;
        smartFree((smartEntry*)pose->worldPositions);
        pose->worldPositions = NULL;
    }
}

void animPoseWorldMatrixEvalOn(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    AnimationPoseData* data = animPoseGetBaseData(pose);
    u8 heap;

    if (!(pose->flags & 0x80)) {
        pose->flags |= 0x80;
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
        pose->worldMatrices = (Mtx**)smartAlloc(sizeof(Mtx) * data->groupCount, heap);
    }
}

void animPoseVivianMain(s32 poseId, Vec* pos) {
    AnimationPose* pose = &wp->poses[poseId];

    pose->flags |= 0x20;
    pose->matrix[0][3] = pos->x;
    pose->matrix[1][3] = pos->y;
    pose->matrix[2][3] = pos->z;
}

s32 animPoseGetVivianType(s32 poseId) {
    AnimationPose* pose = &wp->poses[poseId];
    return pose->vivianType;
}

void animPoseSetDispCallBack(s32 poseId, void* callback, void* userData) {
    AnimationPose* pose = &wp->poses[poseId];
    pose->dispCallback = callback;
    pose->dispUserDataCallback = userData;
}

inline void animPoseMainSub(AnimationPose* pose) {
    s32 id = (u32)((u8*)pose - (u8*)wp->poses) / sizeof(AnimationPose);
    if (pose->typeFlags & 2) {
        AnimationPoseData* data = animPoseGetBaseData(pose);
        s32 posNum = data->mBufferPosNum;
        s32 nrmNum = data->mBufferNrmNum;
        if (posNum + nrmNum <= wp->mFloatScratchRP) {
            if (wp->mFloatScratchWP + (posNum + nrmNum) > wp->mFloatScratchRP) {
                wp->mFloatScratchWP = 0;
            }
            wp->mbUseFloatScratch = 1;
            wp->vtxArrayPos = &wp->mpFloatScratch[wp->mFloatScratchWP];
            wp->mFloatScratchWP += posNum;
            wp->vtxArrayNrm = &wp->mpFloatScratch[wp->mFloatScratchWP];
            wp->mFloatScratchWP += nrmNum;
        }
        animPoseMain(id);
        wp->mbUseFloatScratch = 0;
    }
    else {
        animPoseMain(id);
    }
}

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
    u32 d8;
    u32 dc;
    s32 id;

    pose = &wp->poses[poseId];
    curPose = wp->mpCurPose;
    curAnimData = wp->mpCurAnimData;
    curPoseData = wp->mpCurPoseData;
    texObj0 = wp->mpTexObjs[0];
    texObj1 = wp->mpTexObjs[1];
    curTexObj = wp->mpCurTexObj;
    texData0 = wp->mpTexDatas[0];
    texData1 = wp->mpTexDatas[1];
    curTexData = wp->mpCurTexData;
    paperMtx0 = wp->paperMtxPtr[0];
    paperMtx1 = wp->paperMtxPtr[1];
    paperMtx2 = wp->paperMtxPtr[2];
    d8 = wp->paperTexMode0;
    dc = wp->paperTexMode1;

    wp->mpCurPose = pose;
    wp->mpCurAnimData = animPoseGetBaseData(pose)->mpAnims[pose->animId].data;
    wp->mpCurPoseData = animPoseGetBaseData(pose);
    wp->mpTexObjs[0] = NULL;
    wp->mpTexObjs[1] = NULL;
    wp->mpCurTexObj = NULL;
    wp->mpTexDatas[0] = NULL;
    wp->mpTexDatas[1] = NULL;
    wp->mpCurTexData = NULL;
    wp->paperMtxPtr[0] = NULL;
    wp->paperMtxPtr[1] = NULL;
    wp->paperMtxPtr[2] = NULL;
    wp->paperTexMode0 = 0;
    wp->paperTexMode1 = 0;

    if (pose->lastFrameZero == -1) {
        animPoseMainSub(pose);
    }
    renderProc(shapeId, pose->keyframeIdx);

    wp->mpCurPose = curPose;
    wp->mpCurAnimData = curAnimData;
    wp->mpCurPoseData = curPoseData;
    wp->mpTexObjs[0] = texObj0;
    wp->mpTexObjs[1] = texObj1;
    wp->mpCurTexObj = curTexObj;
    wp->mpTexDatas[0] = texData0;
    wp->mpTexDatas[1] = texData1;
    wp->mpCurTexData = curTexData;
    wp->paperMtxPtr[0] = paperMtx0;
    wp->paperMtxPtr[1] = paperMtx1;
    wp->paperMtxPtr[2] = paperMtx2;
    wp->paperTexMode0 = d8;
    wp->paperTexMode1 = dc;
}

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

const char* animPoseGetGroupName(s32 poseId, s32 group) {
    return animPoseGetAnimBaseDataPtr(poseId)->groups[group].name;
}

// .bss definitions last, reverse target order: emitted g_modeling_mtx_stack, work, tanfTbl as in the target
// (asm references alone would order them by first use)
f32 tanfTbl[0xB2];
AnimationWork work;
Mtx g_modeling_mtx_stack[32];
