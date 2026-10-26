#pragma once

#include <dolphin/types.h>
#include "evt/evt_word.h"
#include "manager/evtmgr.h"

// Script Helpers
#define NUMARGS(...) PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, N, ...) N
#define PP_RSEQ_N() 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define PTR(value) (EvtWord)(value)
#define STRING(value) PTR(value)
#define FLOAT(value) ((s32)(1024.0f * (value)) - EVTDAT_FLOAT_BASE)

#define EVT_CMD(param_count, opcode) (((param_count) << 16) | (opcode))

#define EVT_DEF(name) EvtWord(name)[]

#define EVT_BEGIN(name) EvtWord(name)[] = {

#define EVT_END()     \
    OPCODE_END_SCRIPT \
    }                 \
    ;

// EVT_END plus one trailing zero pad word
#define EVT_END_PAD() \
    OPCODE_END_SCRIPT, \
    0                  \
    }                  \
    ;

// EVT_END plus four trailing zero pad words
#define EVT_END_PAD4() \
    OPCODE_END_SCRIPT, \
    0, 0, 0, 0         \
    }                  \
    ;

// Used to define native C functions that scripts can call
#define USER_FUNC(function) s32(function)(EventEntry * event, BOOL isFirstCall)

// Termination
// #define END EVT_CMD(0, OPCODE_END_SCRIPT),
#define END
#define RETURN EVT_CMD(0, OPCODE_END_EVT),

// Labels / Jumps
#define LABEL(LABEL_ID) EVT_CMD(1, OPCODE_LBL), LABEL_ID,
#define GOTO(LABEL_ID) EVT_CMD(1, OPCODE_GOTO), LABEL_ID,

// Loops
#define LOOP(LOOP_COUNT) EVT_CMD(1, OPCODE_DO), LOOP_COUNT,
#define END_LOOP EVT_CMD(0, OPCODE_WHILE),
#define BREAK_LOOP EVT_CMD(0, OPCODE_DO_BREAK),
#define CONTINUE_LOOP EVT_CMD(0, OPCODE_DO_CONTINUE),

// Debug output (left in retail scripts)
#define DEBUG_PUT_MSG(msg) EVT_CMD(1, OPCODE_DEBUG_PUT_MSG), msg,
#define DEBUG_MSG_CLEAR EVT_CMD(0, OPCODE_DEBUG_MSG_CLEAR),
#define DEBUG_PUT_REG(reg) EVT_CMD(1, OPCODE_DEBUG_PUT_REG), reg,
#define DEBUG_NAME(name) EVT_CMD(1, OPCODE_DEBUG_NAME), name,
#define DEBUG_REM(msg) EVT_CMD(1, OPCODE_DEBUG_REM), msg,
#define DEBUG_BP EVT_CMD(0, OPCODE_DEBUG_BP),

// Waiting
#define WAIT_FRAMES(FRAMES) EVT_CMD(1, OPCODE_WAIT_FRM), FRAMES,
#define WAIT_MS(MILLISECONDS) EVT_CMD(1, OPCODE_WAIT_MSEC), MILLISECONDS,
// Blocks the current script as long as (expr) is true.
#define WAIT_WHILE(expr) EVT_CMD(1, OPCODE_HALT), expr,

// Conditionals
#define IF_STR_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IF_STR_EQUAL), var1, var2,
#define IF_STR_NOT_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IF_STR_NOT_EQUAL), var1, var2,
#define IF_STR_LESS(var1, var2) EVT_CMD(2, OPCODE_IF_STR_SMALL), var1, var2,
#define IF_STR_GREATER(var1, var2) EVT_CMD(2, OPCODE_IF_STR_LARGE), var1, var2,
#define IF_STR_LESS_OR_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IF_STR_SMALL_EQUAL), var1, var2,
#define IF_STR_GREATER_OR_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IF_STR_LARGE_EQUAL), var1, var2,

