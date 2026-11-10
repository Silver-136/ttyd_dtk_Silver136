#define NULL 0

#define max_funcs 64

void __begin_critical_region(int region);
void __end_critical_region(int region);
void __kill_critical_regions(void);
void __destroy_global_chain(void);
void _ExitProcess(void);

extern void (*_dtors[])(void);

#define atexit_funcs_access 0

int __aborting = 0;

void (*__atexit_funcs[max_funcs])(void);
int __atexit_curr_func = 0;

void (*__stdio_exit)(void) = NULL;
void (*__console_exit)(void) = NULL;

void exit(int status) {
    void (**dtor)(void);

    if (!__aborting) {
        __begin_critical_region(atexit_funcs_access);
        __end_critical_region(atexit_funcs_access);

        __destroy_global_chain();

        dtor = _dtors;
        while (*dtor != NULL) {
            (*dtor)();
            dtor++;
        }

        if (__stdio_exit) {
            (*__stdio_exit)();
            __stdio_exit = NULL;
        }
    }

    __begin_critical_region(atexit_funcs_access);
    while (__atexit_curr_func > 0)
        (*__atexit_funcs[--__atexit_curr_func])();
    __end_critical_region(atexit_funcs_access);

    __kill_critical_regions();

    if (__console_exit) {
        (*__console_exit)();
        __console_exit = NULL;
    }

    _ExitProcess();
}
