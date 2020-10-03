//
//  storage.h
//  QuantumC
//
//  Created by Suhas Vittal on 9/13/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

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

#define SQR(x) ((x)*(x))

#define STATES_TO_BYTES(s) 32*s

uint16_t    _to_b2f(float n);
float       _from_b2f(uint16_t n);
uint16_t    __b2fadd__(uint16_t, uint16_t);
uint16_t    __b2fmul__(uint16_t, uint16_t);

uint16_t    _to_little_endian(uint16_t);

#define MODULUS_BIT_SIZE    16
#define ANGLE_BIT_SIZE      16
#define ANGLE_BASE          (1L << ANGLE_BIT_SIZE)

/* Conversion to/from 16 bit state to rect form (64 bit). */
void __polar_to_rect__(uint8_t*, float*);
void __rect_to_polar__(float*, uint8_t*);

/* Uniform method of getting and setting
   magnitudes and angles from a state array.

   Odd values correspond to magnitudes.
   Even values correspond to angles.
 */
uint64_t    __get_value__(uint8_t*, uint64_t);
void        __set_value__(uint8_t*, uint64_t, uint64_t);
/* Shifts the address n places. */
uint8_t*    __offset__(uint8_t*, uint64_t);
/* Get the location and size of the k-th value in the byte array. */
void        __travel__(uint64_t, uint64_t*, uint8_t*);

#define GET(p, i)       __get_value__(p, i)
#define SET(p, i, n)    __set_value__(p, i, n)
#define MOV(p, i)       __offset__(p, i)

#endif /* storage_h */

#ifdef __cplusplus
}
#endif
