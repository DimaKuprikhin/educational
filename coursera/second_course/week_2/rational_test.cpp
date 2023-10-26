#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

// int Gcd(const int first, const int second) {
//     if(second == 0) {
//         return first;
//     }
//     return Gcd(second, first % second);
// }

// class Rational {
// public:
//     Rational()
//         : numerator(0)
//         , denominator(1)
//         {}

//     Rational(int numerator, int denominator) {
//         if(denominator < 0) {
//             denominator *= -1;
//             numerator *= -1;
//         }
//         int gcd = std::abs(Gcd(numerator, denominator));
//         this->numerator = numerator / gcd;
//         this->denominator = denominator / gcd;
//     }

//     int Numerator() const {
//         return numerator;
//     }

//     int Denominator() const {
//         return denominator;
//     }

// private:
//     int numerator;
//     int denominator;
// };

void DefaultConstructorTest() {
    Rational r;
    AssertEqual(r.Numerator(), 0, "default numerator should be 0");
    AssertEqual(r.Denominator(), 1, "default denominator should be 1");
}

void SimplifyingFractionTest() {
    Rational r(2, 2);
    AssertEqual(r.Numerator(), 1, "fraction 2/2 should be simplified");
    AssertEqual(r.Denominator(), 1, "fraction 2/2 should be simplified");
    r = { 16, -12 };
    AssertEqual(r.Numerator(), -4, "fraction 16/-12 should be simplified");
    AssertEqual(r.Denominator(), 3, "fraction 16/-12 should be simplified");
}

void NegativeFractionTest() {
    Rational r(-2, 3);
    AssertEqual(r.Numerator(), -2, "negative fraction should have negative numerator");
    AssertEqual(r.Denominator(), 3, "negative fraction should have negative numerator");
    r = { 2, -3 };
    AssertEqual(r.Numerator(), -2, "negative fraction should have negative numerator");
    AssertEqual(r.Denominator(), 3, "negative fraction should have negative numerator");
}

void PositiveFractionTest() {
    Rational r(2, 3);
    AssertEqual(r.Numerator(), 2, "positive fraction should have positive numerator and denominator");
    AssertEqual(r.Denominator(), 3, "positive fraction should have positive numerator and denominator");
    r = { -2, -3 };
    AssertEqual(r.Numerator(), 2, "positive fraction should have positive numerator and denominator");
    AssertEqual(r.Denominator(), 3, "positive fraction should have positive numerator and denominator");
}

void ZeroNumeratorTest() {
    Rational r(0, 1);
    AssertEqual(r.Numerator(), 0, "fraction with zero numerator should have denominator equals 1");
    AssertEqual(r.Denominator(), 1, "fraction with zero numerator should have denominator equals 1");
    r = { 0, 3 };
    AssertEqual(r.Numerator(), 0, "fraction with zero numerator should have denominator equals 1");
    AssertEqual(r.Denominator(), 1, "fraction with zero numerator should have denominator equals 1");
    r = { 0, -7 };
    AssertEqual(r.Numerator(), 0, "fraction with zero numerator should have denominator equals 1");
    AssertEqual(r.Denominator(), 1, "fraction with zero numerator should have denominator equals 1");
}

int main() {
    TestRunner runner;
    runner.RunTest(DefaultConstructorTest, "default constructor");
    runner.RunTest(SimplifyingFractionTest, "simplifying fraction");
    runner.RunTest(NegativeFractionTest, "negative fraction");
    runner.RunTest(PositiveFractionTest, "positive fraction");
    runner.RunTest(ZeroNumeratorTest, "zero numerator");

    return 0;
}