#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;

class NumArray {
  vector<int> prefix_sum;
 public:
  NumArray(vector<int> &nums) {
    prefix_sum.resize(nums.size());
    prefix_sum.front() = nums.front();
    for (int i = 1; i < nums.size(); ++i) {
      prefix_sum[i] = prefix_sum[i - 1] + nums[i];
    }
  }

  int sumRange(int left, int right) {
    return (0 < left) ? prefix_sum[right] - prefix_sum[left - 1] : prefix_sum[right];
  }
};

void TestNumArray() {
  {
    vector<int> nums{-2, 0, 3, -5, 2, -1};
    NumArray num_array{nums};
    assert(1 == num_array.sumRange(0, 2));
    assert(-1 == num_array.sumRange(2, 5));
    assert(-3 == num_array.sumRange(0, 5));
  }
  {
    vector<int> nums{-8261, 2300, -1429, 6274, 9650, -3267, 1414, -8102, 6251, -5979, -5291, -4616, -4703};
    NumArray num_array{nums};
    assert(4830 == num_array.sumRange(0, 8));
    assert(6383 == num_array.sumRange(4, 5));
  }
}

int ProcessInput(istream& in) {
  string line;
  if (getline(in, line)) {
    vector<int> nums;
    stringstream snums{line};
    int n;
    while (snums >> n) {
      nums.push_back(n);
    }
    NumArray num_array{nums};;
    int count;
    in >> count;
    getline(in, line);
    for (int i = 0; i < count; ++i) {
      int left, right;
      if (in >> left >> right) {
        cout << num_array.sumRange(left, right) << '\n';
      } else {
        cerr << "Failed to parse query "s << count;
        return 1;
      }
    }
  }

  return 0;
}

int main(int argc, char** argv) {
  if (argc == 2) {
    ifstream in{argv[1]};
    if (in.is_open()) {
      ProcessInput(in);
    } else {
      cerr << "Cannot read "s << filesystem::current_path().append(argv[1]) << endl;
      return 1;
    }
  } else {
    ProcessInput(cin);
  }

  return 0;
}
