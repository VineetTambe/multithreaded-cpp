#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#define N 5

using namespace std;

void run_philosopher_w_mutex(int phil_idx, mutex &left_fork, mutex &right_fork,
                             mutex &io_m) {
  int eat_cnt = 0;
  for (;;) {
    {
      lock_guard<mutex> lock(io_m);
      cout << "Philosopher " << phil_idx << " thinking.\n";
    }
    this_thread::sleep_for(3s);

    {
      lock_guard<mutex> lock(io_m);
      cout << "Philosopher " << phil_idx << " hungry.\n";
    }
    // this_thread::sleep_for(3s);
    lock_guard<mutex> f1(left_fork);
    lock_guard<mutex> f2(right_fork);
    {
      lock_guard<mutex> lock(io_m);
      cout << "Philosopher " << phil_idx << " eating.\n";
      cout << "Philosopher " << phil_idx << " has eaten " << ++eat_cnt
           << " times.\n";
    }
    this_thread::sleep_for(3s);
  }
}

int main() {
  mutex forks[N];
  mutex io_mutex;

  thread t1(run_philosopher_w_mutex, 0, std::ref(forks[0]), std::ref(forks[1]),
            std::ref(io_mutex));
  thread t2(run_philosopher_w_mutex, 1, std::ref(forks[1]), std::ref(forks[2]),
            std::ref(io_mutex));
  thread t3(run_philosopher_w_mutex, 2, std::ref(forks[2]), std::ref(forks[3]),
            std::ref(io_mutex));
  thread t4(run_philosopher_w_mutex, 3, std::ref(forks[3]), std::ref(forks[4]),
            std::ref(io_mutex));
  thread t5(run_philosopher_w_mutex, 4, std::ref(forks[4]), std::ref(forks[0]),
            std::ref(io_mutex));

  // thread t1([&] { run_philosopher_w_mutex(0, forks[0], forks[1], io_mutex);
  // }); thread t2([&] { run_philosopher_w_mutex(1, forks[1], forks[2],
  // io_mutex); }); thread t3([&] { run_philosopher_w_mutex(2, forks[2],
  // forks[3], io_mutex); }); thread t4([&] { run_philosopher_w_mutex(3,
  // forks[3], forks[4], io_mutex); }); thread t5([&] {
  // run_philosopher_w_mutex(4, forks[4], forks[0], io_mutex); });

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();

  return 0;
}