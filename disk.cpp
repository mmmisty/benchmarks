#include <iostream>
#include <stdio.h>
#include <math.h>
#include <list>
#include <thread>
#include <mutex>
#include "stdlib.h"
#include <string.h>
#include <unistd.h>

using namespace std;

list<double> durations;
mutex mu;

void sequence_write(int id, int size, int loop, char* fileName){
    char* mem  = (char *) malloc(size);
    FILE* pFile;
    pFile = fopen(fileName, "w");
    if(pFile == NULL) {
        cout << "File error." << endl;
        return;
    }
    clock_t start, end;
    start = clock();
    for (int i = 0; i < loop; ++i) {
        fwrite(mem, sizeof(char), size, pFile);
    }
    end = clock();
    double seconds = (double) (end - start) / CLOCKS_PER_SEC;

    mu.lock();
    durations.push_back(seconds);
    mu.unlock();

    fclose(pFile);
    free(mem);
}

void random_write(int id, int size, int loop, char* fileName){
    char* mem  = (char *) malloc(size);
    FILE* pFile;
    pFile = fopen(fileName, "w");
    if(pFile == NULL) {
        cout << "File error." << endl;
        return;
    }

    srand(time(NULL));

    clock_t start, end;
    start = clock();
    for (int i = 0; i < loop; ++i) {
        fwrite(mem, sizeof(char), size, pFile);
        long int pos = ftell(pFile);
        fseek(pFile, rand() % (pos+1), SEEK_SET);
    }
    end = clock();
    double seconds = (double) (end - start) / CLOCKS_PER_SEC;

    mu.lock();
    durations.push_back(seconds);
    mu.unlock();

    fclose(pFile);
    free(mem);
}

void sequence_read(int id, int size, int loop, char* fileName){
    char* mem  = (char *) malloc(size);
    FILE* pFile;

    char alterName[80];
    strcpy(alterName, fileName);
    alterName[strlen(fileName)] = id + '0';
    alterName[strlen(fileName) + 1] = 0;

//    cout << alterName << endl;

    pFile = fopen(alterName, "w");
    if(pFile == NULL) {
        cout << "File error." << endl;
        return;
    }
    for (int i = 0; i < loop; ++i) {
        fwrite(mem, sizeof(char), size, pFile);
    }
    fclose(pFile);

    pFile = fopen(alterName, "r");
    if (pFile == NULL) {
        cout << "File error." << endl;
        return;
    }
    // get the size of file
    fseek(pFile, 0, SEEK_END);
    long fileSize = ftell(pFile);
    rewind(pFile);

    if (loop * size > fileSize) {
        cout << "The file is only " << fileSize << "B, try a loop number smaller than " << fileSize / size << endl;
        return;
    }


    clock_t start, end;
    start = clock();
    for (int i = 0; i < loop; ++i) {
        fread(mem, sizeof(char), size, pFile);
//        cout << mem << endl;
    }
    end = clock();
    double seconds = (double) (end - start) / CLOCKS_PER_SEC;

    if (remove(alterName) != 0) {
        cout << "Error deleting file " << alterName << endl;
    }
    mu.lock();
    durations.push_back(seconds);
    mu.unlock();

    fclose(pFile);
    free(mem);
}

