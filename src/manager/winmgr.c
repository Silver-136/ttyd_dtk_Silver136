#include "manager/winmgr.h"

void* wp;
void* __memAlloc(s32 heap, u32 size);
void* memset(void* dst, int val, unsigned long size);
void __memFree(s32 heap, void* ptr);
extern u8 itemDataTable[];
char* msgSearch(char* msg);
void winMgrHelpDraw(void* win);

/* Reorder pilot candidate: functions arranged in _main.map order for main/manager/winmgr. */

void select_disp3_mario(void* win) {
    extern void* pouchGetPtr(void);
    extern s32 sprintf(char* str, const char* fmt, ...);
    extern char* msgSearch(char* msg);
    extern void winMgrHelpDraw(void* win);
    extern char vec3_803003a0[];
    extern char str_help_80428008[];
    extern char* mario_status_name_table[];

    char buf[0x50];
    void* data;
    void* list;
    u16 flags;
    s32 index;
    char* label;
    u16 status;

    data = *(void**)((s32)win + 0x2C);
    pouchGetPtr();
    if ((*(u32*)((s32)*(void**)((s32)wp + 4) + *(s32*)((s32)data + 0x2C) * 0x44) & 4) == 0) {
        index = *(s32*)((s32)data + 0xC);
        list = *(void**)((s32)data + 0x30);
        flags = *(u16*)((s32)list + (index << 2));
        if (flags & 2) {
            if (flags & 4) {
                label = &vec3_803003a0[0x300];
            } else {
                label = &vec3_803003a0[0x30C];
            }
        } else {
            label = str_help_80428008;
        }
        status = *(u16*)((s32)list + (index << 2) + 2);
        if (*(s32*)((s32)data + 4) == 9) {
            sprintf(buf, &vec3_803003a0[0x314], label, mario_status_name_table[status]);
        } else {
            sprintf(buf, &vec3_803003a0[0x324], label, mario_status_name_table[status]);
        }
        *(char**)((s32)win + 0x34) = msgSearch(buf);
        winMgrHelpDraw(win);
    }
}

void select_disp3_party(void* win) {
    extern void* pouchGetPtr(void);
    extern s32 sprintf(char* str, const char* fmt, ...);
    extern char* msgSearch(char* msg);
    extern void winMgrHelpDraw(void* win);
    extern char vec3_803003a0[];
    extern char str_help_80428008[];
    extern u16 party_id_table[];
    extern char* party_labelname_table[];

    char buf[0x50];
    void* data;
    void* list;
    void* pouch;
    u16 flags;
    u16 party;
    char* label;
    char* state;
    s32 index;
    s32 count;

    data = *(void**)((s32)win + 0x2C);
    pouch = pouchGetPtr();
    if ((*(u32*)((s32)*(void**)((s32)wp + 4) + *(s32*)((s32)data + 0x2C) * 0x44) & 4) == 0) {
        index = *(s32*)((s32)data + 0xC);
        list = *(void**)((s32)data + 0x30);
        flags = *(u16*)((s32)list + (index << 2));
        label = str_help_80428008;
        if (flags & 2) {
            label = &vec3_803003a0[0x30C];
            if (*(s32*)((s32)data + 4) == 6) {
                state = &vec3_803003a0[0x334];
            } else {
                state = &vec3_803003a0[0x33C];
            }
            party = *(u16*)((s32)list + (index << 2) + 2);
            count = *(s16*)((s32)pouch + party_id_table[party] * 0xE + 0xC) + 1;
            sprintf(buf, &vec3_803003a0[0x348], label, state, count, party_labelname_table[party]);
            *(char**)((s32)win + 0x34) = msgSearch(buf);
        } else {
            *(char**)((s32)win + 0x34) = msgSearch(label);
        }
        winMgrHelpDraw(win);
    }
}

void select_disp3(void* win) {
    typedef struct SelectEntry {
        u16 unk0;
        u16 item;
    } SelectEntry;

    void* data = *(void**)((s32)win + 0x2C);
    s32 id = *(s32*)((s32)data + 0x2C);
    void* mgr = *(void**)((s32)wp + 4);

    if ((*(u32*)((s32)mgr + id * 0x44) & 4) == 0) {
        s32 index = *(s32*)((s32)data + 0xC);
        u8* table = (u8*)itemDataTable;
        SelectEntry* list = *(SelectEntry**)((s32)data + 0x30);
        u16 item = list[index].item;

        *(char**)((s32)win + 0x34) = msgSearch(*(char**)(table + item * 0x28 + 8));
        winMgrHelpDraw(win);
    }
}

void select_main3(void* win) {
    extern void FontGetMessageWidthLine(char* msg, u16* lines);
    extern u32 keyGetDirTrg(s32 controller);

    u16 lines;
    s32 b;
    s32 a;

    lines = 0;
    if ((*(u32*)win & 4) == 0) {
        if (*(char**)((s32)win + 0x34) != 0) {
            FontGetMessageWidthLine(*(char**)((s32)win + 0x34), &lines);
            *(s32*)((s32)win + 0x3C) = lines + 1;
        }
        if (*(s32*)((s32)win + 0x3C) > 2) {
            if (keyGetDirTrg(0) & 0x100) {
                *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) - 2;
                if (*(s32*)((s32)win + 0x38) < 0) {
                    *(s32*)((s32)win + 0x38) = 0;
                }
            } else if (keyGetDirTrg(0) & 0x200) {
                a = *(s32*)((s32)win + 0x3C) + 1;
                b = *(s32*)((s32)win + 0x38) + 1;
                if (((s32)(a + ((u32)a >> 31)) >> 1) - 1 > ((s32)(b + ((u32)b >> 31)) >> 1)) {
                    *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) + 2;
                }
            }
        }
    }
}

