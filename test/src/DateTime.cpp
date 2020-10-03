#include <System/DateTime.h>
#include <System/TimeZoneInfo.h>
#include <fmt/xchar.h>
#include <gtest/gtest.h>

using namespace CS2CPP_NAMESPACE_NAME;

TEST(DateTime, OperatorAdd)
{
    auto dateTime = DateTime(2022, 2, 7, 23, 6, 47) + TimeSpan(488, 260, 5, 24);
    EXPECT_EQ(dateTime.Year(), 2023);
    EXPECT_EQ(dateTime.Month(), 6);
    EXPECT_EQ(dateTime.Day(), 21);
    EXPECT_EQ(dateTime.Hour(), 19);
    EXPECT_EQ(dateTime.Minute(), 12);
    EXPECT_EQ(dateTime.Second(), 11);
}

TEST(DateTime, OperatorEqual)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47), DateTime(2022, 2, 7, 23, 6, 47));
    EXPECT_NE(DateTime(2022, 2, 7, 23, 6, 47), DateTime(2022, 2, 7, 23, 6, 48));
}

TEST(DateTime, Add)
{
    auto dateTime = DateTime(2022, 11, 6, 22, 59, 2, 456).Add(TimeSpan(56, 11, 11, 33, 993));
    EXPECT_EQ(dateTime.Year(), 2023);
    EXPECT_EQ(dateTime.Month(), 1);
    EXPECT_EQ(dateTime.Day(), 2);
    EXPECT_EQ(dateTime.Hour(), 10);
    EXPECT_EQ(dateTime.Minute(), 10);
    EXPECT_EQ(dateTime.Second(), 36);
    EXPECT_EQ(dateTime.Millisecond(), 449);
    EXPECT_EQ(dateTime.Ticks(), 638082510364490000);
    EXPECT_EQ(dateTime.TimeOfDay().Ticks(), 366364490000);
    EXPECT_EQ(dateTime.Kind(), DateTimeKind::Unspecified);
}

TEST(DateTime, AddYears)
{
    auto dateTIme = DateTime(2022, 2, 7, 23, 6, 47).AddYears(30);
    EXPECT_EQ(dateTIme.Year(), 2052);
    EXPECT_EQ(dateTIme.Month(), 2);
    EXPECT_EQ(dateTIme.Day(), 7);
    EXPECT_EQ(dateTIme.Hour(), 23);
    EXPECT_EQ(dateTIme.Minute(), 6);
    EXPECT_EQ(dateTIme.Second(), 47);
    EXPECT_EQ(dateTIme.Ticks(), 647265568070000000);
}

TEST(DateTime, AddMonths)
{
    auto dateTime = DateTime(2022, 2, 7, 23, 6, 47).AddMonths(30);
    EXPECT_EQ(dateTime.Year(), 2024);
    EXPECT_EQ(dateTime.Month(), 8);
    EXPECT_EQ(dateTime.Day(), 7);
    EXPECT_EQ(dateTime.Hour(), 23);
    EXPECT_EQ(dateTime.Minute(), 6);
    EXPECT_EQ(dateTime.Second(), 47);
    EXPECT_EQ(dateTime.Ticks(), 638586688070000000);
}

TEST(DateTime, AddDays)
{
    auto dateTime1 = DateTime(2022, 2, 7, 23, 6, 47).AddDays(30);
    EXPECT_EQ(dateTime1.Year(), 2022);
    EXPECT_EQ(dateTime1.Month(), 3);
    EXPECT_EQ(dateTime1.Day(), 9);
    EXPECT_EQ(dateTime1.Hour(), 23);
    EXPECT_EQ(dateTime1.Minute(), 6);
    EXPECT_EQ(dateTime1.Second(), 47);
    EXPECT_EQ(dateTime1.Ticks(), 637824640070000000);

    auto dateTime2 = DateTime(2022, 2, 7, 23, 6, 47).AddDays(37.458);
    EXPECT_EQ(dateTime2.Year(), 2022);
    EXPECT_EQ(dateTime2.Month(), 3);
    EXPECT_EQ(dateTime2.Day(), 17);
    EXPECT_EQ(dateTime2.Hour(), 10);
    EXPECT_EQ(dateTime2.Minute(), 6);
    EXPECT_EQ(dateTime2.Second(), 18);
    EXPECT_EQ(dateTime2.Ticks(), 637831083782000000);
}

