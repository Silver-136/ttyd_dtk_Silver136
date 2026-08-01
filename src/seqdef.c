#include "seqdef.h"

typedef void (*SequenceCallback)(void);

typedef struct SequenceDefinition {
    SequenceCallback init;
    SequenceCallback main;
    SequenceCallback exit;
} SequenceDefinition;

#define DECLARE_SEQUENCE(name)       \
    extern void seq_##name##Init(void); \
    extern void seq_##name##Main(void); \
    extern void seq_##name##Exit(void)

DECLARE_SEQUENCE(logo);
DECLARE_SEQUENCE(title);
DECLARE_SEQUENCE(game);
DECLARE_SEQUENCE(mapChange);
DECLARE_SEQUENCE(battle);
DECLARE_SEQUENCE(gameOver);
DECLARE_SEQUENCE(load);
DECLARE_SEQUENCE(e3);

#undef DECLARE_SEQUENCE

#define SEQUENCE(name) \
    { seq_##name##Init, seq_##name##Main, seq_##name##Exit }

SequenceDefinition seq_data_80304400[] = {
    SEQUENCE(logo),
    SEQUENCE(title),
    SEQUENCE(game),
    SEQUENCE(mapChange),
    SEQUENCE(battle),
    SEQUENCE(gameOver),
    SEQUENCE(load),
    SEQUENCE(e3),
};

#undef SEQUENCE
