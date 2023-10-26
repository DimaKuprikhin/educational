#include <gtest/gtest.h>
#include <long_integer.h>

TEST(LongIntegerTest, ConstructFromStringTest) {
    EXPECT_EQ(LongInteger("455"), 455);
    EXPECT_EQ(LongInteger("+1"), 1);
    EXPECT_EQ(LongInteger("-1000000001"), -1000000001);
}

TEST(LongIntegerTest, OperatorPlusTest) {
    EXPECT_EQ(LongInteger("12") + 1000, 1012);
    EXPECT_EQ(LongInteger("-12") + 1000, 988);
    EXPECT_EQ(LongInteger("12") + -1000, -988);
    EXPECT_EQ(LongInteger("-12") + -1000, -1012);
    EXPECT_EQ(LongInteger(500000000) + 500000001, 1000000001);
    EXPECT_EQ(LongInteger(1) + -1000000002, -1000000001);
    EXPECT_EQ(LongInteger(INT64_MIN) + INT64_MAX + 2, 1);
}

TEST(LongIntegerTest, OperatorMinusTest) {
    EXPECT_EQ(LongInteger("12") - 1000, -988);
    EXPECT_EQ(LongInteger("-12") - 1000, -1012);
    EXPECT_EQ(LongInteger("12") - -1000, 1012);
    EXPECT_EQ(LongInteger("-12") - -1000, 988);
    EXPECT_EQ(500000000_li - -500000001, 1000000001);
}

TEST(LongIntegerTest, OperatorMultiplyTest) {
    EXPECT_EQ(LongInteger("12") * 32, 384);
    EXPECT_EQ(LongInteger("12") * -32, -384);
    EXPECT_EQ(LongInteger("+0") * 32, 0);
    EXPECT_EQ(LongInteger("12000000000000") * 32000000,
              "384000000000000000000"_li);
    EXPECT_EQ(LongInteger("12000000000000") * -32000000,
              LongInteger("-384000000000000000000"));
    EXPECT_EQ(LongInteger("-12345600000000") * LongInteger(-78912300000000),
              LongInteger("974219690880000000000000000"));
}

TEST(LongIntegerTest, OperatorDivideTest) {
    EXPECT_EQ(LongInteger("384") / 32, 12);
    EXPECT_EQ(LongInteger("-384") / 12, -32);
    EXPECT_EQ(LongInteger("+0") / 32, 0);
    EXPECT_EQ(LongInteger("975461057789971041000000000000000") / LongInteger(987654321000),
              LongInteger("987654321000000000000"));
}

TEST(LongIntegerTest, OperatorRemainderTest) {
    EXPECT_EQ(LongInteger(3000000005) % 1000000001, 2);
    EXPECT_EQ(LongInteger(-3000000005) % 1000000001, -2);
    EXPECT_EQ(LongInteger(3000000005) % -1000000001, 2);
    EXPECT_EQ(LongInteger(-3000000005) % -1000000001, -2);
    EXPECT_EQ(LongInteger(-0) % -1000000001, 0);
}

TEST(LongIntegerTest, OperatorLessTest) {
    EXPECT_EQ(LongInteger("12") < LongInteger(1000), true);
    EXPECT_EQ(LongInteger("-12") < LongInteger(1000), true);
    EXPECT_EQ(LongInteger("12") < LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("-12") < LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("12") < LongInteger(12), false);
    EXPECT_EQ(LongInteger("-12") < LongInteger(-12), false);
}

TEST(LongIntegerTest, OperatorGreaterTest) {
    EXPECT_EQ(LongInteger("12") > LongInteger(1000), false);
    EXPECT_EQ(LongInteger("-12") > LongInteger(1000), false);
    EXPECT_EQ(LongInteger("12") > LongInteger(-1000), true);
    EXPECT_EQ(LongInteger("-12") > LongInteger(-1000), true);
    EXPECT_EQ(LongInteger("12") > LongInteger(12), false);
    EXPECT_EQ(LongInteger("-12") > LongInteger(-12), false);
}

TEST(LongIntegerTest, OperatorLessOrEqualTest) {
    EXPECT_EQ(LongInteger("12") <= LongInteger(1000), true);
    EXPECT_EQ(LongInteger("-12") <= LongInteger(1000), true);
    EXPECT_EQ(LongInteger("12") <= LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("-12") <= LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("12") <= LongInteger(12), true);
    EXPECT_EQ(LongInteger("-12") <= LongInteger(-12), true);
}

TEST(LongIntegerTest, OperatorGreaterOrEqualTest) {
    EXPECT_EQ(LongInteger("12") >= LongInteger(1000), false);
    EXPECT_EQ(LongInteger("-12") >= LongInteger(1000), false);
    EXPECT_EQ(LongInteger("12") >= LongInteger(-1000), true);
    EXPECT_EQ(LongInteger("-12") >= LongInteger(-1000), true);
    EXPECT_EQ(LongInteger("12") >= LongInteger(12), true);
    EXPECT_EQ(LongInteger("-12") >= LongInteger(-12), true);
}

TEST(LongIntegerTest, OperatorEqualTest) {
    EXPECT_EQ(LongInteger("12") == LongInteger(1000), false);
    EXPECT_EQ(LongInteger("-12") == LongInteger(1000), false);
    EXPECT_EQ(LongInteger("12") == LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("-12") == LongInteger(-1000), false);
    EXPECT_EQ(LongInteger("12") == LongInteger(12), true);
    EXPECT_EQ(LongInteger("-12") == LongInteger(-12), true);
    EXPECT_EQ(LongInteger("0") == LongInteger(0), true);
    EXPECT_EQ(LongInteger("-0") == LongInteger(0), true);
    EXPECT_EQ(LongInteger("0") == LongInteger(-0), true);
    EXPECT_EQ(LongInteger("-0") == LongInteger(-0), true);
}

TEST(LongIntegerTest, AbsTest) {
    EXPECT_EQ(LongInteger("12").Abs() == LongInteger(12), true);
    EXPECT_EQ(LongInteger("-12").Abs() == LongInteger(12), true);
}
