#include "event/evt_eff.h"

extern s32 evtGetValue();
extern void* effNameToPtr();
extern void effDelete();
extern void effSoftDelete();

s32 evt_eff64(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern s16* effGetSetN64(char*);
    extern s32 strcmp(const char*, const char*);
    extern void effSetName(s32, char*);
    extern char zero_8042033c;

    extern void* effAkariChargeN64Entry();
    extern void* effAkariPoseN64Entry();
    extern void* effAkariSflushN64Entry();
    extern void* effBalloonHeihoN64Entry();
    extern void* effBalloonN64Entry();
    extern void* effBlowN64Entry();
    extern void* effBombN64Entry();
    extern void* effBomheiN64Entry();
    extern void* effBoobooN64Entry();
    extern void* effBreathFireN64Entry();
    extern void* effBrokenBarrierN64Entry();
    extern void* effBushN64Entry();
    extern void* effButterflyN64Entry();
    extern void* effCloudN64Entry();
    extern void* effCoinN64Entry();
    extern void* effConfettiN64Entry();
    extern void* effCoolLeafN64Entry();
    extern void* effCrystalN64Entry();
    extern void* effDogabonAttackN64Entry();
    extern void* effExpBomN64Entry();
    extern void* effExplosionN64Entry();
    extern void* effFallLeafN64Entry();
    extern void* effFireDustN64Entry();
    extern void* effFireFlowerN64Entry();
    extern void* effFireRingN64Entry();
    extern void* effFireSparkN64Entry();
    extern void* effFireworksN64Entry();
    extern void* effFlameN64Entry();
    extern void* effFlowerN64Entry();
    extern void* effFreezeN64Entry();
    extern void* effGarekiN64Entry();
    extern void* effGlassN64Entry();
    extern void* effGuruguruN64Entry();
    extern void* effHagetakaFlushN64Entry();
    extern void* effHitN64Entry();
    extern void* effHokoriN64Entry();
    extern void* effHolographyN64Entry();
    extern void* effHoraoSpellN64Entry();
    extern void* effIceN64Entry();
    extern void* effKamekiTornadeN64Entry();
    extern void* effKemuri10N64Entry();
    extern void* effKemuri11N64Entry();
    extern void* effKemuri12N64Entry();
    extern void* effKemuri1N64Entry();
    extern void* effKemuri2N64Entry();
    extern void* effKemuri3N64Entry();
    extern void* effKemuri4N64Entry();
    extern void* effKemuri5N64Entry();
    extern void* effKemuri6N64Entry();
    extern void* effKemuri7N64Entry();
    extern void* effKemuri8N64Entry();
    extern void* effKemuri9N64Entry();
    extern void* effKumokumoChipN64Entry();
    extern void* effKumokumoN64Entry();
    extern void* effLensflareN64Entry();
    extern void* effMagic1N64Entry();
    extern void* effMagic2N64Entry();
    extern void* effMagic3N64Entry();
    extern void* effMagicHoraoN64Entry();
    extern void* effOnpuN64Entry();
    extern void* effOpukuJetwN64Entry();
    extern void* effOpukuJetwrainN64Entry();
    extern void* effOpukuWcurtainN64Entry();
    extern void* effOpukuWgunN64Entry();
    extern void* effOpukuWprotectN64Entry();
    extern void* effPacknBfireN64Entry();
    extern void* effPeachDustN64Entry();
    extern void* effPikkyoloN64Entry();
    extern void* effPokopiPchargeN64Entry();
    extern void* effPokopiPipoN64Entry();
    extern void* effPokopiThunderN64Entry();
    extern void* effPturnN64Entry();
    extern void* effRadiationN64Entry();
    extern void* effRappaN64Entry();
    extern void* effRecoverHeartN64Entry();
    extern void* effReleaseN64Entry();
    extern void* effRippleN64Entry();
    extern void* effRotHammerFailN64Entry();
    extern void* effRotHammerSuccessN64Entry();
    extern void* effRubbleN64Entry();
    extern void* effShockN64Entry();
    extern void* effSmallStarN64Entry();
    extern void* effSnowDustN64Entry();
    extern void* effSnowN64Entry();
    extern void* effSnowfallN64Entry();
    extern void* effSnowmanN64Entry();
    extern void* effSstarN64Entry();
    extern void* effStampN64Entry();
    extern void* effStarPowerN64Entry();
    extern void* effStardust2N64Entry();
    extern void* effStardustN64Entry();
    extern void* effStoneSmokeN64Entry();
    extern void* effSunlightN64Entry();
    extern void* effSweatN64Entry();
    extern void* effThruHammerN64Entry();
    extern void* effThunderN64Entry();
    extern void* effThunderflareN64Entry();
    extern void* effTimestampN64Entry();
    extern void* effVaporN64Entry();
    extern void* effWangFlushN64Entry();
    extern void* effWaterDamageN64Entry();
    extern void* effWhirlwindN64Entry();

    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32* argBase;
    s32* floatArgs;
    s32* rawArgs;
    char* outName;
    char* setName;
    s32 rawArg3;
    s32 i0, i1, i3, i4, i5, i6, i7, i8, i9;
    f32 f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10;
    void* eff;
    s16* set;
    s32 id;

    outName = (char*)evtGetValue(pEvt, args[0]);
    evtGetValue(pEvt, args[0]);
    evtGetValue(pEvt, args[0]);

    argBase = args + 2;
    setName = (char*)evtGetValue(pEvt, args[1]);
    floatArgs = argBase;
    evtGetValue(pEvt, args[1]);
    rawArgs = argBase;
    evtGetValue(pEvt, args[1]);

    i0 = evtGetValue(pEvt, argBase[0]);
    i1 = evtGetValue(pEvt, argBase[1]);
    evtGetValue(pEvt, argBase[2]);
    i3 = evtGetValue(pEvt, argBase[3]);
    i4 = evtGetValue(pEvt, argBase[4]);
    i5 = evtGetValue(pEvt, argBase[5]);
    i6 = evtGetValue(pEvt, argBase[6]);
    i7 = evtGetValue(pEvt, argBase[7]);
    i8 = evtGetValue(pEvt, argBase[8]);
    i9 = evtGetValue(pEvt, argBase[9]);
    evtGetValue(pEvt, argBase[10]);
    evtGetValue(pEvt, argBase[11]);
    evtGetValue(pEvt, argBase[12]);

    f0 = evtGetFloat(pEvt, floatArgs[0]);
    f1 = evtGetFloat(pEvt, floatArgs[1]);
    f2 = evtGetFloat(pEvt, floatArgs[2]);
    f3 = evtGetFloat(pEvt, floatArgs[3]);
    f4 = evtGetFloat(pEvt, floatArgs[4]);
    f5 = evtGetFloat(pEvt, floatArgs[5]);
    f6 = evtGetFloat(pEvt, floatArgs[6]);
    f7 = evtGetFloat(pEvt, floatArgs[7]);
    f8 = evtGetFloat(pEvt, floatArgs[8]);
    f9 = evtGetFloat(pEvt, floatArgs[9]);
    f10 = evtGetFloat(pEvt, floatArgs[10]);
    evtGetFloat(pEvt, floatArgs[11]);
    evtGetFloat(pEvt, floatArgs[12]);

    rawArg3 = rawArgs[1];
    eff = 0;
    set = effGetSetN64(setName);
    id = *set;

    switch (id) {
        case 0:
            eff = effExpBomN64Entry(f0, f1, f2);
            break;
        case 1:
            eff = effKemuri1N64Entry(f1, f2, f3, f4, i0);
            break;
        case 2:
            eff = effKemuri2N64Entry(f1, f2, f3, f4, f5, i0);
            break;
        case 3:
            eff = effKemuri3N64Entry(f0, f1, f2, f3);
            break;
        case 4:
            eff = effKemuri4N64Entry(f1, f2, f3, f4, f5, i0);
            break;
        case 5:
            eff = effKemuri5N64Entry(f0, f1, f2, f3);
            break;
        case 6:
            eff = effKemuri6N64Entry(f1, f2, f3, i0);
            break;
        case 7:
            eff = effKemuri7N64Entry(f0, f1, f2, f3, f4);
            break;
        case 8:
            eff = effFlowerN64Entry(f1, f2, f3, i0, i4);
            break;
        case 9:
            eff = effSnowN64Entry(f0, f1, f3, i3);
            break;
        case 10:
            eff = effSstarN64Entry(f1, f2, f3, f4, f5, f6, (f32)i7, i0);
            break;
        case 11:
            eff = effStardustN64Entry(f1, f2, f3, f4, i0);
            break;
        case 12:
            eff = effMagic1N64Entry(f1, f2, f3, f4, f5, f6, i0, i7);
            break;
        case 13:
            eff = effMagic2N64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 14:
            eff = effFallLeafN64Entry(f1, f2, f3, i0, i4);
            break;
        case 15:
            eff = effHokoriN64Entry(f1, f2, f3, i0, i4);
            break;
        case 16:
            eff = effGarekiN64Entry(f1, f2, f3, f4, i0);
            break;
        case 17:
            eff = effKemuri8N64Entry(f1, f2, f3, i0);
            break;
        case 18:
            eff = effSmallStarN64Entry(f1, f2, f3, f4, f5, f6, i0, i7);
            break;
        case 19:
            eff = effBombN64Entry(f1, f2, f3, i0);
            break;
        case 20:
            eff = effWangFlushN64Entry(f1, f2, f3, i0, i4);
            break;
        case 21:
            eff = effMagic3N64Entry(f1, f2, f3, i0, i4);
            break;
        case 22:
            eff = effKemuri9N64Entry(f1, f2, f3, f4, f6, i0, i5, i7);
            break;
        case 23:
            eff = effKemuri10N64Entry(f1, f2, f3, f4, f5, f6, f7, i0);
            break;
        case 25:
            eff = effRappaN64Entry(f1, f2, f3, f4, f5, i0, i6);
            break;
        case 26:
            eff = effStardust2N64Entry(f1, f2, f3, f4, f5, i0, i6, i7);
            break;
        case 27:
            eff = effRippleN64Entry(f1, f2, f3, f4, i0);
            break;
        case 28:
            eff = effExplosionN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 29:
            eff = effKemuri11N64Entry(f1, f2, f3, i0);
            break;
        case 30:
            eff = effOnpuN64Entry(f1, f2, f3, i0);
            break;
        case 31:
            eff = effKemuri12N64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 32:
            eff = effSweatN64Entry(f1, f2, f3, f4, f5, i0, i6);
            break;
        case 33:
            eff = effBalloonN64Entry(f1, f2, f3, f4, f5, i0);
            break;
        case 34:
            eff = effCoolLeafN64Entry(f1, f2, f3, i0);
            break;
        case 35:
            eff = effBushN64Entry(f1, f2, f3, i0);
            break;
        case 36:
            eff = effRotHammerSuccessN64Entry(f1, f2, f3, i0, i4, i5);
            break;
        case 37:
            eff = effRotHammerFailN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 38:
            eff = effGuruguruN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 39:
            eff = effIceN64Entry(f1, f2, f3, i0);
            break;
        case 40:
            eff = effStampN64Entry(f1, f2, f3, i0);
            break;
        case 41:
            eff = effThruHammerN64Entry(f1, f2, f3, f4, i0);
            break;
        case 42:
            eff = effRadiationN64Entry(f1, f2, f3, f4, i0);
            break;
        case 43:
            eff = effThunderN64Entry(f1, f2, f3, f4, f5, i0);
            break;
        case 44:
            eff = effBreathFireN64Entry(f1, f2, f3, f4, f5, f6, i0, i7, i8, i9);
            break;
        case 45:
            eff = effBrokenBarrierN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 46:
            eff = effFireRingN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 47:
            eff = effFireDustN64Entry(f1, f2, f3, f4, f5, i0, i6, i7);
            break;
        case 48:
            eff = effFlameN64Entry(f1, f2, f3, f4, i0);
            break;
        case 49:
            eff = effBlowN64Entry(f1, f2, f3, f4, f5, f6, i0, i7);
            break;
        case 50:
            eff = effBomheiN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 51:
            eff = effFireFlowerN64Entry(f1, f2, f3, i0, i4);
            break;
        case 52:
            eff = effRecoverHeartN64Entry(f1, f2, f3, i0, i4);
            break;
        case 53:
            eff = effPturnN64Entry(f1, f2, f3, i0, i4);
            break;
        case 54:
            eff = effHitN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 55:
            eff = effConfettiN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 56:
            eff = effSnowfallN64Entry(i0, i1);
            break;
        case 57:
            eff = effWhirlwindN64Entry(f2, i0, rawArg3, i3);
            break;
        case 58:
            eff = effThunderflareN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 59:
            eff = effCoinN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 60:
            eff = effShockN64Entry(f1, f2, f3, f4, f5, i0);
            break;
        case 61:
            eff = effBalloonHeihoN64Entry(f1, f2, f3, f4, f5, i0, i6);
            break;
        case 62:
            eff = effRubbleN64Entry(f1, f2, f3, f4, f5, f6, f7, i0, i8);
            break;
        case 63:
            effMagicHoraoN64Entry(f1, f2, f3, f4, f6, f8, i0, i5, i7, i9);
            break;
        case 64:
            eff = effLensflareN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 65:
            eff = effCrystalN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 66:
            eff = effButterflyN64Entry(f1, f2, f3, i0);
            break;
        case 67:
            eff = effAkariChargeN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 68:
            eff = effAkariSflushN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 69:
            eff = effOpukuWgunN64Entry(f1, f2, f3, f4, f5, f6, f7, i0, i8);
            break;
        case 70:
            eff = effOpukuWprotectN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 71:
            eff = effOpukuWcurtainN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 72:
            eff = effOpukuJetwN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 73:
            eff = effOpukuJetwrainN64Entry(f1, f2);
            break;
        case 74:
            eff = effPokopiThunderN64Entry(f1, f2, f3, f4, f5, f6, f7, i0, i8);
            break;
        case 75:
            eff = effWaterDamageN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 76:
            eff = effSnowmanN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 77:
            eff = effHolographyN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 78:
            eff = effTimestampN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 79:
            eff = effPacknBfireN64Entry(f1, f2, f3, f4, f5, f6, f7, i0, i8, i9);
            break;
        case 80:
            eff = effPokopiPipoN64Entry(f1, f2, f3, f4, f5, f6, f7, i0, i8);
            break;
        case 81:
            eff = effCloudN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 82:
            eff = effDogabonAttackN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 83:
            eff = effKamekiTornadeN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 84:
            eff = effStoneSmokeN64Entry(f1, f2);
            break;
        case 85:
            eff = effPokopiPchargeN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 86:
            eff = effHagetakaFlushN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 87:
            eff = effKumokumoN64Entry(f1, f2, f3, f4, f5, f6, i0, i7);
            break;
        case 88:
            eff = effVaporN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 89:
            eff = effFireSparkN64Entry(f1, f2, f3, f4, f5, f6, f9, f10, i0, i7, i8);
            break;
        case 90:
            eff = effHoraoSpellN64Entry(f1, f2);
            break;
        case 91:
            eff = effSnowDustN64Entry(f1, f2, f3, f4, f5, f6, i0, i7, i8);
            break;
        case 92:
            eff = effAkariPoseN64Entry(f1, f2, f3, f4, i0, i5, i6);
            break;
        case 93:
            eff = effKumokumoChipN64Entry(f1, f2, f3, f4, f5, f6, f7, f8, i0);
            break;
        case 94:
            eff = effFireworksN64Entry(f1, f2, f3, f4, f5, f6, f7, i0, i8);
            break;
        case 95:
            eff = effGlassN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 96:
            eff = effBoobooN64Entry();
            break;
        case 97:
            eff = effPikkyoloN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 98:
            eff = effFreezeN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 99:
            eff = effSunlightN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 100:
            eff = effReleaseN64Entry(f1, f2, f3, f4, i0, i5);
            break;
        case 101:
            eff = effPeachDustN64Entry(f1, f2, f3, f4, (f32)i5, i0);
            break;
        case 102:
            eff = effStarPowerN64Entry(f1, f2, f3, f4, i0, i5);
            break;
    }

    if ((eff != 0) && (outName != 0) && (strcmp(outName, &zero_8042033c) != 0)) {
        effSetName((s32)eff, outName);
    }

    return 2;
}

