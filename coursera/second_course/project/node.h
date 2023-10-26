#pragma once
#include "date.h"
#include <functional>
#include <memory>

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation {
    Or,
    And
};

template<typename T>
int EvaluateComparison(Comparison cmp, const T& lhs, const T& rhs) {
    if(cmp == Comparison::Less) {
        return lhs < rhs ? 1 : 0;
    }
    if(cmp == Comparison::LessOrEqual) {
        return lhs <= rhs ? 1 : 0;
    }
    if(cmp == Comparison::Greater) {
        return lhs > rhs ? 1 : 0;
    }
    if(cmp == Comparison::GreaterOrEqual) {
        return lhs >= rhs ? 1 : 0;
    }
    if(cmp == Comparison::Equal) {
        return lhs == rhs ? 1 : 0;
    }
    if(cmp == Comparison::NotEqual) {
        return lhs != rhs ? 1 : 0;
    }
    return 0;
}

class Node {
public:
    Node() {}
    virtual int Evaluate(const Date& date, const std::string& event) const = 0;
};

class EmptyNode : public Node {
public:
    int Evaluate(const Date& date, const std::string& event) const override;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison cmp, const Date& date);
    int Evaluate(const Date& date, const std::string& event) const override;
    void Print(std::ostream& stream) const;

private:
    const Comparison cmp;
    const Date date;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison cmp, const std::string& value);
    int Evaluate(const Date& date, const std::string& event) const override;

private:
    const Comparison cmp;
    const std::string value;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation op, std::shared_ptr<Node> left,
                                                std::shared_ptr<Node> right);
    int Evaluate(const Date& date, const std::string& event) const override;

private:
public:
    const LogicalOperation op;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};
