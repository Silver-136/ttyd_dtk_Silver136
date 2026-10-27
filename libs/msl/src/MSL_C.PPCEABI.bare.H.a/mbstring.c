typedef unsigned long size_t;
typedef unsigned short wchar_t;

char* strncpy(char* dst, const char* src, size_t n);

static int is_utf8_complete(const char* s, size_t n);

static int unicode_to_UTF8(char* s, wchar_t wchar) {
    int number_of_bytes;
    wchar_t wide_char;
    char* target_ptr;
    char first_byte_mark[4] = {0x00, 0x00, 0xC0, 0xE0};

    if (!s)
        return 0;

    wide_char = wchar;

    if (wide_char < 0x80)
        number_of_bytes = 1;
    else if (wide_char < 0x800)
        number_of_bytes = 2;
    else
        number_of_bytes = 3;

    target_ptr = s + number_of_bytes;

    switch (number_of_bytes) {
    case 3:
        *--target_ptr = (wide_char & 0x3F) | 0x80;
        wide_char >>= 6;
    case 2:
        *--target_ptr = (wide_char & 0x3F) | 0x80;
        wide_char >>= 6;
    case 1:
        *--target_ptr = wide_char | first_byte_mark[number_of_bytes];
    }

    return number_of_bytes;
}

size_t wcstombs(char* s, const wchar_t* pwcs, size_t n) {
    int chars_written = 0;
    int result;
    char temp[3];
    wchar_t* source;

    if (!s || !pwcs)
        return 0;

    source = (wchar_t*)pwcs;

    while (chars_written <= n) {
        if (!*source) {
            *(s + chars_written) = '\0';
            break;
        } else {
            result = unicode_to_UTF8(temp, *source++);
            if ((chars_written + result) <= n) {
                strncpy(s + chars_written, temp, result);
                chars_written += result;
            } else
                break;
        }
    }

    return chars_written;
}

static inline int __mbtowc_noconv(wchar_t* pwc, const char* s, size_t n) {
    int number_of_bytes;
    int check_byte_count;
    char* source;
    wchar_t result_chr = 0;

    if (!s)
        return 0;

    if (n <= 0)
        return -1;

    number_of_bytes = is_utf8_complete(s, n);
    if (number_of_bytes < 0)
        return -1;

    source = (char*)s;
    switch (number_of_bytes) {
    case 3:
        result_chr |= (*source++ & 0x0F);
        result_chr <<= 6;
    case 2:
        result_chr |= (*source++ & 0x3F);
        result_chr <<= 6;
    case 1:
        result_chr |= (*source++ & 0x7F);
    }

    if (result_chr == 0)
        check_byte_count = 0;
    else if (result_chr < 0x80)
        check_byte_count = 1;
    else if (result_chr < 0x800)
        check_byte_count = 2;
    else
        check_byte_count = 3;

    if (check_byte_count != number_of_bytes)
        return -1;

    if (pwc)
        *pwc = result_chr;

    return number_of_bytes;
}

int mbtowc(wchar_t* pwc, const char* s, size_t n) {
    return __mbtowc_noconv(pwc, s, n);
}

static int is_utf8_complete(const char* s, size_t n) {
    if (n == 0)
        return -1;

    if (*s == 0x00)
        return 0;

    if ((*s & 0x80) == 0x00)
        return 1;
    else if ((*s & 0xE0) == 0xC0) {
        if (n >= 2) {
            if ((*(s + 1) & 0x80) == 0x80)
                return 2;
            return -1;
        } else
            return -2;
    } else if ((*s & 0xF0) == 0xE0) {
        if (n >= 3) {
            if (((*(s + 1) & 0x80) == 0x80) && ((*(s + 2) & 0x80) == 0x80))
                return 3;
            return -1;
        } else if (((n == 2) && ((*(s + 1) & 0x80) == 0x80)) || (n == 1))
            return -2;
        return -1;
    } else
        return -1;
}
