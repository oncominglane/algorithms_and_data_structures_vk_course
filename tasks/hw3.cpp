#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <limits>

using namespace std;

// 1. integer square root: O(log n) time, O(1) extra space
// Search in [0, target]: the root of a nonnegative integer cannot exceed it.
// Each step halves the range; right is the floor of the root at the end.
int binarySearchSqrt(int target)
{
    int left = 0;
    int right = target;

    while (left <= right)
    {
        int middle = left + (right - left) / 2;
        long long square = 1LL * middle * middle;

        if (square > target)
        {
            right = middle - 1;
            continue;
        }
        if (square < target)
        {
            left = middle + 1;
            continue;
        }
        return middle;
    }

    return right;
}

// 2. two copiers: O(log(1 + n * max(x, y))) time, O(1) extra space
// n >= 0, x > 0, y > 0. First make one copy in min(x, y) minutes.
// Then time / x + time / y must be at least n - 1; this condition is monotonic.
long long copyTime(int n, int x, int y)
{
    if (n == 0)
        return 0;

    long long left = 0;
    long long right = 1LL * (n - 1) * max(x, y);

    while (left + 1 < right)
    {
        long long middle = left + (right - left) / 2;

        if (middle / x + middle / y < n - 1)
            left = middle;
        else
            right = middle;
    }

    return right + min(x, y);
}

// 3. feed animals: O(n log n + m log m) time, O(n + m) space for input copies
// Give the smallest suitable portion to the least demanding unfed animal.
// Smaller portions cannot feed anyone remaining; larger ones are saved for later.
int feedAnimals(vector<int> animals, vector<int> food)
{
    if (animals.empty() || food.empty())
        return 0;

    sort(animals.begin(), animals.end());
    sort(food.begin(), food.end());

    size_t count = 0;
    for (int portion : food)
    {
        if (portion >= animals[count])
            count++;
        if (count == animals.size())
            break;
    }

    return static_cast<int>(count);
}

// 4. extra letter: O(|a| + |b|) expected time, O(k) extra space
// Count letters in b, then subtract occurrences from a.
// The added letter is the only one with a positive count; k is the alphabet size.
string extraLetter(const string& a, const string& b)
{
    unordered_map<char, int> counts;
    for (char letter : b)
        counts[letter]++;

    for (char letter : a)
    {
        auto it = counts.find(letter);
        if (it != counts.end())
            it->second--;
    }

    for (const auto& entry : counts)
    {
        if (entry.second > 0)
            return string(1, entry.first);
    }

    return "";
}

// 5. two sum in an unsorted array: O(n) expected time, O(n) extra space
// Cache value -> index, so each complement lookup takes O(1) on average.
// Check different indices: one array element cannot be used twice.
vector<int> twoSum(const vector<int>& nums, int target)
{
    unordered_map<long long, int> cache;
    for (int i = 0; i < static_cast<int>(nums.size()); i++)
        cache[nums[i]] = i;

    for (int i = 0; i < static_cast<int>(nums.size()); i++)
    {
        long long diff = 1LL * target - nums[i];
        auto it = cache.find(diff);
        if (it != cache.end() && it->second != i)
            return {i, it->second};
    }

    return {};
}

// 6. group anagrams: O(L + k * n) expected time, O(L + k * n) space
// L is the total input length, n is the number of strings, k = 256 byte values.
// Equal character counts give equal keys. Separators prevent ambiguous keys.
vector<vector<string>> groupAnagrams(const vector<string>& strs)
{
    unordered_map<string, size_t> groups;
    vector<vector<string>> result;

    for (const string& word : strs)
    {
        array<size_t, 256> counts{};
        for (unsigned char letter : word)
            counts[letter]++;

        string key;
        for (size_t count : counts)
            key += to_string(count) + '#';

        auto it = groups.find(key);
        if (it == groups.end())
        {
            groups[key] = result.size();
            result.push_back({word});
        }
        else
            result[it->second].push_back(word);
    }

    return result;
}

