//
//  qubit.h
//  QuantumC
//
//  Created by Suhas Vittal on 9/6/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef qubit_h
#define qubit_h

#include <stdint.h>
#include <stdlib.h>

/*
    Entangled qubit.
 */
struct qubit_sys {
    uint8_t     n_qubits;  // number of qubits, number of states is just 1 << n_qubits.
    uint8_t*    state_p;
};

/* The below function frees the inputs and allocates the output. */
struct qubit_sys* tensor_product(struct qubit_sys*, struct qubit_sys*);

struct qubit_sys* create_one(void);
struct qubit_sys* create_zero(void);
struct qubit_sys* create_uniform(uint8_t);
struct qubit_sys* create_qubit_sys(uint8_t);

void measure(struct qubit_sys*);

void print_qubit(struct qubit_sys*);

#define QFREE(q)    free(q->state_p); free(q)

#endif /* qubit_h */

#ifdef __cplusplus
}
#endif
