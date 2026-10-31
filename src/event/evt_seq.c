#include "evt/evt_seq.h"
#include "driver/seqdrv.h"
#include "evt/evt_word.h"
#include "manager/evtmgr_cmd.h"

/**
 * Switches the game to sequence args[0] (field, battle, game over, map change, ...) with map name args[1]
 * and entrance (bero) name args[2]; blocks so the calling script stops while the sequence changes.
 */
USER_FUNC(evt_seq_set_seq) {
    EvtWord* args = event->args;
    SequenceType type;
    const char *map, *bero;

    type = evtGetValue(event, args[0]);
    map = (const char*)evtGetValue(event, args[1]);
    bero = (const char*)evtGetValue(event, args[2]);
    seqSetSeq(type, map, bero);
    return EVT_RETURN_BLOCK;
}

/** Blocks the script until the current game sequence is args[0]. */
USER_FUNC(evt_seq_wait) {
    SequenceType current, target;

    target = evtGetValue(event, *event->args);
    current = seqGetSeq();
    return (current != target) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}
