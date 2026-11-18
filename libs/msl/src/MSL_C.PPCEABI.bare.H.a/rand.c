static unsigned long next = 1;

void srand(unsigned long seed) {
    next = seed;
}

int rand(void) {
    next = next * 1103515245 + 12345;
    return (next >> 16) & 0x7FFF;
}
