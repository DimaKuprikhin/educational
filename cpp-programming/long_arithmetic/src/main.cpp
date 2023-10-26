#include <expression_evaluator.h>
#include <iostream>
#include <long_integer.h>

int main() {
    std::cout << "Enter \"help\" for hints and \"exit\" to exit the program." << std::endl;
    while(true) {
        std::cout << ">" << std::flush;
        std::string input;
        std::getline(std::cin, input);
        if(input == "help") {
            std::cout << "Supported operations: \"+\", \"-\", \"*\", \"/\", \"%\".\n"
                      << "Operators priority by descending:\n"
                      << "- \"*\", \"/\", \"%\"\n"
                      << "- \"+\", \"-\"\n"
                      << "Operators with the same priority evaluated sequentially.\n"
                      << "Unary minus is properly processed only for the first number in\n"
                      << "an expression and for the first number after an open brace.\n"
                      << "\"-2 + (-3 * 5)\" -> right\n"
                      << "\"3 - -2\" -> wrond\n" << std::flush;
            continue;
        }
        if(input == "exit") {
            break;
        }

        try {
            std::cout << ExpressionEvaluator::Evaluate(input) << std::endl;
        }
        catch(...) {
            std::cout << "Incorrect expression. Enter \"help\" for hints." << std::endl;
        }
    }
}
