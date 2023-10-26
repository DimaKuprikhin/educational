#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
    std::map<std::string, std::set<std::string>> synonyms;

    int queries;
    std::cin >> queries;
    while(queries--) {
        std::string queryType;
        std::cin >> queryType;
        if(queryType == "ADD") {
            std::string first, second;
            std::cin >> first >> second;
            synonyms[first].insert(second);
            synonyms[second].insert(first);
        }
        if(queryType == "COUNT") {
            std::string word;
            std::cin >> word;
            std::cout << synonyms[word].size() << "\n";
        }
        if(queryType == "CHECK") {
            std::string first, second;
            std::cin >> first >> second;
            std::cout << (synonyms[first].find(second) == synonyms[first].end() ? "NO\n" : "YES\n");
        }
    }
}

