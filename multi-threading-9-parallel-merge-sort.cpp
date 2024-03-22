// #include <iostream>
// #include <promise>
// #include <thread>
// #include <vector>

#include <bits/stdc++.h>

using namespace std;

vector<int> merge(vector<int> &left, vector<int> &right) {
  int len = left.size() + right.size();
  vector<int> vec;
  int l_itr = 0, r_itr = 0;

  while (l_itr < left.size() || r_itr < right.size()) {
    if (l_itr < left.size() && r_itr < right.size()) {
      if (left[l_itr] < right[r_itr]) {
        vec.push_back(left[l_itr]);
        l_itr++;
      } else {
        vec.push_back(right[r_itr]);
        r_itr++;
      }
    } else if (l_itr < left.size()) {
      vec.push_back(left[l_itr]);
      l_itr++;
    } else if (r_itr < right.size()) {
      vec.push_back(right[r_itr]);
      r_itr++;
    } else {
      // redundant break
      break;
    }
  }
  return vec;
}

vector<int> merge_sort_async(vector<int> &arr, int start, int end) {
  if (start == end) {
    vector<int> temp;
    if (start < arr.size()) temp = {arr[start]};
    return temp;
  }

  int mid = (int)(end - start) / 2 + start;

  vector<int> left = async(&merge_sort_async, ref(arr), start, mid).get();
  vector<int> right = async(&merge_sort_async, ref(arr), mid + 1, end).get();

  return merge(left, right);
}

void merge_sort_thread(vector<int> &arr, int start, int end, vector<int> &ans) {
  if (start == end) {
    if (start < arr.size()) ans = {arr[start]};
    return;
  }

  int mid = (int)(end - start) / 2 + start;

  // cout << "start = " << start << " | end = " << end << endl;

  vector<int> left;
  vector<int> right;

  thread t1(&merge_sort_thread, ref(arr), start, mid, ref(left));
  thread t2(&merge_sort_thread, ref(arr), mid + 1, end, ref(right));

  t1.join();
  t2.join();

  ans = merge(left, right);
}

void merge_sort_thread2(vector<int> &arr, int start, int end,
                        vector<int> &ans) {
  if (start == end) {
    if (start < arr.size()) ans = {arr[start]};
    return;
  }

  int mid = (int)(end - start) / 2 + start;

  // cout << "start = " << start << " | end = " << end << endl;

  vector<int> left;
  vector<int> right;
  merge_sort_thread2(arr, start, mid, left);
  merge_sort_thread2(arr, mid + 1, end, right);

  ans = merge(left, right);
}

std::vector<int> generateRandomVector(int n) {
  std::vector<int> vec(n);

  // Fill the vector with consecutive integers from 0 to n-1
  for (int i = 0; i < n; ++i) {
    vec[i] = i;
  }

  // Shuffle the vector using random device as seed
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(vec.begin(), vec.end(), g);

  return vec;
}

void run_thread2(vector<int> &vec) {
  // vector<int> vec = {5, 4, 2, 1, 0, 9, 8, 3, 7, 6};
  const clock_t begin_time = clock();

  int n_threads = 4;

  vector<vector<int>> sorted_vec(n_threads);

  vector<thread> threads;

  int factor = (int)vec.size() / n_threads;

  for (int i = 0; i < vec.size() - factor + 1; i += factor) {
    int start = i;
    int end = i + factor - 1;
    if ((i + factor) >= (vec.size() - 2)) end = vec.size() - 1;
    threads.push_back(thread(&merge_sort_thread2, ref(vec), start, end,
                             ref(sorted_vec[i / factor])));
  }

  for (int i = 0; i < n_threads; i++) {
    threads[i].join();
  }
  vector<int> res;
  for (int i = 0; i < n_threads; i++) {
    res = merge(sorted_vec[i], res);
  }

  // t0.join();/

  // cout << "Sorted array = " << endl;

  // for (int i = 0; i < res.size(); i++) {
  //   cout << res[i] << " ";
  // }

  cout << "4 thread Time taken = " << clock() - begin_time << endl;
}

void run_thread(vector<int> &vec) {
  // vector<int> vec = {5, 4, 2, 1, 0, 9, 8, 3, 7, 6};
  const clock_t begin_time = clock();
  vector<int> sorted_vec;

  thread t0(&merge_sort_thread, ref(vec), 0, vec.size() - 1, ref(sorted_vec));
  t0.join();

  // cout << "Sorted array = " << endl;

  // for (int i = 0; i < sorted_vec.size(); i++) {
  //   cout << sorted_vec[i] << " ";
  // }

  cout << "lots thread Time taken = " << clock() - begin_time << endl;
}

void run_asyn(vector<int> &vec) {
  // vector<int> vec = {5, 4, 2, 1, 0, 9, 8, 3, 7, 6};

  const clock_t begin_time = clock();
  vector<int> sorted_vec =
      async(&merge_sort_async, ref(vec), 0, vec.size() - 1).get();

  // cout << "Sorted array = " << endl;

  // for (int i = 0; i < sorted_vec.size(); i++) {
  //   cout << sorted_vec[i] << " ";
  // }
  cout << "async nTime taken = " << clock() - begin_time << endl;
}

int main() {
  int n = 10000;  // Number of elements you want in the vector
  std::vector<int> vec = generateRandomVector(n);

  // vector<int> vec = {5, 4, 2, 1, 0, 9, 8, 3, 7, 6};
  run_thread(vec);
  run_asyn(vec);
  run_thread2(vec);
  return 0;
}