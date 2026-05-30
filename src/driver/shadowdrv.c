#include "driver/shadowdrv.h"

void* cswp;
void* pswp;
void* dswp;
u8 rampTex16[0x20];
u8 rampTex8[0x20];

extern u8 shadowConfig[];
extern void* smartTexObj(void*, s32);

void* shadowGetProjShadowConfig(void) {
    return shadowConfig + (*(s32*)((s32)pswp + 8) * 0x48);
}

u32 shadowGetProjShadowColor(void) {
    return *(u32*)((s32)pswp + 0x100);
}

void* shadowGetProjShadowProjTexMtx(void) {
    return (void*)((s32)pswp + 0x7C);
}

void* shadowGetProjShadowTexObj(void) {
    void* wp = pswp;
    if ((*(u32*)wp & 1) != 0) {
        return smartTexObj((void*)((s32)wp + 0xDC), *(s32*)((s32)wp + 0xFC));
    }
    return 0;
}

void shadowProjShadowDrawCountUp(void) {
    (*(s32*)((s32)pswp + 4))++;
}

void* shadowGetDepthShadowConfig(void) {
    return shadowConfig + (*(s32*)((s32)dswp + 8) * 0x48);
}

void* shadowGetDepthShadowProjTexMtx(void) {
    return (void*)((s32)dswp + 0x7C);
}

void* shadowGetDepthShadowDepthMtx(void) {
    return (void*)((s32)dswp + 0xAC);
}

void* shadowGetDepthShadowTexObj(void) {
    void* wp = dswp;
    if ((*(u32*)wp & 1) != 0) {
        return smartTexObj((void*)((s32)wp + 0xDC), *(s32*)((s32)wp + 0xFC));
    }
    return 0;
}

void shadowDepthShadowDrawCountUp(void) {
    (*(s32*)((s32)dswp + 4))++;
}

void* shadowGetRamp16TexObj(void) {
    return rampTex16;
}

void* shadowGetRamp8TexObj(void) {
    return rampTex8;
}

void* shadowGetCharShadowProjection(void) {
    return *(void**)((s32)cswp + 0x10C);
}

u32 shadowGetCharShadowColor(void) {
    return *(u32*)((s32)cswp + 0x100);
}

void* shadowGetCharShadowProjTexMtx(void) {
    return (void*)((s32)cswp + 0x7C);
}

void* shadowGetCharShadowTexObj(void) {
    void* wp = cswp;
    if ((*(u32*)wp & 1) != 0) {
        return smartTexObj((void*)((s32)wp + 0xDC), *(s32*)((s32)wp + 0xFC));
    }
    return 0;
}

void shadowFlagOn(s32 id, u16 flag) {
    u16* flags = (u16*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C);
    *flags |= flag;
}

void shadowSetCollision(s32 id, s32 on) {
    u16* flags = (u16*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C);
    if (on != 0) {
        *flags |= 1;
    } else {
        *flags &= ~1;
    }
}

void shadowSetCamId(s32 id, s16 camId) {
    *(s16*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C + 0x1A) = camId;
}

void shadowSetSize(s32 id, f32 size) {
    *(f32*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C + 0x10) = size;
}

void shadowSetVolumeDecay(s32 id, f32 a, f32 b, f32 c) {
    void* entry = (void*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C);
    *(f32*)((s32)entry + 0x1C) = a;
    *(f32*)((s32)entry + 0x20) = b;
    *(f32*)((s32)entry + 0x24) = c;
}

void U_shadowSetMode(s32 id, u8 mode) {
    *(u8*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C + 0x28) = mode;
}

void shadowSetType(s32 id, u8 type) {
    *(u8*)((s32)*(void**)((s32)cswp + 0x108) + id * 0x5C + 3) = type;
}


u8 shadowDisp(int param_1, s32 param_2) {
    return 0;
}


u8 shadowCharShadowDisp_Projection(void) {
    return 0;
}


u8 shadowInit(void) {
    return 0;
}


u8 shadowCharShadowDisp_Texture(void) {
    return 0;
}


u8 cylinderShadowDraw(void) {
    return 0;
}


u8 depthShadowEnd(void) {
    return 0;
}


u8 shadowCharShadowDisp_Polygon(void) {
    return 0;
}


u8 projShadowEnd(s32 param_1, u32* param_2) {
    return 0;
}


u8 shadowCharShadowDisp_Collision(void) {
    return 0;
}


u8 cylinder(void) {
    return 0;
}


u8 shadowMain(void) {
    return 0;
}


u8 shadowEntry(double param_1, double param_2, double param_3, double param_4) {
    return 0;
}


int shadowEntryMode(double param_1, double param_2, double param_3, double param_4, u8 param_5) {
    return 0;
}


u32 shadowGetDepthShadowColor(void) {
    return 0;
}


s32 _filter(s32 param_1, int param_2) {
    return 0;
}