#define IF_FLOAT_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IFF_EQUAL), var1, var2,
#define IF_FLOAT_NOT_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IFF_NOT_EQUAL), var1, var2,
#define IF_FLOAT_LESS(var1, var2) EVT_CMD(2, OPCODE_IFF_SMALL), var1, var2,
#define IF_FLOAT_GREATER(var1, var2) EVT_CMD(2, OPCODE_IFF_LARGE), var1, var2,
#define IF_FLOAT_LESS_OR_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IFF_SMALL_EQUAL), var1, var2,
#define IF_FLOAT_GREATER_OR_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IFF_LARGE_EQUAL), var1, var2,

#define IF_INT_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IF_EQUAL), var1, var2,
#define IF_INT_NOT_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IF_NOT_EQUAL), var1, var2,
#define IF_INT_LESS(var1, var2) EVT_CMD(2, OPCODE_IF_SMALL), var1, var2,
#define IF_INT_GREATER(var1, var2) EVT_CMD(2, OPCODE_IF_LARGE), var1, var2,
#define IF_INT_LESS_OR_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IF_SMALL_EQUAL), var1, var2,
#define IF_INT_GREATER_OR_EQUAL(var1, var2) EVT_CMD(2, OPCODE_IF_LARGE_EQUAL), var1, var2,

#define IF_BITS_SET(var1, var2) EVT_CMD(2, OPCODE_IF_FLAG), var1, var2,
#define IF_BITS_CLEAR(var1, var2) EVT_CMD(2, OPCODE_IF_NOT_FLAG), var1, var2,

#define ELSE EVT_CMD(0, OPCODE_ELSE),
#define END_IF EVT_CMD(0, OPCODE_END_IF),

// Switch Statements
#define SWITCH(expr) EVT_CMD(1, OPCODE_SWITCH), expr,
#define SWITCH_RAW(expr) EVT_CMD(1, OPCODE_SWITCHI), expr,
#define CASE_EQUAL(expr) EVT_CMD(1, OPCODE_CASE_EQUAL), expr,
#define CASE_NOT_EQUAL(expr) EVT_CMD(1, OPCODE_CASE_NOT_EQUAL), expr,
#define CASE_LESS(expr) EVT_CMD(1, OPCODE_CASE_SMALL), expr,
#define CASE_GREATER(expr) EVT_CMD(1, OPCODE_CASE_LARGE), expr,
#define CASE_LESS_OR_EQUAL(expr) EVT_CMD(1, OPCODE_CASE_SMALL_EQUAL), expr,
#define CASE_GREATER_OR_EQUAL(expr) EVT_CMD(1, OPCODE_CASE_LARGE_EQUAL), expr,
#define CASE_DEFAULT EVT_CMD(0, OPCODE_CASE_ETC),
#define CASE_OR_EQUAL(val) EVT_CMD(1, OPCODE_CASE_OR), val,
#define CASE_AND_EQUAL(val) EVT_CMD(1, OPCODE_CASE_AND), val,
#define CASE_FLAG(val) EVT_CMD(1, OPCODE_CASE_FLAG), val,
#define CASE_END EVT_CMD(0, OPCODE_CASE_END),
#define CASE_BETWEEN(LOWER, UPPER) EVT_CMD(2, OPCODE_CASE_BETWEEN), LOWER, UPPER,
#define BREAK_SWITCH EVT_CMD(0, OPCODE_SWITCH_BREAK),
#define END_SWITCH EVT_CMD(0, OPCODE_END_SWITCH),

// Assignment
#define SET_VALUE(target, expr) EVT_CMD(2, OPCODE_SET), target, expr,
#define SET_RAW_VALUE(target, literal) EVT_CMD(2, OPCODE_SETI), target, literal,
#define SET_FLOAT_VALUE(target, expr) EVT_CMD(2, OPCODE_SETF), target, expr,

