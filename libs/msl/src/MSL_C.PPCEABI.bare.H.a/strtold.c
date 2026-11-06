#define EOF -1
#include "ansi_fp.h"
#include "lconv.h"

#define MAX_SIG_DIG 20

extern float __float_huge[];
extern float __float_nan[];
extern double __double_huge[];
extern double __extended_min[];
extern double __extended_max[];

#include "ctype_map.h"
#include "strtoul.h"

#define isalpha(c) ((int)(__ctype_map[(unsigned char)(c)] & __letter))
#define isdigit(c) ((int)(__ctype_map[(unsigned char)(c)] & __digit))
#define isspace(c) ((int)(__ctype_map[(unsigned char)(c)] & __whitespace))
#define isxdigit(c) ((int)(__ctype_map[(unsigned char)(c)] & __hex_digit))
#define toupper(c) ((c == EOF) ? EOF : ((int)__upper_map[(unsigned char)(c)]))

enum scan_states {
    start = 0x0001,
    sig_start = 0x0002,
    leading_sig_zeroes = 0x0004,
    int_digit_loop = 0x0008,
    frac_start = 0x0010,
    frac_digit_loop = 0x0020,
    sig_end = 0x0040,
    exp_start = 0x0080,
    exp_sign = 0x0100,
    leading_exp_digit = 0x0200,
    exp_digit_loop = 0x0400,
    finished = 0x0800,
    failure = 0x1000,
    nan_state = 0x2000,
    inf_state = 0x4000,
    hex_state = 0x8000
};

enum hex_scan_states {
    not_hex = 0x00,
    hex_start = 0x01,
    leading_hex_zeroes = 0x02,
    int_hex_digit_loop = 0x04,
    frac_hex_start = 0x08,
    frac_hex_digit_loop = 0x10,
    p_exp_start = 0x20,
    p_exp_sign = 0x40,
    p_exp_digit_loop = 0x100
};

#define final_state(scan_state) (scan_state & (finished | failure))
#define success(scan_state) (scan_state & (leading_sig_zeroes | int_digit_loop | frac_digit_loop | leading_exp_digit | exp_digit_loop | finished))
#define fetch() (count++, (*ReadProc)(ReadProcArg, 0, __GetAChar))
#define unfetch(c) (*ReadProc)(ReadProcArg, c, __UngetAChar)

