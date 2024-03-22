#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

// void run(int n) {
//   for (int i = 0; i < n; i++) {
//     cout << "Thread " << this_thread::get_id() << " is running" << endl;
//     this_thread::sleep_for(chrono::milliseconds(1000));
//   }
// }

class Obj {
 private:
  long val;

 public:
  Obj() = default;
  ~Obj() = default;

  Obj(long n) { val = n; }

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

  thread t1(task1, ob);
  thread t2(task1, ob);

  t1.join();
  t2.join();
  return 0;
}