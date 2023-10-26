#include "responses.h"

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if(r.buses.size() == 0) {
        return os << "No stop";
    }
    for(const auto& bus : r.buses) {
        os << bus << " ";
    }
    return os;
}

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
