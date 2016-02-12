# !/bin/bash

g++ -c disk.cpp -pthread -std=c++11
g++ disk.o -o disk.out -pthread -std=c++11

./disk.out 0 0 0 1 26 1 file0.bin
./disk.out 0 0 0 2 25 1 file0.bin
./disk.out 0 0 1 1 16 1 file0.bin
./disk.out 0 0 1 2 15 1 file0.bin
./disk.out 0 0 2 1 8 1 file0.bin
./disk.out 0 0 2 2 7 1 file0.bin
./disk.out 0 1 0 1 22 1 file0.bin
./disk.out 0 1 0 2 21 1 file0.bin
./disk.out 0 1 1 1 18 1 file0.bin
./disk.out 0 1 1 2 18 1 file0.bin
./disk.out 0 1 2 1 9 1 file0.bin
./disk.out 0 1 2 2 9 1 file0.bin
./disk.out 1 0 0 1 15 1 file0.bin
./disk.out 1 0 0 2 14 1 file0.bin
./disk.out 1 0 1 1 15 1 file0.bin
./disk.out 1 0 1 2 12 1 file0.bin
./disk.out 1 0 2 1 10 1 file0.bin
./disk.out 1 0 2 2 9 1 file0.bin
./disk.out 1 1 0 1 20 1 file0.bin
./disk.out 1 1 0 2 20 1 file0.bin
./disk.out 1 1 1 1 20 1 file0.bin
./disk.out 1 1 1 2 20 1 file0.bin
./disk.out 1 1 2 1 14 1 file0.bin
./disk.out 1 1 2 2 13 1 file0.bin

