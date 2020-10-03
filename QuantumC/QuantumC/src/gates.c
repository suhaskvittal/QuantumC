//
//  gates.c
//  QuantumC
//
//  Created by Suhas Vittal on 9/7/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#include "../include/gates.h"
#include "../include/storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct qubit_sys* apply_gate(struct qubit_sys* q,
                             void (*g_p)(uint8_t**))
{
    uint64_t n_states = 1L << q->n_qubits;
    uint8_t* s_p[n_states];
    
    for (uint64_t i = 0; i < n_states; i++) {
        s_p[i] = MOV(q->state_p, 2*i);
    }
    g_p(s_p);
    return q;
}

struct qubit_sys* p_apply_gate(struct qubit_sys* q, uint8_t* qubit_n,
                               uint8_t qubit_n_size, void (*g_p)(uint8_t**))
{
    uint64_t** subsys_arr;
    uint64_t n_subsys = get_subsys(q, qubit_n, qubit_n_size, &subsys_arr);
    uint64_t subsys_sz = 1 << qubit_n_size;
    
    /* The subsys array contains the indices for each subsystem.
     We simply have to extract the states at these indices
     and place them in an array. */
    
    uint8_t* s_p[subsys_sz];  // just reuse this each loop
    for (uint8_t i = 0; i < n_subsys; i++) {
        for (uint64_t j = 0; j < subsys_sz; j++) {
            s_p[j] = MOV(q->state_p, 2*subsys_arr[i][j]);
        }
        g_p(s_p);
        free(subsys_arr[i]);
    }
    free(subsys_arr);
    return q;
}

uint64_t get_subsys(struct qubit_sys* q, uint8_t* qubit_n, uint8_t qubit_n_size, uint64_t*** subsys_pp)
{
    uint64_t n_subsys = 1L << (q->n_qubits - qubit_n_size);
    uint64_t subsys_sz = 1L << qubit_n_size;
    
    *subsys_pp = malloc(n_subsys * sizeof(uint64_t*));
    (*subsys_pp)[0] = calloc(subsys_sz, sizeof(uint64_t));
    
    uint8_t other_qubit_n[q->n_qubits];  // 1 if not in qubit_n
    memset(other_qubit_n, 1, sizeof(other_qubit_n));
    
    uint8_t i = 0;
    uint8_t j = 0;
    while (j < qubit_n_size) {
        if (i == qubit_n[j]) {
            __populate_vert__(qubit_n[j], j + 1, subsys_sz, *subsys_pp);
            other_qubit_n[i] = 0;
            j++;
        }
        i++;
    }
    
    // allocate all the subsystems
    for (uint8_t i = 1; i < n_subsys; i++) {
        (*subsys_pp)[i] = malloc(subsys_sz * sizeof(uint64_t));
        memmove((*subsys_pp)[i], (*subsys_pp)[0], subsys_sz * sizeof(uint64_t));
    }
    
    uint8_t k = 0;
    for (uint8_t i = 0; i < q->n_qubits; i++) {
        if (other_qubit_n[i]) {
            __populate_horz__(i, 1 << (k++), subsys_sz, n_subsys, subsys_pp);
        }
    }
    
    return n_subsys;
}

void __populate_vert__(uint8_t n, uint8_t step, uint64_t sys_sz, uint64_t** sys_p) {
    for (uint8_t i = step; i < sys_sz; i += step) {
        for (uint8_t j = 0; j < step && i < sys_sz; j++) {
            (*sys_p)[i++] |= 1L << n;
        }
    }
}

void __populate_horz__(uint8_t n, uint8_t step, uint64_t sys_sz, uint64_t n_sys, uint64_t*** sys_pp) {
    for (uint8_t i = step; i < n_sys; i += step) {
        for (uint8_t j = 0; j < step && i < n_sys; j++) {
            __toggle_sysbit__(n, sys_sz, (*sys_pp)[i++]);
        }
    }
}

void __toggle_sysbit__(uint8_t n, uint64_t sys_sz, uint64_t* sys) {
    while (sys_sz--) {
        sys[sys_sz] |= 1L << n;
    }
}


void _hadamard(uint8_t** s_p) {
    /* It is very hard to compute hadamard of a polar form
     complex number. We will convert the data to rect form,
     apply the gate, and then convert back. */
    float a_st[4];
    __polar_to_rect__(s_p[0], a_st);
    __polar_to_rect__(s_p[1], a_st + 2);
    
    float inv_rt2_b10 = sqrtf(0.5);
    // initialize states for b
    float b_st[4] = {
        inv_rt2_b10 * (a_st[0] + a_st[2]),
        inv_rt2_b10 * (a_st[1] + a_st[3]),
        inv_rt2_b10 * (a_st[0] - a_st[2]),
        inv_rt2_b10 * (a_st[1] - a_st[3])
    };
    
    // now convert back to polar form for b
    __rect_to_polar__(b_st, s_p[0]);
    __rect_to_polar__(b_st + 2, s_p[1]);
}

