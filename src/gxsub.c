#include "version.h"
#include "gxsub.h"

//.sdata
#if VERSION <= VERSION_DEMO_US
f32 queen_uv[4]
    ATTRIBUTE_ALIGN(32)
    = {0.0f, -0.0016000001f, 0.0f, -0.00080000004f};
#else
f32 queen_uv[4] = {0.0f, -0.0016000001f, 0.0f, -0.00080000004f};
#endif
