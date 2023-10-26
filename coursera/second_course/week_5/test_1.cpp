#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <exception>

/*
Поскольку в задании явно не говорится, можно ли менять функцию main() - т.е. требуется ли сохранять интерфейсы классов для обратной совместимости, будем считать, что разрешены любые изменения в коде.
*/

/*
Вспомогательная функция для вывода ошибки
*/
void PrintNotImplementedError(const std::string& funcId, const std::string& classId)
{
    std::cout << "Function '" << funcId << "' not impleneted for class '" << classId << "'" << std::endl;
};


/*
Категории жителей
*/
enum class CitizenTypes : std::uint8_t
{
      STUDENT
    , TEACHER
    , POLICEMAN
};
const std::string CitizenTypeToString(const CitizenTypes type)
{
    switch (type) {
    case CitizenTypes::STUDENT:
        return "Student";
    case CitizenTypes::TEACHER:
        return "Teacher";
    case CitizenTypes::POLICEMAN:
        return "Policeman";
    default:
        return "Unknown citizen " + std::to_string(static_cast<std::uint8_t>(type));
    }
};
std::ostream& operator<<(std::ostream& os, const CitizenTypes type)
{
    os << CitizenTypeToString(type);
    return os;
};

/*
Базовая структура входных параметров для классов
*/
struct CitizenProps
{
    std::string name;
    CitizenTypes type;
    CitizenProps(const std::string &name_, const CitizenTypes type_):
          name(name_)
        , type(type_)
    {};
};

/*
Входные параметры для класса Student
*/
struct StudentProps : CitizenProps
{
    std::string song;
    StudentProps(const std::string& name_, const std::string& song_) :
          CitizenProps(name_, CitizenTypes::STUDENT)
        , song(song_)
    {};
};

/*
Входные параметры для класса Teacher
*/
struct TeacherProps : CitizenProps
{
    std::string subject;
    TeacherProps(const std::string& name_, const std::string& subject_) :
        CitizenProps(name_, CitizenTypes::TEACHER)
        , subject(subject_)
    {};
};

/*
Входные параметры для класса Policeman
*/
struct PolicemanProps : CitizenProps
{
    PolicemanProps(const std::string& name_) :
        CitizenProps(name_, CitizenTypes::POLICEMAN)
    {};
};

/*
Базовый класс для определения жителей (Student, Teacher, etc)
*/
class Citizen
{
public:
    Citizen() {};
    //Запрос информации об имени
    virtual const std::string Name() const = 0;
    //Запрос информации о категории жителя
    virtual const CitizenTypes Type() const = 0;
    //Запрос информации о прогулке до заданного места
    virtual void Walk(const std::string &) const = 0;

    /*
    NOTE нижеследующие методы необходимы для унификации интерфейса класса, поскольку нельзя объявить виртуальным шаблонный метод
    */
    virtual void Learn() const = 0;
    virtual void SingSong() const = 0;
    virtual void Teach() const = 0;
    virtual void Check(const Citizen &) const = 0;
};

/*
Класс студента: доступны методы Learn, Walk, Sing
*/
class Student : public Citizen {
public:

    Student(const StudentProps &props_):
          Citizen()
        , props(props_)
    {
    }
    void Learn() const override {
        std::cout << "Student: " << props.name << " learns" << std::endl;
    }
    void Walk(const std::string &destination) const override {
        std::cout << "Student: " << props.name << " walks to: " << destination << std::endl;
    }
    void SingSong() const override {
        std::cout << "Student: " << props.name << " sings a song: " << props.song << std::endl;
    }
    const std::string Name() const override {
        return props.name;
    }
    const CitizenTypes Type() const override {
        return props.type;
    }
    void Teach() const override {
        PrintNotImplementedError(__func__, typeid(this).name());
    };
    void Check(const Citizen&) const override {
        PrintNotImplementedError(__func__, typeid(this).name());
    };

public:
    StudentProps props;
};

/*
Класс учителя - доступны методы Teach, Walk
*/
class Teacher : public Citizen {
public:

    Teacher(const TeacherProps& props_) :
        Citizen()
        , props(props_)
    {
    }
    void Teach() const override {
        std::cout << "Teacher: " << props.name << " teaches: " << props.subject << std::endl;
    }
    void Walk(const std::string &destination) const override {
        std::cout << "Teacher: " << props.name << " walks to: " << destination << std::endl;
    }
    const std::string Name() const override {
        return props.name;
    }
    const CitizenTypes Type() const override {
        return props.type;
    }
    void Learn() const override {
        PrintNotImplementedError(__func__, typeid(this).name());
    };
    void SingSong() const override {
        PrintNotImplementedError(__func__, typeid(this).name());
    };
    void Check(const Citizen&) const override {
        PrintNotImplementedError(__func__, typeid(this).name());
    };

public:
    TeacherProps props;
};

