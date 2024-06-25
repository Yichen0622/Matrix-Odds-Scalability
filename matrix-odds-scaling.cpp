#include <iostream>
#include <bits/stdc++.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
using namespace std::chrono;
// maximum size of matrix
#define DIM 1000

// maximum number of threads
// #define NUM_THREAD 8
int NUM_THREAD;
int matA[DIM][DIM];
int result[16];

void* countOdds(void* arg)
{
  int tid = *((int *)arg);
  result[tid] = 0;
  int chunkSize = int(DIM/NUM_THREAD);
  
  int myStart = tid * chunkSize;
  int myEnd = min(myStart+chunkSize, DIM);

  for (int i=myStart; i<myEnd; ++i)
    for (int j=0; j< DIM; ++j)
      if(matA[i][j] % 2 !=0 )
         ++result[tid];
}

// Driver Code
int main(int argc, char** argv)
{
    if (argc != 2) return -1;

    NUM_THREAD = atoi(argv[1]);
    
    cout << "NUM_THREAD is " << NUM_THREAD << endl;
    
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            matA[i][j] = rand() % 10;
        }
    }

    pthread_t threads[NUM_THREAD];
    int taskids[NUM_THREAD];
    
    auto start = high_resolution_clock::now();
    for (int t = 0; t < NUM_THREAD; t++) {
	taskids[t] = t;
        pthread_create(&threads[t], NULL, countOdds, (void*) &taskids[t]);
    }

    for (int t = 0; t < NUM_THREAD; t++)
        pthread_join(threads[t], NULL);
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    
    int odds = 0;
    for (int i = 0; i < NUM_THREAD; i++) {
            odds+=result[i];
    }
    cout << endl
         << "The number of odd elements of A " << endl;
    cout << odds << " ";
    cout << endl;
    return 0;
}
