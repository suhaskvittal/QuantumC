# QuantumC

An open-source library for quantum simulation written in C.

*Created by Suhas Vittal in September 2020.*

# Contents
   1. [Background](#background)
       1. [Qubits and States](##qubits-and-states) 
       2. [The Hadamard (H) Gate](##the-hadamard-(h)-gate)
       3. [Onto Two Qubits and the CNOT Gate](##onto-two-qubits-and-the-cnot-gate)
       4. [Entanglement](##entanglement)
       5. [The Quantum Advantage](##the-quantum-advantage)
   2. [Implementation](#implementation)
   3. [Features](#features)
   4. [Usage](#usage)
       1. [First Way](##first-way)
       2. [Second Way](##second-way)
   5. [Documentation](#documentation)
   6. [Download](#download)
   7. [Future Work](#future-work)
   8. [License](#license)

# Background
We've all heard about quantum computing as a solution to hard problems for classical computers, but how exactly does quantum computing work?

## Qubits and States

Quantum computing, like classical computing, has a basic unit of information: the qubit. You've likely heard qubits described capable of being a 0, a 1, or both simultaneously. Well, that's not exactly true. To fully describe a qubit, we have to start with what a state is.

A quantum state is a vector that represents some outcome. We often represent a quantum state as <img src="svgs/a32964b094548d7cd5ce51a1dca1adb0.svg?invert_in_darkmode" valign=-4.109589000000009px width=30.93042644999999pt height=16.438356pt/>, where <img src="svgs/c87ca14efe98f13246c068238f27f47b.svg?invert_in_darkmode" valign=-0.6427030499999994px width=42.53981489999999pt height=11.966898899999999pt/> and <img src="svgs/187d28095d3e7eb75b9e982c84d0ad55.svg?invert_in_darkmode" valign=-4.109589000000009px width=20.35392809999999pt height=16.438356pt/> is called a *ket*. Mathematically, we can say <img src="svgs/187d28095d3e7eb75b9e982c84d0ad55.svg?invert_in_darkmode" valign=-4.109589000000009px width=20.35392809999999pt height=16.438356pt/> is a vector with a <img src="svgs/034d0a6be0424bffe9a6e7ac9236c0f5.svg?invert_in_darkmode" valign=0.0px width=8.219209349999991pt height=10.5936072pt/> in the <img src="svgs/332cc365a4987aacce0ead01b8bdcc0b.svg?invert_in_darkmode" valign=0.0px width=9.39498779999999pt height=7.0776222pt/>-th position, but we can leave that as an implementation detail. Finally, a singular qubit is just a pair of quantum states: 

<p align="center"><img src="svgs/9604e0bf88a70d3ba47cbf7cac939881.svg?invert_in_darkmode" valign=0.0px width=79.1895225pt height=16.438356pt/></p>

We call this a superposition of states.

Now, when people say a qubit can be a 0 and a 1 simultaneously, this is a half-lie. We call a qubit a superposition of the two states <img src="svgs/20c95f3e6acae5fea447f699387cf0d2.svg?invert_in_darkmode" valign=-4.109589000000009px width=19.178149649999988pt height=16.438356pt/> and <img src="svgs/df71a1c14c4aef839456351e93f7e141.svg?invert_in_darkmode" valign=-4.109589000000009px width=19.178149649999988pt height=16.438356pt/>. When we measure a superposition of states, we have a probability of measuring the qubit as either in state <img src="svgs/20c95f3e6acae5fea447f699387cf0d2.svg?invert_in_darkmode" valign=-4.109589000000009px width=19.178149649999988pt height=16.438356pt/> or state <img src="svgs/df71a1c14c4aef839456351e93f7e141.svg?invert_in_darkmode" valign=-4.109589000000009px width=19.178149649999988pt height=16.438356pt/>. After measuring a qubit, we can't measure the qubit and hope for a new result! Measurement is final.

So, what are these probabilities? That's where the complex numbers come in -- the probability is the absolute value squared. For our qubit, the probability of measuring a state of 0 is <img src="svgs/0379db287bec2c744311525f57fb12b2.svg?invert_in_darkmode" valign=-4.109589000000009px width=26.261492399999987pt height=17.4904653pt/> and the probability of measuring a state of 1 is <img src="svgs/bfdaa6f6510a823b9bf1e215af9aee0a.svg?invert_in_darkmode" valign=-4.109589000000009px width=25.850526899999988pt height=17.4904653pt/>. Finally, there is one final constraint -- the sum of these probabilities must equal 1, so <img src="svgs/9b6a8cd85dd4007b9e507ab9260efb3b.svg?invert_in_darkmode" valign=-5.013835199999992px width=120.42223379999999pt height=19.726228499999998pt/>. This is often called the *2-norm*.

## The Hadamard (H) Gate
Now, let's consider an operation on a single qubit. One often used quantum gate is the Hadamard gate, which is often denoted as an <img src="svgs/7b9a0316a2fcd7f01cfd556eedf72e96.svg?invert_in_darkmode" valign=0.0px width=14.99998994999999pt height=11.232861749999998pt/> in quantum circuit diagrams. As a qubit is simply a sum of two vectors, a quantum gate is just a matrix operation. In fact, because every gate is simply a matrix operation, this makes quantum gates *reversible*, which is a property not found in most classical gates. For example, given the output of a classical OR or AND gate, you can't determine the inputs. However, for a quantum gate, you can simply compute a matrix inverse. But even this is not required, because some gates are self-invertible! In fact, the only part of quantum computing that is irreversible is measurement.

The Hadamard gate is an example of a self-invertible gate. The matrix for the Hadamard gate is written as:
<p align="center"><img src="svgs/ccb4d3c4ef9866c9eed40d36591748b5.svg?invert_in_darkmode" valign=0.0px width=106.78081424999999pt height=39.452455349999994pt/></p>
We can describe the output of <img src="svgs/7b9a0316a2fcd7f01cfd556eedf72e96.svg?invert_in_darkmode" valign=0.0px width=14.99998994999999pt height=11.232861749999998pt/> as two maps:
1. <img src="svgs/47e475e67887b7a84d39ad0b930a8e9d.svg?invert_in_darkmode" valign=-8.942346600000008px width=137.27854635pt height=22.830173849999998pt/>
2. <img src="svgs/6e73d0dda075bd37cf631c3b190ee89a.svg?invert_in_darkmode" valign=-8.942346600000008px width=137.27854635pt height=22.830173849999998pt/>
   
So, if we want to compute what happens to a qubit 
<p align="center"><img src="svgs/30c639dac4bbb7e6090c86d514124b8a.svg?invert_in_darkmode" valign=0.0px width=124.85152679999999pt height=16.438356pt/></p> 
after applying <img src="svgs/7b9a0316a2fcd7f01cfd556eedf72e96.svg?invert_in_darkmode" valign=0.0px width=14.99998994999999pt height=11.232861749999998pt/>, then we can simply do a few substitutions. Then we get:
<p align="center"><img src="svgs/71250ab6f5cecef66cf8d74a72704d8c.svg?invert_in_darkmode" valign=0.0px width=208.66624844999998pt height=37.8236826pt/></p>
And if you apply <img src="svgs/7b9a0316a2fcd7f01cfd556eedf72e96.svg?invert_in_darkmode" valign=0.0px width=14.99998994999999pt height=11.232861749999998pt/> to <img src="svgs/e499c73396fbebb9bb6efc3289a97301.svg?invert_in_darkmode" valign=-4.109589000000009px width=38.744342999999986pt height=16.438356pt/>, you just get <img src="svgs/535b15667b86f1b118010d4c218fecb9.svg?invert_in_darkmode" valign=0.0px width=12.785434199999989pt height=11.232861749999998pt/> again because <img src="svgs/7b9a0316a2fcd7f01cfd556eedf72e96.svg?invert_in_darkmode" valign=0.0px width=14.99998994999999pt height=11.232861749999998pt/> is the inverse of itself.

You may be thinking: this isn't so bad! However, when you start adding multiple qubits in, this becomes more complicated. However, this complication is also where the power of quantum computing comes from.

## Onto Two Qubits and the CNOT Gate
Let's up the ante: now we have two qubits <img src="svgs/7ee79316e2c8646be6351bb6c682673b.svg?invert_in_darkmode" valign=-4.109589000000009px width=146.04648629999997pt height=16.438356pt/> and <img src="svgs/f5e909152bb6d9a338f871538fe27a8a.svg?invert_in_darkmode" valign=-4.109589000000009px width=146.04648629999997pt height=16.438356pt/>. We can do all the same things as we did before: we can apply a gate like the Hadamard gate to both qubits, and it works without a catch. We can also do one more "operation" (but this more of a mathematical convenience), which is the *tensor product*. The tensor product of two qubits is just 

<p align="center"><img src="svgs/c2cdd0d24a4daf77783806bc6872f596.svg?invert_in_darkmode" valign=0.0px width=381.3094923pt height=16.438356pt/></p>

Make no mistake, <img src="svgs/3e7ca040ea2e45ffa7f4444ce24c7fa9.svg?invert_in_darkmode" valign=-4.109589000000009px width=27.39735899999999pt height=16.438356pt/>, <img src="svgs/e79978c03d8d6a8a7ddd602f7a627a21.svg?invert_in_darkmode" valign=-4.109589000000009px width=27.39735899999999pt height=16.438356pt/>, <img src="svgs/a46fcb7e2c27e13141eefbd0da5721ae.svg?invert_in_darkmode" valign=-4.109589000000009px width=27.39735899999999pt height=16.438356pt/>, and <img src="svgs/71094dee2d0de951d6b3b08611702f25.svg?invert_in_darkmode" valign=-4.109589000000009px width=27.39735899999999pt height=16.438356pt/> are all states, so the 2-norm must equal 1. This new superposition represents each possible pair of states we can obtain during measurement. We can perform a tensor product between three qubits to get triplets of states, four qubits to get quadruplets, etc. The tensor product of <img src="svgs/55a049b8f161ae7cfeb0197d75aff967.svg?invert_in_darkmode" valign=0.0px width=9.86687624999999pt height=7.0776222pt/> qubits is known as an *<img src="svgs/55a049b8f161ae7cfeb0197d75aff967.svg?invert_in_darkmode" valign=0.0px width=9.86687624999999pt height=7.0776222pt/>-qubit system*.

Nevertheless, let's introduce a two qubit gate: the controlled NOT, often called CNOT, gate. One thing to note about quantum gates is that they have the same number of inputs as outputs -- after all, if they didn't, they wouldn't be reversible! So, the CNOT gate has two inputs and two outputs.

The CNOT gate performs flips the second qubit if the state of the first qubit is <img src="svgs/df71a1c14c4aef839456351e93f7e141.svg?invert_in_darkmode" valign=-4.109589000000009px width=19.178149649999988pt height=16.438356pt/>. So, the possible results are <img src="svgs/a19c327a3f4ebb27cc146824629a7093.svg?invert_in_darkmode" valign=-4.109589000000009px width=80.36531909999998pt height=16.438356pt/>, <img src="svgs/7b7e5fb3a345f736c0bc63399b9a4623.svg?invert_in_darkmode" valign=-4.109589000000009px width=80.36531909999998pt height=16.438356pt/>, <img src="svgs/5e731cec2d6d48e6292f61cb51168dfd.svg?invert_in_darkmode" valign=-4.109589000000009px width=80.36531909999998pt height=16.438356pt/>, and <img src="svgs/4edda6341c5a84c0c9f3991a96575646.svg?invert_in_darkmode" valign=-4.109589000000009px width=80.36531909999998pt height=16.438356pt/>. The CNOT gate is self-invertible. 

Now, with the CNOT gate and Hadamard gate, we can introduce some complications. Let's create a small quantum circuit with two qubits <img src="svgs/1cf9f2a84f7acbc240e37ad4b2555dd7.svg?invert_in_darkmode" valign=-4.109589000000009px width=72.21461444999998pt height=16.438356pt/> and <img src="svgs/f1b50922db0b9b1af48c2bf85bdbbb00.svg?invert_in_darkmode" valign=-4.109589000000009px width=72.21461444999998pt height=16.438356pt/>. First, let's perform a Hadamard gate on the first qubit to get <img src="svgs/f3ef08e1c56c8cb756e7faa27d5e3c7f.svg?invert_in_darkmode" valign=-8.942346600000008px width=148.3059171pt height=22.830173849999998pt/>. Now, let's apply the CNOT gate on our two qubits. The tensor product of our two qubits is just <img src="svgs/2f011f7f84a84216507099addaa48eb2.svg?invert_in_darkmode" valign=-8.942346600000008px width=184.90422884999998pt height=22.830173849999998pt/>. When we apply the CNOT gate to this 2-qubit system, we get:
<p align="center"><img src="svgs/abe168f42d7869912f0fac1d9972fb11.svg?invert_in_darkmode" valign=0.0px width=189.47042729999998pt height=37.0017615pt/></p>
Now, let's just break down our 2-qubit system into the original two qubits -- this should be easy... right?

## Entanglement

Hopefully, you haven't spend too much time trying to break down the system -- it is impossible to do so. When an <img src="svgs/55a049b8f161ae7cfeb0197d75aff967.svg?invert_in_darkmode" valign=0.0px width=9.86687624999999pt height=7.0776222pt/>-qubit system is irreducible to the product of <img src="svgs/55a049b8f161ae7cfeb0197d75aff967.svg?invert_in_darkmode" valign=0.0px width=9.86687624999999pt height=7.0776222pt/> qubits, the <img src="svgs/55a049b8f161ae7cfeb0197d75aff967.svg?invert_in_darkmode" valign=0.0px width=9.86687624999999pt height=7.0776222pt/> are *entangled*. The particular outcome we got is called a *Bell state*.

You've likely heard something about quantum entanglement before. What really is it? Entanglement, cursorily described, indicates high correlation in measurement. If we take a look at our 2-qubit system we made in the previous section, we have an equal chance of measuring <img src="svgs/3e7ca040ea2e45ffa7f4444ce24c7fa9.svg?invert_in_darkmode" valign=-4.109589000000009px width=27.39735899999999pt height=16.438356pt/> and <img src="svgs/71094dee2d0de951d6b3b08611702f25.svg?invert_in_darkmode" valign=-4.109589000000009px width=27.39735899999999pt height=16.438356pt/>. This means we can measure the first qubit as a 0 or a 1, and based on the outcome, we can say with high confidence that the second qubit is in the same state without measuring it.

So, what happens if we perform operations on qubits after entangling them? Let's go back to our 2-qubit system:
<p align="center"><img src="svgs/abe168f42d7869912f0fac1d9972fb11.svg?invert_in_darkmode" valign=0.0px width=189.47042729999998pt height=37.0017615pt/></p>
We may be unable to separate the system into 2 qubits, be we can still apply 1-qubit gates to the system: note that a state <img src="svgs/8e59b26e7f55742a4e82b2bd7b00af82.svg?invert_in_darkmode" valign=-4.109589000000009px width=90.88284149999998pt height=16.438356pt/>. So, apply a hadamard gate to the first qubit. We get:
<p align="center"><img src="svgs/ef03ecf28d9b13246d3acc56da9b4539.svg?invert_in_darkmode" valign=0.0px width=270.74885804999997pt height=32.990165999999995pt/></p>
This is still entangled, but single gates still work! The biggest takeaway, as mind-bending as entanglement can be, is that entanglement simply indicates extremely high correlation for measurement results.

## The Quantum Advantage
For this last part of the background section, I will talk about what is and isn't possible with a classical computer simulating quantum circuits.

## Memory

When we consider an <img src="svgs/55a049b8f161ae7cfeb0197d75aff967.svg?invert_in_darkmode" valign=0.0px width=9.86687624999999pt height=7.0776222pt/>-qubit system, we have at most <img src="svgs/f8f25e4580c418a51dc556db0d8d2b93.svg?invert_in_darkmode" valign=0.0px width=16.34523329999999pt height=10.91968515pt/> states as each qubit is at most a superposition of 2 states. So, when you add a qubit to a qubit system, you effectively double the memory footprint of the system. Due to this phenomenon, quantum computers have a massive advantage over classical computers: quantum computers don't need to keep track of entanglement. This defines the defining difference between classical and quantum bits: classical bits are always defined, quantum bits are never defined up until measurement.

## Time

When you examine quantum algorithms, you will often see multiple gates being performed in parallel. While parallel gate application is possible on a classical computer using multithreading, chances are you will be unable to perform many gates simultaneously. An example of this is seen in Grover's algorithm. Grovers's algorithm applies two gates <img src="svgs/4a4301b305cf393b7bd4bd50ac35462f.svg?invert_in_darkmode" valign=-4.703174849999989px width=18.92360579999999pt height=15.936036599999998pt/> and <img src="svgs/a7b075ff4fc9ccef2e9801d8a1d3a0f0.svg?invert_in_darkmode" valign=-2.4657286499999893px width=17.42810849999999pt height=13.698590399999999pt/> to <img src="svgs/f9c4988898e7f532b9f826a75014ed3c.svg?invert_in_darkmode" valign=0.0px width=14.99998994999999pt height=11.232861749999998pt/> states; each gate performs their operation across each state simultaneously. Because the algorithm has to get the right result, it has to apply the two gates <img src="svgs/2ca4b6fa01f356e6a956946f39f08a20.svg?invert_in_darkmode" valign=-1.8630661500000016px width=28.69866284999999pt height=16.438356pt/> times -- this is a process called *amplitude amplification*. 

Take into account these limitations as you explore quantum algorithms!

# Implementation
QuantumC uses represents quantum states with comparably less bits to other simulators like Microsoft's Q#. QuantumC represents each quantum state as a polar form complex number 
<p align="center"><img src="svgs/6818aa282d26d69935972e5b2486403f.svg?invert_in_darkmode" valign=0.0px width=153.029019pt height=39.452455349999994pt/></p>

where <img src="svgs/428a78f0b4509ab362c98b4a37a21e75.svg?invert_in_darkmode" valign=-4.109589000000009px width=119.15505524999998pt height=17.1982437pt/>, and <img src="svgs/d585f3b61e4d94d51743eaf786356b90.svg?invert_in_darkmode" valign=-2.235141150000005px width=70.99670984999999pt height=13.650669449999999pt/>. Specifically, <img src="svgs/89f2e0d2d24bcf44db73aab8fc03252c.svg?invert_in_darkmode" valign=0.0px width=7.87295519999999pt height=7.0776222pt/> is represented as a modified half-precision floating point integer with 5 exponent bits and 11 (+1) mantissa bits, and <img src="svgs/55a049b8f161ae7cfeb0197d75aff967.svg?invert_in_darkmode" valign=0.0px width=9.86687624999999pt height=7.0776222pt/> is a 16-bit positive integer. This mean that each quantum state is 32 bits, so a system of 30 qubits can be represented in exactly 4 GiB, which is much less than the 16 GiB required by Q#. The largest system representable with 16 GiB using QuantumC is 32 qubits.

# Features
QuantumC implements 12 elementary gates along with the QFT and QFT<img src="svgs/2351d2e88576cfc62e0f3db8a8ab779c.svg?invert_in_darkmode" valign=0.0px width=16.82656799999999pt height=13.380876299999999pt/> gates. These gates are

1. Single-qubit gates: Hadamard, Pauli-X (NOT), Pauli-Y, Pauli-Z, Phase Shift, and Square Root of NOT.
2. Double-qubit gates: Controlled NOT (CNOT), Swap, Square Root of Swap, and Controlled Phase Shift.
3. Triple-qubit gates: Toffoli (Controlled CNOT, or CCNOT), and Controlled Swap (CSWAP).

There is also an implementation of Quantum Fourier Transform (QFT) and inverse Quantum Fourier Transform (QFT<img src="svgs/2351d2e88576cfc62e0f3db8a8ab779c.svg?invert_in_darkmode" valign=0.0px width=16.82656799999999pt height=13.380876299999999pt/>).

# Usage

Let's try to create an entangled 2-qubit system. We will need to include:

```c
#include "qubit.h"
#include "gates.h"
```

## First Way

First we must declare our 2-qubit system, with each bit initialized to 0. We can use the `create_qubit_sys` function.

```c
struct qubit_sys* q = create_qubit_sys(2);
```
Then, we will apply the Hadamard gate onto the first qubit using the `p_apply_gate` function, which applies a gate to a sub-system of the system:

```c
uint8_t i = 0;
p_apply_gate(q, &i, 1, &_hadamard);
```

and then finally apply a CNOT gate onto the system using the `apply_gate` function:

```c
apply_gate(q, &_cnot);
```

Finally, we print our qubit and free our system:
```c
print_qubit(q);
QFREE(q);
```
which will output the hexadecimal values of each state. We should get 
`fda8|0000 0000|0000 0000|0000 fda8|0000` as our output, which indicates a coefficient of <img src="svgs/3229ead1c7afe860d0500b3cbd5e94d2.svg?invert_in_darkmode" valign=-8.942346600000008px width=17.351680499999997pt height=22.830173849999998pt/> for the states <img src="svgs/3e7ca040ea2e45ffa7f4444ce24c7fa9.svg?invert_in_darkmode" valign=-4.109589000000009px width=27.39735899999999pt height=16.438356pt/> and <img src="svgs/71094dee2d0de951d6b3b08611702f25.svg?invert_in_darkmode" valign=-4.109589000000009px width=27.39735899999999pt height=16.438356pt/>.

## Second Way
But I think using the `p_apply_gate` function is cumbersome. The only time the `p_apply_gate` should be used is if the system is already entangled or when dealing with memory-sensitive situations. We can do the above with much less hassle by using the `tensor_product` function:

```c
struct qubit_sys* a = create_zero();
struct qubit_sys* b = create_zero();
apply_gate(a, &_hadamard);
// note that tensor_product frees the inputs.
struct qubit_sys* ab = tensor_product(a, b);  
apply_gate(ab, &_cnot);
print_qubit(ab);
QFREE(ab);
```

We can also shorten the above code further by using the `create_uniform` function instead of a hadamard gate:

```c
/* create_uniform creates a qubit system 
such that each state has equal probability.*/
struct qubit_sys* a = create_uniform(1);
struct qubit_sys* b = create_zero();
// note that tensor_product frees the inputs.
struct qubit_sys* ab = tensor_product(a, b);  
apply_gate(ab, &_cnot);
print_qubit(ab);
QFREE(ab);
```

This creates the same entangled 2-qubit system, without the hassle of using `p_apply_gate`. The tradeoff is in the memory footprint. The `tensor_product` function requires <img src="svgs/27bab83a05e94a7c15a396201e3dcd57.svg?invert_in_darkmode" valign=-4.109589000000009px width=50.080834649999986pt height=16.438356pt/> additional memory, where <img src="svgs/55a049b8f161ae7cfeb0197d75aff967.svg?invert_in_darkmode" valign=0.0px width=9.86687624999999pt height=7.0776222pt/> and <img src="svgs/0e51a2dede42189d77627c4d742822c3.svg?invert_in_darkmode" valign=0.0px width=14.433101099999991pt height=7.0776222pt/> are the memory footprints of the first and second variables, because it has to dynamically allocate the output system. However, `p_apply_gate` takes longer as it must iterate over multiple tuples in the system, but has no memory footprint.

## Heuristics
The file `main.c` has a few functions that provide code examples as well as other tests, like converting between single-precision floats and the modified half-precision float used throughout QuantumC. Unix and Linux users can create an executable for `main.c` by calling `make test`. This also builds the binaries in the `bin` folder.

# Documentation
A documentation readme is located at `QuantumC/QuantumC/DOCS.md` (eventually).

# Download
Working on creating a library script. For now, just build the binaries and copy the header files into your project.

# Future Work
The current library implements the Schrödinger algorithm for quantum simulation. There are other algorithms, which I plan to implement so the library has more versatility. Other places for improvement are:

1. Increasing the accuracy of the complex number data representation.
2. Adding in more gates. 
3. Optimizing current algorithms.

Contributions are always welcome.

# License
Likely MIT or GPL.
