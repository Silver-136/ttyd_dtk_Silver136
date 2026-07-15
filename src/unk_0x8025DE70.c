#include "unk_0x8025DE70.h"

void N_setupErrorHandler(void) {
    extern void OSSetErrorHandler(s32 error, void* handler);
    extern void N_systemErrorHandler(void);

    OSSetErrorHandler(2, N_systemErrorHandler);
    OSSetErrorHandler(3, N_systemErrorHandler);
    OSSetErrorHandler(6, N_systemErrorHandler);
}


/* CHATGPT STUB FILL: main/unk_0x8025DE70 20260624_185035 */

/* stub-fill: N_systemErrorHandler | prototype_only | source_prototype */
void N_systemErrorHandler(u16 error, u8* context, u32 dsisr, u32 dar) {
    extern s32 N_getDebugMode(void);
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern s32 sprintf(char* dst, const char* fmt, ...);
    extern void psndExit(void);
    extern void PPCHalt(void);
    char message[4096];
    char line[1024];
    u32* backchain;
    s32 i;
    if (N_getDebugMode() < 0) {
        if (error == 2) strcpy(message, "OS ERROR: DSI\n");
        else if (error == 3) strcpy(message, "OS ERROR: ISI\n");
        else if (error == 6) strcpy(message, "OS ERROR: PROGRAM\n");
        else strcpy(message, "OS ERROR\n");
        sprintf(line, "Context: 0x%08x\n", context);
        strcat(message, line);
        for (i = 0; i < 16; i++) {
            sprintf(line, "r%2d 0x%08x  r%2d 0x%08x\n", i,
                    *(u32*)(context + i * 4), i + 16,
                    *(u32*)(context + 0x40 + i * 4));
            strcat(message, line);
        }
        sprintf(line, "LR 0x%08x CR 0x%08x\n", *(u32*)(context + 0x84), *(u32*)(context + 0x80));
        strcat(message, line);
        sprintf(line, "SRR0 0x%08x SRR1 0x%08x\n", *(u32*)(context + 0x198), *(u32*)(context + 0x19C));
        strcat(message, line);
        sprintf(line, "DSISR 0x%08x DAR 0x%08x\n", dsisr, dar);
        strcat(message, line);
        strcat(message, "\nAddress Back Chain LR Save\n");
        backchain = *(u32**)(context + 4);
        for (i = 0; backchain != 0 && backchain != (u32*)-1 && i < 16; i++) {
            sprintf(line, "0x%08x 0x%08x 0x%08x\n", backchain, backchain[0], backchain[1]);
            strcat(message, line);
            backchain = (u32*)backchain[0];
        }
        sprintf(line, "Instruction at 0x%08x DSISR 0x%08x\n", *(u32*)(context + 0x198), dar);
        strcat(message, line);
    }
    psndExit();
    PPCHalt();
    for (;;) {}
}

