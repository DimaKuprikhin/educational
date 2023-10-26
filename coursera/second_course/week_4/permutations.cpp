#include <algorithm>
#include <iostream>
#include <vector>

void Print(const std::vector<int>& vec) {
    for(const auto el : vec) {
        std::cout << el << " ";
    }
    std::cout << "\n";
}

int main() {
    int size;
    std::cin >> size;
    
    std::vector<int> a(size);
    for(int i = 0; i < a.size(); ++i) {
        a[i] = a.size() - i;
    }

    Print(a);
    while(std::next_permutation(a.begin(), a.end(), [](int lhs, int rhs)->bool{ return lhs > rhs; })) {
        Print(a);
    }
}
