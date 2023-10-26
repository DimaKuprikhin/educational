#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <stdexcept>
using namespace std;

int Gcd(const int first, const int second) {
    if(second == 0) {
        return first;
    }
    return Gcd(second, first % second);
}

class Rational {
public:
    Rational() {}

    Rational(int numerator, int denominator) {
        if(denominator == 0) {
            throw std::invalid_argument("Denominator can't be zero");
        }
        if(denominator < 0) {
            denominator *= -1;
            numerator *= -1;
        }
        int gcd = std::abs(Gcd(numerator, denominator));
        this->numerator = numerator / gcd;
        this->denominator = denominator / gcd;
    }

    int Numerator() const {
        return numerator;
    }

    int Denominator() const {
        return denominator;
    }

    bool operator<(const Rational& other) const {
        return (*this - other).Numerator() < 0;
    }

    bool operator>(const Rational& other) const {
        return other < *this;
    }

    bool operator==(const Rational& other) const {
        return !(*this != other);
    }

    bool operator!=(const Rational& other) const {
        return *this < other || *this > other;
    }

    Rational operator+(const Rational& other) const {
        return { numerator * other.denominator + other.numerator * denominator,
                 denominator * other.denominator };
    }

    Rational operator-(const Rational& other) const {
        return { numerator * other.denominator - other.numerator * denominator,
                 denominator * other.denominator };
    }

    Rational operator*(const Rational& other) const {
        return { numerator * other.numerator, denominator * other.denominator };
    }

    Rational operator/(const Rational& other) const {
        if(other.numerator == 0) {
            throw std::domain_error("Division by zero");
        }
        return { numerator * other.denominator, denominator * other.numerator };
    }

private:
    int numerator = 0;
    int denominator = 1;
};

std::istream& operator>>(std::istream& stream, Rational& rational) {
    int numerator, denominator;
    char slash;
    if(!(stream >> numerator >> slash >> denominator) || slash != '/') {
        return stream;
    }
    rational = { numerator, denominator };
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Rational& rational) {
    return stream << rational.Numerator() << "/" << rational.Denominator();
}

int main() {
    try {
        Rational r(1, 0);
        cout << "Doesn't throw in case of zero denominator" << endl;
        return 1;
    } catch (invalid_argument&) {
    }

    try {
        auto x = Rational(1, 2) / Rational(0, 1);
        cout << "Doesn't throw in case of division by zero" << endl;
        return 2;
    } catch (domain_error&) {
    }

    cout << "OK" << endl;
    return 0;
}