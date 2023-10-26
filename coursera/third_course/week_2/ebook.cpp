#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
public:
  ReadingManager()
    : pagesById(ReadingManager::MAX_USER_COUNT_, 0)
    , usersWithPages(ReadingManager::MAX_PAGES, 0)
    {}

  void Read(int user_id, int page_count) {
    const int prevPages = pagesById[user_id];
    if(prevPages > 0) {
      --usersWithPages[prevPages];
    }
    else {
      ++users;
    }
    pagesById[user_id] = page_count;
    ++usersWithPages[page_count];
  }

  double Cheer(int user_id) const {
    if(pagesById[user_id] == 0) {
      return 0;
    }
    if(users == 1) {
      return 1;
    }
    int count = 0;
    for(int i = pagesById[user_id] - 1; i >= 1; --i) {
      count += usersWithPages[i];
    }
    return (double)count / (users - 1.0);
  }

private:
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGES = 1000;

  vector<int> pagesById;
  vector<int> usersWithPages;
  int users = 0;
};


int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}