void random_read(int id, int size, int loop, char* fileName){
    char* mem  = (char *) malloc(size);
    FILE* pFile;

    char alterName[80];
    strcpy(alterName, fileName);
    alterName[strlen(fileName)] = id + '0';
    alterName[strlen(fileName) + 1] = 0;

//    cout << alterName << endl;

    pFile = fopen(alterName, "w");
    if(pFile == NULL) {
        cout << "File error." << endl;
        return;
    }
    for (int i = 0; i < loop; ++i) {
        fwrite(mem, sizeof(char), size, pFile);
    }
    fclose(pFile);

    pFile = fopen(alterName, "r");
    if (pFile == NULL) {
        cout << "File error." << endl;
        return;
    }
    // get the size of file
    fseek(pFile, 0, SEEK_END);
    long fileSize = ftell(pFile);
    rewind(pFile);

    if (loop * size > fileSize) {
        cout << "The file is only " << fileSize << "B, try a loop number smaller than " << fileSize / size << endl;
        return;
    }

    srand(time(NULL));
    int *dir = new int[loop];
    for (int j = 0; j < loop; ++j) {
        dir[j] = rand() % (fileSize - size);
    }

    clock_t start, end;
    start = clock();
    for (int i = 0; i < loop; ++i) {
        fseek(pFile, dir[i], SEEK_SET);
        fread(mem, sizeof(char), size, pFile);
    }
    end = clock();
    double seconds = (double) (end - start) / CLOCKS_PER_SEC;

    if (remove(alterName) != 0) {
        cout << "Error deleting file " << alterName << endl;
    }

    mu.lock();
    durations.push_back(seconds);
    mu.unlock();

    fclose(pFile);
    free(mem);
}

int main(int argc, char** argv) {
    int nSize[] = {1, 1024, 1024*1024}; // 1B, 1K, 1M

    int access = 1; // 0 for sequence access, 1 for random access
    int operation = 0; // 0 for read, 1 for write
    int sizeNum = 2; // to decide block size
    int thread_num = 1;
    int loopExp = 10;
    int nExperiment = 2;
    char* fileName = "/Users/mialiu/Projects/benchmark-disk/file0.bin";

    if (argc > 7) {
        access = atoi(argv[1]);
        operation = atoi(argv[2]);
        sizeNum = atoi(argv[3]);
        thread_num = atoi(argv[4]);
        loopExp = atoi(argv[5]);
        nExperiment = atoi(argv[6]);
        fileName = argv[7];
    }
    int loop = (int) pow(2.0, (double) loopExp);
    int size = nSize[sizeNum];
//    cout << "size: " << size << endl;
    cout << thread_num << " threads for " << (access == 0 ? "sequence" : "random")
    << (operation == 0 ? " read " : " write ") << (sizeNum == 0 ? "1B" : (sizeNum == 1 ? "1K" : "1M"))
    << " disk run 2^" << loopExp << " times for " << nExperiment << " experiments" << endl << endl;

    while(nExperiment--) {
        durations.clear();
        thread *thr = new thread[thread_num];
        //cout << "duration size: " << durations.size() << endl;
        for (int i = 0; i < thread_num; ++i) {
            //cout << "Creating thread " << i << endl;
            if (operation == 1) {
                if (0 == access) {
                    thr[i] = thread(sequence_write, i, size, loop, fileName);
                } else {
                    thr[i] = thread(random_write, i, size, loop, fileName);
                }
            } else {
                if (0 == access) {
                    thr[i] = thread(sequence_read, i+nExperiment, size, loop, fileName);
                } else {
                    thr[i] = thread(random_read, i, size, loop, fileName);
                }
            }
        }
        for (int i = 0; i < thread_num; ++i) {
            thr[i].join();
        }

        if (durations.size() != thread_num) {
            cout << "WRONG DATA: duration size: " << durations.size() << ", thread: " << thread_num << endl;
        }
        for (list<double>::iterator it = durations.begin(); it != durations.end(); it++) {
            cout << *it << '\t';
        }
        //delete thr;
        cout << endl;
        //cout << "End. There are " << nExperiment << " more to go." << endl << endl;
        if (nExperiment != 0) {
            usleep(10 * 1000 * 1000);
        }
    }
    return 0;
}

void run(){

    int nSize[] = {1, 1024, 1024*1024}; // 1B, 1KB, 1MB

    int size = nSize[0];
    int loop = 1024*1024;
    char* mem  = (char *) malloc(size);
    FILE* pFile;
    pFile = fopen("/Users/mialiu/Projects/benchmark-disk/file0.bin", "w");
    clock_t start, end;
    start = clock();
    for (int i = 0; i < loop; ++i) {
        fwrite(mem, sizeof(char), size, pFile);
    }
    end = clock();
    double seconds = (double) (end - start) / CLOCKS_PER_SEC;
    fclose(pFile);

    cout << "sequential durations: " << seconds << endl;
}