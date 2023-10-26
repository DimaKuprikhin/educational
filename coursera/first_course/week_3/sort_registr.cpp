#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

char ToLower(const char c) {
    if(c >= 'A' && c <= 'Z') {
        return c - 'A' + 'a';
    }
    return c;
}

int main() {
    int size;
    std::cin >> size;
    std::vector<std::string> a(size);
    for(auto& el : a) {
        std::cin >> el;
    }

    std::sort(a.begin(), a.end(), [](const std::string& lhs, const std::string& rhs) -> bool {
        for(int i = 0; i < std::min(lhs.size(), rhs.size()); ++i) {
            const char l = ToLower(lhs[i]);
            const char r = ToLower(rhs[i]);
            if(l != r) {
                return l < r;
            }
        }
        return lhs.size() < rhs.size();
    });

    for(const auto& el : a) {
        std::cout << el << " ";
    }
}

