#pragma once

#include <cmath>
#include <cstdint>
#include <string>

CS2CPP_NAMESPACE_BEGIN

struct TimeSpan final
{
public:
    constexpr TimeSpan(int32_t hours, int32_t minutes, int32_t seconds) noexcept;
    constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds) noexcept;
    constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds) noexcept;
    constexpr explicit TimeSpan(int64_t ticks) noexcept;

public:
    constexpr TimeSpan operator+(TimeSpan value) const noexcept;
    constexpr TimeSpan operator-(TimeSpan value) const noexcept;
    constexpr TimeSpan operator*(double factor) const noexcept;
    friend constexpr TimeSpan operator*(double lhs, TimeSpan rhs) noexcept;
    constexpr double operator/(TimeSpan value) const noexcept;
    constexpr TimeSpan operator/(double divisor) const noexcept;
    constexpr TimeSpan operator+() const noexcept;
    constexpr TimeSpan operator-() const noexcept;
    TimeSpan& operator+=(TimeSpan value) noexcept;
    TimeSpan& operator-=(TimeSpan value) noexcept;
    TimeSpan& operator*=(double factor) noexcept;
    TimeSpan& operator/=(double divisor) noexcept;
    constexpr bool operator==(TimeSpan value) const noexcept;
    constexpr bool operator!=(TimeSpan value) const noexcept;
    constexpr bool operator>(TimeSpan value) const noexcept;
    constexpr bool operator>=(TimeSpan value) const noexcept;
    constexpr bool operator<(TimeSpan value) const noexcept;
    constexpr bool operator<=(TimeSpan value) const noexcept;

public:
    static constexpr int32_t Compare(TimeSpan lhs, TimeSpan rhs) noexcept;
    constexpr int32_t CompareTo(TimeSpan value) const noexcept;
    static constexpr bool Equals(TimeSpan lhs, TimeSpan rhs) noexcept;
    constexpr bool Equals(TimeSpan value) const noexcept;
    constexpr int32_t Days() const noexcept;
    constexpr int32_t Hours() const noexcept;
    constexpr int32_t Minutes() const noexcept;
    constexpr int32_t Seconds() const noexcept;
    constexpr int32_t Milliseconds() const noexcept;
    constexpr int64_t Ticks() const noexcept;
    constexpr double TotalDays() const noexcept;
    constexpr double TotalHours() const noexcept;
    constexpr double TotalMinutes() const noexcept;
    constexpr double TotalSeconds() const noexcept;
    constexpr double TotalMilliseconds() const noexcept;
    constexpr TimeSpan Add(TimeSpan value) const noexcept;
    constexpr TimeSpan Divide(double divisor) const noexcept;
    constexpr double Divide(TimeSpan value) const noexcept;
    constexpr TimeSpan Multiply(double factor) const noexcept;
    constexpr TimeSpan Negate() const noexcept;
    constexpr TimeSpan Subtract(TimeSpan value) const noexcept;
    constexpr TimeSpan Duration() const noexcept;
    static constexpr TimeSpan FromDays(double value) noexcept;
    static constexpr TimeSpan FromHours(double value) noexcept;
    static constexpr TimeSpan FromMinutes(double value) noexcept;
    static constexpr TimeSpan FromSeconds(double value) noexcept;
    static constexpr TimeSpan FromMilliseconds(double value) noexcept;
    static constexpr TimeSpan FromTicks(int64_t value) noexcept;
    constexpr int32_t GetHashCode() const noexcept;
    static constexpr TimeSpan MaxValue() noexcept;
    static constexpr TimeSpan MinValue() noexcept;
    static constexpr TimeSpan Zero() noexcept;
    std::u16string ToString() const;
    std::u16string ToString(std::u16string_view format) const;

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

    int64_t ticks_ = 0;
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
    ticks_((TicksPerDay * days) + (TicksPerHour * hours) + (TicksPerMinute * minutes) + (TicksPerSecond * seconds) + (TicksPerMillisecond * milliseconds))
{
}

constexpr TimeSpan::TimeSpan(int64_t ticks) noexcept :
    ticks_(ticks)
{
}

constexpr TimeSpan TimeSpan::operator+(TimeSpan value) const noexcept
{
    return TimeSpan(ticks_ + value.ticks_);
}

constexpr TimeSpan TimeSpan::operator-(TimeSpan value) const noexcept
{
    return TimeSpan(ticks_ - value.ticks_);
}

constexpr TimeSpan TimeSpan::operator*(double factor) const noexcept
{
    auto ticks = static_cast<double>(ticks_) * factor;
    return TimeSpan(static_cast<int64_t>(ticks));
}

constexpr double TimeSpan::operator/(TimeSpan value) const noexcept
{
    return static_cast<double>(ticks_) / static_cast<double>(value.ticks_);
}

constexpr TimeSpan TimeSpan::operator/(double divisor) const noexcept
{
    return TimeSpan(static_cast<int64_t>(static_cast<double>(ticks_) / divisor));
}

constexpr TimeSpan operator*(double lhs, TimeSpan rhs) noexcept
{
    return TimeSpan(static_cast<int64_t>(lhs * static_cast<double>(rhs.ticks_)));
}

constexpr TimeSpan TimeSpan::operator+() const noexcept
{
    return TimeSpan(ticks_);
}

constexpr TimeSpan TimeSpan::operator-() const noexcept
{
    return TimeSpan(-ticks_);
}

inline TimeSpan& TimeSpan::operator+=(TimeSpan value) noexcept
{
    ticks_ += value.ticks_;
    return *this;
}

