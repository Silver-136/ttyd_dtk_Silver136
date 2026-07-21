#include "event.h"

void stg0_00_init(void) {
    extern void* pouchGetPtr(void);
    extern void pouchInit(void* pouch);
    extern void pouchGetItem(s32 item);
    extern void pouchSetCoin(s32 coins);
    extern void pouchReviseMarioParam(void);
    extern void badgeShop_init(void);
    extern void yuugijou_init(void);
    extern void johoya_init(void);
    extern void marioSetCharMode(s32 mode);
    extern void statusWinForceUpdate(void);
    extern void* gp;
    void* gpPtr;
    pouchInit(pouchGetPtr());
    pouchGetItem(6);
    pouchGetItem(0x32);
    pouchGetItem(9);
    pouchSetCoin(0x64);
    pouchReviseMarioParam();
    badgeShop_init();
    yuugijou_init();
    johoya_init();
    gpPtr = gp;
    *(s32*)((s32)gpPtr + 0x24) = 0;
    *(s32*)((s32)gpPtr + 0x20) = 0;
    marioSetCharMode(0);
    statusWinForceUpdate();
}

void stg0_05_init(void) {
    extern void swSet(s32 flag);
    swSet(0x49B);
}

void stg0_08_init(void) {
    extern void partyJoin(s32 party);
    partyJoin(1);
}

void stg0_11_init(void) {
    extern void swSet(s32 flag);
    swSet(0x4B7);
}

void stg0_12_02_init(void) {
    extern void swSet(s32 flag);
    swSet(0x537);
}

void stg0_13_init00(void) { extern void pouchGetItem(s32 item); extern void swSet(s32 flag); pouchGetItem(4); swSet(0x539); }

void stg0_16_init(void) { extern void pouchGetStarStone(s32 id); pouchGetStarStone(0); }

void stg1_01a_init(void) { extern void swSet(s32 flag); swSet(0x6F7); }

void stg1_03_init(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    swSet(7);
    pouchReceiveMail(0x10);
    pouchReceiveMail(0x1A);
}

void stg1_06_01_init(void) {
    extern void pouchGetItem(s32 item);
    extern void swSet(s32 flag);
    pouchGetItem(0x3D);
    swSet(0x6EF);
    pouchGetItem(0x3E);
    swSet(0x6EE);
}

void stg1_08_init(void) {
    extern void partyJoin(s32 party);
    extern void swSet(s32 flag);
    partyJoin(2);
    swSet(0x6F5);
    swSet(0x6F6);
}

void stg1_09_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x3D); pouchRemoveItem(0x3E); }

void stg1_09_02_init(void) {
    ;
}

void stg1_10_init(void) {
    ;
}

void stg1_12_02_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0xD); }

void stg1_13_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0xD); }

void stg1_13_01_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x22); }

void stg1_13_02_init(void) { extern void pouchRemoveItem(s32 item); extern void pouchGetItem(s32 item); pouchRemoveItem(0x22); pouchGetItem(2); }

void stg1_13_03_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0xC); }

void stg1_14_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0xC); }

void stg1_15_02_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0xF); }

void stg1_15_03_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0xF); }

void stg1_15_04_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0xE); }

void stg1_16_init00(void) {
    extern void pouchRemoveItem(s32 item);
    extern void pouchGetItem(s32 item);
    extern void pouchEquipBadgeID(s32 badge);
    extern void pouchReviseMarioParam(void);
    pouchRemoveItem(0xE);
    pouchGetItem(0x13B);
    pouchGetItem(0xF2);
    pouchGetItem(0xF1);
    pouchGetItem(0xF7);
    pouchEquipBadgeID(0x13B);
    pouchReviseMarioParam();
}

void stg1_16_init01(void) { extern void swSet(s32 flag); swSet(0x5DA); }

void stg1_17_init(void) { extern void pouchGetItem(s32 item); extern void pouchGetStarStone(s32 id); pouchGetItem(0x72); pouchGetStarStone(1); }

void stg1_19_init(void) {
    extern void swSet(s32 flag);
    swSet(0x1067);
    swSet(0x1068);
    swSet(0x1069);
}