// Arithmetic
#define ADD(target, expr) EVT_CMD(2, OPCODE_ADD), target, expr,
#define SUBTRACT(target, expr) EVT_CMD(2, OPCODE_SUB), target, expr,
#define MULTIPLY(target, expr) EVT_CMD(2, OPCODE_MUL), target, expr,
#define DIVIDE(target, expr) EVT_CMD(2, OPCODE_DIV), target, expr,
#define MODULO(target, expr) EVT_CMD(2, OPCODE_MOD), target, expr,

#define ADD_FLOAT(target, expr) EVT_CMD(2, OPCODE_ADDF), target, expr,
#define SUBTRACT_FLOAT(target, expr) EVT_CMD(2, OPCODE_SUBF), target, expr,
#define MULTIPLY_FLOAT(target, expr) EVT_CMD(2, OPCODE_MULF), target, expr,
#define DIVIDE_FLOAT(target, expr) EVT_CMD(2, OPCODE_DIVF), target, expr,

// Sequential Reads
#define SET_INT_READ_ADDR(ptr) EVT_CMD(1, OPCODE_SET_READ), ptr,
#define READ_INT(dest) EVT_CMD(1, OPCODE_READ), dest,
#define READ_INT2(dest1, dest2) EVT_CMD(2, OPCODE_READ2), dest1, dest2,
#define READ_INT3(dest1, dest2, dest3) EVT_CMD(3, OPCODE_READ3), dest1, dest2, dest3,
#define READ_INT4(dest1, dest2, dest3, dest4) EVT_CMD(4, OPCODE_READ4), dest1, dest2, dest3, dest4,
#define READ_INT_INDEXED(dest, offset) EVT_CMD(2, OPCODE_READ_N), dest, offset,

#define SET_FLOAT_READ_ADDR(ptr) EVT_CMD(1, OPCODE_SET_READF), ptr,
#define READ_FLOAT(dest) EVT_CMD(1, OPCODE_READF), dest,
#define READ_FLOAT2(dest1, dest2) EVT_CMD(2, OPCODE_READF2), dest1, dest2,
#define READ_FLOAT3(dest1, dest2, dest3) EVT_CMD(3, OPCODE_READF3), dest1, dest2, dest3,
#define READ_FLOAT4(dest1, dest2, dest3, dest4) EVT_CMD(4, OPCODE_READF4), dest1, dest2, dest3, dest4,
#define READ_FLOAT_INDEXED(dest, offset) EVT_CMD(2, OPCODE_READF_N), dest, offset,

// User Work / User Flag Base
#define SET_UW_BASE(expr) EVT_CMD(1, OPCODE_SET_USER_WRK), expr,
#define SET_UF_BASE(expr) EVT_CMD(1, OPCODE_SET_USER_FLG), expr,
#define ALLOCATE_UW(count, destVar) EVT_CMD(2, OPCODE_ALLOC_USER_WRK), count, destVar,

// Bitwise Operations
#define AND(target, val) EVT_CMD(2, OPCODE_AND), target, val,
#define AND_RAW(target, rawVal) EVT_CMD(2, OPCODE_ANDI), target, rawVal,
#define OR(target, val) EVT_CMD(2, OPCODE_OR), target, val,
#define OR_RAW(target, rawVal) EVT_CMD(2, OPCODE_ORI), target, rawVal,

// Time Conversion
#define CONVERT_MS_TO_FRAME(dest, ms) EVT_CMD(2, OPCODE_SET_FRAME_FROM_MSEC), dest, ms,
#define CONVERT_FRAME_TO_MS(dest, frameCount) EVT_CMD(2, OPCODE_SET_MSEC_FROM_FRAME), dest, frameCount,

