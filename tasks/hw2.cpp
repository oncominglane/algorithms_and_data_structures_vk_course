#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

struct ListNode
{
    int val;
    ListNode* next;

    ListNode(int value, ListNode* nextNode = nullptr)
        : val(value), next(nextNode) {}
};

// 1. reverse linked list: O(n) time, O(1) extra space
ListNode* reverseLinkedList(ListNode* head)
{
    ListNode* prev = nullptr;
    ListNode* current = head;

    while (current != nullptr)
    {
        ListNode* next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    return prev;
}

// 2. middle of linked list: O(n) time, O(1) extra space
// For an even-length list, return the second middle node.
ListNode* middleNode(ListNode* head)
{
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

// 3. remove all nodes with value val: O(n) time, O(1) extra space
// The list nodes must be allocated with new.
ListNode* removeElements(ListNode* head, int val)
{
    ListNode dummy(0, head);
    ListNode* prev = &dummy;
    ListNode* current = head;

    while (current != nullptr)
    {
        if (current->val == val)
        {
            prev->next = current->next;
            delete current;
            current = prev->next;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }

    return dummy.next;
}

// 4. check whether a is a subsequence of b: O(|b|) time, O(1) extra space
bool isSubsequence(const string& a, const string& b)
{
    size_t i = 0;
    size_t j = 0;

    while (i < a.size() && j < b.size())
    {
        if (a[i] == b[j])
            i++;
        j++;
    }

    return i == a.size();
}

// 5. pair sum in a sorted array: O(n) time, O(1) extra space
bool hasPairWithSum(const vector<int>& nums, int target)
{
    size_t left = 0;
    size_t right = nums.size();

    while (right - left > 1)
    {
        long long sum = 1LL * nums[left] + nums[right - 1];
        if (sum == target)
            return true;
        if (sum < target)
            left++;
        else
            right--;
    }

    return false;
}

// 6. palindrome: O(n) time, O(1) extra space
bool isPalindrome(const string& s)
{
    size_t left = 0;
    size_t right = s.size();

    while (left < right)
    {
        right--;
        if (s[left] != s[right])
            return false;
        left++;
    }

    return true;
}

// 7. remove duplicates from a sorted array: O(n) time, O(1) extra space
void removeDuplicates(vector<int>& nums)
{
    if (nums.empty())
        return;

    size_t slow = 1;
    for (size_t fast = 1; fast < nums.size(); fast++)
    {
        if (nums[fast] != nums[slow - 1])
        {
            nums[slow] = nums[fast];
            slow++;
        }
    }
    nums.resize(slow);
}

// 8. merge sorted linked lists: O(n + m) time, O(1) extra space
ListNode* mergeSortedLists(ListNode* list1, ListNode* list2)
{
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (list1 != nullptr && list2 != nullptr)
    {
        if (list1->val <= list2->val)
        {
            tail->next = list1;
            list1 = list1->next;
        }
        else
        {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    tail->next = list1 != nullptr ? list1 : list2;
    return dummy.next;
}

// Helpers for list tests
ListNode* makeList(initializer_list<int> values)
{
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int value : values)
    {
        tail->next = new ListNode(value);
        tail = tail->next;
    }
    return dummy.next;
}

bool listEquals(ListNode* head, initializer_list<int> values)
{
    for (int value : values)
    {
        if (head == nullptr || head->val != value)
            return false;
        head = head->next;
    }
    return head == nullptr;
}

void deleteList(ListNode* head)
{
    while (head != nullptr)
    {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main()
{
    // 1
    ListNode* list = makeList({1, 2, 3, 4, 5});
    list = reverseLinkedList(list);
    assert(listEquals(list, {5, 4, 3, 2, 1}));
    deleteList(list);
    assert(reverseLinkedList(nullptr) == nullptr);
    list = makeList({1});
    list = reverseLinkedList(list);
    assert(listEquals(list, {1}));
    deleteList(list);

    // 2
    list = makeList({1, 2, 3, 4, 5});
    assert(middleNode(list)->val == 3);
    deleteList(list);
    list = makeList({1, 2, 3, 4});
    assert(middleNode(list)->val == 3);
    deleteList(list);
    assert(middleNode(nullptr) == nullptr);

    // 3
    list = makeList({2, 2, 1, 2, 3, 2});
    list = removeElements(list, 2);
    assert(listEquals(list, {1, 3}));
    list = removeElements(list, 9);
    assert(listEquals(list, {1, 3}));
    deleteList(list);
    list = makeList({2, 2});
    list = removeElements(list, 2);
    assert(list == nullptr);
    assert(removeElements(nullptr, 2) == nullptr);

    // 4
    assert(isSubsequence("abc", "ahbgdc"));
    assert(!isSubsequence("abc", "acb"));
    assert(!isSubsequence("aa", "a"));
    assert(isSubsequence("", "abc"));
    assert(!isSubsequence("a", ""));

    // 5
    assert(hasPairWithSum({1, 2, 3, 4, 6}, 8));
    assert(hasPairWithSum({2, 2}, 4));
    assert(!hasPairWithSum({1, 2, 3}, 7));
    assert(!hasPairWithSum({2}, 4));
    assert(!hasPairWithSum({}, 0));
    assert(!hasPairWithSum({2147483647, 2147483647}, -2));

    // 6
    assert(isPalindrome("racecar"));
    assert(isPalindrome("abba"));
    assert(isPalindrome(""));
    assert(isPalindrome("a"));
    assert(!isPalindrome("hello"));
    assert(!isPalindrome("ab"));

    // 7
    vector<int> nums = {1, 1, 2, 2, 3, 3};
    removeDuplicates(nums);
    assert((nums == vector<int>{1, 2, 3}));
    nums = {4, 4, 4};
    removeDuplicates(nums);
    assert((nums == vector<int>{4}));
    nums = {1, 2, 3};
    removeDuplicates(nums);
    assert((nums == vector<int>{1, 2, 3}));
    nums.clear();
    removeDuplicates(nums);
    assert(nums.empty());

    // 8
    ListNode* list1 = makeList({3, 6, 8});
    ListNode* list2 = makeList({4, 7, 9, 11});
    list = mergeSortedLists(list1, list2);
    assert(listEquals(list, {3, 4, 6, 7, 8, 9, 11}));
    deleteList(list);
    list1 = makeList({1, 2, 2});
    list2 = makeList({1, 2, 3});
    list = mergeSortedLists(list1, list2);
    assert(listEquals(list, {1, 1, 2, 2, 2, 3}));
    deleteList(list);
    list1 = makeList({1, 2});
    list = mergeSortedLists(nullptr, list1);
    assert(list == list1);
    assert(listEquals(list, {1, 2}));
    deleteList(list);
    list1 = makeList({3});
    list = mergeSortedLists(list1, nullptr);
    assert(list == list1);
    deleteList(list);
    assert(mergeSortedLists(nullptr, nullptr) == nullptr);

    cout << "All tests passed!" << endl;
    return 0;
}
