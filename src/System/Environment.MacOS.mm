#include <Foundation/Foundation.h>
#include <mach/mach_time.h>
#include <pthread.h>

#include "System/Environment.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::environment
{

std::u16string GetSpecialDirectory(NSString* path, std::u16string_view postfix)
{
    std::u16string ret(reinterpret_cast<const char16_t*>([path cStringUsingEncoding:NSUTF16LittleEndianStringEncoding]));
    ret += postfix;
    
    return ret;
}

std::u16string GetSpecialDirectory(NSSearchPathDirectory pathDirectory, NSSearchPathDomainMask pathDomainMask, std::u16string_view postfix)
{
    auto paths = NSSearchPathForDirectoriesInDomains(pathDirectory, pathDomainMask, YES);
    if ([paths count] <= 0)
        return {};

    return GetSpecialDirectory(paths[0], postfix);
}

}

int32_t Environment::GetCurrentManagedThreadId()
{
    return pthread_mach_thread_np(pthread_self());
}

std::u16string Environment::GetFolderPath(SpecialFolder folder)
{
    using detail::environment::GetSpecialDirectory;

    switch (folder)
    {
    case SpecialFolder::ApplicationData:
        return GetSpecialDirectory(NSHomeDirectory(), u".config");

    case SpecialFolder::CommonApplicationData:
        return GetSpecialDirectory(@"/usr/share", {u"", 0});

    case SpecialFolder::Desktop:
    case SpecialFolder::DesktopDirectory:
        return GetSpecialDirectory(NSDesktopDirectory, NSUserDomainMask, {u"", 0});

    case SpecialFolder::Fonts:
        return GetSpecialDirectory(NSLibraryDirectory, NSUserDomainMask, u"/Fonts");

    case SpecialFolder::Favorites:
        return GetSpecialDirectory(NSLibraryDirectory, NSUserDomainMask, u"/Favorites");

    case SpecialFolder::InternetCache:
        return GetSpecialDirectory(NSCachesDirectory, NSUserDomainMask, {u"", 0});

    case SpecialFolder::ProgramFiles:
        return GetSpecialDirectory(@"/Applications", {u"", 0});

    case SpecialFolder::System:
        return GetSpecialDirectory(@"/System", {u"", 0});

    case SpecialFolder::UserProfile:
    case SpecialFolder::MyDocuments:
        return GetSpecialDirectory(NSHomeDirectory(), {u"", 0});

    case SpecialFolder::MyMusic:
        return GetSpecialDirectory(NSMusicDirectory, NSUserDomainMask, {u"", 0});

    case SpecialFolder::MyPictures:
        return GetSpecialDirectory(NSPicturesDirectory, NSUserDomainMask, {u"", 0});

    default:
        return {};
    }
}

const std::u16string& Environment::GetCommandLine()
{
    static std::u16string commandLine = []()
    {
        std::u16string ret;

        auto arguments = [[NSProcessInfo processInfo] arguments];
        for (NSUInteger i = 0; i < [arguments count] - 1; ++i)
        {
            std::u16string_view argument(reinterpret_cast<const char16_t*>([arguments[i]
                cStringUsingEncoding:NSUTF16LittleEndianStringEncoding]));
            
            ret += argument;
            ret += u' ';
        }

        std::u16string_view argument(reinterpret_cast<const char16_t*>([arguments[arguments.count - 1]
            cStringUsingEncoding:NSUTF16LittleEndianStringEncoding]));
        ret += argument;
    
        return ret;
    } ();

    return commandLine;
}

int64_t Environment::GetTickCount()
{
    return int64_t(mach_absolute_time() * 0.000001f);
}

CS2CPP_NAMESPACE_END
