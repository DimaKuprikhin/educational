#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Man
{
public:
	Man(const string &name, const string &profession) : Name(name), Profession(profession)
	{
	}

	const string Name;
	const string Profession;

	virtual void Walk(const string &destination) const = 0;
};

class Student : public Man
{
public:
	Student(const string &name, const string &favouriteSong)
		: Man(name, "Student"), favouriteSong_(favouriteSong)
	{
	}

	void Learn() const
	{
		cout << "Student: " << Name << " learns" << endl;
	}

	void Walk(const string &destination) const override
	{
		cout << "Student: " << Name << " walks to: " << destination << endl;
		cout << "Student: " << Name << " sings a song: " << favouriteSong_ << endl;
	}

	void SingSong() const
	{
		cout << "Student: " << Name << " sings a song: " << favouriteSong_ << endl;
	}

private:
	string favouriteSong_;
};

class Teacher : public Man
{
public:
	Teacher(const string &name, const string &subject)
		: Man(name, "Teacher"), subject_(subject)
	{
	}

	void Teach() const
	{
		cout << "Teacher: " << Name << " teaches: " << subject_ << endl;
	}

	void Walk(const string &destination) const override
	{
		cout << "Teacher: " << Name << " walks to: " << destination << endl;
	}

private:
	const string subject_;
};

class Policeman : public Man
{
public:
	Policeman(const string &name) : Man(name, "Policeman")
	{
	}

	void Check(const Man &m) const
	{
		cout << "Policeman: " << Name << " checks " << m.Profession << ". "
			 << m.Profession << "'s name is: " << m.Name << endl;
	}

	void Walk(const string &destination) const override
	{
		cout << "Policeman: " << Name << " walks to: " << destination << endl;
	}
};

void VisitPlaces(const Man &m, const vector<string> &places)
{
	for (auto p : places)
	{
		m.Walk(p);
	}
}

int main()
{
	Teacher t("Jim", "Math");
	Student s("Ann", "We will rock you");
	Policeman p("Bob");

	VisitPlaces(t, {"Moscow", "London"});
	p.Check(s);
	VisitPlaces(s, {"Moscow", "London"});
	return 0;
}