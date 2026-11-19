#include <iostream>
#include <vector>
#include <optional>
#include <queue>
#include <stack>
#include <tuple>
#include <algorithm>
#include <functional>
#include <cassert>

using namespace std;

struct TreeNode
{
    int val;
    int balance_factor = 0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;

    explicit TreeNode(int value) : val(value) {}
};

// 1. Complete binary tree: O(n) time, O(n) space.
// BFS visits positions left to right. No node may follow a missing child.
bool isCompleteTree(const TreeNode* root)
{
    if (!root) return true;
    queue<const TreeNode*> nodes;
    nodes.push(root);
    bool missing = false;
    while (!nodes.empty())
    {
        const TreeNode* node = nodes.front();
        nodes.pop();
        if (!node)
            missing = true;
        else
        {
            if (missing) return false;
            nodes.push(node->left);
            nodes.push(node->right);
        }
    }
    return true;
}

// 2. Merge K sorted arrays: O(K + N log(K + 1)) time, O(K) heap space,
// plus O(N) for the result. The heap contains the next value from each array.
vector<int> mergeSortedArrays(const vector<vector<int>>& arrays)
{
    using Entry = tuple<int, size_t, size_t>; // value, array index, element index
    priority_queue<Entry, vector<Entry>, greater<Entry>> heap;
    for (size_t i = 0; i < arrays.size(); i++)
        if (!arrays[i].empty()) heap.emplace(arrays[i][0], i, 0);

    vector<int> result;
    while (!heap.empty())
    {
        auto [value, arrayIndex, index] = heap.top();
        heap.pop();
        result.push_back(value);
        index++;
        if (index < arrays[arrayIndex].size())
            heap.emplace(arrays[arrayIndex][index], arrayIndex, index);
    }
    return result;
}

// 3. K-th element in an array, counting duplicates: O(n log(k + 1)) time,
// O(k) space. Keep the k smallest in a max-heap (or largest in a min-heap).
// k is 1-based; an invalid k returns nullopt.
optional<int> kthSmallest(const vector<int>& nums, size_t k)
{
    if (k == 0 || k > nums.size()) return nullopt;
    priority_queue<int> heap;
    for (int value : nums)
    {
        heap.push(value);
        if (heap.size() > k) heap.pop();
    }
    return heap.top();
}

optional<int> kthLargest(const vector<int>& nums, size_t k)
{
    if (k == 0 || k > nums.size()) return nullopt;
    priority_queue<int, vector<int>, greater<int>> heap;
    for (int value : nums)
    {
        heap.push(value);
        if (heap.size() > k) heap.pop();
    }
    return heap.top();
}

// 4. In-order traversal of a BST visits values in sorted order; reverse it
// for largest. O(h + k) time and O(h) space, with h the tree height.
optional<int> kthInBST(const TreeNode* root, size_t k, bool largest = false)
{
    if (k == 0) return nullopt;
    stack<const TreeNode*> nodes;
    const TreeNode* current = root;
    while (current || !nodes.empty())
    {
        while (current)
        {
            nodes.push(current);
            current = largest ? current->right : current->left;
        }
        current = nodes.top();
        nodes.pop();
        if (--k == 0) return current->val;
        current = largest ? current->left : current->right;
    }
    return nullopt;
}

// 5. Post-order computes each height once: O(n) time, O(h) recursion space.
// Empty subtree height is 0; balance factor is left height minus right height.
int fillBalanceFactors(TreeNode* root)
{
    if (!root) return 0;
    int leftHeight = fillBalanceFactors(root->left);
    int rightHeight = fillBalanceFactors(root->right);
    root->balance_factor = leftHeight - rightHeight;
    return 1 + max(leftHeight, rightHeight);
}

// 6. Swap children at every node: O(n) time, O(h) recursion space.
void mirrorTree(TreeNode* root)
{
    if (!root) return;
    swap(root->left, root->right);
    mirrorTree(root->left);
    mirrorTree(root->right);
}

// Helper: heap-indexed level order, as in hw4.cpp.
TreeNode* buildTree(const vector<optional<int>>& values, size_t i = 0)
{
    if (i >= values.size() || !values[i]) return nullptr;
    TreeNode* root = new TreeNode(*values[i]);
    if (i < values.size() / 2)
    {
        root->left = buildTree(values, 2 * i + 1);
        root->right = buildTree(values, 2 * i + 2);
    }
    return root;
}

void deleteTree(TreeNode* root)
{
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main()
{
    // 1
    TreeNode* complete = buildTree({1, 2, 3, 4, 5, 6});
    TreeNode* incomplete = buildTree({1, 2, 3, 4, nullopt, 6});
    assert(isCompleteTree(complete));
    assert(!isCompleteTree(incomplete));
    assert(isCompleteTree(nullptr));
    TreeNode* rightOnly = buildTree({1, nullopt, 2});
    assert(!isCompleteTree(rightOnly));

    // 2
    assert((mergeSortedArrays({{1, 4, 7}, {}, {-2, 4, 8}, {0, 3}})
            == vector<int>{-2, 0, 1, 3, 4, 4, 7, 8}));
    assert(mergeSortedArrays({{}, {}}).empty());
    assert(mergeSortedArrays({}).empty());

    // 3
    vector<int> nums = {3, 1, 2, 2, -1};
    assert(kthSmallest(nums, 2) == 1);
    assert(kthLargest(nums, 2) == 2);
    assert(kthSmallest(nums, 5) == 3);
    assert(kthLargest(nums, 5) == -1);
    assert(!kthSmallest(nums, 0));
    assert(!kthLargest(nums, 6));
    assert(!kthSmallest({}, 1));

    // 4
    TreeNode* bst = buildTree({5, 3, 8, 1, 4, 7, 10, 0});
    assert(kthInBST(bst, 1) == 0);
    assert(kthInBST(bst, 3) == 3);
    assert(kthInBST(bst, 1, true) == 10);
    assert(kthInBST(bst, 8, true) == 0);
    assert(!kthInBST(bst, 9));
    assert(!kthInBST(bst, 0));
    assert(!kthInBST(nullptr, 1));

    // 5
    assert(fillBalanceFactors(bst) == 4);
    assert(bst->balance_factor == 1);
    assert(bst->left->balance_factor == 1);
    assert(bst->left->left->balance_factor == 1);
    assert(bst->right->balance_factor == 0);
    assert(bst->left->left->left->balance_factor == 0);
    assert(fillBalanceFactors(nullptr) == 0);

    // 6
    mirrorTree(bst);
    assert(bst->left->val == 8 && bst->right->val == 3);
    assert(bst->left->left->val == 10 && bst->right->right->val == 1);
    assert(bst->right->right->right->val == 0);
    mirrorTree(bst);
    assert(kthInBST(bst, 1) == 0 && kthInBST(bst, 1, true) == 10);
    mirrorTree(nullptr);

    deleteTree(complete);
    deleteTree(incomplete);
    deleteTree(rightOnly);
    deleteTree(bst);
    cout << "All tests passed!" << endl;
    return 0;
}
