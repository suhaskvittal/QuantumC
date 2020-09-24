//
//  storage.c
//  QuantumC
//
//  Created by Suhas Vittal on 9/13/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#include "storage.h"

uint64_t _to_b2f(float n) {
    return (uint64_t) (n * MODULUS_BASE + 0.5);
}

float _from_b2f(uint64_t n) {
    return ((float) n) / MODULUS_BASE;
}

uint64_t __b2fadd__(uint64_t a, uint64_t b) {
    float sum = _from_b2f(a) + _from_b2f(b);
    return _to_b2f(sum > 1.0f ? 1.0f : sum);
}

uint64_t __b2fmul__(uint64_t a, uint64_t b) {
    float prod = _from_b2f(a) * _from_b2f(b);
    return _to_b2f(prod);
}

uint64_t _convert_btwn_b2f(uint64_t n, uint8_t b1, uint8_t b2) {
    float sum = 0.0f;
    uint8_t b = __modulus_bit_size__(b1);
    
    float p = 0.5;
    while (b--) {
        uint8_t bit = (n >> b) & 0x1;
        sum += bit * p;
        p *= 0.5;
    }
    
    b = __modulus_bit_size__(b2);
    uint64_t f = 0;
    p = 0.5;
    while (b--) {
        if (n >= p) {
            f |= 1 << b;
        }
        p *= 0.5;
    }
    
    return f;
}

uint64_t _to_little_endian(uint64_t n) {
    int test = 1;
    // little endian if true
    if (*(char *)&test == 1) {
        return n;
    } else {
        return __builtin_bswap64(n);
    }
}

uint8_t __modulus_bit_size__(uint8_t precision) {
    switch (precision) {
        case QUBIT_8_8:
        case QUBIT_8_16:
        case QUBIT_8_32:
            return 8;
        case QUBIT_16_8:
        case QUBIT_16_16:
        case QUBIT_16_32:
            return 16;
        case QUBIT_32_8:
        case QUBIT_32_16:
        case QUBIT_32_32:
            return 32;
        default:
            return 8;
    }
}

uint8_t __angle_bit_size__(uint8_t precision) {
    switch (precision) {
        case QUBIT_8_8:
        case QUBIT_16_8:
        case QUBIT_32_8:
            return 8;
        case QUBIT_32_16:
        case QUBIT_16_16:
        case QUBIT_8_16:
            return 16;
        case QUBIT_8_32:
        case QUBIT_16_32:
        case QUBIT_32_32:
            return 32;
        default:
            return 8;
    }
}

uint64_t __modulus_base__(uint8_t precision) {
    switch (precision) {
        case 8:
            return 200;
        case 16:
            return 20000;
        default:
            return 200;
    }
}

uint64_t __angle_base__(uint8_t precision) {
    return 1 << __angle_bit_size__(precision);
}
