#include <string>

struct Specialization {
    explicit Specialization(const std::string& value)
        : value(value)
        {}
    const std::string value;
};

struct Course {
    explicit Course(const std::string& value)
        : value(value)
        {}
    const std::string value;
};

struct Week {
    explicit Week(const std::string& value)
        : value(value)
        {}
    const std::string value;
};

struct LectureTitle {
    LectureTitle(const Specialization& spec, const Course& course, const Week& week)
        : specialization(spec.value)
        , course(course.value)
        , week(week.value)
        {}

    std::string specialization;
    std::string course;
    std::string week;
};

