//
//  storage.h
//  QuantumC
//
//  Created by Suhas Vittal on 9/13/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#ifndef storage_h
#define storage_h

#include <stdint.h>

#define QUBIT_8_8   0
#define QUBIT_16_8  1
#define QUBIT_16_16 2
#define QUBIT_8_16  3
#define QUBIT_32_8  4
#define QUBIT_32_16 5
#define QUBIT_32_32 6
#define QUBIT_16_32 7
#define QUBIT_8_32  8

extern uint8_t G_QUBIT_PRECISION;

#define STATES_TO_BYTES(s) \
    (__modulus_bit_size__(G_QUBIT_PRECISION) + \
        __angle_bit_size__(G_QUBIT_PRECISION))*(s)/8

uint64_t    _to_b2f(float n);
float       _from_b2f(uint64_t n);
uint64_t    __b2fadd__(uint64_t, uint64_t);
uint64_t    __b2fmul__(uint64_t, uint64_t);

uint64_t    _convert_btwn_b2f(uint64_t, uint8_t, uint8_t);

uint64_t    _to_little_endian(uint64_t);

uint8_t     __modulus_bit_size__(uint8_t);
uint8_t     __angle_bit_size__(uint8_t);
uint64_t    __modulus_base__(uint8_t);
uint64_t    __angle_base__(uint8_t);

#define MODULUS_BIT_SIZE    __modulus_bit_size__(G_QUBIT_PRECISION)
#define ANGLE_BIT_SIZE      __angle_bit_size__(G_QUBIT_PRECISION)
#define MODULUS_BASE    __modulus_base__(MODULUS_BIT_SIZE)
#define ANGLE_BASE      __angle_base__(G_QUBIT_PRECISION)

#endif /* storage_h */
