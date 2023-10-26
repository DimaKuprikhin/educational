#include <iostream>
#include <vector>

int main() {
    uint64_t n, r;
    std::cin >> n >> r;

    uint64_t ans = 0;
    for(int i = 0; i < n; ++i) {
        uint64_t w, h, d;
        std::cin >> w >> h >> d;
        ans += w * h * d * r;
    }
    std::cout << ans << "\n";
}
