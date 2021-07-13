#pragma once

#include <chrono>
#include <cstdint>
#include <ctime>
#include <tuple>

#include "TimeSpan.h"

CS2CPP_NAMESPACE_BEGIN

enum class DateTimeKind
{
    Unspecified = 0x0,
    Local = 0x1,
    Utc = 0x2,
};

enum class DayOfWeek
{
    Sunday = 0,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
};

struct DateTime final
{
public:
    constexpr DateTime() noexcept = default;
    constexpr DateTime(int32_t year, int32_t month, int32_t day) noexcept;
    constexpr DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second) noexcept;
    constexpr DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, DateTimeKind dateTimeKind) noexcept;
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
    [[nodiscard]] static constexpr int32_t Compare(DateTime lhs, DateTime rhs) noexcept;
    [[nodiscard]] constexpr int32_t CompareTo(DateTime value) const noexcept;
    [[nodiscard]] static DateTime Now();
    [[nodiscard]] static DateTime UtcNow();
    [[nodiscard]] static DateTime Today();
    [[nodiscard]] static constexpr bool IsLeapYear(int32_t year) noexcept;
    [[nodiscard]] static constexpr int32_t DaysInMonth(int32_t year, int32_t month) noexcept;
    [[nodiscard]] constexpr DateTime AddYears(int32_t value) const noexcept;
    [[nodiscard]] constexpr DateTime AddMonths(int32_t value) const noexcept;
    [[nodiscard]] constexpr DateTime AddDays(double value) const noexcept;
    [[nodiscard]] constexpr DateTime AddHours(double value) const noexcept;
    [[nodiscard]] constexpr DateTime AddMinutes(double value) const noexcept;
    [[nodiscard]] constexpr DateTime AddSeconds(double value) const noexcept;
    [[nodiscard]] constexpr int32_t Year() const noexcept;
    [[nodiscard]] constexpr int32_t Month() const noexcept;
    [[nodiscard]] constexpr int32_t Day() const noexcept;
    [[nodiscard]] constexpr int32_t Hour() const noexcept;
    [[nodiscard]] constexpr int32_t Minute() const noexcept;
    [[nodiscard]] constexpr int32_t Second() const noexcept;
    [[nodiscard]] constexpr int64_t Ticks() const noexcept;
    [[nodiscard]] constexpr TimeSpan TimeOfDay() const noexcept;
    [[nodiscard]] constexpr DateTime Date() const noexcept;
    [[nodiscard]] static constexpr enum DayOfWeek DayOfWeek(int32_t year, int32_t month, int32_t day);
    [[nodiscard]] constexpr enum DayOfWeek DayOfWeek() const noexcept;
    [[nodiscard]] constexpr int32_t DayOfYear() const noexcept;
    [[nodiscard]] constexpr DateTimeKind Kind() const noexcept;
    [[nodiscard]] static constexpr DateTime MaxValue() noexcept;
    [[nodiscard]] static constexpr DateTime MinValue() noexcept;
    [[nodiscard]] static constexpr DateTime UnixEpoch() noexcept;
    [[nodiscard]] DateTime ToLocalTime() const;
    [[nodiscard]] DateTime ToUniversalTime() const;
    [[nodiscard]] int64_t ToFileTime() const;
    [[nodiscard]] int64_t ToFileTimeUtc() const;
    [[nodiscard]] std::u16string ToString() const;
    [[nodiscard]] std::u16string ToString(std::u16string_view format) const;

private:
    [[nodiscard]] static constexpr int64_t DateToTicks(int32_t year, int32_t month, int32_t day) noexcept;
    [[nodiscard]] static constexpr int64_t TimeToTicks(int32_t hour, int32_t minute, int32_t second) noexcept;
    [[nodiscard]] constexpr int32_t GetDatePart(int32_t part) const noexcept;
    [[nodiscard]] static int64_t GetTimeSinceUnixEpoch();

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

    uint64_t _ticks = 0;
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

constexpr DateTime::DateTime(uint64_t ticks, DateTimeKind dateTimeKind) noexcept :
    _ticks(ticks | (static_cast<uint64_t>(dateTimeKind) << DateTime::KindShift))
{
}

constexpr DateTime::DateTime(uint64_t ticks) noexcept :
    _ticks(ticks)
{
}

constexpr DateTime DateTime::operator+(TimeSpan rhs) const noexcept
{
    return DateTime(Ticks() + rhs.Ticks(), Kind());
}

constexpr TimeSpan DateTime::operator-(DateTime rhs) const noexcept
{
    return TimeSpan(Ticks() - rhs.Ticks());
}

constexpr DateTime DateTime::operator-(TimeSpan rhs) const noexcept
{
    return DateTime(Ticks() - rhs.Ticks(), Kind());
}

inline DateTime& DateTime::operator+=(TimeSpan rhs) noexcept
{
    _ticks += rhs.Ticks();
    return *this;
}

