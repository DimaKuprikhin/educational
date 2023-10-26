#include <iostream>
#include <map>
#include <exception>
#include <string>

template<typename K, typename V>
V& GetRefStrict(std::map<K, V>& map, const K& key) {
    auto iter = map.find(key);
    if(iter == map.end()) {
        throw std::runtime_error("");
    }
    return iter->second;
}

// int main() {
//     std::map<int, std::string> m = {{0, "value"}};
//     std::string& item = GetRefStrict(m, 0);
//     item = "newvalue";
//     std::cout << m[0] << std::endl; // выведет newvalue
// }
