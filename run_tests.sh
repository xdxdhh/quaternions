#!/bin/sh

cmake -S . -B build &&
cmake --build build &&
./build/test_quaternions