void select_disp2(void* win) {
    extern char* msgSearch(char*);
    extern u32 FontGetMessageWidthLine(char*, u16*);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void FontDrawStart(void);
    extern void FontDrawMessageMtx(f32[3][4], char*);
    extern char vec3_803003a0[];

    f32 scaleMtx[3][4];
    f32 transMtx[3][4];
    void* select = *(void**)((s32)win + 0x2C);
    char* message = msgSearch(&vec3_803003a0[0x358]);
    void* entries = *(void**)((s32)wp + 4);
    void* linked;
    u16 lines;
    u32 messageWidth;
    f32 drawnWidth;
    f32 scale;
    f32 x;
    f32 y;
    s32 type = *(s32*)((s32)select + 4);
    s32 index;

    switch (type) {
        case 3:
            message = msgSearch(&vec3_803003a0[0x36C]);
            break;
        case 4:
            message = msgSearch(&vec3_803003a0[0x380]);
            break;
        case 5:
            message = msgSearch(&vec3_803003a0[0x394]);
            break;
        case 0xB:
            message = msgSearch(&vec3_803003a0[0x404]);
            break;
    }

    index = *(s32*)((s32)select + 0x28);
    linked = (void*)((s32)entries + index * 0x44);
    if ((*(u32*)linked & 4) == 0) {
        messageWidth = FontGetMessageWidthLine(message, &lines);
        drawnWidth = (f32)messageWidth;
        scale = 1.0f;
        if (drawnWidth > (f32)(*(s32*)((s32)win + 0x20) - 30)) {
            scale = (f32)(*(s32*)((s32)win + 0x20) - 30) / drawnWidth;
            drawnWidth = (f32)(*(s32*)((s32)win + 0x20) - 30);
        }

        PSMTXScale(scaleMtx, scale, 1.0f, 1.0f);
        x = (f32)*(s32*)((s32)win + 0x18) +
            ((f32)*(s32*)((s32)win + 0x20) - drawnWidth) * 0.5f;
        y = (f32)(*(s32*)((s32)win + 0x1C) -
            (*(s32*)((s32)win + 0x24) - (lines + 1) * 24) / 2);
        PSMTXTrans(transMtx, x, y, 0.0f);
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        FontDrawStart();
        FontDrawMessageMtx(transMtx, message);

        if ((*(u32*)linked & 1) != 0) {
            void* desc = *(void**)((s32)win + 0x28);
            *(s32*)((s32)linked + 0x18) = *(s32*)((s32)win + 0x18);
            *(s32*)((s32)linked + 0x1C) =
                *(s32*)((s32)desc + 0x10) + ((u32)lines * 22 >> 1);
            *(s32*)((s32)linked + 0x20) = *(s32*)((s32)win + 0x20);
            *(s32*)((s32)linked + 0x24) =
                *(s32*)((s32)desc + 0x18) + (u32)lines * 22;
        }
    }
}

void select_disp_luigi(void* win) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void GXGetScissor(u32*, u32*, u32*, u32*);
    extern void GXSetScissor(u32, u32, u32, u32);
    extern void winFontInit(void);
    extern void winFontSetWidth(Vec*, Vec*, u32*, f32, char*, ...);
    extern void winFontSet(Vec*, Vec*, u32*, char*, ...);
    extern void winFontSetEdgeWidth(Vec*, Vec*, u32*, f32, char*, ...);
    extern char* msgSearch(char*);
    extern u16 FontGetMessageWidth(char*);
    extern void iconDispGx(f64, Vec*, u16, u16);
    extern s32 sprintf(char*, const char*, ...);
    extern void* gpGlobals;
    extern char str_msg_window_title_5_80300808[];
    extern char str_msg_window_title_6_8030081c[];
    u8* entry = (u8*)win;
    u8* select = *(u8**)(entry + 0x2C);
    u8* rows = *(u8**)(select + 0x30);
    u8* text = *(u8**)((u8*)wp + 4);
    s32 rowCount = *(s32*)(select + 0x20);
    s32 rowOffset = *(s32*)(select + 0x10);
    f32 listYOffset = *(f32*)(select + 0x18);
    s32 type = *(s32*)(select + 4);
    u32 oldX, oldY, oldW, oldH;
    u32 normal = 0xFFFFFFFF;
    u32 disabled = 0x808080FF;
    u32 titleColor = 0xFFFFFFFF;
    Vec pos, scale;
    char number[32];
    char* title;
    f32 titleWidth;
    s32 i;

    if ((*(u32*)(text + *(s32*)(select + 0x2C) * 0x44) & 4) != 0)
        return;
    GXGetScissor(&oldX, &oldY, &oldW, &oldH);
    GXSetScissor(*(s32*)(entry + 0x18) + 0x130,
                 0x112 - *(s32*)(entry + 0x1C),
                 *(s32*)(entry + 0x20), *(s32*)(entry + 0x24) - 0x32);
    scale.x = scale.y = scale.z = 1.0f;
    for (i = 0; i < rowCount; i++) {
        f32 y = listYOffset + (f32)(*(s32*)(entry + 0x1C) - 0x20 - i * 0x18);
        if (y - 32.0f <= (f32)*(s32*)(entry + 0x1C) &&
            y + 32.0f >= (f32)(*(s32*)(entry + 0x1C) - *(s32*)(entry + 0x24))) {
            u32 color = (*(u16*)(rows + i * 4) & 1) ? disabled : normal;
            char* message = *(char**)(text + i * 0x10);
            pos.x = (f32)(*(s32*)(entry + 0x18) + 0x19);
            pos.y = y;
            pos.z = 0.0f;
            winFontInit();
            winFontSetWidth(&pos, &scale, &color, 200.0f, message);
            if (type == 0x11 && (*(u16*)(rows + i * 4) & 1) == 0) {
                sprintf(number, "%3d", *(s16*)(text + i * 0x10 + 0xA));
                pos.x = (f32)(*(s32*)(entry + 0x18) + 0xF5);
                winFontInit();
                winFontSet(&pos, &scale, &color, number);
            }
        }
    }
    GXSetScissor(oldX, oldY, oldW, oldH);
    title = msgSearch(type == 0x11 ? str_msg_window_title_5_80300808 : str_msg_window_title_6_8030081c);
    titleWidth = (f32)FontGetMessageWidth(title);
    if (titleWidth < 120.0f) titleWidth = 120.0f;
    pos.x = (f32)*(s32*)(entry + 0x18) + ((f32)*(s32*)(entry + 0x20) - titleWidth) * 0.5f;
    pos.y = (f32)(*(s32*)(entry + 0x1C) + 0xE);
    pos.z = 0.0f;
    winFontInit();
    winFontSetEdgeWidth(&pos, &scale, &titleColor, 120.0f, title);
    pos.x = *(f32*)(select + 0x24);
    pos.y = *(f32*)(select + 0x28);
    iconDispGx(1.0, &pos, 0x14, 0x1F8);
    if (rowCount > 8 && ((*(u32*)((u8*)gpGlobals + 0x10) & 0x1F) < 0x14)) {
        pos.x = (f32)*(s32*)(entry + 0x18) + (f32)*(s32*)(entry + 0x20) * 0.5f;
        if (rowOffset != 0) {
            pos.y = (f32)(*(s32*)(entry + 0x1C) - 0x24);
            iconDispGx(0.6, &pos, 0x10, 0x1BD);
        }
        if (rowOffset != rowCount - 8) {
            pos.y = (f32)(*(s32*)(entry + 0x1C) - *(s32*)(entry + 0x24) - 0xC);
            iconDispGx(0.6, &pos, 0x10, 0x1BE);
        }
    }
}

