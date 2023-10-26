#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

int main() {
    std::ifstream input("input.txt");
    int n, m;
    char comma;
    input >> n >> m;
    while(n--) {
        for(int i = 0; i < m; ++i) {
            int value;
            input >> value;
            if(i < m - 1) {
                input >> comma;
            }

            std::cout << std::setw(10) << std::right << value;
            if(i < m - 1) {
                std::cout << ' ';
            }
        }
        if(n > 0) {
            std::cout << "\n";
        }
    }
}
