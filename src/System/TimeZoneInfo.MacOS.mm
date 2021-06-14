#include <Foundation/Foundation.h>

#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

namespace
{

auto ConvertToUTF16String = [](NSString* str) {
    return std::u16string(reinterpret_cast<const char16_t*>([str cStringUsingEncoding:NSUTF16StringEncoding]));
};

}

[[nodiscard]] TimeZoneInfo TimeZoneInfo::CreateLocal()
{
    NSTimeZone* localTimeZone = [NSTimeZone localTimeZone];
    auto id = ConvertToUTF16String([localTimeZone name]);
    auto supportsDaylightSavingTime = [localTimeZone isDaylightSavingTime];
    auto baseUtcOffset = TimeSpan(TimeSpan::TicksPerSecond * [localTimeZone secondsFromGMT]);

    NSLocale* currentLocale = [NSLocale currentLocale];
    auto standardName = ConvertToUTF16String([localTimeZone localizedName:NSTimeZoneNameStyleStandard locale:currentLocale]);
    auto daylightDisplayName = ConvertToUTF16String([localTimeZone localizedName:NSTimeZoneNameStyleDaylightSaving locale:currentLocale]);

    return TimeZoneInfo(id, baseUtcOffset, standardName, standardName, daylightDisplayName, supportsDaylightSavingTime);
}

CS2CPP_NAMESPACE_END
