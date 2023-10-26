#include <string>
#include <vector>

void MoveStrings(std::vector<std::string>& source, std::vector<std::string>& dest) {
    for(auto& string : source) {
        dest.push_back(std::move(string));
    }
    source.clear();
}

