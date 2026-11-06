#include "ansi_files.h"

typedef void (*__idle_proc)(void);

unsigned long OSGetConsoleType(void);
int InitializeUART(unsigned long baudRate);
int WriteUARTN(const void* buf, unsigned long len);
int __TRK_write_console(__file_handle handle, unsigned char* buffer, size_t* count, __idle_proc idle_proc);

int __write_console(__file_handle handle, unsigned char* buffer, size_t* count, __idle_proc idle_proc) {
    static int initialized = 0;
    int ret;

    // 0x20000000 = OS_CONSOLE_TDEV (dolphin/os.h): only consoles other than a TDEV use the UART, at 57600 baud
    if ((OSGetConsoleType() & 0x20000000) == 0) {
        ret = 0;
        if (initialized == 0) {
            ret = InitializeUART(57600);
            if (ret == 0)
                initialized = 1;
        }
        if (ret != 0)
            return __io_error;
        if (WriteUARTN(buffer, *count) != 0) {
            *count = 0;
            return __io_error;
        }
    }
    __TRK_write_console(handle, buffer, count, idle_proc);
    return __no_io_error;
}
