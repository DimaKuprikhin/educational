#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>

using std::set;
using std::map;
using std::string;
using std::stringstream;
using std::vector;

vector<string> Split(const string& string, const char delim) {
    vector<std::string> result;
    int beginIndex = 0;
    for(int i = 0; i < string.size(); ++i) {
        if(string[i] == delim) {
            result.push_back(string.substr(beginIndex, i - beginIndex));
            beginIndex = i + 1;
        }
    }
    if(beginIndex < string.size()) {
        result.push_back(string.substr(beginIndex));
    }
    return result;
}

// *-[-]*-[-]*
/*
    1234--1--2
*/

class Date {
public:
    Date(const string& date) {
        // const auto splitted = Split(date, '-');
        vector<string> splitted;
        try {
            int firstDash = -1, secondDash = -1;
            for(int i = 0; i < date.size(); ++i) {
                if(date[i] == '-') {
                    if(firstDash == -1) firstDash = i;
                    else if(i == firstDash + 1) {}
                    else if(secondDash == -1) secondDash = i;
                    else if(i == secondDash + 1) {}
                    else throw 1;
                }
            }
            if(secondDash == -1) {
                throw 1;
            }
            splitted = { date.substr(0, firstDash),
                         date.substr(firstDash + 1, secondDash - firstDash - 1),
                         date.substr(secondDash + 1) };
            for(const auto& part : splitted) {
                for(const char c : part) {
                    if((c < '0' || c > '9') && c != '-' && c != '+') {
                        throw 1;
                    }
                }
            }
            year = std::stoi(splitted[0]);
            month = std::stoi(splitted[1]);
            day = std::stoi(splitted[2]);
        } catch(...) {
            throw std::runtime_error("Wrong date format: " + date);
        }
        if(month < 1 || month > 12) {
            throw std::runtime_error("Month value is invalid: " + splitted[1]);
        }
        if(day < 1 || day > 31) {
            throw std::runtime_error("Day value is invalid: " + splitted[2]);
        }
    }

    int GetYear() const {
        return year;
    }

    int GetMonth() const {
        return month;
    }

    int GetDay() const {
        return day;
    }

    string ToString() const {
        stringstream result;
        result << std::setw(4) << std::setfill('0') << year << '-'
                << std::setw(2) << std::setfill('0') << month << '-'
                << std::setw(2) << std::setfill('0') << day;
        return result.str();
    }

private:
    int day;
    int month;
    int year;
};

bool operator<(const Date& lhs, const Date& rhs) {
    if(lhs.GetYear() != rhs.GetYear()) {
        return lhs.GetYear() < rhs.GetYear();
    }
    if(lhs.GetMonth() != rhs.GetMonth()) {
        return lhs.GetMonth() < rhs.GetMonth();
    }
    return lhs.GetDay() < rhs.GetDay();
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        events[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const string& event) {
        const auto iter = events[date].find(event);
        if(iter == events[date].end()) {
            return false;
        }
        events[date].erase(iter);
        return true;
    }

    int DeleteDate(const Date& date) {
        int result = events[date].size();
        events[date].clear();
        return result;
    }

    vector<string> Find(const Date& date) const {
        vector<string> result;
        if(events.find(date) == events.end()) {
            return {};
        }
        for(const auto& event : events.at(date)) {
            result.push_back(event);
        }
        return result;
    }
  
    void Print(std::ostream& stream) const {
        for(const auto& dateEvents : events) {
            for(const auto& event : dateEvents.second) {
                stream << dateEvents.first.ToString() << " " << event << "\n";
            }
        }
    }

private:
    map<Date, set<string>> events;
};

class DBQueriesHandler {
public:
    bool Handle(const std::string& query, std::ostream& stream) {
        const auto splittedQuery = Split(query, ' ');
        if(splittedQuery.size() == 0) {
            return true;
        }
        const auto& queryType = splittedQuery[0];
        try {
            if(queryType == "Add") {
                db.AddEvent({ splittedQuery[1] }, splittedQuery[2]);
            }
            else if(queryType == "Find") {
                for(const auto& event : db.Find({ splittedQuery[1] })) {
                    stream << event << "\n";
                }
            }
            else if(queryType == "Del") {
                if(splittedQuery.size() == 2) {
                    int result = db.DeleteDate({ splittedQuery[1] });
                    stream << "Deleted " << result << " events\n";
                }
                else {
                    if(db.DeleteEvent({ splittedQuery[1] }, splittedQuery[2])) {
                        stream << "Deleted successfully\n";
                    }
                    else {
                        stream << "Event not found\n";
                    }
                }   
            }
            else if(queryType == "Print") {
                db.Print(stream);
            }
            else {
                stream << "Unknown command: " << queryType << "\n";
                return false;
            }
        } catch(const std::exception& ex) {
            stream << ex.what() << "\n";
            return false;
        }
        return true;
    }

private:
    Database db;
};

int main() {
    // std::cout << std::stoi("1%") << "\n";
    // {
    //     DBQueriesHandler handler;
    //     std::stringstream stream;
    //     handler.Handle("Add 0-1-2 event1", stream);
    //     handler.Handle("Add 1-2-3 event2", stream);
    //     handler.Handle("Find 0-1-2", stream);
    //     handler.Handle("", stream);
    //     handler.Handle("Del 0-1-2", stream);
    //     handler.Handle("Print", stream);
    //     handler.Handle("Del 1-2-3 event2", stream);
    //     handler.Handle("Del 1-2-3 event2", stream);
    //     string expected = string("event1\n")
    //                     + "Deleted 1 events\n"
    //                     + "0001-02-03 event2\n"
    //                     + "Deleted successfully\n"
    //                     + "Event not found\n";
    //     std::cout << (expected == stream.str() ? "" : "Wrong: 1\n");
    // }

    // {
    //     DBQueriesHandler handler;
    //     std::stringstream stream;
    //     handler.Handle("Add 1-1-1 event", stream);
    //     handler.Handle("Add 0-1-2 event", stream);
    //     handler.Handle("Add 0-2-1 event", stream);
    //     handler.Handle("Add 0-1-4 event", stream);
    //     handler.Handle("Add 0-1-3 event", stream);
    //     handler.Handle("Add 0-2-2 event", stream);
    //     handler.Handle("Add 4-12-31 event1a", stream);
    //     handler.Handle("Add 4-12-31 event2", stream);
    //     handler.Handle("Add 4-12-31 event1b", stream);
    //     handler.Handle("Print", stream);
    //     string expected = string("0000-01-02 event\n")
    //                     + "0000-01-03 event\n"
    //                     + "0000-01-04 event\n"
    //                     + "0000-02-01 event\n"
    //                     + "0000-02-02 event\n"
    //                     + "0001-01-01 event\n"
    //                     + "0004-12-31 event1a\n"
    //                     + "0004-12-31 event1b\n"
    //                     + "0004-12-31 event2\n";
    //     std::cout << (expected == stream.str() ? "" : "Wrong: 2\n");
    //     std::cout << stream.str() << "\n";

    // }

    DBQueriesHandler handler;
    string query;
    while(std::getline(std::cin, query)) {
        if(!handler.Handle(query, std::cout)) {
            break;
        }
    }

    return 0;
}