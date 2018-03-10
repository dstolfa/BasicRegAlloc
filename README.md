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

## Structure of the code

The current tree of the code is as follows:

```
.
├── README.md
├── examples
│   ├── example1
│   │   ├── main.cpp
│   │   ├── meson.build
│   │   ├── testfile
│   │   ├── testfile2
│   │   └── testfile3
│   └── meson.build
├── lib
│   ├── BasicRegAlloc.cpp
│   ├── BasicRegAlloc.h
│   └── meson.build
└── meson.build
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

## Build

This currently uses the [meson](http://mesonbuild.com) build system to achieve
simplicity of configuration and build files themselves. Meson is supported on
many major operating systems, including FreeBSD, Mac OS X, Linux and Windows. In
order to build the allocator, install meson and issue:

```
meson builddir --buildtype (plain|debug|debugoptimized|release)
```

This will generate a ninja file in builddir, which can be run to compile the
program. This code is written in C++14 and will not build without a compiler
that supports it.

## Usage

When you've built the library and the examples, you can run each example
individually. Currently, there are no tests written for the allocator so issuing
`ninja test` won't do anything.
