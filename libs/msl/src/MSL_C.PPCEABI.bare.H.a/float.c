/*
 * The JP demo map lists seventeen objects here in this order; the game links six. The stripped ones are
 * restored where their names fix the bits (IEEE minimum, maximum, epsilon, infinity; "extended" is double,
 * as __extended_min/max show). Left out: __double_tiny and __double_nan (after __double_huge), __extended_tiny
 * and __extended_nan (after __extended_huge): their bit patterns are not shown anywhere.
 */

unsigned long __float_nan[] = {0x7FFFFFFF};
unsigned long __float_huge[] = {0x7F800000};
unsigned long __double_min[] = {0x00100000, 0x00000000};
unsigned long __double_max[] = {0x7FEFFFFF, 0xFFFFFFFF};
unsigned long __double_epsilon[] = {0x3CB00000, 0x00000000};
unsigned long __double_huge[] = {0x7FF00000, 0x00000000};
unsigned long __extended_min[] = {0x00100000, 0x00000000};
unsigned long __extended_max[] = {0x7FEFFFFF, 0xFFFFFFFF};
unsigned long __extended_epsilon[] = {0x3CB00000, 0x00000000};
unsigned long __extended_huge[] = {0x7FF00000, 0x00000000};
unsigned long __float_min[] = {0x00800000};
unsigned long __float_max[] = {0x7F7FFFFF};
unsigned long __float_epsilon[] = {0x34000000};
