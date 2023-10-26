#include "test_runner.h"

#include <vector>

using namespace std;

template <typename Token>
using Sentence = vector<Token>;

// РљР»Р°СЃСЃ Token РёРјРµРµС‚ РјРµС‚РѕРґ bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    vector<Sentence<Token>> result(1);
    bool isPrevTokenEnd = false;
    for(auto& token : tokens) {
        const bool isCurrEnd = token.IsEndSentencePunctuation();
        if(isPrevTokenEnd && !isCurrEnd) {
            result.push_back({});
        }
        isPrevTokenEnd = isCurrEnd;
        result.back().push_back(std::move(token));
    }
    return result;
}


struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
  return stream << token.data;
}

// РўРµСЃС‚ СЃРѕРґРµСЂР¶РёС‚ РєРѕРїРёСЂРѕРІР°РЅРёСЏ РѕР±СЉРµРєС‚РѕРІ РєР»Р°СЃСЃР° TestToken.
// Р”Р»СЏ РїСЂРѕРІРµСЂРєРё РѕС‚СЃСѓС‚СЃС‚РІРёСЏ РєРѕРїРёСЂРѕРІР°РЅРёР№ РІ С„СѓРЅРєС†РёРё SplitIntoSentences
// РЅРµРѕР±С…РѕРґРёРјРѕ РЅР°РїРёСЃР°С‚СЊ РѕС‚РґРµР»СЊРЅС‹Р№ С‚РµСЃС‚.
void TestSplitting() {
  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!"}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
        {{"Without"}, {"copies"}, {".", true}},
    })
  );
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);
  return 0;
}