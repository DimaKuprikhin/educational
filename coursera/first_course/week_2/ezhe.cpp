#include <iostream>
#include <vector>

int main() {
    int queries;
    std::cin >> queries;

    std::vector<std::vector<std::string>> tasks(32);
    std::vector<int> days({ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 });
    int month = 0;

    while(queries--) {
        std::string queryType;
        std::cin >> queryType;
        
        if(queryType == "DUMP") {
            int day;
            std::cin >> day;
            std::cout << tasks[day].size();
            for(const auto& task : tasks[day]) {
                std::cout << " " << task;
            }
            std::cout << "\n";
        }
        if(queryType == "NEXT") {
            month = (month + 1) % 12;
            for(int i = days[month] + 1; i <= 31; ++i) {
                for(auto& task : tasks[i]) {
                    tasks[days[month]].push_back(std::move(task));
                }
                tasks[i].clear();
            }
        }
        if(queryType == "ADD") {
            int day;
            std::string task;
            std::cin >> day >> task;
            tasks[day].push_back(task);
        }
    }
}

