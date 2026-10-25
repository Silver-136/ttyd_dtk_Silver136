#ifndef _ASSERT_H_
#define _ASSERT_H_

#if __STDC_VERSION__ >= 201112L
// The C11 way
#define static_assert(cond, msg) _Static_assert(cond, #msg)
#else
// Before C11: a false condition gives an array of negative size, which does not compile
#define static_assert(cond, msg) typedef char static_assertion_##msg[(cond)?1:-1]
#endif

#endif
