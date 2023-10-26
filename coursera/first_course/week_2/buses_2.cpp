#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    std::map<std::vector<std::string>, int> routes;

    int queries;
    std::cin >> queries;
    while(queries--) {
        int stops;
        std::cin >> stops;
        std::vector<std::string> route(stops);
        for(int i = 0; i < stops; ++i) {
            std::cin >> route[i];
        }

        const auto iter = routes.find(route);
        if(iter == routes.end()) {
            routes[route] = routes.size() + 1;
            std::cout << "New bus " << routes.size() << "\n";
        }
        else {
            std::cout << "Already exists for " << iter->second << "\n";
        }
    }
}

