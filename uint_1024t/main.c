#include "cmake-build-debug/uint_1024.h"

int main() {
    uint1024_t x = from_uint(0);
    uint1024_t y = from_uint(0);
    /*
    t = add_op(t, from_uint(10));
    t = mult_op(t, from_uint(100));
    t = subtr_op(t, from_uint(1000));
    */

    scanf_value(&x);
    scanf_value(&y);
    uint1024_t p = subtr_op(x, y);
    printf_value(p);
    return 0;
}
