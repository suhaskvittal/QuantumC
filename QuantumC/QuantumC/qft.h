//
//  qft.h
//  QuantumC
//
//  Created by Suhas Vittal on 9/14/20.
//  Copyright © 2020 Suhas Vittal. All rights reserved.
//

#ifndef qft_h
#define qft_h

#include "qubit.h"

struct qubit_sys* qft(struct qubit_sys*);
struct qubit_sys* iqft(struct qubit_sys*);

void __qft_cpshift__(uint8_t**);
void __iqft_cpshift__(uint8_t**);

#endif /* qft_h */