void stg1_20_init(void) { extern void marioSetCharMode(s32 mode); marioSetCharMode(0); }

void stg1_30_init(void) { extern void pouchReceiveMail(s32 id); extern void pouchOpenMail(s32 id); pouchReceiveMail(0); pouchOpenMail(0); }

void stg1_30_init01(void) { extern void swSet(s32 flag); swSet(0x4A4); swSet(0x4A3); }

void stg1_35_init01(void) { extern void swSet(s32 flag); swSet(0x536); }

void stg2_00_init(void) {
    extern void* pouchGetPtr(void);
    extern void pouchSetMaxHP(s32 hp);
    extern void pouchSetMaxFP(s32 fp);
    void* pouch = pouchGetPtr();
    pouchSetMaxHP(0xF);
    pouchSetMaxFP(0xF);
    *(s16*)((s32)pouch + 0x92) = 6;
    *(s16*)((s32)pouch + 0x94) = 6;
    *(s16*)((s32)pouch + 0x8A) = 3;
}

void stg2_03_03_init(void) {
    ;
}

void stg2_09_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x3F); }

void stg2_10_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void partyJoin(s32 party);
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    pouchRemoveItem(0x3F);
    partyJoin(5);
    swSet(0x11);
    pouchReceiveMail(0x1B);
}

void stg2_11_init(void) { extern void swSet(s32 flag); swSet(0xB0A); }

void stg2_12_01_init(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    swSet(0xB3C);
    swByteSet(0x28B, 0);
}

void stg2_14_init(void) { extern void pouchGetItem(s32 item); extern void swSet(s32 flag); pouchGetItem(0x10); swSet(0xB17); }

void stg2_15_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void swSet(s32 flag);
    pouchRemoveItem(0x10);
    swSet(0xB10);
    swSet(0xB11);
}

void stg2_16_init00(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    extern void pouchGetItem(s32 item);
    swSet(0xB3F);
    swByteSet(0x28B, 9);
    pouchGetItem(0x40);
}

void stg2_16_init01(void) { extern void swSet(s32 flag); swSet(0xB0D); }

void stg2_18_init01(void) { extern void swSet(s32 flag); swSet(0xB3E); }

void stg2_18_init02(void) { extern void swSet(s32 flag); swSet(0xB1F); swSet(0xB21); }

void stg2_19_init(void) { extern void swSet(s32 flag); extern void pouchGetItem(s32 item); swSet(0xB24); pouchGetItem(0x11); }

void stg2_20_init(void) { extern void swSet(s32 flag); swSet(0xB14); }

void stg2_21_init00(void) {
    extern void pouchRemoveItem(s32 item);
    extern void swSet(s32 flag);
    extern void swClear(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    pouchRemoveItem(0x11);
    swSet(0xB12);
    swSet(0xB13);
    swSet(0xB3D);
    swClear(0xB14);
    swByteSet(0x28B, 0x62);
}

void stg2_21_init02(void) { extern void swSet(s32 flag); swSet(0xB27); swSet(0xB29); }

void stg2_21_01_init(void) {
    extern void swByteSet(s32 byte, s32 value);

    swByteSet(0x28C, 0xF6);
    swByteSet(0x28D, 0xFF);
    swByteSet(0x28E, 0xFF);
    swByteSet(0x28F, 0xFF);
    swByteSet(0x290, 0xFF);
    swByteSet(0x291, 0xFF);
    swByteSet(0x292, 0xFF);
    swByteSet(0x293, 0xFF);
    swByteSet(0x294, 0xFF);
    swByteSet(0x295, 0xFF);
    swByteSet(0x296, 0xFF);
    swByteSet(0x297, 0xFF);
    swByteSet(0x298, 0x7F);
}

void stg2_21_02_init(void) { extern void swSet(s32 flag); swSet(0xB38); }

void stg2_21_03_init(void) { extern void swSet(s32 flag); swSet(0xB22); }

void stg2_21_04_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(7); }

