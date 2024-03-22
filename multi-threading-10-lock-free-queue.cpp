// #include <atomic>
// #include <iostream>
// #include <thread>

#include <bits/stdc++.h>

using namespace std;

template <typename T>
class LockFreeQueue {
 private:
  struct Node {
    atomic<T> data;
    atomic<Node *> next;

    Node(T d) : data(d), next(nullptr) {}
  };

  atomic<Node *> head = nullptr;
  atomic<Node *> tail = nullptr;
  mutex pop_mutex;
  condition_variable cv;

 public:
  void push(T data) {
    Node *new_node = new Node(data);
    if (head) {
      head.load()->next.store(new_node);
      head.store(head.load()->next.load());
    } else {
      head.store(new_node);
      tail.store(new_node);
    }
    cv.notify_one();
  }

  T pop() {
    unique_lock<mutex> ul(pop_mutex);
    cv.wait(ul, [&] { return head.load() != nullptr; });
    if (tail) {
      Node *end_node = tail.exchange(nullptr);
      tail.store(end_node->next);
      int data = end_node->data.load();
      delete end_node;
      return data;
    } else {
      std::cout << "Queue empty!" << std::endl;
      return -1;
    }
  }

  void print() {
    atomic<Node *> next_node = tail.load();
    cout << "\nQueue = ";
    while (next_node) {
      cout << next_node.load()->data.load() << " ";
      next_node.store(next_node.load()->next.load());
    }
    cout << endl;
  }
};

void pusher(int n, LockFreeQueue<int> *queue) {
  for (int i = 0; i < n; i++) {
    queue->push(i);
  }
}

void popper(int n, LockFreeQueue<int> *queue) {
  for (int i = 0; i < n; i++) {
    int val = queue->pop();
    cout << "Val poped = " << val << endl;
    this_thread::sleep_for(100ms);
  }
}

int main() {
  LockFreeQueue<int> *queue = new LockFreeQueue<int>;

  //   queue->push(5);
  //   queue->push(4);
  //   queue->push(3);
  //   queue->push(2);

  //   queue->print();

  //   int val = queue->pop();
  //   cout << "Val poped = " << val << endl;
  //   val = queue->pop();
  //   cout << "Val poped = " << val << endl;
  //   val = queue->pop();
  //   cout << "Val poped = " << val << endl;
  //   val = queue->pop();
  //   cout << "Val poped = " << val << endl;
  //   val = queue->pop();
  //   cout << "Val poped = " << val << endl;
  //   val = queue->pop();
  //   cout << "Val poped = " << val << endl;

  thread t3(popper, 300, queue);
  this_thread::sleep_for(1s);
  thread t1(pusher, 100, queue);
  thread t2(pusher, 200, queue);

  t1.join();
  t2.join();

  //   queue->print();

  t3.join();

  delete queue;

  return 0;
}