#pragma once

#include <cstdint>

CS2CPP_NAMESPACE_BEGIN

struct TimeSpan final
{
public:
    constexpr TimeSpan(int32_t hours, int32_t minutes, int32_t seconds) noexcept;
    constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds) noexcept;
    constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds) noexcept;
    constexpr explicit TimeSpan(int64_t ticks) noexcept;

public:
    constexpr bool operator==(TimeSpan rhs) const noexcept;
    constexpr bool operator!=(TimeSpan rhs) const noexcept;
    constexpr bool operator>(TimeSpan rhs) const noexcept;
    constexpr bool operator>=(TimeSpan rhs) const noexcept;
    constexpr bool operator<(TimeSpan rhs) const noexcept;
    constexpr bool operator<=(TimeSpan rhs) const noexcept;
    constexpr TimeSpan operator+(TimeSpan rhs) const noexcept;
    constexpr TimeSpan operator-(TimeSpan rhs) const noexcept;
    TimeSpan& operator+=(TimeSpan rhs) noexcept;
    TimeSpan& operator-=(TimeSpan rhs) noexcept;
    constexpr TimeSpan operator*(TimeSpan timeSpan) const noexcept;
    friend constexpr TimeSpan operator*(double factor, TimeSpan timeSpan) noexcept;
    constexpr TimeSpan operator/(double divisor) const noexcept;
    constexpr TimeSpan operator-() const noexcept;
    constexpr TimeSpan operator+() const noexcept;

public:
    [[nodiscard]] static constexpr int32_t Compare(TimeSpan lhs, TimeSpan rhs) noexcept;
    [[nodiscard]] constexpr int32_t CompareTo(TimeSpan value) const noexcept;
    [[nodiscard]] constexpr int32_t GetDays() const noexcept;
    [[nodiscard]] constexpr int32_t GetHours() const noexcept;
    [[nodiscard]] constexpr int32_t GetMinutes() const noexcept;
    [[nodiscard]] constexpr int32_t GetSeconds() const noexcept;
    [[nodiscard]] constexpr int32_t GetMilliseconds() const noexcept;
    [[nodiscard]] constexpr int64_t GetTicks() const noexcept;
    [[nodiscard]] constexpr double GetTotalDays() const noexcept;
    [[nodiscard]] constexpr double GetTotalHours() const noexcept;
    [[nodiscard]] constexpr double GetTotalMinutes() const noexcept;
    [[nodiscard]] constexpr double GetTotalSeconds() const noexcept;
    [[nodiscard]] constexpr double GetTotalMilliseconds() const noexcept;
    [[nodiscard]] constexpr TimeSpan Add(TimeSpan timeSpan) const noexcept;
    [[nodiscard]] constexpr TimeSpan Divide(double divisor) const noexcept;
    [[nodiscard]] constexpr double Divide(TimeSpan timeSpan) const noexcept;
    [[nodiscard]] constexpr TimeSpan Multiply(double factor) const noexcept;
    [[nodiscard]] constexpr TimeSpan Negate() const noexcept;
    [[nodiscard]] constexpr TimeSpan Subtract(TimeSpan timeSpan) const noexcept;
    [[nodiscard]] constexpr TimeSpan Duration() const noexcept;
    [[nodiscard]] static constexpr TimeSpan FromDays(double value) noexcept;
    [[nodiscard]] static constexpr TimeSpan FromHours(double value) noexcept;
    [[nodiscard]] static constexpr TimeSpan FromMinutes(double value) noexcept;
    [[nodiscard]] static constexpr TimeSpan FromSeconds(double value) noexcept;
    [[nodiscard]] static constexpr TimeSpan FromMilliseconds(double value) noexcept;
    [[nodiscard]] static constexpr TimeSpan FromTicks(int64_t value) noexcept;
    [[nodiscard]] constexpr size_t GetHashCode() const noexcept;
    [[nodiscard]] static constexpr TimeSpan GetMaxValue() noexcept;
    [[nodiscard]] static constexpr TimeSpan GetMinValue() noexcept;
    [[nodiscard]] static constexpr TimeSpan GetZero() noexcept;
    //string ToString();
    //string ToString(string format);
    //string ToString(string format, IFormatProvider formatProvider);

private:
    static constexpr int64_t TimeToTicks(int32_t hour, int32_t minute, int32_t second) noexcept;

