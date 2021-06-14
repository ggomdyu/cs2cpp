#include <cstdio>
#include <Foundation/Foundation.h>
#include <Foundation/NSString.h>
#include <utime.h>

#include "System/IO/File.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::file
{

NSDate* ConvertDateTimeToNative(DateTime dateTime)
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
    auto modificationDate = [date dateByAddingTimeInterval:[[NSTimeZone systemTimeZone] secondsFromGMT]];

    return modificationDate;
}

}
    
bool File::SetCreationTimeUtc(std::u16string_view path, DateTime creationTimeUtc)
{
    auto date = detail::file::ConvertDateTimeToNative(creationTimeUtc);
    auto attributes = [NSDictionary dictionaryWithObjectsAndKeys:date, NSFileCreationDate, nullptr];
    
    auto fileManager = [NSFileManager defaultManager];
    auto nsPath = [NSString stringWithCharacters:reinterpret_cast<const unichar*>(path.data()) length:path.length()];

    return !![fileManager setAttributes: attributes ofItemAtPath:nsPath error: nullptr];
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
    
    auto attributes = [NSDictionary dictionaryWithObjectsAndKeys:date, NSFileModificationDate, nullptr];
    auto characters = reinterpret_cast<const unichar*>(path.data());
    auto nsPath = [NSString stringWithCharacters:characters length:path.length()];

    return [[NSFileManager defaultManager] setAttributes:attributes ofItemAtPath:nsPath error:nil];
}

bool File::Copy(std::u16string_view srcPath, std::u16string_view destPath, bool overwrite)
{
    if (overwrite)
        Delete(destPath);

    auto srcPathChars = reinterpret_cast<const unichar*>(srcPath.data());
    auto destPathChars = reinterpret_cast<const unichar*>(destPath.data());

    auto nsSrcPath = [NSString stringWithCharacters:srcPathChars length:srcPath.length()];
    auto nsDestPath = [NSString stringWithCharacters:destPathChars length:destPath.length()];

    return [[NSFileManager defaultManager] copyItemAtPath:nsSrcPath toPath:nsDestPath error:nil];
}

SafeFilePointer File::InternalFileOpen(std::u16string_view path, std::u16string_view mode)
{
    auto pathChars = reinterpret_cast<const unichar*>(path.data());
    auto modeChars = reinterpret_cast<const unichar*>(mode.data());

    return SafeFilePointer(fopen(
        [NSString stringWithCharacters:pathChars length:path.length()].UTF8String,
        [NSString stringWithCharacters:modeChars length:mode.length()].UTF8String
    ));
}

CS2CPP_NAMESPACE_END
