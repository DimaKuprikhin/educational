#include <cmath>
#include <iostream>

bool equal(const double lhs, const double rhs) {
    return std::abs(lhs - rhs) < 1e-9;
}

int main() {
    double a, b, c;
    std::cin >> a >> b >> c;
    if(a == 0) {
        if(b == 0) {
            return 0;
        }
        std::cout << -c / b << "\n";
    }
    else if(b == 0) {
        if(c == 0) {
            std::cout << "0\n";
        }
        else {
            if(-c / a < 0) {
                return 0;
            }
            std::cout << std::sqrt(-c / a) << " " << -std::sqrt(-c / a) << "\n";
        }
    }
    else if(c == 0) {
        std::cout << "0 " << -b / a << "\n";
    }
    else {
        double d = b * b - 4 * a * c;
        if(d < 0) {
            return 0;
        }
        d = std::sqrt(d);
        double x1 = (-b + d) / (2 * a);
        double x2 = (-b - d) / (2 * a);
        if(!equal(x1, x2)) {
            std::cout << x1 << " " << x2 << "\n";
        }
        else {
            std::cout << x1 << "\n";
        }
    }
}

