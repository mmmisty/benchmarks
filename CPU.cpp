#include <iostream>
#include <cmath>
#include <thread>
#include <list>
#include <mutex>
#include <unistd.h>

using namespace std;

list<double> durations;
mutex mu;

void integer_thread(int id, int loop) {
    //cout << "Integer Thread " << id << endl;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int result = 0;

    for (int i = 0; i < loop; ++i) {
        for (int j = 0; j < 10; ++j) {
            result += i;
        }
    }
    gettimeofday(&end, NULL);
    double seconds = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;

    mu.lock();
    durations.push_back(seconds);
    mu.unlock();
}

void double_thread(int id, int loop) {
    //cout << "Double Thread " << id << endl;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    double result = 0.0;

    for (int i = 0; i < loop; ++i) {
        for (int j = 0; j < 10; ++j) {
            result += result;
        }
    }
    gettimeofday(&end, NULL);
    double seconds = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;

    mu.lock();
    durations.push_back(seconds);
    mu.unlock();
}

int main(int argc, char** argv) {
    int operation = 0; // 0 for integer, 1 for float
    int thread_num = 1;
    int loopExp = 9;
    int nExperiment = 1;
    if (argc > 4) {
        operation = atoi(argv[1]);
        thread_num = atoi(argv[2]);
        loopExp = atoi(argv[3]);
        nExperiment = atoi(argv[4]);
    }

    int loop = (int) pow(10.0, (double) loopExp);
    cout << thread_num << " threads for " << (operation == 0 ? "integer" : "double")
        << " run 10*10^" << loopExp << " times for " << nExperiment << " experiments" << endl << endl;

    while (nExperiment--) {
        durations.clear();

        thread *thr = new thread[thread_num];
        //cout << "duration size: " << durations.size() << endl;
        for (int i = 0; i < thread_num; ++i) {
            //cout << "Creating thread " << i << endl;
            if (0 == operation) {
                thr[i] = thread(integer_thread, i, loop);
            } else {
                thr[i] = thread(double_thread, i, loop);
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