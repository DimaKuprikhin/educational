#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>
#include <map>

using namespace std;

template <typename String>
std::vector<std::vector<String>> GroupHeavyStrings(vector<String> strings) {
    std::map<String, std::vector<String>> groups;
    for (String& string : strings) {
        String copy = string;
        std::sort(copy.begin(), copy.end());
        copy.erase(std::unique(copy.begin(), copy.end()), copy.end());
        groups[std::move(copy)].push_back(std::move(string));
    }
    std::vector<std::vector<String>> result;
    result.reserve(groups.size());
    for (auto& [key, group] : groups) {
      result.push_back(std::move(group));
    }
    return result;
}

void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups));
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups));
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  return 0;
}