void stg2_21_06_init00(void) {
    extern void swByteSet(s32 byte, s32 value);

    swByteSet(0x28C, 0);
    swByteSet(0x28D, 0);
    swByteSet(0x28E, 0);
    swByteSet(0x28F, 0);
    swByteSet(0x290, 0);
    swByteSet(0x291, 0);
    swByteSet(0x292, 0);
    swByteSet(0x293, 0);
    swByteSet(0x294, 0);
    swByteSet(0x295, 0);
    swByteSet(0x296, 0);
    swByteSet(0x297, 0);
    swByteSet(0x298, 0);
}

void stg2_21_06_init01(void) { extern void swSet(s32 flag); swSet(0xB33); }

void stg2_23_init(void) { extern void swSet(s32 flag); swSet(0xB30); }

void stg2_23_01_init(void) { extern void swClear(s32 flag); extern void swSet(s32 flag); swClear(0xB30); swSet(0xB40); }

void stg2_25_init(void) {
    extern void swClear(s32 flag);
    extern void pouchGetItem(s32 item);
    extern void pouchGetStarStone(s32 id);
    swClear(0xB40);
    pouchGetItem(0x73);
    pouchGetStarStone(2);
}

void stg2_25_02_init(void) { extern void swSet(s32 flag); swSet(0x106A); }

void stg2_26_init(void) { extern void marioSetCharMode(s32 mode); marioSetCharMode(0); }

void stg2_40_init(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    swSet(0xB44);
    pouchReceiveMail(1);
    pouchOpenMail(1);
}

void stg2_44_init(void) { extern void swSet(s32 flag); swSet(0x4CE); }

void stg2_47_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x34); }

void stg2_end_init(void) { extern void swSet(s32 flag); extern void pouchReceiveMail(s32 id); swSet(8); pouchReceiveMail(0x11); }

void stg3_06_init(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    swSet(0x954);
    swByteSet(0x1F5, 0x13);
    swByteSet(0x1F6, 0x2B);
    swByteSet(0x1F7, 1);
    swByteSet(0x1F8, 1);
}


void stg3_07_init__9(void) {
    extern void swSet(s32 flag);
    extern void swClear(s32 flag);
    extern void swByteSet(s32 byte, s32 value);

    swSet(0x9B4);
    swByteSet(0x1FB, 0x13);
    swClear(0x954);
    swSet(0x958);
    swSet(0x95B);
    swSet(0x95C);
    swSet(0x9CD);
    swSet(0x98B);
}

void stg3_07_init__8(void) {
    extern void swSet(s32 flag);
    extern void swClear(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 2; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swClear(0x954);
    swSet(0x966);
}

void stg3_07_init__7(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 3; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x967);
}

void stg3_07_init__6(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 4; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x968);
}

void stg3_10_init__5(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 5; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x969);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void stg3_12_init__4(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 byte, s32 value);
    extern void pouchReceiveMail(s32 mail);
    extern void* gp;
    s32 i;
    u8* time;
    void* gpPtr;
    u32 timeWords[2];

    for (i = 0; i < 6; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x96A);
    swSet(0x957);
    gpPtr = gp;
    timeWords[0] = *(u32*)((s32)gpPtr + 0x20);
    timeWords[1] = *(u32*)((s32)gpPtr + 0x24);
    time = (u8*)timeWords;
    for (i = 0; i < 8; i++) {
        swByteSet(i + 0x1FC, time[i]);
    }
    swSet(0x12);
    pouchReceiveMail(0x1C);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void stg3_12_init__3(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 7; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x96B);
}

void stg3_12_init__2(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 8; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x96C);
}

void stg3_12_init__1(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 9; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x96D);
}

void stg3_12_2_init(void) {
    extern void yoshiSetColor(void);
    extern void* msgSearch(const char* msg);
    extern void pouchSetYoshiName(void* name);
    extern void partyJoin(s32 party);
    extern void swSet(s32 flag);
    extern const char str_name_party3_802ed258[];
    yoshiSetColor();
    pouchSetYoshiName(msgSearch(str_name_party3_802ed258));
    partyJoin(4);
    swSet(0x95D);
}

void stg3_20_init_10(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    s32 i;
    for (i = 0; i < 0xA; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x96E);
    swSet(0x95E);
    pouchReceiveMail(6);
    pouchOpenMail(6);
}

