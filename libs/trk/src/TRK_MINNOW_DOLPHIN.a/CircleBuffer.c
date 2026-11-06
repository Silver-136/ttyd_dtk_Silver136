typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long size_t;

void* memcpy(void* dst, const void* src, size_t n);
void MWInitializeCriticalSection(unsigned int* section);
void MWEnterCriticalSection(unsigned int* section);
void MWExitCriticalSection(unsigned int* section);

/* Ring buffer: mBytesToRead = bytes stored, mBytesToWrite = free space. */
typedef struct CircleBuffer {
    u8* read_ptr;
    u8* write_ptr;
    u8* start_ptr;
    u32 size;
    u32 mBytesToRead;
    u32 mBytesToWrite;
    unsigned int mCriticalSection;
} CircleBuffer;

u32 CBGetBytesAvailableForRead(CircleBuffer* cb) {
    return cb->mBytesToRead;
}

void CircleBufferInitialize(CircleBuffer* cb, u8* buf, u32 size) {
    cb->start_ptr = buf;
    cb->size = size;
    cb->read_ptr = cb->start_ptr;
    cb->write_ptr = cb->start_ptr;
    cb->mBytesToRead = 0;
    cb->mBytesToWrite = cb->size;
    MWInitializeCriticalSection(&cb->mCriticalSection);
}

int CircleBufferWriteBytes(CircleBuffer* cb, u8* buf, u32 size) {
    u32 bytesToWrite;

    if (size > cb->mBytesToWrite)
        return -1;

    MWEnterCriticalSection(&cb->mCriticalSection);

    bytesToWrite = cb->size - (cb->write_ptr - cb->start_ptr);

    if (bytesToWrite >= size) {
        memcpy(cb->write_ptr, buf, size);
        cb->write_ptr += size;
    } else {
        memcpy(cb->write_ptr, buf, bytesToWrite);
        memcpy(cb->start_ptr, buf + bytesToWrite, size - bytesToWrite);
        cb->write_ptr = cb->start_ptr + size - bytesToWrite;
    }

    if (cb->size == cb->write_ptr - cb->start_ptr)
        cb->write_ptr = cb->start_ptr;

    cb->mBytesToWrite -= size;
    cb->mBytesToRead += size;

    MWExitCriticalSection(&cb->mCriticalSection);
    return 0;
}

int CircleBufferReadBytes(CircleBuffer* cb, u8* buf, u32 size) {
    u32 bytesToRead;

    if (size > cb->mBytesToRead)
        return -1;

    MWEnterCriticalSection(&cb->mCriticalSection);

    bytesToRead = cb->size - (cb->read_ptr - cb->start_ptr);

    if (size < bytesToRead) {
        memcpy(buf, cb->read_ptr, size);
        cb->read_ptr += size;
    } else {
        memcpy(buf, cb->read_ptr, bytesToRead);
        memcpy(buf + bytesToRead, cb->start_ptr, size - bytesToRead);
        cb->read_ptr = cb->start_ptr + size - bytesToRead;
    }

    if (cb->size == cb->read_ptr - cb->start_ptr)
        cb->read_ptr = cb->start_ptr;

    cb->mBytesToWrite += size;
    cb->mBytesToRead -= size;

    MWExitCriticalSection(&cb->mCriticalSection);
    return 0;
}
