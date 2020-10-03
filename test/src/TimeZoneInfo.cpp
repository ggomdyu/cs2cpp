#include <gtest/gtest.h>
#include <System/DateTime.h>
#include <System/TimeZoneInfo.h>

using namespace CS2CPP_NAMESPACE_NAME;

TEST(TimeZoneInfo, GetSystemTimeZones)
{
    auto timeZones = TimeZoneInfo::GetSystemTimeZones();
    EXPECT_GT(timeZones.Length(), 0);
}

TEST(TimeZoneInfo, FindSystemTimeZoneById)
{
    auto timeZone = TimeZoneInfo::FindSystemTimeZoneById(u"Asia/Tokyo");
    EXPECT_TRUE(timeZone.has_value());
    EXPECT_EQ(timeZone->BaseUtcOffset().Hours(), 9);
}

TEST(TimeZoneInfo, ConvertTime)
{
    auto& localTimeZone = TimeZoneInfo::Local();

    auto dateTime = DateTime(2011, 10, 9, 0, 0, 10, DateTimeKind::Utc);
    EXPECT_EQ(TimeZoneInfo::ConvertTime(dateTime, TimeZoneInfo::Utc(), localTimeZone).Ticks(), dateTime.Ticks() + localTimeZone.BaseUtcOffset().Ticks());

    auto dateTime2 = DateTime(2011, 10, 9, 0, 0, 10, DateTimeKind::Local);
    EXPECT_EQ(TimeZoneInfo::ConvertTime(dateTime2, localTimeZone, TimeZoneInfo::Utc()).Ticks(), dateTime2.Ticks() - localTimeZone.BaseUtcOffset().Ticks());

    auto dateTime3 = DateTime(2011, 10, 9, 0, 0, 10, DateTimeKind::Unspecified);
    EXPECT_EQ(TimeZoneInfo::ConvertTime(dateTime3, localTimeZone, TimeZoneInfo::Utc()).Ticks(), dateTime3.Ticks() - localTimeZone.BaseUtcOffset().Ticks());
    EXPECT_EQ(TimeZoneInfo::ConvertTime(dateTime3, TimeZoneInfo::Utc(), localTimeZone).Ticks(), dateTime3.Ticks() + localTimeZone.BaseUtcOffset().Ticks());
}

TEST(TimeZoneInfo, ConvertTimeFromUtc)
{
    auto& localTimeZone = TimeZoneInfo::Local();

    auto dateTime = DateTime(2011, 10, 9, 0, 0, 10, DateTimeKind::Utc);
    EXPECT_EQ(TimeZoneInfo::ConvertTimeFromUtc(dateTime, localTimeZone).Ticks(), dateTime.Ticks() + localTimeZone.BaseUtcOffset().Ticks());
}

TEST(TimeZoneInfo, ConvertTimeToUtc)
{
    auto& localTimeZone = TimeZoneInfo::Local();

    auto dateTime = DateTime(2011, 10, 9, 0, 0, 10, DateTimeKind::Local);
    EXPECT_EQ(TimeZoneInfo::ConvertTimeToUtc(dateTime, localTimeZone).Ticks(), dateTime.Ticks() - localTimeZone.BaseUtcOffset().Ticks());
}