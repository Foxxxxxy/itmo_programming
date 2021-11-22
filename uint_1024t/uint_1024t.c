#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "uint_1024.h"

const uint64_t BASE_S = 4294967296;
const uint32_t INT_MAX = 4294967295;

void UE_EX() {
    printf("Overflow.\n");
    _Exit(EXIT_FAILURE);
};

size_t sizeof_uint1024_t () {
    return sizeof(uint1024_t);
}

uint1024_t from_uint(unsigned int x) {
    uint1024_t r;
    for (int i = 0; i < 31; ++i)
        r.data[i] = 0;
    r.data[31] = x;
    return r;
}


void swap_uint(uint1024_t *x, uint1024_t *y) {
    uint1024_t t = *x;
    *x = *y;
    *y = t;
}

int is_null_uint1024_t(uint1024_t x) {
    uint1024_t z = from_uint(0);
    return (memcmp(&x, &z, sizeof_uint1024_t()));
}


int dig_num(uint1024_t x) {
    for (int i = 0; i < 32; ++i) {
        if (x.data[i] > 0)
            return 32 - i;
    }
    return 0;
}


int equal_uint(uint1024_t x, uint1024_t y) {
    int f = 1;
    for (int i = 0; i < 32; ++i) {
        if (x.data[i] != y.data[i]) {
            f = 0;
            break;
        }
    }
    return f;
}


void greater_uint(uint1024_t x, uint1024_t y) {
    for (int i = 0; i < 32; ++i) {
        if (x.data[i] == 0 && y.data[i] > 0 || x.data[i] < y.data[i]) {
            swap_uint(&x, &y);
            return;
        }
    }
}


uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t res = from_uint(0);

    for (int i = 31; i >= 0; --i) {
        int ovf  = 0;
        if (y.data[i] > INT_MAX - x.data[i])
            ovf = 1;

        if (i == 0 && ovf)
            UE_EX();

        if (ovf) {
            uint64_t r = (y.data[i] + x.data[i]) % BASE_S;
            res.data[i] += r;
            res.data[i - 1]++;
        } else
            res.data[i] += (y.data[i] + x.data[i]);
    }
    return res;
}


uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t res = from_uint(0);

    if (equal_uint(x, y))
        return res;

    greater_uint(x, y);

    for (int i = 31; i >= 0; --i) {
        if (x.data[i] < y.data[i]) {
            for (int j = i - 1, f = 0; j >= 0; --j) {
                if (x.data[j] > 0) {
                    x.data[j]--;
                    uint64_t k = BASE_S - y.data[i] + x.data[i];
                    res.data[i] = (uint32_t) k;
                    f = 1;
                }
                if (x.data[j] == 0)
                    x.data[j] = INT_MAX;
                if(f)
                    break;
            }
        } else
            res.data[i] = x.data[i] - y.data[i];
    }

    return res;
}


uint1024_t small_mult_op (uint1024_t x, uint32_t y) {
    uint1024_t res = from_uint(0);
    for (int i = 31; i >= 0; --i) {
        uint64_t k = (uint64_t)x.data[i] * y + res.data[i];
        if (k >= BASE_S) {
            uint32_t r = (uint32_t) (k % BASE_S);
            uint32_t d = (uint32_t) (k / BASE_S);
            res.data[i] = r;
            uint1024_t s = from_uint(0);
            s.data[i - 1] = d;
            res = add_op(res, s);
        } else
            res.data[i] = k;
    }
    return res;
}


uint1024_t mult_op (uint1024_t x, uint1024_t y) {
    uint1024_t res = from_uint(0);

    for (int i = 31; i >= 0; --i) {
        uint1024_t t = small_mult_op(x, y.data[i]);
        int s = 31 - i;

        if (s + dig_num(t) > 32)
            UE_EX();

        uint1024_t tt = from_uint(0);
        for (int j = 31 - s, c = 31; j >= 0; --j, --c) {
            tt.data[j] = t.data[c];
        }

        res = add_op(res, tt);
    }

    return res;
}


void scanf_value(uint1024_t *x) {
    char *s = (char *) malloc(500);
    scanf("%s", s);
    *x = from_uint(0);
    for (int i = 0; i < strlen(s); ++i) {
        char c = s[strlen(s) - i - 1];
        int d = c - '0';
        uint1024_t tenp = from_uint(1);
        int k = 0;
        while (k < i) {
            tenp = mult_op(tenp, from_uint(10));
            ++k;
        }
        uint1024_t temp = mult_op(tenp, from_uint(d));
        *x = add_op(*x, temp);
    }
}


void printf_value(uint1024_t x) {
    uint1024_t t = from_uint(0);
    char str[350];
    int c = 0;
    uint64_t k;
    while (is_null_uint1024_t(x) != 0) {
        k = 0;
        for (int i = 0; i <= 31; ++i) {
            t.data[i] = ((uint64_t)x.data[i] + (uint64_t)k * BASE_S) / 10;
            k = ((uint64_t)x.data[i] + (uint64_t)k * BASE_S) % 10;
        }
        x = t;
        str[c] = (char)(k + '0');
        ++c;
    }

    for (int i = c - 1; i >= 0; --i)
        printf("%c", str[i]);
}

