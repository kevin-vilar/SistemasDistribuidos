#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <math.h>

bool isPrime(int n){
	int i, m;
	m = n/2;    
	for(i=2;i<=m;i++) {    
		if(n%i==0)    
		{    
			return false;    
		}    
	}
	return true;  
}

int getRandomNumber()
{
  return rand() % (int)10e7 + 1;
}

class Semaphore
{
  private:
    std::mutex mutex;
    std::condition_variable conditionVariable;
    int count;

  public:
    Semaphore(int count_ = 0)
        : count(count_){}

    inline void notify()
    {
      std::unique_lock<std::mutex> lock(mutex);
      count++;
      conditionVariable.notify_one();
    }
    inline void wait()
    {
      std::unique_lock<std::mutex> lock(mutex);
      while (count == 0)
        conditionVariable.wait(lock);
      count--;
    }
};

class MutexCounter
{
private:
  std::mutex mutex;
  int counter;

public:
  MutexCounter()
  {
    this->counter = 0;
  }

  int getCount()
  {
    mutex.lock();
    int c = this->counter;
    mutex.unlock();
    return c;
  }

  void incrementCount()
  {
    mutex.lock();
    this->counter++;
    mutex.unlock();
  }
};

int main(int argc, char **argv)
{
  srand(time(NULL));

  int M = (int)10e5;
  int Np = atoi(argv[1]);
  int Nc = atoi(argv[2]);
  int N = atoi(argv[3]);

  MutexCounter producerCounter;
  MutexCounter consumerCounter;

  std::vector<int> buffer;
  std::vector<std::thread> producers;
  std::vector<std::thread> consumers;

  Semaphore mutex(1);
  Semaphore empty(N);
  Semaphore full(0);
  
  int i;
  
  for (i = 0; i < Np; i++)
  {
    producers.push_back(std::thread([&]() {
      while (producerCounter.getCount() < M)
      {
        producerCounter.incrementCount();
        empty.wait();
        mutex.wait();
        buffer.push_back(getRandomNumber());
        mutex.notify();
        full.notify();
      }
    }));
  }

  for (i = 0; i < Nc; i++)
  {
    consumers.push_back(std::thread([&]() {
      while (consumerCounter.getCount() < M)
      {
        consumerCounter.incrementCount();
        full.wait();
        mutex.wait();
        int n = buffer.back();
        buffer.pop_back();
        mutex.notify();
        empty.notify();
        std::cout << "O número: " << n << " é primo? " << isPrime(n) << std::endl;
      }
    }));
  }

  for (i = 0; i < Np; i++)
    producers[i].join();
  for (i = 0; i < Nc; i++)
    consumers[i].join();
  
  return 0;
}