void select_disp_mario(void* win) {
    typedef struct { f32 x, y, z; } Vec;
    extern void* wp;
    extern void pouchGetPtr(void);
    extern void GXGetScissor(s32*, s32*, s32*, s32*);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void winFontInit(void);
    extern void winFontSet(Vec*, Vec*, void*, char*);
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32, Vec*, Vec*, void*);
    extern s32 sprintf(char*, char*, ...);
    extern char* mario_status_name_table[];
    extern s32 mario_status_icon_table[];
    extern s16 mario_status_point_table[];
    u8* w = win;
    u8* sel = *(u8**)(w + 0x2C);
    u8* rows = *(u8**)(sel + 0x30);
    s32 oldX, oldY, oldW, oldH;
    s32 i;
    Vec pos, fontScale, iconScale;
    u32 color = 0xFFFFFFFF;
    char buf[16];

    pouchGetPtr();
    GXGetScissor(&oldX, &oldY, &oldW, &oldH);
    GXSetScissor(*(s32*)(w + 0x18) + 0x130, 0x112 - *(s32*)(w + 0x1C), *(s32*)(w + 0x20), *(s32*)(w + 0x24) - 0x32);
    fontScale.x = fontScale.y = fontScale.z = 1.0f;
    iconScale.x = iconScale.y = iconScale.z = 1.0f;
    for (i = 0; i < *(s32*)(sel + 0x34); i++) {
        u16 flags = *(u16*)(rows + i * 4);
        u16 value = *(u16*)(rows + i * 4 + 2);
        f32 rowY = *(f32*)(sel + 0x1C) + (f32)(*(s32*)(w + 0x1C) - 0x2C - i * 24);
        if (rowY - 32.0f <= (f32)*(s32*)(w + 0x1C) && rowY + 32.0f >= (f32)(*(s32*)(w + 0x1C) - *(s32*)(w + 0x24))) {
            winFontInit();
            pos.x = (f32)(*(s32*)(w + 0x18) + 0x3C); pos.y = rowY + 12.0f; pos.z = 0.0f;
            winFontSet(&pos, &fontScale, &color, mario_status_name_table[value * 3]);
            if (flags & 1) winIconGrayInit(); else winIconInit();
            pos.x = (f32)(*(s32*)(w + 0x18) + 0x23); pos.y = rowY; pos.z = 0.0f;
            winIconSet(mario_status_icon_table[value], &pos, &iconScale, &color);
            sprintf(buf, "%3d", mario_status_point_table[value]);
            winFontInit();
            pos.x = (f32)(*(s32*)(w + 0x18) + 0xA0); pos.y = rowY + 12.0f;
            winFontSet(&pos, &fontScale, &color, buf);
        }
    }
    GXSetScissor(oldX, oldY, oldW, oldH);
}

void select_disp_party(void* win) {
    typedef struct { f32 x, y, z; } Vec;
    extern void* pouchGetPtr(void);
    extern char* pouchGetYoshiName(void);
    extern char* msgSearch(char*);
    extern u16 FontGetMessageWidth(char*);
    extern void GXGetScissor(s32*, s32*, s32*, s32*);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void winFontInit(void);
    extern void winFontSet(Vec*, Vec*, void*, char*);
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32, Vec*, Vec*, void*);
    extern char name_party0[], name_party1[], name_party2[], name_party4[], name_party5[], name_party6[];
    u8* w = win;
    u8* sel = *(u8**)(w + 0x2C);
    u8* rows = *(u8**)(sel + 0x30);
    char* names[7];
    s32 oldX, oldY, oldW, oldH, i;
    Vec pos, textScale, iconScale;
    u32 color = 0xFFFFFFFF;

    pouchGetPtr();
    names[0]=msgSearch(name_party0); names[1]=msgSearch(name_party1); names[2]=msgSearch(name_party4);
    names[3]=pouchGetYoshiName(); names[4]=msgSearch(name_party5); names[5]=msgSearch(name_party2); names[6]=msgSearch(name_party6);
    GXGetScissor(&oldX,&oldY,&oldW,&oldH);
    GXSetScissor(*(s32*)(w+0x18)+0x130,0x112-*(s32*)(w+0x1C),*(s32*)(w+0x20),*(s32*)(w+0x24)-0x32);
    iconScale.x=iconScale.y=iconScale.z=1.0f;
    for(i=0;i<*(s32*)(sel+0x34);i++) {
        u16 flags=*(u16*)(rows+i*4), value=*(u16*)(rows+i*4+2);
        f32 rowY=*(f32*)(sel+0x1C)+(f32)(*(s32*)(w+0x1C)-0x2C-i*24);
        if(rowY-32.0f <= (f32)*(s32*)(w+0x1C) && rowY+32.0f >= (f32)(*(s32*)(w+0x1C)-*(s32*)(w+0x24))) {
            textScale.x=textScale.y=textScale.z=1.0f;
            if(FontGetMessageWidth(names[value])>170) textScale.x=170.0f/(f32)FontGetMessageWidth(names[value]);
            winFontInit(); pos.x=(f32)(*(s32*)(w+0x18)+0x3C); pos.y=rowY+12.0f; pos.z=0.0f;
            winFontSet(&pos,&textScale,&color,names[value]);
            if(flags&1) winIconGrayInit(); else winIconInit();
            pos.x=(f32)(*(s32*)(w+0x18)+0x23); pos.y=rowY; pos.z=0.0f;
            winIconSet(value+0x120,&pos,&iconScale,&color);
        }
    }
    GXSetScissor(oldX,oldY,oldW,oldH);
}

