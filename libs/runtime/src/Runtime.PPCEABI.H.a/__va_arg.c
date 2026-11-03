typedef struct __va_list_struct {
    char gpr;
    char fpr;
    char reserved[2];
    char* input_arg_area;
    char* reg_save_area;
} _va_list[1];

void* __va_arg(_va_list v_list, int type) {
    char* addr;
    char* reg = &(v_list->gpr);
    int registerIndex = v_list->gpr;
    int maxsize = 8;
    int size = 4;
    int increment = 1;
    int even = 0;
    int fpr_offset = 0;
    int regsize = 4;

    if (type == 3) {
        reg = &(v_list->fpr);
        registerIndex = v_list->fpr;
        size = 8;
        fpr_offset = 32;
        regsize = 8;
    }

    if (type == 2) {
        size = 8;
        maxsize--;
        if (registerIndex & 1)
            even = 1;
        increment = 2;
    }

    if (registerIndex < maxsize) {
        registerIndex += even;
        addr = v_list->reg_save_area + fpr_offset + (registerIndex * regsize);
        *reg = registerIndex + increment;
    } else {
        *reg = 8;
        addr = v_list->input_arg_area;
        addr = (char*)(((unsigned long)(addr) + ((size)-1)) & ~((size)-1));
        v_list->input_arg_area = addr + size;
    }

    if (type == 0)
        addr = *((char**)addr);

    return addr;
}
