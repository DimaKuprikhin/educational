#include "test_runner.h"

#include <array>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <iostream>

using namespace std;

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, size_t step_size) {
  const size_t values = last - first;
  if(values <= 1) {
    return;
  }
  std::vector<typename RandomIt::value_type> permutation;
  permutation.reserve(values);

  struct Node {
    size_t prev;
    size_t next;
  };
  // array
  std::vector<Node> nodes(values);
  nodes[0] = { nodes.size() - 1, 1 };
  nodes.back() = { nodes.size() - 2, 0 };
  for(size_t i = 1; i < nodes.size() - 1; ++i) {
    nodes[i] = { i - 1, i + 1 };
  }

  int64_t index = (-static_cast<int64_t>(step_size)) % static_cast<int64_t>(values);
  if(index < 0) {
    index += values;
  }
  for(int i = 0; i < values; ++i) {
    for(int step = 0; step < step_size; ++step) {
      index = nodes[index].next;
    }
    permutation.push_back(std::move(*(first + index)));
    nodes[nodes[index].prev].next = nodes[index].next;
    nodes[nodes[index].next].prev = nodes[index].prev;
    index = nodes[index].prev;
  }
  for(size_t i = 0; i < values; ++i) {
    *(first + i) = std::move(permutation[i]);    
  }
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    ASSERT_EQUAL(numbers_copy, numbers);
  }
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
    ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
  }
  {
    vector<int> numbers = { 1 };
    MakeJosephusPermutation(begin(numbers), end(numbers), 1);
    ASSERT_EQUAL(numbers, vector<int>({1}));
  }
  {
    vector<int> numbers = { 1, 2 };
    MakeJosephusPermutation(begin(numbers), end(numbers), 1);
    ASSERT_EQUAL(numbers, vector<int>({1, 2}));
  }
  {
    vector<int> numbers = { 1, 2 };
    MakeJosephusPermutation(begin(numbers), end(numbers), 2);
    ASSERT_EQUAL(numbers, vector<int>({1, 2}));
  }
}

// Р­С‚Рѕ СЃРїРµС†РёР°Р»СЊРЅС‹Р№ С‚РёРї, РєРѕС‚РѕСЂС‹Р№ РїРѕРјРѕР¶РµС‚ РІР°Рј СѓР±РµРґРёС‚СЊСЃСЏ, С‡С‚Рѕ РІР°С€Р° СЂРµР°Р»РёР·Р°С†РёСЏ
// С„СѓРЅРєС†РёРё MakeJosephusPermutation РЅРµ РІС‹РїРѕР»РЅСЏРµС‚ РєРѕРїРёСЂРѕРІР°РЅРёРµ РѕР±СЉРµРєС‚РѕРІ.
// РЎРµР№С‡Р°СЃ РІС‹, РІРѕР·РјРѕР¶РЅРѕ, РЅРµ РїРѕРЅРёРјР°РµС‚Рµ РєР°Рє РѕРЅ СѓСЃС‚СЂРѕРµРЅ, РѕРґРЅР°РєРѕ РјС‹ СЂР°СЃСЃРєР°Р¶РµРј,
// РїРѕС‡РµРјСѓ РѕРЅ СѓСЃС‚СЂРѕРµРЅ РёРјРµРЅРЅРѕ С‚Р°Рє, РґР°Р»РµРµ РІ Р±Р»РѕРєРµ РїСЂРѕ move-СЃРµРјР°РЅС‚РёРєСѓ вЂ”
// РІ РІРёРґРµРѕ В«РќРµРєРѕРїРёСЂСѓРµРјС‹Рµ С‚РёРїС‹В»

struct NoncopyableInt {
  int value;

  NoncopyableInt(int value) : value(value) {}

  NoncopyableInt(const NoncopyableInt&) = delete;
  NoncopyableInt& operator=(const NoncopyableInt&) = delete;

  NoncopyableInt(NoncopyableInt&&) = default;
  NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
  return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
  return os << v.value;
}

void TestAvoidsCopying() {
  vector<NoncopyableInt> numbers;
  numbers.push_back({1});
  numbers.push_back({2});
  numbers.push_back({3});
  numbers.push_back({4});
  numbers.push_back({5});

  MakeJosephusPermutation(begin(numbers), end(numbers), 2);

  vector<NoncopyableInt> expected;
  expected.push_back({1});
  expected.push_back({3});
  expected.push_back({5});
  expected.push_back({4});
  expected.push_back({2});

  ASSERT_EQUAL(numbers, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  RUN_TEST(tr, TestAvoidsCopying);
  return 0;
}