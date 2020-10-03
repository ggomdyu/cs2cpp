#pragma once

#include <chrono>
#include <cstdint>
#include <ctime>
#include <tuple>

#include "DateTimeKind.h"
#include "DayOfWeek.h"
#include "TimeSpan.h"

CS2CPP_NAMESPACE_BEGIN

struct DateTime final
{
public:
    constexpr DateTime() noexcept = default;
    constexpr DateTime(int32_t year, int32_t month, int32_t day) noexcept;
    constexpr DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second) noexcept;
    constexpr DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, DateTimeKind dateTimeKind) noexcept;
    constexpr DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond) noexcept;
    constexpr DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond, DateTimeKind dateTimeKind) noexcept;
    constexpr DateTime(uint64_t ticks, DateTimeKind dateTimeKind) noexcept;
    constexpr explicit DateTime(uint64_t ticks) noexcept;

public:
    constexpr DateTime operator+(TimeSpan rhs) const noexcept;
    constexpr TimeSpan operator-(DateTime rhs) const noexcept;
    constexpr DateTime operator-(TimeSpan rhs) const noexcept;
    DateTime& operator+=(TimeSpan rhs) noexcept;
    DateTime& operator-=(TimeSpan rhs) noexcept;
    constexpr bool operator==(DateTime rhs) const noexcept;
    constexpr bool operator!=(DateTime rhs) const noexcept;
    constexpr bool operator>(DateTime rhs) const noexcept;
    constexpr bool operator>=(DateTime rhs) const noexcept;
    constexpr bool operator<(DateTime rhs) const noexcept;
    constexpr bool operator<=(DateTime rhs) const noexcept;

public:
    constexpr DateTime Add(TimeSpan value) const noexcept;
    constexpr DateTime AddYears(int32_t value) const noexcept;
    constexpr DateTime AddMonths(int32_t value) const noexcept;
    constexpr DateTime AddDays(double value) const noexcept;
    constexpr DateTime AddHours(double value) const noexcept;
    constexpr DateTime AddMilliseconds(double value) const noexcept;
    constexpr DateTime AddMinutes(double value) const noexcept;
    constexpr DateTime AddSeconds(double value) const noexcept;
    constexpr DateTime AddTicks(int64_t value) const noexcept;
    constexpr DateTime Subtract(TimeSpan value) const noexcept;
    static constexpr int32_t Compare(DateTime lhs, DateTime rhs) noexcept;
    constexpr int32_t CompareTo(DateTime value) const noexcept;
    constexpr bool Equals(DateTime value) const noexcept;
    static constexpr bool Equals(DateTime lhs, DateTime rhs) noexcept;
    static constexpr int32_t DaysInMonth(int32_t year, int32_t month) noexcept;
    static DateTime FromFileTime(int64_t fileTime) noexcept;
    static DateTime FromFileTimeUtc(int64_t fileTime) noexcept;
    constexpr int32_t GetHashCode() const noexcept;
    constexpr int32_t Year() const noexcept;
    constexpr int32_t Month() const noexcept;
    constexpr int32_t Day() const noexcept;
    constexpr int32_t Hour() const noexcept;
    constexpr int32_t Minute() const noexcept;
    constexpr int32_t Second() const noexcept;
    constexpr int32_t Millisecond() const noexcept;
    constexpr int64_t Ticks() const noexcept;
    constexpr TimeSpan TimeOfDay() const noexcept;
    constexpr DateTime Date() const noexcept;
    constexpr enum DayOfWeek DayOfWeek() const noexcept;
    static constexpr enum DayOfWeek DayOfWeek(int32_t year, int32_t month, int32_t day);
    constexpr int32_t DayOfYear() const noexcept;
    constexpr DateTimeKind Kind() const noexcept;
    static constexpr DateTime MaxValue() noexcept;
    static constexpr DateTime MinValue() noexcept;
    static DateTime Now();
    static DateTime UtcNow();
    static DateTime Today();
    static constexpr DateTime UnixEpoch() noexcept;
    static constexpr bool IsLeapYear(int32_t year) noexcept;
    static constexpr DateTime SpecifyKind(DateTime value, DateTimeKind kind) noexcept;
    int64_t ToFileTime() const;
    int64_t ToFileTimeUtc() const;
    DateTime ToLocalTime() const;
    DateTime ToUniversalTime() const;
    std::u16string ToLongDateString() const;
    std::u16string ToLongTimeString() const;
    std::u16string ToShortDateString() const;
    std::u16string ToShortTimeString() const;
    std::u16string ToString() const;
    std::u16string ToString(std::u16string_view format) const;