void stg3_20_init_9(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 0xB; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x96F);
}

void stg3_20_init_8(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 0xC; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x970);
}

void stg3_20_01_init(void) { extern void pouchReceiveMail(s32 id); extern void pouchOpenMail(s32 id); pouchReceiveMail(7); pouchOpenMail(7); }

void stg3_20_02_init(void) {
    extern void pouchGetItem(s32 item);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    pouchGetItem(0xA);
    pouchReceiveMail(8);
    pouchOpenMail(8);
}

void stg3_20_init_7(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    s32 i;
    for (i = 0; i < 0xD; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x971);
    swSet(0x994);
    swSet(0x9D5);
    pouchReceiveMail(9);
    pouchOpenMail(9);
}

void stg3_20_init_6(void) {
    extern void swSet(s32 flag);
    extern void swClear(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 0xE; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x972);
    swClear(0x994);
}

void stg3_20_init_5(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 0xF; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x973);
}

void stg3_20_init_4(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 0x10; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x974);
    swSet(0x995);
}

void stg3_20_06_init(void) {
    extern void swClear(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    swClear(0x995);
    pouchReceiveMail(0xA);
    pouchOpenMail(0xA);
}

void stg3_20_07_00_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x12); }

void stg3_20_08_init(void) { extern void pouchReceiveMail(s32 id); extern void pouchOpenMail(s32 id); pouchReceiveMail(0xB); pouchOpenMail(0xB); }

void stg3_20_09_init(void) { extern void pouchReceiveMail(s32 id); extern void pouchOpenMail(s32 id); pouchReceiveMail(0xC); pouchOpenMail(0xC); }

void stg3_20_12_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void swSet(s32 flag);
    pouchRemoveItem(0x12);
    swSet(0x979);
    swSet(0x97A);
    swSet(0x97B);
    swSet(0x97C);
    swSet(0x97D);
    swSet(0x97E);
    swSet(0x97F);
    swSet(0x980);
}

void stg3_20_init_3(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    s32 i;
    for (i = 0; i < 0x11; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x975);
    pouchReceiveMail(0xD);
    pouchOpenMail(0xD);
}

void stg3_20_init_2(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    s32 i;
    for (i = 0; i < 0x12; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x976);
    swSet(0x990);
    swSet(0x993);
}

void stg3_20_init_1(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    s32 i;
    for (i = 0; i < 0x13; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x977);
    swSet(0x95A);
    swSet(0x996);
    pouchReceiveMail(0xE);
    pouchOpenMail(0xE);
}

void stg3_20_13_init(void) { extern void swClear(s32 flag); swClear(0x996); }

void stg3_20_14_init(void) {
    extern void pouchGetItem(s32 item);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    pouchGetItem(0x13);
    pouchReceiveMail(0xF);
    pouchOpenMail(0xF);
}

void stg3_20_15_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x13); }

