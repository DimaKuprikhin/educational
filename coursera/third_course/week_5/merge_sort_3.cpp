#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    const size_t size = range_end - range_begin;
    if(size < 2) {
        return;
    }
    std::vector<typename RandomIt::value_type> temp(make_move_iterator(range_begin),
                                                    make_move_iterator(range_end));
    RandomIt firstEnd = temp.begin() + size / 3;
    RandomIt secondEnd = temp.begin() + 2 * size / 3;
    MergeSort(temp.begin(), firstEnd);
    MergeSort(firstEnd, secondEnd);
    MergeSort(secondEnd, temp.end());
    std::vector<typename RandomIt::value_type> firstSecondParts;
    std::merge(std::make_move_iterator(temp.begin()), std::make_move_iterator(firstEnd),
                std::make_move_iterator(firstEnd), std::make_move_iterator(secondEnd),
                std::back_inserter(firstSecondParts));
    std::merge(std::make_move_iterator(firstSecondParts.begin()),
                std::make_move_iterator(firstSecondParts.end()),
                std::make_move_iterator(secondEnd), std::make_move_iterator(temp.end()),
                range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}