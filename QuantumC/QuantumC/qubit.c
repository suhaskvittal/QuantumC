//
//  qubit.c
//  QuantumC
//
//  Created by Suhas Vittal on 9/6/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#include "qubit.h"
#include "gates.h"
#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

uint8_t G_QUBIT_PRECISION = QUBIT_8_8;

struct qubit_sys* tensor_product(struct qubit_sys* a, struct qubit_sys* b) {
    uint8_t e_level = a->n_qubits + b->n_qubits;
    uint8_t states[STATES_TO_BYTES(1L << e_level)];
    
    uint64_t k = 0;
    
    uint64_t a_n_st = 1 << a->n_qubits;
    uint64_t b_n_st = 1 << b->n_qubits;
    for (uint64_t i = 0; i < 2*a_n_st; i += 2) {
        uint64_t a_mod = GET(a->state_p, i);
        uint64_t a_ang = GET(a->state_p, i + 1);
        
        for (uint64_t j = 0; j < 2*b_n_st; j += 2) {
            uint64_t b_mod = GET(b->state_p, j);
            uint64_t b_ang = GET(b->state_p, j + 1);
            
            // set values in states
            SET(states, k, __b2fmul__(a_mod, b_mod));  // multiply moduli
            SET(states, k + 1, __b2fadd__(a_ang, b_ang));  // add angles
            k += 2;
        }
    }
    QFREE(a);
    QFREE(b);
    struct qubit_sys* ab = malloc(1 * sizeof(struct qubit_sys));
    ab->state_p = malloc(sizeof(states));
    memmove(ab->state_p, states, sizeof(states) / sizeof(uint8_t));
    ab->n_qubits = e_level;
    return ab;
}

struct qubit_sys* create_one() {
    struct qubit_sys* q_p = malloc(1 * sizeof(struct qubit_sys));
    q_p->state_p = calloc(STATES_TO_BYTES(2), sizeof(uint8_t));
    SET(q_p->state_p, 2, _to_b2f(1.0f));
    q_p->n_qubits = 1;
    
    return q_p;
}

struct qubit_sys* create_zero() {
    struct qubit_sys* q_p = malloc(1 * sizeof(struct qubit_sys));
    q_p->state_p = calloc(STATES_TO_BYTES(2), sizeof(uint8_t));
    SET(q_p->state_p, 0, _to_b2f(1.0f));
    q_p->n_qubits = 1;
    
    return q_p;
}

struct qubit_sys* create_uniform(uint8_t sys_size) {
    struct qubit_sys* q_p = create_qubit_sys(sys_size);
    for (uint8_t i = 0; i < sys_size; i++) {
        p_apply_gate(q_p, &i, 1, &_hadamard);
    }
    
    return q_p;
}

struct qubit_sys* create_qubit_sys(uint8_t sys_size) {
    struct qubit_sys* q_p = malloc(1 * sizeof(struct qubit_sys));
    q_p->state_p = calloc(STATES_TO_BYTES(1L << sys_size), sizeof(uint8_t));
    SET(q_p->state_p, 0, _to_b2f(1.0f));
    q_p->n_qubits = sys_size;
    
    return q_p;
}

