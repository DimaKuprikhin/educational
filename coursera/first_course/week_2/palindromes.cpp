#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> PalindromFilter(const std::vector<std::string>& words, int minLength) {
    std::vector<std::string> result;
    for(const auto& word : words) {
        if(word.size() < minLength) {
            continue;
        }
        bool isPalindrome = true;
        for(int i = 0; i < word.size() / 2; ++i) {
            if(word[i] != word[word.size() - i - 1]) {
                isPalindrome = false;
                break;
            }
        }
        if(isPalindrome) {
            result.push_back(word);
        }
    }
    return result;
}

