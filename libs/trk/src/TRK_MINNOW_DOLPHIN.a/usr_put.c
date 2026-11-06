typedef int BOOL;

BOOL GetTRKConnected(void);
void SetTRKConnected(BOOL connected);
void OSReport(const char* msg);

/* chars_left is never set to nonzero: the loop ends only at the NUL and the result is always 0. */
BOOL usr_puts_serial(const char* msg) {
    BOOL chars_left = 0;
    char c;
    char buf[2];

    while (!chars_left && (c = *msg++) != '\0') {
        BOOL wasConnected = GetTRKConnected();

        buf[0] = c;
        buf[1] = '\0';

        SetTRKConnected(0);
        OSReport(buf);
        SetTRKConnected(wasConnected);

        chars_left = 0;
    }

    return chars_left;
}

void usr_put_initialize(void) {}