inline TimeSpan& TimeSpan::operator-=(TimeSpan value) noexcept
{
    ticks_ -= value.ticks_;
    return *this;
}

inline TimeSpan& TimeSpan::operator*=(double factor) noexcept
{
    auto ticks = static_cast<double>(ticks_) * factor;
    ticks_ = static_cast<int64_t>(ticks);

    return *this;
}

inline TimeSpan& TimeSpan::operator/=(double divisor) noexcept
{
    auto ticks = static_cast<double>(ticks_) / divisor;
    ticks_ = static_cast<int64_t>(ticks);

    return *this;
}

constexpr bool TimeSpan::operator==(TimeSpan value) const noexcept
{
    return ticks_ == value.ticks_;
}

constexpr bool TimeSpan::operator!=(TimeSpan value) const noexcept
{
    return ticks_ != value.ticks_;
}

constexpr bool TimeSpan::operator>(TimeSpan value) const noexcept
{
    return ticks_ > value.ticks_;
}

constexpr bool TimeSpan::operator>=(TimeSpan value) const noexcept
{
    return ticks_ >= value.ticks_;
}

constexpr bool TimeSpan::operator<(TimeSpan value) const noexcept
{
    return ticks_ < value.ticks_;
}

constexpr bool TimeSpan::operator<=(TimeSpan value) const noexcept
{
    return ticks_ <= value.ticks_;
}

constexpr int32_t TimeSpan::Compare(TimeSpan lhs, TimeSpan rhs) noexcept
{
    return lhs.CompareTo(rhs);
}

constexpr int32_t TimeSpan::CompareTo(TimeSpan value) const noexcept
{
    if (ticks_ > value.ticks_)
    {
        return 1;
    }

    if (ticks_ < value.ticks_)
    {
        return -1;
    }

    return 0;
}

constexpr bool TimeSpan::Equals(TimeSpan lhs, TimeSpan rhs) noexcept
{
    return lhs.Equals(rhs);
}

constexpr bool TimeSpan::Equals(TimeSpan value) const noexcept
{
    return ticks_ == value.ticks_;
}

constexpr int32_t TimeSpan::Days() const noexcept
{
    return static_cast<int32_t>(ticks_ / TicksPerDay);
}

constexpr int32_t TimeSpan::Hours() const noexcept
{
    return static_cast<int32_t>((ticks_ / TicksPerHour) % 24);
}

constexpr int32_t TimeSpan::Minutes() const noexcept
{
    return static_cast<int32_t>((ticks_ / TicksPerMinute) % 60);
}

constexpr int32_t TimeSpan::Seconds() const noexcept
{
    return static_cast<int32_t>((ticks_ / TicksPerSecond) % 60);
}

constexpr int32_t TimeSpan::Milliseconds() const noexcept
{
    return static_cast<int32_t>((ticks_ / TicksPerMillisecond) % 1000);
}

constexpr int64_t TimeSpan::Ticks() const noexcept
{
    return ticks_;
}

constexpr double TimeSpan::TotalDays() const noexcept
{
    return static_cast<double>(ticks_) * DaysPerTick;
}

constexpr double TimeSpan::TotalHours() const noexcept
{
    return static_cast<double>(ticks_) * HoursPerTick;
}

constexpr double TimeSpan::TotalMinutes() const noexcept
{
    return static_cast<double>(ticks_) * MinutesPerTick;
}

constexpr double TimeSpan::TotalSeconds() const noexcept
{
    return static_cast<double>(ticks_) * SecondsPerTick;
}

constexpr double TimeSpan::TotalMilliseconds() const noexcept
{
    return static_cast<double>(ticks_) * MillisecondsPerTick;
}

constexpr TimeSpan TimeSpan::Add(TimeSpan value) const noexcept
{
    return TimeSpan(ticks_ + value.ticks_);
}

constexpr TimeSpan TimeSpan::Divide(double divisor) const noexcept
{
    return TimeSpan(static_cast<int64_t>(static_cast<double>(ticks_) / divisor));
}

constexpr double TimeSpan::Divide(TimeSpan value) const noexcept
{
    return static_cast<double>(ticks_) / static_cast<double>(value.ticks_);
}

constexpr TimeSpan TimeSpan::Multiply(double factor) const noexcept
{
    return TimeSpan(static_cast<int64_t>(static_cast<double>(ticks_) * factor));
}

constexpr TimeSpan TimeSpan::Negate() const noexcept
{
    return TimeSpan(-ticks_);
}

constexpr TimeSpan TimeSpan::Subtract(TimeSpan value) const noexcept
{
    return TimeSpan(ticks_ - value.ticks_);
}

constexpr TimeSpan TimeSpan::Duration() const noexcept
{
    return TimeSpan(ticks_);
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
    auto totalSeconds = (static_cast<int64_t>(hour) * 3600) + (static_cast<int64_t>(minute) * 60) *
        (static_cast<int64_t>(second));
    return totalSeconds * TicksPerSecond;
}

constexpr int32_t TimeSpan::GetHashCode() const noexcept
{
    return static_cast<int32_t>(ticks_ ^ (ticks_ >> 32));
}

constexpr TimeSpan TimeSpan::MaxValue() noexcept
{
    return TimeSpan(std::numeric_limits<decltype(ticks_)>::max());
}

constexpr TimeSpan TimeSpan::MinValue() noexcept
{
    return TimeSpan(std::numeric_limits<decltype(ticks_)>::min());
}

constexpr TimeSpan TimeSpan::Zero() noexcept
{
    return TimeSpan(0);
}

CS2CPP_NAMESPACE_END
