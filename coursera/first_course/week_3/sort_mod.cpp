#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int size;
    std::cin >> size;
    std::vector<int> a(size);
    for(auto& el : a) {
        std::cin >> el;
    }

    std::sort(a.begin(), a.end(), [](const int lhs, const int rhs)->bool {return std::abs(lhs) < std::abs(rhs);});
    for(const auto el : a) {
        std::cout << el << " ";
    }
}

