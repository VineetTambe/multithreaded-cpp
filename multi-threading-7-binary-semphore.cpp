// Question:
//     You are given a paragraph, which contain n number of words,
//     you are given m threads.What you need to do is,
//     each thread should print one word and give the control to next thread,
//     this way each thread will keep on printing one word,
//     in case last thread come,
//     it should invoke the first
//         thread.Printing will repeat until all the words are printed in
//             paragraph.Finally all threads should exit gracefully.What kind of
//                 synchronization will use ?
//

// #include <chrono>
// #include <condition_variable>
// #include <iostream>
// #include <semaphore>
// #include <shared_ptr>
// #include <thread>
// #include <vector>

#include <bits/stdc++.h>

#define N_THREAD 5

using namespace std;

struct bin_smph {
  binary_semaphore sem{0};
  bin_smph() {}
};

void task(int task_id, bin_smph (&smph)[N_THREAD]) {
  while (true) {
    smph[task_id].sem.acquire();

    cout << "Task id = " << task_id << endl;
    this_thread::sleep_for(1s);

    smph[(task_id + 1) % N_THREAD].sem.release();
  }
}

void task2(int task_id, binary_semaphore &smph) {
  smph.acquire();

  cout << "Task id = " << task_id << endl;
  this_thread::sleep_for(1s);

  smph.release();
}

void task3(int task_id, int &read_id, binary_semaphore &smph) {
  smph.acquire();

  cout << "Task id = " << task_id << " | read_id = " << read_id << endl;
  read_id++;
  this_thread::sleep_for(1s);

  smph.release();
}

void run_test_w_array() {
  bin_smph smph[N_THREAD];

  thread t1([&] { task(0, ref(smph)); });
  thread t2([&] { task(1, ref(smph)); });
  thread t3([&] { task(2, ref(smph)); });
  thread t4([&] { task(3, ref(smph)); });
  thread t5([&] { task(4, ref(smph)); });

  smph[0].sem.release();

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
}

void run_test_w_sem() {
  binary_semaphore smph{0};
  thread t1([&] { task2(0, ref(smph)); });
  thread t2([&] { task2(1, ref(smph)); });
  thread t3([&] { task2(2, ref(smph)); });
  thread t4([&] { task2(3, ref(smph)); });
  thread t5([&] { task2(4, ref(smph)); });

  smph.release();

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
}

void run_test_w_sem_read_id() {
  binary_semaphore smph{0};
  int read_id = 0;
  thread t1([&] { task3(0, ref(read_id), ref(smph)); });
  thread t2([&] { task3(1, ref(read_id), ref(smph)); });
  thread t3([&] { task3(2, ref(read_id), ref(smph)); });
  thread t4([&] { task3(3, ref(read_id), ref(smph)); });
  thread t5([&] { task3(4, ref(read_id), ref(smph)); });

  smph.release();

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
}

int main() {
  cout << "Running single semaphore example!\n";
  run_test_w_sem();

  cout << "Running single semaphore synchronized read_id example!\n";
  run_test_w_sem_read_id();

  cout << "Running array semaphore example!\n";
  run_test_w_array();

  return 0;
}