void stg3_21_init(void) {
    extern void swSet(s32 flag);
    extern void swByteSet(s32 flag, s32 value);
    swSet(0x954);
    swByteSet(0x1F5, 0);
    swSet(0x94F);
    swByteSet(0x1F6, 6);
    swByteSet(0x1F7, 1);
    swByteSet(0x1F8, 1);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void stg3_30_init(void) {
    extern void swSet(s32 flag);
    extern void swClear(s32 flag);
    extern void swByteSet(s32 byte, s32 value);
    s32 i;

    for (i = 0; i < 20; i++) {
        swSet(0x9B4 - i);
    }
    swByteSet(0x1FB, 0x14 - i);
    swSet(0x978);
    swClear(0x94F);
    swClear(0x954);
    swByteSet(0x1F5, 0);
    swSet(0x955);
    swSet(0x95F);
    swSet(0x960);
    swSet(0x9D1);
    swSet(0x9D2);
    swSet(0x9D3);
    swSet(0x9D4);
}

void stg3_31_00_init(void) {
    extern void swClear(s32 flag);
    extern void pouchGetItem(s32 item);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    swClear(0x955);
    pouchGetItem(0x41);
    pouchReceiveMail(0x2B);
    pouchOpenMail(0x2B);
}

void stg3_31_init(void) { extern void swClear(s32 flag); swClear(0x955); }

void stg3_32_init(void) { extern void swSet(s32 flag); swSet(0x961); }

void stg3_33_init(void) { extern void pouchGetItem(s32 item); extern void pouchGetStarStone(s32 id); pouchGetItem(0x74); pouchGetStarStone(3); }

void stg3_33_03_init(void) { extern void swSet(s32 flag); swSet(0x106B); swSet(0x106C); }

void stg3_33_04_init(void) { extern void swClear(s32 flag); swClear(0x106C); }

void stg3_34_init(void) {
    extern void swSet(s32 flag);
    extern void swClear(s32 flag);
    extern void marioSetCharMode(s32 mode);
    extern void pouchRemoveItem(s32 item);
    s32 i;

    swSet(0x962);
    swSet(0x963);
    marioSetCharMode(0);
    pouchRemoveItem(0x41);
    for (i = 0; i < 20; i++) {
        swClear(i + 0x9A1);
    }
    swClear(0x958);
    swClear(0x95F);
    swClear(0x960);
    swClear(0x94F);
    swClear(0x9E1);
    swClear(0x9E4);
    swClear(0x963);
}

void stg3_35_init(void) { extern void swSet(s32 flag); swSet(0x9C0); }

void stg3_50_init(void) { extern void pouchReceiveMail(s32 id); extern void pouchOpenMail(s32 id); pouchReceiveMail(2); pouchOpenMail(2); }

void stg3_52_init(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    swSet(0x53D);
    swSet(9);
    pouchReceiveMail(0x12);
}

void stg3_end_init(void) {
    ;
}

void stage4_00_init(void) {
    ;
}

void stage4_04_init(void) {
    extern void pouchGetItem(s32 item);
    pouchGetItem(0x15);
}

void stage4_04_01_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void swSet(s32 flag);
    extern void pouchGetItem(s32 item);
    pouchRemoveItem(0x15);
    swSet(0x82B);
    pouchGetItem(0x23);
}

void stage4_05_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void pouchGetItem(s32 item);
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    pouchRemoveItem(0x23);
    pouchGetItem(3);
    swSet(0x13);
    pouchReceiveMail(0x1D);
}

void stage4_09_05_init(void) {
    extern void pouchGetItem(s32 item);
    pouchGetItem(0x16);
}

void stage4_10_init(void) {
    extern void pouchRemoveItem(s32 item);
    pouchRemoveItem(0x16);
}

void stg4_11_init(void) {
    extern void partyLeft(s32 party);
    partyLeft(1);
    partyLeft(2);
    partyLeft(5);
    partyLeft(4);
}

void stg4_13_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x43); }

void stg4_13_02_init(void) { extern void pouchRemoveItem(s32 item); extern void partyJoin(s32 party); pouchRemoveItem(0x43); partyJoin(6); }

void stg4_13_03_init(void) { extern void swSet(s32 flag); swSet(0x81B); }

void stg4_14_init(void) {
    extern void pouchGetItem(s32 item);
    extern void swSet(s32 flag);
    pouchGetItem(0x44);
    swSet(0x81B);
    swSet(0x8B6);
    swSet(0x8B7);
}

void stg4_15_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x44); }

void stg4_15_03_init(void) {
    extern void partyJoin(s32 party);
    extern void pouchGetItem(s32 item);
    extern void pouchGetStarStone(s32 stone);
    partyJoin(1);
    partyJoin(2);
    partyJoin(5);
    partyJoin(4);
    pouchGetItem(0x75);
    pouchGetStarStone(4);
}

void stg4_16_init(void) { extern void marioSetCharMode(s32 mode); marioSetCharMode(0); }

void stg4_30_init(void) { extern void pouchReceiveMail(s32 id); extern void pouchOpenMail(s32 id); pouchReceiveMail(3); pouchOpenMail(3); }

void stg4_35_init(void) { extern void swSet(s32 flag); swSet(0x4C0); swSet(0x4C1); }

void stg4_36_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x45); }

void stg4_37_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x45); }

