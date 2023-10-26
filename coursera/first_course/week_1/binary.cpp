#include <iostream>

int main() {
    int number;
    std::cin >> number;
    bool hasOne = false;
    for(int i = 30; i >= 0; --i) {
        int curr = (number >> i) & 1;
        hasOne |= curr;
        if(hasOne) {
            std::cout << curr;
        }
    }
    std::cout << "\n";
}