private:
    static constexpr int64_t DateToTicks(int32_t year, int32_t month, int32_t day) noexcept;
    static constexpr int64_t TimeToTicks(int32_t hour, int32_t minute, int32_t second) noexcept;
    constexpr int32_t GetDatePart(int32_t part) const noexcept;
    static int64_t GetTimeSinceUnixEpoch();

private:
    static constexpr uint64_t TicksMask = 0x3FFFFFFFFFFFFFFF;
    static constexpr uint64_t KindShift = 62;
    static constexpr int32_t DatePartYear = 0;
    static constexpr int32_t DatePartDayOfYear = 1;
    static constexpr int32_t DatePartMonth = 2;
    static constexpr int32_t DatePartDay = 3;
    static constexpr int32_t DaysToMonth365[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    static constexpr int32_t DaysToMonth366[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
    static constexpr int32_t DaysPerYear = 365;
    static constexpr int32_t DaysPer4Years = DaysPerYear * 4 + 1;
    static constexpr int32_t DaysPer100Years = DaysPer4Years * 25 - 1;
    static constexpr int32_t DaysPer400Years = DaysPer100Years * 4 + 1;
    static constexpr int32_t DaysTo1601 = DaysPer400Years * 4;
    static constexpr int32_t DaysTo10000 = DaysPer400Years * 25 - 366;
    static constexpr int64_t FileTimeOffset = DaysTo1601 * TimeSpan::TicksPerDay;

    uint64_t ticks_ = 0;
};

constexpr DateTime::DateTime(int32_t year, int32_t month, int32_t day) noexcept :
    DateTime(year, month, day, 0, 0, 0)
{
}

constexpr DateTime::DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second) noexcept :
    DateTime(year, month, day, hour, minute, second, DateTimeKind::Unspecified)
{
}

constexpr DateTime::DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, DateTimeKind dateTimeKind) noexcept :
    DateTime(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second), dateTimeKind)
{
}

constexpr DateTime::DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond) noexcept :
    DateTime(year, month, day, hour, minute, second, millisecond, DateTimeKind::Unspecified)
{
}

constexpr DateTime::DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond, DateTimeKind dateTimeKind) noexcept :
    DateTime(DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) + TimeSpan::TicksPerMillisecond * millisecond, dateTimeKind)
{
}

constexpr DateTime::DateTime(uint64_t ticks, DateTimeKind dateTimeKind) noexcept :
    ticks_(ticks | (static_cast<uint64_t>(dateTimeKind) << DateTime::KindShift))
{
}

constexpr DateTime::DateTime(uint64_t ticks) noexcept :
    DateTime(ticks, DateTimeKind::Unspecified)
{
}

constexpr DateTime DateTime::operator+(TimeSpan rhs) const noexcept
{
    return {static_cast<uint64_t>(Ticks() + rhs.Ticks()), Kind()};
}

constexpr TimeSpan DateTime::operator-(DateTime rhs) const noexcept
{
    return TimeSpan(Ticks() - rhs.Ticks());
}

constexpr DateTime DateTime::operator-(TimeSpan rhs) const noexcept
{
    return {static_cast<uint64_t>(Ticks() - rhs.Ticks()), Kind()};
}

constexpr bool DateTime::operator==(DateTime rhs) const noexcept
{
    return Ticks() == rhs.Ticks();
}

constexpr bool DateTime::operator!=(DateTime rhs) const noexcept
{
    return Ticks() != rhs.Ticks();
}

constexpr bool DateTime::operator>(DateTime rhs) const noexcept
{
    return Ticks() > rhs.Ticks();
}

constexpr bool DateTime::operator>=(DateTime rhs) const noexcept
{
    return Ticks() >= rhs.Ticks();
}

constexpr bool DateTime::operator<(DateTime rhs) const noexcept
{
    return Ticks() < rhs.Ticks();
}

constexpr bool DateTime::operator<=(DateTime rhs) const noexcept
{
    return Ticks() <= rhs.Ticks();
}

constexpr DateTime DateTime::Add(TimeSpan value) const noexcept
{
    return AddTicks(value.Ticks());
}