// Raw Memory Access
#define POKE_INT(value, address) EVT_CMD(2, OPCODE_SET_RAM), value, address,
#define POKE_FLOAT(value, address) EVT_CMD(2, OPCODE_SET_RAMF), value, address,
#define PEEK_INT(destVar, address) EVT_CMD(2, OPCODE_GET_RAM), destVar, address,
#define PEEK_FLOAT(destVar, address) EVT_CMD(2, OPCODE_GET_RAMF), destVar, address,

// Indirect Variable Access
#define SET_INT_INDEXED(indexVar, expr) EVT_CMD(2, OPCODE_SETR), indexVar, expr,
#define SET_FLOAT_INDEXED(indexVar, expr) EVT_CMD(2, OPCODE_SETRF), indexVar, expr,
#define GET_INT_INDEXED(indexVar, destVar) EVT_CMD(2, OPCODE_GETR), indexVar, destVar,
#define GET_FLOAT_INDEXED(indexVar, destVar) EVT_CMD(2, OPCODE_GETRF), indexVar, destVar,

// Native Calls
#define CALL(...) EVT_CMD(NUMARGS((EvtWord)__VA_ARGS__), OPCODE_USER_FUNC), (EvtWord)__VA_ARGS__,

// Spawning / Controlling Events
#define SPAWN_EVENT(script) EVT_CMD(1, OPCODE_RUN_EVT), PTR(script),
#define SPAWN_EVENT_GET_ID(script, idOut) EVT_CMD(2, OPCODE_RUN_EVT_ID), script, idOut,
// Blocks until the child finished
#define CALL_CHILD(script) EVT_CMD(1, OPCODE_RUN_CHILD_EVT), PTR(script),
#define DELETE_EVENT(tid) EVT_CMD(1, OPCODE_DELETE_EVT), tid,
#define RESTART_EVENT(script) EVT_CMD(1, OPCODE_RESTART_EVT), script,

#define SET_PRIORITY(priority) EVT_CMD(1, OPCODE_SET_PRI), priority,
#define SET_SPEED(timeScale) EVT_CMD(1, OPCODE_SET_SPD), timeScale,
#define SET_TYPE(type) EVT_CMD(1, OPCODE_SET_TYPE), type,

#define SUSPEND_TYPES(types) EVT_CMD(1, OPCODE_STOP_ALL), types,
#define RESUME_TYPES(types) EVT_CMD(1, OPCODE_START_ALL), types,
#define SUSPEND_TYPES_OTHER(types) EVT_CMD(1, OPCODE_STOP_OTHER), types,
#define RESUME_TYPES_OTHER(types) EVT_CMD(1, OPCODE_START_OTHER), types,
#define SUSPEND_EVENT(tid) EVT_CMD(1, OPCODE_STOP_ID), tid,
#define RESUME_EVENT(tid) EVT_CMD(1, OPCODE_START_ID), tid,
#define IS_EVENT_RUNNING(tid, resultOut) EVT_CMD(2, OPCODE_CHK_EVT), tid, resultOut,

// Inline-block Events
#define BEGIN_INLINE_EVENT EVT_CMD(0, OPCODE_INLINE_EVT),
#define BEGIN_INLINE_EVENT_GET_ID(idOut) EVT_CMD(1, OPCODE_INLINE_EVT_ID), idOut,
#define END_INLINE_EVENT EVT_CMD(0, OPCODE_END_INLINE),
// Siblings are tied to the current script's lifetime
#define BEGIN_SIBLING_EVENT EVT_CMD(0, OPCODE_BROTHER_EVT),
#define BEGIN_SIBLING_EVENT_GET_ID(idOut) EVT_CMD(1, OPCODE_BROTHER_EVT_ID), idOut,
#define END_SIBLING_EVENT EVT_CMD(0, OPCODE_END_BROTHER),

