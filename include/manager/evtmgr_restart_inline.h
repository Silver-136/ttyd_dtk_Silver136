#pragma once

#pragma warn_no_side_effect off
static inline void evtRestartMakeJumpTable(void* entry) {
    s32 params, opcode, label;
    s32 i, n;
    s32* cmd;
    s32 zero = 0;
    u8* tableEntry;
    s8* labelIds;
    i = 0;
    tableEntry = (u8*)entry;
    for (n = 0; n < 2; n++) {
        labelIds = (s8*)entry + i + 0x1C;
        labelIds[0] = -1;
        *(s32**)(tableEntry + 0x2C) = (s32*)zero;
        labelIds[1] = -1;
        *(s32**)(tableEntry + 0x30) = (s32*)zero;
        labelIds[2] = -1;
        *(s32**)(tableEntry + 0x34) = (s32*)zero;
        labelIds[3] = -1;
        *(s32**)(tableEntry + 0x38) = (s32*)zero;
        labelIds[4] = -1;
        *(s32**)(tableEntry + 0x3C) = (s32*)zero;
        labelIds[5] = -1;
        *(s32**)(tableEntry + 0x40) = (s32*)zero;
        labelIds[6] = -1;
        *(s32**)(tableEntry + 0x44) = (s32*)zero;
        labelIds[7] = -1;
        *(s32**)(tableEntry + 0x48) = (s32*)zero;
        i += 8;
        tableEntry += 0x20;
    }

    n = 0;
    cmd = *(s32**)((u8*)entry + 0x14);
    tableEntry = (u8*)entry;
    while (1) {
        opcode = *cmd & 0xFFFF;
        params = *cmd++ >> 16;
        label = *cmd;
        cmd += params;

        switch (opcode) {
            case 1:
                return;
            case 3:
                *(s8*)((u8*)entry + 0x1C + n) = (s8)label;
                *(s32**)(tableEntry + 0x2C) = cmd;
                n++;
                tableEntry += 4;
        }
        if (n >= 16) {
            "unused";
        }
    }

}
#pragma warn_no_side_effect reset

