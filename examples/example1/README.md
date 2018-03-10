# example1

## Description

This example exercises the register allocator in a fairly generic way. It takes
an input in the form of:

```
k : Integer -> number of registers
n : Integer -> number of inputs
```

This is followed by `n` inputs in the format of:

```
id        : Integer -> virtual register identifier (starting at 1)
liveBegin : Integer -> when the virtual register becomes live
liveEnd   : Integer -> when the virtual register is referenced and removed from
                       the live set
weight    : Integer -> the "weight" parameter in the register allocator
```

The program will then construct a clash graph using this information and
pretty-print it in the following format:

```
Element 1 = | n1 | n2 | ... | ni |
Element 2 = | ... |
...
Element n = | ... |
```

where the sequence of 1 ... n is the sequence of all identifiers in the clash
graph. Each element n1 ... ni represents the neighbours of that particular node
in the clash graph (clashes).

The final result of the example is a list of relations in the form of
`(id, register)`, denoting the register that was allocated to a particular
virtual register with the identifier of `id`. If the value was spilled in some
abstract spill slot (for example the stack), the output will represent this as
-1:

```
(1, 0)
(2, 1)
(3, -1)
```
