extern void __TRK_copy_vectors(void);

__declspec(section ".init")
void __TRK_reset(void) {
    OSResetSystem(0, 0, 0);
}
