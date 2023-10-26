#include <iostream>
#include <map>
#include <queue>
#include <string>

struct Event {
    int64_t time;
    int64_t clientId;
    int64_t rooms;
};
struct Hotel {
    std::map<int64_t, int64_t> clients;
    int64_t roomsSum = 0;
    std::queue<Event> events;
};

void update(Hotel& hotel, int64_t currentTime) {
    while(hotel.events.size() > 0 && hotel.events.front().time <= currentTime - 86400) {
        const auto& event = hotel.events.front();
        hotel.roomsSum -= event.rooms;
        hotel.clients[event.clientId]--;
        if(hotel.clients[event.clientId] == 0) {
            hotel.clients.erase(event.clientId);
        }
        hotel.events.pop();
    }
}

int main() {
    int queriesCount;
    std::cin >> queriesCount;

    std::map<std::string, Hotel> hotels;
    int64_t currentTime = 0;
    
    while(queriesCount--) {
        std::string type, hotelName;
        int64_t time, clientId, rooms;

        std::cin >> type;
        if(type == "BOOK") {
            std::cin >> time >> hotelName >> clientId >> rooms;
            currentTime = time;
            auto& hotel = hotels[hotelName];
            hotel.events.push({ time, clientId, rooms });
            hotel.roomsSum += rooms;
            hotel.clients[clientId]++;
        }
        else if(type == "CLIENTS") {
            std::cin >> hotelName;
            update(hotels[hotelName], currentTime);
            std::cout << hotels[hotelName].clients.size() << "\n";
        }
        else if(type == "ROOMS") {
            std::cin >> hotelName;
            update(hotels[hotelName], currentTime);
            std::cout << hotels[hotelName].roomsSum << "\n";
        }
        else {
            throw 1;
        }
    }
}