/*
Класс полицейского - доступны методы Walk, Check
*/
class Policeman : public Citizen {
public:
    Policeman(const PolicemanProps& props_) :
        Citizen()
        , props(props_)
    {
    }
    void Check(const Citizen& c) const override {
        std::cout << "Policeman: " << props.name << " checks " << c.Type() << ". " << c.Type() << "'s name is: " << c.Name() << std::endl;
    }
    void Walk(const std::string &destination) const override {
        std::cout << "Policeman: " << props.name << " walks to: " << destination << std::endl;
    }
    const std::string Name() const override {
        return props.name;
    }
    const CitizenTypes Type() const override {
        return props.type;
    }
    void Learn() const override {
        PrintNotImplementedError(__func__, typeid(this).name());
    };
    void SingSong() const override {
        PrintNotImplementedError(__func__, typeid(this).name());
    };
    void Teach() const override {
        PrintNotImplementedError(__func__, typeid(this).name());
    };

public:
    PolicemanProps props;
};

/*
Абстрактная фабрика для создания жителя
*/
class CitizenFactory
{
public:
    CitizenFactory() {};
    virtual std::shared_ptr<Citizen> CreateCitizen() = 0;
};

/*
Фабрика для создания класса Student
*/
class StudentFactory : public CitizenFactory
{
public:
    StudentFactory(const StudentProps& props_) :
        CitizenFactory()
        , props(props_)
    {
    };
    std::shared_ptr<Citizen> CreateCitizen()
    {
        return std::make_shared<Student>(props);
    }

private:
    StudentProps props;
};

/*
Фабрика для создания класса Teacher
*/
class TeacherFactory : public CitizenFactory
{
public:
    TeacherFactory(const TeacherProps& props_) :
        CitizenFactory()
        , props(props_)
    {
    };
    std::shared_ptr<Citizen> CreateCitizen()
    {
        return std::make_shared<Teacher>(props);
    }

private:
    TeacherProps props;
};

/*
Фабрика для создания класса Policeman
*/
class PolicemanFactory : public CitizenFactory
{
public:
    PolicemanFactory(const PolicemanProps& props_) :
        CitizenFactory()
        , props(props_)
    {
    };
    std::shared_ptr<Citizen> CreateCitizen()
    {
        return std::make_shared<Policeman>(props);
    }

private:
    PolicemanProps props;
};

/*
Выбор необходимой фабрики
*/
std::shared_ptr<CitizenFactory> CreateFactory(const CitizenTypes type, const std::string& name, const std::string& info)
{
    switch (type) {
    case CitizenTypes::STUDENT:
        return std::make_shared<StudentFactory>(StudentProps(name, info));
    case CitizenTypes::TEACHER:
        return std::make_shared<TeacherFactory>(TeacherProps(name, info));
    case CitizenTypes::POLICEMAN:
        return std::make_shared<PolicemanFactory>(PolicemanProps(name));
    default:
        throw std::invalid_argument(CitizenTypeToString(type));
    }
}

/*
Метод имитации прогулки для жителей
*/
void VisitPlaces(const Citizen &c, const std::vector<std::string> places) {
    for (auto p : places) {
        c.Walk(p);
    }
}


int main() {
    //Старая инициализация - совместимость сохранена
    Teacher t({ "Jim", "Math" });
    Student s({ "Ann", "We will rock you" });
    Policeman p({ "Bob" });
    VisitPlaces(t, { "Moscow", "London" });
    p.Check(s);
    VisitPlaces(s, { "Moscow", "London" });

    //Новая инициализация
    std::cout << std::endl << "<===== New Approach =====>" << std::endl;
    //Создаём вектор начальных данных для тестов
    std::vector<std::pair<CitizenTypes, std::pair<std::string, std::string>>> citizens;
    citizens.push_back({ CitizenTypes::TEACHER, { "Jim", "Math" } });
    citizens.push_back({ CitizenTypes::STUDENT, { "Ann", "We will rock you" } });
    citizens.push_back({ CitizenTypes::POLICEMAN, { "Bob", "" } });
    std::vector<std::string> destinations;
    destinations.push_back("Moscow");
    destinations.push_back("London");
    //Далее выполняем операции по каждому объекту
    for (std::size_t i = 0; i < citizens.size(); i++) {
        //Создаём жителя с помощью абстрактной фабрики
        const auto citizen = CreateFactory(citizens[i].first, citizens[i].second.first, citizens[i].second.second)->CreateCitizen();
        //Выводим общую информацию
        std::cout << std::endl << "Welcome new citizen: " << citizen->Name() << " is a " << citizen->Type() << std::endl;
        //Проверяем работу методов созданного класса
        VisitPlaces(*citizen, destinations);
        citizen->Learn();
        citizen->SingSong();
        citizen->Teach();
        for (std::size_t j = 0; j <= i; j++) {
            const auto citizenToCheck = CreateFactory(citizens[j].first, citizens[j].second.first, citizens[j].second.second)->CreateCitizen();
            citizen->Check(*citizenToCheck);
        }
    }
    
    return 0;
}