void stg4_end_init(void) { extern void swSet(s32 flag); extern void pouchReceiveMail(s32 id); swSet(0xA); pouchReceiveMail(0x13); }

void stg5_12_init01(void) { extern void swSet(s32 flag); swSet(0xC36); }

void stg5_15_init01(void) { extern void swSet(s32 flag); swSet(0xC3B); }

void stg5_15_init02(void) { extern void swSet(s32 flag); swSet(0xC39); }

void stg5_17_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0xA6); }

void stg5_18_init(void) { extern void pouchGetItem(s32 item); extern void pouchRemoveItem(s32 item); pouchGetItem(0x46); pouchRemoveItem(0xA6); }

void stg5_18_init_02(void) { extern void pouchRemoveItem(s32 item); extern void partyJoin(s32 party); pouchRemoveItem(0x46); partyJoin(3); }

void stg5_19_init00(void) {
    ;
}

void stg5_19_init01(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x47); }

void stg5_19_init02(void) {
    extern void pouchRemoveItem(s32 item);
    extern void swSet(s32 flag);
    pouchRemoveItem(0x47);
    swSet(0xC43);
    swSet(0xC4D);
}

void stg5_19_01_init(void) {
    ;
}

void stg5_21_init00(void) { extern void swSet(s32 flag); swSet(0xBA2); }

void stg5_21_init01(void) { extern void swSet(s32 flag); swSet(0xBA1); swSet(0xBA3); }

void stg5_21_init03(void) { extern void swSet(s32 flag); swSet(0xBA5); }

void stg5_21_01_init00(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x24); }

void stg5_22_init00(void) { extern void pouchRemoveItem(s32 item); extern void pouchGetItem(s32 item); pouchRemoveItem(0x24); pouchGetItem(5); }

void stg5_22_init01(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x48); }

void stg5_29_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x48); }

void stg5_30_init01(void) { extern void swSet(s32 flag); swSet(0xBA6); }

void stg5_30_init02(void) {
    extern void swSet(s32 flag);
    swSet(0xBA7);
    swSet(0xBA8);
    swSet(0xBB0);
}

void stg5_43_init(void) { extern void pouchGetItem(s32 item); extern void pouchGetStarStone(s32 id); pouchGetItem(0x76); pouchGetStarStone(5); }

void stg5_47_init(void) {
    extern void swSet(s32 flag);
    swSet(0x106F);
    swSet(0x1070);
    swSet(0x1071);
}

void stg5_58_init(void) { extern void marioSetCharMode(s32 mode); marioSetCharMode(0); }

void stg5_59_01_init(void) { extern void swSet(s32 flag); swSet(0x49C); }

void stg5_60_init(void) {
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    extern void swSet(s32 flag);
    extern void swClear(s32 flag);
    pouchReceiveMail(4);
    pouchOpenMail(4);
    swSet(0x14);
    pouchReceiveMail(0x1E);
    swClear(0x49C);
}

void stg5_62_init(void) { extern void swSet(s32 flag); swSet(0x4CF); }

void stg5_63_init(void) { extern void swSet(s32 flag); swSet(0xC35); }

void stg5_64_init(void) { extern void swClear(s32 flag); swClear(0xC35); }

void stg5_64_01_init(void) { extern void swSet(s32 flag); extern void pouchGetItem(s32 item); swSet(0xC47); pouchGetItem(0x49); }

void stg5_64_02_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x49); }

void stg5_64_03_init(void) { extern void swSet(s32 flag); swSet(0x49A); }

void stg5_65_init(void) { extern void swClear(s32 flag); extern void swSet(s32 flag); swClear(0x49A); swSet(0x4CF); }

void stg5_66_init(void) { extern void pouchGetItem(s32 item); extern void swSet(s32 flag); pouchGetItem(0x35); swSet(0x4CF); }

void stg6_00_init(void) { extern void swSet(s32 flag); extern void pouchReceiveMail(s32 id); swSet(0xB); pouchReceiveMail(0x14); }

void stg6_06_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x4A); }

void stg6_12_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x4D); }

void stg6_13_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x4D); }

void stg6_18_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x4E); }

