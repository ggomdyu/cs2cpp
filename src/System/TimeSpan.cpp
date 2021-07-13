#include <unicode/datefmt.h>
#include <unicode/smpdtfmt.h>

#include "System/TimeSpan.h"

CS2CPP_NAMESPACE_BEGIN

class TimeSpanFormatter final
{
public:
    [[nodiscard]] static std::u16string Format(int64_t ticks);
    [[nodiscard]] static std::u16string Format(int64_t ticks, std::u16string_view format);

private:
    [[nodiscard]] static std::u16string Format(int64_t ticks, icu::DateFormat& formatter);
};

std::u16string TimeSpanFormatter::Format(int64_t ticks)
{
    std::unique_ptr<icu::DateFormat> formatter(icu::DateFormat::createTimeInstance(icu::DateFormat::kDefault,
        icu::Locale::getRoot()));
    return Format(ticks, *formatter);
}

std::u16string TimeSpanFormatter::Format(int64_t ticks, std::u16string_view format)
{
    UErrorCode status = U_ZERO_ERROR;

    auto formatter = icu::SimpleDateFormat(icu::UnicodeString(format.data(), static_cast<int32_t>(format.length())),
        icu::Locale::getDefault(), status);
    if (U_FAILURE(status))
        return {};

    return Format(ticks, formatter);
}

std::u16string TimeSpanFormatter::Format(int64_t ticks, icu::DateFormat& formatter)
{
    icu::UnicodeString formatted;

    formatter.setTimeZone(*icu::TimeZone::getGMT());
    formatter.format(static_cast<UDate>(ticks) / TimeSpan::TicksPerMillisecond, formatted);

    return {formatted.getBuffer(), static_cast<size_t>(formatted.length())};
}

std::u16string TimeSpan::ToString() const
{
    return TimeSpanFormatter::Format(Ticks());
}

std::u16string TimeSpan::ToString(std::u16string_view format) const
{
    return TimeSpanFormatter::Format(Ticks(), format);
}

CS2CPP_NAMESPACE_END