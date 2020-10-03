#include <unicode/datefmt.h>
#include <unicode/smpdtfmt.h>

#include "System/TimeSpan.h"

CS2CPP_NAMESPACE_BEGIN

namespace
{

std::u16string Format(int64_t ticks, std::u16string_view format)
{
    UErrorCode status = U_ZERO_ERROR;

    auto formatter = icu::SimpleDateFormat(icu::UnicodeString(format.data(), static_cast<int32_t>(format.length())), icu::Locale::getDefault(), status);
    if (U_FAILURE(status))
    {
        return {};
    }

    icu::UnicodeString formatted;
    formatter.setTimeZone(*icu::TimeZone::getGMT());
    formatter.format(static_cast<UDate>(ticks) / TimeSpan::TicksPerMillisecond, formatted);

    return std::u16string(formatted.getBuffer(), static_cast<size_t>(formatted.length()));
}

}

std::u16string TimeSpan::ToString() const
{
    return Format(Ticks(), u"hh:mm:ss");
}

std::u16string TimeSpan::ToString(std::u16string_view format) const
{
    return Format(Ticks(), format);
}

CS2CPP_NAMESPACE_END