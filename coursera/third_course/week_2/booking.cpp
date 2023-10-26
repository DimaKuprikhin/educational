#include <iostream>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

class BookingSystem {
public:
    void Book(int64_t time, const std::string& hotelName, int64_t clientId, int64_t rooms) {
        currentTime = std::max(currentTime, time);
        hotels[hotelName].Book(time, clientId, rooms);
    }

    int64_t Clients(const std::string& hotelName) {
        if(hotels.find(hotelName) == hotels.end()) {
            return 0;
        }
        return hotels.at(hotelName).Clients(currentTime);
    }

    int64_t Rooms(const std::string& hotelName) {
        if(hotels.find(hotelName) == hotels.end()) {
            return 0;
        }
        return hotels.at(hotelName).Rooms(currentTime);
    }

private:
    class HotelData {
    public:
        void Book(int64_t time, int64_t clientId, int64_t rooms) {
            bookings.push_front({ time, clientId, rooms });
            uniqueClients.insert(clientId);
            clients.insert(clientId);
            roomsSum += rooms;
        }

        int64_t Clients(int64_t currentTime) {
            Update(currentTime);
            return uniqueClients.size();
        }

        int64_t Rooms(int64_t currentTime) {
            Update(currentTime);
            return roomsSum;
        }

    private:
        void Update(int64_t currentTime) {
            while(!bookings.empty() && bookings.back().time <= currentTime - 86400) {
                clients.erase(clients.find(bookings.back().clientId));
                if(clients.find(bookings.back().clientId) == clients.end()) {
                    uniqueClients.erase(bookings.back().clientId);
                }
                roomsSum -= bookings.back().rooms;
                bookings.pop_back();
            }
        }

        struct Booking {
            int64_t time;
            int64_t clientId;
            int64_t rooms;
        };

        std::deque<Booking> bookings;
        std::unordered_multiset<int64_t> clients;
        std::unordered_set<int64_t> uniqueClients;
        int64_t roomsSum;
    };

    std::unordered_map<std::string, HotelData> hotels;
    int64_t currentTime = 0;

};

struct Query {
    std::string queryType;
    std::string hotelName;
    int64_t time;
    int64_t clientId;
    int64_t roomCount;
};

std::istream& operator>>(std::istream& stream, Query& query) {
    stream >> query.queryType;
    if(query.queryType == "BOOK") {
        return stream >> query.time >> query.hotelName >> query.clientId >> query.roomCount;
    }
    if(query.queryType == "CLIENTS") {
        return stream >> query.hotelName;
    }
    if(query.queryType == "ROOMS") {
        return stream >> query.hotelName;
    }
    throw std::invalid_argument("Invalid query type.");
}

struct Response {
    std::string queryType;
    int64_t response;
};

std::ostream& operator<<(std::ostream& stream, const Response& response) {
    if(response.queryType == "BOOK") {
        return stream;
    }
    if(response.queryType == "CLIENTS" || response.queryType == "ROOMS") {
        return stream << response.response;
    }
    throw std::invalid_argument("Invalid query type in response.");
}

void ExecQuery(BookingSystem& system, const Query& query, std::ostream& stream) {
    if(query.queryType == "BOOK") {
        system.Book(query.time, query.hotelName, query.clientId, query.roomCount);
        return;
    }
    if(query.queryType == "CLIENTS") {
        stream << Response{ query.queryType, system.Clients(query.hotelName) } << "\n";
        return;
    }
    if(query.queryType == "ROOMS") {
        stream << Response{ query.queryType, system.Rooms(query.hotelName) } << "\n";
        return;
    }
}

int main() {
    int queryCount;
    std::cin >> queryCount;
    BookingSystem system;
    while(queryCount--) {
        Query query;
        std::cin >> query;
        ExecQuery(system, query, std::cout);
    }
}
