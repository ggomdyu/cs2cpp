#pragma once

#include <optional>
#include <unicode/timezone.h>

#include "System/DateTime.h"
#include "System/ReadOnlySpan.h"

CS2CPP_NAMESPACE_BEGIN

class TimeZoneInfo final
{
public:
    explicit TimeZoneInfo(icu::TimeZone* timeZone) noexcept;

public:
    static DateTime ConvertTime(DateTime dateTime, const TimeZoneInfo& destinationTimeZone);
    static DateTime ConvertTime(DateTime dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone);
    static DateTime ConvertTimeFromUtc(DateTime dateTime, const TimeZoneInfo& destinationTimeZone);
    static DateTime ConvertTimeToUtc(DateTime dateTime, const TimeZoneInfo& sourceTimeZone);
    static DateTime ConvertTimeToUtc(DateTime dateTime);
    static std::optional<TimeZoneInfo> FindSystemTimeZoneById(std::u16string_view id);
    static ReadOnlySpan<TimeZoneInfo> GetSystemTimeZones();
    static const TimeZoneInfo& Local();
    static const TimeZoneInfo& Utc();
    std::u16string_view Id() const noexcept;
    std::u16string_view DisplayName() const noexcept;
    std::u16string_view DaylightName() const noexcept;
    std::u16string_view StandardName() const noexcept;
    TimeSpan BaseUtcOffset() const noexcept;
    bool IsSupportDaylightSavingTime() const noexcept;
    std::u16string_view ToString() const noexcept;

private:
    static DateTimeKind GetCorrespondingKind(const TimeZoneInfo& timeZone);
    static std::u16string CreateId(const icu::TimeZone* timeZone);
    static std::u16string CreateDisplayName(const icu::TimeZone* timeZone);
    static std::u16string CreateStandardName(const icu::TimeZone* timeZone);
    static std::u16string CreateDaylightName(const icu::TimeZone* timeZone);

private:
    std::unique_ptr<icu::TimeZone> timeZone_;
    TimeSpan baseUtcOffset_ = TimeSpan(0);
    std::u16string displayName_;
    std::u16string standardDisplayName_;
    std::u16string daylightDisplayName_;
    std::u16string id_;
};

CS2CPP_NAMESPACE_END