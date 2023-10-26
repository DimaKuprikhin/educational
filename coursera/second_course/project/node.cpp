#include "node.h"

int EmptyNode::Evaluate(const Date& date, const std::string& event) const {
    return 1;
}

DateComparisonNode::DateComparisonNode(const Comparison cmp, const Date& date)
    : cmp(cmp)
    , date(date)
    {}

int DateComparisonNode::Evaluate(const Date& date, const std::string& event) const {
    return EvaluateComparison(cmp, date, this->date);
}

void DateComparisonNode::Print(std::ostream& stream) const {
    stream << (cmp == Comparison::NotEqual ? "!=" : "?") << date;
}

EventComparisonNode::EventComparisonNode(const Comparison cmp, const std::string& value)
    : cmp(cmp)
    , value(value)
    {}

int EventComparisonNode::Evaluate(const Date& date, const std::string& event) const {
    return EvaluateComparison(cmp, event, value);
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation op,
    std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : op(op)
        , left(left)
        , right(right)
        {}

int LogicalOperationNode::Evaluate(const Date& date, const std::string& event) const {
    if(op == LogicalOperation::Or) {
        return left->Evaluate(date, event) | right->Evaluate(date, event);
    }
    if(op == LogicalOperation::And) {
        return left->Evaluate(date, event) & right->Evaluate(date, event);
    }
    return 0;
}
