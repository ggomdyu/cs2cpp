#include <gtest/gtest.h>
#include <System/TimeSpan.h>

using namespace CS2CPP_NAMESPACE_NAME;

static void Expect(TimeSpan timeSpan, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t milliseconds, int64_t ticks)
{
    EXPECT_EQ(timeSpan.Days(), day);
    EXPECT_EQ(timeSpan.Hours(), hour);
    EXPECT_EQ(timeSpan.Minutes(), minute);
    EXPECT_EQ(timeSpan.Seconds(), second);
    EXPECT_EQ(timeSpan.Milliseconds(), milliseconds);
    EXPECT_EQ(timeSpan.Ticks(), ticks);
}

TEST(TimeSpan, OperatorAdd)
{
    auto timeSpan = TimeSpan(2, 7, 23) + TimeSpan(1, 5, 5, 24) + TimeSpan(5, 9, 44, 8, 444) + TimeSpan(8883);
    Expect(timeSpan, 6, 16, 56, 55, 444, 5794154448883);
}

TEST(TimeSpan, OperatorSubtract)
{
    auto timeSpan = TimeSpan(2, 7, 23) - TimeSpan(1, 5, 5, 24) + TimeSpan(5, 9, 44, 8, 444) + TimeSpan(8883);
    Expect(timeSpan, 4, 6, 46, 7, 444, 3699674448883);
}

TEST(TimeSpan, OperatorMultiply)
{
    Expect(TimeSpan(2, 7, 23) * 2.0, 0, 4, 14, 46, 0, 152860000000);
    Expect(2.0 * TimeSpan(2, 7, 23), 0, 4, 14, 46, 0, 152860000000);
}

TEST(TimeSpan, OperatorPlus)
{
    Expect(+TimeSpan(2, 7, 23), 0, 2, 7, 23, 0, 76430000000);
}

TEST(TimeSpan, OperatorAddAssignment)
{
    auto timeSpan = TimeSpan(2, 7, 23) += TimeSpan(1, 5, 5, 24);
    Expect(timeSpan, 1, 7, 12, 47, 0, 1123670000000);
}

TEST(TimeSpan, OperatorSubtractAssignment)
{
    auto timeSpan = TimeSpan(2, 7, 23) -= TimeSpan(1, 5, 5, 24);
    Expect(timeSpan, -1, -2, -58, -1, 0, -970810000000);
}

TEST(TimeSpan, OperatorMultiplyAssignment)
{
    auto timeSpan = TimeSpan(2, 7, 23) *= 2.0;
    Expect(timeSpan, 0, 4, 14, 46, 0, 152860000000);
}

TEST(TimeSpan, OperatorDivideAssignment)
{
    auto timeSpan = TimeSpan(2, 7, 23) /= 2.0;
    Expect(timeSpan, 0, 1, 3, 41, 500, 38215000000);
}

TEST(TimeSpan, OperatorEqual)
{
    EXPECT_EQ(TimeSpan(2, 7, 23), TimeSpan(2, 7, 23));
}

TEST(TimeSpan, OperatorNotEqual)
{
    EXPECT_NE(TimeSpan(2, 7, 24), TimeSpan(2, 7, 23));
}

TEST(TimeSpan, OperatorGreaterThan)
{
    EXPECT_GT(TimeSpan(2, 7, 24), TimeSpan(2, 7, 23));
}

TEST(TimeSpan, OperatorGreaterThanOrEqual)
{
    EXPECT_GE(TimeSpan(2, 7, 23), TimeSpan(2, 7, 23));
    EXPECT_GE(TimeSpan(2, 7, 24), TimeSpan(2, 7, 23));
}

TEST(TimeSpan, OperatorLessThan)
{
    EXPECT_LT(TimeSpan(2, 7, 22), TimeSpan(2, 7, 23));
    EXPECT_LT(TimeSpan(2, 7, 23), TimeSpan(2, 7, 24));
}

TEST(TimeSpan, OperatorLessThanOrEqual)
{
    EXPECT_LE(TimeSpan(2, 7, 23), TimeSpan(2, 7, 23));
    EXPECT_LE(TimeSpan(2, 7, 23), TimeSpan(2, 7, 24));
}

TEST(TimeSpan, CompareTo)
{
    EXPECT_EQ(TimeSpan(2, 7, 23).CompareTo(TimeSpan(2, 7, 22)), 1);
    EXPECT_EQ(TimeSpan(2, 7, 22).CompareTo(TimeSpan(2, 7, 22)), 0);
    EXPECT_EQ(TimeSpan(2, 7, 22).CompareTo(TimeSpan(2, 7, 23)), -1);
}

TEST(TimeSpan, Compare)
{
    EXPECT_EQ(TimeSpan::Compare(TimeSpan(2, 7, 23), TimeSpan(2, 7, 22)), 1);
    EXPECT_EQ(TimeSpan::Compare(TimeSpan(2, 7, 22), TimeSpan(2, 7, 22)), 0);
    EXPECT_EQ(TimeSpan::Compare(TimeSpan(2, 7, 22), TimeSpan(2, 7, 23)), -1);
}

TEST(TimeSpan, Equals)
{
    EXPECT_TRUE(TimeSpan(2, 7, 23).Equals(TimeSpan(2, 7, 23)));
    EXPECT_FALSE(TimeSpan(2, 7, 24).Equals(TimeSpan(2, 7, 23)));
    EXPECT_TRUE(TimeSpan::Equals(TimeSpan(2, 7, 23), TimeSpan(2, 7, 23)));
    EXPECT_FALSE(TimeSpan::Equals(TimeSpan(2, 7, 24), TimeSpan(2, 7, 23)));
}

