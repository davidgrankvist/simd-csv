#!/usr/bin/sh

mkdir -p bin

gcc src/*.c -I src/ -mavx2 -g -o bin/simd-csv
