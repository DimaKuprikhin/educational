#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include <sstream>
#include <iomanip>
using namespace std;

class Figure {
public:
    virtual std::string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Triangle : public Figure {
public:
    Triangle(const double a, const double b, const double c)
        : a(a), b(b), c(c) {}
    std::string Name() const override {
        return name;
    }
    double Perimeter() const override {
        return a + b + c;
    }
    double Area() const override {
        const double sp = (a + b + c) / 2;
        return std::sqrt(sp * (sp - a) * (sp - b) * (sp - c));
    }
private:
    const double a;
    const double b;
    const double c;
    static const std::string name;
};

const std::string Triangle::name = "TRIANGLE"; 

class Circle : public Figure {
public:
    Circle(const double r) : r(r) {}
    std::string Name() const override {
        return name;
    }
    double Perimeter() const override {
        return 6.28 * r;
    }
    double Area() const override {
        return 3.14 * r * r;
    }
private:
    const double r;
    static const std::string name;
};

const std::string Circle::name = "CIRCLE";

class Rect : public Figure {
public:
    Rect(const double width, const double height)
        : width(width), height(height) {}
    std::string Name() const override {
        return name;
    }
    double Perimeter() const override {
        return 2 * (width + height);
    }
    double Area() const override {
        return width * height;
    }
private:
    const double width;
    const double height;
    static const std::string name;
};

const std::string Rect::name = "RECT";

std::shared_ptr<Figure> CreateFigure(std::istream& stream) {
    std::string figureType;
    stream >> figureType;
    if(figureType == "TRIANGLE") {
        int a, b, c;
        stream >> a >> b >> c;
        return std::make_shared<Triangle>(a, b, c);
    }
    else if(figureType == "CIRCLE") {
        int r;
        stream >> r;
        return std::make_shared<Circle>(r);
    }
    else // if(figureType == "RECT")
    {
        int w, h;
        stream >> w >> h;
        return std::make_shared<Rect>(w, h);
    }
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            // Пропускаем "лишние" ведущие пробелы.
            // Подробнее об std::ws можно узнать здесь:
            // https://en.cppreference.com/w/cpp/io/manip/ws
            is >> ws;
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}   