#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

#define UPDATE_FIELD(ticket, field, values) \
{                                           \
    auto it = values.find(#field);          \
    if(it != values.end()) {                \
        istringstream is(it->second);       \
        is >> ticket.field;                 \
    }                                       \
}


bool operator==(const Date& lhs, const Date& rhs) {
    return lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day;
}

std::ostream& operator<<(std::ostream& stream, const Date& date) {
    return stream << date.year << '-' << date.month << '-' << date.day;
}

std::istream& operator>>(std::istream& stream, Date& date) {
    stream >> std::ws;
    std::string year, month, day;
    std::getline(stream, year, '-');
    std::getline(stream, month, '-');
    stream >> day;
    date.year = std::stoi(year);
    date.month = std::stoi(month);
    date.day = std::stoi(day);
    return stream;
}

bool operator==(const Time& lhs, const Time& rhs) {
    return lhs.hours == rhs.hours && lhs.minutes == rhs.minutes;
}

std::ostream& operator<<(std::ostream& stream, const Time& time) {
    return stream << time.hours << ':' << time.minutes;
}

std::istream& operator>>(std::istream& stream, Time& time) {
    stream >> std::ws;
    std::string hours, minutes;
    std::getline(stream, hours, ':');
    stream >> minutes;
    time.hours = std::stoi(hours);
    time.minutes = std::stoi(minutes);
    return stream;
}

void TestUpdate() {
  AirlineTicket t;
  t.price = 0;

  const map<string, string> updates1 = {
    {"departure_date", "2018-2-28"},
    {"departure_time", "17:40"},
  };
  UPDATE_FIELD(t, departure_date, updates1);
  UPDATE_FIELD(t, departure_time, updates1);
  UPDATE_FIELD(t, price, updates1);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 0);

  const map<string, string> updates2 = {
    {"price", "12550"},
    {"arrival_time", "20:33"},
  };
  UPDATE_FIELD(t, departure_date, updates2);
  UPDATE_FIELD(t, departure_time, updates2);
  UPDATE_FIELD(t, arrival_time, updates2);
  UPDATE_FIELD(t, price, updates2);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 12550);
  ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);
}