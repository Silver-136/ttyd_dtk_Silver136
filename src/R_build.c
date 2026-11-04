/* Build date and time stamps. This object is linked ahead of system.o and holds the first 0x18 bytes of
   .rodata. */
#include "version.h"

#if VERSION >= VERSION_US
#if VERSION >= VERSION_DEMO_EU
const char R_build_date[] = "Sep 26 2004";
/** Build time of the EU version (15:37:03). */
const char R_build_time[] = "15:37:03";
#else
const char R_build_date[] = "Sep  9 2004";
const char R_build_time[] = "09:34:02";
#endif
#else
const char R_build_date[] = "Jun 17 2004";
const char R_build_time[] = "17:20:01";
#endif
