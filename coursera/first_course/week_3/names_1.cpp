#include <iostream>
#include <map>
#include <string>
#include <vector>

class Person {
public:
    void ChangeFirstName(int year, const std::string& first_name) {
        // добавить факт изменения имени на first_name в год year
        firstNames.insert({ -year, first_name });
    }

    void ChangeLastName(int year, const std::string& last_name) {
        // добавить факт изменения фамилии на last_name в год year
        lastNames.insert({ -year, last_name });
    }

    std::string GetFullName(int year) {
        // получить имя и фамилию по состоянию на конец года year
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

private:
    std::map<int, std::string> firstNames;
    std::map<int, std::string> lastNames;

};

int main() {
  Person person;
  
  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    std::cout << person.GetFullName(year) << std::endl;
  }
  
  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    std::cout << person.GetFullName(year) << std::endl;
  }
  
  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    std::cout << person.GetFullName(year) << std::endl;
  }
  
  return 0;
}

