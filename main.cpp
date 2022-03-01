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
    Node *root = new Node(start, start + count - 1, 0);
    if (1 < count) {
      root->left = buildSumTree(start, count / 2, nums);
      root->right = buildSumTree(start + count / 2, count - count / 2, nums);
      root->sum = root->left->sum + root->right->sum;
    } else {
      root->sum = nums[start];
    }

    return root;
  }

  bool inRange(int x, int start, int end) {
    return start <= x and x <= end;
  }

  int sumRange(Node *root, int left, int right) {
    if (root) {
      const bool lr_contains_node_start = inRange(root->start, left, right);
      const bool lr_contains_node_end = inRange(root->end, left, right);
      const bool lr_contains_node = lr_contains_node_start && lr_contains_node_end;
      const bool node_contains_lr = !lr_contains_node_start and !lr_contains_node_end;
      const bool intersects = lr_contains_node_start || lr_contains_node_end;
      if (lr_contains_node) { return root->sum; }
      else if (node_contains_lr || intersects) {
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
