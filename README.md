# BasicRegAlloc

## Explanation

This is a very simple register allocator that doesn't take into account
non-orthogonal instructions that may implicitly modify registers, nor does it
allow for specification of preference where things should be allocated. It is
based on the graph colouring approach with the following heuristics:

* The colour is picked as the first available colour using a set difference of
  an ordered set starting from 0 up to k.
* The spilling heuristic is defined as the element with the minimum weight
  (third parameter to the allocator). This allocator does not differentiate in
  ordering of the colouring depending on the spill happening or not.
* The colouring order is (typically) done starting from the node with the
  highest degree, which means that policy that is used to place nodes in the
  stack is the minimum degree. 
* The implementation of the clash graph is done via an adjacency matrix.

None of the code here should be relied on staying the same and is currently
intended as a standalone allocator that takes the in-order indices of virtual
registers (SSA or non-SSA), live ranges and weights.

Things to note about this particular register allocator is that it is an eager
one when it comes to reusing registers. That means that it will produce
dependencies between different instructions as they have been generated and in
pipelined implementations cause stalls. The assumption here is that the hardware
provides a sufficient amount of registers to be able to perform instruction
scheduling in the hardware itself even if there are dependencies between
instructions that reuse registers and seemingly cause a stall.

## Structure of the code

The current tree of the code is as follows:

```
.
├── examples
│   ├── example1
│   │   ├── main.cpp
│   │   ├── meson.build
│   │   ├── README.md
│   │   ├── testfile
│   │   ├── testfile2
│   │   └── testfile3
│   │   ...
│   └── meson.build
├── lib
│   ├── BasicRegAlloc.cpp
│   ├── BasicRegAlloc.h
│   └── meson.build
├── meson.build
├── README.md
└── tests
    ├── BasicRegisterAllocTest.h
    ├── inputs
    │   └── 1
    │       ...
    ├── meson.build
    └── no_overlapping_range
        ├── full-overlap-spill-one.cpp
        ├── meson.build
        ├── OverlapPolicy.cpp
        └── OverlapPolicy.h
    ...
```

The `lib` subdirectory contains the register allocation library. This is meant
to be as simple as possible of a library that provides the functionality of
register allocation in a straightforward way and is intended to be very
lightweight. In order to use it, include `BasicRegAlloc.h` in your source file
and link against the library. The graph colouring is done in
`BasicRegAlloc.cpp` as described in the explanation section.

The `examples` subdirectory contains various example programs built on the
library with various sample inputs for those programs. These are all built by
default and can be run from the command line for experimentation. Each example
has its own self-contained `README.md` that explains the purpose of the example
and how it operates.

All of the tests are located in the `tests` subdirectory. The tests are
organised in such a way that only a single property is checked with the tests in
each subdirectory under `tests`. This would mean that `no_overlapping_range`
tests whether or not any two or more virtual registers are allocated to the same
physical register at the same time even though their live ranges overlap.
`BasicRegisterAllocTest.h` implements a generic testing interface for
`BasicRegAlloc`, while the policy sources in each subdirectory implement the
verification and error reporting code. These two primitives should be used to
develop all of the future tests.

## Build

This currently uses the [meson](http://mesonbuild.com) build system to achieve
simplicity of configuration and build files themselves. Meson is supported on
many major operating systems, including FreeBSD, macOS, Linux and Windows. In
order to build the allocator, install meson and issue:

```
meson builddir --buildtype (plain|debug|debugoptimized|release)
```

This will generate a ninja file in builddir, which can be run to compile the
program. This code is written in C++17 and will not build without a compiler
that supports it. The build has only been tested with `clang++` and `ld.lld`
currently on FreeBSD and Linux. `ld.gold` should work, but `g++` might not.
Apple's `ld` should also work but `llvm` from [homebrew](https://brew.sh) should
be used, as `-lstdc++fs` is not supported in Apple's version of `clang`.

## Usage

When you've built the library and the examples, you can run each example
individually. In addition to that, running `ninja test` or `meson test` will
start running all of the tests currently implemented.
