//
//  gates.h
//  QuantumC
//
//  Created by Suhas Vittal on 9/7/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef gates_h
#define gates_h

#include "qubit.h"

#include <stdint.h>

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

#endif /* gates_h */

#ifdef __cplusplus
}
#endif
