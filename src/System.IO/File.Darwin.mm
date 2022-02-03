#include <cstdio>
#include <Foundation/Foundation.h>
#include <Foundation/NSString.h>
#include <utime.h>

#include "System.IO/File.h"

CS2CPP_NAMESPACE_BEGIN

static NSDate* DateTimeToNative(DateTime dateTime)
{
    auto dateComponents = [[NSDateComponents alloc] init];
    [dateComponents setYear:dateTime.Year()];
    [dateComponents setMonth:dateTime.Month()];
    [dateComponents setDay:dateTime.Day()];
    [dateComponents setHour:dateTime.Hour()];
    [dateComponents setMinute:dateTime.Minute()];
    [dateComponents setSecond:dateTime.Second()];

    auto calendar = [NSCalendar currentCalendar];
    auto date = [calendar dateFromComponents:dateComponents];

    return dateTime.Kind() == DateTimeKind::Utc
        ? [date dateByAddingTimeInterval:static_cast<NSTimeInterval>([[NSTimeZone systemTimeZone] secondsFromGMT])]
        : date;
}

bool File::SetCreationTime(std::u16string_view path, DateTime creationTimeUtc)
{
    auto date = DateTimeToNative(creationTimeUtc);

    auto attributes = @{NSFileCreationDate : date};
    auto nsPath = [NSString stringWithCharacters:reinterpret_cast<const unichar*>(path.data())
                                          length:path.length()];

    return !![[NSFileManager defaultManager] setAttributes: attributes
                                              ofItemAtPath:nsPath
                                                     error: nil];
}
    
bool File::SetLastAccessTime(std::u16string_view path, DateTime lastAccessTimeUtc)
{
    int64_t ticks = (lastAccessTimeUtc.ToUniversalTime().Ticks() - DateTime::UnixEpoch().Ticks()) / TimeSpan::TicksPerSecond;

    auto characters = reinterpret_cast<const unichar*>(path.data());
    auto utf8Path = [NSString stringWithCharacters:characters
                                            length:path.length()].UTF8String;

    utimbuf buf{ticks, ticks};
    return utime(utf8Path, &buf) == 0;
}

bool File::SetLastWriteTime(std::u16string_view path, DateTime lastWriteTimeUtc)
{
    auto date = DateTimeToNative(lastWriteTimeUtc);
    
    auto attributes = @{NSFileModificationDate : date};
    auto characters = reinterpret_cast<const unichar*>(path.data());
    auto nsPath = [NSString stringWithCharacters:characters
                                          length:path.length()];

    return [[NSFileManager defaultManager] setAttributes:attributes
                                            ofItemAtPath:nsPath
                                                   error:nil];
}

bool File::Copy(std::u16string_view srcPath, std::u16string_view destPath, bool overwrite)
{
    if (overwrite)
    {
        Delete(destPath);
    }

    auto nsSrcPath = [NSString stringWithCharacters:reinterpret_cast<const unichar*>(srcPath.data())
                                             length:srcPath.length()];
    auto nsDestPath = [NSString stringWithCharacters:reinterpret_cast<const unichar*>(destPath.data())
                                              length:destPath.length()];

    return [[NSFileManager defaultManager] copyItemAtPath:nsSrcPath
                                                   toPath:nsDestPath
                                                    error:nil];
}

CS2CPP_NAMESPACE_END
