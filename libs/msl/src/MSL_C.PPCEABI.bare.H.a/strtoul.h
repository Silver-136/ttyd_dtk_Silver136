#ifndef MSL_STRTOUL_H
#define MSL_STRTOUL_H

/* The behavior argument of a ReadProc callback (scanf.c __StringRead): read the next character, push one
   character back, or test the end-of-input flag. */
enum __ReadProcActions { __GetAChar, __UngetAChar, __TestForError };

unsigned long __strtoul(int base, int max_width, int (*ReadProc)(void*, int, int), void* ReadProcArg,
                        int* chars_scanned, int* negative, int* overflow);
unsigned long long __strtoull(int base, int max_width, int (*ReadProc)(void*, int, int), void* ReadProcArg,
                              int* chars_scanned, int* negative, int* overflow);
long double __strtold(int max_width, int (*ReadProc)(void*, int, int), void* ReadProcArg, int* chars_scanned,
                      int* overflow);

#endif