TEST(DateTime, AddHours)
{
    auto dateTime1 = DateTime(2022, 2, 7, 23, 6, 47).AddHours(30);
    EXPECT_EQ(dateTime1.Year(), 2022);
    EXPECT_EQ(dateTime1.Month(), 2);
    EXPECT_EQ(dateTime1.Day(), 9);
    EXPECT_EQ(dateTime1.Hour(), 5);
    EXPECT_EQ(dateTime1.Minute(), 6);
    EXPECT_EQ(dateTime1.Second(), 47);
    EXPECT_EQ(dateTime1.Ticks(), 637799800070000000);

    auto dateTime2 = DateTime(2022, 2, 7, 23, 6, 47).AddHours(37.458);
    EXPECT_EQ(dateTime2.Year(), 2022);
    EXPECT_EQ(dateTime2.Month(), 2);
    EXPECT_EQ(dateTime2.Day(), 9);
    EXPECT_EQ(dateTime2.Hour(), 12);
    EXPECT_EQ(dateTime2.Minute(), 34);
    EXPECT_EQ(dateTime2.Second(), 15);
    EXPECT_EQ(dateTime2.Ticks(), 637800068558000000);
}

TEST(DateTime, AddMilliseconds)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 58).AddMilliseconds(1000).Second(), 59);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 58).AddMilliseconds(2000).Second(), 0);

    auto dateTime = DateTime(2022, 2, 7, 23, 6, 47).AddMilliseconds(37458);
    EXPECT_EQ(dateTime.Year(), 2022);
    EXPECT_EQ(dateTime.Month(), 2);
    EXPECT_EQ(dateTime.Day(), 7);
    EXPECT_EQ(dateTime.Hour(), 23);
    EXPECT_EQ(dateTime.Minute(), 7);
    EXPECT_EQ(dateTime.Second(), 24);
    EXPECT_EQ(dateTime.Millisecond(), 458);
    EXPECT_EQ(dateTime.Ticks(), 637798720444580000);
}

TEST(DateTime, AddMinutes)
{
    auto dateTime1 = DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(9743);
    EXPECT_EQ(dateTime1.Year(), 2022);
    EXPECT_EQ(dateTime1.Month(), 2);
    EXPECT_EQ(dateTime1.Day(), 14);
    EXPECT_EQ(dateTime1.Hour(), 17);
    EXPECT_EQ(dateTime1.Minute(), 29);
    EXPECT_EQ(dateTime1.Second(), 47);
    EXPECT_EQ(dateTime1.Ticks(), 637804565870000000);

    auto dateTime2 = DateTime(2022, 2, 7, 23, 6, 47).AddMinutes(34471);
    EXPECT_EQ(dateTime2.Year(), 2022);
    EXPECT_EQ(dateTime2.Month(), 3);
    EXPECT_EQ(dateTime2.Day(), 3);
    EXPECT_EQ(dateTime2.Hour(), 21);
    EXPECT_EQ(dateTime2.Minute(), 37);
    EXPECT_EQ(dateTime2.Second(), 47);
    EXPECT_EQ(dateTime2.Ticks(), 637819402670000000);
}

TEST(DateTime, AddSeconds)
{
    auto dateTime = DateTime(2022, 2, 7, 23, 6, 47).AddSeconds(75645);
    EXPECT_EQ(dateTime.Year(), 2022);
    EXPECT_EQ(dateTime.Month(), 2);
    EXPECT_EQ(dateTime.Day(), 8);
    EXPECT_EQ(dateTime.Hour(), 20);
    EXPECT_EQ(dateTime.Minute(), 7);
    EXPECT_EQ(dateTime.Second(), 32);
    EXPECT_EQ(dateTime.Ticks(), 637799476520000000);
}

TEST(DateTime, AddTicks)
{
    auto dateTime = DateTime(2022, 11, 6, 22, 59, 2, 456).AddTicks(6380825103);
    EXPECT_EQ(dateTime.Year(), 2022);
    EXPECT_EQ(dateTime.Month(), 11);
    EXPECT_EQ(dateTime.Day(), 6);
    EXPECT_EQ(dateTime.Hour(), 23);
    EXPECT_EQ(dateTime.Minute(), 9);
    EXPECT_EQ(dateTime.Second(), 40);
    EXPECT_EQ(dateTime.Millisecond(), 538);
    EXPECT_EQ(dateTime.Ticks(), 638033729805385103);
    EXPECT_EQ(dateTime.TimeOfDay().Ticks(), 833805385103);
    EXPECT_EQ(dateTime.Kind(), DateTimeKind::Unspecified);
}

TEST(DateTime, Subtract)
{
    auto dateTime = DateTime(2022, 11, 6, 22, 59, 2, 456).Subtract(TimeSpan(30, 9, 45, 3));
    EXPECT_EQ(dateTime.Year(), 2022);
    EXPECT_EQ(dateTime.Month(), 10);
    EXPECT_EQ(dateTime.Day(), 7);
    EXPECT_EQ(dateTime.Hour(), 13);
    EXPECT_EQ(dateTime.Minute(), 13);
    EXPECT_EQ(dateTime.Second(), 59);
    EXPECT_EQ(dateTime.Millisecond(), 456);
    EXPECT_EQ(dateTime.Ticks(), 638007452394560000);
    EXPECT_EQ(dateTime.TimeOfDay().Ticks(), 476394560000);
    EXPECT_EQ(dateTime.Kind(), DateTimeKind::Unspecified);
}

