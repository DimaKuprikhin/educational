#include <iostream>
#include <list>

int main() {
    std::list<int> l = { 1, 2, 3, 4, 5, 6 };
    auto it1 = l.begin();
    ++it1, ++it1, ++it1;
    auto it2 = l.begin();
    ++it2;
    std::cout << *it1 << " " << *it2 << "\n";

    auto temp = it1;
    ++temp;
    l.splice(it2, l, it1);
    l.splice(temp, l, it2);

    for(const auto el : l) {
        std::cout << el << " ";
    }
    std::cout << "\n";
}
