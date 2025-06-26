# simd-csv

CSV parsing with SIMD.

## About

The purpose of this project is to learn more about SIMD (Single Instruction, Multiple Data) by applying it to CSV parsing.

SIMD is essentially a way for a CPU to do batch operations. You load a large register (for example 256 bits) with data,
operate on it in one go and then interpret the result as an operation on multiple values (for example 32 separate bytes).

In CSV parsing, one way you can use SIMD is to find all delimiters and newlines within a chunk of characters at once,
instead of looping over one character at a time.