public:
    static constexpr int64_t TicksPerMillisecond = 10000;
    static constexpr int64_t TicksPerSecond = TicksPerMillisecond * 1000;
    static constexpr int64_t TicksPerMinute = TicksPerSecond * 60;
    static constexpr int64_t TicksPerHour = TicksPerMinute * 60;
    static constexpr int64_t TicksPerDay = TicksPerHour * 24;

private:
    static constexpr double DaysPerTick = 1.0 / TicksPerDay;
    static constexpr double HoursPerTick = 1.0 / TicksPerHour;
    static constexpr double MinutesPerTick = 1.0 / TicksPerMinute;
    static constexpr double SecondsPerTick = 1.0 / TicksPerSecond;
    static constexpr double MillisecondsPerTick = 1.0 / TicksPerMillisecond;

    int64_t _ticks = 0;
};

constexpr TimeSpan::TimeSpan(int32_t hours, int32_t minutes, int32_t seconds) noexcept :
    TimeSpan(0, hours, minutes, seconds, 0)
{
}

constexpr TimeSpan::TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds) noexcept :
    TimeSpan(days, hours, minutes, seconds, 0)
{
}

constexpr TimeSpan::TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds) noexcept :
    _ticks((TicksPerDay * days) + (TicksPerHour * hours) + (TicksPerMinute * minutes) + (TicksPerSecond * seconds) + (TicksPerMillisecond * milliseconds))
{
}

constexpr TimeSpan::TimeSpan(int64_t ticks) noexcept :
    _ticks(ticks)
{
}

constexpr bool TimeSpan::operator==(TimeSpan rhs) const noexcept
{
    return _ticks == rhs._ticks;
}

constexpr bool TimeSpan::operator!=(TimeSpan rhs) const noexcept
{
    return _ticks != rhs._ticks;
}

constexpr bool TimeSpan::operator>(TimeSpan rhs) const noexcept
{
    return _ticks > rhs._ticks;
}

constexpr bool TimeSpan::operator>=(TimeSpan rhs) const noexcept
{
    return _ticks >= rhs._ticks;
}

constexpr bool TimeSpan::operator<(TimeSpan rhs) const noexcept
{
    return _ticks < rhs._ticks;
}

constexpr bool TimeSpan::operator<=(TimeSpan rhs) const noexcept
{
    return _ticks <= rhs._ticks;
}

constexpr TimeSpan TimeSpan::operator*(TimeSpan timeSpan) const noexcept
{
    return TimeSpan(_ticks * timeSpan._ticks);
}

constexpr TimeSpan TimeSpan::operator+(TimeSpan rhs) const noexcept
{
    return TimeSpan(_ticks + rhs._ticks);
}

constexpr TimeSpan TimeSpan::operator-(TimeSpan rhs) const noexcept
{
    return TimeSpan(_ticks - rhs._ticks);
}

inline TimeSpan& TimeSpan::operator+=(TimeSpan rhs) noexcept
{
    _ticks += rhs._ticks;
    return *this;
}

inline TimeSpan& TimeSpan::operator-=(TimeSpan rhs) noexcept
{
    _ticks -= rhs._ticks;
    return *this;
}

constexpr TimeSpan TimeSpan::operator/(double divisor) const noexcept
{
    return TimeSpan(static_cast<int64_t>(static_cast<double>(_ticks) / divisor));
}

constexpr TimeSpan operator*(double factor, TimeSpan timeSpan) noexcept
{
    return TimeSpan(static_cast<int64_t>(factor * static_cast<double>(timeSpan._ticks)));
}

constexpr TimeSpan TimeSpan::operator-() const noexcept
{
    return TimeSpan(-_ticks);
}

constexpr TimeSpan TimeSpan::operator+() const noexcept
{
    return TimeSpan(_ticks);
}

constexpr int32_t TimeSpan::Compare(TimeSpan lhs, TimeSpan rhs) noexcept
{
    return lhs.CompareTo(rhs);
}

constexpr int32_t TimeSpan::CompareTo(TimeSpan value) const noexcept
{
    if (_ticks > value._ticks)
    {
        return 1;
    }
    if (_ticks < value._ticks)
    {
        return -1;
    }

    return 0;
}

constexpr int32_t TimeSpan::GetDays() const noexcept
{
    return static_cast<int32_t>(_ticks / TicksPerDay);
}

constexpr int32_t TimeSpan::GetHours() const noexcept
{
    return (_ticks / TicksPerHour) % 24;
}

constexpr int32_t TimeSpan::GetMinutes() const noexcept
{
    return (_ticks / TicksPerMinute) % 60;
}

