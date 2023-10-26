#pragma once
#include <functional>
#include <long_integer.h>

/* 
Содержит метод, рекурсивно вычисляющий значение выражения.
На каждом шаге алгоритма, выражение делится относительно самой правой
наименее приоритетной операции.
Дерево вызовов для выражения "2 + (3 / 5 * 4)": 
        (+)
        / \
      (2) (*) 
          / \
        (/) (4)
        / \
      (3) (5)
*/
class ExpressionEvaluator {
public:

    // Вычисляет значение выражение, представленного строкой.
    static LongInteger Evaluate(const std::string_view& expression);

private:

    // Вычисляет значение выражения после применения метода PrepareExpression.
    static LongInteger EvaluatePrepared(const std::string_view& expression);

    // Находит в выражении позицию оператора с самым низким приоритетом.
    // Если таких несколько, возвращается позиция самого последнего из таких.
    // При этом учитываются скобки, то есть для выражения "2 * (3 + 5)"
    // метод вернет позицию "*", хотя приоритет "+" ниже.
    static int FindLeastPriorityOperatorPosition(const std::string_view& expression);

    // Проверяет, имеет ли expression вид (x), где x - другое выражение, при
    // этом учитываются скобки, то есть для expression = "(x)(y)" метод вернет false.
    static bool IsExpressionInsideBraces(const std::string_view& expression);

    // Удаляет все пробельные символы из выражения.
    static std::string PrepareExpression(const std::string_view& expression);

private:
    using BinaryOperation = std::function<LongInteger(const LongInteger& lhs, const LongInteger& rhs)>;

    static const std::unordered_map<char, BinaryOperation> operations;
    static const std::unordered_map<char, int> operatorPriorities;

};
