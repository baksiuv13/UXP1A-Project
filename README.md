# UXP1A-Project
UNIX system architecture(UXP1A) project.

## Compile

Compile from workspace root with command:

    cmake CMakeList.txt
    make

This generate `client.out` file (CLI client to test Linda library) and test executable. BOOST_LIBRARY is required. To run test, use `make test`. 

## How to use client
To use `client.out`, at first argument set path to
shared memory-key file; if process is first and should
create this memory, set second argument to `-c`. For example:

    ./client.out shm-test-file -c

Only first client need `-c` flag.

When running, following commands are possible:

* `help` to see simple help
* `output <size:int> {<type:i|f|s> <value:int|float|string>}`
to make _output_ operation on given tuple
* `input <size:int> <timeout_sec:int> {<type:i|f|s> <cmp:==|<|<=|>|>=|*> [<value:int|float|string>]}`
for _input_ operation
* `read <size:int> <timeout_sec:int> {<type:i|f|s> <cmp:==|<|<=|>|>=|*> [<value:int|float|string>]}`
for _read_ operation

Pattern `<size:int> {<type:i|f|s> <value:int|float|string>}` means:
a tuple with __size__ elements, for each element: __type__ as `i` (int),
`f` (float) or `s` (string) and __value__ for this element. E.g.:

    output 4 i 1 s abc f 3.1415 s d

Is tuple `(1, "abc", 3.1415, "d")`