constexpr int32_t TimeSpan::GetSeconds() const noexcept
{
    return (_ticks / TicksPerSecond) % 60;
}

constexpr int32_t TimeSpan::GetMilliseconds() const noexcept
{
    return static_cast<int32_t>((_ticks / TicksPerMillisecond) % 1000);
}

constexpr int64_t TimeSpan::GetTicks() const noexcept
{
    return _ticks;
}

constexpr double TimeSpan::GetTotalDays() const noexcept
{
    return static_cast<double>(_ticks) * DaysPerTick;
}

constexpr double TimeSpan::GetTotalHours() const noexcept
{
    return static_cast<double>(_ticks) * HoursPerTick;
}

constexpr double TimeSpan::GetTotalMinutes() const noexcept
{
    return static_cast<double>(_ticks) * MinutesPerTick;
}

constexpr double TimeSpan::GetTotalSeconds() const noexcept
{
    return static_cast<double>(_ticks) * SecondsPerTick;
}

constexpr double TimeSpan::GetTotalMilliseconds() const noexcept
{
    return static_cast<double>(_ticks) * MillisecondsPerTick;
}

constexpr TimeSpan TimeSpan::Add(TimeSpan timeSpan) const noexcept
{
    return TimeSpan(_ticks + timeSpan._ticks);
}

constexpr TimeSpan TimeSpan::Divide(double divisor) const noexcept
{
    return TimeSpan(static_cast<int64_t>(static_cast<double>(_ticks) / divisor));
}

constexpr double TimeSpan::Divide(TimeSpan timeSpan) const noexcept
{
    return static_cast<double>(_ticks) / timeSpan._ticks;
}

constexpr TimeSpan TimeSpan::Multiply(double factor) const noexcept
{
    return TimeSpan(static_cast<int64_t>(static_cast<double>(_ticks) * factor));
}

constexpr TimeSpan TimeSpan::Negate() const noexcept
{
    return TimeSpan(-_ticks);
}

constexpr TimeSpan TimeSpan::Subtract(TimeSpan timeSpan) const noexcept
{
    return TimeSpan(_ticks - timeSpan._ticks);
}

constexpr TimeSpan TimeSpan::Duration() const noexcept
{
    return TimeSpan(_ticks);
}

constexpr TimeSpan TimeSpan::FromDays(double value) noexcept
{
    return TimeSpan(static_cast<int64_t>(value * static_cast<double>(TimeSpan::TicksPerDay)));
}

constexpr TimeSpan TimeSpan::FromHours(double value) noexcept
{
    return TimeSpan(static_cast<int64_t>(value * static_cast<double>(TimeSpan::TicksPerHour)));
}

constexpr TimeSpan TimeSpan::FromMinutes(double value) noexcept
{
    return TimeSpan(static_cast<int64_t>(value * static_cast<double>(TimeSpan::TicksPerMinute)));
}

constexpr TimeSpan TimeSpan::FromSeconds(double value) noexcept
{
    return TimeSpan(static_cast<int64_t>(value * static_cast<double>(TimeSpan::TicksPerSecond)));
}

constexpr TimeSpan TimeSpan::FromMilliseconds(double value) noexcept
{
    return TimeSpan(static_cast<int64_t>(value * static_cast<double>(TimeSpan::TicksPerMillisecond)));
}

constexpr TimeSpan TimeSpan::FromTicks(int64_t value) noexcept
{
    return TimeSpan(value);
}

constexpr int64_t TimeSpan::TimeToTicks(int32_t hour, int32_t minute, int32_t second) noexcept
{
    const auto totalSeconds = (static_cast<int64_t>(hour) * 3600) + (static_cast<int64_t>(minute) * 60) * (static_cast<int64_t>(second));
    return totalSeconds * TicksPerSecond;
}

constexpr size_t TimeSpan::GetHashCode() const noexcept
{
    return static_cast<size_t>(_ticks ^ (_ticks >> 32));
}

constexpr TimeSpan TimeSpan::GetMaxValue() noexcept
{
    return TimeSpan(std::numeric_limits<decltype(_ticks)>::max());
}

constexpr TimeSpan TimeSpan::GetMinValue() noexcept
{
    return TimeSpan(std::numeric_limits<decltype(_ticks)>::min());
}

constexpr TimeSpan TimeSpan::GetZero() noexcept
{
    return TimeSpan(0);
}

CS2CPP_NAMESPACE_END
