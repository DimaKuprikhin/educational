#include <iostream>
#include <string>
#include <set>
#include <vector>

template<typename T>
std::vector<T> FindGreaterElements(const std::set<T>& elements, const T& border) {
    std::vector<T> result;
    for(auto it = elements.upper_bound(border); it != elements.end(); ++it) {
        result.push_back(*it);
    }
    return result;
}
