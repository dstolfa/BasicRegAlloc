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
