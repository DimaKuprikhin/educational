#include <iostream>
#include <vector>
#include <map>
#include <set>

template<typename T>
T Sqr(T value);

template<typename T>
std::vector<T> Sqr(const std::vector<T>& vector);

template<typename T, typename K>
std::map<T, K> Sqr(const std::map<T, K>& map);

template<typename T, typename K>
std::pair<T, K> Sqr(const std::pair<T, K>& pair);

template<typename T>
T Sqr(T value) {
    return value * value;
}

template<typename T>
std::vector<T> Sqr(const std::vector<T>& vector) {
    std::vector<T> result(vector.size());
    for(int i = 0; i < vector.size(); ++i) {
        result[i] = Sqr(vector[i]);
    }
    return result;
}

template<typename T, typename K>
std::map<T, K> Sqr(const std::map<T, K>& map) {
    std::map<T, K> result;
    for(const auto& entry : map) {
        result.insert({ entry.first, Sqr(entry.second) });
    }
    return result;
}

template<typename T, typename K>
std::pair<T, K> Sqr(const std::pair<T, K>& pair) {
    return { Sqr(pair.first), Sqr(pair.second) };
}

// int main() {
    // std::cout << Sqr(3) << "\n";
    // const auto squaredVec = Sqr(std::vector<int>({ 1, 2, 3, 5 }));
    // for(const int i : squaredVec) {
    //     std::cout << i << " ";
    // }
    // std::cout << "\n";

    // const auto squaredMap = Sqr(std::map<int, int>({{ 1, 1 }, {2, 2}, {5, 5}}));
    // for(const auto& entry : squaredMap) {
    //     std::cout << "{ " << entry.first << " " << entry.second << " }, ";
    // }
    // std::cout << "\n";

    // const auto squaredPair = Sqr(std::pair<int, int>(5, 6));
    // std::cout << squaredPair.first << " " << squaredPair.second << "\n";

    // Пример вызова функции
// }
