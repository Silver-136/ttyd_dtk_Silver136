typedef struct DestructorChain {
    struct DestructorChain* next;
    void (*destructor)(void*, short);
    void* object;
} DestructorChain;

DestructorChain* __global_destructor_chain;

void __destroy_global_chain(void) {
    DestructorChain* gdc;

    while ((gdc = __global_destructor_chain) != 0) {
        __global_destructor_chain = gdc->next;
        gdc->destructor(gdc->object, -1);
    }
}

/* Listed in .dtors, so the global destructor chain runs at exit. */
__declspec(section ".dtors") static void* const __destroy_global_chain_reference = __destroy_global_chain;
