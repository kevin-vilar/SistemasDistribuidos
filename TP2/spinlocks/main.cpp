#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

std::atomic_flag lock = ATOMIC_FLAG_INIT;

// Define release/acquire functions
void releaseLock(){
    lock.clear();
}
void acquireLock(){
    while(lock.test_and_set());
}

// Define the general function to make the total sum increase
int totalSum = 0;
void AddToTotalSum(const uint8_t n){
    acquireLock();

    totalSum+=n;

    releaseLock();
}

// The sum function ran by each thread
void* makeThreadsSum(int b, int e, vector<uint8_t> *n){
    for (int i=b; i<e; i++) {
        AddToTotalSum(n->at(i));
    }
    pthread_exit(nullptr);
}

// The function that defines the amount of sums made by each thread depending on de K and N values
void sumForEachThread(int k, double n){
    totalSum = 0;
    int nk = int(n) / k;

    vector<thread> threads;
    vector<uint8_t> numbers = vector<uint8_t>(100);
    numbers.reserve(int(n));

    for (int i=0; i<n; ++i) {
        numbers.push_back(100);
    }
    threads.reserve(k);

    for (int i=0; i<k; i++) {
        vector<uint8_t> *pointer = &numbers;
        threads.push_back(thread(makeThreadsSum, i*nk, (i+1)*nk, pointer));
    }

    for (auto& th : threads)
        th.join();
}
int main() {
    int amountOfThreads[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    double amountOfNumbers[3] = {pow(10,7), pow(10,8), pow(10,9)};

    for (int threads: amountOfThreads) {
        for (double values: amountOfNumbers) {
            const auto startTime = chrono::system_clock::now();
            sumForEachThread(threads,values);
            const auto finalTime = chrono::system_clock::now();
            chrono::duration<double> duration = finalTime - startTime;

            printf("Para os valores N=%.f e K=%d o tempo para executar a soma foi de %.2f\n",values,threads,duration.count());
        }
    }
}
