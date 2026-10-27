#include "ansi_files.h"

extern int errno;
#define EFPOS 40

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

enum { __read = 1, __write = 2, __read_write = 3 };

#define files_access 2

#define set_error(file)          \
    do {                         \
        (file)->state.error = 1; \
        (file)->buffer_len = 0;  \
    } while (0)

long _ftell(FILE* file) {
    int charsInUndoBuffer = 0;
    unsigned long position;
    unsigned char* p;
    size_t n;

    if (!(file->mode.file_kind == __disk_file || file->mode.file_kind == __console_file) || file->state.error) {
        errno = EFPOS;
        return (-1);
    }

    if (file->state.io_state == __neutral)
        return (file->position);

    position = file->buffer_pos + (file->buffer_ptr - file->buffer);

    if (file->state.io_state >= __rereading) {
        charsInUndoBuffer = file->state.io_state - __rereading + 1;
        position -= charsInUndoBuffer;
    }

    if (!file->mode.binary_io) {
        n = file->buffer_ptr - file->buffer - charsInUndoBuffer;
        p = (unsigned char*)file->buffer;

        while (n--)
            if (*p++ == '\n')
                position++;
    }

    return (position);
}

long ftell(FILE* file) {
    long retval;

    __begin_critical_region(files_access);

    retval = _ftell(file);

    __end_critical_region(files_access);

    return (retval);
}

int _fseek(FILE* file, fpos_t offset, int file_mode) {
    fpos_t position;
    __pos_proc pos_proc;

    if (file->mode.file_kind != __disk_file || file->state.error) {
        errno = EFPOS;
        return (-1);
    }

    if (file->state.io_state == __writing) {
        if (__flush_buffer(file, NULL)) {
            set_error(file);
            errno = EFPOS;
            return (-1);
        }
    }

    if (file_mode == SEEK_CUR) {
        file_mode = SEEK_SET;
        position = _ftell(file);
        offset += position;
    }

    if ((file_mode != SEEK_END) && (file->mode.io_mode != __read_write) &&
        ((file->state.io_state == __reading) || (file->state.io_state == __rereading))) {
        if ((offset >= file->position) || (offset < file->buffer_pos))
            file->state.io_state = __neutral;
        else {
            file->buffer_ptr = file->buffer + (offset - file->buffer_pos);
            file->buffer_len = file->position - offset;
            file->state.io_state = __reading;
        }
    } else
        file->state.io_state = __neutral;

    if (file->state.io_state == __neutral) {
        if ((pos_proc = file->position_proc) != NULL && (*pos_proc)(file->handle, &offset, file_mode, file->ref_con)) {
            set_error(file);
            errno = EFPOS;
            return (-1);
        } else {
            file->state.eof = 0;
            file->position = offset;
            file->buffer_len = 0;
        }
    }

    return (0);
}

int fseek(FILE* file, long offset, int file_mode) {
    fpos_t real_offset = (fpos_t)offset;
    int retval;

    __begin_critical_region(files_access);

    retval = _fseek(file, real_offset, file_mode);

    __end_critical_region(files_access);

    return (retval);
}
