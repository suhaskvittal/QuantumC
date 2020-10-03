//
//  storage.c
//  QuantumC
//
//  Created by Suhas Vittal on 9/13/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#include "../include/storage.h"

#include <math.h>

/*
    Adjusted half-precision:
        5 exponent bits with an offset of 32.
        11+1 mantissa bits.
 
        x0000 = 0, xFFFF = 1
 */

uint16_t _to_b2f(float n) {
    if (n >= 1.0f)  { return 0xFFFF; }
    if (n == 0.0f)  { return 0x0000; }
    // convert to adjusted half-precision
    
    uint32_t mantissa = 0x0000;
    float pow = 0.5f;
    uint8_t k = 0;
    while (n > 0.0f && k < 32) {
        if (n >= pow) {
            n -= pow;
            mantissa |= (1 << (32 - k - 1));
        } else if (pow > n && pow - n <= 0.0001) {  // adjusting for rounding error
            n = 0.0f;
            mantissa |= (1 << (32 - k - 1));
        }
        pow *= 0.5f;
        k++;
    }
    
    // normalize value
    uint8_t exp = 31;
    while (((mantissa >> 31) & 0x1) != 1) {
        exp--;
        mantissa <<= 1;
    }
    uint16_t hfp = ((exp & 0x1F) << 11) | (mantissa >> 21);
    return hfp;
}

float _from_b2f(uint16_t n) {
    if (n == 0xFFFF) { return 1.0f; }
    if (n == 0x0000) { return 0.0f; }
    
    uint8_t exp = (n >> 11) & 0x1F;
    uint16_t mantissa = n & 0x7FF;
    
    float pow = 0.5f;
    float val = 0.0f;
    for (uint8_t i = 0; i < 11; i++) {
        val += pow * ((mantissa >> (11 - i - 1)) & 0x1);
        pow *= 0.5f;
    }
    
    while (exp < 31) {
        val *= 0.5f;
        exp++;
    }
    
    return val;
}

uint16_t __b2fadd__(uint16_t a, uint16_t b) {
    float sum = _from_b2f(a) + _from_b2f(b);
    return _to_b2f(sum > 1.0f ? 1.0f : sum);
}

uint16_t __b2fmul__(uint16_t a, uint16_t b) {
    float prod = _from_b2f(a) * _from_b2f(b);
    return _to_b2f(prod);
}
uint16_t _to_little_endian(uint16_t n) {
    int test = 1;
    // little endian if true
    if (*(char *)&test == 1) {
        return n;
    } else {
        return (uint16_t) __builtin_bswap64(n);
    }
}

void __polar_to_rect__(uint8_t* p, float* arr) {
    uint64_t mod = GET(p, 0);
    uint64_t ang = GET(p, 1);
    
    arr[0] = _from_b2f(mod) * cosf((ang / (ANGLE_BASE * 1.0f)) * 2 * M_PI);
    arr[1] = _from_b2f(mod) * sinf((ang / (ANGLE_BASE * 1.0f)) * 2 * M_PI);
}

void __rect_to_polar__(float* st, uint8_t* p) {
    float mod = sqrtf(SQR(st[0]) + SQR(st[1]));
    float ang = atan2f(st[1], st[0]);

    uint64_t p0 = _to_b2f(mod);
    
    //if (p0 < MODULUS_BASE && ang > M_PI) {
    //    p0 |= MODULUS_BASE;
    //    ang -= M_PI;
    //}
    
    SET(p, 0, p0);
    if (p0) SET(p, 1, (uint64_t)
                (ang * (ANGLE_BASE / (2 * M_PI)) + 0.5)
                );
    else    SET(p, 1, 0);
}

uint64_t __get_value__(uint8_t* p, uint64_t k) {
    uint64_t loc;
    uint8_t nbytes;
    __travel__(k, &loc, &nbytes);
    
    uint64_t data = 0;
    for (int i = 0; i < nbytes; i++) {
        data |= p[loc + i] << (i << 3);
    }
    
    return data;
}

void __set_value__(uint8_t* p, uint64_t k, uint64_t x) {
    uint64_t loc;
    uint8_t nbytes;
    __travel__(k, &loc, &nbytes);
    
    for (int i = 0; i < nbytes; i++) {
        p[loc + i] = x & 0xFF;
        x >>= 8;
    }
}

uint8_t* __offset__(uint8_t* p, uint64_t k) {
    uint64_t loc;
    uint8_t tmp;
    __travel__(k, &loc, &tmp);  // we don't use tmp
    return p + loc;
}

void __travel__(uint64_t k, uint64_t* i_p, uint8_t* b_p) {
    *i_p =
            MODULUS_BIT_SIZE / (8 * sizeof(uint8_t)) * ((k + 1) / 2)
            + ANGLE_BIT_SIZE / (8 * sizeof(uint8_t)) * (k / 2);
    *b_p = (k % 2 ? ANGLE_BIT_SIZE : MODULUS_BIT_SIZE) / (8 * sizeof(uint8_t));
}
