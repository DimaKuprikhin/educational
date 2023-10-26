#include <expression_evaluator.h>

LongInteger ExpressionEvaluator::Evaluate(const std::string_view& expression) {
    const auto preparedExpression = PrepareExpression(expression);
    return EvaluatePrepared(preparedExpression);
}

LongInteger ExpressionEvaluator::EvaluatePrepared(const std::string_view& expression) {
    if(IsExpressionInsideBraces(expression)) {
        return EvaluatePrepared(expression.substr(1, expression.size() - 2));
    }

    int leastPriorityOperatorPos = FindLeastPriorityOperatorPosition(expression);
    if(leastPriorityOperatorPos == -1) {
        LongInteger result(expression);
        return result;
    }
    const auto lhs = expression.substr(0, leastPriorityOperatorPos);
    const auto rhs = expression.substr(leastPriorityOperatorPos + 1);
    return operations.at(expression[leastPriorityOperatorPos])(EvaluatePrepared(lhs),
                                                               EvaluatePrepared(rhs));
}

int ExpressionEvaluator::FindLeastPriorityOperatorPosition(const std::string_view& expression) {
    int leastPriorityOperatorPos = -1;
    int leastPriorityOperatorPriority = -1;
    int bracesLevel = 0;
    for(int i = 0; i < static_cast<int>(expression.size()); ++i) {
        if(expression[i] == '(') {
            ++bracesLevel;
        }
        if(expression[i] == ')') {
            --bracesLevel;
        }
        if(bracesLevel > 0) {
            continue;
        }
        const auto& prioritiesPtr = operatorPriorities.find(expression[i]);
        if(prioritiesPtr != operatorPriorities.end()
            && (leastPriorityOperatorPos == -1
                || leastPriorityOperatorPriority >= operatorPriorities.at(expression[i])))
        {
            leastPriorityOperatorPos = i;
            leastPriorityOperatorPriority = operatorPriorities.at(expression[i]);
        }
    }
    return leastPriorityOperatorPos;
}

bool ExpressionEvaluator::IsExpressionInsideBraces(const std::string_view& expression) {
    if(expression[0] != '(' || expression.back() != ')') {
        return false;
    }
    int bracesLevel = 1;
    for(const char c : expression.substr(1, expression.size() - 2)) {
        if(c == '(') {
            ++bracesLevel;
        }
        if(c == ')') {
            --bracesLevel;
        }
        if(bracesLevel == 0) {
            return false;
        }
    }
    return true;
}

std::string ExpressionEvaluator::PrepareExpression(const std::string_view& expression) {
    std::string result;
    for(const auto& c : expression) {
        if(c > ' ') {
            result += c;
        }
    }
    return result;
}

const std::unordered_map<char, ExpressionEvaluator::BinaryOperation> 
ExpressionEvaluator::operations =
std::unordered_map<char, BinaryOperation>({
    { '*', [](const auto& lhs, const auto& rhs)->auto { return lhs * rhs; } },
    { '/', [](const auto& lhs, const auto& rhs)->auto { return lhs / rhs; } },
    { '%', [](const auto& lhs, const auto& rhs)->auto { return lhs % rhs; } },
    { '+', [](const auto& lhs, const auto& rhs)->auto { return lhs + rhs; } },
    { '-', [](const auto& lhs, const auto& rhs)->auto { return lhs - rhs; } }
});

const std::unordered_map<char, int> ExpressionEvaluator::operatorPriorities =
std::unordered_map<char, int>({
    { '*', 2 },
    { '/', 2 },
    { '%', 2 },
    { '+', 1 },
    { '-', 1 }
});
