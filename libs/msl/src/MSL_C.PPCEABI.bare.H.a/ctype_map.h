#ifndef MSL_CTYPE_MAP_H
#define MSL_CTYPE_MAP_H

/* Character class tables (ctype.c). Each __ctype_map entry is a set of the flag bits below. */
extern unsigned char __ctype_map[];
extern unsigned char __lower_map[];
extern unsigned char __upper_map[];

#define __control_char 0x01
#define __motion_char 0x02
#define __space_char 0x04
#define __punctuation 0x08
#define __digit 0x10
#define __hex_digit 0x20
#define __lower_case 0x40
#define __upper_case 0x80

#define __letter (__lower_case | __upper_case)
#define __whitespace (__motion_char | __space_char)

#endif
