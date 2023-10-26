#pragma once
#include <iostream>
#include <string>
#include <vector>
using std::istream;
using std::string;
using std::vector;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q);
