#ifndef UINT_1024T_UINT_1024_H
#define UINT_1024T_UINT_1024_H

#include <stdint.h>

typedef struct uint1024_t uint1024_t;

struct uint1024_t {
    uint32_t data [32];
};

uint1024_t from_uint(unsigned int x);

int is_null_uint1024_t (uint1024_t x);

void swap_uint(uint1024_t *x, uint1024_t *y);

void greater_uint(uint1024_t x, uint1024_t y);

uint1024_t add_op(uint1024_t x, uint1024_t y);

uint1024_t subtr_op(uint1024_t x, uint1024_t y);

uint1024_t small_mult_op(uint1024_t x, unsigned int y);
uint1024_t mult_op(uint1024_t x, uint1024_t y);


void scanf_value(uint1024_t* x);
void printf_value(uint1024_t x);

#endif //UINT_1024T_UINT_1024_H