void _paulix(uint8_t** s_p) {
    uint64_t a0 = GET(s_p[0], 0);
    uint64_t a1 = GET(s_p[0], 1);
    SET(s_p[0], 0, GET(s_p[1], 0));
    SET(s_p[0], 1, GET(s_p[1], 1));
    SET(s_p[1], 0, a0);
    SET(s_p[1], 1, a1);
}

void _pauliy(uint8_t** s_p) {
    uint64_t a0 = GET(s_p[0], 0);
    uint64_t a1 = GET(s_p[0], 1);
    SET(s_p[0], 0, GET(s_p[1], 0));
    SET(s_p[0], 1, (uint64_t) (GET(s_p[1], 1) + (0.75f * ANGLE_BASE)) % ANGLE_BASE);
    SET(s_p[1], 0, a0);
    SET(s_p[1], 1, (uint64_t) (a1 + (0.25f * ANGLE_BASE)) % ANGLE_BASE);
}

void _pauliz(uint8_t** s_p) {
    SET(s_p[1], 1, (uint64_t) (GET(s_p[1], 1) + (0.5f * ANGLE_BASE)) % ANGLE_BASE);
}

void _pshift(uint8_t** s_p, float phase) {
    if (GET(s_p[1], 0)) {
        SET(s_p[1], 1, GET(s_p[1], 1) + (phase * ANGLE_BASE) / (2.0f * M_PI));  // simply add the angles
    }
}

void _rtnot(uint8_t** s_p) {
    float a_st[4];
    __polar_to_rect__(s_p[0], a_st);
    __polar_to_rect__(s_p[1], a_st + 2);
    
    float b_st[4] = {
        0.5f * (a_st[0] - a_st[1]) + 0.5f * (a_st[2] + a_st[3]),
        0.5f * (a_st[0] + a_st[1]) + 0.5f * (a_st[3] - a_st[2]),
        0.5f * (a_st[0] + a_st[1]) + 0.5f * (a_st[2] - a_st[3]),
        0.5f * (a_st[1] - a_st[0]) + 0.5f * (a_st[2] + a_st[3])
    };
    
    __rect_to_polar__(b_st, s_p[0]);
    __rect_to_polar__(b_st + 2, s_p[1]);
}

void _swap(uint8_t** s_p) {
    uint64_t a2 = GET(s_p[1], 0);
    uint64_t a3 = GET(s_p[1], 1);
    SET(s_p[1], 0, GET(s_p[1], 0));
    SET(s_p[1], 1, GET(s_p[1], 1));
    SET(s_p[2], 0, a2);
    SET(s_p[2], 1, a3);
}

void _rtswap(uint8_t** s_p) {
    float a_st[8];
    for (uint8_t i = 0; i < 4; i++) {
        __polar_to_rect__(s_p[i], a_st + 2*i);
    }
    
    float b_st[8] = {
        a_st[0], // 00 is the same
        a_st[1],
        0.5f*(a_st[2] - a_st[3]) + 0.5f*(a_st[4] + a_st[5]),
        0.5f*(a_st[2] + a_st[3]) + 0.5f*(a_st[5] - a_st[4]),
        0.5f*(a_st[2] + a_st[3]) + 0.5f*(a_st[4] - a_st[5]),
        0.5f*(a_st[3] - a_st[2]) + 0.5f*(a_st[4] + a_st[5]),
        a_st[6], // 11 is the same
        a_st[7],
    };
    for (uint8_t i = 0; i < 4; i++) {
        __rect_to_polar__(b_st + 2*i, s_p[i]);
    }
}

void _cnot(uint8_t** s_p) {
    uint64_t a4 = GET(s_p[2], 0);
    uint64_t a5 = GET(s_p[2], 1);
    SET(s_p[2], 0, GET(s_p[3], 0));
    SET(s_p[2], 1, GET(s_p[3], 1));
    SET(s_p[3], 0, a4);
    SET(s_p[3], 1, a5);
}

void _cpshift(uint8_t** s_p, float phase) {
    if (GET(s_p[3], 0)) {
        SET(s_p[3], 1, GET(s_p[3], 1) + (phase * ANGLE_BASE) / (2.0f * M_PI));  // simply add the angles
    }
}

void _ccnot(uint8_t** s_p) {
    uint64_t a12 = GET(s_p[6], 0);
    uint64_t a13 = GET(s_p[6], 1);
    SET(s_p[6], 0, GET(s_p[7], 0));
    SET(s_p[7], 1, GET(s_p[7], 1));
    SET(s_p[6], 0, a12);
    SET(s_p[7], 1, a13);
}

void _cswap(uint8_t** s_p) {
    uint64_t a10 = GET(s_p[5], 0);
    uint64_t a11 = GET(s_p[5], 1);
    SET(s_p[5], 0, GET(s_p[5], 0));
    SET(s_p[5], 1, GET(s_p[5], 0));
    SET(s_p[6], 0, a10);
    SET(s_p[6], 1, a11);
}