void select_disp(void* win) {
    typedef struct { f32 x, y, z; } Vec;
    extern u8 itemDataTable[];
    extern char* msgSearch(char*);
    extern void GXGetScissor(s32*,s32*,s32*,s32*);
    extern void GXSetScissor(s32,s32,s32,s32);
    extern void winFontInit(void);
    extern void winFontSetWidth(Vec*,Vec*,void*,char*,f32);
    extern void winIconInit(void);
    extern void winIconGrayInit(void);
    extern void winIconSet(s32,Vec*,Vec*,void*);
    u8* w=win;
    u8* sel=*(u8**)(w+0x2C);
    u8* rows=*(u8**)(sel+0x30);
    s32 oldX,oldY,oldW,oldH,i;
    Vec pos,scale;
    u32 color=0xFFFFFFFF;
    GXGetScissor(&oldX,&oldY,&oldW,&oldH);
    GXSetScissor(*(s32*)(w+0x18)+0x130,0x112-*(s32*)(w+0x1C),*(s32*)(w+0x20),*(s32*)(w+0x24)-0x32);
    scale.x=scale.y=scale.z=1.0f;
    for(i=0;i<*(s32*)(sel+0x34);i++) {
        u16 flags=*(u16*)(rows+i*4), value=*(u16*)(rows+i*4+2);
        u8* item=itemDataTable+value*0x28;
        f32 rowY=*(f32*)(sel+0x1C)+(f32)(*(s32*)(w+0x1C)-0x2C-i*24);
        if(rowY-32.0f <= (f32)*(s32*)(w+0x1C) && rowY+32.0f >= (f32)(*(s32*)(w+0x1C)-*(s32*)(w+0x24))) {
            if(flags&2) { if(flags&1) winIconGrayInit(); else winIconInit(); pos.x=(f32)(*(s32*)(w+0x18)+0xF); pos.y=rowY; pos.z=0.0f; winIconSet(0x9A,&pos,&scale,&color); }
            if(flags&1) winIconGrayInit(); else winIconInit();
            pos.x=(f32)(*(s32*)(w+0x18)+0x23); pos.y=rowY; pos.z=0.0f;
            winIconSet(*(s16*)(item+0x20),&pos,&scale,&color);
            winFontInit(); pos.x=(f32)(*(s32*)(w+0x18)+0x3C); pos.y=rowY+12.0f;
            winFontSetWidth(&pos,&scale,&color,msgSearch(*(char**)(item+8)),185.0f);
        }
    }
    GXSetScissor(oldX,oldY,oldW,oldH);
}

void select_main(void* win) {
    extern void psndSFXOn(s32);
    extern void statusWinForceOpen(void);
    extern void statusWinForceOff(void);
    extern u32 keyGetDirRep(s32);
    extern u32 keyGetDirTrg(s32);
    extern u16 keyGetButtonTrg(s32);

    void* select = *(void**)((s32)win + 0x2C);
    void* entries = *(void**)((s32)wp + 4);
    s32 state = *(s32*)((s32)select + 8);
    s32 count = *(s32*)((s32)select + 0x20);
    s32* entryIndices = (s32*)((s32)select + 0x24);
    s32 i;

    if (state == 2) {
        for (i = 0; i < count; i++) {
            void* entry = (void*)((s32)entries + entryIndices[i] * 0x44);
            if ((*(u32*)entry & 1) != 0) {
                *(s32*)((s32)entry + 4) = 2;
                *(s32*)((s32)entry + 8) = 0;
            }
        }
        psndSFXOn(0x2002B);
        statusWinForceOff();
        *(s32*)((s32)select + 8) += 1;
    } else if (state < 2) {
        if (state == 0) {
            for (i = 0; i < count; i++) {
                void* entry = (void*)((s32)entries + entryIndices[i] * 0x44);
                if ((*(u32*)entry & 1) != 0) {
                    *(s32*)((s32)entry + 4) = 1;
                    *(s32*)((s32)entry + 8) = 0;
                    *(s32*)((s32)entry + 0x38) = 0;
                    *(s32*)((s32)entry + 0x3C) = 0;
                    *(void**)((s32)entry + 0x34) = 0;
                }
            }
            if (count > 2) {
                void* help = (void*)((s32)entries + entryIndices[2] * 0x44);
                *(s32*)((s32)help + 0x38) = 0;
                *(s32*)((s32)help + 0x3C) = 0;
                *(void**)((s32)help + 0x34) = 0;
            }
            psndSFXOn(0x2002A);
            statusWinForceOpen();
            *(s32*)((s32)select + 8) += 1;
        } else if (state >= 0) {
            void* first = (void*)((s32)entries + entryIndices[0] * 0x44);
            if ((*(u32*)first & 4) == 0) {
                s32 previous = *(s32*)((s32)select + 0xC);
                s32 cursor;
                s32 rowCount = *(s32*)((s32)select + 0x34);
                s32 offset;
                u16 rowFlags;

                if (keyGetDirRep(0) & 0x2000) {
                    *(s32*)((s32)select + 0xC) += 1;
                }
                if (keyGetDirRep(0) & 0x1000) {
                    *(s32*)((s32)select + 0xC) -= 1;
                }
                cursor = *(s32*)((s32)select + 0xC);
                if (cursor >= rowCount) {
                    if ((keyGetDirTrg(0) & 0x2000) == 0) {
                        *(s32*)((s32)select + 0xC) -= 1;
                    } else {
                        *(s32*)((s32)select + 0xC) = 0;
                        *(s32*)((s32)select + 0x10) = 0;
                    }
                }
                if (*(s32*)((s32)select + 0xC) < 0) {
                    if ((keyGetDirTrg(0) & 0x1000) == 0) {
                        *(s32*)((s32)select + 0xC) += 1;
                    } else {
                        *(s32*)((s32)select + 0xC) = rowCount - 1;
                        if (rowCount - 8 > 0) {
                            *(s32*)((s32)select + 0x10) = rowCount - 8;
                        }
                    }
                }

                cursor = *(s32*)((s32)select + 0xC);
                offset = *(s32*)((s32)select + 0x10);
                if (previous < cursor) {
                    if (cursor > 5 && offset < (u32)(rowCount - 8)) {
                        *(s32*)((s32)select + 0x10) = offset + 1;
                    }
                } else if (cursor < previous && cursor - offset < 3 && offset > 0) {
                    *(s32*)((s32)select + 0x10) = offset - 1;
                }
                if (previous != cursor) {
                    psndSFXOn(0x20005);
                }

                if (keyGetButtonTrg(0) & 0x100) {
                    rowFlags = *(u16*)((s32)*(void**)((s32)select + 0x30) + cursor * 4);
                    if ((rowFlags & 1) == 0) {
                        psndSFXOn(0x20012);
                        *(s32*)((s32)select + 8) += 1;
                    } else {
                        psndSFXOn(0x20014);
                    }
                } else if ((keyGetButtonTrg(0) & 0x200) && (*(u16*)select & 0x100)) {
                    psndSFXOn(0x20013);
                    *(u16*)select |= 0x2000;
                    *(s32*)((s32)select + 8) += 1;
                }

                offset = *(s32*)((s32)select + 0x10);
                *(f32*)((s32)select + 0x1C) +=
                    ((f32)offset * 24.0f - *(f32*)((s32)select + 0x1C)) / 6.0f;
                *(f32*)((s32)select + 0x14) +=
                    ((f32)*(s32*)((s32)win + 0x18) - *(f32*)((s32)select + 0x14)) / 6.0f;
                *(f32*)((s32)select + 0x18) +=
                    (-((f32)(cursor - offset) * 24.0f -
                       (f32)(*(s32*)((s32)win + 0x1C) - 54)) -
                     *(f32*)((s32)select + 0x18)) / 6.0f;
            }
        }
    } else if (state != 4 && state < 4) {
        void* first = (void*)((s32)entries + entryIndices[0] * 0x44);
        if ((*(u32*)first & 4) == 0) {
            for (i = 0; i < count; i++) {
                void* entry = (void*)((s32)entries + entryIndices[i] * 0x44);
                *(u32*)entry &= ~1;
            }
            *(u16*)select |= 0x1000;
            *(s32*)((s32)select + 8) += 1;
        }
    }
}

