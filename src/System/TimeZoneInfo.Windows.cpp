#include "System/TimeZoneInfo.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

TimeZoneInfo TimeZoneInfo::CreateLocal()
{
    DYNAMIC_TIME_ZONE_INFORMATION tzi;
    const auto isSupportDaylightSavingTime = GetDynamicTimeZoneInformation(&tzi) != TIME_ZONE_ID_UNKNOWN;

    const TimeSpan baseUtcOffset(TimeSpan::TicksPerMinute * -tzi.Bias);

    const auto id = reinterpret_cast<const char16_t*>(tzi.TimeZoneKeyName);
    const auto standardName = reinterpret_cast<const char16_t*>(tzi.StandardName);
    const auto daylightDisplayName = reinterpret_cast<const char16_t*>(tzi.DaylightName);

    return TimeZoneInfo(id, baseUtcOffset, standardName, standardName, daylightDisplayName, isSupportDaylightSavingTime);
}

CS2CPP_NAMESPACE_END
