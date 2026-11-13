#include <iostream>
#include <vector>
#include <optional>
#include <queue>
#include <cassert>

using namespace std;

struct TreeNode
{
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;

    explicit TreeNode(int value) : val(value) {}
};

// 1. Build a tree from heap-indexed level order: O(n) time and space.
// Children of index i are at 2*i+1 and 2*i+2; nullopt means no node.
TreeNode* buildTree(const vector<optional<int>>& values, size_t i = 0)
{
    if (i >= values.size() || !values[i])
        return nullptr;

    TreeNode* root = new TreeNode(*values[i]);
    // Guard the arithmetic as well as the array bounds.
    if (i < values.size() / 2)
    {
        root->left = buildTree(values, 2 * i + 1);
        root->right = buildTree(values, 2 * i + 2);
    }
    return root;
}

// 2. Compare mirrored pairs: O(n) time, O(h) recursion space.
bool isMirror(const TreeNode* a, const TreeNode* b)
{
    if (!a || !b)
        return a == b;
    return a->val == b->val && isMirror(a->left, b->right)
           && isMirror(a->right, b->left);
}

bool isSymmetric(const TreeNode* root)
{
    return !root || isMirror(root->left, root->right);
}

// 3. BFS reaches the nearest leaf first: O(n) time, O(n) space.
// Depth counts nodes, so a single-node tree has depth 1.
int minDepth(const TreeNode* root)
{
    if (!root)
        return 0;
    queue<const TreeNode*> nodes;
    nodes.push(root);
    int depth = 1;
    while (!nodes.empty())
    {
        size_t count = nodes.size();
        while (count-- > 0)
        {
            const TreeNode* node = nodes.front();
            nodes.pop();
            if (!node->left && !node->right)
                return depth;
            if (node->left) nodes.push(node->left);
            if (node->right) nodes.push(node->right);
        }
        depth++;
    }
    return depth;
}

// 4. BST extrema are the leftmost and rightmost nodes: O(h) time, O(1) space.
// An empty tree has no product; for one node, multiply its value by itself.
optional<long long> minMaxProduct(const TreeNode* root)
{
    if (!root)
        return nullopt;
    const TreeNode* smallest = root;
    const TreeNode* largest = root;
    while (smallest->left) smallest = smallest->left;
    while (largest->right) largest = largest->right;
    return 1LL * smallest->val * largest->val;
}

// 5. Corresponding nodes must have equal values and equal subtrees.
// O(n) time, O(h) recursion space.
bool isSameTree(const TreeNode* a, const TreeNode* b)
{
    if (!a || !b)
        return a == b;
    return a->val == b->val && isSameTree(a->left, b->left)
           && isSameTree(a->right, b->right);
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
    TreeNode* tree = buildTree({1, 2, 3, nullopt, 4});
    assert(tree && tree->left && tree->right);
    assert(!tree->left->left && tree->left->right->val == 4);
    assert(buildTree({}) == nullptr);
    assert(buildTree({nullopt, 1}) == nullptr);

    // 2
    TreeNode* symmetric = buildTree({1, 2, 2, 3, 4, 4, 3});
    TreeNode* asymmetric = buildTree({1, 2, 2, nullopt, 3, nullopt, 3});
    assert(isSymmetric(symmetric));
    assert(!isSymmetric(asymmetric));
    assert(isSymmetric(nullptr));

    // 3
    assert(minDepth(tree) == 2);
    TreeNode* chain = buildTree({1, 2, nullopt, 3});
    assert(minDepth(chain) == 3);
    assert(minDepth(nullptr) == 0);

    // 4
    TreeNode* bst = buildTree({5, 3, 8, 1, 4, 7, 10});
    assert(minMaxProduct(bst) == 10);
    TreeNode* single = buildTree({-3});
    assert(minMaxProduct(single) == 9);
    assert(!minMaxProduct(nullptr));
    TreeNode* rightOnly = buildTree({2, nullopt, 5});
    assert(minMaxProduct(rightOnly) == 10);

    // 5
    TreeNode* copy = buildTree({1, 2, 3, nullopt, 4});
    TreeNode* different = buildTree({1, 2, 3, 4});
    assert(isSameTree(tree, copy));
    assert(!isSameTree(tree, different));
    assert(!isSameTree(tree, nullptr));
    assert(isSameTree(nullptr, nullptr));

    deleteTree(tree);
    deleteTree(symmetric);
    deleteTree(asymmetric);
    deleteTree(chain);
    deleteTree(bst);
    deleteTree(single);
    deleteTree(rightOnly);
    deleteTree(copy);
    deleteTree(different);
    cout << "All tests passed!" << endl;
    return 0;
}
