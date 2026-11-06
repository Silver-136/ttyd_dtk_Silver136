#include <stdarg.h>
#include "ctype_map.h"
#include "strtoul.h"

enum argument_options {
    normal_argument,
    char_argument,
    short_argument,
    long_argument,
    long_long_argument,
    double_argument,
    long_double_argument,
    wchar_argument
};

typedef unsigned char char_map[32];

typedef struct {
    unsigned char suppress_assignment;
    unsigned char field_width_specified;
    unsigned char argument_options;
    unsigned char conversion_char;
    int field_width;
    char_map char_set;
} scan_format;

#define set_char_map(map, ch) map[(unsigned char)(ch) >> 3] |= (1 << ((ch) & 7))
#define bad_conversion 0xFF

static const char* parse_format(const char* format_string, scan_format* format);


typedef struct {
    char* str;
    int eos;
} __InStr;

extern int mbtowc(unsigned short*, const char*, unsigned long);

#define sc_isspace(c) (__ctype_map[(unsigned char)(c)] & __whitespace)
#define tst_char_map(map, ch) (map[(unsigned char)(ch) >> 3] & (1 << ((ch) & 7)))

int __sformatter(int (*ReadProc)(void*, int, int), void* ReadProcArg, const char* format_str, va_list arg);
int __StringRead(void* arg, int ch, int behavior);

int sscanf(const char* buffer, const char* format, ...) {
    va_list args;
    __InStr s;
    va_start(args, format);
    s.str = (char*)buffer;
    if (buffer == 0 || *buffer == 0) {
        return -1;
    }
    s.eos = 0;
    return __sformatter(__StringRead, &s, format, args);
}

int __StringRead(void* arg, int ch, int behavior) {
    __InStr* s = (__InStr*)arg;
    char c;
    switch (behavior) {
    case __GetAChar:
        c = *s->str;
        if (c == 0) {
            s->eos = 1;
            return -1;
        }
        s->str++;
        return (unsigned char)c;
    case __UngetAChar:
        if (s->eos == 0) {
            s->str--;
        } else {
            s->eos = 0;
        }
        return ch;
    case __TestForError:
        return s->eos;
    }
    return 0;
}

