#include "search_server.h"
#include "iterator_range.h"
#include "parse.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <string_view>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  index = move(new_index);
  // index.Print(std::cout);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  vector<size_t> docid_count(index.docs.size(), 0);
  for (string current_query; getline(query_input, current_query); ) {
    docid_count.assign(index.docs.size(), 0);
    const auto words = SplitIntoWords(current_query);

    for (const auto& word : words) {
      for (const auto [docid, count] : index.Lookup(word)) {
        docid_count[docid] += count;
      }
    }

    struct TopFive {
      std::array<pair<size_t, size_t>, 6> values;
      size_t size = 0;

      void Add(pair<size_t, size_t> value) {
        if(value.second == 0) {
          return;
        }
        values[size++] = value;
        for(int i = size - 1; i >= 1; --i) {
          if(values[i].second > values[i - 1].second) {
            swap(values[i], values[i - 1]);
          }
        }
        size = size > 5 ? 5 : size;
      }
    };
    TopFive top;
    for(size_t i = 0; i < docid_count.size(); ++i) {
      top.Add({ i, docid_count[i] });
    }

    search_results_output << current_query << ':';
    for(size_t i = 0; i < top.size; ++i) {
      size_t docid = top.values[i].first;
      size_t hitcount = top.values[i].second;
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;
  }
}

void InvertedIndex::Add(string document) {
  docs.push_back(move(document));
  const size_t docid = docs.size() - 1;
  std::vector<string_view> words;
  words.reserve(1000);
  for (const auto& word : SplitBySV(docs.back(), ' ', words)) {
    ++index[word][docid];
  }
}

const unordered_map<size_t, size_t>& InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    static unordered_map<size_t, size_t> empty;
    return empty;
  }
}
