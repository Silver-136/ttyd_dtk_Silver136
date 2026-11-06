#include "ansi_files.h"

int fwide(FILE* stream, int orientation) {
    if (stream == NULL || stream->mode.file_kind == __closed_file)
        return 0;

    // each of the three orientations returns; there is no return after the switch
    switch (stream->mode.file_orientation) {
        case __unoriented:
            if (orientation > 0)
                stream->mode.file_orientation = __wide_oriented;
            else if (orientation < 0)
                stream->mode.file_orientation = __char_oriented;
            return orientation;
        case __wide_oriented:
            return 1;
        case __char_oriented:
            return -1;
    }
}
