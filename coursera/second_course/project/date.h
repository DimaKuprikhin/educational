#pragma once

#include <iostream>

class Date {
public:
    Date(int year, int month, int day);
    int GetDay() const;
    int GetMonth() const;
    int GetYear() const;
    Date GetPrevious() const;
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;

private:
    int year;
    int month;
    int day;
};

Date ParseDate(std::istream& stream);

std::ostream& operator<<(std::ostream& stream, const Date& date);
