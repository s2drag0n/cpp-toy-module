#include "my_malloc.h"
#include <iostream>

int main() {

    std::cout << "normal malloc" << std::endl;
    /* normal malloc */
    for (int i = 16; i < 32; ++i) {
        void *p = malloc(i);
        std::cout << p << std::endl;
        free(p);
    }

    std::cout << "aligned malloc" << std::endl;
    /* my_malloc */
    for (int i = 16; i < 32; ++i) {
        void *p = malloc_aligned(i, 128);
        std::cout << p << std::endl;
        my_free(p);
    }
}
