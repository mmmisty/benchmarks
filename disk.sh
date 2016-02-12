# !/bin/bash

g++ -c disk.cpp -pthread -std=c++11
g++ disk.o -o disk.out -pthread -std=c++11

./disk.out 0 0 0 1 26 5 file0.bin
./disk.out 0 0 0 2 25 5 file0.bin
./disk.out 0 0 1 1 16 5 file0.bin
./disk.out 0 0 1 2 15 5 file0.bin
./disk.out 0 0 2 1 8 5 file0.bin
./disk.out 0 0 2 2 7 5 file0.bin
./disk.out 0 1 0 1 22 5 file0.bin
./disk.out 0 1 0 2 21 5 file0.bin
./disk.out 0 1 1 1 18 5 file0.bin
./disk.out 0 1 1 2 18 5 file0.bin
./disk.out 0 1 2 1 9 5 file0.bin
./disk.out 0 1 2 2 9 5 file0.bin
./disk.out 1 0 0 1 15 5 file0.bin
./disk.out 1 0 0 2 14 5 file0.bin
./disk.out 1 0 1 1 15 5 file0.bin
./disk.out 1 0 1 2 12 5 file0.bin
./disk.out 1 0 2 1 10 5 file0.bin
./disk.out 1 0 2 2 9 5 file0.bin
./disk.out 1 1 0 1 20 5 file0.bin
./disk.out 1 1 0 2 20 5 file0.bin
./disk.out 1 1 1 1 20 5 file0.bin
./disk.out 1 1 1 2 20 5 file0.bin
./disk.out 1 1 2 1 14 5 file0.bin
./disk.out 1 1 2 2 13 5 file0.bin

