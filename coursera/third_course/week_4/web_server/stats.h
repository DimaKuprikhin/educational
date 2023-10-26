#pragma once

#include "http_request.h"

#include <array>
#include <string_view>
#include <string>
#include <map>
using namespace std;

class Stats {
public:
  Stats();

  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;

private:
  int FindInArray(std::string* array, size_t size, std::string_view value) {
    for(int i = 0; i < size; ++i) {
      if(array[i] == value) {
        return i;
      }
    }
    return size;
  }

  std::string methods[4];
  std::string uries[5];
  std::string unknownMethod = "UNKNOWN";
  std::string unknownUri = "unknown";

  std::array<int, 5> methodCounts = { 0, 0, 0, 0, 0 };
  std::array<int, 6> uriCounts = { 0, 0, 0, 0, 0, 0 };
  mutable std::map<string_view, int> methodCountsMap;
  mutable std::map<string_view, int> uriCountsMap;
};

HttpRequest ParseRequest(string_view line);
