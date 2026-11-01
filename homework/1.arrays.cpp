
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <limits>

using namespace std;

// 1. sum of Numbers
long long sumNumbers(int n) 
{
    return 1LL * n * (n + 1) / 2;
}

// 2. two sum (sorted array)
vector<int> twoSum(const vector<int>& nums, int target) 
{
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;

    while (left < right) 
    {
        long long sum = 1LL * nums[left] + nums[right];

        if (sum == target) 
            return {left, right};
        else if (sum < target) 
            ++left;
        else
            --right;
    }

    return {};
}

// 3. reverse array
void reverseArray(vector<int>& nums) 
{
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;

    while (left < right) 
    {
        swap(nums[left], nums[right]);

        ++left;
        --right;
    }
}


// Helper: reverse elements in [left, right]
void reverseRange(vector<int>& nums, int left, int right) 
{
    while (left < right) 
    {
        swap(nums[left], nums[right]);
        ++left;
        --right;
    }
}

// 4. rotate array
void rotateArray(vector<int>& nums, int k) 
{
    int n = static_cast<int>(nums.size());
    if (n == 0) return;

    k %= n;

    reverseRange(nums, 0, n - 1);
    reverseRange(nums, 0, k - 1);
    reverseRange(nums, k, n - 1);
}

// 5. merge sorted arrays
vector<int> mergeSortedArrays( const vector<int>& a, const vector<int>& b) 
{
    vector<int> result;
    result.reserve(a.size() + b.size());

    size_t i = 0, j = 0;

    while (i < a.size() && j < b.size()) 
    {
        if (a[i] <= b[j])
        {
            result.push_back(a[i]);
            i++;
        }
        else
        {
            result.push_back(b[j]);
            j++;
        }
    }

    while (i < a.size()) 
        result.push_back(a[i++]);

    while (j < b.size())
        result.push_back(b[j++]);

    return result;
}

// 6. merge sorted arrays in-place
// a has m sorted elements followed by n free slots
void mergeSortedArraysInPlace(
    vector<int>& a,
    int m,
    const vector<int>& b
)
{
    int i = m - 1;
    int j = static_cast<int>(b.size()) - 1;
    int k = m + j;

    while (j >= 0) 
    {
        if (i >= 0 && a[i] > b[j]) 
            a[k--] = a[i--];
        else 
            a[k--] = b[j--];
    }
}

// 7. minimum size subarray sum
int minSubArrayLen(const vector<int>& nums, int target) 
{
    int left = 0;
    int minLen = numeric_limits<int>::max();
    long long currentSum = 0;

    for (int right = 0; right < static_cast<int>(nums.size()); ++right) 
    {
        currentSum += nums[right];

        while (currentSum >= target) 
        {
            minLen = min(minLen, right - left + 1);
            currentSum -= nums[left++];
        }
    }

    return minLen == numeric_limits<int>::max() ? 0 : minLen;
}

// 8.  sort binary array
void sortBinaryArray(vector<int>& nums) 
{
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;

    while (left < right) 
    {
        if (nums[left] == 0) 
            ++left;
        else if (nums[right] == 1) 
            --right;
        else 
        {
            swap(nums[left], nums[right]);
            ++left;
            --right;
        }
    }
}

// 9. dutch flag
void sortColors(vector<int>& nums) 
{
    int low = 0;
    int mid = 0;
    int high = static_cast<int>(nums.size()) - 1;

    while (mid <= high) {
        if (nums[mid] == 0) 
        {
            swap(nums[low], nums[mid]);
            ++low;
            ++mid;
        }
        else if (nums[mid] == 1) 
            ++mid;
        else 
        {
            swap(nums[mid], nums[high]);
            --high;
        }
    }
}

// 10. Move Even Numbers First
void moveEvenFirst(vector<int>& nums) 
{
    int evenIndex = 0;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) 
    {
        if (nums[i] % 2 == 0) 
        {
            swap(nums[i], nums[evenIndex]);
            ++evenIndex;
        }
    }
}

// 11. move zeroes
void moveZeroes(vector<int>& nums) 
 {
    int nonZeroIndex = 0;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) 
    {
        if (nums[i] != 0) 
        {
            swap(nums[i], nums[nonZeroIndex]);
            ++nonZeroIndex;
        }
    }
}



int main() 
{
    // 1
    assert(sumNumbers(100) == 5050);
    assert(sumNumbers(1000000) == 500000500000LL);

    // 2
    assert((twoSum({3, 8, 9, 11, 16, 18, 19, 21}, 25)
            == vector<int>{2, 4}));
    assert(twoSum({1, 2, 3}, 10).empty());

    // 3
    vector<int> nums = {1, 2, 3, 4, 5};
    reverseArray(nums);
    assert((nums == vector<int>{5, 4, 3, 2, 1}));

    // 4
    vector<int> a = {1, 2, 3, 4, 5, 6, 7};
    rotateArray(a, 3);
    assert((a == vector<int>{5, 6, 7, 1, 2, 3, 4}));

    // 5
    assert((mergeSortedArrays({1, 7, 9}, {3, 8, 10, 11})
            == vector<int>{1, 3, 7, 8, 9, 10, 11}));

    // 6
    vector<int> b = {3, 8, 10, 11, 0, 0, 0};
    mergeSortedArraysInPlace(b, 4, {1, 7, 9});
    assert((b == vector<int>{1, 3, 7, 8, 9, 10, 11}));

    // 7
    assert(minSubArrayLen({2, 3, 1, 2, 4, 3}, 7) == 2);
    assert(minSubArrayLen({1, 1, 1}, 10) == 0);

    // 8
    vector<int> c = {0, 1, 1, 0, 1, 0, 1, 0};
    sortBinaryArray(c);
    assert((c == vector<int>{0, 0, 0, 0, 1, 1, 1, 1}));

    // 9
    vector<int> e = {2, 0, 2, 1, 1, 0};
    sortColors(e);
    assert((e == vector<int>{0, 0, 1, 1, 2, 2}));

    // 10
    vector<int> f = {3, 2, 4, 1, 11, 8, 9};
    moveEvenFirst(f);
    assert((f == vector<int>{2, 4, 8, 1, 11, 3, 9}));

    // 11
    vector<int> g = {0, 0, 1, 0, 3, 12};
    moveZeroes(g);
    assert((g == vector<int>{1, 3, 12, 0, 0, 0}));

    cout << "All tests passed!" << endl;

    return 0;
}
