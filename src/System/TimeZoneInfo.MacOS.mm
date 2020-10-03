#include <Foundation/Foundation.h>

#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

[[nodiscard]] TimeZoneInfo TimeZoneInfo::CreateLocal()
{
    auto toUtf16 = [](NSString* str) {
        return reinterpret_cast<const char16_t*>([str cStringUsingEncoding:NSUTF16StringEncoding]);
    };

    NSTimeZone* localTimeZone = [NSTimeZone localTimeZone];
    auto id = toUtf16([localTimeZone name]);
    auto supportsDaylightSavingTime = [localTimeZone isDaylightSavingTime];
    auto baseUtcOffset = TimeSpan(TimeSpan::TicksPerSecond * [localTimeZone secondsFromGMT]);

    NSLocale* currentLocale = [NSLocale currentLocale];
    auto standardName = toUtf16([localTimeZone localizedName:NSTimeZoneNameStyleStandard locale:currentLocale]);
    auto daylightDisplayName = toUtf16([localTimeZone localizedName:NSTimeZoneNameStyleDaylightSaving locale:currentLocale]);

    return TimeZoneInfo(id, baseUtcOffset, standardName, standardName, daylightDisplayName, supportsDaylightSavingTime);
}

CS2CPP_NAMESPACE_END