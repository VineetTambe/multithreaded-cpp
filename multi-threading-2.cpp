#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

class Obj {
 private:
  long val;
  mutex m;

 public:
  Obj() = default;
  ~Obj() = default;

  Obj(long n) { val = n; }

  void increment() {
    lock_guard<mutex> lock(m);
    for (int i = 0; i < 10000; i++) ++val;
    // cout << "val = " << val << endl;
  }

  void print_val() { cout << val << endl; }

  void count_until(long cnt) {
    val++;
    cout << "val = " << val << endl;
    while (cnt != val) {
      cnt++;
    }
    this_thread::sleep_for(3s);
  }
};

void task1(Obj* ob) {
  cout << "Inside thread " << this_thread::get_id() << endl;
  ob->count_until(0);
}

int main() {
  Obj* ob = new Obj(100000);

  thread t1(&Obj::increment, ob);
  thread t2(&Obj::increment, ob);

  int n = 100;

  // thread t3(
  //     [](int& n) {
  //       n++;
  //       cout << n << endl;
  //     },
  //     std::ref(n));

  // thread t4(
  //     [](int& n) {
  //       n++;
  //       cout << n << endl;
  //     },
  //     std::ref(n));

  t1.join();
  t2.join();

  ob->print_val();

  // t3.join();
  // t4.join();

  return 0;
}