int __sformatter(int (*ReadProc)(void*, int, int), void* ReadProcArg, const char* format_str, va_list arg) {
    char* arg_ptr;
    int num_chars;
    int chars_read;
    int items_assigned;
    int conversions;
    int base;
    int negative;
    int overflow;
    const char* format_ptr;
    int format_char;
    char c;
    scan_format format;
    long long_num;
    unsigned long u_long_num;
    long long long_long_num;
    unsigned long long u_long_long_num;
    long double long_double_num;
    int rval;

    format_ptr = format_str;
    chars_read = 0;
    items_assigned = 0;
    conversions = 0;

    while ((format_char = (signed char)*format_ptr) != 0) {
        if (sc_isspace(format_char)) {
            while (sc_isspace(*++format_ptr)) {}
            while (sc_isspace(c = ReadProc(ReadProcArg, 0, __GetAChar))) {
                chars_read++;
            }
            ReadProc(ReadProcArg, c, __UngetAChar);
            continue;
        }
        if (format_char != '%') {
            if ((unsigned char)format_char != (c = ReadProc(ReadProcArg, 0, __GetAChar))) {
                ReadProc(ReadProcArg, c, __UngetAChar);
                goto exit;
            }
            chars_read++;
            format_ptr++;
            continue;
        }

        format_ptr = parse_format(format_ptr, &format);

        if (!format.suppress_assignment && format.conversion_char != '%') {
            arg_ptr = va_arg(arg, char*);
        } else {
            arg_ptr = 0;
        }

        if (format.conversion_char != 'n' && ReadProc(ReadProcArg, 0, __TestForError)) {
            goto exit;
        }

        switch (format.conversion_char) {
        case 'd':
            base = 10;
            goto signed_int;
        case 'i':
            base = 0;
        signed_int:
            if (format.argument_options == long_long_argument) {
                u_long_long_num = __strtoull(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
            }
            if (format.argument_options != long_long_argument) {
                u_long_num = __strtoul(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
            }
            if (!num_chars) {
                goto exit;
            }
            chars_read += num_chars;
            if (format.argument_options == long_long_argument) {
                long_long_num = (negative ? -u_long_long_num : u_long_long_num);
            } else {
                long_num = (negative ? -u_long_num : u_long_num);
            }
            if (arg_ptr) {
                switch (format.argument_options) {
                case normal_argument:
                    *(int*)arg_ptr = long_num;
                    break;
                case char_argument:
                    *(char*)arg_ptr = long_num;
                    break;
                case short_argument:
                    *(short*)arg_ptr = long_num;
                    break;
                case long_argument:
                    *(long*)arg_ptr = long_num;
                    break;
                case long_long_argument:
                    *(long long*)arg_ptr = long_long_num;
                    break;
                }
                items_assigned++;
            }
            conversions++;
            break;

        case 'o':
            base = 8;
            goto unsigned_int;
        case 'u':
            base = 10;
            goto unsigned_int;
        case 'x':
        case 'X':
            base = 16;
        unsigned_int:
            if (format.argument_options == long_long_argument) {
                u_long_long_num = __strtoull(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
            }
            if (format.argument_options != long_long_argument) {
                u_long_num = __strtoul(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
            }
            if (!num_chars) {
                goto exit;
            }
            chars_read += num_chars;
            if (negative) {
                if (format.argument_options == long_long_argument) {
                    u_long_long_num = -u_long_long_num;
                } else {
                    u_long_num = -u_long_num;
                }
            }
            if (arg_ptr) {
                switch (format.argument_options) {
                case normal_argument:
                    *(unsigned int*)arg_ptr = u_long_num;
                    break;
                case char_argument:
                    *(unsigned char*)arg_ptr = u_long_num;
                    break;
                case short_argument:
                    *(unsigned short*)arg_ptr = u_long_num;
                    break;
                case long_argument:
                    *(unsigned long*)arg_ptr = u_long_num;
                    break;
                case long_long_argument:
                    *(unsigned long long*)arg_ptr = u_long_long_num;
                    break;
                }
                items_assigned++;
            }
            conversions++;
            break;

        case 'a':
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
            long_double_num = __strtold(format.field_width, ReadProc, ReadProcArg, &num_chars, &overflow);
            if (!num_chars) {
                goto exit;
            }
            chars_read += num_chars;
            if (arg_ptr) {
                switch (format.argument_options) {
                case normal_argument:
                    *(float*)arg_ptr = long_double_num;
                    break;
                case double_argument:
                    *(double*)arg_ptr = long_double_num;
                    break;
                case long_double_argument:
                    *(long double*)arg_ptr = long_double_num;
                    break;
                }
                items_assigned++;
            }
            conversions++;
            break;

        case 'c':
            if (!format.field_width_specified) {
                format.field_width = 1;
            }
            if (arg_ptr) {
                num_chars = 0;
                while (format.field_width-- && (rval = ReadProc(ReadProcArg, 0, __GetAChar)) != -1) {
                    c = rval;
                    if (format.argument_options == wchar_argument) {
                        mbtowc((unsigned short*)arg_ptr, &c, 1);
                        // BUG: each wide character is stored as two bytes, but the destination advances by
                        // one byte (the %s/%[ branch below advances by two).
                        arg_ptr++;
                    } else {
                        *arg_ptr++ = c;
                    }
                    num_chars++;
                }
                if (!num_chars) {
                    goto exit;
                }
                chars_read += num_chars;
                items_assigned++;
            } else {
                num_chars = 0;
                while (format.field_width-- && (c = ReadProc(ReadProcArg, 0, __GetAChar)) != -1) {
                    num_chars++;
                }
                if (!num_chars) {
                    goto exit;
                }
            }
            conversions++;
            break;
        case '%':
            while (sc_isspace(c = ReadProc(ReadProcArg, 0, __GetAChar))) {
                chars_read++;
            }
            if (c != '%') {
                ReadProc(ReadProcArg, c, __UngetAChar);
                goto exit;
            }
            chars_read++;
            break;

        case 's':
            c = ReadProc(ReadProcArg, 0, __GetAChar);
            while (sc_isspace((int)c)) {
                chars_read++;
                c = ReadProc(ReadProcArg, 0, __GetAChar);
            }
            ReadProc(ReadProcArg, c, __UngetAChar);
            // fall through: after the leading whitespace, %s reads like %[ with its non-whitespace set
        case '[':
            if (arg_ptr) {
                num_chars = 0;
                while (format.field_width-- && (c = ReadProc(ReadProcArg, 0, __GetAChar)) != -1
                       && tst_char_map(format.char_set, c)) {
                    if (format.argument_options == wchar_argument) {
                        mbtowc((unsigned short*)arg_ptr, &c, 1);
                        arg_ptr += 2;
                    } else {
                        *arg_ptr++ = c;
                    }
                    num_chars++;
                }
                if (!num_chars) {
                    ReadProc(ReadProcArg, c, __UngetAChar);
                    goto exit;
                }
                chars_read += num_chars;
                if (format.argument_options == wchar_argument) {
                    *(unsigned short*)arg_ptr = 0;
                } else {
                    *arg_ptr = 0;
                }
                items_assigned++;
            } else {
                num_chars = 0;
                while (format.field_width-- && (c = ReadProc(ReadProcArg, 0, __GetAChar)) != -1
                       && tst_char_map(format.char_set, c)) {
                    num_chars++;
                }
                if (!num_chars) {
                    ReadProc(ReadProcArg, c, __UngetAChar);
                    break;
                }
                chars_read += num_chars;
            }
            if (format.field_width >= 0) {
                ReadProc(ReadProcArg, c, __UngetAChar);
            }
            conversions++;
            break;

        case 'n':
            if (arg_ptr) {
                switch (format.argument_options) {
                case normal_argument:
                    *(int*)arg_ptr = chars_read;
                    break;
                case short_argument:
                    *(short*)arg_ptr = chars_read;
                    break;
                case long_argument:
                    *(long*)arg_ptr = chars_read;
                    break;
                case char_argument:
                    *(char*)arg_ptr = chars_read;
                    break;
                case long_long_argument:
                    *(long long*)arg_ptr = chars_read;
                    break;
                }
            }
            break;

        case bad_conversion:
        default:
            goto exit;
        }
    }

exit:
    if (ReadProc(ReadProcArg, 0, __TestForError) && conversions == 0) {
        return -1;
    }
    return items_assigned;
}



static const char* parse_format(const char* format_string, scan_format* format) {
    const char* s = format_string;
    int c;
    int flag_found;
    scan_format f = {0, 0, normal_argument, 0, 0x7FFFFFFF, {0}};

    if ((c = *++s) == '%') {
        f.conversion_char = c;
        *format = f;
        return ((const char*)s + 1);
    }

    if (c == '*') {
        f.suppress_assignment = 1;
        c = *++s;
    }

    if (__ctype_map[(unsigned char)c] & __digit) {
        f.field_width = 0;
        do {
            f.field_width = (f.field_width * 10) + (c - '0');
            c = *++s;
        } while (__ctype_map[(unsigned char)c] & __digit);

        if (f.field_width == 0) {
            f.conversion_char = bad_conversion;
            *format = f;
            return ((const char*)s + 1);
        }
        f.field_width_specified = 1;
    }

    flag_found = 1;

    switch (c) {
    case 'h':
        f.argument_options = short_argument;
        if (s[1] == 'h') {
            f.argument_options = char_argument;
            c = *++s;
        }
        break;
    case 'l':
        f.argument_options = long_argument;
        if (s[1] == 'l') {
            f.argument_options = long_long_argument;
            c = *++s;
        }
        break;
    case 'L':
        f.argument_options = long_double_argument;
        break;
    default:
        flag_found = 0;
    }

    if (flag_found) {
        c = *++s;
    }

    f.conversion_char = c;

    switch (c) {
    case 'd':
    case 'i':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
        if (f.argument_options == long_double_argument) {
            f.conversion_char = bad_conversion;
            break;
        }
        break;
    case 'a':
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
        if (f.argument_options == char_argument || f.argument_options == short_argument
            || f.argument_options == long_long_argument) {
            f.conversion_char = bad_conversion;
            break;
        }
        if (f.argument_options == long_argument) {
            f.argument_options = double_argument;
        }
        break;
    case 'p':
        f.argument_options = long_argument;
        f.conversion_char = 'x';
        break;
    case 'c':
        if (f.argument_options == long_argument) {
            f.argument_options = wchar_argument;
        } else if (f.argument_options != normal_argument) {
            f.conversion_char = bad_conversion;
        }
        break;
    case 's':
        if (f.argument_options == long_argument) {
            f.argument_options = wchar_argument;
        } else if (f.argument_options != normal_argument) {
            f.conversion_char = bad_conversion;
        }
        {
            int i;
            unsigned char* p;
            for (i = sizeof(f.char_set), p = f.char_set; i; --i) {
                *p++ = 0xFF;
            }
            // every character but whitespace: 0xC1 clears '\t'..'\r' (0x09-0x0D), 0xFE clears ' ' (0x20)
            f.char_set[1] = 0xC1;
            f.char_set[4] = 0xFE;
        }
        break;
    case 'n':
        break;
    case '[':
        if (f.argument_options == long_argument) {
            f.argument_options = wchar_argument;
        } else if (f.argument_options != normal_argument) {
            f.conversion_char = bad_conversion;
        }
        c = *++s;
        {
            int invert = 0;
            if (c == '^') {
                invert = 1;
                c = *++s;
            }
            if (c == ']') {
                set_char_map(f.char_set, ']');
                c = *++s;
            }
            while (c && c != ']') {
                int d;
                set_char_map(f.char_set, c);
                if (*(s + 1) == '-' && (d = *(s + 2)) != 0 && d != ']') {
                    while (++c <= d) {
                        set_char_map(f.char_set, c);
                    }
                    c = *(s += 3);
                } else {
                    c = *++s;
                }
            }
            if (!c) {
                f.conversion_char = bad_conversion;
                break;
            }
            if (invert) {
                int i;
                unsigned char* p;
                for (i = sizeof(f.char_set), p = f.char_set; i; --i, ++p) {
                    *p = ~*p;
                }
                break;
            }
        }
        break;
    default:
        f.conversion_char = bad_conversion;
        break;
    }

    *format = f;
    return ((const char*)s + 1);
}