s32 unk_8023cf04(void* win) {
    return *(s32*)((s32)win + 0xC);
}

void winMgrSelectDelete(void* select) {
    register s32 keep = (s32)select;

    __memFree(0, *(void**)(keep + 0x30));
    __memFree(0, (void*)keep);
}

s32 winMgrSelectOther(void* select, void* event) {
    extern char* msgSearch(char*);
    extern u8 itemDataTable[];

    u16 flags = *(u16*)select;
    s32* lwData = (s32*)((s32)event + 0x9C);
    s32 cursor;
    u16 value;
    u8* item;

    if ((flags & 0x1000) == 0) {
        return 0;
    }
    if ((flags & 0x2000) != 0) {
        return -1;
    }

    cursor = *(s32*)((s32)select + 0xC);
    value = *(u16*)((s32)*(void**)((s32)select + 0x30) + cursor * 4 + 2);

    switch (*(s32*)((s32)select + 4)) {
        case 0xB:
            item = itemDataTable + value * 0x28;
            lwData[1] = value;
            lwData[2] = (s32)msgSearch(*(char**)(item + 8));
            lwData[3] = *(u16*)(item + 0x12);
            lwData[4] = *(u8*)(item + 0x18);
            break;
    }
    return 1;
}

s32 winMgrSelect(void* select) {
    u16 flags = *(u16*)select;

    if (flags & 0x1000) {
        if (flags & 0x2000) {
            return -1;
        }
        if (*(s32*)((s32)select + 0x38) != 0 && *(s32*)((s32)select + 0xC) == 0 && (flags & 0x100)) {
            return -1;
        }
        {
            s32 offset = *(s32*)((s32)select + 0xC) << 2;
            void* list = *(void**)((s32)select + 0x30);

            list = (void*)((s32)list + offset);
            return *(u16*)((s32)list + 2);
        }
    }
    return 0;
}

int unk_8023d524() {
    return 0;
}

s32 unk_8023d59c(void* a, void* b) {
    u16 ai = *(u16*)((s32)a + 2);
    u16 bi = *(u16*)((s32)b + 2);
    s16* values = (s16*)(itemDataTable + 0x12);
    s32 av = *(s16*)((s32)values + ai * 0x28);
    s32 bv = *(s16*)((s32)values + bi * 0x28);

    if (av > bv) {
        return 1;
    }
    return av < bv ? -1 : 0;
}

s32 unk_8023d5e4(void* a, void* b) {
    u16 ai = *(u16*)((s32)a + 2);
    u16 bi = *(u16*)((s32)b + 2);
    u8* values = itemDataTable + 0x18;
    u16 av = *(u16*)(values + ai * 0x28);
    u16 bv = *(u16*)(values + bi * 0x28);
    if (av > bv) {
        return 1;
    }
    return av < bv ? -1 : 0;
}

