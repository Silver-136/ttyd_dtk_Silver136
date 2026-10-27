#define NULL 0

typedef unsigned long mem_size;

void __begin_critical_region(int region);
void __end_critical_region(int region);
void* memset(void* dst, int val, unsigned long n);
void __sys_free(void* ptr);

#define malloc_pool_access 1

typedef struct Block {
    struct Block* prev_;
    struct Block* next_;
    mem_size max_size_;
    mem_size size_;
} Block;

typedef struct SubBlock {
    mem_size size_;
    Block* bp_;
    struct SubBlock* prev_;
    struct SubBlock* next_;
} SubBlock;

struct FixSubBlock;

typedef struct FixBlock {
    struct FixBlock* prev_;
    struct FixBlock* next_;
    mem_size client_size_;
    struct FixSubBlock* start_;
    mem_size n_allocated_;
} FixBlock;

typedef struct FixSubBlock {
    FixBlock* block_;
    struct FixSubBlock* next_;
} FixSubBlock;

typedef struct FixStart {
    FixBlock* tail_;
    FixBlock* head_;
} FixStart;

#define num_fix_pools 6

typedef struct __mem_pool_obj {
    Block* start_;
    FixStart fix_start[num_fix_pools];
} __mem_pool_obj;

static const mem_size fix_pool_sizes[] = {4, 12, 20, 36, 52, 68};

#define max_fix_pool_size 68

#define Block_size(ths) ((ths)->size_ & ~7)
#define Block_start(ths) (*(SubBlock**)((char*)(ths) + Block_size((ths)) - sizeof(mem_size)))

#define SubBlock_size(ths) ((ths)->size_ & ~7)
#define SubBlock_block(ths) ((Block*)((mem_size)((ths)->bp_) & ~1))
#define SubBlock_is_free(ths) (!((ths)->size_ & 2))
#define SubBlock_from_pointer(ptr) ((SubBlock*)((char*)(ptr) - 8))
#define FixSubBlock_from_pointer(ptr) ((FixSubBlock*)((char*)(ptr) - 4))

#define classify(ptr) (*(mem_size*)((char*)(ptr) - sizeof(mem_size)) & 1)

static inline void SubBlock_set_free(SubBlock* ths) {
    mem_size this_size = SubBlock_size(ths);
    ths->size_ &= ~2;
    *(mem_size*)((char*)ths + this_size) &= ~4;
    *(mem_size*)((char*)ths + this_size - sizeof(mem_size)) = this_size;
}

#define SubBlock_set_size(ths, sz)                                         \
    (ths)->size_ &= ~(~7);                                                 \
    (ths)->size_ |= (sz) & ~7;                                             \
    if (SubBlock_is_free((ths)))                                           \
        *(mem_size*)((char*)(ths) + (sz) - sizeof(mem_size)) = (sz)

static SubBlock* SubBlock_merge_prev(SubBlock* ths, SubBlock** start) {
    mem_size prevsz;
    SubBlock* p;

    if (!(ths->size_ & 4)) {
        prevsz = *(mem_size*)((char*)ths - sizeof(mem_size));
        if (prevsz & 2)
            return ths;
        p = (SubBlock*)((char*)ths - prevsz);
        SubBlock_set_size(p, prevsz + SubBlock_size(ths));
        if (*start == ths)
            *start = (*start)->next_;
        ths->next_->prev_ = ths->prev_;
        ths->next_->prev_->next_ = ths->next_;
        return p;
    }
    return ths;
}

static void SubBlock_merge_next(SubBlock* ths, SubBlock** start) {
    SubBlock* next_sub;
    mem_size this_size;

    next_sub = (SubBlock*)((char*)ths + SubBlock_size(ths));
    if (SubBlock_is_free(next_sub)) {
        this_size = SubBlock_size(ths) + SubBlock_size(next_sub);
        SubBlock_set_size(ths, this_size);
        if (SubBlock_is_free(ths))
            *(mem_size*)((char*)ths + this_size) &= ~4;
        else
            *(mem_size*)((char*)ths + this_size) |= 4;
        if (*start == next_sub)
            *start = (*start)->next_;
        if (*start == next_sub)
            *start = NULL;
        next_sub->next_->prev_ = next_sub->prev_;
        next_sub->prev_->next_ = next_sub->next_;
    }
}

