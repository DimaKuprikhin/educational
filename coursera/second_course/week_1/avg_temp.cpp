#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    
    std::vector<int64_t> temps(n);
    int64_t sum = 0;
    for(int i = 0; i < n; ++i) {
        std::cin >> temps[i];
        sum += temps[i];
    }
    int64_t avg = sum / n;

    int ans = 0;
    for(int i = 0; i < n; ++i) {
        if(temps[i] > avg) {
            ++ans;
        }
    }
    std::cout << ans << "\n";
    for(int i = 0; i < n; ++i) {
        if(temps[i] > avg) {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";
}
