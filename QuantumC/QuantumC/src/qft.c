//
//  qft.c
//  QuantumC
//
//  Created by Suhas Vittal on 9/14/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#include "../include/qft.h"

#include <stdio.h>
#include <math.h>

#include "../include/gates.h"

/* Observe that we can't write lambdas in C. Thus, we just
 declare a static global variable and modify that to change
 the output of the controlled phase shift gate. */

static uint8_t PHASE_SHIFT_M = 2;

struct qubit_sys* qft(struct qubit_sys* q) {
    PHASE_SHIFT_M = 2;
    
    for (uint8_t i = 0; i < q->n_qubits; i++) {
        p_apply_gate(q, &i, 1, &_hadamard);
        // apply a series of controlled phase shifts
        uint8_t pair[2] = { i, 0 };  // order doesn't matter, as only the 11 state is changed
        for (uint8_t j = i + 1; j < q->n_qubits; j++) {
            pair[1] = j;

            p_apply_gate(q, pair, 2, &__qft_cpshift__);            PHASE_SHIFT_M++;
        }
        
        PHASE_SHIFT_M = 2;
    }
    
    return q;
}

struct qubit_sys* iqft(struct qubit_sys* q) {
    PHASE_SHIFT_M = 2;
    
    for (uint8_t i = q->n_qubits - 1;; i--) {
        // apply a series of controlled phase shifts
        uint8_t pair[2] = { 0, i };
        for (uint8_t j = q->n_qubits - 1; j > i; j--) {
            pair[0] = j;
            p_apply_gate(q, pair, 2, &__iqft_cpshift__);
            PHASE_SHIFT_M--;
        }
        p_apply_gate(q, &i, 1, &_hadamard);
        
        PHASE_SHIFT_M = q->n_qubits - i + 1;
        // Note that unsigned ints can't go below 0! We do a manual break here.
        if (i == 0) { break; }
    }
    
    return q;
}

void __qft_cpshift__(uint8_t** s_p) {
    return _cpshift(s_p, (2.0f * M_PI) / (1 << PHASE_SHIFT_M));
}

void __iqft_cpshift__(uint8_t** s_p) {
    return _cpshift(s_p, (-2.0f * M_PI) / (1 << PHASE_SHIFT_M));
}




