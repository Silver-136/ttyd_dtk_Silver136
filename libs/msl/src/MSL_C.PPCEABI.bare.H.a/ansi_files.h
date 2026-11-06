#ifndef _MSL_ANSI_FILES_H
#define _MSL_ANSI_FILES_H

typedef unsigned long size_t;
typedef unsigned short wchar_t;
typedef unsigned long fpos_t;

#define NULL 0
#define EOF (-1)

typedef unsigned long __file_handle;

/* __file_modes.io_mode bits */
enum __io_modes { __read = 1, __write = 2, __read_write = 3, __append = 4 };

/* __file_modes.buffer_mode */
#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

/* fseek origins */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct {
    unsigned int open_mode : 2;
    unsigned int io_mode : 3;
    unsigned int buffer_mode : 2;
    unsigned int file_kind : 3;
    unsigned int file_orientation : 2;
    unsigned int binary_io : 1;
} __file_modes;

enum __file_kinds {
    __closed_file,
    __disk_file,
    __console_file,
    __unavailable_file
};

enum __file_orientation {
    __unoriented,
    __char_oriented,
    __wide_oriented
};

typedef struct {
    unsigned int io_state : 3;
    unsigned int free_buffer : 1;
    unsigned char eof;
    unsigned char error;
} __file_state;

enum __io_states {
    __neutral,
    __writing,
    __reading,
    __rereading
};

enum __io_results {
    __no_io_error,
    __io_error,
    __io_EOF
};

typedef int (*__pos_proc)(__file_handle file, fpos_t* position, int mode, void* ref_con);
typedef int (*__io_proc)(__file_handle file, unsigned char* buff, size_t* count, void* ref_con);
typedef int (*__close_proc)(__file_handle file);

#define __ungetc_buffer_size 2

typedef struct _FILE {
    __file_handle handle;
    __file_modes mode;
    __file_state state;
    unsigned char is_dynamically_allocated;
    unsigned char char_buffer;
    unsigned char char_buffer_overflow;
    unsigned char ungetc_buffer[__ungetc_buffer_size];
    wchar_t ungetwc_buffer[__ungetc_buffer_size];
    unsigned long position;
    unsigned char* buffer;
    unsigned long buffer_size;
    unsigned char* buffer_ptr;
    unsigned long buffer_len;
    unsigned long buffer_alignment;
    unsigned long saved_buffer_len;
    unsigned long buffer_pos;
    __pos_proc position_proc;
    __io_proc read_proc;
    __io_proc write_proc;
    __close_proc close_proc;
    void* ref_con;
    struct _FILE* next_file_struct;
} FILE;

extern FILE __files[];

void __prep_buffer(FILE* file);
int __flush_buffer(FILE* file, size_t* bytes_flushed);
int fflush(FILE* file);
int fclose(FILE* file);
void free(void* ptr);
void __begin_critical_region(int region);
void __end_critical_region(int region);

/* critical-region ids of the stream and file-list code (both 2; printf.c names it stdout_access) */
#define __stdio_access 2
#define files_access 2

/* Marks a stream as failed and drops its buffered data. */
#define set_error(file)          \
    do {                         \
        (file)->state.error = 1; \
        (file)->buffer_len = 0;  \
    } while (0)

#endif
