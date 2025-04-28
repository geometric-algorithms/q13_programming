#!/bin/bash

g++ -std=c++17 -O2 -o main main.cpp fortune.h point.h arc.h event.h || exit 1
./main input.txt || exit 1
python3 plot_voronoi.py || exit 1
