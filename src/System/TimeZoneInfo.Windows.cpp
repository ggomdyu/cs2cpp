#include "System/TimeZoneInfo.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

TimeZoneInfo TimeZoneInfo::CreateLocal()
{
    DYNAMIC_TIME_ZONE_INFORMATION tzi;

    auto isSupportDaylightSavingTime = GetDynamicTimeZoneInformation(&tzi) != TIME_ZONE_ID_UNKNOWN;
    auto baseUtcOffset = TimeSpan(TimeSpan::TicksPerMinute * -tzi.Bias);

    auto id = String(reinterpret_cast<const char16_t*>(tzi.TimeZoneKeyName))
    auto standardName = String(reinterpret_cast<const char16_t*>(tzi.StandardName));
    auto daylightDisplayName = String(reinterpret_cast<const char16_t*>(tzi.DaylightName);

    return TimeZoneInfo(std::move(id), baseUtcOffset, standardName, standardName, std::move(daylightDisplayName),
        isSupportDaylightSavingTime);
}

CS2CPP_NAMESPACE_END
