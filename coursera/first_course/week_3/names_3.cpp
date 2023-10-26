#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::cout;
using std::endl;

class Person {
public:

    Person(const std::string& firstName, const std::string& lastName, const int birthYear)
        : birthYear(birthYear)
    {
        firstNames[-birthYear] = firstName;
        lastNames[-birthYear] = lastName;
    }

    void ChangeFirstName(int year, const std::string& first_name) {
        if(year < birthYear) {
            return;
        }
        firstNames[-year] = first_name;
    }

    void ChangeLastName(int year, const std::string& last_name) {
        if(year < birthYear) {
            return;
        }
        lastNames[-year] = last_name;
    }

    std::string GetFullName(int year) const {
        if(year < birthYear) {
            return "No person";
        }
        const auto firstNameIt = firstNames.lower_bound(-year);
        const auto lastNameIt = lastNames.lower_bound(-year);
        if(firstNameIt == firstNames.end() && lastNameIt == lastNames.end()) {
            return "Incognito";
        }
        if(firstNameIt == firstNames.end()) {
            return lastNameIt->second + " with unknown first name";
        }
        if(lastNameIt == lastNames.end()) {
            return firstNameIt->second + " with unknown last name";
        }
        return firstNameIt->second + " " + lastNameIt->second;
    }

    std::string GetFullNameWithHistory(int year) const {
        if(year < birthYear) {
            return "No person";
        }
        const auto firstNameIt = firstNames.lower_bound(-year);
        const auto lastNameIt = lastNames.lower_bound(-year);
        if(firstNameIt == firstNames.end() && lastNameIt == lastNames.end()) {
            return "Incognito";
        }

        std::vector<std::string> firstNamesHistory;
        std::vector<std::string> lastNamesHistory;
        if(firstNameIt != firstNames.end()) {
            firstNamesHistory.push_back(firstNameIt->second);
        }
        if(lastNameIt != lastNames.end()) {
            lastNamesHistory.push_back(lastNameIt->second);
        }
        for(auto it = firstNameIt; it != firstNames.end(); ++it) {
            if(firstNamesHistory.size() == 0 || firstNamesHistory.back() != it->second) {
                firstNamesHistory.push_back(it->second);
            }
        }
        for(auto it = lastNameIt; it != lastNames.end(); ++it) {
            if(lastNamesHistory.size() == 0 || lastNamesHistory.back() != it->second) {
                lastNamesHistory.push_back(it->second);
            }
        }

        std::string firstName = (firstNamesHistory.size() > 0 ? firstNamesHistory[0] : "");
        std::string lastName = (lastNamesHistory.size() > 0 ? lastNamesHistory[0] : "");
        if(firstNamesHistory.size() > 1) {
            firstName += " (";
            for(int i = 1; i < firstNamesHistory.size(); ++i) {
                if(firstName.back() != '(') {
                    firstName += ", ";
                }
                firstName += firstNamesHistory[i];
            }
            firstName += ')';
        }
        if(lastNamesHistory.size() > 1) {
            lastName += " (";
            for(int i = 1; i < lastNamesHistory.size(); ++i) {
                if(lastName.back() != '(') {
                    lastName += ", ";
                }
                lastName += lastNamesHistory[i];
            }
            lastName += ')';
        }

        if(firstName.size() == 0) {
            return lastName + " with unknown first name";
        }
        if(lastName.size() == 0) {
            return firstName + " with unknown last name";
        }
        return firstName + " " + lastName;
    }

private:
    int birthYear;
    std::map<int, std::string> firstNames;
    std::map<int, std::string> lastNames;

};


