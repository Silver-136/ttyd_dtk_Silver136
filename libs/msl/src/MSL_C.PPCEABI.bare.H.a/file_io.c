#include "ansi_files.h"
#include "ctype_map.h"

static inline int tolower(int c) { return (c == EOF) ? EOF : (int)__lower_map[(unsigned char)c]; }

int __flush_all(void);
long ftell(FILE* file);

int __msl_strnicmp(const char* str1, const char* str2, int n) {
    int i;
    char c1, c2;

    for (i = 0; i < n; i++) {
        c1 = tolower(*str1++);
        c2 = tolower(*str2++);

        if (c1 < c2)
            return -1;

        if (c1 > c2)
            return 1;

        if (c1 == '\0')
            return 0;
    }

    return 0;
}

int fflush(FILE* file) {
    fpos_t position;

    if (!file)
        return (__flush_all());

    if (file->state.error || file->mode.file_kind == __closed_file)
        return (EOF);

    if (file->mode.io_mode == __read)
        return (0);

    if (file->state.io_state >= __rereading)
        file->state.io_state = __reading;

    if (file->state.io_state == __reading)
        file->buffer_len = 0;

    if (file->state.io_state != __writing) {
        file->state.io_state = __neutral;
        return (0);
    }

    if (file->mode.file_kind != __disk_file)
        position = 0;
    else
        position = ftell(file);

    if (__flush_buffer(file, NULL)) {
        set_error(file);
        return (EOF);
    }

    file->state.io_state = __neutral;
    file->position = position;
    file->buffer_len = 0;

    return (0);
}

int fclose(FILE* file) {
    int flush_result, close_result;

    if (file == NULL)
        return (EOF);

    if (file->mode.file_kind == __closed_file)
        return (0);

    flush_result = fflush(file);

    close_result = (*file->close_proc)(file->handle);

    file->mode.file_kind = __closed_file;
    file->handle = NULL;

    if (file->state.free_buffer)
        free(file->buffer);

    return ((flush_result || close_result) ? EOF : 0);
}
