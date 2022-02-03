#include <sstream>
#include <unicode/locid.h>

#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

static std::vector<TimeZoneInfo> CreateTimeZoneInfos()
{
    std::vector<TimeZoneInfo> ret;
    std::unique_ptr<icu::StringEnumeration, decltype(&free)> timeZoneIds(icu::TimeZone::createEnumeration(), free);

    UErrorCode status = U_ZERO_ERROR;
    while (true)
    {
        const icu::UnicodeString* timeZoneId = timeZoneIds->snext(status);
        if (!timeZoneId || status != U_ZERO_ERROR)
        {
            break;
        }

        ret.emplace_back(icu::TimeZone::createTimeZone(*timeZoneId));
    }

    return ret;
}

TimeZoneInfo::TimeZoneInfo(icu::TimeZone* timeZone) noexcept :
    timeZone_(timeZone),
    id_(CreateId(timeZone)),
    baseUtcOffset_(static_cast<int64_t>(timeZone->getRawOffset()) * 10000),
    displayName_(CreateDisplayName(timeZone)),
    standardDisplayName_(CreateStandardName(timeZone)),
    daylightDisplayName_(CreateDaylightName(timeZone))
{
}

const TimeZoneInfo& TimeZoneInfo::Local()
{
    static auto timeZone = TimeZoneInfo(icu::TimeZone::createDefault());
    return timeZone;
}

const TimeZoneInfo& TimeZoneInfo::Utc()
{
    static auto timeZone = TimeZoneInfo(icu::TimeZone::createTimeZone(u"UTC"));
    return timeZone;
}

DateTime TimeZoneInfo::ConvertTime(DateTime dateTime, const TimeZoneInfo& destinationTimeZone)
{
    if (dateTime.Kind() == DateTimeKind::Utc)
    {
        return ConvertTime(dateTime, Utc(), destinationTimeZone);
    }

    return ConvertTime(dateTime, Local(), destinationTimeZone);
}

DateTime TimeZoneInfo::ConvertTime(DateTime dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone)
{
    // The kind of dateTime and sourceTimeZone must be the same.
    auto sourceKind = GetCorrespondingKind(sourceTimeZone);
    if (dateTime.Kind() != sourceKind && dateTime.Kind() != DateTimeKind::Unspecified)
    {
        return dateTime;
    }

    // Filter special cases like UTC->UTC or Local->Local
    auto destinationKind = GetCorrespondingKind(destinationTimeZone);
    if (sourceKind == destinationKind && sourceKind != DateTimeKind::Unspecified && destinationKind != DateTimeKind::Unspecified)
    {
        return dateTime;
    }

    // Convert the dateTime utc offset to 0.
    auto ticks = dateTime.Ticks() - sourceTimeZone.BaseUtcOffset().Ticks();
    if (destinationKind == DateTimeKind::Utc)
    {
        return {static_cast<uint64_t>(ticks), destinationKind};
    }

    return {static_cast<uint64_t>(ticks + destinationTimeZone.BaseUtcOffset().Ticks()), destinationKind};
}

DateTime TimeZoneInfo::ConvertTimeFromUtc(DateTime dateTime, const TimeZoneInfo& destinationTimeZone)
{
    return ConvertTime(dateTime, Utc(), destinationTimeZone);
}

DateTime TimeZoneInfo::ConvertTimeToUtc(DateTime dateTime, const TimeZoneInfo& sourceTimeZone)
{
    return ConvertTime(dateTime, sourceTimeZone, Utc());
}

DateTime TimeZoneInfo::ConvertTimeToUtc(DateTime dateTime)
{
    if (dateTime.Kind() == DateTimeKind::Utc)
    {
        return dateTime;
    }

    return ConvertTime(dateTime, Local(), TimeZoneInfo::Utc());
}

TimeSpan TimeZoneInfo::BaseUtcOffset() const noexcept
{
    return baseUtcOffset_;
}

std::u16string_view TimeZoneInfo::Id() const noexcept
{
    return id_;
}

std::u16string_view TimeZoneInfo::DisplayName() const noexcept
{
    return displayName_;
}

std::u16string_view TimeZoneInfo::DaylightName() const noexcept
{
    return daylightDisplayName_;
}

std::u16string_view TimeZoneInfo::StandardName() const noexcept
{
    return standardDisplayName_;
}

bool TimeZoneInfo::IsSupportDaylightSavingTime() const noexcept
{
    return timeZone_->useDaylightTime();
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

std::u16string_view TimeZoneInfo::ToString() const noexcept
{
    return DisplayName();
}

std::optional<TimeZoneInfo> TimeZoneInfo::FindSystemTimeZoneById(std::u16string_view id)
{
    icu::TimeZone* timeZone = icu::TimeZone::createTimeZone({id.data(), static_cast<int32_t>(id.length())});
    if (*timeZone == icu::TimeZone::getUnknown())
    {
        return std::nullopt;
    }

    return std::optional<TimeZoneInfo>(timeZone);
}

ReadOnlySpan<TimeZoneInfo> TimeZoneInfo::GetSystemTimeZones()
{
    static std::vector<TimeZoneInfo> timeZoneInfos = CreateTimeZoneInfos();
    return timeZoneInfos;
}

std::u16string TimeZoneInfo::CreateId(const icu::TimeZone* timeZone)
{
    icu::UnicodeString str;
    timeZone->getDisplayName(false, icu::TimeZone::LONG, icu::Locale::getUS(), str);

    return {str.getBuffer(), static_cast<size_t>(str.length())};
}

std::u16string TimeZoneInfo::CreateDisplayName(const icu::TimeZone* timeZone)
{
    icu::UnicodeString displayName, id;
    timeZone->getDisplayName(false, icu::TimeZone::LONG_GMT, icu::Locale::getDefault(), displayName);
    timeZone->getDisplayName(false, icu::TimeZone::LONG, icu::Locale::getDefault(), id);

    std::basic_stringstream<char16_t> ss;
    ss << u'(';
    ss << std::u16string_view(displayName.getBuffer(), displayName.length());
    ss << u") ";
    ss << std::u16string_view(id.getBuffer(), id.length());

    return ss.str();
}

std::u16string TimeZoneInfo::CreateStandardName(const icu::TimeZone* timeZone)
{
    icu::UnicodeString str;
    timeZone->getDisplayName(false, icu::TimeZone::LONG, icu::Locale::getDefault(), str);

    return {str.getBuffer(), static_cast<size_t>(str.length())};
}

std::u16string TimeZoneInfo::CreateDaylightName(const icu::TimeZone* timeZone)
{
    icu::UnicodeString str;
    timeZone->getDisplayName(true, icu::TimeZone::LONG, icu::Locale::getDefault(), str);

    return {str.getBuffer(), static_cast<size_t>(str.length())};
}

CS2CPP_NAMESPACE_END
