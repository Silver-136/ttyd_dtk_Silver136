#include "driver/seqdrv.h"
#include "evt/evt_snd.h"
#include "seq/seq_logo.h"
#include "seq/seq_title.h"
#include "seq/seq_game.h"
#include "seq/seq_mapchange.h"
#include "seq/seq_gameover.h"
#include "seq/seq_load.h"
#include "seq/seq_e3.h"

extern void seq_battleInit(SequenceWork* work);
extern void seq_battleMain(SequenceWork* work);
extern void seq_battleExit(SequenceWork* work);

SequenceCallback seq_data[8][3] = {
    {seq_logoInit, seq_logoMain, seq_logoExit},
    {seq_titleInit, seq_titleMain, seq_titleExit},
    {seq_gameInit, seq_gameMain, seq_gameExit},
    {seq_mapChangeInit, seq_mapChangeMain, seq_mapChangeExit},
    {seq_battleInit, seq_battleMain, seq_battleExit},
    {seq_gameOverInit, seq_gameOverMain, seq_gameOverExit},
    {seq_loadInit, seq_loadMain, seq_loadExit},
    {seq_e3Init, seq_e3Main, seq_e3Exit},
};
