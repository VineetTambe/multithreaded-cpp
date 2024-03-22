#include <chrono>
#include <future>
#include <iostream>
// #include <promise>
#include <thread>

using namespace std;

void run(promise<int> some_promise) {
  this_thread ::sleep_for(3s);
  some_promise.set_value(2000);
}

int main() {
  promise<int> some_promise;
  future<int> some_future = some_promise.get_future();

  thread t1(run, move(some_promise));

  cout << "waiting for thread to run!\n";

  cout << "result = " << some_future.get() << endl;

  if (t1.joinable()) t1.join();

  return 0;
}