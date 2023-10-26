#include "test_runner.h"
#include "profile.h"

#include <map>
#include <future>
#include <string>
using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other) {
    for(const auto& p : other.word_frequences) {
      word_frequences[p.first] += p.second;
    }
  }
};

Stats ExploreKeyWords2(const set<string>& key_words, const std::vector<std::string>& words) {
  Stats result;
  for(const auto& word : words) {
    if(key_words.find(word) != key_words.end()) {
      ++result.word_frequences[word];
    }
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  std::vector<std::string> inputWords;
  std::string word;
  while(input) {
    input >> word;
    inputWords.push_back(std::move(word));
  }

  std::vector<std::vector<std::string>> words;
  words.reserve(4);
  std::vector<std::future<Stats>> futures;
  futures.reserve(4);
  Stats result;
  for(int i = 0; i < 4; ++i) {
    auto begin = inputWords.begin() + i * inputWords.size() / 4;
    auto end = inputWords.begin() + (i + 1) * inputWords.size() / 4;
    words.push_back({ std::make_move_iterator(begin), std::make_move_iterator(end) });
    futures.push_back(std::async(ExploreKeyWords2, cref(key_words), cref(words.back())));
  }
  
  for(auto& future : futures) {
    result += future.get();
  }
  return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}