#define Block_link(ths, sb)                                  \
    {                                                        \
        SubBlock** st;                                       \
        SubBlock_set_free((sb));                             \
        st = &Block_start((ths));                            \
        if (*st != NULL) {                                   \
            (sb)->prev_ = (*st)->prev_;                      \
            (sb)->prev_->next_ = (sb);                       \
            (sb)->next_ = *st;                               \
            (*st)->prev_ = (sb);                             \
            *st = (sb);                                      \
            *st = SubBlock_merge_prev(*st, st);              \
            SubBlock_merge_next(*st, st);                    \
        } else {                                             \
            *st = (sb);                                      \
            (sb)->prev_ = (sb);                              \
            (sb)->next_ = (sb);                              \
        }                                                    \
        if ((ths)->max_size_ < SubBlock_size(*st))           \
            (ths)->max_size_ = SubBlock_size(*st);           \
    }

static inline int Block_empty(Block* ths) {
    SubBlock* sb = (SubBlock*)((char*)ths + 16);
    return SubBlock_is_free(sb) && SubBlock_size(sb) == Block_size(ths) - 24;
}

void deallocate_from_var_pools(__mem_pool_obj* pool_obj, void* ptr) {
    SubBlock* sb = SubBlock_from_pointer(ptr);
    Block* bp = SubBlock_block(sb);

    Block_link(bp, sb);
    if (Block_empty(bp)) {
        Block* next = bp->next_;
        if (next == bp)
            next = NULL;
        if (pool_obj->start_ == bp)
            pool_obj->start_ = next;
        if (next != NULL) {
            next->prev_ = bp->prev_;
            next->prev_->next_ = next;
        }
        bp->prev_ = bp->next_ = NULL;
        __sys_free(bp);
    }
}

void deallocate_from_fixed_pools(__mem_pool_obj* pool_obj, void* ptr, mem_size size) {
    mem_size i = 0;
    FixSubBlock* p;
    FixBlock* b;
    FixStart* fs;

    while (size > fix_pool_sizes[i])
        ++i;

    fs = &pool_obj->fix_start[i];
    p = FixSubBlock_from_pointer(ptr);
    b = p->block_;

    if (b->start_ == NULL && fs->head_ != b) {
        if (fs->tail_ == b) {
            fs->head_ = fs->head_->prev_;
            fs->tail_ = fs->tail_->prev_;
        } else {
            b->prev_->next_ = b->next_;
            b->next_->prev_ = b->prev_;
            b->next_ = fs->head_;
            b->prev_ = b->next_->prev_;
            b->prev_->next_ = b;
            b->next_->prev_ = b;
            fs->head_ = b;
        }
    }

    p->next_ = b->start_;
    b->start_ = p;

    if (--b->n_allocated_ == 0) {
        if (fs->head_ == b)
            fs->head_ = b->next_;
        if (fs->tail_ == b)
            fs->tail_ = b->prev_;
        b->prev_->next_ = b->next_;
        b->next_->prev_ = b->prev_;
        if (fs->head_ == b)
            fs->head_ = NULL;
        if (fs->tail_ == b)
            fs->tail_ = NULL;
        deallocate_from_var_pools(pool_obj, b);
    }
}

void __pool_free(__mem_pool_obj* pool_obj, void* ptr) {
    mem_size size;

    if (ptr == NULL)
        return;

    size = !classify(ptr) ? FixSubBlock_from_pointer(ptr)->block_->client_size_ : SubBlock_size(SubBlock_from_pointer(ptr)) - 8;

    if (size <= max_fix_pool_size)
        deallocate_from_fixed_pools(pool_obj, ptr, size);
    else
        deallocate_from_var_pools(pool_obj, ptr);
}

static __mem_pool_obj* get_malloc_pool(void) {
    static __mem_pool_obj protopool;
    static unsigned char init = 0;

    if (!init) {
        memset(&protopool, 0, sizeof(__mem_pool_obj));
        init = 1;
    }
    return &protopool;
}

void free(void* ptr) {
    __begin_critical_region(malloc_pool_access);
    __pool_free(get_malloc_pool(), ptr);
    __end_critical_region(malloc_pool_access);
}
