#include <vector>

void Reverse(std::vector<int>& numbers) {
    for(int i = 0; i < numbers.size() / 2; ++i) {
        std::swap(numbers[i], numbers[numbers.size() - 1 - i]);
    }
}

