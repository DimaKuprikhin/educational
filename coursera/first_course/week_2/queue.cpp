#include <iostream>
#include <vector>

int main() {
    int queries;
    std::cin >> queries;

    // 0 - quiet, 1 - worried
    std::vector<int> queue;
    int worryCount = 0;

    while(queries--) {
        std::string queryType;
        std::cin >> queryType;
        if(queryType == "WORRY_COUNT") {
            std::cout << worryCount << "\n";
            continue;
        }
        int queryArg;
        std::cin >> queryArg;
        if(queryType == "WORRY") {
            if(queue[queryArg] == 0) {
                ++worryCount;
            }
            queue[queryArg] = 1;
        }
        if(queryType == "QUIET") {
            if(queue[queryArg] == 1) {
                --worryCount;
            }
            queue[queryArg] = 0;
        }
        if(queryType == "COME") {
            for(int i = 0; i < queryArg; ++i) {
                queue.push_back(0);
            }
            for(int i = 0; i < -queryArg; ++i) {
                if(queue.back() == 1) {
                    --worryCount;
                }
                queue.pop_back();
            }
        }
    }
}

