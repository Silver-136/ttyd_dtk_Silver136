#include <dolphin.h>

// Weak default: reports that the AMC code is not a stub (0); amcstubs/AmcExi2Stubs.c has the strong version
// that returns 1.
__declspec(weak) int AMC_IsStub(void);

__declspec(weak) int AMC_IsStub(void) {
    return 0;
}
