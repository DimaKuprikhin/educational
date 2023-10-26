#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> SplitIntoWords(const std::string& s) {
    std::vector<std::string> result;
    int beginIndex = 0;
    for(int i = 0; i < s.size(); ++i) {
        if(s[i] == ' ') {
            result.push_back(s.substr(beginIndex, i - beginIndex));
            beginIndex = i + 1;
        }
    }
    result.push_back(s.substr(beginIndex));
    return result;
}

int main() {
  std::string s = "C Cpp Java Python";

  std::vector<std::string> words = SplitIntoWords(s);
  std::cout << words.size() << " ";
  for (auto it = std::begin(words); it != std::end(words); ++it) {
    if (it != std::begin(words)) {
      std::cout << "/";
    }
    std::cout << *it;
  }
  std::cout << std::endl;
  
  return 0;
}
