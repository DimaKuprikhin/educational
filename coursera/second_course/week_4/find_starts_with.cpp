#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

template<typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt begin, RandomIt end, char prefix) {
    struct Comp {
        bool operator()(const std::string& s, const char prefix) const {
            return s[0] < prefix;
        }
        bool operator()(const char prefix, const std::string& s) const {
            return prefix < s[0];
        }
    };
    return std::equal_range(begin, end, prefix, Comp() );
}

template<typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt begin,
                                             RandomIt end,
                                             const std::string& prefix)
{
    return { 
        std::lower_bound(begin, end, prefix,
            [](const std::string& s, const std::string& prefix)-> bool {
                return std::lexicographical_compare(s.begin(), s.begin() + prefix.size(),
                                                    prefix.begin(), prefix.end());
            }),
        std::upper_bound(begin, end, prefix,
            [](const std::string& prefix, const std::string& s)-> bool {
                return std::lexicographical_compare(prefix.begin(), prefix.end(),
                                                    s.begin(), s.begin() + prefix.size());
            })
    };
}

int main() {
    {
    const vector<string> sorted_strings = {"moscow", "murmansk", "vologda"};
  
  const auto m_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
  for (auto it = m_result.first; it != m_result.second; ++it) {
    cout << *it << " ";
  }
  cout << endl;
  
  const auto p_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
  cout << (p_result.first - begin(sorted_strings)) << " " <<
      (p_result.second - begin(sorted_strings)) << endl;
  
  const auto z_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
  cout << (z_result.first - begin(sorted_strings)) << " " <<
      (z_result.second - begin(sorted_strings)) << endl;
    }
{
      const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};
  
  const auto mo_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
  for (auto it = mo_result.first; it != mo_result.second; ++it) {
    cout << *it << " ";
  }
  cout << endl;
  
  const auto mt_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
  cout << (mt_result.first - begin(sorted_strings)) << " " <<
      (mt_result.second - begin(sorted_strings)) << endl;
  
  const auto na_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
  cout << (na_result.first - begin(sorted_strings)) << " " <<
      (na_result.second - begin(sorted_strings)) << endl;
}
  
  return 0;
}
