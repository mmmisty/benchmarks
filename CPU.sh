# !/bin/bash

g++ -c CPU.cpp -pthread -std=c++11
g++ CPU.o -o CPU.out -pthread -std=c++11

./CPU.out 0 1 8 5
./CPU.out 0 2 8 5
./CPU.out 0 4 8 5
./CPU.out 1 1 8 5
./CPU.out 1 2 8 5
./CPU.out 1 4 8 5
./CPU.out 0 1 8 600
./CPU.out 1 1 8 600