TEST(DateTime, Compare)
{
    EXPECT_EQ(DateTime::Compare(DateTime(2022, 2, 7, 23, 6, 47), DateTime(2022, 2, 7, 23, 6, 47)), 0);
    EXPECT_EQ(DateTime::Compare(DateTime(2022, 2, 7, 23, 6, 47), DateTime(2022, 2, 8, 23, 6, 47)), -1);
    EXPECT_EQ(DateTime::Compare(DateTime(2022, 2, 8, 23, 6, 47), DateTime(2022, 2, 7, 23, 6, 47)), 1);
}

TEST(DateTime, CompareTo)
{
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).CompareTo(DateTime(2022, 2, 7, 23, 6, 47)), 0);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47).CompareTo(DateTime(2022, 2, 8, 23, 6, 47)), -1);
    EXPECT_EQ(DateTime(2022, 2, 8, 23, 6, 47).CompareTo(DateTime(2022, 2, 7, 23, 6, 47)), 1);
}

TEST(DateTime, Equals)
{
    auto dateTime1 = DateTime(2022, 2, 7, 23, 6, 47, DateTimeKind::Utc);
    auto dateTime2 = DateTime(2022, 2, 7, 23, 6, 47, DateTimeKind::Local);
    EXPECT_TRUE(DateTime::Equals(dateTime1, dateTime2));
    EXPECT_TRUE(DateTime::Equals(dateTime1, dateTime1));
    EXPECT_TRUE(dateTime1.Equals(dateTime2));
    EXPECT_TRUE(dateTime1.Equals(dateTime1));
}

TEST(DateTime, DaysInMonth)
{
    EXPECT_EQ(DateTime::DaysInMonth(2000, 1), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 2), 29);
    EXPECT_EQ(DateTime::DaysInMonth(2001, 2), 28);
    EXPECT_EQ(DateTime::DaysInMonth(2004, 2), 29);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 3), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 4), 30);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 5), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 6), 30);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 7), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 8), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 9), 30);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 10), 31);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 11), 30);
    EXPECT_EQ(DateTime::DaysInMonth(2000, 12), 31);
}

TEST(DateTime, FromFileTime)
{
    auto tempA = DateTime::FromFileTime(10000000);
    auto tempB = DateTime::FromFileTimeUtc(0);
    EXPECT_EQ(tempA - tempB, TimeZoneInfo::Local().BaseUtcOffset().Add(TimeSpan(0, 0, 1)));
    EXPECT_EQ(tempA.Year(), 1601);
    EXPECT_EQ(tempA.Month(), 1);
    EXPECT_EQ(tempA.Day(), 1);
    EXPECT_EQ(tempB.Hour(), 0);
    EXPECT_EQ(tempA.Second(), 1);
}

TEST(DateTime, GetHashCode)
{
    EXPECT_EQ(DateTime::MaxValue().GetHashCode(), -537061494);
    EXPECT_EQ(DateTime::MinValue().GetHashCode(), 0);
    EXPECT_EQ((DateTime(2022, 11, 6, 22, 59, 2, 456, DateTimeKind::Local)).GetHashCode(), 2007634890);
}

TEST(DateTime, Date)
{
    EXPECT_EQ(DateTime::Now().Date(), DateTime::Today());
}

TEST(DateTime, DayOfWeek)
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
    auto dateTime = DateTime::MaxValue();
    EXPECT_EQ(dateTime.Year(), 9999);
    EXPECT_EQ(dateTime.Month(), 12);
    EXPECT_EQ(dateTime.Day(), 31);
    EXPECT_EQ(dateTime.Hour(), 23);
    EXPECT_EQ(dateTime.Minute(), 59);
    EXPECT_EQ(dateTime.Second(), 59);
    EXPECT_EQ(dateTime.Millisecond(), 999);
    EXPECT_EQ(dateTime.Kind(), DateTimeKind::Unspecified);
    EXPECT_EQ(dateTime.Ticks(), 3155378975999999999);
    EXPECT_EQ(dateTime.TimeOfDay().Ticks(), 863999999999);
}

TEST(DateTime, MinValue)
{
    auto dateTime = DateTime::MinValue();
    EXPECT_EQ(dateTime.Year(), 1);
    EXPECT_EQ(dateTime.Month(), 1);
    EXPECT_EQ(dateTime.Day(), 1);
    EXPECT_EQ(dateTime.Hour(), 0);
    EXPECT_EQ(dateTime.Minute(), 0);
    EXPECT_EQ(dateTime.Second(), 0);
    EXPECT_EQ(dateTime.Millisecond(), 0);
    EXPECT_EQ(dateTime.Kind(), DateTimeKind::Unspecified);
    EXPECT_EQ(dateTime.Ticks(), 0);
    EXPECT_EQ(dateTime.TimeOfDay().Ticks(), 0);
}

