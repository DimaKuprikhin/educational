#include "phone_number.h"
#include <stdexcept>

PhoneNumber::PhoneNumber(const string& internationalNumber) {
    if(internationalNumber[0] != '+') {
        throw std::invalid_argument("number must start with plus");
    }
    int firstDashIndex = -1;
    int secondDashIndex = -1;
    for(int i = 1; i < internationalNumber.size(); ++i) {
        if(internationalNumber[i] == '-') {
            if(firstDashIndex == -1) {
                firstDashIndex = i;
            }
            else {
                secondDashIndex = i;
                break;
            }
        }
    }
    if(secondDashIndex == -1) {
        throw std::invalid_argument("number must contain two dashes");
    }
    country_code_ = internationalNumber.substr(1, firstDashIndex - 1);
    city_code_ = internationalNumber.substr(firstDashIndex + 1, secondDashIndex - firstDashIndex - 1);
    local_number_ = internationalNumber.substr(secondDashIndex + 1);
    if(country_code_.size() == 0 || city_code_.size() == 0 || local_number_.size() == 0) {
        throw std::invalid_argument("codes can't be empty");
    }
}

string PhoneNumber::GetCountryCode() const {
    return country_code_;
}

string PhoneNumber::GetCityCode() const {
    return city_code_;
}

string PhoneNumber::GetLocalNumber() const {
    return local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
    return '+' + country_code_ + '-' + city_code_ + '-' + local_number_;
}
