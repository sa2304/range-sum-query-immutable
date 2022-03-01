#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

class NumArray {
  struct Node {
    explicit Node(int start, int end, int sum)
        : start(start), end(end), sum(sum) {}

    Node *left = nullptr;
    Node *right = nullptr;

    int start;
    int end;

    int sum;
  };

  Node *root;

  Node *buildSumTree(int start, int count, const vector<int> &nums) {
    int sum = accumulate(nums.begin() + start, nums.begin() + start + count, 0);
//    clog << "sum["s << start << ";"s << start + count - 1 << "] = "s << sum << endl;
    Node *root = new Node(start, start + count - 1, sum);
    if (1 < count) {
      root->left = buildSumTree(start, count / 2, nums);
      root->right = buildSumTree(start + count / 2, count - count / 2, nums);
    }

    return root;
  }

  bool inRange(int x, int start, int end) {
    return start <= x and x <= end;
  }

  int sumRange(Node *root, int left, int right) {
    if (root) {
//      clog << "sumRange("s << left << ","s << right << ")"s << endl;
//      clog << "node=[left="s << root->start << ", right="s << root->end << "]"s << endl;
      vector<bool> range_tests{inRange(root->start, left, right), inRange(root->end, left, right)};
      auto get_bool = [](bool test) { return test; };
      bool lr_contains_node = all_of(range_tests.begin(), range_tests.end(), get_bool);
      bool node_contains_lr = none_of(range_tests.begin(), range_tests.end(), get_bool);
      bool intersects = any_of(range_tests.begin(), range_tests.end(), get_bool);
      if (lr_contains_node) { return root->sum; }
      else if (node_contains_lr or intersects) {
        return sumRange(root->left, left, right) + sumRange(root->right, left, right);
      }
    }

    return 0;
  }

 public:
  NumArray(vector<int> &nums) {
    root = buildSumTree(0, nums.size(), nums);
  }

  int sumRange(int left, int right) {
    return sumRange(root, left, right);
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

int main() {
  TestNumArray();
  cout << "Ok!" << endl;
  return 0;
}
