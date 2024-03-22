#include <chrono>
#include <future>
#include <iostream>
#include <thread>

using namespace std;

int run() {
  this_thread::sleep_for(5s);
  return 100;
}

int main() {
  cout << "start!\n";
  future<int> res = async(std::launch::deferred, run);
  cout << "waiting for value! \n";
  cout << "Val = " << res.get() << endl;
  return 0;
}