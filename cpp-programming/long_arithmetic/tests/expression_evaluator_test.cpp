#include <expression_evaluator.h>
#include <gtest/gtest.h>

TEST(ExpressionParserTestSuite, Test) {
    EXPECT_EQ(ExpressionEvaluator::Evaluate("1"), 1);
    EXPECT_EQ(ExpressionEvaluator::Evaluate("-1"), -1);
    EXPECT_EQ(ExpressionEvaluator::Evaluate("(((1)))"), 1);
    EXPECT_EQ(ExpressionEvaluator::Evaluate("1 + 2"), 3);
    EXPECT_EQ(ExpressionEvaluator::Evaluate("2 * 3 / 2"), 3);
    EXPECT_EQ(ExpressionEvaluator::Evaluate("2 + ((-2 - 2 * 12 % 5) + 50) / 9"), 6);
    EXPECT_EQ(ExpressionEvaluator::Evaluate("1000000000 * 9000000000"), "9000000000000000000"_li);
    EXPECT_EQ(ExpressionEvaluator::Evaluate("(3) * (2)"), 6);
    EXPECT_EQ(ExpressionEvaluator::Evaluate("((12 + 3) % 4 * 7) * (50 / (3 * (1 + 2))) + 3 - 4 / 2"), 106);
}
