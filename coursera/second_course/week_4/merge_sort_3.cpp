#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

template <typename RandomIt>
void MergeSort(RandomIt begin, RandomIt end) {
    if(begin + 1 == end) {
        return;
    }
    RandomIt leftMid = begin + (end - begin) / 3;
    RandomIt rightMid = begin + 2 * (end - begin) / 3;
    MergeSort(begin, leftMid);
    MergeSort(leftMid, rightMid);
    MergeSort(rightMid, end);
    std::vector<typename RandomIt::value_type> temp;
    std::merge(begin, leftMid, leftMid, rightMid, std::back_inserter(temp));
    std::vector<typename RandomIt::value_type> temp2;
    std::merge(temp.begin(), temp.end(), rightMid, end, std::back_inserter(temp2));
    std::copy(temp2.begin(), temp2.end(), begin);
}

int main() {
    std::vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  return 0;
}
