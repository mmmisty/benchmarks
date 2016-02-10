#include <iostream>
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

void sequence_access(int id, int size, int loop) {
    //cout << "Integer Thread " << id << endl;
    char *gbmemory = (char *) malloc(loop * size);
    // 1B buffer
    char *readBuffer = (char *) malloc(size);

    clock_t start, end;
    start = clock();

    for (int i = 0; i < loop; ++i) {
        memcpy(readBuffer, gbmemory + size * i, size);
    }
    end = clock();
    double seconds = (double) (end - start) / CLOCKS_PER_SEC;
//    cout << "sequential durations: " << seconds << endl;

    mu.lock();
    durations.push_back(seconds);
    mu.unlock();

    free(gbmemory);
    free(readBuffer);
}

void random_access(int id, int size, int loop) {
    //cout << "Integer Thread " << id << endl;
    char *gbmemory = (char *) malloc(loop * size);
    // 1B buffer
    char *readBuffer = (char *) malloc(size);

    clock_t start, end;

    srand(time(NULL));
    int *dir = new int[loop];
    for (int j = 0; j < loop; ++j) {
        dir[j] = rand() % loop;
    }
    start = clock();

    for (int j = 0; j < loop; ++j) {
        memcpy(readBuffer, gbmemory + size * dir[j], size);
    }
    end = clock();
    double seconds = (double) (end - start) / CLOCKS_PER_SEC;
    //cout << "sequential durations: " << seconds << endl;

    mu.lock();
    durations.push_back(seconds);
    mu.unlock();

    free(gbmemory);
    free(readBuffer);
    delete dir;
}

int main(int argc, char** argv) {
    int nSize[] = {1, 1024, 1024*1024}; // 1B, 1K, 1M

    int access = 0; // 0 for sequence access, 1 for random access
    int sizeNum = 0; // to decide block size
    int thread_num = 1;
    int loopExp = 30;
    int nExperiment = 1;

    if (argc > 5) {
        access = atoi(argv[1]);
        sizeNum = atoi(argv[2]);
        thread_num = atoi(argv[3]);
        loopExp = atoi(argv[4]);
        nExperiment = atoi(argv[5]);
    }
    int loop = (int) pow(2.0, (double) loopExp);
    int size = nSize[sizeNum];
//    cout << "size: " << size << endl;
    cout << thread_num << " threads for " << (access == 0 ? "sequence" : "random") << " accessing " << (sizeNum == 0 ? "1B" : (access == 1 ? "1K" : "1M"))
    << " memory run 2^" << loopExp << " times for " << nExperiment << " experiments" << endl << endl;

    while(nExperiment--) {
        durations.clear();
        thread *thr = new thread[thread_num];
        //cout << "duration size: " << durations.size() << endl;
        for (int i = 0; i < thread_num; ++i) {
            //cout << "Creating thread " << i << endl;
            if (0 == access) {
                thr[i] = thread(sequence_access, i, size, loop);
            } else {
                thr[i] = thread(random_access, i, size, loop);
            }
        }
        for (int i = 0; i < thread_num; ++i) {
            thr[i].join();
        }

        if (durations.size() != thread_num) {
            cout << "duration size: " << durations.size() << ", thread: " << thread_num << endl;
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

void run() {
    int nSize[] = {1, 1024, 1024*1024}; // 1B, 1K, 1M

    for (int k = 2; k >= 0; --k) {

        int size = nSize[k];
        cout << "size: " << size << endl;
        int loop = 1024 * 1024 * 1024 / size;
        char *gbmemory = (char *) malloc(1024 * 1024 * 1024);
        // 1B buffer
        char *readBuffer = (char *) malloc(size);

        clock_t start, end;
        srand(time(NULL));
        // random access
        int* dir = new int[loop];
        for (int j = 0; j < loop; ++j) {
            dir[j] = rand() % loop;
        }
        start = clock();
        for (int j = 0; j < loop; ++j) {
            memcpy(readBuffer, gbmemory + size * dir[j], size);
        }

        end = clock();
        double seconds = (double) (end - start) / CLOCKS_PER_SEC;
        cout << "random durations: " << seconds << endl;

        free(gbmemory);
        gbmemory = (char *) malloc(1024 * 1024 * 1024);

        // sequential access
        start = clock();
        for (int i = 0; i < loop; ++i) {
            memcpy(readBuffer, gbmemory + size * i, size);
        }
        end = clock();
        seconds = (double) (end - start) / CLOCKS_PER_SEC;
        cout << "sequential durations: " << seconds << endl;


        free(gbmemory);
        free(readBuffer);
        delete dir;

    }
}