void stg6_19_init(void) { extern void pouchRemoveItem(s32 item); extern void pouchGetItem(s32 item); pouchRemoveItem(0x4E); pouchGetItem(0x4F); }

void stg6_20_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x4F); }

void stg6_22_init(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    swSet(0x15);
    pouchReceiveMail(0x1F);
}

void stg6_24_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x50); }

void stg6_27_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void pouchGetItem(s32 item);
    pouchRemoveItem(0x50);
    pouchGetItem(0x4B);
    pouchGetItem(0x4C);
}

void stg6_29_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x18); }

void stg6_30_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x18); }

void stg6_32_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x19); }

void stg6_33_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x19); }

void stg6_34_init(void) {
    extern void swSet(s32 flag);
    swSet(0xE95);
    swSet(0xE96);
    swSet(0xE97);
    swSet(0xE98);
    swSet(0xE99);
    swSet(0xE9A);
    swSet(0xE9B);
    swSet(0xE9C);
    swSet(0xE9D);
    swSet(0xE9E);
    swSet(0xE9F);
}

void stg6_37_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(8); }

void stg6_38_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x1A); }

void stg6_39_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x1A); }

void stg6_44_init(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    swSet(0xD);
    pouchReceiveMail(0x16);
}

void stg6_47_00_init(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    swSet(0xC);
    pouchReceiveMail(0x15);
}

void stg6_54_init(void) {
    extern void pouchGetItem(s32 item);
    extern void pouchGetStarStone(s32 stone);
    pouchGetItem(0x77);
    pouchGetStarStone(6);
}

void stg6_56_init(void) { extern void marioSetCharMode(s32 mode); marioSetCharMode(0); }

void stg6_70_init(void) {
    extern void pouchReceiveMail(s32 mail);
    extern void pouchOpenMail(s32 mail);
    pouchReceiveMail(5);
    pouchOpenMail(5);
}

void stg6_71_01_init(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    extern void* gp;
    u64 ticks;

    swSet(0xE);
    pouchReceiveMail(0x17);
    ticks = *(u32*)0x800000F8;
    ticks >>= 2;
    ticks *= 0xB4;
    *(u64*)((s32)gp + 0x20) += ticks;
}

void stg6_72_init(void) {
    ;
}

void stg6_end_init01(void) {
    extern void pouchGetItem(s32 item);
    extern void swSet(s32 flag);
    pouchGetItem(0xB);
    swSet(0x4AB);
}

void stg6_end_init02(void) { extern void swSet(s32 flag); swSet(0x53C); }

void stg7_01_init(void) { extern void swSet(s32 flag); swSet(0xF2C); }

void stg7_03_white_0(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    extern void pouchGetItem(s32 item);
    swSet(0x16);
    pouchReceiveMail(0x20);
    swSet(0xF22);
    pouchGetItem(0x52);
}

void stg7_03_white_1(void) {
    extern void swSet(s32 flag);
    swSet(0xF25);
}

void stg7_03_white_2(void) {
    extern void swSet(s32 flag);
    swSet(0xF26);
}

void stg7_03_white_3(void) {
    extern void swSet(s32 flag);
    swSet(0xF27);
}

void stg7_03_white_4(void) {
    extern void swSet(s32 flag);
    swSet(0xF31);
}

void stg7_03_white_5(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    swSet(0xF28);
    swSet(0xF);
    pouchReceiveMail(0x18);
}

void stg7_04_init(void) { extern void swSet(s32 flag); swSet(0xF24); }

void stg7_06_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x52); }

void stg7_10_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x1B); }

void stg7_11_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x1B); }

void stg7_15_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x1C); }

void stg7_15_room_A_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x1C); }

void stg7_15_room_B_init(void) {
    extern void pouchGetItem(s32 item);
    extern void swSet(s32 flag);
    pouchGetItem(0x1D);
    swSet(0x1056);
}

void stg7_15_room_C_init(void) {
    extern void pouchGetItem(s32 item);
    extern void swSet(s32 flag);
    pouchGetItem(0x1E);
    swSet(0x1057);
}

