#include <unicode/datefmt.h>
#include <unicode/smpdtfmt.h>

#include "System/DateTime.h"
#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

static std::u16string Format(int64_t ticks, icu::DateFormat& formatter)
{
    icu::UnicodeString formatted;
    formatter.setTimeZone(*icu::TimeZone::getGMT());
    formatter.format(static_cast<UDate>(ticks - DateTime::UnixEpoch().Ticks()) / TimeSpan::TicksPerMillisecond, formatted);

    return {formatted.getBuffer(), static_cast<size_t>(formatted.length())};
}

DateTime& DateTime::operator+=(TimeSpan rhs) noexcept
{
    ticks_ += rhs.Ticks();
    return *this;
}

DateTime& DateTime::operator-=(TimeSpan rhs) noexcept
{
    ticks_ -= rhs.Ticks();
    return *this;
}

DateTime DateTime::FromFileTime(int64_t fileTime) noexcept
{
    return FromFileTimeUtc(fileTime).ToLocalTime();
}

DateTime DateTime::FromFileTimeUtc(int64_t fileTime) noexcept
{
    return DateTime(fileTime + FileTimeOffset, DateTimeKind::Utc);
}

DateTime DateTime::Now()
{
    auto utcOffset = TimeZoneInfo::Local().BaseUtcOffset().Ticks();
    return DateTime(UnixEpoch().Ticks() + GetTimeSinceUnixEpoch() + utcOffset, DateTimeKind::Local);
}

DateTime DateTime::UtcNow()
{
    return DateTime(UnixEpoch().Ticks() + GetTimeSinceUnixEpoch(), DateTimeKind::Utc);
}

DateTime DateTime::Today()
{
    return Now().Date();
}

int64_t DateTime::ToFileTime() const
{
    return ToUniversalTime().ToFileTimeUtc();
}

int64_t DateTime::ToFileTimeUtc() const
{
    return (Kind() == DateTimeKind::Local ? ToUniversalTime().Ticks() : Ticks()) - FileTimeOffset;
}

DateTime DateTime::ToLocalTime() const
{
    if (Kind() == DateTimeKind::Local)
    {
        return *this;
    }

    return TimeZoneInfo::ConvertTimeFromUtc(*this, TimeZoneInfo::Local());
}

DateTime DateTime::ToUniversalTime() const
{
    return TimeZoneInfo::ConvertTimeToUtc(*this);
}

std::u16string DateTime::ToLongDateString() const
{
    auto formatter = std::unique_ptr<icu::DateFormat>(icu::DateFormat::createDateInstance(icu::DateFormat::kFull));
    return Format(Ticks(), *formatter);
}

std::u16string DateTime::ToLongTimeString() const
{
    auto formatter = std::unique_ptr<icu::DateFormat>(icu::DateFormat::createTimeInstance());
    return Format(Ticks(), *formatter);
}

std::u16string DateTime::ToShortDateString() const
{
    auto formatter = std::unique_ptr<icu::DateFormat>(icu::DateFormat::createDateInstance(icu::DateFormat::kMedium));
    return Format(Ticks(), *formatter);
}

std::u16string DateTime::ToShortTimeString() const
{
    using namespace icu;

    auto formatter = std::unique_ptr<icu::DateFormat>(icu::DateFormat::createTimeInstance(icu::DateFormat::kShort));
    return Format(Ticks(), *formatter);
}

std::u16string DateTime::ToString() const
{
    auto formatter = std::unique_ptr<icu::DateFormat>(icu::DateFormat::createDateTimeInstance());
    return Format(Ticks(), *formatter);
}

std::u16string DateTime::ToString(std::u16string_view format) const
{
    UErrorCode status = U_ZERO_ERROR;

    auto formatter = icu::SimpleDateFormat(icu::UnicodeString(format.data(), static_cast<int32_t>(format.length())),
        icu::Locale::getDefault(), status);
    if (U_FAILURE(status))
    {
        return {};
    }

    return Format(Ticks(), formatter);
}

int64_t DateTime::GetTimeSinceUnixEpoch()
{
    using namespace std::chrono;

    auto timeSinceEpoch = system_clock::now().time_since_epoch();
    if constexpr (decltype(timeSinceEpoch)::period::den == 10'000'000)
    {
        return timeSinceEpoch.count();
    }
    else
    {
        return duration_cast<duration<int64_t, std::ratio<1, 10000000>>>(timeSinceEpoch).count();
    }
}

CS2CPP_NAMESPACE_END