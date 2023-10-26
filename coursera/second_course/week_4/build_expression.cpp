#include <iostream>
#include <string>
#include <vector>
#include <queue>

int main() {
    int x;
    std::cin >> x;

    int operations;
    std::cin >> operations;

    std::queue<std::pair<char, int>> q;
    for(int i = 0; i < operations; ++i) {
        char operation;
        int operand;
        std::cin >> operation >> operand;
        q.push({ operation, operand });
    }

    for(int i = 0; i < operations; ++i) {
        std::cout << '(';
    }
    std::cout << x;
    while(!q.empty()) {
        std::cout << ") " << q.front().first << ' ' << std::to_string(q.front().second);
        q.pop();
    }
    std::cout << "\n";
}
