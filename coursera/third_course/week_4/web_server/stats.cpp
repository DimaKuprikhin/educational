#include "stats.h"
#include <iostream>

Stats::Stats() {
    methods[0] = "GET";
    methods[1] = "POST";
    methods[2] = "PUT";
    methods[3] = "DELETE";
    uries[0] = "/";
    uries[1] = "/order";
    uries[2] = "/product";
    uries[3] = "/basket";
    uries[4] = "/help";
    for(int i = 0; i < 4; ++i) {
        methodCountsMap.insert({ methods[i], 0 });
    }
    methodCountsMap.insert({ unknownMethod, 0 });
    for(int i = 0; i < 5; ++i) {
        uriCountsMap.insert({ uries[i], 0 });
    }
    uriCountsMap.insert({ unknownUri, 0 });
}

void Stats::AddMethod(string_view method) {
    ++methodCounts[FindInArray(methods, 4, method)];
}

void Stats::AddUri(string_view uri) {
    ++uriCounts[FindInArray(uries, 5, uri)];
}

const std::map<string_view, int>& Stats::GetMethodStats() const {
    for(int i = 0; i < 4; ++i) {
        methodCountsMap[methods[i]] = methodCounts[i];
    }
    methodCountsMap[unknownMethod] = methodCounts[4];
    return methodCountsMap;
}

const std::map<string_view, int>& Stats::GetUriStats() const {
    for(int i = 0; i < 5; ++i) {
        uriCountsMap[uries[i]] = uriCounts[i];
    }
    uriCountsMap[unknownUri] = uriCounts[5];
    return uriCountsMap;
}

HttpRequest ParseRequest(std::string_view line) {
    // std::cout << line << " ";
    while(line[0] == ' ') {
        line.remove_prefix(1);
    }
    HttpRequest result;
    size_t space = line.find(' ');
    result.method = line.substr(0, space);
    line.remove_prefix(space + 1);
    while(line[0] == ' ') {
        line.remove_prefix(1);
    }
    space = line.find(' ');
    result.uri = line.substr(0, space);
    line.remove_prefix(space + 1);
    result.protocol = line;
    // std::cout << "[" << result.method << "] [" << result.uri << "]\n";
    return result;
}