void stg7_16_init(void) {
    extern void pouchGetItem(s32 item);
    extern void swSet(s32 flag);
    pouchGetItem(0x1F);
    swSet(0x1054);
    swSet(0x1063);
}

void stg7_17_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void swSet(s32 flag);
    pouchRemoveItem(0x1D);
    pouchRemoveItem(0x1E);
    pouchRemoveItem(0x1F);
    swSet(0x105F);
    swSet(0x1060);
    swSet(0x1061);
}

void stg7_19_init(void) {
    extern void swSet(s32 flag);
    extern void pouchGetItem(s32 item);
    swSet(0x1055);
    pouchGetItem(0x20);
}

void stg7_20_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x20); }

void stg7_25_00_init(void) {
    extern void pouchGetItem(s32 item);
    extern void pouchGetStarStone(s32 stone);
    pouchGetItem(0x78);
    pouchGetStarStone(7);
}

void stg7_50_init(void) { extern void swSet(s32 flag); swSet(0x104F); }

void stg7_51_init(void) {
    extern void swSet(s32 flag);
    extern void pouchReceiveMail(s32 mail);
    extern void swClear(s32 flag);
    swSet(0x10);
    pouchReceiveMail(0x19);
    pouchReceiveMail(0x21);
    swClear(0x104F);
    swSet(0x52F);
}

void stg7_52_init(void) {
    ;
}

void stg8_01_02_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x2E); }

void stg8_02_init(void) { extern void pouchRemoveItem(s32 item); pouchRemoveItem(0x2E); }

void stg8_05_init(void) {
    extern void swSet(s32 flag);
    extern void pouchGetItem(s32 item);
    swSet(0x111C);
    swSet(0x10E6);
    swSet(0x10E7);
    swSet(0x10E8);
    swSet(0x10E9);
    swSet(0x1109);
    pouchGetItem(0x25);
}


void stg8_06_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void swSet(s32 flag);
    extern void evtSetValue(void* event, s32 variable, s32 value);

    pouchRemoveItem(0x25);
    swSet(0x10EA);
    swSet(0x10EB);
    swSet(0x10EC);
    swSet(0x10ED);
    swSet(0x10EE);
    swSet(0x10EF);
    swSet(0x10F0);
    swSet(0x10F1);
    swSet(0x10F2);
    evtSetValue(0, 0xF5DE05E2, 8);
    swSet(0x10F3);
    swSet(0x10F4);
    swSet(0x10F5);
    swSet(0x10F6);
    swSet(0x10F7);
    swSet(0x10F8);
    swSet(0x10F9);
    swSet(0x10FA);
    swSet(0x10FB);
    swSet(0x10FC);
    swSet(0x10FD);
    swSet(0x10FE);
    swSet(0x10FF);
    swSet(0x1100);
    evtSetValue(0, 0xF5DE05E3, 4);
    swSet(0x110A);
    swSet(0x110B);
    swSet(0x110C);
    swSet(0x110D);
    swSet(0x110E);
    swSet(0x110F);
    swSet(0x1110);
    swSet(0x1111);
}

void stg8_10_init(void) { extern void pouchGetItem(s32 item); pouchGetItem(0x2F); }

void stg8_11_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void swSet(s32 flag);
    pouchRemoveItem(0x2F);
    swSet(0x1103);
}

void stg8_13_init(void) {
    extern void swSet(s32 flag);
    swSet(0x1104);
    swSet(0x1105);
    swSet(0x1106);
}

void stg8_14_init(void) {
    extern void pouchGetItem(s32 item);
    extern void swSet(s32 flag);
    pouchGetItem(0x30);
    swSet(0x1112);
}

void stg8_15_init(void) {
    extern void pouchRemoveItem(s32 item);
    extern void swSet(s32 flag);
    pouchRemoveItem(0x30);
    swSet(0x1107);
}

void N_stg9_02_init(void) {
    extern void swSet(s32 flag);
    extern void partyJoin(s32 party);
    swSet(0x150E);
    partyJoin(7);
}

void* eventStgDtPtr(s32 id) {
    extern u8 event_dt[];
    return &event_dt[id * 0x10];
}

s32 eventStgNum(void) {
    return 10;
}
