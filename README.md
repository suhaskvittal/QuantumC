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

A quantum state is a vector that represents some outcome. We often represent a quantum state as $\alpha \lvert x \rangle$, where $\alpha \in \mathbb{C}$ and $\lvert x \rangle$ is called a *ket*. Mathematically, we can say $\lvert x \rangle$ is a vector with a $1$ in the $x$-th position, but we can leave that as an implementation detail. Finally, a singular qubit is just a pair of quantum states: 

$$ \alpha \lvert 0 \rangle + \beta \lvert 1 \rangle$$

We call this a superposition of states.

Now, when people say a qubit can be a 0 and a 1 simultaneously, this is a half-lie. We call a qubit a superposition of the two states $\lvert 0 \rangle$ and $\lvert 1 \rangle$. When we measure a superposition of states, we have a probability of measuring the qubit as either in state $\lvert 0 \rangle$ or state $\lvert 1 \rangle$. After measuring a qubit, we can't measure the qubit and hope for a new result! Measurement is final.

So, what are these probabilities? That's where the complex numbers come in -- the probability is the absolute value squared. For our qubit, the probability of measuring a state of 0 is $|\alpha|^2$ and the probability of measuring a state of 1 is $|\beta|^2$. Finally, there is one final constraint -- the sum of these probabilities must equal 1, so $\sqrt{|\alpha|^2 + |\beta|^2} = 1$. This is often called the *2-norm*.

## The Hadamard (H) Gate
Now, let's consider an operation on a single qubit. One often used quantum gate is the Hadamard gate, which is often denoted as an $H$ in quantum circuit diagrams. As a qubit is simply a sum of two vectors, a quantum gate is just a matrix operation. In fact, because every gate is simply a matrix operation, this makes quantum gates *reversible*, which is a property not found in most classical gates. For example, given the output of a classical OR or AND gate, you can't determine the inputs. However, for a quantum gate, you can simply compute a matrix inverse. But even this is not required, because some gates are self-invertible! In fact, the only part of quantum computing that is irreversible is measurement.

The Hadamard gate is an example of a self-invertible gate. The matrix for the Hadamard gate is written as:
$$ H = \begin{pmatrix}
    1 & 1 \\
    1 & -1
\end{pmatrix}
$$
$H$ maps $\lvert 0 \rangle$ to $\frac{1}{\sqrt{2}}\left( \lvert 0 \rangle + \lvert 1 \rangle \right)$ and $\lvert 1 \rangle$ to $\frac{1}{\sqrt{2}}\left( \lvert 0 \rangle - \lvert 1 \rangle \right)$. So, if we want to compute what happens to a qubit $\lvert \Psi \rangle = \alpha \lvert 0 \rangle + \beta \lvert 1 \rangle$ after applying $H$ to $\lvert \Psi \rangle$, then we can simply do a few substitutions. Then we get:
$$
    H \lvert \Psi \rangle = \frac{\alpha + \beta}{\sqrt{2}} \lvert 0 \rangle + \frac{\alpha - \beta}{\sqrt{2}} \lvert 1 \rangle
$$
And if you apply $H$ to $H \lvert \Psi \rangle$, you just get $\Psi$ again because $H$ is the inverse of itself.

You may be thinking: this isn't so bad! However, when you start adding multiple qubits in, this becomes more complicated. However, this complication is also where the power of quantum computing comes from.

## Onto Two Qubits and the CNOT Gate
Let's up the ante: now we have two qubits $\lvert \Psi_0 \rangle = \alpha_0 \lvert 0 \rangle + \beta_0 \lvert 1 \rangle$ and $\lvert \Psi_1 \rangle = \alpha_1 \lvert 0 \rangle + \beta_1 \lvert 1 \rangle$. We can do all the same things as we did before: we can apply a gate like the Hadamard gate to both qubits, and it works without a catch. We can also do one more "operation" (but this more of a mathematical convenience), which is the *tensor product*. The tensor product of two qubits is just 

$$ \lvert \Psi_0 \Psi_1 \rangle = \alpha_0\alpha_1 \lvert 00 \rangle + \alpha_0\beta_1 \lvert 01 \rangle + \beta_0\alpha_1 \lvert 10 \rangle + \beta_0\beta_1 \lvert 11 \rangle$$

Make no mistake, $\lvert 00 \rangle$, $\lvert 01 \rangle$, $\lvert 10 \rangle$, and $\lvert 11 \rangle$ are all states, so the 2-norm must equal 1. This new superposition represents each possible pair of states we can obtain during measurement. We can perform a tensor product between three qubits to get triplets of states, four qubits to get quadruplets, etc. The tensor product of $n$ qubits is known as an *$n$-qubit system*.

Nevertheless, let's introduce a two qubit gate: the controlled NOT, often called CNOT, gate. One thing to note about quantum gates is that they have the same number of inputs as outputs -- after all, if they didn't, they wouldn't be reversible! So, the CNOT gate has two inputs and two outputs.

The CNOT gate performs flips the second qubit if the state of the first qubit is $\lvert 1 \rangle$. So, the possible results are $\lvert 00 \rangle \mapsto \lvert 00 \rangle$, $\lvert 01 \rangle \mapsto \lvert 01 \rangle$, $\lvert 10 \rangle \mapsto \lvert 11 \rangle$, and $\lvert 11 \rangle \mapsto \lvert 10 \rangle$. The CNOT gate is self-invertible. 

