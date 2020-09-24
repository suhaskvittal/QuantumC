//
//  main.c
//  QuantumC
//
//  Created by Suhas Vittal on 9/6/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "qubit.h"
#include "gates.h"
#include "storage.h"
#include "qft.h"

void __print_qubit__(struct qubit_sys* q_p) {
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

int __compress_bell_state__() {
    printf("----------------MATH TEST---------------\n");
    
    // bell state: 1/rt(2) [00 + 11]
    
    uint64_t inv_rt2_b254 = _to_b2f(sqrtf(0.5f));
    printf("1/sqrt(2)\t--b2f-> %llx (%f) \n",
           inv_rt2_b254, _from_b2f(inv_rt2_b254));
    printf("0.5\t\t--b2f-> %llx (%f) \n",
           _to_b2f(0.5f), _from_b2f(_to_b2f(0.5f)));
    uint64_t inv_rt2_sqr_b254 =
        __b2fmul__(inv_rt2_b254, inv_rt2_b254);
    printf("b254 sqr(rt(2))\t\t %llx (%f)\n",
           inv_rt2_sqr_b254, _from_b2f(inv_rt2_sqr_b254));
    uint64_t bell_state_p = __b2fadd__(inv_rt2_sqr_b254, inv_rt2_sqr_b254);
    printf("P(00) + P(11)\t\t %llx (%f)\n", bell_state_p, _from_b2f(bell_state_p));
    
    printf("----------------------------------------\n");
    return 0;
}

int __test_ent__() {
    struct qubit_sys* a = malloc(1 * sizeof(struct qubit_sys));
    a->state_p = calloc(STATES_TO_BYTES(2), sizeof(uint8_t));
    a->n_qubits = 1;
    
    struct qubit_sys* b = malloc(1 * sizeof(struct qubit_sys));
    b->state_p = calloc(STATES_TO_BYTES(2), sizeof(uint8_t));
    b->n_qubits = 1;
    
    SET(a->state_p, 0, _to_b2f(1.0f));  // |0>
    SET(b->state_p, 0, _to_b2f(1.0f));  // |1>
    
    printf("-------------ENTANGLEMENT TEST---------------\n");
    
    /* We can create a Bell state by doing the
     following operation:
        CNOT(H|0>, |1>)
     This yields a Bell state.
     */
    
    printf("A:\t\t"); __print_qubit__(a);
    printf("B:\t\t"); __print_qubit__(b);
    apply_gate(a, &_hadamard);
    printf("H(A):\t\t"); __print_qubit__(a);
    struct qubit_sys* ab = tensor_product(a, b);  // a and b have been freed
    printf("AB:\t\t"); __print_qubit__(ab);
    apply_gate(ab, &_cnot);
    
    /* This final output should yield a representation of
     1/rt(2) in the 1st and 4th state of ab. To see the
     decimal representation, use the _from_b2f function. */
    
    printf("CNOT(AB):\t"); __print_qubit__(ab);
    
    printf("---------------------------------------------\n");
    
    QFREE(ab);
    
    return 0;
}

int __uniform_test__() {
    struct qubit_sys* q = create_uniform(4);
    printf("---------------UNIFORM TEST------------------\n");

    printf("Q:\t\t"); __print_qubit__(q);
    
    printf("---------------------------------------------\n");
    
    QFREE(q);
    
    return 0;
}

int __qft_test__() {
    struct qubit_sys* q = create_uniform(4);
    
    printf("------------------QFT TEST-------------------\n");
    
    printf("Q:\t\t"); __print_qubit__(q);
    qft(q);
    printf("QFT(Q):\t\t"); __print_qubit__(q);
    iqft(q);
    printf("IQFT(QFT(Q)):\t"); __print_qubit__(q);
    printf("---------------------------------------------\n");
    
    QFREE(q);
    
    return 0;
}

int __qsz_test__() {
    struct qubit_sys* q = create_qubit_sys(30);
    
    printf("q addr:\t\t%p\n", q);
    printf("s_p addr:\t%p\n", q->state_p);
    
    QFREE(q);
    return 0;
}

int main(int argc, const char * argv[]) {
    G_QUBIT_PRECISION = QUBIT_8_16;
    
    printf("MODULUS_BASE: %llu\n", MODULUS_BASE);
    printf("ARGUMENT BASE: %llu\n", ANGLE_BASE);
    
    // __test_ent__();
    // __uniform_test__();
    // __qft_test__();
    __qsz_test__();
    
    //printf("Press any key to exit...\n");
    //getchar();
    
    return 0;
}
