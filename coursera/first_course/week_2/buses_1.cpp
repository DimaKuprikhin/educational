#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    std::map<std::string, std::vector<std::string>> buses;
    std::map<std::string, std::vector<std::string>> stops;

    int queries;
    std::cin >> queries;
    while(queries--) {
        std::string requestType;
        std::cin >> requestType;
        if(requestType == "NEW_BUS") {
            std::string bus;
            int stopCount;
            std::cin >> bus >> stopCount;
            while(stopCount--) {
                std::string stop;
                std::cin >> stop;
                buses[bus].push_back(stop);
                stops[stop].push_back(bus);
            }
        }
        if(requestType == "BUSES_FOR_STOP") {
            std::string stop;
            std::cin >> stop;
            const auto it = stops.find(stop);
            if(it == stops.end()) {
                std::cout << "No stop\n";
            }
            else {
                for(const auto& bus : it->second) {
                    std::cout << bus << " ";
                }
                std::cout << "\n";
            }
        }
        if(requestType == "STOPS_FOR_BUS") {
            std::string bus;
            std::cin >> bus;
            const auto it = buses.find(bus);
            if(it == buses.end()) {
                std::cout << "No bus\n";
            }
            else {
                for(const auto& stop : it->second) {
                    std::cout << "Stop " << stop << ":";
                    if(stops[stop].size() == 1) {
                        std::cout << " no interchange\n";
                    }
                    else {
                        for(const auto& busThroughStop : stops[stop]) {
                            if(busThroughStop != bus) {
                                std::cout << " " << busThroughStop;
                            }
                        }
                        std::cout << "\n";
                    }
                }
            }
        }
        if(requestType == "ALL_BUSES") {
            if(buses.size() == 0) {
                std::cout << "No buses\n";
            }
            for(const auto& busIter : buses) {
                std::cout << "Bus " << busIter.first << ":";
                for(const auto& stop : busIter.second) {
                    std::cout << " " << stop;
                }
                std::cout << "\n";
            }
        }
    }
}

