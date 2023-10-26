#include <iostream>
#include <string>

int main() {
    std::string str;
    std::cin >> str;
    int occurrences = 0;
    for(int i = 0; i < str.size(); ++i) {
        if(str[i] == 'f') {
            ++occurrences;
        }
        if(occurrences == 2) {
            std::cout << i << "\n";
            break;
        }
    }
    if(occurrences < 2) {
        std::cout << (occurrences == 0 ? "-2\n" : "-1\n");
    }
}

