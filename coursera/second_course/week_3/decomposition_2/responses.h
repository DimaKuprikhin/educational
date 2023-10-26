#pragma once
#include <iostream>
#include <string>
#include <vector>
using std::ostream;
using std::pair;
using std::string;
using std::vector;

struct BusesForStopResponse {
    vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r);

struct StopsForBusResponse {
    string bus;
    vector<pair<string, vector<string>>> stops;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r);

struct AllBusesResponse {
    vector<pair<string, vector<string>>> buses;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r);