constexpr DateTime DateTime::AddYears(int32_t value) const noexcept
{
    return AddMonths(value * 12);
}

constexpr DateTime DateTime::AddMonths(int32_t value) const noexcept
{
    auto year = GetDatePart(DatePartYear);
    auto month = GetDatePart(DatePartMonth);
    auto i = month + value;
    if (i >= 0)
    {
        month = ((i - 1) % 12) + 1;
        year = year + i / 12;
    }
    else
    {
        month = 12 + i % 12;
        year = year + (i - 11) / 12;
    }

    if (year < 1 || year > 9999)
    {
        return DateTime(0);
    }

    auto day = GetDatePart(DatePartDay);
    auto days = DaysInMonth(year, month);
    if (day > days)
    {
        day = days;
    }

    return {static_cast<uint64_t>(DateToTicks(year, month, day) + (Ticks() % TimeSpan::TicksPerDay)), Kind()};
}

constexpr DateTime DateTime::AddDays(double value) const noexcept
{
    return {static_cast<uint64_t>(Ticks() + static_cast<int64_t>(value * TimeSpan::TicksPerDay)), Kind()};
}

constexpr DateTime DateTime::AddHours(double value) const noexcept
{
    return {static_cast<uint64_t>(Ticks() + static_cast<int64_t>(value * TimeSpan::TicksPerHour)), Kind()};
}

constexpr DateTime DateTime::AddMilliseconds(double value) const noexcept
{
    return {static_cast<uint64_t>(Ticks() + static_cast<int64_t>(value * TimeSpan::TicksPerMillisecond)), Kind()};
}

constexpr DateTime DateTime::AddMinutes(double value) const noexcept
{
    return {static_cast<uint64_t>(Ticks() + static_cast<int64_t>(value * TimeSpan::TicksPerMinute)), Kind()};
}

constexpr DateTime DateTime::AddSeconds(double value) const noexcept
{
    return {static_cast<uint64_t>(Ticks() + static_cast<int64_t>(value * TimeSpan::TicksPerSecond)), Kind()};
}

constexpr DateTime DateTime::AddTicks(int64_t value) const noexcept
{
    return {static_cast<uint64_t>(Ticks() + value), Kind()};
}

constexpr DateTime DateTime::Subtract(TimeSpan value) const noexcept
{
    return {static_cast<uint64_t>(Ticks() - value.Ticks()), Kind()};
}

constexpr int32_t DateTime::Compare(DateTime lhs, DateTime rhs) noexcept
{
    return lhs.CompareTo(rhs);
}

constexpr int32_t DateTime::CompareTo(DateTime value) const noexcept
{
    if (Ticks() > value.Ticks())
    {
        return 1;
    }

    if (Ticks() < value.Ticks())
    {
        return -1;
    }

    return 0;
}

constexpr bool DateTime::Equals(DateTime value) const noexcept
{
    return Ticks() == value.Ticks();
}

constexpr bool DateTime::Equals(DateTime lhs, DateTime rhs) noexcept
{
    return lhs.Equals(rhs);
}

constexpr int32_t DateTime::DaysInMonth(int32_t year, int32_t month) noexcept
{
    if (month == 2)
    {
        const auto& days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
        return static_cast<int32_t>(days[month] - days[month - 1]);
    }

    return static_cast<int32_t>(DaysToMonth365[month] - DaysToMonth365[month - 1]);
}

constexpr int32_t DateTime::GetHashCode() const noexcept
{
    auto ticks = Ticks();
    return static_cast<int32_t>(ticks ^ (ticks >> 32));
}

constexpr int32_t DateTime::Year() const noexcept
{
    return GetDatePart(DatePartYear);
}

constexpr int32_t DateTime::Month() const noexcept
{
    return GetDatePart(DatePartMonth);
}

constexpr int32_t DateTime::Day() const noexcept
{
    return GetDatePart(DatePartDay);
}

constexpr int32_t DateTime::Hour() const noexcept
{
    return static_cast<int32_t>((Ticks() / TimeSpan::TicksPerHour) % 24);
}

constexpr int32_t DateTime::Minute() const noexcept
{
    return static_cast<int32_t>((Ticks() / TimeSpan::TicksPerMinute) % 60);
}

constexpr int32_t DateTime::Second() const noexcept
{
    return static_cast<int32_t>((Ticks() / TimeSpan::TicksPerSecond) % 60);
}

