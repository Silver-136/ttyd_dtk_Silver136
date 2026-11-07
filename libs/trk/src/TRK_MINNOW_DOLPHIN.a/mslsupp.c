#include "trk.h"

typedef unsigned long size_t;
typedef int BOOL;

BOOL GetTRKConnected(void);
unsigned char GetUseSerialIO(void);
unsigned char TRKAccessFile(unsigned int cmd, unsigned int file_handle, size_t* length, unsigned char* buffer);
unsigned char TRKCloseFile(unsigned int cmd, unsigned int file_handle);

int __access_file(unsigned int handle, unsigned char* buffer, size_t* count, void* ref_con,
                                unsigned int cmd) {
    size_t countTemp;
    unsigned char ioResult;

    if (GetTRKConnected() == 0)
        return DS_IOError;

    countTemp = *count;
    ioResult = TRKAccessFile(cmd, handle, &countTemp, buffer);
    *count = countTemp;

    switch (ioResult) {
    case DS_IONoError:
        return DS_IONoError;
    case DS_IOEOF:
        return DS_IOEOF;
    }

    return DS_IOError;
}

int __read_console(unsigned long handle, unsigned char* buffer, size_t* count, void* ref_con) {
    if (GetUseSerialIO() == 0)
        return DS_IOError;
    return __access_file(DS_Stdin, buffer, count, ref_con, DSMSG_ReadFile);
}

int __TRK_write_console(unsigned long handle, unsigned char* buffer, size_t* count, void* ref_con) {
    if (GetUseSerialIO() == 0)
        return DS_IOError;
    return __access_file(DS_Stdout, buffer, count, ref_con, DSMSG_WriteFile);
}

int __close_console(unsigned long handle) {
    unsigned int ioResult;

    if (GetTRKConnected() == 0)
        return DS_IOError;

    ioResult = TRKCloseFile(DSMSG_CloseFile, handle);

    switch ((unsigned char)ioResult) {
    case DS_IONoError:
        return DS_IONoError;
    case DS_IOEOF:
        return DS_IOEOF;
    }

    return DS_IOError;
}