// Data types
#define EVTDAT_LW_MAX (-20 * 1000000)
#define EVTDAT_GW_MAX (-40 * 1000000)
#define EVTDAT_LF_MAX (-60 * 1000000)
#define EVTDAT_GF_MAX (-80 * 1000000)
#define EVTDAT_LSWF_MAX (-100 * 1000000)
#define EVTDAT_GSWF_MAX (-120 * 1000000)
#define EVTDAT_LSW_MAX (-140 * 1000000)
#define EVTDAT_GSW_MAX (-160 * 1000000)
#define EVTDAT_UW_MAX (-180 * 1000000)
#define EVTDAT_UF_MAX (-200 * 1000000)
#define EVTDAT_FLOAT_MAX (-220 * 1000000)
#define EVTDAT_POINTER_MAX (-250 * 1000000)
// "no variable": an argument slot the command neither reads nor writes
#define EVT_NULLPTR EVTDAT_POINTER_MAX
#define EVTDAT_ADDR_MAX (-270 * 1000000)

#define EVTDAT_LW_BASE (30 * 1000000)
#define EVTDAT_GW_BASE (50 * 1000000)
#define EVTDAT_LF_BASE (70 * 1000000)
#define EVTDAT_GF_BASE (90 * 1000000)
#define EVTDAT_LSWF_BASE (110 * 1000000)
#define EVTDAT_GSWF_BASE (130 * 1000000)
#define EVTDAT_LSW_BASE (150 * 1000000)
#define EVTDAT_GSW_BASE (170 * 1000000)
#define EVTDAT_UW_BASE (190 * 1000000)
#define EVTDAT_UF_BASE (210 * 1000000)
#define EVTDAT_FLOAT_BASE (230 * 1000000)

#define LW(id) ((id) - EVTDAT_LW_BASE)
#define GW(id) ((id) - EVTDAT_GW_BASE)
#define LF(id) ((id) - EVTDAT_LF_BASE)
#define GF(id) ((id) - EVTDAT_GF_BASE)
#define LSWF(id) ((id) - EVTDAT_LSWF_BASE)
#define GSWF(id) ((id) - EVTDAT_GSWF_BASE)
#define LSW(id) ((id) - EVTDAT_LSW_BASE)
#define GSW(id) ((id) - EVTDAT_GSW_BASE)
#define UW(id) ((id) - EVTDAT_UW_BASE)
#define UF(id) ((id) - EVTDAT_UF_BASE)

/* Saved variables with a known meaning */
#define GSW_STORY_PROGRESS GSW(0)             /* how far the story has advanced; GSW 0 alone is stored as s32 */
#define GSW_GLITZ_PIT_OPPONENT GSW(501)       /* Glitz Pit: the opponent of the current match */
#define GSW_GLITZ_PIT_CONDITION GSW(502)      /* Glitz Pit: the special condition of the current match */
#define GSW_GLITZ_PIT_RANK GSW(580)           /* Glitz Pit: Mario's saved rank */
#define GSW_PIT_FLOOR GSW(1321)               /* Pit of 100 Trials: floor index 0..99 (shown as 1..100) */
#define GSWF_SHOP_TUTORIAL_SEEN GSWF(0)
#define GSWF_SAVE_BLOCK_TUTORIAL_SEEN GSWF(233)
#define GSWF_ITEM_TUTORIAL_SEEN GSWF(235)
#define GSWF_GLITZ_PIT_MATCH_RESERVED GSWF(2388)
#define GSWF_GLITZ_PIT_WON_LAST_MATCH GSWF(2443)

/* Trouble Center (gor_irai.c). Trouble ids are the game's own: each reward script prints irai_NN messages and sets
 * GSWF_TROUBLE_DONE(NN). Names are the US trouble titles. */