Now, with the CNOT gate and Hadamard gate, we can introduce some complications. Let's create a small quantum circuit with two qubits $\lvert \Psi_0 \rangle = \lvert 0 \rangle$ and $\lvert \Psi_1 \rangle = \lvert 0 \rangle$. First, let's perform a Hadamard gate on the first qubit to get $\lvert \Psi_0 \rangle = \frac{1}{\sqrt{2}} \left( \lvert 0 \rangle + \lvert 1 \rangle \right)$. Now, let's apply the CNOT gate on our two qubits. The tensor product of our two qubits is just $\lvert \Psi_0\Psi_1 \rangle = \frac{1}{\sqrt{2}} \left( \lvert 00 \rangle + \lvert 01 \rangle \right)$. When we apply the CNOT gate to this 2-qubit system, we get:
$$
\lvert \Psi_0\Psi_1 \rangle = \frac{1}{\sqrt{2}} \left( \lvert 00 \rangle + \lvert 11 \rangle \right)
$$
Now, let's just break down our 2-qubit system into the original two qubits -- this should be easy... right?

## Entanglement

Hopefully, you haven't spend too much time trying to break down the system -- it is impossible to do so. When an $n$-qubit system is irreducible to the product of $n$ qubits, the $n$ are *entangled*. The particular outcome we got is called a *Bell state*.

You've likely heard something about quantum entanglement before. What really is it? Entanglement, cursorily described, indicates high correlation in measurement. If we take a look at our 2-qubit system we made in the previous section, we have an equal chance of measuring $\lvert 00 \rangle$ and $\lvert 11 \rangle$. This means we can measure the first qubit as a 0 or a 1, and based on the outcome, we can say with high confidence that the second qubit is in the same state without measuring it.

So, what happens if we perform operations on qubits after entangling them? Let's go back to our 2-qubit system:
$$
\lvert \Psi_0\Psi_1 \rangle = \frac{1}{\sqrt{2}} \left( \lvert 00 \rangle + \lvert 11 \rangle \right)
$$
We may be unable to separate the system into 2 qubits, be we can still apply 1-qubit gates to the system: note that a state $\lvert xy \rangle = \lvert x \rangle \lvert y \rangle$. So, apply a hadamard gate to the first qubit. We get:
$$
\lvert \Psi_0\Psi_1 \rangle = \frac{1}{2} \left( \lvert 00 \rangle + \lvert 01 \rangle + \lvert 10 \rangle - \lvert 11 \rangle \right)
$$
This is still entangled, but single gates still work! The biggest takeaway, as mind-bending as entanglement can be, is that entanglement simply indicates extremely high correlation for measurement results.

## The Quantum Advantage
For this last part of the background section, I will talk about what is and isn't possible with a classical computer simulating quantum circuits.

## Memory

When we consider an $n$-qubit system, we have at most $2^n$ states as each qubit is at most a superposition of 2 states. So, when you add a qubit to a qubit system, you effectively double the memory footprint of the system. Due to this phenomenon, quantum computers have a massive advantage over classical computers: quantum computers don't need to keep track of entanglement. This defines the defining difference between classical and quantum bits: classical bits are always defined, quantum bits are never defined up until measurement.

## Time

When you examine quantum algorithms, you will often see multiple gates being performed in parallel. While parallel gate application is possible on a classical computer using multithreading, chances are you will be unable to perform many gates simultaneously. An example of this is seen in Grover's algorithm. Grovers's algorithm applies two gates $U_f$ and $U_s$ to $N$ states; each gate performs their operation across each state simultaneously. Because the algorithm has to get the right result, it has to apply the two gates $\sqrt{N}$ times -- this is a process called *amplitude amplification*. 

Take into account these limitations as you explore quantum algorithms!

# Implementation
QuantumC uses represents quantum states with comparably less bits to other simulators like Microsoft's Q#. QuantumC represents each quantum state as a polar form complex number 
$$\alpha = r \cdot \exp \left( \frac{2\pi i \cdot k}{n} \right)$$

where $r \in [0, 1], n \in \mathbb{Z}^+$, and $0 \leq k < n$. Specifically, $r$ is represented as a modified half-precision floating point integer with 5 exponent bits and 11 (+1) mantissa bits, and $n$ is a 16-bit positive integer. This mean that each quantum state is 32 bits, so a system of 30 qubits can be represented in exactly 4 GiB, which is much less than the 16 GiB required by Q#. The largest system representable with 16 GiB using QuantumC is 32 qubits.

# Features
QuantumC implements 12 elementary gates along with the QFT and QFT$^{-1}$ gates. These gates are

1. Single-qubit gates: Hadamard, Pauli-X (NOT), Pauli-Y, Pauli-Z, Phase Shift, and Square Root of NOT.
2. Double-qubit gates: Controlled NOT (CNOT), Swap, Square Root of Swap, and Controlled Phase Shift.
3. Triple-qubit gates: Toffoli (Controlled CNOT, or CCNOT), and Controlled Swap (CSWAP).

There is also an implementation of Quantum Fourier Transform (QFT) and inverse Quantum Fourier Transform (QFT$^{-1}$).

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
`fda8|0000 0000|0000 0000|0000 fda8|0000` as our output, which indicates a coefficient of $\frac{1}{\sqrt{2}}$ for the states $\lvert 00 \rangle$ and $\lvert 11 \rangle$.

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

This creates the same entangled 2-qubit system, without the hassle of using `p_apply_gate`. The tradeoff is in the memory footprint. The `tensor_product` function requires $O(nm)$ additional memory, where $n$ and $m$ are the memory footprints of the first and second variables, because it has to dynamically allocate the output system. However, `p_apply_gate` takes longer as it must iterate over multiple tuples in the system, but has no memory footprint.

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