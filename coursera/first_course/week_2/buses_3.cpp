#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
    std::map<std::set<std::string>, int> routes;

    int queries;
    std::cin >> queries;
    while(queries--) {
        int stops;
        std::set<std::string> route;
        std::cin >> stops;
        while(stops--) {
            std::string stop;
            std::cin >> stop;
            route.insert(std::move(stop));
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