TEST(TimeSpan, TotalDays)
{
    EXPECT_FLOAT_EQ(TimeSpan(2, 7, 23).TotalDays(), 0.088460648148148149);
}

TEST(TimeSpan, TotalHours)
{
    EXPECT_FLOAT_EQ(TimeSpan(2, 7, 23).TotalHours(), 2.1230555555555557);
}

TEST(TimeSpan, TotalMinutes)
{
    EXPECT_FLOAT_EQ(TimeSpan(2, 7, 23).TotalMinutes(), 127.38333333333334);
}

TEST(TimeSpan, TotalSeconds)
{
    EXPECT_FLOAT_EQ(TimeSpan(2, 7, 23).TotalSeconds(), 7643);
}

TEST(TimeSpan, TotalMilliseconds)
{
    EXPECT_FLOAT_EQ(TimeSpan(2, 7, 23).TotalMilliseconds(), 7643000);
}

TEST(TimeSpan, Add)
{
    auto timeSpan = TimeSpan(2, 7, 23).Add(TimeSpan(1, 5, 5, 24));
    Expect(timeSpan, 1, 7, 12, 47, 0, 1123670000000);
}

TEST(TimeSpan, Divide)
{
    auto timeSpan = TimeSpan(2, 7, 23).Divide(2.0);
    Expect(timeSpan, 0, 1, 3, 41, 500, 38215000000);
}

TEST(TimeSpan, Multiply)
{
    auto timeSpan = TimeSpan(2, 7, 23).Multiply(2.0);
    Expect(timeSpan, 0, 4, 14, 46, 0, 152860000000);
}

TEST(TimeSpan, Negate)
{
    auto timeSpan = TimeSpan(2, 7, 23).Negate();
    Expect(timeSpan, 0, -2, -7, -23, 0, -76430000000);
}

TEST(TimeSpan, Subtract)
{
    auto timeSpan = TimeSpan(2, 7, 23).Subtract(TimeSpan(1, 5, 5, 24));
    Expect(timeSpan, -1, -2, -58, -1, 0, -970810000000);
}

TEST(TimeSpan, Duration)
{
    auto timeSpan = TimeSpan(2, 7, 23).Duration();
    Expect(timeSpan, 0, 2, 7, 23, 0, 76430000000);
}

TEST(TimeSpan, FromDays)
{
    auto timeSpan = TimeSpan::FromDays(333.574);
    Expect(timeSpan, 333, 13, 46, 33, 600, 288207936000000);
}

TEST(TimeSpan, FromHours)
{
    auto timeSpan = TimeSpan::FromHours(3333.574);
    Expect(timeSpan, 138, 21, 34, 26, 400, 120008664000000);
}

TEST(TimeSpan, FromMinutes)
{
    auto timeSpan = TimeSpan::FromMinutes(33333.574);
    Expect(timeSpan, 23, 3, 33, 34, 440, 20000144400000);
}

TEST(TimeSpan, FromSeconds)
{
    auto timeSpan = TimeSpan::FromSeconds(333333.574);
    Expect(timeSpan, 3, 20, 35, 33, 574, 3333335740000);
}

TEST(TimeSpan, FromMilliseconds)
{
    auto timeSpan = TimeSpan::FromMilliseconds(3333333.574);
    Expect(timeSpan, 0, 0, 55, 33, 333, 33333335740);
}

TEST(TimeSpan, FromTicks)
{
    auto timeSpan = TimeSpan::FromTicks(333333333574);
    Expect(timeSpan, 0, 9, 15, 33, 333, 333333333574);
}

TEST(TimeSpan, GetHashCode)
{
    EXPECT_EQ(TimeSpan(0, 0, 0).GetHashCode(), 0);
    EXPECT_EQ(TimeSpan(2, 7, 23).GetHashCode(), -879411311);
}

TEST(TimeSpan, MaxValue)
{
    auto timeSpan = TimeSpan::MaxValue();
    Expect(timeSpan, 10675199, 2, 48, 5, 477, std::numeric_limits<int64_t>::max());
}

TEST(TimeSpan, MinValue)
{
    auto timeSpan = TimeSpan::MinValue();
    Expect(timeSpan, -10675199, -2, -48, -5, -477, std::numeric_limits<int64_t>::min());
}

TEST(TimeSpan, Zero)
{
    auto timeSpan = TimeSpan::Zero();
    Expect(timeSpan, 0, 0, 0, 0, 0, 0);
}

TEST(TimeSpan, ToString)
{
    auto timeSpanStr = TimeSpan(2, 7, 23).ToString();
    EXPECT_TRUE(timeSpanStr.find(u"02") != std::string::npos);
    EXPECT_TRUE(timeSpanStr.find(u"07") != std::string::npos);
    EXPECT_TRUE(timeSpanStr.find(u"23") != std::string::npos);
    EXPECT_EQ(std::count(timeSpanStr.begin(), timeSpanStr.end(), u':'), 2);

    timeSpanStr = TimeSpan(2, 7, 23).ToString(u"hh:mm:ss");
    EXPECT_TRUE(timeSpanStr.find(u"02") != std::string::npos);
    EXPECT_TRUE(timeSpanStr.find(u"07") != std::string::npos);
    EXPECT_TRUE(timeSpanStr.find(u"23") != std::string::npos);
    EXPECT_EQ(std::count(timeSpanStr.begin(), timeSpanStr.end(), u':'), 2);
}