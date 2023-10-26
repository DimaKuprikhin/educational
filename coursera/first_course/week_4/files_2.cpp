#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream reader("input.txt");
    std::ofstream writer("output.txt");
    if(reader && writer) {
        std::string line;
        while(std::getline(reader, line)) {
            writer << line << "\n";
        }
    }
}
