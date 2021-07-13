#include <unicode/datefmt.h>
#include <unicode/smpdtfmt.h>

#include "System/DateTime.h"
#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

class DateTimeFormatter final
{
public:
    [[nodiscard]] static std::u16string Format(int64_t ticks);
    [[nodiscard]] static std::u16string Format(int64_t ticks, std::u16string_view format);

private:
    [[nodiscard]] static std::u16string Format(int64_t ticks, icu::DateFormat& formatter);
};

std::u16string DateTimeFormatter::Format(int64_t ticks)
{
    std::unique_ptr<icu::DateFormat> formatter(icu::DateFormat::createDateTimeInstance());
    return Format(ticks, *formatter);
}

std::u16string DateTimeFormatter::Format(int64_t ticks, std::u16string_view format)
{
    UErrorCode status = U_ZERO_ERROR;

    auto formatter = icu::SimpleDateFormat(icu::UnicodeString(format.data(), static_cast<int32_t>(format.length())),
        icu::Locale::getDefault(), status);
    if (U_FAILURE(status))
        return {};

    return Format(ticks, formatter);
}

std::u16string DateTimeFormatter::Format(int64_t ticks, icu::DateFormat& formatter)
{
    icu::UnicodeString formatted;

    formatter.setTimeZone(*icu::TimeZone::getGMT());
    formatter.format(static_cast<UDate>(ticks - DateTime::UnixEpoch().Ticks()) /
        TimeSpan::TicksPerMillisecond, formatted);

    return {formatted.getBuffer(), static_cast<size_t>(formatted.length())};
}

DateTime DateTime::ToLocalTime() const
{
    return TimeZoneInfo::ConvertTimeFromUtc(*this, TimeZoneInfo::Local());
}

DateTime DateTime::ToUniversalTime() const
{
    return TimeZoneInfo::ConvertTimeToUtc(*this);
}

std::u16string DateTime::ToString() const
{
    return DateTimeFormatter::Format(Ticks());
}

std::u16string DateTime::ToString(std::u16string_view format) const
{
    return DateTimeFormatter::Format(Ticks(), format);
}

CS2CPP_NAMESPACE_END