#include <iostream>

int gcd(const int first, const int second) {
    if(second == 0) {
        return first;
    }
    return gcd(second, first % second);
}

int main() {
    int first, second;
    std::cin >> first >> second;
    std::cout << gcd(first, second);
}

