#include "event/evt_johoya.h"
#include "event/evt_cmd.h"

extern int sprintf(char* str, const char* format, ...);
extern char* msgSearch(char* key);
extern int strcmp(const char* s1, const char* s2);

typedef struct JohoyaEntry {
    u16 flags;
    u8 pad2[0xA];
    s16 unkC;
    u8 padE[2];
} JohoyaEntry;

typedef struct JohoyaData {
    JohoyaEntry* entries;
    s32 count;
    s32 unk8;
    JohoyaEntry* end;
    s32 index;
    u8 flags[0x27];
    u8 pad3B[5];
} JohoyaData;

const char str_STG0_00_802fc318[] = "STG0_00";
const char str_STG0_01_802fc320[] = "STG0_01";
const char str_STG0_02_802fc328[] = "STG0_02";
const char str_STG0_03_802fc330[] = "STG0_03";
const char str_STG0_04_802fc338[] = "STG0_04";
const char str_STG0_05_802fc340[] = "STG0_05";
const char str_STG0_06_802fc348[] = "STG0_06";
const char str_STG0_07_802fc350[] = "STG0_07";
const char str_STG0_08_802fc358[] = "STG0_08";
const char str_STG0_09_802fc360[] = "STG0_09";
const char str_STG0_10_802fc368[] = "STG0_10";
const char str_STG0_10_01_802fc370[] = "STG0_10_01";
const char str_STG0_11_802fc37c[] = "STG0_11";
const char str_STG0_12_802fc384[] = "STG0_12";
const char str_STG0_12_01_802fc38c[] = "STG0_12_01";
const char str_STG0_12_02_802fc398[] = "STG0_12_02";
const char str_STG0_13_802fc3a4[] = "STG0_13";
const char str_STG0_14_802fc3ac[] = "STG0_14";
const char str_STG0_15_802fc3b4[] = "STG0_15";
const char str_STG0_16_802fc3bc[] = "STG0_16";
const char str_STG0_17_802fc3c4[] = "STG0_17";
const char str_STG0_19_802fc3cc[] = "STG0_19";
const char str_STG0_END_802fc3d4[] = "STG0_END";
const char str_STG1_00_802fc3e0[] = "STG1_00";
const char str_STG1_00_01_802fc3e8[] = "STG1_00_01";
const char str_STG1_01_802fc3f4[] = "STG1_01";
const char str_STG1_01a_802fc3fc[] = "STG1_01a";
const char str_STG1_02_802fc408[] = "STG1_02";
const char str_STG1_03_802fc410[] = "STG1_03";
const char str_STG1_04_802fc418[] = "STG1_04";
const char str_STG1_05_802fc420[] = "STG1_05";
const char str_STG1_06_802fc428[] = "STG1_06";
const char str_STG1_06_01_802fc430[] = "STG1_06_01";
const char str_STG1_07_802fc43c[] = "STG1_07";
const char str_STG1_08_802fc444[] = "STG1_08";
const char str_STG1_09_802fc44c[] = "STG1_09";
const char str_STG1_09_01_802fc454[] = "STG1_09_01";
const char str_STG1_09_02_802fc460[] = "STG1_09_02";
const char str_STG1_10_802fc46c[] = "STG1_10";
const char str_STG1_11_802fc474[] = "STG1_11";
const char str_STG1_12_802fc47c[] = "STG1_12";
const char str_STG1_12_01_802fc484[] = "STG1_12_01";
const char str_STG1_12_02_802fc490[] = "STG1_12_02";
const char str_STG1_13_802fc49c[] = "STG1_13";
const char str_STG1_13_01_802fc4a4[] = "STG1_13_01";
const char str_STG1_13_01_01_802fc4b0[] = "STG1_13_01_01";
const char str_STG1_13_02_802fc4c0[] = "STG1_13_02";
const char str_STG1_13_03_802fc4cc[] = "STG1_13_03";
const char str_STG1_14_802fc4d8[] = "STG1_14";
const char str_STG1_14_01_802fc4e0[] = "STG1_14_01";
const char str_STG1_15_802fc4ec[] = "STG1_15";
const char str_STG1_15_01_802fc4f4[] = "STG1_15_01";
const char str_STG1_15_02_802fc500[] = "STG1_15_02";
const char str_STG1_15_03_802fc50c[] = "STG1_15_03";
const char str_STG1_15_04_802fc518[] = "STG1_15_04";
const char str_STG1_16_802fc524[] = "STG1_16";
const char str_STG1_17_802fc52c[] = "STG1_17";
const char str_STG1_18_802fc534[] = "STG1_18";
const char str_STG1_18_01_802fc53c[] = "STG1_18_01";
const char str_STG1_19_802fc548[] = "STG1_19";
const char str_STG1_19_01_802fc550[] = "STG1_19_01";
const char str_STG1_19_02_802fc55c[] = "STG1_19_02";
const char str_STG1_20_802fc568[] = "STG1_20";
const char str_STG1_21_802fc570[] = "STG1_21";
const char str_STG1_30_802fc578[] = "STG1_30";
const char str_STG1_31_802fc580[] = "STG1_31";
const char str_STG1_32_802fc588[] = "STG1_32";
const char str_STG1_33_802fc590[] = "STG1_33";
const char str_STG1_34_802fc598[] = "STG1_34";
const char str_STG1_35_802fc5a0[] = "STG1_35";
const char str_STG1_END_802fc5a8[] = "STG1_END";
const char str_STG2_00_802fc5b4[] = "STG2_00";
const char str_STG2_01_802fc5bc[] = "STG2_01";
const char str_STG2_02_802fc5c4[] = "STG2_02";
const char str_STG2_03_802fc5cc[] = "STG2_03";
const char str_STG2_03_01_802fc5d4[] = "STG2_03_01";
const char str_STG2_03_02_802fc5e0[] = "STG2_03_02";
const char str_STG2_03_03_802fc5ec[] = "STG2_03_03";
const char str_STG2_03_04_802fc5f8[] = "STG2_03_04";
const char str_STG2_03_05_802fc604[] = "STG2_03_05";
const char str_STG2_04_802fc610[] = "STG2_04";
const char str_STG2_05_802fc618[] = "STG2_05";
const char str_STG2_05_01_802fc620[] = "STG2_05_01";
const char str_STG2_05_02_802fc62c[] = "STG2_05_02";
const char str_STG2_06_802fc638[] = "STG2_06";
const char str_STG2_07_802fc640[] = "STG2_07";
const char str_STG2_08_802fc648[] = "STG2_08";
const char str_STG2_09_802fc650[] = "STG2_09";
const char str_STG2_10_802fc658[] = "STG2_10";
const char str_STG2_11_802fc660[] = "STG2_11";
const char str_STG2_12_802fc668[] = "STG2_12";
const char str_STG2_12_01_802fc670[] = "STG2_12_01";
const char str_STG2_12_02_802fc67c[] = "STG2_12_02";
const char str_STG2_13_802fc688[] = "STG2_13";
const char str_STG2_14_802fc690[] = "STG2_14";
const char str_STG2_15_802fc698[] = "STG2_15";
const char str_STG2_16_802fc6a0[] = "STG2_16";
const char str_STG2_17_802fc6a8[] = "STG2_17";
const char str_STG2_18_802fc6b0[] = "STG2_18";
const char str_STG2_19_802fc6b8[] = "STG2_19";
const char str_STG2_19_01_802fc6c0[] = "STG2_19_01";
const char str_STG2_20_802fc6cc[] = "STG2_20";
const char str_STG2_21_802fc6d4[] = "STG2_21";
const char str_STG2_21_01_802fc6dc[] = "STG2_21_01";
const char str_STG2_21_02_802fc6e8[] = "STG2_21_02";
const char str_STG2_21_03_802fc6f4[] = "STG2_21_03";
const char str_STG2_21_04_802fc700[] = "STG2_21_04";
const char str_STG2_21_06_802fc70c[] = "STG2_21_06";
const char str_STG2_22_802fc718[] = "STG2_22";
const char str_STG2_23_802fc720[] = "STG2_23";
const char str_STG2_23_01_802fc728[] = "STG2_23_01";
const char str_STG2_24_802fc734[] = "STG2_24";
const char str_STG2_25_802fc73c[] = "STG2_25";
const char str_STG2_25_01_802fc744[] = "STG2_25_01";
const char str_STG2_25_02_802fc750[] = "STG2_25_02";
const char str_STG2_25_03_802fc75c[] = "STG2_25_03";
const char str_STG2_25_04_802fc768[] = "STG2_25_04";
const char str_STG2_25_05_802fc774[] = "STG2_25_05";
const char str_STG2_26_802fc780[] = "STG2_26";
const char str_STG2_27_802fc788[] = "STG2_27";
const char str_STG2_40_802fc790[] = "STG2_40";
const char str_STG2_41_802fc798[] = "STG2_41";
const char str_STG2_44_802fc7a0[] = "STG2_44";
const char str_STG2_45_802fc7a8[] = "STG2_45";
const char str_STG2_46_802fc7b0[] = "STG2_46";
const char str_STG2_47_802fc7b8[] = "STG2_47";
const char str_STG2_END_802fc7c0[] = "STG2_END";
const char str_STG3_00_802fc7cc[] = "STG3_00";
const char str_STG3_01_802fc7d4[] = "STG3_01";
const char str_STG3_02_802fc7dc[] = "STG3_02";
const char str_STG3_03_802fc7e4[] = "STG3_03";
const char str_STG3_04_802fc7ec[] = "STG3_04";
const char str_STG3_05_802fc7f4[] = "STG3_05";
const char str_STG3_06_802fc7fc[] = "STG3_06";
const char str_STG3_07_802fc804[] = "STG3_07";
const char str_STG3_10_802fc80c[] = "STG3_10";
const char str_STG3_11_802fc814[] = "STG3_11";
const char str_STG3_12_802fc81c[] = "STG3_12";
const char str_STG3_20_802fc824[] = "STG3_20";
const char str_STG3_20_00_802fc82c[] = "STG3_20_00";
const char str_STG3_20_01_802fc838[] = "STG3_20_01";
const char str_STG3_20_02_802fc844[] = "STG3_20_02";
const char str_STG3_20_03_802fc850[] = "STG3_20_03";
const char str_STG3_20_04_802fc85c[] = "STG3_20_04";
const char str_STG3_20_05_802fc868[] = "STG3_20_05";
const char str_STG3_20_06_802fc874[] = "STG3_20_06";
const char str_STG3_20_07_802fc880[] = "STG3_20_07";
const char str_STG3_20_07_00_802fc88c[] = "STG3_20_07_00";
const char str_STG3_20_08_802fc89c[] = "STG3_20_08";
const char str_STG3_20_09_802fc8a8[] = "STG3_20_09";
const char str_STG3_20_10_802fc8b4[] = "STG3_20_10";
const char str_STG3_20_11_802fc8c0[] = "STG3_20_11";
const char str_STG3_20_12_802fc8cc[] = "STG3_20_12";
const char str_STG3_20_13_802fc8d8[] = "STG3_20_13";
const char str_STG3_20_14_802fc8e4[] = "STG3_20_14";
const char str_STG3_20_15_802fc8f0[] = "STG3_20_15";
const char str_STG3_20_16_802fc8fc[] = "STG3_20_16";
const char str_STG3_21_802fc908[] = "STG3_21";
const char str_STG3_30_802fc910[] = "STG3_30";
const char str_STG3_30_01_802fc918[] = "STG3_30_01";
const char str_STG3_30_02_802fc924[] = "STG3_30_02";
const char str_STG3_30_03_802fc930[] = "STG3_30_03";
const char str_STG3_30_04_802fc93c[] = "STG3_30_04";
const char str_STG3_31_802fc948[] = "STG3_31";
const char str_STG3_32_802fc950[] = "STG3_32";
const char str_STG3_33_802fc958[] = "STG3_33";
const char str_STG3_33_01_802fc960[] = "STG3_33_01";
const char str_STG3_33_02_802fc96c[] = "STG3_33_02";
const char str_STG3_33_03_802fc978[] = "STG3_33_03";
const char str_STG3_33_04_802fc984[] = "STG3_33_04";
const char str_STG3_33_05_802fc990[] = "STG3_33_05";
const char str_STG3_33_06_802fc99c[] = "STG3_33_06";
const char str_STG3_34_802fc9a8[] = "STG3_34";
const char str_STG3_35_802fc9b0[] = "STG3_35";
const char str_STG3_50_802fc9b8[] = "STG3_50";
const char str_STG3_51_802fc9c0[] = "STG3_51";
const char str_STG3_52_802fc9c8[] = "STG3_52";
const char str_STG3_END_802fc9d0[] = "STG3_END";
const char str_STG4_00_802fc9dc[] = "STG4_00";
const char str_STG4_01_00_802fc9e4[] = "STG4_01_00";
const char str_STG4_01_802fc9f0[] = "STG4_01";
const char str_STG4_02_802fc9f8[] = "STG4_02";
const char str_STG4_02_02_802fca00[] = "STG4_02_02";
const char str_STG4_02_03_802fca0c[] = "STG4_02_03";
const char str_STG4_03_802fca18[] = "STG4_03";
const char str_STG4_04_802fca20[] = "STG4_04";
const char str_STG4_04_01_00_802fca28[] = "STG4_04_01_00";
const char str_STG4_04_01_802fca38[] = "STG4_04_01";
const char str_STG4_05_802fca44[] = "STG4_05";
const char str_STG4_06_802fca4c[] = "STG4_06";
const char str_STG4_06_01_802fca54[] = "STG4_06_01";
const char str_STG4_06_02_802fca60[] = "STG4_06_02";
const char str_STG4_07_802fca6c[] = "STG4_07";
const char str_STG4_08_802fca74[] = "STG4_08";
const char str_STG4_09_802fca7c[] = "STG4_09";
const char str_STG4_09_01_802fca84[] = "STG4_09_01";
const char str_STG4_09_02_802fca90[] = "STG4_09_02";
const char str_STG4_09_03_802fca9c[] = "STG4_09_03";
const char str_STG4_09_04_802fcaa8[] = "STG4_09_04";
const char str_STG4_09_05_802fcab4[] = "STG4_09_05";
const char str_STG4_10_802fcac0[] = "STG4_10";
const char str_STG4_10_02_802fcac8[] = "STG4_10_02";
const char str_STG4_11_802fcad4[] = "STG4_11";
const char str_STG4_11_02_802fcadc[] = "STG4_11_02";
const char str_STG4_12_802fcae8[] = "STG4_12";
const char str_STG4_12_01_802fcaf0[] = "STG4_12_01";
const char str_STG4_13_802fcafc[] = "STG4_13";
const char str_STG4_13_02_802fcb04[] = "STG4_13_02";
const char str_STG4_13_03_802fcb10[] = "STG4_13_03";
const char str_STG4_13_04_802fcb1c[] = "STG4_13_04";
const char str_STG4_14_802fcb28[] = "STG4_14";
const char str_STG4_15_802fcb30[] = "STG4_15";
const char str_STG4_15_02_802fcb38[] = "STG4_15_02";
const char str_STG4_15_03_802fcb44[] = "STG4_15_03";
const char str_STG4_15_04_802fcb50[] = "STG4_15_04";
const char str_STG4_15_05_802fcb5c[] = "STG4_15_05";
const char str_STG4_15_06_802fcb68[] = "STG4_15_06";
const char str_STG4_15_07_802fcb74[] = "STG4_15_07";
const char str_STG4_15_08_802fcb80[] = "STG4_15_08";
const char str_STG4_16_802fcb8c[] = "STG4_16";
const char str_STG4_17_802fcb94[] = "STG4_17";
const char str_STG4_30_802fcb9c[] = "STG4_30";
const char str_STG4_31_802fcba4[] = "STG4_31";
const char str_STG4_32_802fcbac[] = "STG4_32";
const char str_STG4_33_802fcbb4[] = "STG4_33";
const char str_STG4_34_802fcbbc[] = "STG4_34";
const char str_STG4_35_802fcbc4[] = "STG4_35";
const char str_STG4_36_802fcbcc[] = "STG4_36";
const char str_STG4_37_802fcbd4[] = "STG4_37";
const char str_STG4_END_802fcbdc[] = "STG4_END";
const char str_STG5_00_802fcbe8[] = "STG5_00";
const char str_STG5_10_802fcbf0[] = "STG5_10";
const char str_STG5_11_802fcbf8[] = "STG5_11";
const char str_STG5_12_802fcc00[] = "STG5_12";
const char str_STG5_13_802fcc08[] = "STG5_13";
const char str_STG5_15_802fcc10[] = "STG5_15";
const char str_STG5_16_802fcc18[] = "STG5_16";
const char str_STG5_17_802fcc20[] = "STG5_17";
const char str_STG5_18_802fcc28[] = "STG5_18";
const char str_STG5_18_01_802fcc30[] = "STG5_18_01";
const char str_STG5_18_02_802fcc3c[] = "STG5_18_02";
const char str_STG5_18_03_802fcc48[] = "STG5_18_03";
const char str_STG5_19_802fcc54[] = "STG5_19";
const char str_STG5_19_01_802fcc5c[] = "STG5_19_01";
const char str_STG5_19_02_802fcc68[] = "STG5_19_02";
const char str_STG5_20_802fcc74[] = "STG5_20";
const char str_STG5_21_802fcc7c[] = "STG5_21";
const char str_STG5_21_01_802fcc84[] = "STG5_21_01";
const char str_STG5_22_802fcc90[] = "STG5_22";
const char str_STG5_29_802fcc98[] = "STG5_29";
const char str_STG5_30_802fcca0[] = "STG5_30";
const char str_STG5_31_802fcca8[] = "STG5_31";
const char str_STG5_32_802fccb0[] = "STG5_32";
const char str_STG5_33_802fccb8[] = "STG5_33";
const char str_STG5_34_802fccc0[] = "STG5_34";
const char str_STG5_35_802fccc8[] = "STG5_35";
const char str_STG5_36_802fccd0[] = "STG5_36";
const char str_STG5_40_802fccd8[] = "STG5_40";
const char str_STG5_41_802fcce0[] = "STG5_41";
const char str_STG5_41_01_802fcce8[] = "STG5_41_01";
const char str_STG5_42_802fccf4[] = "STG5_42";
const char str_STG5_43_802fccfc[] = "STG5_43";
const char str_STG5_44_802fcd04[] = "STG5_44";
const char str_STG5_45_802fcd0c[] = "STG5_45";
const char str_STG5_46_802fcd14[] = "STG5_46";
const char str_STG5_47_802fcd1c[] = "STG5_47";
const char str_STG5_48_802fcd24[] = "STG5_48";
const char str_STG5_49_802fcd2c[] = "STG5_49";
const char str_STG5_58_802fcd34[] = "STG5_58";
const char str_STG5_59_802fcd3c[] = "STG5_59";
const char str_STG5_59_01_802fcd44[] = "STG5_59_01";
const char str_STG5_60_802fcd50[] = "STG5_60";
const char str_STG5_61_802fcd58[] = "STG5_61";
const char str_STG5_62_802fcd60[] = "STG5_62";
const char str_STG5_63_802fcd68[] = "STG5_63";
const char str_STG5_64_802fcd70[] = "STG5_64";
const char str_STG5_64_01_802fcd78[] = "STG5_64_01";
const char str_STG5_64_02_802fcd84[] = "STG5_64_02";
const char str_STG5_64_03_802fcd90[] = "STG5_64_03";
const char str_STG5_65_802fcd9c[] = "STG5_65";
const char str_STG5_66_802fcda4[] = "STG5_66";
const char str_STG5_END_802fcdac[] = "STG5_END";
const char str_STG6_00_802fcdb8[] = "STG6_00";
const char str_STG6_01_802fcdc0[] = "STG6_01";
const char str_STG6_02_802fcdc8[] = "STG6_02";
const char str_STG6_03_802fcdd0[] = "STG6_03";
const char str_STG6_04_802fcdd8[] = "STG6_04";
const char str_STG6_05_802fcde0[] = "STG6_05";
const char str_STG6_06_802fcde8[] = "STG6_06";
const char str_STG6_07_802fcdf0[] = "STG6_07";
const char str_STG6_08_802fcdf8[] = "STG6_08";
const char str_STG6_09_802fce00[] = "STG6_09";
const char str_STG6_10_802fce08[] = "STG6_10";
const char str_STG6_11_802fce10[] = "STG6_11";
const char str_STG6_12_802fce18[] = "STG6_12";
const char str_STG6_13_802fce20[] = "STG6_13";
const char str_STG6_14_802fce28[] = "STG6_14";
const char str_STG6_15_802fce30[] = "STG6_15";
const char str_STG6_16_802fce38[] = "STG6_16";
const char str_STG6_17_802fce40[] = "STG6_17";
const char str_STG6_18_802fce48[] = "STG6_18";
const char str_STG6_19_802fce50[] = "STG6_19";
const char str_STG6_20_802fce58[] = "STG6_20";
const char str_STG6_21_802fce60[] = "STG6_21";
const char str_STG6_22_802fce68[] = "STG6_22";
const char str_STG6_23_802fce70[] = "STG6_23";
const char str_STG6_24_802fce78[] = "STG6_24";
const char str_STG6_24_02_802fce80[] = "STG6_24_02";
const char str_STG6_25_802fce8c[] = "STG6_25";
const char str_STG6_26_802fce94[] = "STG6_26";
const char str_STG6_27_802fce9c[] = "STG6_27";
const char str_STG6_28_802fcea4[] = "STG6_28";
const char str_STG6_29_802fceac[] = "STG6_29";
const char str_STG6_30_802fceb4[] = "STG6_30";
const char str_STG6_31_802fcebc[] = "STG6_31";
const char str_STG6_32_802fcec4[] = "STG6_32";
const char str_STG6_33_802fcecc[] = "STG6_33";
const char str_STG6_34_802fced4[] = "STG6_34";
const char str_STG6_35_802fcedc[] = "STG6_35";
const char str_STG6_36_802fcee4[] = "STG6_36";
const char str_STG6_37_802fceec[] = "STG6_37";
const char str_STG6_38_802fcef4[] = "STG6_38";
const char str_STG6_39_802fcefc[] = "STG6_39";
const char str_STG6_40_802fcf04[] = "STG6_40";
const char str_STG6_41_802fcf0c[] = "STG6_41";
const char str_STG6_42_802fcf14[] = "STG6_42";
const char str_STG6_43_802fcf1c[] = "STG6_43";
const char str_STG6_44_802fcf24[] = "STG6_44";
const char str_STG6_45_802fcf2c[] = "STG6_45";
const char str_STG6_46_802fcf34[] = "STG6_46";
const char str_STG6_47_00_802fcf3c[] = "STG6_47_00";
const char str_STG6_47_802fcf48[] = "STG6_47";
const char str_STG6_48_802fcf50[] = "STG6_48";
const char str_STG6_49_00_802fcf58[] = "STG6_49_00";
const char str_STG6_49_802fcf64[] = "STG6_49";
const char str_STG6_50_802fcf6c[] = "STG6_50";
const char str_STG6_51_802fcf74[] = "STG6_51";
const char str_STG6_52_802fcf7c[] = "STG6_52";
const char str_STG6_53_802fcf84[] = "STG6_53";
const char str_STG6_54_802fcf8c[] = "STG6_54";
const char str_STG6_55_802fcf94[] = "STG6_55";
const char str_STG6_55_01_802fcf9c[] = "STG6_55_01";
const char str_STG6_55_02_802fcfa8[] = "STG6_55_02";
const char str_STG6_55_03_802fcfb4[] = "STG6_55_03";
const char str_STG6_55_04_802fcfc0[] = "STG6_55_04";
const char str_STG6_56_802fcfcc[] = "STG6_56";
const char str_STG6_57_802fcfd4[] = "STG6_57";
const char str_STG6_70_802fcfdc[] = "STG6_70";
const char str_STG6_71_802fcfe4[] = "STG6_71";
const char str_STG6_71_01_802fcfec[] = "STG6_71_01";
const char str_STG6_72_802fcff8[] = "STG6_72";
const char str_STG6_END_802fd000[] = "STG6_END";
const char str_STG7_00_802fd00c[] = "STG7_00";
const char str_STG7_01_802fd014[] = "STG7_01";
const char str_STG7_02_802fd01c[] = "STG7_02";
const char str_STG7_03_802fd024[] = "STG7_03";
const char str_STG7_04_802fd02c[] = "STG7_04";
const char str_STG7_05_802fd034[] = "STG7_05";
const char str_STG7_06_802fd03c[] = "STG7_06";
const char str_STG7_07_802fd044[] = "STG7_07";
const char str_STG7_07_01_802fd04c[] = "STG7_07_01";
const char str_STG7_08_802fd058[] = "STG7_08";
const char str_STG7_09_802fd060[] = "STG7_09";
const char str_STG7_10_802fd068[] = "STG7_10";
const char str_STG7_11_802fd070[] = "STG7_11";
const char str_STG7_12_802fd078[] = "STG7_12";
const char str_STG7_13_802fd080[] = "STG7_13";
const char str_STG7_14_802fd088[] = "STG7_14";
const char str_STG7_15_802fd090[] = "STG7_15";
const char str_STG7_16_802fd098[] = "STG7_16";
const char str_STG7_17_802fd0a0[] = "STG7_17";
const char str_STG7_18_802fd0a8[] = "STG7_18";
const char str_STG7_19_802fd0b0[] = "STG7_19";
const char str_STG7_20_802fd0b8[] = "STG7_20";
const char str_STG7_25_00_802fd0c0[] = "STG7_25_00";
const char str_STG7_25_00_1_802fd0cc[] = "STG7_25_00_1";
const char str_STG7_25_802fd0dc[] = "STG7_25";
const char str_STG7_26_802fd0e4[] = "STG7_26";
const char str_STG7_50_802fd0ec[] = "STG7_50";
const char str_STG7_51_802fd0f4[] = "STG7_51";
const char str_STG7_52_802fd0fc[] = "STG7_52";
const char str_STG7_END_802fd104[] = "STG7_END";
const char str_STG8_00_802fd110[] = "STG8_00";
const char str_STG8_01_802fd118[] = "STG8_01";
const char str_STG8_01_01_802fd120[] = "STG8_01_01";
const char str_STG8_01_02_802fd12c[] = "STG8_01_02";
const char str_STG8_02_802fd138[] = "STG8_02";
const char str_STG8_03_802fd140[] = "STG8_03";
const char str_STG8_04_802fd148[] = "STG8_04";
const char str_STG8_05_802fd150[] = "STG8_05";
const char str_STG8_06_802fd158[] = "STG8_06";
const char str_STG8_07_802fd160[] = "STG8_07";
const char str_STG8_08_802fd168[] = "STG8_08";
const char str_STG8_09_802fd170[] = "STG8_09";
const char str_STG8_10_802fd178[] = "STG8_10";
const char str_STG8_11_802fd180[] = "STG8_11";
const char str_STG8_12_802fd188[] = "STG8_12";
const char str_STG8_13_802fd190[] = "STG8_13";
const char str_STG8_14_802fd198[] = "STG8_14";
const char str_STG8_14_01_802fd1a0[] = "STG8_14_01";
const char str_STG8_15_802fd1ac[] = "STG8_15";
const char str_STG8_16_802fd1b4[] = "STG8_16";
const char str_STG8_END_802fd1bc[] = "STG8_END";
const char str_STG9_00_802fd1c8[] = "STG9_00";
const char str_STG9_01_802fd1d0[] = "STG9_01";
const char str_STG9_02_802fd1d8[] = "STG9_02";
const char str_STG9_END_802fd1e0[] = "STG9_END";

