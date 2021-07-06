#include <cstdio>
#include <Foundation/Foundation.h>
#include <Foundation/NSString.h>
#include <utime.h>

#include "System/IO/File.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::file
{

[[nodiscard]] NSDate* ConvertDateTimeToNative(DateTime dateTime)
{
    auto dateComponents = [[NSDateComponents alloc] init];
    [dateComponents setYear:dateTime.GetYear()];
    [dateComponents setMonth:dateTime.GetMonth()];
    [dateComponents setDay:dateTime.GetDay()];
    [dateComponents setHour:dateTime.GetHour()];
    [dateComponents setMinute:dateTime.GetMinute()];
    [dateComponents setSecond:dateTime.GetSecond()];

    auto calendar = [NSCalendar currentCalendar];
    auto date = [calendar dateFromComponents:dateComponents];

    return dateTime.GetKind() == DateTimeKind::Utc ?
        [date dateByAddingTimeInterval:[[NSTimeZone systemTimeZone] secondsFromGMT]] : date;
}

}
    
bool File::SetCreationTimeUtc(std::u16string_view path, DateTime creationTimeUtc)
{
    auto date = detail::file::ConvertDateTimeToNative(creationTimeUtc);

    auto attributes = @{NSFileCreationDate : date};
    auto nsPath = [NSString stringWithCharacters:reinterpret_cast<const unichar*>(path.data()) length:path.length()];
    return !![[NSFileManager defaultManager] setAttributes: attributes ofItemAtPath:nsPath error: nil];
}
    
bool File::SetLastAccessTimeUtc(std::u16string_view path, DateTime lastAccessTimeUtc)
{
    int64_t ticks = (lastAccessTimeUtc.ToUniversalTime().GetTicks() - DateTime::GetUnixEpoch().GetTicks()) /
        TimeSpan::TicksPerSecond;

    auto characters = reinterpret_cast<const unichar*>(path.data());
    auto utf8Path = [NSString stringWithCharacters:characters length:path.length()].UTF8String;

    utimbuf buf{ticks, ticks};
    return utime(utf8Path, &buf) == 0;
}

bool File::SetLastWriteTimeUtc(std::u16string_view path, DateTime lastWriteTimeUtc)
{
    auto date = detail::file::ConvertDateTimeToNative(lastWriteTimeUtc);
    
    auto attributes = @{NSFileModificationDate : date};
    auto characters = reinterpret_cast<const unichar*>(path.data());
    auto nsPath = [NSString stringWithCharacters:characters length:path.length()];

    return [[NSFileManager defaultManager] setAttributes:attributes ofItemAtPath:nsPath error:nil];
}

bool File::Copy(std::u16string_view srcPath, std::u16string_view destPath, bool overwrite)
{
    if (overwrite)
        Delete(destPath);

    auto nsSrcPath = [NSString stringWithCharacters:reinterpret_cast<const unichar*>(srcPath.data())
        length:srcPath.length()];
    auto nsDestPath = [NSString stringWithCharacters:reinterpret_cast<const unichar*>(destPath.data())
        length:destPath.length()];

    return [[NSFileManager defaultManager] copyItemAtPath:nsSrcPath toPath:nsDestPath error:nil];
}

SafeFilePointer File::InternalFileOpen(std::u16string_view path, std::u16string_view mode)
{
    return SafeFilePointer(fopen(
        [NSString stringWithCharacters:reinterpret_cast<const unichar*>(path.data()) length:path.length()].UTF8String,
        [NSString stringWithCharacters:reinterpret_cast<const unichar*>(mode.data()) length:mode.length()].UTF8String
    ));
}

CS2CPP_NAMESPACE_END
