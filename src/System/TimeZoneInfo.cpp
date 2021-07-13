#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

TimeZoneInfo::TimeZoneInfo(std::u16string id, TimeSpan baseUtcOffset, std::u16string displayName, std::u16string standardDisplayName, std::u16string daylightDisplayName, bool supportsDaylightSavingTime) noexcept :
    _id(std::move(id)),
    _baseUtcOffset(baseUtcOffset),
    _displayName(std::move(displayName)),
    _standardDisplayName(std::move(standardDisplayName)),
    _daylightDisplayName(std::move(daylightDisplayName)),
    _supportsDaylightSavingTime(supportsDaylightSavingTime)
{
}

const TimeZoneInfo& TimeZoneInfo::Utc()
{
    static auto timeZoneInfo = []()
    {
        std::u16string id(u"UTC");
        return TimeZoneInfo(id, TimeSpan(0), id, id, id, false);
    }();
    return timeZoneInfo;
}

DateTime TimeZoneInfo::ConvertTime(DateTime dateTime, const TimeZoneInfo& destinationTimeZone)
{
    if (dateTime.Kind() == DateTimeKind::Utc)
        return ConvertTime(dateTime, Utc(), destinationTimeZone);

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

std::u16string_view TimeZoneInfo::GetId() const noexcept
{
    return _id;
}

TimeSpan TimeZoneInfo::GetBaseUtcOffset() const noexcept
{
    return _baseUtcOffset;
}

std::u16string_view TimeZoneInfo::GetStandardDisplayName() const noexcept
{
    return _standardDisplayName;
}

std::u16string_view TimeZoneInfo::GetDaylightDisplayName() const noexcept
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
        return DateTimeKind::Utc;

    if (&timeZone == &Local())
        return DateTimeKind::Local;

    return DateTimeKind::Unspecified;
}

DateTime TimeZoneInfo::ConvertTime(DateTime dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone)
{
    // The kind of dateTime and sourceTimeZone must be the same.
    auto sourceKind = GetCorrespondingKind(sourceTimeZone);
    if (dateTime.Kind() != sourceKind)
        return dateTime;

    // Filter the special case like UTC->UTC or Local->Local
    auto destinationKind = GetCorrespondingKind(destinationTimeZone);
    if (sourceKind != DateTimeKind::Unspecified && destinationKind != DateTimeKind::Unspecified && sourceKind == destinationKind)
        return dateTime;

    // Convert the dateTime utc offset to 0.
    auto ticks = dateTime.Ticks() - sourceTimeZone.GetBaseUtcOffset().Ticks();
    if (destinationKind == DateTimeKind::Local)
        return DateTime(ticks + destinationTimeZone.GetBaseUtcOffset().Ticks(), DateTimeKind::Local);

    return DateTime(ticks, destinationKind);
}

const TimeZoneInfo& TimeZoneInfo::Local()
{
    static auto timeZoneInfo = CreateLocal();
    return timeZoneInfo;
}

CS2CPP_NAMESPACE_END