extern void* mapalloc_base_ptr;
JohoyaData _jdt = {0};
char* evtNoLabel[] = {
    (char*)str_STG0_00_802fc318, (char*)str_STG0_01_802fc320, (char*)str_STG0_02_802fc328, (char*)str_STG0_03_802fc330,
    (char*)str_STG0_04_802fc338, (char*)str_STG0_05_802fc340, (char*)str_STG0_06_802fc348, (char*)str_STG0_07_802fc350,
    (char*)str_STG0_08_802fc358, (char*)str_STG0_09_802fc360, (char*)str_STG0_10_802fc368, (char*)str_STG0_10_01_802fc370,
    (char*)str_STG0_11_802fc37c, (char*)str_STG0_12_802fc384, (char*)str_STG0_12_01_802fc38c, (char*)str_STG0_12_02_802fc398,
    (char*)str_STG0_13_802fc3a4, (char*)str_STG0_14_802fc3ac, (char*)str_STG0_15_802fc3b4, (char*)str_STG0_16_802fc3bc,
    (char*)str_STG0_17_802fc3c4, (char*)str_STG0_19_802fc3cc, (char*)str_STG0_END_802fc3d4, (char*)str_STG1_00_802fc3e0,
    (char*)str_STG1_00_01_802fc3e8, (char*)str_STG1_01_802fc3f4, (char*)str_STG1_01a_802fc3fc, (char*)str_STG1_02_802fc408,
    (char*)str_STG1_03_802fc410, (char*)str_STG1_04_802fc418, (char*)str_STG1_05_802fc420, (char*)str_STG1_06_802fc428,
    (char*)str_STG1_06_01_802fc430, (char*)str_STG1_07_802fc43c, (char*)str_STG1_08_802fc444, (char*)str_STG1_09_802fc44c,
    (char*)str_STG1_09_01_802fc454, (char*)str_STG1_09_02_802fc460, (char*)str_STG1_10_802fc46c, (char*)str_STG1_11_802fc474,
    (char*)str_STG1_12_802fc47c, (char*)str_STG1_12_01_802fc484, (char*)str_STG1_12_02_802fc490, (char*)str_STG1_13_802fc49c,
    (char*)str_STG1_13_01_802fc4a4, (char*)str_STG1_13_01_01_802fc4b0, (char*)str_STG1_13_02_802fc4c0, (char*)str_STG1_13_03_802fc4cc,
    (char*)str_STG1_14_802fc4d8, (char*)str_STG1_14_01_802fc4e0, (char*)str_STG1_15_802fc4ec, (char*)str_STG1_15_01_802fc4f4,
    (char*)str_STG1_15_02_802fc500, (char*)str_STG1_15_03_802fc50c, (char*)str_STG1_15_04_802fc518, (char*)str_STG1_16_802fc524,
    (char*)str_STG1_17_802fc52c, (char*)str_STG1_18_802fc534, (char*)str_STG1_18_01_802fc53c, (char*)str_STG1_19_802fc548,
    (char*)str_STG1_19_01_802fc550, (char*)str_STG1_19_02_802fc55c, (char*)str_STG1_20_802fc568, (char*)str_STG1_21_802fc570,
    (char*)str_STG1_30_802fc578, (char*)str_STG1_31_802fc580, (char*)str_STG1_32_802fc588, (char*)str_STG1_33_802fc590,
    (char*)str_STG1_34_802fc598, (char*)str_STG1_35_802fc5a0, (char*)str_STG1_END_802fc5a8, (char*)str_STG2_00_802fc5b4,
    (char*)str_STG2_01_802fc5bc, (char*)str_STG2_02_802fc5c4, (char*)str_STG2_03_802fc5cc, (char*)str_STG2_03_01_802fc5d4,
    (char*)str_STG2_03_02_802fc5e0, (char*)str_STG2_03_03_802fc5ec, (char*)str_STG2_03_04_802fc5f8, (char*)str_STG2_03_05_802fc604,
    (char*)str_STG2_04_802fc610, (char*)str_STG2_05_802fc618, (char*)str_STG2_05_01_802fc620, (char*)str_STG2_05_02_802fc62c,
    (char*)str_STG2_06_802fc638, (char*)str_STG2_07_802fc640, (char*)str_STG2_08_802fc648, (char*)str_STG2_09_802fc650,
    (char*)str_STG2_10_802fc658, (char*)str_STG2_11_802fc660, (char*)str_STG2_12_802fc668, (char*)str_STG2_12_01_802fc670,
    (char*)str_STG2_12_02_802fc67c, (char*)str_STG2_13_802fc688, (char*)str_STG2_14_802fc690, (char*)str_STG2_15_802fc698,
    (char*)str_STG2_16_802fc6a0, (char*)str_STG2_17_802fc6a8, (char*)str_STG2_18_802fc6b0, (char*)str_STG2_19_802fc6b8,
    (char*)str_STG2_19_01_802fc6c0, (char*)str_STG2_20_802fc6cc, (char*)str_STG2_21_802fc6d4, (char*)str_STG2_21_01_802fc6dc,
    (char*)str_STG2_21_02_802fc6e8, (char*)str_STG2_21_03_802fc6f4, (char*)str_STG2_21_04_802fc700, (char*)str_STG2_21_06_802fc70c,
    (char*)str_STG2_22_802fc718, (char*)str_STG2_23_802fc720, (char*)str_STG2_23_01_802fc728, (char*)str_STG2_24_802fc734,
    (char*)str_STG2_25_802fc73c, (char*)str_STG2_25_01_802fc744, (char*)str_STG2_25_02_802fc750, (char*)str_STG2_25_03_802fc75c,
    (char*)str_STG2_25_04_802fc768, (char*)str_STG2_25_05_802fc774, (char*)str_STG2_26_802fc780, (char*)str_STG2_27_802fc788,
    (char*)str_STG2_40_802fc790, (char*)str_STG2_41_802fc798, (char*)str_STG2_44_802fc7a0, (char*)str_STG2_45_802fc7a8,
    (char*)str_STG2_46_802fc7b0, (char*)str_STG2_47_802fc7b8, (char*)str_STG2_END_802fc7c0, (char*)str_STG3_00_802fc7cc,
    (char*)str_STG3_01_802fc7d4, (char*)str_STG3_02_802fc7dc, (char*)str_STG3_03_802fc7e4, (char*)str_STG3_04_802fc7ec,
    (char*)str_STG3_05_802fc7f4, (char*)str_STG3_06_802fc7fc, (char*)str_STG3_07_802fc804, (char*)str_STG3_10_802fc80c,
    (char*)str_STG3_11_802fc814, (char*)str_STG3_12_802fc81c, (char*)str_STG3_20_802fc824, (char*)str_STG3_20_00_802fc82c,
    (char*)str_STG3_20_01_802fc838, (char*)str_STG3_20_02_802fc844, (char*)str_STG3_20_03_802fc850, (char*)str_STG3_20_04_802fc85c,
    (char*)str_STG3_20_05_802fc868, (char*)str_STG3_20_06_802fc874, (char*)str_STG3_20_07_802fc880, (char*)str_STG3_20_07_00_802fc88c,
    (char*)str_STG3_20_08_802fc89c, (char*)str_STG3_20_09_802fc8a8, (char*)str_STG3_20_10_802fc8b4, (char*)str_STG3_20_11_802fc8c0,
    (char*)str_STG3_20_12_802fc8cc, (char*)str_STG3_20_13_802fc8d8, (char*)str_STG3_20_14_802fc8e4, (char*)str_STG3_20_15_802fc8f0,
    (char*)str_STG3_20_16_802fc8fc, (char*)str_STG3_21_802fc908, (char*)str_STG3_30_802fc910, (char*)str_STG3_30_01_802fc918,
    (char*)str_STG3_30_02_802fc924, (char*)str_STG3_30_03_802fc930, (char*)str_STG3_30_04_802fc93c, (char*)str_STG3_31_802fc948,
    (char*)str_STG3_32_802fc950, (char*)str_STG3_33_802fc958, (char*)str_STG3_33_01_802fc960, (char*)str_STG3_33_02_802fc96c,
    (char*)str_STG3_33_03_802fc978, (char*)str_STG3_33_04_802fc984, (char*)str_STG3_33_05_802fc990, (char*)str_STG3_33_06_802fc99c,
    (char*)str_STG3_34_802fc9a8, (char*)str_STG3_35_802fc9b0, (char*)str_STG3_50_802fc9b8, (char*)str_STG3_51_802fc9c0,
    (char*)str_STG3_52_802fc9c8, (char*)str_STG3_END_802fc9d0, (char*)str_STG4_00_802fc9dc, (char*)str_STG4_01_00_802fc9e4,
    (char*)str_STG4_01_802fc9f0, (char*)str_STG4_02_802fc9f8, (char*)str_STG4_02_02_802fca00, (char*)str_STG4_02_03_802fca0c,
    (char*)str_STG4_03_802fca18, (char*)str_STG4_04_802fca20, (char*)str_STG4_04_01_00_802fca28, (char*)str_STG4_04_01_802fca38,
    (char*)str_STG4_05_802fca44, (char*)str_STG4_06_802fca4c, (char*)str_STG4_06_01_802fca54, (char*)str_STG4_06_02_802fca60,
    (char*)str_STG4_07_802fca6c, (char*)str_STG4_08_802fca74, (char*)str_STG4_09_802fca7c, (char*)str_STG4_09_01_802fca84,
    (char*)str_STG4_09_02_802fca90, (char*)str_STG4_09_03_802fca9c, (char*)str_STG4_09_04_802fcaa8, (char*)str_STG4_09_05_802fcab4,
    (char*)str_STG4_10_802fcac0, (char*)str_STG4_10_02_802fcac8, (char*)str_STG4_11_802fcad4, (char*)str_STG4_11_02_802fcadc,
    (char*)str_STG4_12_802fcae8, (char*)str_STG4_12_01_802fcaf0, (char*)str_STG4_13_802fcafc, (char*)str_STG4_13_02_802fcb04,
    (char*)str_STG4_13_03_802fcb10, (char*)str_STG4_13_04_802fcb1c, (char*)str_STG4_14_802fcb28, (char*)str_STG4_15_802fcb30,
    (char*)str_STG4_15_02_802fcb38, (char*)str_STG4_15_03_802fcb44, (char*)str_STG4_15_04_802fcb50, (char*)str_STG4_15_05_802fcb5c,
    (char*)str_STG4_15_06_802fcb68, (char*)str_STG4_15_07_802fcb74, (char*)str_STG4_15_08_802fcb80, (char*)str_STG4_16_802fcb8c,
    (char*)str_STG4_17_802fcb94, (char*)str_STG4_30_802fcb9c, (char*)str_STG4_31_802fcba4, (char*)str_STG4_32_802fcbac,
    (char*)str_STG4_33_802fcbb4, (char*)str_STG4_34_802fcbbc, (char*)str_STG4_35_802fcbc4, (char*)str_STG4_36_802fcbcc,
    (char*)str_STG4_37_802fcbd4, (char*)str_STG4_END_802fcbdc, (char*)str_STG5_00_802fcbe8, (char*)str_STG5_10_802fcbf0,
    (char*)str_STG5_11_802fcbf8, (char*)str_STG5_12_802fcc00, (char*)str_STG5_13_802fcc08, (char*)str_STG5_15_802fcc10,
    (char*)str_STG5_16_802fcc18, (char*)str_STG5_17_802fcc20, (char*)str_STG5_18_802fcc28, (char*)str_STG5_18_01_802fcc30,
    (char*)str_STG5_18_02_802fcc3c, (char*)str_STG5_18_03_802fcc48, (char*)str_STG5_19_802fcc54, (char*)str_STG5_19_01_802fcc5c,
    (char*)str_STG5_19_02_802fcc68, (char*)str_STG5_20_802fcc74, (char*)str_STG5_21_802fcc7c, (char*)str_STG5_21_01_802fcc84,
    (char*)str_STG5_22_802fcc90, (char*)str_STG5_29_802fcc98, (char*)str_STG5_30_802fcca0, (char*)str_STG5_31_802fcca8,
    (char*)str_STG5_32_802fccb0, (char*)str_STG5_33_802fccb8, (char*)str_STG5_34_802fccc0, (char*)str_STG5_35_802fccc8,
    (char*)str_STG5_36_802fccd0, (char*)str_STG5_40_802fccd8, (char*)str_STG5_41_802fcce0, (char*)str_STG5_41_01_802fcce8,
    (char*)str_STG5_42_802fccf4, (char*)str_STG5_43_802fccfc, (char*)str_STG5_44_802fcd04, (char*)str_STG5_45_802fcd0c,
    (char*)str_STG5_46_802fcd14, (char*)str_STG5_47_802fcd1c, (char*)str_STG5_48_802fcd24, (char*)str_STG5_49_802fcd2c,
    (char*)str_STG5_58_802fcd34, (char*)str_STG5_59_802fcd3c, (char*)str_STG5_59_01_802fcd44, (char*)str_STG5_60_802fcd50,
    (char*)str_STG5_61_802fcd58, (char*)str_STG5_62_802fcd60, (char*)str_STG5_63_802fcd68, (char*)str_STG5_64_802fcd70,
    (char*)str_STG5_64_01_802fcd78, (char*)str_STG5_64_02_802fcd84, (char*)str_STG5_64_03_802fcd90, (char*)str_STG5_65_802fcd9c,
    (char*)str_STG5_66_802fcda4, (char*)str_STG5_END_802fcdac, (char*)str_STG6_00_802fcdb8, (char*)str_STG6_01_802fcdc0,
    (char*)str_STG6_02_802fcdc8, (char*)str_STG6_03_802fcdd0, (char*)str_STG6_04_802fcdd8, (char*)str_STG6_05_802fcde0,
    (char*)str_STG6_06_802fcde8, (char*)str_STG6_07_802fcdf0, (char*)str_STG6_08_802fcdf8, (char*)str_STG6_09_802fce00,
    (char*)str_STG6_10_802fce08, (char*)str_STG6_11_802fce10, (char*)str_STG6_12_802fce18, (char*)str_STG6_13_802fce20,
    (char*)str_STG6_14_802fce28, (char*)str_STG6_15_802fce30, (char*)str_STG6_16_802fce38, (char*)str_STG6_17_802fce40,
    (char*)str_STG6_18_802fce48, (char*)str_STG6_19_802fce50, (char*)str_STG6_20_802fce58, (char*)str_STG6_21_802fce60,
    (char*)str_STG6_22_802fce68, (char*)str_STG6_23_802fce70, (char*)str_STG6_24_802fce78, (char*)str_STG6_24_02_802fce80,
    (char*)str_STG6_25_802fce8c, (char*)str_STG6_26_802fce94, (char*)str_STG6_27_802fce9c, (char*)str_STG6_28_802fcea4,
    (char*)str_STG6_29_802fceac, (char*)str_STG6_30_802fceb4, (char*)str_STG6_31_802fcebc, (char*)str_STG6_32_802fcec4,
    (char*)str_STG6_33_802fcecc, (char*)str_STG6_34_802fced4, (char*)str_STG6_35_802fcedc, (char*)str_STG6_36_802fcee4,
    (char*)str_STG6_37_802fceec, (char*)str_STG6_38_802fcef4, (char*)str_STG6_39_802fcefc, (char*)str_STG6_40_802fcf04,
    (char*)str_STG6_41_802fcf0c, (char*)str_STG6_42_802fcf14, (char*)str_STG6_43_802fcf1c, (char*)str_STG6_44_802fcf24,
    (char*)str_STG6_45_802fcf2c, (char*)str_STG6_46_802fcf34, (char*)str_STG6_47_00_802fcf3c, (char*)str_STG6_47_802fcf48,
    (char*)str_STG6_48_802fcf50, (char*)str_STG6_49_00_802fcf58, (char*)str_STG6_49_802fcf64, (char*)str_STG6_50_802fcf6c,
    (char*)str_STG6_51_802fcf74, (char*)str_STG6_52_802fcf7c, (char*)str_STG6_53_802fcf84, (char*)str_STG6_54_802fcf8c,
    (char*)str_STG6_55_802fcf94, (char*)str_STG6_55_01_802fcf9c, (char*)str_STG6_55_02_802fcfa8, (char*)str_STG6_55_03_802fcfb4,
    (char*)str_STG6_55_04_802fcfc0, (char*)str_STG6_56_802fcfcc, (char*)str_STG6_57_802fcfd4, (char*)str_STG6_70_802fcfdc,
    (char*)str_STG6_71_802fcfe4, (char*)str_STG6_71_01_802fcfec, (char*)str_STG6_72_802fcff8, (char*)str_STG6_END_802fd000,
    (char*)str_STG7_00_802fd00c, (char*)str_STG7_01_802fd014, (char*)str_STG7_02_802fd01c, (char*)str_STG7_03_802fd024,
    (char*)str_STG7_04_802fd02c, (char*)str_STG7_05_802fd034, (char*)str_STG7_06_802fd03c, (char*)str_STG7_07_802fd044,
    (char*)str_STG7_07_01_802fd04c, (char*)str_STG7_08_802fd058, (char*)str_STG7_09_802fd060, (char*)str_STG7_10_802fd068,
    (char*)str_STG7_11_802fd070, (char*)str_STG7_12_802fd078, (char*)str_STG7_13_802fd080, (char*)str_STG7_14_802fd088,
    (char*)str_STG7_15_802fd090, (char*)str_STG7_16_802fd098, (char*)str_STG7_17_802fd0a0, (char*)str_STG7_18_802fd0a8,
    (char*)str_STG7_19_802fd0b0, (char*)str_STG7_20_802fd0b8, (char*)str_STG7_25_00_802fd0c0, (char*)str_STG7_25_00_1_802fd0cc,
    (char*)str_STG7_25_802fd0dc, (char*)str_STG7_26_802fd0e4, (char*)str_STG7_50_802fd0ec, (char*)str_STG7_51_802fd0f4,
    (char*)str_STG7_52_802fd0fc, (char*)str_STG7_END_802fd104, (char*)str_STG8_00_802fd110, (char*)str_STG8_01_802fd118,
    (char*)str_STG8_01_01_802fd120, (char*)str_STG8_01_02_802fd12c, (char*)str_STG8_02_802fd138, (char*)str_STG8_03_802fd140,
    (char*)str_STG8_04_802fd148, (char*)str_STG8_05_802fd150, (char*)str_STG8_06_802fd158, (char*)str_STG8_07_802fd160,
    (char*)str_STG8_08_802fd168, (char*)str_STG8_09_802fd170, (char*)str_STG8_10_802fd178, (char*)str_STG8_11_802fd180,
    (char*)str_STG8_12_802fd188, (char*)str_STG8_13_802fd190, (char*)str_STG8_14_802fd198, (char*)str_STG8_14_01_802fd1a0,
    (char*)str_STG8_15_802fd1ac, (char*)str_STG8_16_802fd1b4, (char*)str_STG8_END_802fd1bc, (char*)str_STG9_00_802fd1c8,
    (char*)str_STG9_01_802fd1d0, (char*)str_STG9_02_802fd1d8, (char*)str_STG9_END_802fd1e0
};

