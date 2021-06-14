#include <gtest/gtest.h>
#include <System/DateTime.h>

using tg::DateTime;
using tg::DateTimeKind;
using tg::DayOfWeek;
using tg::TimeSpan;

TEST(DateTime, OperatorAdd)
{
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).GetYear(), 2023);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).GetMonth(), 6);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).GetDay(), 21);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).GetHour(), 19);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).GetMinute(), 12);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).GetSecond(), 11);
}

TEST(DateTime, OperatorEquals)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47), DateTime(2022, 2, 7, 23, 6, 47));
    EXPECT_NE(DateTime(2022, 2, 7, 23, 6, 47), DateTime(2022, 2, 7, 23, 6, 48));
}

TEST(DateTime, Compare)
{
    EXPECT_EQ(DateTime::Compare(DateTime(2022, 2, 7, 23, 6, 47), DateTime(2022, 2, 7, 23, 6, 47)), 0);
    EXPECT_EQ(DateTime::Compare(DateTime(2022, 2, 7, 23, 6, 47), DateTime(2022, 2, 8, 23, 6, 47)), -1);
    EXPECT_EQ(DateTime::Compare(DateTime(2022, 2, 8, 23, 6, 47), DateTime(2022, 2, 7, 23, 6, 47)), 1);
}

TEST(DateTime, IsLeapYear)
{
    EXPECT_FALSE(DateTime::IsLeapYear(1));
    EXPECT_FALSE(DateTime::IsLeapYear(2));
    EXPECT_FALSE(DateTime::IsLeapYear(3));
    EXPECT_TRUE(DateTime::IsLeapYear(4));
    EXPECT_FALSE(DateTime::IsLeapYear(5));
    EXPECT_FALSE(DateTime::IsLeapYear(6));
    EXPECT_FALSE(DateTime::IsLeapYear(7));
    EXPECT_TRUE(DateTime::IsLeapYear(8));
    EXPECT_FALSE(DateTime::IsLeapYear(10));
    EXPECT_TRUE(DateTime::IsLeapYear(40));
    EXPECT_FALSE(DateTime::IsLeapYear(100));
    EXPECT_TRUE(DateTime::IsLeapYear(400));
    EXPECT_FALSE(DateTime::IsLeapYear(1000));
}

TEST(DateTime, DaysInMonth)
{
    EXPECT_EQ(DateTime::DaysInMonth(2020, 1), 31);
    EXPECT_EQ(DateTime::DaysInMonth(8, 2), 29);
    EXPECT_EQ(DateTime::DaysInMonth(2400, 2), 29);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 2), 29);
    EXPECT_EQ(DateTime::DaysInMonth(2021, 2), 28);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 3), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 4), 30);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 5), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 6), 30);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 7), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 8), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 9), 30);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 10), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 11), 30);
    EXPECT_EQ(DateTime::DaysInMonth(2020, 12), 31);
}

TEST(DateTime, AddYears)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).GetYear(), 2052);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).GetMonth(), 2);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).GetDay(), 7);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).GetHour(), 23);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).GetMinute(), 6);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).GetSecond(), 47);
}
TEST(DateTime, AddMonths)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).GetYear(), 2024);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).GetMonth(), 8);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).GetDay(), 7);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).GetHour(), 23);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).GetMinute(), 6);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).GetSecond(), 47);
}

TEST(DateTime, AddDays)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).GetYear(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).GetMonth(), 3);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).GetDay(), 9);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).GetHour(), 23);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).GetMinute(), 6);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).GetSecond(), 47);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).GetYear(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).GetMonth(), 3);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).GetDay(), 17);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).GetHour(), 10);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).GetMinute(), 6);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).GetSecond(), 18);
}

TEST(DateTime, AddSeconds)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).GetYear(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).GetMonth(), 2);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).GetDay(), 8);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).GetHour(), 20);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).GetMinute(), 7);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).GetSecond(), 32);
}

TEST(DateTime, GetDate)
{
    auto now = DateTime::Now();
    auto today = now.GetDate();
    EXPECT_EQ(today.GetYear(), now.GetYear());
    EXPECT_EQ(today.GetMonth(), now.GetMonth());
    EXPECT_EQ(today.GetDay(), now.GetDay());
    EXPECT_EQ(today.GetHour(), 0);
    EXPECT_EQ(today.GetMinute(), 0);
    EXPECT_EQ(today.GetSecond(), 0);
}

TEST(DateTime, GetDayOfWeek)
{
    EXPECT_EQ(DateTime::GetDayOfWeek(2020, 2, 7), DayOfWeek::Friday);
    EXPECT_EQ(DateTime::GetDayOfWeek(2020, 2, 29), DayOfWeek::Saturday);
    EXPECT_EQ(DateTime::GetDayOfWeek(2021, 4, 1), DayOfWeek::Thursday);
    EXPECT_EQ(DateTime::GetDayOfWeek(2021, 2, 28), DayOfWeek::Sunday);
    EXPECT_EQ(DateTime(2020, 2, 7, 23, 6, 47).GetDayOfWeek(), DayOfWeek::Friday);
    EXPECT_EQ(DateTime(2020, 2, 29, 23, 6, 47).GetDayOfWeek(), DayOfWeek::Saturday);
    EXPECT_EQ(DateTime(2021, 4, 1, 4, 34, 11).GetDayOfWeek(), DayOfWeek::Thursday);
    EXPECT_EQ(DateTime(2021, 2, 28, 4, 34, 11).GetDayOfWeek(), DayOfWeek::Sunday);
}