typedef enum TroubleId {
    TROUBLE_NONE = 0,
    TROUBLE_NEED_A_KEY = 1,              /* Garf, gor_02 */
    TROUBLE_SAFE_DELIVERY = 2,           /* McGoomba, gor_01 */
    TROUBLE_PRICE_ADJUSTMENT = 3,        /* Arfur, gor_02 */
    TROUBLE_FIND_THIS_GUY = 4,           /* Goomther, gor_01 */
    TROUBLE_HIT_ME_PLEASE = 5,           /* Mousimilian, gor_01 */
    TROUBLE_IM_HUNGRY = 6,               /* Bomberto, gor_00 */
    TROUBLE_TRY_TO_FIND_ME = 7,          /* Koopook, gon_08 */
    TROUBLE_LISTEN_TO_ME = 8,            /* Mayor Kroop, nok_01 */
    TROUBLE_ORDER_ME_AN_ITEM = 9,        /* Plenn T., gor_01 */
    TROUBLE_EMERGENCY_SHROOM = 10,       /* Puni Elder, mri_01 */
    TROUBLE_PLAY_WITH_ME = 11,           /* Lahla, gor_03 */
    TROUBLE_HELP_MY_DADDY = 12,          /* Pine T. Jr., tik_06 */
    TROUBLE_HELP_WANTED = 13,            /* Jolene, tou_05 */
    TROUBLE_HEARTFUL_CAKE_RECIPE = 14,   /* Merlee, tik_12 */
    TROUBLE_THE_FOOD_I_WANT = 15,        /* Bub-ulber, nok_00 */
    TROUBLE_ELUSIVE_BADGE = 16,          /* Ms. Mowz, gor_01 */
    TROUBLE_NEWSLETTER = 17,             /* Mayor Dour, usu_00 */
    TROUBLE_SEEKING_LEGENDARY_BOOK = 18, /* Zess T., gor_01 (done = Mario has the Cookbook) */
    TROUBLE_IMPORTANT_THING = 19,        /* Frankie, gor_03 */
    TROUBLE_TELL_THAT_PERSON = 20,       /* Eve, usu_01 */
    TROUBLE_LOOKING_FOR_A_GAL = 21,      /* Goom Goom, dou_04 */
    TROUBLE_GET_THESE_INGREDIENTS = 22,  /* Chef Shimi, rsh_03 */
    TROUBLE_I_MUST_HAVE_THAT_BOOK = 23,  /* Toodles, pik_00 */
    TROUBLE_SECURITY_CODE = 24,          /* Businessman, tou_01 */
    TROUBLE_DELIVERY_PLEASE = 25,        /* Goldbob, pik_00 */
    TROUBLE_I_CANT_SPEAK = 26,           /* Gob, bom_02 */
    TROUBLE_I_WANNA_MEET_LUIGI = 27,     /* Toadia, pik_01 */
    TROUBLE_ROUST_THESE_CADS = 28,       /* Doe T., win_00 */
    TROUBLE_HELP_ME_MAKE_UP = 29,        /* Bub, pik_01 */
    TROUBLE_ERASE_THAT_GRAFFITI = 30,    /* Swob, bom_02 */
} TroubleId;
#define GSW_TROUBLE_ACCEPTED GSW(1420)              /* TroubleId of the accepted trouble, TROUBLE_NONE if none */
#define GSWF_TROUBLE_BOARD_EXPLAINED GSWF(5374)     /* the board's first explanation (irai_00..03) was given */
#define GSWF_TROUBLE_DONE(id) GSWF(5374 + (id))     /* trouble id solved */
#define GSWF_TROUBLE_WORK(n) GSWF(5405 + (n))       /* n = 0..16: progress flags of the accepted trouble */
#define GSW_TROUBLE_WORK(n) GSW(1469 + (n))         /* n = 0..5: progress values of the accepted trouble */
/* the same as save-work indices for swGet / swByteGet in C */
#define SW_TROUBLE_ACCEPTED 1420
#define SWF_TROUBLE_DONE(id) ((id) + 5374)

#define EVT_RETURN_BLOCK 0
#define EVT_RETURN_YIELD 1
#define EVT_RETURN_DONE 2
#define EVT_RETURN_REPEAT 3
#define EVT_RETURN_FINISH 255
