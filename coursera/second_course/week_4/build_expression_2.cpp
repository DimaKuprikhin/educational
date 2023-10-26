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
    int braces = 0;
    for(int i = 0; i < operations; ++i) {
        char operation;
        int operand;
        std::cin >> operation >> operand;
        if((operation == '*' || operation == '/')
            && q.size() > 0
            && (q.back().first == '+' || q.back().first == '-'))
        {
            ++braces;
        }
        q.push({ operation, operand });
    }

    for(int i = 0; i < braces; ++i) {
        std::cout << '(';
    }
    std::cout << x;
    char prevOperation = ' ';
    while(!q.empty()) {
        if((prevOperation == '+' || prevOperation == '-')
            && (q.front().first == '*' || q.front().first == '/'))
        {
            std::cout << ')';
        }
        std::cout << ' ' << q.front().first << ' ' << std::to_string(q.front().second);
        prevOperation = q.front().first;
        q.pop();
    }
    std::cout << "\n";
}