constexpr int32_t DateTime::Millisecond() const noexcept
{
    return static_cast<int32_t>((Ticks() / TimeSpan::TicksPerMillisecond) % 1000);
}

constexpr int64_t DateTime::Ticks() const noexcept
{
    return static_cast<int64_t>(ticks_ & TicksMask);
}

constexpr TimeSpan DateTime::TimeOfDay() const noexcept
{
    return TimeSpan(Ticks() % TimeSpan::TicksPerDay);
}

constexpr DateTime DateTime::Date() const noexcept
{
    auto ticks = Ticks();
    return {static_cast<uint64_t>(ticks - (ticks % TimeSpan::TicksPerDay)), Kind()};
}

constexpr DayOfWeek DateTime::DayOfWeek() const noexcept
{
    return static_cast<enum DayOfWeek>((Ticks() / TimeSpan::TicksPerDay + 1) % 7);
}

constexpr DayOfWeek DateTime::DayOfWeek(int32_t year, int32_t month, int32_t day)
{
    constexpr int32_t termTable[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    year -= month < 3;
    return static_cast<enum DayOfWeek>((year + year / 4 - year / 100 + year / 400 + termTable[month - 1] + day) % 7);
}

constexpr int32_t DateTime::DayOfYear() const noexcept
{
    return GetDatePart(DatePartDayOfYear);
}

constexpr DateTimeKind DateTime::Kind() const noexcept
{
    return static_cast<DateTimeKind>(ticks_ >> KindShift);
}

constexpr DateTime DateTime::MaxValue() noexcept
{
    return {static_cast<uint64_t>(DaysTo10000 * TimeSpan::TicksPerDay - 1), DateTimeKind::Unspecified};
}

constexpr DateTime DateTime::MinValue() noexcept
{
    return {0, DateTimeKind::Unspecified};
}

constexpr DateTime DateTime::UnixEpoch() noexcept
{
    return {1970, 1, 1, 0, 0, 0, DateTimeKind::Utc};
}

constexpr bool DateTime::IsLeapYear(int32_t year) noexcept
{
    return (year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0));
}

constexpr DateTime DateTime::SpecifyKind(DateTime value, DateTimeKind kind) noexcept
{
    return {static_cast<uint64_t>(value.Ticks()), kind};
}

constexpr int64_t DateTime::DateToTicks(int32_t year, int32_t month, int32_t day) noexcept
{
    if (1 > month || month > 12)
    {
        return 0;
    }

    const auto& days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
    if (1 > day || day > days[month])
    {
        return 0;
    }

    auto y = year - 1;
    auto n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;
    return n * TimeSpan::TicksPerDay;
}

constexpr int64_t DateTime::TimeToTicks(int32_t hour, int32_t minute, int32_t second) noexcept
{
    auto totalSeconds = (static_cast<int64_t>(hour) * 3600) + (static_cast<int64_t>(minute) * 60) + static_cast<int64_t>(second);
    return totalSeconds * TimeSpan::TicksPerSecond;
}

constexpr int32_t DateTime::GetDatePart(int32_t part) const noexcept
{
    auto n = Ticks() / TimeSpan::TicksPerDay;
    auto y400 = n / DaysPer400Years;
    n -= y400 * DaysPer400Years;

    auto y100 = n / DaysPer100Years;
    if (y100 == 4)
    {
        y100 = 3;
    }
    n -= y100 * DaysPer100Years;

    auto y4 = n / DaysPer4Years;
    n -= y4 * DaysPer4Years;
    auto y1 = n / DaysPerYear;
    if (y1 == 4)
    {
        y1 = 3;
    }

    if (part == DatePartYear)
    {
        return static_cast<int32_t>(y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1);
    }

    n -= y1 * DaysPerYear;
    if (part == DatePartDayOfYear)
    {
        return static_cast<int32_t>(n + 1);
    }

    auto isLeapYear = y1 == 3 && (y4 != 24 || y100 == 3);
    const auto& days = isLeapYear ? DaysToMonth366 : DaysToMonth365;
    auto m = (n >> 5) + 1;
    while (n >= days[m])
    {
        ++m;
    }

    if (part == DatePartMonth)
    {
        return static_cast<int32_t>(m);
    }

    return static_cast<int32_t>(n - days[m - 1] + 1);
}

CS2CPP_NAMESPACE_END
