#include "sum_reverse_sort.h"
#include <algorithm>

int Sum(int x, int y) {
    return x + y;
}

string Reverse(string s) {
    for(int i = 0; i < s.size() / 2; ++i) {
        std::swap(s[i], s[s.size() - i - 1]);
    }
    return s;
}

void Sort(vector<int>& nums) {
    std::sort(nums.begin(), nums.end());
}
