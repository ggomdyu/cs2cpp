#include "System/DateTime.h"
#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

DateTime DateTime::ToLocalTime() const
{
    return TimeZoneInfo::ConvertTimeFromUtc(*this, TimeZoneInfo::Local());
}

DateTime DateTime::ToUniversalTime() const
{
    return TimeZoneInfo::ConvertTimeToUtc(*this);
}

CS2CPP_NAMESPACE_END