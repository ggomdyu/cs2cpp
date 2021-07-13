#include <fmt/format.h>
#include <gtest/gtest.h>
#include <System/DateTime.h>
#include <System/TimeZoneInfo.h>
#include <System/Environment.h>
#include <System/Diagnostics/Debug.h>

#include <unicode/udat.h>
#include <unicode/datefmt.h>
#include <unicode/unistr.h>
#include <unicode/dtptngen.h>
#include <unicode/dtfmtsym.h>

using namespace tg;

TEST(DateTime, OperatorAdd)
{
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).Year(), 2023);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).Month(), 6);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).Day(), 21);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).Hour(), 19);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).Minute(), 12);
    EXPECT_EQ((DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24)).Second(), 11);
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

TEST(DateTime, UtcNow)
{
    auto now = DateTime::Now();
    auto utcNow = DateTime::UtcNow();
    auto timeDiff = now - utcNow;
    EXPECT_FLOAT_EQ(static_cast<double>(TimeZoneInfo::Local().GetBaseUtcOffset().Hours()), timeDiff.TotalHours());
}

TEST(DateTime, Today)
{
    auto now = DateTime::Now();
    auto today = DateTime::Today();
    EXPECT_EQ(now.Year(), today.Year());
    EXPECT_EQ(now.Month(), today.Month());
    EXPECT_EQ(now.Day(), today.Day());
    EXPECT_EQ(today.Hour(), 0);
    EXPECT_EQ(today.Minute(), 0);
    EXPECT_EQ(today.Second(), 0);
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
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).Year(), 2052);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).Month(), 2);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).Day(), 7);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).Hour(), 23);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).Minute(), 6);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).Second(), 47);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddYears(30).Ticks(), 647265568070000000L);
}
TEST(DateTime, AddMonths)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).Year(), 2024);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).Month(), 8);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).Day(), 7);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).Hour(), 23);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).Minute(), 6);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).Second(), 47);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30).Ticks(), 638586688070000000L);
}

TEST(DateTime, AddDays)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).Year(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).Month(), 3);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).Day(), 9);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).Hour(), 23);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).Minute(), 6);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).Second(), 47);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(30).Ticks(), 637824640070000000L);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).Year(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).Month(), 3);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).Day(), 17);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).Hour(), 10);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).Minute(), 6);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).Second(), 18);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458).Ticks(), 637831083782000000L);
}

TEST(DateTime, AddHours)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(30).Year(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(30).Month(), 2);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(30).Day(), 9);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(30).Hour(), 5);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(30).Minute(), 6);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(30).Second(), 47);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(30).Ticks(), 637799800070000000L);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(37.458).Year(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(37.458).Month(), 2);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(37.458).Day(), 9);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(37.458).Hour(), 12);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(37.458).Minute(), 34);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(37.458).Second(), 15);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddHours(37.458).Ticks(), 637800068558000000L);
}

TEST(DateTime, AddMinutes)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(9743).Year(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(9743).Month(), 2);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(9743).Day(), 14);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(9743).Hour(), 17);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(9743).Minute(), 29);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(9743).Second(), 47);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(9743).Ticks(), 637804565870000000L);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(34471).Year(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(34471).Month(), 3);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(34471).Day(), 3);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(34471).Hour(), 21);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(34471).Minute(), 37);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(34471).Second(), 47);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(34471).Ticks(), 637819402670000000L);
}

TEST(DateTime, AddSeconds)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).Year(), 2022);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).Month(), 2);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).Day(), 8);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).Hour(), 20);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).Minute(), 7);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).Second(), 32);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645).Ticks(), 637799476520000000L);
}

TEST(DateTime, TimeOfDay)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).TimeOfDay(), TimeSpan(23, 6, 47));
    EXPECT_EQ(DateTime(2024, 3, 7, 23, 6, 47).TimeOfDay(), TimeSpan(23, 6, 47));
}

TEST(DateTime, Date)
{
    auto now = DateTime::Now();
    auto today = now.Date();
    EXPECT_EQ(today.Year(), now.Year());
    EXPECT_EQ(today.Month(), now.Month());
    EXPECT_EQ(today.Day(), now.Day());
    EXPECT_EQ(today.Hour(), 0);
    EXPECT_EQ(today.Minute(), 0);
    EXPECT_EQ(today.Second(), 0);
}

TEST(DateTime, GetDayOfWeek)
{
    EXPECT_EQ(DateTime::DayOfWeek(2020, 2, 7), DayOfWeek::Friday);
    EXPECT_EQ(DateTime::DayOfWeek(2020, 2, 29), DayOfWeek::Saturday);
    EXPECT_EQ(DateTime::DayOfWeek(2021, 4, 1), DayOfWeek::Thursday);
    EXPECT_EQ(DateTime::DayOfWeek(2021, 2, 28), DayOfWeek::Sunday);
    EXPECT_EQ(DateTime(2020, 2, 7, 23, 6, 47).DayOfWeek(), DayOfWeek::Friday);
    EXPECT_EQ(DateTime(2020, 2, 29, 23, 6, 47).DayOfWeek(), DayOfWeek::Saturday);
    EXPECT_EQ(DateTime(2021, 4, 1, 4, 34, 11).DayOfWeek(), DayOfWeek::Thursday);
    EXPECT_EQ(DateTime(2021, 2, 28, 4, 34, 11).DayOfWeek(), DayOfWeek::Sunday);
}

TEST(DateTime, DayOfYear)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).DayOfYear(), 38);
    EXPECT_EQ(DateTime(2024, 3, 7, 23, 6, 47).DayOfYear(), 67);
    EXPECT_EQ(DateTime(1000, 12, 7, 23, 6, 47).DayOfYear(), 341);
    EXPECT_EQ(DateTime(2011, 12, 7, 23, 6, 47).DayOfYear(), 341);
}

TEST(DateTime, MaxValue)
{
    EXPECT_EQ(DateTime::MaxValue().Year(), 9999);
    EXPECT_EQ(DateTime::MaxValue().Month(), 12);
    EXPECT_EQ(DateTime::MaxValue().Day(), 31);
    EXPECT_EQ(DateTime::MaxValue().Hour(), 23);
    EXPECT_EQ(DateTime::MaxValue().Minute(), 59);
    EXPECT_EQ(DateTime::MaxValue().Second(), 59);
}

TEST(DateTime, MinValue)
{
    EXPECT_EQ(DateTime::MinValue().Year(), 1);
    EXPECT_EQ(DateTime::MinValue().Month(), 1);
    EXPECT_EQ(DateTime::MinValue().Day(), 1);
    EXPECT_EQ(DateTime::MinValue().Hour(), 0);
    EXPECT_EQ(DateTime::MinValue().Minute(), 0);
    EXPECT_EQ(DateTime::MinValue().Second(), 0);
}

TEST(DateTime, UnixEpoch)
{
    EXPECT_EQ(DateTime::UnixEpoch().Year(), 1970);
    EXPECT_EQ(DateTime::UnixEpoch().Month(), 1);
    EXPECT_EQ(DateTime::UnixEpoch().Day(), 1);
    EXPECT_EQ(DateTime::UnixEpoch().Hour(), 0);
    EXPECT_EQ(DateTime::UnixEpoch().Minute(), 0);
    EXPECT_EQ(DateTime::UnixEpoch().Second(), 0);
}