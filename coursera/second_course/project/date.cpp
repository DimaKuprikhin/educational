#include "date.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

Date::Date(int year, int month, int day)
    : year(year)
    , month(month)
    , day(day)
    {}

int Date::GetDay() const {
    return day;
}

int Date::GetMonth() const {
    return month;
}

int Date::GetYear() const {
    return year;
}

Date Date::GetPrevious() const {
    Date result(day, month, year);
    --result.day;
    if(result.day < 1) {
        --result.month;
        result.day = 31;
    }
    if(result.month < 1) {
        --result.year;
        result.month = 12;
    }
    return result;
}

bool Date::operator<(const Date& other) const {
    if(this->year != other.year) {
        return this->year < other.year;
    }
    if(this->month != other.month) {
        return this->month < other.month;
    }
    return this->day < other.day;
}

bool Date::operator>(const Date& other) const {
    return other < *this;
}

bool Date::operator<=(const Date& other) const {
    return !(other < *this);
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

bool Date::operator!=(const Date& other) const {
    return *this < other || other < *this;
}

bool Date::operator==(const Date& other) const {
    return !(*this != other);
}

Date ParseDate(std::istream& stream) {
    std::string date;
    try {
        stream >> std::ws;
        std::getline(stream, date, ' ');
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
        std::vector<std::string> splitted = { date.substr(0, firstDash),
                     date.substr(firstDash + 1, secondDash - firstDash - 1),
                     date.substr(secondDash + 1) };
        for(const auto& part : splitted) {
            for(const char c : part) {
                if((c < '0' || c > '9') && c != '-' && c != '+') {
                    throw 1;
                }
            }
        }
        Date result(std::stoi(splitted[0]), std::stoi(splitted[1]), std::stoi(splitted[2]));
        return result;
    } catch(...) {
        throw std::runtime_error("Wrong date format: " + date);
    }
}

std::ostream& operator<<(std::ostream& stream, const Date& date) {
    return stream << std::setw(4) << std::setfill('0') << date.GetYear() << '-'
                    << std::setw(2) << std::setfill('0') << date.GetMonth() << '-'
                    << std::setw(2) << std::setfill('0') << date.GetDay();
}
