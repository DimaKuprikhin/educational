#pragma once
#include "date.h"
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <unordered_set>

class Database {
public:
    struct Entry {
        const Date date;
        const std::string event;
    };

    void Add(const Date& date, const std::string& event);
    void Print(std::ostream& stream) const;
    int RemoveIf(const std::function<int(const Date&, const std::string&)>& predicate);
    std::vector<Entry> FindIf(const std::function<int(const Date&, const std::string&)>& predicate) const;
    Entry Last(const Date& date) const;

private:
    struct MapCmp {
        bool operator()(const Date& lhs, const Date& rhs) const {
            return lhs > rhs;
        }
    };
    struct VectorSet {
        std::vector<std::string> vector;
        std::unordered_set<std::string> set;
        void push_back(const std::string& value) {
            vector.push_back(value);
            set.insert(value);
        }
        void pop_back() {
            set.erase(vector.back());
            vector.pop_back();
        }
        bool find(const std::string& value) const { return set.find(value) != set.end(); }
        const std::string& back() const { return vector.back(); }
        auto begin() const { return vector.begin(); }
        auto end() const { return vector.end(); }
        size_t size() const { return vector.size(); }
    };
    std::map<Date, VectorSet, MapCmp> db;

};

std::ostream& operator<<(std::ostream& stream, const Database::Entry& entry);