s32* winMgrSelectEntry(u32 selectType, s32 newItem, s32 isCancellable) {
    extern void* wp;
    extern void* __memAlloc(s32,u32);
    extern void memset(void*,s32,u32);
    extern s32 pouchKeyItem(s32);
    extern s32 pouchHaveItem(s32);
    extern s32 pouchHaveBadge(s32);
    u8* select=__memAlloc(0,0x3C);
    u8* rows;
    s32 i,count=0;
    memset(select,0,0x3C);
    *(u32*)(select+4)=selectType;
    *(s32*)(select+0x0C)=0;
    *(s32*)(select+0x10)=0;
    *(f32*)(select+0x14)=0.0f;
    *(f32*)(select+0x18)=0.0f;
    *(f32*)(select+0x1C)=0.0f;
    *(s32*)(select+0x38)=newItem;
    if(isCancellable) *(u16*)select|=0x100;
    if(selectType==0) rows=__memAlloc(0,0x1E8);
    else if(selectType==1||selectType==3||selectType==4) rows=__memAlloc(0,0x54);
    else rows=__memAlloc(0,0x324);
    memset(rows,0,selectType==0?0x1E8:(selectType==1||selectType==3||selectType==4?0x54:0x324));
    *(u8**)(select+0x30)=rows;
    if(newItem!=0) { *(u16*)(rows+2)=(u16)newItem; count=1; }
    if(selectType==0) {
        for(i=0;i<0x79;i++){s32 item=pouchKeyItem(i); if(item!=0){*(u16*)(rows+count*4+2)=(u16)item; count++;}}
    } else if(selectType==1||selectType==3||selectType==4) {
        for(i=0;i<0x14;i++){s32 item=pouchHaveItem(i); if(item!=0){*(u16*)(rows+count*4+2)=(u16)item; count++;}}
    } else {
        for(i=0;i<0x78;i++){s32 item=pouchHaveBadge(i); if(item!=0){*(u16*)(rows+count*4+2)=(u16)item; count++;}}
    }
    *(s32*)(select+0x34)=count;
    return (s32*)select;
}

void winMgrHelpDraw(void* win) {
    extern void GXSetScissor(s32, s32, s32, s32);
    extern char* msgSearch(char*);
    extern s32 strcmp(char*, char*);
    extern void* lotteryGetPtr(void);
    extern s32 sprintf(char*, char*, ...);
    extern void FontDrawStart(void);
    extern void FontDrawMessage(s32, s32, char*);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void iconDispGx2(f32[3][4], s32, s32);
    extern char vec3_803003a0[];
    extern void* gpGlobals;

    f32 trans[3][4];
    f32 scale[3][4];
    char buffer[0x400];
    char* help = *(char**)((s32)win + 0x34);
    s32 cursor = *(s32*)((s32)win + 0x38);
    s32 lineCount = *(s32*)((s32)win + 0x3C);
    s32 x;
    s32 y;

    if ((*(u32*)win & 4) != 0) {
        return;
    }

    GXSetScissor(0, 0x180, 0x260, 0x38);
    if (strcmp(help, msgSearch(&vec3_803003a0[0x554])) == 0) {
        void* lottery = lotteryGetPtr();
        sprintf(buffer, help, *(s16*)((s32)lottery + 0x20));
        help = buffer;
    }
    FontDrawStart();
    FontDrawMessage(-210, cursor * 28 - 144, help);
    GXSetScissor(0, 0, 0x260, 0x1E0);

    if (lineCount > 2) {
        x = *(s32*)((s32)win + 0x18) + *(s32*)((s32)win + 0x20) - 12;
        if ((*(u32*)((s32)gpGlobals + 0x1C) & 0x1F) < 0x14) {
            if (cursor != 0) {
                y = *(s32*)((s32)win + 0x1C) - *(s32*)((s32)win + 0x24) + 44;
                PSMTXTrans(trans, (f32)x, (f32)y, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(trans, scale, trans);
                iconDispGx2(trans, 0x10, 0x1BD);
            }
            if (cursor + 2 != lineCount) {
                y = *(s32*)((s32)win + 0x1C) - *(s32*)((s32)win + 0x24) - 4;
                PSMTXTrans(trans, (f32)x, (f32)y, 0.0f);
                PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
                PSMTXConcat(trans, scale, trans);
                iconDispGx2(trans, 0x10, 0x1BE);
            }
        }

        y = *(s32*)((s32)win + 0x1C) - *(s32*)((s32)win + 0x24) + 10;
        PSMTXTrans(trans, (f32)x, (f32)y, 0.0f);
        PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
        PSMTXConcat(trans, scale, trans);
        iconDispGx2(trans, 0x10, 0x78);
    }
}

void winMgrHelpMain(void* win) {
    extern void FontGetMessageWidthLine(char* msg, u16* lines);
    extern u32 keyGetDirTrg(s32 controller);

    u16 lines;
    s32 b;
    s32 a;

    lines = 0;
    if ((*(u32*)win & 4) == 0) {
        if (*(char**)((s32)win + 0x34) != 0) {
            FontGetMessageWidthLine(*(char**)((s32)win + 0x34), &lines);
            *(s32*)((s32)win + 0x3C) = lines + 1;
        }
        if (*(s32*)((s32)win + 0x3C) > 2) {
            if (keyGetDirTrg(0) & 0x100) {
                *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) - 2;
                if (*(s32*)((s32)win + 0x38) < 0) {
                    *(s32*)((s32)win + 0x38) = 0;
                }
            } else if (keyGetDirTrg(0) & 0x200) {
                a = *(s32*)((s32)win + 0x3C) + 1;
                b = *(s32*)((s32)win + 0x38) + 1;
                if (((s32)(a + ((u32)a >> 31)) >> 1) - 1 > ((s32)(b + ((u32)b >> 31)) >> 1)) {
                    *(s32*)((s32)win + 0x38) = *(s32*)((s32)win + 0x38) + 2;
                }
            }
        }
    }
}

void winMgrHelpInit(void* win) {
    *(s32*)((s32)win + 0x38) = 0;
    *(s32*)((s32)win + 0x3C) = 0;
    *(s32*)((s32)win + 0x34) = 0;
}

void* winMgrGetPtr(s32 id) {
    return (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
}

void winMgrSetPriority(s32 id, s32 priority) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x30) = priority;
    }
}

void winMgrSetSize(s32 id, s32 x, s32 y, s32 w, s32 h) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x18) = x;
        *(s32*)((s32)win + 0x1C) = y;
        *(s32*)((s32)win + 0x20) = w;
        *(s32*)((s32)win + 0x24) = h;
    }
}

s32 winMgrAction(s32 id) {
    return (*(u32*)((s32)*(void**)((s32)wp + 4) + id * 0x44) >> 2) & 1;
}

void winMgrDelete(s32 id) {
    u32* flags = (u32*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    *flags &= ~1;
}

void winMgrCloseAutoDelete(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 2;
        *(s32*)((s32)win + 8) = 0;
        *(u32*)win |= 8;
    }
}

void winMgrClose(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 2;
        *(s32*)((s32)win + 8) = 0;
    }
}

