#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream reader("input.txt");
    if(reader) {
        std::string line;
        while(std::getline(reader, line)) {
            std::cout << line << "\n";
        }
    }
}
