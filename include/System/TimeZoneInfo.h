#pragma once

#include "DateTime.h"
#include "String.h"

CS2CPP_NAMESPACE_BEGIN

class TimeZoneInfo final
{
public:
    TimeZoneInfo() noexcept = default;
    TimeZoneInfo(String id, TimeSpan baseUtcOffset, String displayName, String standardDisplayName, String daylightDisplayName, bool supportsDaylightSavingTime) noexcept;

public:
    [[nodiscard]] static const TimeZoneInfo& Local();
    [[nodiscard]] static const TimeZoneInfo& Utc() noexcept;
    [[nodiscard]] static DateTime ConvertTime(DateTime dateTime, const TimeZoneInfo& destinationTimeZone);
    [[nodiscard]] static DateTime ConvertTimeFromUtc(DateTime dateTime, const TimeZoneInfo& destinationTimeZone);
    [[nodiscard]] static DateTime ConvertTimeToUtc(DateTime dateTime);
    [[nodiscard]] const String& GetId() const noexcept;
    [[nodiscard]] TimeSpan GetBaseUtcOffset() const noexcept;
    [[nodiscard]] const String& GetStandardDisplayName() const noexcept;
    [[nodiscard]] const String& GetDaylightDisplayName() const noexcept;
    [[nodiscard]] bool IsSupportDaylightSavingTime() const noexcept;

private:
    [[nodiscard]] static DateTimeKind GetCorrespondingKind(const TimeZoneInfo& timeZone);
    [[nodiscard]] static DateTime ConvertTime(DateTime dateTime, const TimeZoneInfo& sourceTimeZone, const TimeZoneInfo& destinationTimeZone);
    [[nodiscard]] static TimeZoneInfo CreateLocal();

private:
    String _id;
    TimeSpan _baseUtcOffset = TimeSpan(0);
    String _displayName;
    String _standardDisplayName;
    String _daylightDisplayName;
    bool _supportsDaylightSavingTime = false;
};

CS2CPP_NAMESPACE_END