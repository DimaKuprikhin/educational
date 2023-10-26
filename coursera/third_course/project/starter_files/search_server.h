#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
using namespace std;

class InvertedIndex {
public:
  InvertedIndex() {
    docs.reserve(50000);
    index.reserve(15000);
  }

  void Add(string document);
  const unordered_map<size_t, size_t>& Lookup(const string& word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

  void Print(std::ostream& output) const {
    // output << "Print()\n";
    // output << "index: [\n";
    // for(const auto& [word, indexes] : index) {
    //   output << "\t{ " << word << " : [ ";
    //   for(auto i : indexes) {
    //     output << i << " ";
    //   }
    //   output << " ] }\n";
    // }
    // output << "]\n\n";
  }

  unordered_map<string_view, unordered_map<size_t, size_t>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