void winMgrOpen(s32 id) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 4) = 1;
        *(s32*)((s32)win + 8) = 0;
        *(s32*)((s32)win + 0x38) = 0;
        *(s32*)((s32)win + 0x3C) = 0;
        *(s32*)((s32)win + 0x34) = 0;
    }
}

void winMgrSetParam(s32 id, s32 param) {
    void* win = (void*)((s32)*(void**)((s32)wp + 4) + id * 0x44);
    if ((*(u32*)win & 1) != 0) {
        *(s32*)((s32)win + 0x2C) = param;
    }
}

s32 winMgrEntry(void* tpl) {
    void* win;
    s32 id;
    s32 zero;
    void* savedTpl;

    id = 0;
    win = *(void**)((s32)wp + 4);
    for (id = 0; id < *(s32*)wp; id++, win = (void*)((s32)win + 0x44)) {
        if ((*(u32*)win & 1) == 0) {
            break;
        }
    }

    zero = 0;
    *(s32*)((s32)win + 0) = zero;
    *(u32*)win |= 1;
    *(s32*)((s32)win + 4) = zero;
    *(s32*)((s32)win + 8) = zero;
    *(void**)((s32)win + 0x28) = tpl;
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x18) = *(s32*)((s32)savedTpl + 0xC);
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x1C) = *(s32*)((s32)savedTpl + 0x10);
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x20) = *(s32*)((s32)savedTpl + 0x14);
    savedTpl = *(void**)((s32)win + 0x28);
    *(s32*)((s32)win + 0x24) = *(s32*)((s32)savedTpl + 0x18);
    *(s32*)((s32)win + 0x30) = zero;
    *(s32*)((s32)win + 0x2C) = zero;
    *(s32*)((s32)win + 0x38) = zero;
    *(s32*)((s32)win + 0x3C) = zero;
    return id;
}