u8 evt_eff(s32 pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern s16* effGetSet(char*);
    extern s32 strcmp(const char*, const char*);
    extern void effSetName(s32, char*);
    extern char zero_8042033c;

    extern void* effKemuriEntry();
    extern void* effKemuTestEntry();
    extern void* effStarStoneEntry();
    extern void* effConfettiEntry();
    extern void* effButterflyEntry();
    extern void* effRecoveryEntry();
    extern void* effBreathFireEntry();
    extern void* effHitEntry();
    extern void* effBombEntry();
    extern void* effChargeEntry();
    extern void* effTogeFlushEntry();
    extern void* effIceEntry();
    extern void* effDamageStarEntry();
    extern void* effStarPointEntry();
    extern void* effItemGetEntry();
    extern void* effStageClearEntry();
    extern void* effLevelupEntry();
    extern void* effFallEntry();
    extern void* effMahojinEntry();
    extern void* effNozleEntry();
    extern void* effPuniBalloonEntry();
    extern void* effKissEntry();
    extern void* effSplashEntry();
    extern void* effNiceEntry();
    extern void* effMistEntry();
    extern void* effMist2Entry();
    extern void* effScanningEntry();
    extern void* effParticleEntry();
    extern void* effStardustEntry();
    extern void* effTreasureMapEntry();
    extern void* effSandarsEntry();
    extern void* effSheepEntry();
    extern void* effRankupEntry();
    extern void* effIrekaeruEntry();
    extern void* effSmallStarEntry();
    extern void* effIbukiEntry();
    extern void* effMahornEntry();
    extern void* effToikiEntry();
    extern void* effUranokoEntry();
    extern void* effSyuryouEntry();
    extern void* effHibashiraEntry();
    extern void* effFireEntry();
    extern void* effMachinegunEntry();
    extern void* effRippleEntry();
    extern void* effLasMonEntry();
    extern void* effLaserEntry();
    extern void* effIndirectEntry();
    extern void* effQueenEntry();
    extern void* effQueen2Entry();
    extern void* effGonbabaBreathEntry();
    extern void* effMajinaiEntry();

    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32* argBase;
    s32* floatArgs;
    char* outName;
    char* setName;
    s32 i0;
    s32 i4;
    s32 i5;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 i9;
    f32 f1;
    f32 f2;
    f32 f3;
    f32 f4;
    f32 f5;
    f32 f6;
    f32 f8;
    f32 f10;
    f32 f11;
    void* eff;
    s16* set;
    s32 id;

    outName = (char*)evtGetValue((void*)pEvt, args[0]);
    evtGetValue((void*)pEvt, args[0]);
    evtGetValue((void*)pEvt, args[0]);

    argBase = args + 2;
    setName = (char*)evtGetValue((void*)pEvt, args[1]);
    floatArgs = argBase;
    evtGetValue((void*)pEvt, args[1]);
    evtGetValue((void*)pEvt, args[1]);

    i0 = evtGetValue((void*)pEvt, argBase[0]);
    evtGetValue((void*)pEvt, argBase[1]);
    evtGetValue((void*)pEvt, argBase[2]);
    evtGetValue((void*)pEvt, argBase[3]);
    i4 = evtGetValue((void*)pEvt, argBase[4]);
    i5 = evtGetValue((void*)pEvt, argBase[5]);
    i6 = evtGetValue((void*)pEvt, argBase[6]);
    i7 = evtGetValue((void*)pEvt, argBase[7]);
    i8 = evtGetValue((void*)pEvt, argBase[8]);
    i9 = evtGetValue((void*)pEvt, argBase[9]);
    evtGetValue((void*)pEvt, argBase[10]);
    evtGetValue((void*)pEvt, argBase[11]);
    evtGetValue((void*)pEvt, argBase[12]);

    evtGetFloat((void*)pEvt, floatArgs[0]);
    f1 = evtGetFloat((void*)pEvt, floatArgs[1]);
    f2 = evtGetFloat((void*)pEvt, floatArgs[2]);
    f3 = evtGetFloat((void*)pEvt, floatArgs[3]);
    f4 = evtGetFloat((void*)pEvt, floatArgs[4]);
    f5 = evtGetFloat((void*)pEvt, floatArgs[5]);
    f6 = evtGetFloat((void*)pEvt, floatArgs[6]);
    evtGetFloat((void*)pEvt, floatArgs[7]);
    f8 = evtGetFloat((void*)pEvt, floatArgs[8]);
    evtGetFloat((void*)pEvt, floatArgs[9]);
    f10 = evtGetFloat((void*)pEvt, floatArgs[10]);
    f11 = evtGetFloat((void*)pEvt, floatArgs[11]);
    evtGetFloat((void*)pEvt, floatArgs[12]);

    eff = 0;
    set = effGetSet(setName);
    id = *set;

    switch (id) {
        case 0:
            eff = effKemuriEntry(f1, f2, f3, f4, i0);
            break;
        case 0x1c:
            eff = effKemuTestEntry(f1, f2, f3, f4, i0);
            break;
        case 0x1d:
            eff = effStarStoneEntry(f1, f2, f3, f4, i0);
            break;
        case 1:
            eff = effConfettiEntry(f1, f2, f3, i0, i4);
            break;
        case 3:
            eff = effButterflyEntry(f1, f2, f3);
            break;
        case 10:
            eff = effRecoveryEntry(f1, f2, f3, i0, i4);
            break;
        case 0xd:
            eff = effBreathFireEntry(f1, f2, f3, f4, f5, f6, i0, i7, i8, i9);
            *(f32*)(*(s32*)((s32)eff + 0xc) + 0x38) = f10;
            *(f32*)(*(s32*)((s32)eff + 0xc) + 0x30) = f10;
            *(f32*)(*(s32*)((s32)eff + 0xc) + 0x34) = f11;
            break;
        case 4:
            eff = effHitEntry(f1, f2, f3, f4, i0, i5);
            break;
        case 0x16:
            eff = effBombEntry(f1, f2, f3, f4, i0);
            break;
        case 0x12:
            eff = effChargeEntry(f1, f2, f3, f4, i0, i5);
            break;
        case 0x13:
            eff = effTogeFlushEntry(f1, f2, f3, i0, i4);
            break;
        case 0x14:
            eff = effIceEntry(f1, f2, f3, f4, i0, i5);
            break;
        case 5:
            eff = effDamageStarEntry(f1, f2, f3, (f32)i4, (f32)i5, i0, i6);
            break;
        case 0x19:
            eff = effStarPointEntry(f1, f2, f3, i0, i4);
            break;
        case 0x1e:
            eff = effItemGetEntry(f1, f2, f3, i0);
            break;
        case 0x22:
            eff = effStageClearEntry(f1, f2, f3, i0);
            break;
        case 0x21:
            eff = effLevelupEntry(f1, f2, f3, i0, i4);
            break;
        case 0x23:
            eff = effFallEntry(f1, f2, f3, f4, f5, f6, i0, i7, i8);
            break;
        case 0x27:
            eff = effMahojinEntry(f1, f2, f3, i0);
            break;
        case 0x28:
            eff = effNozleEntry(f1, f2, f3, f4, f5, i0, i6);
            break;
        case 0x1a:
            eff = effPuniBalloonEntry(f1, f2, f3, f4, i0);
            break;
        case 0x2d:
            eff = effKissEntry(f1, f2, f3, f4, i0);
            break;
        case 0x2b:
            eff = effSplashEntry(f1, f2, f3, f4, i0);
            break;
        case 0xc:
            eff = effNiceEntry(f1, f2, f3, i0);
            break;
        case 0x2f:
            eff = effMistEntry(f1, f2, f3, f4, i0, i5);
            break;
        case 0x3a:
            eff = effMist2Entry(f1, f2, f3, f4, f5, i0, i6);
            break;
        case 0x38:
            eff = effScanningEntry(f1, f2, f3, i0);
            break;
        case 0x3b:
            eff = effParticleEntry(f1, f2, f3, f4, f5, f6, i0, i7, i8);
            break;
        case 9:
            effStardustEntry(f1, f2, f3, f4, f5, i0, i6, i7);
            break;
        case 0x2c:
            eff = effTreasureMapEntry(f1, f2, f3, i0);
            break;
        case 0x34:
            eff = effSandarsEntry(f1, f2, f3, (f32)i4, i0);
            break;
        case 0x32:
            eff = effSheepEntry(f1, f2, f3, i0);
            break;
        case 0x37:
            eff = effRankupEntry(f1, f2, f3, i0);
            break;
        case 0x36:
            eff = effIrekaeruEntry(f1, f2, f3, i0);
            break;
        case 6:
            eff = effSmallStarEntry(f1, f2, f3, f4, f5, f6, i0, i7);
            break;
        case 0x3c:
            eff = effIbukiEntry(f1, f2, f3, i0, i4);
            break;
        case 0x3d:
            eff = effMahornEntry(f1, f2, f3, f4, i0, i5, i6);
            break;
        case 0x39:
            eff = effToikiEntry(f1, f2, f3, i0, i4);
            break;
        case 0x41:
            eff = effUranokoEntry(f1, f2, f3, i0, i4);
            break;
        case 0x40:
            eff = effSyuryouEntry(f1, f2, f3, i0, i4);
            break;
        case 0x42:
            eff = effHibashiraEntry(f1, f2, f3, f4, f5, f6, f8, i0, i7, i9);
            break;
        case 0x15:
            eff = effFireEntry(f1, f2, f3, f4, i0, i5);
            break;
        case 0x45:
            eff = effMachinegunEntry(f1, f2, f3, f4, f5, f6);
            break;
        case 0x17:
            eff = effRippleEntry(f1, f2, f3, i0);
            break;
        case 0x46:
            eff = effLasMonEntry(f1, f2, f3, i0);
            break;
        case 0x44:
            eff = effLaserEntry(f1, f2, f3, i0);
            break;
        case 0x3f:
            eff = effIndirectEntry(f1, f2, f3, i0, i4);
            break;
        case 0x4b:
            eff = effQueenEntry(f1, f2, f3, f4, i0, i5);
            break;
        case 0x4c:
            eff = effQueen2Entry(f1, f2, f3, i0);
            break;
        case 0x4d:
            eff = effGonbabaBreathEntry(f1, f2, f3, f4, i0, i5);
            break;
        case 0x4e:
            eff = effMajinaiEntry(f1, f2, f3, i0);
            break;
    }

    if ((eff != 0) && (outName != 0) && (strcmp(outName, &zero_8042033c) != 0)) {
        effSetName((s32)eff, outName);
    }

    return 2;
}

