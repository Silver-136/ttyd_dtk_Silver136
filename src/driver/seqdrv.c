/* seqdrv: sequence driver. Switches between the top-level game sequences (SequenceType: logo, title, game, map
 * change, battle, game over, load, E3 demo); seq_data holds each sequence's Init, Main and Exit callbacks.
 */
#include "driver/seqdrv.h"
#include "system.h"
#include <string.h> //memset prototype

extern SequenceCallback seq_data[8][3];

//.sdata
SequenceType now_seq = SEQ_UNDEFINED;
SequenceType next_seq = SEQ_UNDEFINED;
SequenceType prev_seq = SEQ_UNDEFINED;

//.sbss
SequenceWork seqWork;
const char* next_p1;
const char* next_p0;

/**
 * Clears the sequence work and marks the current, next and previous sequences undefined;
 * called once at boot from mariost.c before the first seqMain.
 */
void seqInit_MARIOSTORY(void) {
    memset(&seqWork, 0, sizeof(SequenceWork));
    now_seq = SEQ_UNDEFINED;
    next_seq = SEQ_UNDEFINED;
    prev_seq = SEQ_UNDEFINED;
}

/**
 * Runs once per frame from the main loop: when a new sequence was requested it waits for drawing to finish,
 * calls the old sequence's exit callback, resets seqWork with the requested map and entrance (bero) names
 * and calls the new one's init; then runs the current sequence's main callback.
 */
void seqMain(void) {
    if (next_seq != now_seq) {
        if (now_seq != SEQ_UNDEFINED) {
            sysWaitDrawSync();
            seq_data[now_seq][2](&seqWork); //call Exit function
        }
        prev_seq = now_seq; //store previous
        memset(&seqWork, 0, sizeof(SequenceWork));
        now_seq = next_seq;
        seqWork.sequence = next_seq;
        seqWork.state = 0;
        seqWork.mapName = next_p0;
        seqWork.beroName = next_p1;
        seq_data[next_seq][0](&seqWork); //call Init function
    }
    seq_data[now_seq][1](&seqWork); //call Main function
}

/**
 * Requests a switch to sequence seq (game, battle, game over, ...) with a map name
 * and entrance (bero) name for the new sequence; the switch happens at the next seqMain.
 */
void seqSetSeq(SequenceType seq, const char* map, const char* bero) {
    next_seq = seq;
    next_p0 = map;
    next_p1 = bero;
}

/** Returns the sequence running now (e.g. SEQ_GAME while Mario walks around a map, SEQ_BATTLE in a fight). */
SequenceType seqGetSeq(void) {
    return now_seq;
}

/**
 * Returns the sequence that ran before the current one;
 * the title and map-change sequences use it to tell a fresh load from a return.
 */
SequenceType seqGetPrevSeq(void) {
    return prev_seq;
}

/**
 * Returns the requested next sequence (equal to the current one when no switch is pending);
 * seq_battle checks it for a pending game over.
 */
SequenceType seqGetNextSeq(void) {
    return next_seq;
}

/** Returns TRUE while a sequence switch is pending (requested by seqSetSeq but not yet applied). */
BOOL seqCheckSeq(void) {
    return now_seq != next_seq;
}