long double __strtold(int max_width, int (*ReadProc)(void*, int, int), void* ReadProcArg, int* chars_scanned, int* overflow) {
    int scan_state = start;
    int hex_scan_state = not_hex;
    int count = 0;
    int spaces = 0;
    int c;
    decimal d = {0, 0, 0, {0, ""}};
    int sign = 0;
    int exp_negative = 0;
    int exp_value = 0;
    int dec_exp = 0;
    union {
        long double v;
        unsigned char b[8];
        short s;
        unsigned long long u;
    } r;
    int sign_detected = 0;
    unsigned char* buf = r.b;
    int hex_digits;
    int hex_exp_negative = 0;
    int hex_exp_digits = 0;
    int hex_dig_count = 0;
    short hex_exp = 0;
    int radix = *(unsigned char*)__lconv.decimal_point;
    *overflow = 0;
    c = fetch();

    while (count <= max_width && c != EOF && !final_state(scan_state)) {
        switch (scan_state) {
        case start:
            if (isspace(c)) {
                c = (*ReadProc)(ReadProcArg, 0, __GetAChar);
                spaces++;
                break;
            }
            switch (toupper(c)) {
            case '-':
                sign = 1;
            case '+':
                c = fetch();
                sign_detected = 1;
                break;
            case 'I':
                c = fetch();
                scan_state = inf_state;
                break;
            case 'N':
                c = fetch();
                scan_state = nan_state;
                break;
            default:
                scan_state = sig_start;
                break;
            }
            break;
        case inf_state: {
            // the 'I' is already read: match "NF" or "NFINITY" (case-insensitive)
            int i = 1;
            char model[] = "INFINITY";
            while (i < 8 && model[i] == toupper(c)) {
                i++;
                c = fetch();
            }
            if (i == 3 || i == 8) {
                r.v = sign ? -__float_huge[0] : __float_huge[0];
                *chars_scanned = spaces + i + sign_detected;
                return r.v;
            }
            scan_state = failure;
            break;
        }
        case nan_state: {
            // the 'N' is already read: match "AN", optionally followed by "(", up to 32 letters or digits and ")"
            int i = 1, j = 0;
            char model[] = "NAN(";
            while (i < 4 && model[i] == toupper(c)) {
                i++;
                c = fetch();
            }
            if (i == 3 || i == 4) {
                if (i == 4) {
                    while (j < 32 && (isdigit(c) || isalpha(c))) {
                        j++;
                        c = fetch();
                    }
                    if (c != ')') {
                        scan_state = failure;
                        break;
                    }
                    j++;
                }
                r.v = sign ? -__float_nan[0] : __float_nan[0];
                *chars_scanned = spaces + i + j + sign_detected;
                return r.v;
            }
            scan_state = failure;
            break;
        }
        case sig_start:
            if (c == radix) {
                scan_state = frac_start;
                c = fetch();
                break;
            }
            if (!isdigit(c)) {
                scan_state = failure;
                break;
            }
            if (c == '0') {
                c = fetch();
                if (toupper(c) == 'X') {
                    hex_scan_state = hex_start;
                    scan_state = hex_state;
                } else {
                    scan_state = leading_sig_zeroes;
                }
            } else {
                scan_state = int_digit_loop;
            }
            break;
        case leading_sig_zeroes:
            if (c == '0') {
                c = fetch();
            } else {
                scan_state = int_digit_loop;
            }
            break;
        case int_digit_loop:
            if (!isdigit(c)) {
                if (c == radix) {
                    scan_state = frac_digit_loop;
                    c = fetch();
                } else {
                    scan_state = sig_end;
                }
                break;
            }
            if (d.sig.length < MAX_SIG_DIG) {
                d.sig.text[d.sig.length++] = c;
            } else {
                dec_exp++;
            }
            c = fetch();
            break;
        case frac_start:
            if (!isdigit(c)) {
                scan_state = failure;
            } else {
                scan_state = frac_digit_loop;
            }
            break;
        case frac_digit_loop:
            if (!isdigit(c)) {
                scan_state = sig_end;
                break;
            }
            if (d.sig.length < MAX_SIG_DIG) {
                if (c != '0' || d.sig.length != 0) {
                    d.sig.text[d.sig.length++] = c;
                }
                dec_exp--;
            }
            c = fetch();
            break;
        case sig_end:
            if (toupper(c) == 'E') {
                scan_state = exp_start;
                c = fetch();
            } else {
                scan_state = finished;
            }
            break;
        case exp_start:
            if (c == '+') {
                c = fetch();
            } else if (c == '-') {
                c = fetch();
                exp_negative = 1;
            }
            scan_state = exp_sign;
            break;
        case exp_sign:
            if (!isdigit(c)) {
                scan_state = failure;
                break;
            }
            if (c == '0') {
                scan_state = leading_exp_digit;
                c = fetch();
            } else {
                scan_state = exp_digit_loop;
            }
            break;
        case leading_exp_digit:
            if (c == '0') {
                c = fetch();
            } else {
                scan_state = exp_digit_loop;
            }
            break;
        case exp_digit_loop:
            if (!isdigit(c)) {
                scan_state = finished;
                break;
            }
            exp_value = exp_value * 10 + (c - '0');
            if (exp_value > 0x7fff) {
                *overflow = 1;
            }
            c = fetch();
            break;
        case hex_state:
            switch (hex_scan_state) {
            case hex_start:
                r.b[0] = 0;
                r.b[1] = 0;
                r.b[2] = 0;
                r.b[3] = 0;
                r.b[4] = 0;
                r.b[5] = 0;
                r.b[6] = 0;
                r.b[7] = 0;
                hex_digits = 2;
                hex_scan_state = leading_hex_zeroes;
                c = fetch();
                break;
            case leading_hex_zeroes:
                if (c == '0') {
                    c = fetch();
                } else {
                    hex_scan_state = int_hex_digit_loop;
                }
                break;
            case int_hex_digit_loop:
                if (!isxdigit(c)) {
                    if (c == radix) {
                        hex_scan_state = frac_hex_start;
                        c = fetch();
                    } else {
                        hex_scan_state = frac_hex_digit_loop;
                    }
                    break;
                }
                if (hex_digits < 17) {
                    unsigned char t = buf[hex_digits / 2];
                    unsigned char dig;
                    int u;
                    hex_dig_count++;
                    u = toupper(c);
                    if (u >= 'A') {
                        u = u - 'A' + 10;
                    } else {
                        u -= '0';
                    }
                    dig = u;
                    if (hex_digits % 2) {
                        t |= dig;
                    } else {
                        t |= dig << 4;
                    }
                    buf[hex_digits++ / 2] = t;
                    c = fetch();
                    break;
                }
                c = fetch();
                break;
            case frac_hex_start:
                if (!isxdigit(c)) {
                    hex_scan_state = frac_hex_digit_loop;
                    break;
                }
                if (hex_digits < 17) {
                    unsigned char t = buf[hex_digits / 2];
                    unsigned char dig;
                    int u;
                    u = toupper(c);
                    if (u >= 'A') {
                        u = u - 'A' + 10;
                    } else {
                        u -= '0';
                    }
                    dig = u;
                    if (hex_digits % 2) {
                        t |= dig;
                    } else {
                        t |= dig << 4;
                    }
                    buf[hex_digits++ / 2] = t;
                    c = fetch();
                    break;
                }
                c = fetch();
                break;
            case frac_hex_digit_loop:
                if (toupper(c) == 'P') {
                    hex_scan_state = p_exp_start;
                    hex_exp_digits++;
                    c = fetch();
                } else {
                    scan_state = finished;
                }
                break;
            case p_exp_start:
                hex_exp_digits++;
                if (c == '-') {
                    hex_exp_negative = 1;
                } else if (c != '+') {
                    unfetch(c);
                    hex_exp_digits--;
                }
                hex_scan_state = p_exp_sign;
                c = fetch();
                break;
            case p_exp_sign:
                if (!isdigit(c)) {
                    scan_state = failure;
                    break;
                }
                if (c == '0') {
                    // BUG: 0x80 is not one of the hex_scan_states and no case handles it; the next pass fetches
                    // nothing and changes no state, so an exponent with a leading zero ("0x1p0") never ends
                    // the loop.
                    hex_scan_state = 0x80;
                    hex_exp_digits++;
                    c = fetch();
                } else {
                    hex_scan_state = p_exp_digit_loop;
                }
                break;
            case p_exp_digit_loop:
                if (!isdigit(c)) {
                    scan_state = finished;
                    break;
                }
                hex_exp = hex_exp * 10 + (c - '0');
                // BUG: tests exp_value (the decimal exponent, still 0 here) instead of hex_exp, so a binary
                // exponent overflow is never reported.
                if (exp_value > 0x7fff) {
                    *overflow = 1;
                }
                hex_exp_digits++;
                c = fetch();
                break;
            }
            break;
        }
    }

    if (!(scan_state = success(scan_state))) {
        *chars_scanned = 0;
    } else {
        *chars_scanned = count - 1 + spaces;
    }
    unfetch(c);

    if (hex_scan_state == not_hex) {
        int n;
        unsigned char* p;
        if (exp_negative) {
            exp_value = -exp_value;
        }
        n = d.sig.length;
        p = &d.sig.text[n];
        while (n-- != 0 && *--p == '0') {
            dec_exp++;
        }
        d.sig.length = n + 1;
        if (d.sig.length == 0) {
            d.sig.text[d.sig.length++] = '0';
        }
        exp_value += dec_exp;
        if (exp_value < -0x8000 || exp_value > 0x7fff) {
            *overflow = 1;
        }
        if (*overflow) {
            if (exp_negative) {
                return 0.0;
            }
            if (sign) {
                return -__double_huge[0];
            }
            return __double_huge[0];
        }
        d.exp = exp_value;
        r.v = __dec2num(&d);
        if (r.v != 0.0 && r.v < __extended_min[0]) {
            *overflow = 1;
        } else if (r.v > __extended_max[0]) {
            *overflow = 1;
            r.v = __double_huge[0];
        }
        if (sign && scan_state) {
            r.v = -r.v;
        }
        return r.v;
    }

    if (r.v) {
        if (hex_exp_negative) {
            hex_exp = -hex_exp;
        }
        while ((*(short*)buf & 0xf0) != 0x10) {
            hex_exp++;
            r.u >>= 1;
        }
        hex_exp += (hex_dig_count - 1) * 4;
        // r.s is the top 16 bits of the double: keep the 4 mantissa bits, store the biased exponent above them
        r.s &= 0xf;
        r.s |= (hex_exp + 0x3ff) << 4;
        *chars_scanned = spaces + sign_detected + hex_digits + 1 + hex_exp_digits;
        if (0.0 != r.v && r.v < __extended_min[0]) {
            r.v = 0.0;
            *overflow = 1;
        } else if (r.v > __extended_max[0]) {
            *overflow = 1;
            r.v = __double_huge[0];
        }
        if (sign) {
            r.s |= 0x8000;
        }
    } else {
        r.v = 0.0;
    }
    return r.v;
}

