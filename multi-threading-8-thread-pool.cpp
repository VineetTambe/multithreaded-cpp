// #include <condition_variable>
// // #include <function>
// #include <iostream>
// #include <mutex>
// #include <thread>
// #include <vector>

#include <bits/stdc++.h>

using namespace std;

class ThreadPool {
 private:
  vector<thread> threads_;
  vector<function<void()>> jobs_;
  mutex queue_mutex_;
  condition_variable cv;
  bool stop_thread_pool = false;

  void thread_loop() {
    function<void()> job;
    while (true) {
      {
        unique_lock<mutex> ul(queue_mutex_);
        cv.wait(ul, [&] { return (jobs_.size() > 0) || (stop_thread_pool); });
        if (stop_thread_pool) break;
        job = jobs_[0];
        jobs_.erase(jobs_.begin() + 0);
      }
      cv.notify_one();
      //   run the job;
      job();

      cout << "\nJob done!\n";
    }
  }

 public:
  ThreadPool(int num_threads) {
    for (int i = 0; i < num_threads; i++) {
      threads_.push_back(thread(&ThreadPool::thread_loop, this));
    }
  }

  bool enqueue_job(function<void()> job) {
    {
      //   unique_lock<mutex> ul(queue_mutex_);
      lock_guard<mutex> lock(queue_mutex_);
      jobs_.push_back(job);
    }
    cv.notify_one();
    return true;
  }

  int get_num_jobs() {
    {
      lock_guard<mutex> lock(queue_mutex_);
      return jobs_.size();
    }
  }

  int get_num_threads() { return threads_.size(); }

  bool stop_threads() {
    {
      unique_lock<mutex> ul(queue_mutex_);
      stop_thread_pool = true;
      cv.notify_all();
    }
    for (int i = 0; i < threads_.size(); i++) {
      if (threads_[i].joinable()) {
        threads_[i].join();
      }
    }
    threads_.clear();
    return true;
  }
};

void task() {
  cout << "Inside thread " << this_thread::get_id() << endl;
  this_thread::sleep_for(1s);
}

int main() {
  ThreadPool th(5);

  th.enqueue_job(task);
  th.enqueue_job(task);
  th.enqueue_job(task);
  th.enqueue_job(task);
  th.enqueue_job(task);
  th.enqueue_job(task);
  th.enqueue_job(task);

  cout << "num jobs enqueued /waiting = " << th.get_num_jobs() << endl;

  this_thread::sleep_for(3s);
  cout << "stopping the thread pool! \n";
  th.stop_threads();
}