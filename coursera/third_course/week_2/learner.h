#include <algorithm>
#include <string>
#include <set>
#include <vector>

using namespace std;

class Learner {
 private:
  set<string> setDict;
  mutable vector<string> dict;
  mutable bool validDict = true;

 public:
  int Learn(const vector<string>& words) {
      int result = setDict.size();
      setDict.insert(words.begin(), words.end());
      result = setDict.size() - result;
      if(result > 0) {
          validDict = false;
      }
      return result;
  }

  const vector<string>& KnownWords() const {
    if(!validDict) {
        dict.clear();
        dict.reserve(setDict.size());
        dict.insert(dict.end(), setDict.begin(), setDict.end());
        validDict = true;
    }
    return dict;
  }
};