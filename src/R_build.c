/* Build date and time stamps. This object is linked ahead of system.o and holds the first 0x18 bytes of
   .rodata. */
#ifdef VERSION_G8ME01
const char R_build_date[] = "Sep  9 2004";
const char R_build_time[] = "09:34:02";
#else
const char R_build_date[] = "Jun 17 2004";
const char R_build_time[] = "17:20:01";
#endif
