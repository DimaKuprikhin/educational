#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

template <typename RandomIt>
void MergeSort(RandomIt begin, RandomIt end) {
    if(begin + 1 == end) {
        return;
    }
    std::vector<typename RandomIt::value_type> copy(begin, end);
    RandomIt mid = copy.begin() + (copy.end() - copy.begin()) / 2;
    MergeSort(copy.begin(), mid);
    MergeSort(mid, copy.end());
    std::merge(copy.begin(), mid, mid, copy.end(), begin);
}

int main() {
    std::vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  return 0;
}
