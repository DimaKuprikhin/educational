#include "bus_manager.h"

void BusManager::AddBus(const string& bus, const vector<string>& stops) {
    buses[bus] = stops;
    for(const auto& stop : stops) {
        this->stops[stop].push_back(bus);
    }
}

BusesForStopResponse BusManager::GetBusesForStop(const string& stop) const {
    BusesForStopResponse response;
    if(stops.find(stop) != stops.end()) {
        for(const auto& bus : stops.at(stop)) {
            response.buses.push_back(bus);
        }
    }
    return response;
}

StopsForBusResponse BusManager::GetStopsForBus(const string& bus) const {
    StopsForBusResponse response;
    response.bus = bus;
    if(buses.find(bus) != buses.end()) {
        for(const auto& stop : buses.at(bus)) {
            response.stops.push_back({ stop, stops.at(stop) });
        }
    }
    return response;
}

AllBusesResponse BusManager::GetAllBuses() const {
    AllBusesResponse response;
    for(const auto& [bus, stops] : buses) {
        response.buses.push_back({ bus, stops });
    }
    return response;
}
