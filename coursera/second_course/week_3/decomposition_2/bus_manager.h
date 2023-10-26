#pragma once
#include "responses.h"
#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops);

    BusesForStopResponse GetBusesForStop(const string& stop) const;

    StopsForBusResponse GetStopsForBus(const string& bus) const;

    AllBusesResponse GetAllBuses() const;

private:
    map<string, vector<string>> buses;
    map<string, vector<string>> stops;
};