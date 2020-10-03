//
//  qubit.c
//  QuantumC
//
//  Created by Suhas Vittal on 9/6/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#include "../include/qubit.h"
#include "../include/gates.h"
#include "../include/storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>


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
    uint16_t uniform_amplitude = _to_b2f(1.0f / sqrtf(1L << sys_size));
    for (uint64_t i = 0; i < 1L << sys_size; i++) {
        SET(q_p->state_p, 2*i, uniform_amplitude);
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

void measure(struct qubit_sys* q) {
    
}

void print_qubit(struct qubit_sys* q_p) {
    uint64_t n_states = 1 << q_p->n_qubits;
    
    uint64_t p = 0;
    while (p < n_states) {
        uint64_t mod = GET(q_p->state_p, 2*p);
        uint64_t ang = GET(q_p->state_p, 2*p + 1);
        
        uint8_t mod_chunks = MODULUS_BIT_SIZE / 4;
        while (mod_chunks--) {
            printf("%llx", (mod >> (mod_chunks * 4)) & 0xF);
        }
        printf("|");
        uint8_t ang_chunks = ANGLE_BIT_SIZE / 4;
        while (ang_chunks--) {
            printf("%llx", (ang >> (ang_chunks * 4)) & 0xF);
        }
        printf(" ");
        p++;
    }
    printf("\n");
}

