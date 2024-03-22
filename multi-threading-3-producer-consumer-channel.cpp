#include <bits/stdc++.h>

using namespace std;

template <typename T>
class Channel {
 private:
  vector<T> val;
  mutex io_mutex;
  std::condition_variable cv;

 public:
  Channel() = default;
  ~Channel() = default;

  void push_data(T data) {
    lock_guard<mutex> lock(io_mutex);
    val.push_back(data);
    cv.notify_one();
  }

  T get_data() {
    std::unique_lock<mutex> ul(io_mutex);
    cv.wait(ul, [this] {
      cout << "waiting for data!\n";
      return val.size() > 0;
    });

    T ret = val[0];
    val.erase(val.begin() + 0);

    return ret;
  }
};

template <typename T>
void task1(std::shared_ptr<Channel<T>> ptr, T data) {
  ptr->push_data(data);
  cout << "Thread 1 | push data | " << endl;
}

template <typename T>
void task2(std::shared_ptr<Channel<T>> ptr) {
  int data = ptr->get_data();
  cout << "Thread 2 | get data = " << data << endl;
}

template <typename T>
void task3(std::shared_ptr<Channel<T>> ptr) {
  int data = ptr->get_data();
  cout << "Thread 3 | get data = " << data << endl;
}

template <typename T>

void task4(std::shared_ptr<Channel<T>> ptr, T data) {
  ptr->push_data(data);
  cout << "Thread 4 | push data " << endl;
}

int main() {
  std::shared_ptr<Channel<int>> main_ptr = std::make_shared<Channel<int>>();

  thread t2(task2<int>, main_ptr);
  thread t3(task3<int>, main_ptr);
  thread t1(task1<int>, main_ptr, (int)0);
  thread t4(task4<int>, main_ptr, (int)5);

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  return 0;
}