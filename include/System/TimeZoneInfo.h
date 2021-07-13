#pragma once

#include <string>

#include "DateTime.h"

CS2CPP_NAMESPACE_BEGIN

class TimeZoneInfo final
{
public:
    TimeZoneInfo(std::u16string id, TimeSpan baseUtcOffset, std::u16string displayName, std::u16string standardDisplayName, std::u16string daylightDisplayName, bool supportsDaylightSavingTime) noexcept;

public:
    [[nodiscard]] static DateTime ConvertTime(DateTime dateTime, const TimeZoneInfo& destinationTimeZone);
    [[nodiscard]] static DateTime ConvertTimeFromUtc(DateTime dateTime, const TimeZoneInfo& destinationTimeZone);
    [[nodiscard]] static DateTime ConvertTimeToUtc(DateTime dateTime);
    [[nodiscard]] TimeSpan GetBaseUtcOffset() const noexcept;
    [[nodiscard]] std::u16string_view GetDaylightDisplayName() const noexcept;
    [[nodiscard]] std::u16string_view GetStandardDisplayName() const noexcept;
    [[nodiscard]] std::u16string_view GetId() const noexcept;
    [[nodiscard]] bool IsSupportDaylightSavingTime() const noexcept;
    [[nodiscard]] static const TimeZoneInfo& Local();
    [[nodiscard]] static const TimeZoneInfo& Utc();

private:
    static DateTimeKind GetCorrespondingKind(const TimeZoneInfo& timeZone);
    static DateTime ConvertTime(DateTime dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone);
    static TimeZoneInfo CreateLocal();

private:
    std::u16string _id;
    TimeSpan _baseUtcOffset = TimeSpan(0);
    std::u16string _displayName;
    std::u16string _standardDisplayName;
    std::u16string _daylightDisplayName;
    bool _supportsDaylightSavingTime = false;
};

CS2CPP_NAMESPACE_END
