#!/bin/bash

g++ -std=c++17 -O2 -o main main.cpp point.h || exit 1
./main < input.txt > output.txt || exit 1
python3 visualize.py || exit 1