void winMgrSeq(void* win) {
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    extern void* gpGlobals;

    void* desc = *(void**)((s32)win + 0x28);
    s32 fadeMode = *(s32*)desc;
    s32 state = *(s32*)((s32)win + 4);
    s32 frame = *(s32*)((s32)win + 8);
    s32 steps = (*(s32*)((s32)gpGlobals + 4) * 8) / 60;

    *(u32*)win |= 4;
    if (fadeMode == 0) {
        if (state == 1) {
            *(u32*)win |= 2;
            *(f32*)((s32)win + 0x14) =
                (f32)intplGetValue(30.0, 0.0, 12, frame, steps);
            *(f32*)((s32)win + 0x10) =
                (f32)intplGetValue(0.0, 1.0, 12, frame, steps);
            *(s32*)((s32)win + 0xC) = 0xFF;
            *(s32*)((s32)win + 8) = frame + 1;
            if (steps < frame + 1) {
                *(s32*)((s32)win + 4) = 0;
            }
        } else if (state == 0) {
            *(u32*)win &= ~4;
        } else if (state < 3) {
            *(f32*)((s32)win + 0x14) =
                (f32)intplGetValue(0.0, 30.0, 12, frame, steps);
            *(f32*)((s32)win + 0x10) =
                (f32)intplGetValue(1.0, 0.0, 12, frame, steps);
            *(s32*)((s32)win + 0xC) = 0xFF;
            *(s32*)((s32)win + 8) = frame + 1;
            if (steps < frame + 1) {
                *(s32*)((s32)win + 4) = 0;
                *(u32*)win &= ~2;
                if (*(u32*)win & 8) {
                    *(u32*)win &= ~1;
                }
            }
        }
    } else if (fadeMode == 1) {
        if (state == 1) {
            *(u32*)win |= 2;
            *(f32*)((s32)win + 0x14) = 1.0f;
            *(f32*)((s32)win + 0x10) =
                (f32)intplGetValue(0.0, 1.0, 12, frame, steps);
            *(s32*)((s32)win + 0xC) =
                (s32)intplGetValue(0.0, 255.0, 12, frame, steps);
            *(s32*)((s32)win + 8) = frame + 1;
            if (steps < frame + 1) {
                *(s32*)((s32)win + 4) = 0;
            }
        } else if (state == 0) {
            *(u32*)win &= ~4;
        } else if (state < 3) {
            *(f32*)((s32)win + 0x14) = 0.0f;
            *(f32*)((s32)win + 0x10) = 1.0f;
            *(s32*)((s32)win + 0xC) =
                (s32)intplGetValue(255.0, 0.0, 12, frame, steps);
            *(s32*)((s32)win + 8) = frame + 1;
            if (steps < frame + 1) {
                *(s32*)((s32)win + 4) = 0;
                *(u32*)win &= ~2;
                if (*(u32*)win & 8) {
                    *(u32*)win &= ~1;
                }
            }
        }
    } else if (fadeMode < 3) {
        if (state == 1) {
            *(u32*)win |= 2;
            *(f32*)((s32)win + 0x14) = 1.0f;
            *(f32*)((s32)win + 0x10) = 1.0f;
            *(s32*)((s32)win + 0xC) = 0xFF;
            *(s32*)((s32)win + 4) = 0;
        } else if (state == 0) {
            *(u32*)win &= ~4;
        } else if (state < 3) {
            *(u32*)win &= ~2;
            *(f32*)((s32)win + 0x14) = 1.0f;
            *(f32*)((s32)win + 0x10) = 1.0f;
            *(s32*)((s32)win + 0xC) = 0xFF;
            *(s32*)((s32)win + 4) = 0;
            if (*(u32*)win & 8) {
                *(u32*)win &= ~1;
            }
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void winMgrMain(void) {
    extern void winMgrSeq(void* win);
    extern void winMgrDisp(s32 cameraId, s32 unused, void* win);
    extern void dispEntry(s32 cameraId, s32 layer, void* dispFunc, void* param, f32 z);

    void* win;
    void* tpl;
    void (*mainFunc)(void*);
    s32 i;

    i = 0;
    win = *(void**)((s32)wp + 4);
    while (i < *(s32*)wp) {
        if ((*(u32*)win & 1) != 0) {
            winMgrSeq(win);
            tpl = *(void**)((s32)win + 0x28);
            mainFunc = *(void (**)(void*))((s32)tpl + 0x20);
            if (mainFunc != 0) {
                mainFunc(win);
            }
            if ((*(u32*)win & 2) != 0) {
                tpl = *(void**)((s32)win + 0x28);
                dispEntry(*(s32*)((s32)tpl + 8), 0, winMgrDisp, win, (f32)(*(s32*)((s32)win + 0x30) + 0x12C));
            }
        }
        i++;
        win = (void*)((s32)win + 0x44);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void winMgrDisp(s32 cameraId, s32 unused, void* win) {
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void windowDispGX2_Waku_col(f32, f32, f32, f32, f32, f32[3][4], s32, u32*);
    extern f32 float_deg2rad_80428060;
    extern u32 dat_80427fa0;
    extern u32 dat_80427fa4;
    extern u32 dat_80427fa8;
    extern u32 dat_80427fac;
    extern u32 dat_80427fb0;

    f32 rot[3][4];
    f32 scaleMtx[3][4];
    void* desc = *(void**)((s32)win + 0x28);
    f32 scale = *(f32*)((s32)win + 0x10);
    u32 alpha = *(u32*)((s32)win + 0xC) & 0xFF;
    u32 color;
    s32 heading;
    s32 width;
    s32 leftWidth;
    s32 rightWidth;
    (void)cameraId;
    (void)unused;

    PSMTXRotRad(rot, 0x7A, float_deg2rad_80428060 * *(f32*)((s32)win + 0x14));
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(rot, scaleMtx, scaleMtx);

    color = (*(u32*)((s32)desc + 0x1C) & 0xFFFFFF00) | alpha;
    windowDispGX2_Waku_col(
        (f32)*(s32*)((s32)win + 0x18),
        (f32)*(s32*)((s32)win + 0x1C),
        (f32)*(s32*)((s32)win + 0x20),
        (f32)*(s32*)((s32)win + 0x24),
        20.0f, scaleMtx, 0, &color);

    heading = *(s32*)((s32)desc + 4);
    width = *(s32*)((s32)win + 0x20);
    if (heading == 1) {
        color = (dat_80427fa0 & 0xFFFFFF00) | alpha;
        windowDispGX2_Waku_col(
            (f32)(*(s32*)((s32)win + 0x18) + (width - 150) / 2),
            (f32)(*(s32*)((s32)win + 0x1C) + 14),
            150.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
    } else if (heading == 3) {
        color = (dat_80427fa4 & 0xFFFFFF00) | alpha;
        windowDispGX2_Waku_col(
            (f32)(*(s32*)((s32)win + 0x18) + 10),
            (f32)(*(s32*)((s32)win + 0x1C) + 14),
            100.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
        color = (dat_80427fa8 & 0xFFFFFF00) | alpha;
        windowDispGX2_Waku_col(
            (f32)(*(s32*)((s32)win + 0x18) + width / 2 - 10),
            (f32)(*(s32*)((s32)win + 0x1C) + 14),
            100.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
    } else if (heading >= 4 && heading < 6) {
        leftWidth = width * 3 / 5;
        rightWidth = width * 2 / 5;
        color = (dat_80427fac & 0xFFFFFF00) | alpha;
        windowDispGX2_Waku_col(
            (f32)(*(s32*)((s32)win + 0x18) + ((leftWidth - 120) * 2) / 3),
            (f32)(*(s32*)((s32)win + 0x1C) + 14),
            120.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
        color = (dat_80427fb0 & 0xFFFFFF00) | alpha;
        windowDispGX2_Waku_col(
            (f32)(*(s32*)((s32)win + 0x18) + leftWidth + (rightWidth - 85) / 3),
            (f32)(*(s32*)((s32)win + 0x1C) + 14),
            85.0f, 28.0f, 8.0f, scaleMtx, 0, &color);
    }

    if (*(void**)((s32)desc + 0x24) != 0) {
        ((void (*)(void*))*(void**)((s32)desc + 0x24))(win);
    }
}

void winMgrReInit(void) {
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x44);
}

void winMgrInit(void) {
    *(s32*)wp = 0x20;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0x44);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x44);
}

s32 unk_8023f79c(void* event, s32 isFirstCall) {
    extern void* pouchGetPtr(void);
    extern s32 partyChkJoin(s32 partyId);
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 target, s32 value);
    extern u16 party_id_table[];

    s32* args;
    void* pouch;
    u16* party;
    s32 joined;
    s32 missing;
    s32 hasValue;
    s32 threshold;
    s32 value;

    args = *(s32**)((s32)event + 0x18);
    pouch = pouchGetPtr();
    joined = 0;
    missing = 0;
    hasValue = 0;
    party = party_id_table;
    while (*party != 0) {
        if (partyChkJoin(*party) != 0) {
            joined++;
            value = *(s16*)((s32)pouch + *party * 0xE + 0xC);
            if (value != 0) {
                hasValue = 1;
                if (evtGetValue(event, 0xF8406022) == 0) {
                    threshold = value >= 1 ? 1 : 0;
                } else {
                    threshold = value >= 2 ? 2 : 0;
                }
                if (threshold == 0) {
                    missing = 1;
                }
            }
        }
        party++;
    }
    if (joined > 1 && missing != 0 && hasValue != 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}

s32 unk_8023f8d0(void* event, s32 isFirstCall) {
    extern void* pouchGetPtr(void);
    extern s32 partyChkJoin(s32 partyId);
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 target, s32 value);
    extern u16 party_id_table[];

    s32* args;
    void* pouch;
    u16* party;
    s32 value;
    s32 threshold;

    args = *(s32**)((s32)event + 0x18);
    pouch = pouchGetPtr();
    party = party_id_table;
    while (*party != 0) {
        if (partyChkJoin(*party) != 0) {
            if (evtGetValue(event, 0xF8406022) == 0) {
                value = *(s16*)((s32)pouch + *party * 0xE + 0xC);
                threshold = value >= 1 ? 1 : 0;
            } else {
                value = *(s16*)((s32)pouch + *party * 0xE + 0xC);
                threshold = value >= 2 ? 2 : 0;
            }
            if (threshold == 0) {
                evtSetValue(event, args[0], 1);
                return 2;
            }
        }
        party++;
    }
    evtSetValue(event, args[0], 0);
    return 2;
}