TEST(DateTime, Now)
{
    auto now = DateTime::Now();
    EXPECT_EQ(now.Kind(), DateTimeKind::Local);
}

TEST(DateTime, UtcNow)
{
    auto now = DateTime::Now();
    auto utcNow = DateTime::UtcNow();
    auto timeDiff = now - utcNow;
    EXPECT_LE(TimeZoneInfo::Local().BaseUtcOffset().Ticks() - timeDiff.Ticks(), 1000);
    EXPECT_EQ(utcNow.Kind(), DateTimeKind::Utc);
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

TEST(DateTime, UnixEpoch)
{
    EXPECT_EQ(DateTime::UnixEpoch().Year(), 1970);
    EXPECT_EQ(DateTime::UnixEpoch().Month(), 1);
    EXPECT_EQ(DateTime::UnixEpoch().Day(), 1);
    EXPECT_EQ(DateTime::UnixEpoch().Hour(), 0);
    EXPECT_EQ(DateTime::UnixEpoch().Minute(), 0);
    EXPECT_EQ(DateTime::UnixEpoch().Second(), 0);
    EXPECT_EQ(DateTime::UnixEpoch().Kind(), DateTimeKind::Utc);
    EXPECT_EQ(DateTime::UnixEpoch().Ticks(), 621355968000000000);
    EXPECT_EQ(DateTime::UnixEpoch().TimeOfDay().Ticks(), 0);
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

TEST(DateTime, SpecifyKind)
{
    auto now = DateTime::Now();
    auto utcNow = DateTime::Now();
    EXPECT_EQ(DateTime::SpecifyKind(now, DateTimeKind::Unspecified).Kind(), DateTimeKind::Unspecified);
    EXPECT_EQ(DateTime::SpecifyKind(utcNow, DateTimeKind::Local).Kind(), DateTimeKind::Local);
}

TEST(DateTime, ToFileTime)
{
    auto offsetTicks = TimeZoneInfo::Local().BaseUtcOffset().Ticks();
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47, DateTimeKind::Utc).ToFileTime(), 132887488070000000);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47, DateTimeKind::Local).ToFileTime(), 132887488070000000 - offsetTicks);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47, DateTimeKind::Unspecified).ToFileTime(), 132887488070000000 - offsetTicks);
}

TEST(DateTime, ToFileTimeUtc)
{
    auto offsetTicks = TimeZoneInfo::Local().BaseUtcOffset().Ticks();
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47, DateTimeKind::Utc).ToFileTimeUtc(), 132887488070000000);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47, DateTimeKind::Local).ToFileTimeUtc(), 132887488070000000 - offsetTicks);
    EXPECT_EQ(DateTime(2022, 2, 7, 23, 6, 47, DateTimeKind::Unspecified).ToFileTimeUtc(), 132887488070000000);
}

TEST(DateTime, ToLongDateString)
{
    auto dateTimeStr = DateTime(2022, 11, 9, 22, 46, 47).ToLongDateString();
    EXPECT_TRUE(dateTimeStr.find(u"2022") != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u'9') != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"10") == std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"46") == std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"47") == std::string::npos);
}

TEST(DateTime, ToLongTimeString)
{
    auto dateTimeStr = DateTime(2022, 11, 9, 22, 46, 47).ToLongTimeString();
    EXPECT_TRUE(dateTimeStr.find(u"2022") == std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u'9') == std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"10") != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"46") != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"47") != std::string::npos);
}

TEST(DateTime, ToShortDateString)
{
    auto dateTimeStr = DateTime(2022, 11, 9, 22, 46, 47).ToShortDateString();
    EXPECT_TRUE(dateTimeStr.find(u"2022") != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u'9') != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"10") == std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"46") == std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"47") == std::string::npos);
}

TEST(DateTime, ToShortTimeString)
{
    auto dateTimeStr = DateTime(2022, 11, 9, 22, 46, 47).ToShortTimeString();
    EXPECT_TRUE(dateTimeStr.find(u"2022") == std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u'9') == std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"10") != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"46") != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"47") == std::string::npos);
}

TEST(DateTime, ToString)
{
    auto dateTime = DateTime(2022, 11, 9, 22, 46, 47);
    EXPECT_EQ(dateTime.ToString(u"yyyy-MM.d hh:m:ss"), u"2022-11.9 10:46:47");

    auto dateTimeStr = dateTime.ToString();
    EXPECT_TRUE(dateTimeStr.find(u"2022") != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u'9') != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"10") != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"46") != std::string::npos);
    EXPECT_TRUE(dateTimeStr.find(u"47") != std::string::npos);
}