s32 johoya_set(void* flags, s32 value, s32 set);
void* _mapAlloc(void* heap, u32 size);
int johoya_data_make(s32 param_1, u32 param_2);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(johoya_data_alloc) {
    JohoyaData* data = &_jdt;
    s32* args = event->args;
    s32 value = evtGetValue(event, args[0]);
    s32 count;
    s32 size;

    data->unk8 = value;
    count = johoya_data_make(value, 0);
    data->count = count;
    size = (count << 4) + 0x40;
    data->entries = _mapAlloc(mapalloc_base_ptr, size);
    memset(data->entries, 0, (data->count << 4) + 0x40);
    data->end = (JohoyaEntry*)((s32)data->entries + (data->count << 4));
    johoya_data_make(data->unk8, 1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 johoya_data_free(void) {
    extern void _mapFree(void* heap, void* ptr);
    JohoyaData* data = &_jdt;

    if (data->entries == NULL) {
        return 2;
    }
    _mapFree(mapalloc_base_ptr, data->entries);
    data->entries = NULL;
    data->count = 0;
    data->unk8 = 0;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(johoya_keti_newjoho_check) {
    JohoyaData* data = &_jdt;
    s32 count = 0;
    s32 offset = 0;
    u8* flags = data->flags;
    s32 i = 0;
    s32* args = event->args;
    s32 total = data->count;
    JohoyaEntry* entries = data->entries;

    while (i < total) {
        s32 entryOffset = offset + 0xC;
        s32 index = *(s16*)((s32)entries + entryOffset);
        s32 mask;
        s32 shift;

        switch (index & 7) {
            case 0:
                mask = 1;
                shift = 0;
                break;
            case 1:
                mask = 2;
                shift = 1;
                break;
            case 2:
                mask = 4;
                shift = 2;
                break;
            case 3:
                mask = 8;
                shift = 3;
                break;
            case 4:
                mask = 0x10;
                shift = 4;
                break;
            case 5:
                mask = 0x20;
                shift = 5;
                break;
            case 6:
                mask = 0x40;
                shift = 6;
                break;
            case 7:
                mask = 0x80;
                shift = 7;
                break;
        }

        if ((s16)((flags[index / 8] & (u8)mask) >> shift) == 0) {
            count++;
        }
        offset += 0x10;
        i++;
    }

    evtSetValue(event, args[0], count);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 johoya_keti_newjoho_makelabel(int param_1) {
    extern u32 strlen(const char* str);
    EventEntry* event = (EventEntry*)param_1;
    s32* args = event->args;
    s32 target = evtGetValue(event, args[0]);
    s32 seen = 0;
    s32 i;
    s32 offset = 0;
    char titleKey[64];
    JohoyaEntry* entries = _jdt.entries;
    u8* flags = _jdt.flags;

    for (i = 0; i < _jdt.count; i++, offset += 0x10) {
        JohoyaEntry* entry = (JohoyaEntry*)((s32)entries + offset);
        s32 index = entry->unkC;
        s32 mask;
        s32 shift;

        switch (index & 7) {
            case 0: mask = 1; shift = 0; break;
            case 1: mask = 2; shift = 1; break;
            case 2: mask = 4; shift = 2; break;
            case 3: mask = 8; shift = 3; break;
            case 4: mask = 0x10; shift = 4; break;
            case 5: mask = 0x20; shift = 5; break;
            case 6: mask = 0x40; shift = 6; break;
            case 7: mask = 0x80; shift = 7; break;
        }

        if ((s16)((flags[index / 8] & (u8)mask) >> shift) == 0) {
            if (target == seen) {
                char* title;
                sprintf((char*)_jdt.end, "%s_%04d", _jdt.unk8, entry->unkC);
                sprintf(titleKey, "jketi_title_%04d", entry->unkC);
                title = msgSearch(titleKey);
                evtSetValue(event, args[1], (s32)_jdt.end);
                evtSetValue(event, args[2], (s32)title);
                evtSetValue(event, args[3], strlen(title));
                evtSetValue(event, args[4], *(s16*)((s32)entry + 0xA));
                _jdt.index = i;
                return 2;
            }
            seen++;
        }
    }

    *(char*)_jdt.end = 0;
    evtSetValue(event, args[1], (s32)_jdt.end);
    evtSetValue(event, args[2], (s32)_jdt.end);
    evtSetValue(event, args[3], 0);
    evtSetValue(event, args[4], 0);
    return 2;
}

s32 johoya_keti_newjoho_setreadflag(void) {
    s32 offset = _jdt.index << 4;
    void* entry = _jdt.entries;

    entry = (void*)((s32)entry + offset);
    johoya_set(_jdt.flags, *(s16*)((s32)entry + 0xC), 1);
    *(u16*)((s32)_jdt.entries + offset) |= 1;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(johoya_keti_oldspeak_check) {
    JohoyaData* data = &_jdt;
    s32 count = 0;
    s32 offset = 0;
    u8* flags = data->flags;
    s32 i = 0;
    s32* args = event->args;
    s32 total = data->count;
    JohoyaEntry* entries = data->entries;

    while (i < total) {
        JohoyaEntry* entry = (JohoyaEntry*)((s32)entries + offset);
        s32 index = entry->unkC;
        s32 mask;
        s32 shift;

        switch (index & 7) {
            case 0:
                mask = 1;
                shift = 0;
                break;
            case 1:
                mask = 2;
                shift = 1;
                break;
            case 2:
                mask = 4;
                shift = 2;
                break;
            case 3:
                mask = 8;
                shift = 3;
                break;
            case 4:
                mask = 0x10;
                shift = 4;
                break;
            case 5:
                mask = 0x20;
                shift = 5;
                break;
            case 6:
                mask = 0x40;
                shift = 6;
                break;
            case 7:
                mask = 0x80;
                shift = 7;
                break;
        }

        if ((s16)((flags[index / 8] & (u8)mask) >> shift) != 0 && (entry->flags & 1) == 0) {
            count++;
        }
        offset += 0x10;
        i++;
    }

    evtSetValue(event, args[0], count);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 johoya_keti_oldspeak_makelabel(int param_1) {
    EventEntry* event = (EventEntry*)param_1;
    s32* args = event->args;
    s32 target = evtGetValue(event, args[0]);
    s32 seen = 0;
    s32 i;
    s32 offset = 0;
    JohoyaEntry* entries = _jdt.entries;
    u8* flags = _jdt.flags;

    for (i = 0; i < _jdt.count; i++, offset += 0x10) {
        JohoyaEntry* entry = (JohoyaEntry*)((s32)entries + offset);
        s32 index = entry->unkC;
        s32 mask;
        s32 shift;

        switch (index & 7) {
            case 0: mask = 1; shift = 0; break;
            case 1: mask = 2; shift = 1; break;
            case 2: mask = 4; shift = 2; break;
            case 3: mask = 8; shift = 3; break;
            case 4: mask = 0x10; shift = 4; break;
            case 5: mask = 0x20; shift = 5; break;
            case 6: mask = 0x40; shift = 6; break;
            case 7: mask = 0x80; shift = 7; break;
        }

        if ((s16)((flags[index / 8] & (u8)mask) >> shift) != 0 && (entry->flags & 1) == 0) {
            if (target == seen) {
                sprintf((char*)_jdt.end, "%s_%04d", _jdt.unk8, entry->unkC);
                evtSetValue(event, args[1], (s32)_jdt.end);
                evtSetValue(event, args[2], *(s32*)((s32)entry + 4));
                evtSetValue(event, args[3], *(s16*)((s32)entry + 8));
                evtSetValue(event, args[4], 0);
                _jdt.index = i;
                return 2;
            }
            seen++;
        }
    }

    *(char*)_jdt.end = 0;
    evtSetValue(event, args[1], (s32)_jdt.end);
    evtSetValue(event, args[2], (s32)_jdt.end);
    evtSetValue(event, args[3], 0);
    evtSetValue(event, args[4], 0);
    return 2;
}

s32 johoya_keti_oldspeak_setreadflag(void) {
    _jdt.entries[_jdt.index].flags |= 1;
    return 2;
}

s32 johoya_perfect_newjoho_setreadflag(void) {
    johoya_set(_jdt.flags + 0xD, _jdt.entries[_jdt.index].unkC, 1);
    return 2;
}

s32 johoya_luigi_newjoho_setreadflag(void) {
    johoya_set(_jdt.flags + 0x1A, _jdt.entries[_jdt.index].unkC, 1);
    return 2;
}


int johoya_data_make(s32 param_1, u32 param_2) {
    extern s32 _ismbblead(s32 c);
    JohoyaData* data = &_jdt;
    char key[64];
    char tag[64];
    s32 pos;
    s32 maxLabel;
    s32 index;
    s32 tagNum;
    s32 done;
    s32 entryIndex;
    s32 body;
    s16 bodyLen;
    char* scan;
    s32 count;
    s32 labelIndex;
    char** table;

    maxLabel = evtGetValue(NULL, GSW(0));
    count = 0;
    index = 0;
    entryIndex = 0;
    while (1) {
        sprintf(key, "%s_%04d", param_1, index);
        scan = msgSearch(key);
        if (scan[0] != '<') {
            break;
        }
        scan += 2;
        if (scan[-1] != '!') {
            break;
        }

        tagNum = 0;
        pos = 0;
        done = 0;
        while (!done) {
            if (_ismbblead(*scan)) {
                if (tagNum == 0) {
                    tag[pos++] = scan[0];
                    tag[pos++] = scan[1];
                    scan += 2;
                } else {
                    pos += 2;
                    scan += 2;
                }
            } else {
                switch (scan[0]) {
                    case '>':
                        done = 1;
                    case ',':
                        scan++;
                        if (tagNum == 0) {
                            tag[pos] = '\0';
                            body = (s32)scan;
                        } else {
                            bodyLen = (s16)pos;
                        }
                        pos = 0;
                        tagNum++;
                        break;
                    default:
                        if (tagNum == 0) {
                            tag[pos++] = *scan++;
                        } else {
                            pos++;
                            scan++;
                        }
                }
            }
        }

        if (!strcmp(tag, "enddata")) {
            break;
        }
        if (strcmp(tag, "")) {
            labelIndex = 0;
            table = evtNoLabel;
            while (labelIndex < 0x197) {
                if (strcmp(tag, *table) == 0) {
                    break;
                }
                labelIndex++;
                table++;
            }
            if (labelIndex >= 0x197) {
                labelIndex = -1;
            }
            if (labelIndex != -1 && labelIndex <= maxLabel) {
                if (param_2 & 1) {
                    *(s16*)((s32)data->entries + entryIndex + 2) = (s16)labelIndex;
                    *(s32*)((s32)data->entries + entryIndex + 4) = body;
                    *(s16*)((s32)data->entries + entryIndex + 8) = bodyLen;
                    *(s16*)((s32)data->entries + entryIndex + 0xA) = 5;
                    *(s16*)((s32)data->entries + entryIndex + 0xC) = (s16)index;
                }
                entryIndex += 0x10;
                count++;
            }
        }
        index++;
    }
    return count;
}

void johoya_init(void) {
    u8* flags = _jdt.flags;

    memset(flags, 0, 0xD);
    memset(flags + 0xD, 0, 0xD);
    memset(flags + 0x1A, 0, 0xD);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 johoya_set(void* flags, s32 value, s32 set) {
    s32 index = (s16)value;
    s32 mask;
    s32 shift;
    s32 byteIndex;
    u8 cleared;
    u8 shifted;

    switch (index & 7) {
        case 0:
            mask = 0xFE;
            shift = 0;
            break;
        case 1:
            mask = 0xFD;
            shift = 1;
            break;
        case 2:
            mask = 0xFB;
            shift = 2;
            break;
        case 3:
            mask = 0xF7;
            shift = 3;
            break;
        case 4:
            mask = 0xEF;
            shift = 4;
            break;
        case 5:
            mask = 0xDF;
            shift = 5;
            break;
        case 6:
            mask = 0xBF;
            shift = 6;
            break;
        case 7:
            mask = 0x7F;
            shift = 7;
            break;
    }

    byteIndex = index / 8;
    if ((s16)set < 0) {
        set = 0;
    } else if ((s16)set > 1) {
        set = 1;
    }

    cleared = (u8)(((u8*)flags)[byteIndex] & mask);
    shifted = (u8)(((s16)set) << shift);
    ((u8*)flags)[byteIndex] = cleared + shifted;
    return set;
}
s32 johoya_get(void* flags, s16 value) {
    s32 index = value;
    s32 mask;
    s32 shift;

    switch (index & 7) {
        case 0:
            mask = 1;
            shift = 0;
            break;
        case 1:
            mask = 2;
            shift = 1;
            break;
        case 2:
            mask = 4;
            shift = 2;
            break;
        case 3:
            mask = 8;
            shift = 3;
            break;
        case 4:
            mask = 0x10;
            shift = 4;
            break;
        case 5:
            mask = 0x20;
            shift = 5;
            break;
        case 6:
            mask = 0x40;
            shift = 6;
            break;
        case 7:
            mask = 0x80;
            shift = 7;
            break;
    }

    return (s16)((((u8*)flags)[index / 8] & (u8)mask) >> shift);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
int search_evt_no(char* label) {
    char** table = evtNoLabel;
    s32 i;

    for (i = 0; i < 0x197; i++, table++) {
        if (strcmp(label, *table) == 0) {
            return i;
        }
    }
    return -1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


char* search_evt_moji(s32 index) {
    return evtNoLabel[index];
}
