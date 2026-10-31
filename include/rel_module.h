// REL module hooks: the constructor/destructor tables the linker emits for every REL and the
// _prolog/_epilog/_unresolved entry points the module loader calls.
#pragma once

typedef void (*voidfunctionptr)(void);

extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _prolog(void);
void _epilog(void);
void _unresolved(void);
