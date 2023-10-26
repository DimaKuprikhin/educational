#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

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

istream& operator>>(istream& is, Query& q) {
    std::string queryType;
    is >> queryType;
    if(queryType == "NEW_BUS") {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int count;
        is >> count;
        q.stops.resize(count);
        for(int i = 0; i < count; ++i) {
            is >> q.stops[i];
        }
    }
    else if(queryType == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    }
    else if(queryType == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    }
    else {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    std::vector<std::string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if(r.buses.size() == 0) {
        return os << "No stop";
    }
    for(const auto& bus : r.buses) {
        os << bus << " ";
    }
    return os;
}

struct StopsForBusResponse {
    std::string bus;
    std::vector<std::pair<std::string, std::vector<std::string>>> stops;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if(r.stops.size() == 0) {
        return os << "No bus";
    }
    for(int i = 0; i < r.stops.size(); ++i) {
        const auto& stop = r.stops[i];
        os << "Stop " << stop.first << ":";
        if(stop.second.size() == 1) {
            os << " no interchange\n";
            continue;
        }
        for(const auto& bus : stop.second) {
            if(bus != r.bus) {
                os << " " << bus;
            }
        }
        if(i < r.stops.size() - 1) {
            os << "\n";
        }
    }
    return os;
}

struct AllBusesResponse {
    std::vector<std::pair<std::string, std::vector<std::string>>> buses;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if(r.buses.size() == 0) {
        return os << "No buses";
    }
    for(const auto& [bus, stops] : r.buses) {
        os << "Bus " << bus << ": ";
        for(const auto& stop : stops) {
            os << stop << " ";
        }
        std::cout << "\n";
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        buses[bus] = stops;
        for(const auto& stop : stops) {
            this->stops[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse response;
        if(stops.find(stop) != stops.end()) {
            for(const auto& bus : stops.at(stop)) {
                response.buses.push_back(bus);
            }
        }
        return response;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse response;
        response.bus = bus;
        if(buses.find(bus) != buses.end()) {
            for(const auto& stop : buses.at(bus)) {
                response.stops.push_back({ stop, stops.at(stop) });
            }
        }
        return response;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse response;
        for(const auto& [bus, stops] : buses) {
            response.buses.push_back({ bus, stops });
        }
        return response;
    }

private:
    std::map<std::string, std::vector<std::string>> buses;
    std::map<std::string, std::vector<std::string>> stops;
};

int main() {
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
}