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

vector<int> merge_sort(vector<int> &arr, int start, int end) {
  if (start == end) {
    vector<int> temp;
    if (start < arr.size()) temp = {arr[start]};
    return temp;
  }

  int mid = (int)(end - start) / 2 + start;
  //   cout << "start = " << start << " | end = " << end << endl;

  vector<int> left = merge_sort(arr, start, mid);
  vector<int> right = merge_sort(arr, mid + 1, end);

  return merge(left, right);
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

int main() {
  int n = 10000;  // Number of elements you want in the vector
  std::vector<int> vec = generateRandomVector(n);

  //  vector<int> sorted_vec = async(merge_sort, ref(vec), 0, vec.size() -
  //  1).get();
  const clock_t begin_time = clock();
  vector<int> sorted_vec = merge_sort(vec, 0, vec.size() - 1);

  //   cout << "Sorted array = " << endl;
  //   for (int i = 0; i < sorted_vec.size(); i++) {
  //     cout << sorted_vec[i] << " ";
  //   }
  cout << "Time taken = " << clock() - begin_time << endl;
  return 0;
}