#include "ansi_files.h"

#define console_buff_size 256
#define _STATIC_FILES 4

enum { must_exist };

int __read_console(__file_handle handle, unsigned char* buffer, size_t* count, void* ref_con);
int __write_console(__file_handle handle, unsigned char* buffer, size_t* count, void* ref_con);
int __close_console(__file_handle handle);

unsigned char stdin_buff[console_buff_size];
unsigned char stdout_buff[console_buff_size];
unsigned char stderr_buff[console_buff_size];

/* stdin, stdout and stderr; the fourth entry stays zero-filled and ends the next_file_struct list. */
FILE __files[_STATIC_FILES] = {
    {0, {must_exist, __read, _IOLBF, __console_file, __unoriented, 0}, {__neutral, 0, 0, 0}, 0, 0, 0, {0, 0}, {0, 0}, 0, stdin_buff, console_buff_size, stdin_buff, 0, 0, 0, 0, NULL, __read_console, __write_console, __close_console, NULL, &__files[1]},
    {1, {must_exist, __write, _IOLBF, __console_file, __unoriented, 0}, {__neutral, 0, 0, 0}, 0, 0, 0, {0, 0}, {0, 0}, 0, stdout_buff, console_buff_size, stdout_buff, 0, 0, 0, 0, NULL, __read_console, __write_console, __close_console, NULL, &__files[2]},
    {2, {must_exist, __write, _IONBF, __console_file, __unoriented, 0}, {__neutral, 0, 0, 0}, 0, 0, 0, {0, 0}, {0, 0}, 0, stderr_buff, console_buff_size, stderr_buff, 0, 0, 0, 0, NULL, __read_console, __write_console, __close_console, NULL, &__files[3]},
};

int __flush_all(void) {
    int result = 0;
    FILE* file = &__files[0];

    while (file) {
        if (file->mode.file_kind != __closed_file && fflush(file))
            result = EOF;
        file = file->next_file_struct;
    }

    return (result);
}

void __close_all(void) {
    FILE* p = &__files[0];
    FILE* plast;

    __begin_critical_region(__stdio_access);

    while (p) {
        if (p->mode.file_kind != __closed_file)
            fclose(p);

        plast = p;
        p = p->next_file_struct;
        if (plast->is_dynamically_allocated)
            free(plast);
        else {
            plast->mode.file_kind = __unavailable_file;
            if ((p != NULL) && p->is_dynamically_allocated)
                plast->next_file_struct = NULL;
        }
    }

    __end_critical_region(__stdio_access);
}
