/* Build date and time stamps. This object is linked ahead of system.o and holds the first 0x18 bytes of
   .rodata. */
#include "version.h"

#if VERSION == VERSION_EU
const char R_build_date[] = "Sep 26 2004";
const char R_build_time[] = "15:37:03";
#elif VERSION == VERSION_DEMO_EU
const char R_build_date[] = "Sep 23 2004";
const char R_build_time[] = "13:32:41";
#elif VERSION == VERSION_DEMO_US2
const char R_build_date[] = "Sep 18 2004";
const char R_build_time[] = "17:37:16";
#elif VERSION == VERSION_US
const char R_build_date[] = "Sep  9 2004";
const char R_build_time[] = "09:34:02";
#else
// JP release and both early demos
const char R_build_date[] = "Jun 17 2004";
const char R_build_time[] = "17:20:01";
#endif
