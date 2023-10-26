#include "query.h"

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