inline DateTime& DateTime::operator-=(TimeSpan rhs) noexcept
{
    _ticks -= rhs.Ticks();
    return *this;
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

constexpr int32_t DateTime::Compare(DateTime lhs, DateTime rhs) noexcept
{
    return lhs.CompareTo(rhs);
}

constexpr int32_t DateTime::CompareTo(DateTime value) const noexcept
{
    if (Ticks() > value.Ticks())
        return 1;

    if (Ticks() < value.Ticks())
        return -1;

    return 0;
}

inline DateTime DateTime::Now()
{
    time_t utcTime = 0;
    time(&utcTime);

#ifdef _MSC_VER
    tm localTimeInfo{};
    gmtime_s(&localTimeInfo, &utcTime);

    auto localTime = mktime(&localTimeInfo);
    auto utcOffset = static_cast<int64_t>((difftime(utcTime, localTime) / 3600.0) * TimeSpan::TicksPerHour);
#else
    const auto* localTimeInfo = localtime(&utcTime);
    int64_t utcOffset = (localTimeInfo->tm_gmtoff / 3600) * TimeSpan::TicksPerHour;
#endif

    return DateTime(UnixEpoch().Ticks() + GetTimeSinceUnixEpoch() + utcOffset, DateTimeKind::Local);
}

inline DateTime DateTime::UtcNow()
{
    return DateTime(UnixEpoch().Ticks() + GetTimeSinceUnixEpoch(), DateTimeKind::Local);
}

inline DateTime DateTime::Today()
{
    return Now().Date();
}

constexpr bool DateTime::IsLeapYear(int32_t year) noexcept
{
    return (year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0));
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

constexpr DateTime DateTime::AddYears(int32_t value) const noexcept
{
    if (value < -10000 || value > 10000)
        return DateTime(0);

    return AddMonths(value * 12);
}

constexpr DateTime DateTime::AddMonths(int32_t value) const noexcept
{
    if (value < -120000 || value > 120000)
        return DateTime(0);

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
        return DateTime(0);

    auto day = GetDatePart(DatePartDay);
    auto days = DaysInMonth(year, month);
    if (day > days)
        day = days;

    return DateTime(DateToTicks(year, month, day) + (Ticks() % TimeSpan::TicksPerDay), Kind());
}

constexpr DateTime DateTime::AddDays(double value) const noexcept
{
    return DateTime(Ticks() + static_cast<int64_t>(value * TimeSpan::TicksPerDay), Kind());
}

constexpr DateTime DateTime::AddHours(double value) const noexcept
{
    return DateTime(Ticks() + static_cast<int64_t>(value * TimeSpan::TicksPerHour), Kind());
}

constexpr DateTime DateTime::AddMinutes(double value) const noexcept
{
    return DateTime(Ticks() + static_cast<int64_t>(value * TimeSpan::TicksPerMinute), Kind());
}

constexpr DateTime DateTime::AddSeconds(double value) const noexcept
{
    return DateTime(Ticks() + static_cast<int64_t>(value * TimeSpan::TicksPerSecond), Kind());
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

constexpr int64_t DateTime::Ticks() const noexcept
{
    return static_cast<int64_t>(_ticks & TicksMask);
}

constexpr TimeSpan DateTime::TimeOfDay() const noexcept
{
    return TimeSpan(Ticks() % TimeSpan::TicksPerDay);
}

constexpr DateTime DateTime::Date() const noexcept
{
    auto ticks = Ticks();
    return DateTime(ticks - (ticks % TimeSpan::TicksPerDay), Kind());
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
    return static_cast<DateTimeKind>(_ticks >> KindShift);
}

constexpr DateTime DateTime::MaxValue() noexcept
{
    return DateTime(DaysTo10000 * TimeSpan::TicksPerDay - 1);
}

constexpr DateTime DateTime::MinValue() noexcept
{
    return DateTime();
}

constexpr DateTime DateTime::UnixEpoch() noexcept
{
    return DateTime(1970, 1, 1);
}

constexpr int64_t DateTime::DateToTicks(int32_t year, int32_t month, int32_t day) noexcept
{
    if (1 > month || month > 12)
        return 0;

    const auto& days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
    if (1 > day || day > days[month])
        return 0;

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
        y100 = 3;
    n -= y100 * DaysPer100Years;

    auto y4 = n / DaysPer4Years;
    n -= y4 * DaysPer4Years;
    auto y1 = n / DaysPerYear;
    if (y1 == 4)
        y1 = 3;

    if (part == DatePartYear)
        return static_cast<int32_t>(y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1);

    n -= y1 * DaysPerYear;
    if (part == DatePartDayOfYear)
        return static_cast<int32_t>(n + 1);

    auto isLeapYear = y1 == 3 && (y4 != 24 || y100 == 3);
    const auto& days = isLeapYear ? DaysToMonth366 : DaysToMonth365;
    auto m = (n >> 5) + 1;
    while (n >= days[m])
        ++m;

    if (part == DatePartMonth)
        return static_cast<int32_t>(m);

    return static_cast<int32_t>(n - days[m - 1] + 1);
}

inline int64_t DateTime::GetTimeSinceUnixEpoch()
{
    auto timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();

    using DurationType = decltype(timeSinceEpoch);
    if constexpr (DurationType::period::den == 10'000'000)
        return timeSinceEpoch.count();
    else
        return std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<1, 10000000>>>(timeSinceEpoch).count();
}

inline int64_t DateTime::ToFileTime() const
{
    return ToFileTimeUtc();
}

inline int64_t DateTime::ToFileTimeUtc() const
{
    return ToUniversalTime().Ticks() - FileTimeOffset;
}

CS2CPP_NAMESPACE_END