u8 evt_eff_fukidashi(s32 pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void* marioGetPtr(void);
    extern s32 strcmp(const char*, const char*);
    extern void* npcNameToPtr(char*);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32);
    extern s32 effFukidashiEntry(f64, f64, f64, f64, f64, s32, s32, s32, s32);
    extern void effSetName(s32, char*);
    extern const char str_me_80420334[];
    extern const char zero_8042033c[];
    extern f32 float_0_80420330;
    extern f32 float_2_80420328;
    extern f32 float_3_8042032c;
    extern f32 float_30_80420338;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 type = evtGetValue((void*)pEvt, args[0]);
    char* name = (char*)evtGetValue((void*)pEvt, args[1]);
    char* target = (char*)evtGetValue((void*)pEvt, args[2]);
    s32 kind = evtGetValue((void*)pEvt, args[3]);
    s32 duration = evtGetValue((void*)pEvt, args[4]);
    f32 x = evtGetFloat((void*)pEvt, args[5]);
    f32 y = evtGetFloat((void*)pEvt, args[6]);
    f32 z = evtGetFloat((void*)pEvt, args[7]);
    f32 w = evtGetFloat((void*)pEvt, args[8]);
    f32 scale = evtGetFloat((void*)pEvt, args[9]);
    s32 flags = evtGetValue((void*)pEvt, args[10]);
    void* ptr;
    f32 h;
    s32 eff = 0;

    if (type == 0) {
        ptr = marioGetPtr();
        h = *(f32*)((s32)ptr + 0x1B8);
        eff = effFukidashiEntry(float_0_80420330, (float_2_80420328 * h) / float_3_8042032c,
                                float_0_80420330, h / float_3_8042032c, scale,
                                kind, duration, -1, flags);
    } else if (type == 1) {
        if (strcmp(target, str_me_80420334) == 0) {
            ptr = *(void**)((s32)pEvt + 0x174);
        } else {
            ptr = npcNameToPtr(target);
        }
        h = *(f32*)((s32)ptr + 0xA4);
        eff = effFukidashiEntry(float_0_80420330, (float_2_80420328 * h) / float_3_8042032c,
                                float_0_80420330, h / float_3_8042032c, scale,
                                kind, duration, (s32)ptr, flags);
    } else if (type == 2) {
        eff = effFukidashiEntry(x, y, z, w, scale, kind, duration, 0, flags);
    } else if (type == 3) {
        ptr = partyGetPtr(marioGetPartyId());
        if (ptr != 0) {
            h = *(f32*)((s32)ptr + 0xA4);
            y = (float_2_80420328 * h) / float_3_8042032c;
            if (*(s32*)((s32)ptr + 0x10) == 2) {
                y = float_30_80420338;
            }
            eff = effFukidashiEntry(float_0_80420330, y, float_0_80420330,
                                    h / float_3_8042032c, scale, kind, duration, -2, flags);
        }
    } else if (type == 4) {
        ptr = partyGetPtr(marioGetExtraPartyId());
        if (ptr != 0) {
            h = *(f32*)((s32)ptr + 0xA4);
            eff = effFukidashiEntry(float_0_80420330, (float_2_80420328 * h) / float_3_8042032c,
                                    float_0_80420330, h / float_3_8042032c, scale,
                                    kind, duration, -3, flags);
        }
    }
    if ((name != 0) && (strcmp(name, zero_8042033c) != 0) && (eff != 0)) {
        effSetName(eff, name);
    }
    return 2;
}

s32 evt_eff_delete(void* pEvt) {
    effDelete(effNameToPtr(evtGetValue(pEvt, **(s32***)((s32)pEvt + 0x18))));
    return 2;
}


s32 evt_eff_softdelete(void* pEvt) {
    effSoftDelete(effNameToPtr(evtGetValue(pEvt, **(s32***)((s32)pEvt + 0x18))));
    return 2;
}


s32 evt_eff_delete_ptr(void* pEvt) {
    effDelete(evtGetValue(pEvt, **(s32***)((s32)pEvt + 0x18)));
    return 2;
}


s32 evt_eff_softdelete_ptr(void* pEvt) {
    effSoftDelete(evtGetValue(pEvt, **(s32***)((s32)pEvt + 0x18)));
    return 2;
}
