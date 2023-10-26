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

// bool IsPalindrom(const std::string& string) {
//     for(int i = 0; i < string.size() / 2; ++i) {
//         if(string[i] != string[string.size() - 1 - i]) {
//             return false;
//         }
//     }
//     return true;
// }

void EmptyStringTest() {
    Assert(IsPalindrom(""), "empty string is a palindrome");
}

void OneSymbolTest() {
    Assert(IsPalindrom("a"), "one symbol is a palindrome");
    Assert(IsPalindrom(" "), "one symbol is a palindrome");
    Assert(IsPalindrom("-"), "one symbol is a palindrome");
}

void AllSymbolsTest() {
    Assert(IsPalindrom("abcdefghijklmnopqrstuvwxyz0123456789  9876543210zyxwvutsrqponmlkjihgfedcba"), "");
    Assert(IsPalindrom(".,/?'\"\\;:[{]}=+-_)(*&^$#@!`~`!@#$^&*()_-+=}]{[:;\\\"'?/,."), "");
}

void NonPalindromesTest() {
    Assert(!IsPalindrom("ab"), "");
    Assert(!IsPalindrom("aab"), "");
    Assert(!IsPalindrom("abccbd"), "");
    Assert(!IsPalindrom("abcdba"), "");
    Assert(!IsPalindrom("abcdecba"), "");
    Assert(!IsPalindrom(" abccba"), "");
    Assert(!IsPalindrom("abccba "), "");
    Assert(!IsPalindrom("abc   c ba"), "");
}

int main() {
    TestRunner runner;
    runner.RunTest(EmptyStringTest, "empty string");
    runner.RunTest(OneSymbolTest, "one symbol");
    runner.RunTest(AllSymbolsTest, "all symbols");
    runner.RunTest(NonPalindromesTest, "non palindromes");

    return 0;
}