// Helper: normalize group order for anagram tests
vector<vector<string>> normalizeGroups(vector<vector<string>> groups)
{
    for (vector<string>& group : groups)
        sort(group.begin(), group.end());
    sort(groups.begin(), groups.end());
    return groups;
}

int main()
{
    // 1
    assert(binarySearchSqrt(0) == 0);
    assert(binarySearchSqrt(1) == 1);
    assert(binarySearchSqrt(2) == 1);
    assert(binarySearchSqrt(8) == 2);
    assert(binarySearchSqrt(16) == 4);
    assert(binarySearchSqrt(numeric_limits<int>::max()) == 46340);

    // 2
    assert(copyTime(0, 1, 2) == 0);
    assert(copyTime(1, 3, 2) == 2);
    assert(copyTime(2, 1, 2) == 2);
    assert(copyTime(4, 1, 1) == 3);
    assert(copyTime(5, 1, 2) == 4);
    assert(copyTime(5, 2, 1) == 4);
    assert(copyTime(1000000000, 10, 10) == 5000000010LL);

    // 3
    assert(feedAnimals({3, 4, 7}, {8, 1, 2}) == 1);
    assert(feedAnimals({3, 8, 1, 4}, {1, 1, 2}) == 1);
    assert(feedAnimals({1, 2, 2}, {7, 1}) == 2);
    assert(feedAnimals({8, 2, 3, 2}, {1, 4, 3, 8}) == 3);
    assert(feedAnimals({1, 8}, {8, 1}) == 2);
    assert(feedAnimals({1}, {1, 2, 3}) == 1);
    assert(feedAnimals({}, {1, 2}) == 0);
    assert(feedAnimals({1, 2}, {}) == 0);

    // 4
    assert(extraLetter("uio", "oeiu") == "e");
    assert(extraLetter("fe", "efo") == "o");
    assert(extraLetter("bbb", "bbbb") == "b");
    assert(extraLetter("", "a") == "a");
    assert(extraLetter("ab", "ab").empty());

    // 5
    assert((twoSum({2, 7, 11, 15}, 9) == vector<int>{0, 1}));
    assert((twoSum({3, 2, 4}, 6) == vector<int>{1, 2}));
    assert((twoSum({3, 3}, 6) == vector<int>{0, 1}));
    assert((twoSum({-3, 4, 3, 90}, 0) == vector<int>{0, 2}));
    assert(twoSum({3}, 6).empty());
    assert(twoSum({1, 2, 3}, 10).empty());
    assert(twoSum({}, 0).empty());
    assert(twoSum({numeric_limits<int>::max(), 1},
                  numeric_limits<int>::min()).empty());
    assert((twoSum({numeric_limits<int>::min(), -1},
                   numeric_limits<int>::min() + 1).empty()));

    // 6
    assert((normalizeGroups(groupAnagrams({"eat", "tea", "tan", "ate", "nat", "bat"}))
            == normalizeGroups({{"bat"}, {"nat", "tan"}, {"ate", "eat", "tea"}})));
    assert((normalizeGroups(groupAnagrams({"won", "now", "aaa", "ooo", "ooo"}))
            == normalizeGroups({{"aaa"}, {"ooo", "ooo"}, {"won", "now"}})));
    assert((groupAnagrams({""}) == vector<vector<string>>{{""}}));
    assert((groupAnagrams({"", ""}) == vector<vector<string>>{{"", ""}}));
    assert((groupAnagrams({"a"}) == vector<vector<string>>{{"a"}}));
    assert((normalizeGroups(groupAnagrams({"aA!", "!Aa", "aa!"}))
            == normalizeGroups({{"aA!", "!Aa"}, {"aa!"}})));
    assert(groupAnagrams({}).empty());

    cout << "All tests passed!" << endl;
    return 0;
}
