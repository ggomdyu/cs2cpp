#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

TimeZoneInfo::TimeZoneInfo(String id, TimeSpan baseUtcOffset, String displayName, String standardDisplayName, String daylightDisplayName, bool supportsDaylightSavingTime) noexcept :
    _id(std::move(id)),
    _baseUtcOffset(baseUtcOffset),
    _displayName(std::move(displayName)),
    _standardDisplayName(std::move(standardDisplayName)),
    _daylightDisplayName(std::move(daylightDisplayName)),
    _supportsDaylightSavingTime(supportsDaylightSavingTime)
{
}

const TimeZoneInfo& TimeZoneInfo::Utc() noexcept
{
    static auto timeZoneInfo = []() {
        const String id = u"UTC";
        return TimeZoneInfo(id, TimeSpan(0), id, id, id, false);
    }();
    return timeZoneInfo;
}

DateTime TimeZoneInfo::ConvertTime(DateTime dateTime, const TimeZoneInfo& destinationTimeZone)
{
    if (dateTime.GetKind() == DateTimeKind::Utc)
    {
        return ConvertTime(dateTime, Utc(), destinationTimeZone);
    }

    return ConvertTime(dateTime, Local(), destinationTimeZone);
}

DateTime TimeZoneInfo::ConvertTimeFromUtc(DateTime dateTime, const TimeZoneInfo& destinationTimeZone)
{
    return ConvertTime(dateTime, Utc(), destinationTimeZone);
}

DateTime TimeZoneInfo::ConvertTimeToUtc(DateTime dateTime)
{
    return ConvertTime(dateTime, Local(), TimeZoneInfo::Utc());
}

const String& TimeZoneInfo::GetId() const noexcept
{
    return _id;
}

TimeSpan TimeZoneInfo::GetBaseUtcOffset() const noexcept
{
    return _baseUtcOffset;
}

const String& TimeZoneInfo::GetStandardDisplayName() const noexcept
{
    return _standardDisplayName;
}

const String& TimeZoneInfo::GetDaylightDisplayName() const noexcept
{
    return _daylightDisplayName;
}

bool TimeZoneInfo::IsSupportDaylightSavingTime() const noexcept
{
    return _supportsDaylightSavingTime;
}

DateTimeKind TimeZoneInfo::GetCorrespondingKind(const TimeZoneInfo& timeZone)
{
    if (&timeZone == &Utc())
    {
        return DateTimeKind::Utc;
    }

    if (&timeZone == &Local())
    {
        return DateTimeKind::Local;
    }

    return DateTimeKind::Unspecified;
}

DateTime TimeZoneInfo::ConvertTime(DateTime dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone)
{
    // The kind of dateTime and sourceTimeZone must be the same.
    const auto sourceKind = GetCorrespondingKind(sourceTimeZone);
    if (dateTime.GetKind() != sourceKind)
    {
        return dateTime;
    }

    // Filter the special case like UTC->UTC or Local->Local
    const auto destinationKind = GetCorrespondingKind(destinationTimeZone);
    if (sourceKind != DateTimeKind::Unspecified && destinationKind != DateTimeKind::Unspecified && sourceKind == destinationKind)
    {
        return dateTime;
    }

    // Convert the dateTime utc offset to 0.
    const auto ticks = dateTime.GetTicks() - sourceTimeZone.GetBaseUtcOffset().GetTicks();
    if (destinationKind == DateTimeKind::Local)
    {
        return DateTime(ticks + destinationTimeZone.GetBaseUtcOffset().GetTicks(), DateTimeKind::Local);
    }

    return DateTime(ticks, destinationKind);
}

const TimeZoneInfo& TimeZoneInfo::Local()
{
    static auto timeZoneInfo = CreateLocal();
    return timeZoneInfo;
}

CS2CPP_NAMESPACE_END