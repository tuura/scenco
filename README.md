# SCENCO - SCEnario ENCOder #

[![Build Status](https://travis-ci.org/tuura/scenco.svg?branch=master)](https://travis-ci.org/tuura/scenco)

**SCENCO** is a crossplatform tool which provides functions for encoding,
synthesis and technology mapping for hardware systems described as behavioural
scenarios, in the form of conditional partial order graphs. The
[SCENCO](https://www.workcraft.org/scenco) toolsuite works in Windows, Linux and
OS X operating systems, and is provided as a part of the
[Workcraft](https://www.workcraft.org/start) distribution.

SCENCO interfaces with [Espresso](https://embedded.eecs.berkeley.edu/pubs/downloads/espresso/)
for synthesis of Boolean equations, with [Abc](https://people.eecs.berkeley.edu/~alanmi/abc/)
for their refactorisation and technology mapping. It includes several approaches
for scenario encoding:

* **sequential encoding**: scenarios are encoded sequentially;
* **random encoding**: scenarios are encoded randomly;
* **heuristic encoding**: an heuristics is used to encode scenarios in order to
minimise the derived microcontroller area;
* **single-literal encoding**: scenarios are encoded with the constraint that
each Boolean condition is composed by at most one literal;
* **SAT-based encoding**: a SAT-solver is used to minimise the derived
microcontroller area ([Clasp](https://potassco.org/clasp/) or [MiniSAT](http://minisat.se/)).
Look at the following link for more information:
[CPOG encodings](https://www.workcraft.org/help/encoding_plugin?s[]=cpog&s[]=encoding).

SCENCO has been tested with benchamarks of two types: *processor instruction sets*,
and *process mining traces*. Results can be found at the following web-page:
[Benchmarks](https://github.com/tuura/graph-families).

### Building from source
A Makefile is provided to simplify building process. Download SCENCO from Github:

`git clone https://github.com/tuura/scenco.git`

Type `make` to compile and build SCENCO from the source code.

Type `make help` to see the help of the Make.

Scenco is meant to be used inside Workcraft, so download it from
<https://www.workcraft.org/start> and start using it following the tutorials
presents in the website.
