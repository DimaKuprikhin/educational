#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person(const string& name) : Name(name) {}

    string GetName() const {
        return Name;
    }

    virtual string GetProfession() const = 0;

    virtual void Walk(const string& destination) const {
        cout << GetProfession() << ": " << Name << " walks to: " << destination << endl;
    }

protected:
    const string Name;
};

class Student : public Person {
public:
    Student(const string& name, const string& favouriteSong)
        : Person(name)
        , FavouriteSong(favouriteSong)
        {}

    string GetProfession() const override {
        return "Student";
    }

    void Learn() const {
        cout << GetProfession() << ": " << Name << " learns" << endl;
    }

    void Walk(const string& destination) const override {
        Person::Walk(destination);
        SingSong();
    }

    void SingSong() const {
        cout << GetProfession() << ": " << Name << " sings a song: " << FavouriteSong << endl;
    }

private:
    const string FavouriteSong;
};

class Teacher : public Person {
public:
    Teacher(string name, string subject)
        : Person(name)
        , Subject(subject)
        {}

    string GetProfession() const override {
        return "Teacher";
    }

    void Teach() const {
        cout << GetProfession() << ": " << Name << " teaches: " << Subject << endl;
    }

private:
    const string Subject;
};


class Policeman : public Person {
public:
    Policeman(const string& name) : Person(name) {}

    void Check(const Person& person) const {
        cout << GetProfession() << ": " << Name << " checks " << person.GetProfession() << ". "
             << person.GetProfession() << "'s name is: " << person.GetName() << endl;
    }

    string GetProfession() const override {
        return "Policeman";
    }
};

void VisitPlaces(const Person& person, const vector<string>& places) {
    for(const auto& place : places) {
        person.Walk(place);
    }
}

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}
