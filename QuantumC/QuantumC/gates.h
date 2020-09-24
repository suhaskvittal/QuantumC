//
//  gates.h
//  QuantumC
//
//  Created by Suhas Vittal on 9/7/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#ifndef gates_h
#define gates_h

#include "qubit.h"

#include <stdint.h>

#define SQR(x) ((x)*(x))

/* All gates write the output at the address pointed to by the input. */

struct qubit_sys* apply_gate(struct qubit_sys*, void (*)(uint8_t**));
struct qubit_sys* p_apply_gate(struct qubit_sys*, uint8_t*, uint8_t, void (*)(uint8_t**));

/* Functions for getting sub-qubit systems.*/
uint64_t get_subsys(struct qubit_sys*, uint8_t*, uint8_t, uint64_t***);
void __populate_vert__(uint8_t, uint8_t, uint64_t, uint64_t**);
void __populate_horz__(uint8_t, uint8_t, uint64_t, uint64_t, uint64_t***);
void __toggle_sysbit__(uint8_t, uint64_t, uint64_t*);


/* Level 1 gates. */
void _hadamard(uint8_t**);
void _paulix(uint8_t**);
void _pauliy(uint8_t**);
void _pauliz(uint8_t**);
void _pshift(uint8_t**, float);
void _rtnot(uint8_t**);
/* Level 2 gates. */
void _swap(uint8_t**);
void _rtswap(uint8_t**);
void _cnot(uint8_t**);
void _cpshift(uint8_t**, float);
/* Level 3 gates. */
void _ccnot(uint8_t**);
void _cswap(uint8_t**);

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

#endif /* gates_h */
