#include "database.h"
#include <algorithm>
#include <stdexcept>

void Database::Add(const Date& date, const std::string& event) {
    if(db[date].find(event)) {
        return;
    }
    db[date].push_back(event);
}

void Database::Print(std::ostream& stream) const {
    for(auto iter = db.rbegin(); iter != db.rend(); ++iter) {
        const auto& date = iter->first;
        const auto& events = iter->second;
        for(const auto& event : events) {
            stream << date << ' ' << event << std::endl;
        }
    }
}

int Database::RemoveIf(const std::function<int(const Date&, const std::string&)>& predicate) {
    int removed = 0;
    std::map<Date, VectorSet, MapCmp> newDb;
    for(auto& [date, events] : db) {
        int sizeBefore = events.size();
        const auto it = std::stable_partition(events.vector.begin(), events.vector.end(),
            [&predicate, date = date](const auto& event)-> bool {
                return predicate(date, event) == 0;
            });
        while(events.end() != it) {
            events.pop_back();
        }
        removed += sizeBefore - events.size();
        if(events.size() > 0) {
            newDb.insert({ date, std::move(events) });
        }
    }
    db = std::move(newDb);
    return removed;
}

std::vector<Database::Entry>
Database::FindIf(const std::function<int(const Date&, const std::string&)>& predicate) const {
    std::vector<Database::Entry> result;
    for(auto iter = db.rbegin(); iter != db.rend(); ++iter) {
        const auto& date = iter->first;
        const auto& events = iter->second;
        std::vector<std::string> eventsInResult(events.size());
        const auto it = std::copy_if(events.vector.begin(), events.vector.end(), eventsInResult.begin(),
            [&predicate, date = date](const auto& event)-> bool {
                return predicate(date, event) != 0;
            });
        while(eventsInResult.end() != it) {
            eventsInResult.pop_back();
        }
        for(auto&& event : eventsInResult) {
            result.push_back({ date, std::move(event) });
        }
    }
    return result;
}

Database::Entry Database::Last(const Date& date) const {
    auto iter = db.lower_bound(date);
    if(iter == db.end()) {
        throw std::invalid_argument("");
    }
    return { iter->first, iter->second.back() };
}

std::ostream& operator<<(std::ostream& stream, const Database::Entry& entry) {
    return stream << entry.date << ' ' << entry.event;
}
