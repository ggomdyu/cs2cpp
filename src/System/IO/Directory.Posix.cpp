#include <array>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>

#include "System/IO/Directory.h"
#include "System/IO/DirectoryInfo.h"
#include "System/IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

extern thread_local std::vector<char> GlobalCharBuffer;

namespace detail::directory
{

std::optional<struct stat> CreateStat(std::u16string_view path)
{
    auto srcBytes = gsl::span(reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * path.length());
    auto destBytes = gsl::span(reinterpret_cast<std::byte*>(GlobalCharBuffer.data()), GlobalCharBuffer.size());
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(), srcBytes, destBytes))
    {
        return {};
    }

    struct stat s{};
    if (stat(reinterpret_cast<const char*>(destBytes.data()), &s) != 0)
    {
        return {};
    }

    return s;
}

bool InternalRecursiveDelete(std::string_view path)
{
    DIR* dir = opendir(reinterpret_cast<const char*>(path.data()));
    if (dir == nullptr)
    {
        return false;
    }
    
    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr)
    {
        // Ignore the . and ..
        if ((ent->d_namlen == 1 && ent->d_name[0] == '.') ||
            (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
        {
            continue;
        }

        auto fullPath = PosixPath::Combine(path, {ent->d_name, ent->d_namlen});
        if (ent->d_type & DT_DIR)
        {
            InternalRecursiveDelete(fullPath);
        }
        else
        {
            unlink(reinterpret_cast<const char*>(fullPath.data()));
        }
    }
    closedir(dir);

    return rmdir(reinterpret_cast<const char*>(path.data())) == 0;
}

}

bool Directory::Exists(std::u16string_view path)
{
    auto s = detail::directory::CreateStat(path);
    if (!s || !S_ISDIR(s->st_mode))
    {
        return false;
    }
    
    return true;
}

bool Directory::Delete(std::u16string_view path, bool recursive)
{
    auto srcBytes = gsl::span(reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * path.length());
    auto destBytes = gsl::span(reinterpret_cast<std::byte*>(GlobalCharBuffer.data()), GlobalCharBuffer.size());
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(), srcBytes, destBytes))
    {
        return {};
    }

    if (!recursive)
    {
        return rmdir(reinterpret_cast<const char*>(destBytes.data())) == 0;
    }

    return detail::directory::InternalRecursiveDelete({reinterpret_cast<const char*>(destBytes.data()), destBytes.size()});
}

bool Directory::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto utf8SrcPath = gsl::span(reinterpret_cast<std::byte*>(GlobalCharBuffer.data()), GlobalCharBuffer.size() / 2);
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(srcPath.data()), sizeof(srcPath[0]) * srcPath.length()}, utf8SrcPath))
    {
        return false;
    }

    struct stat s{};
    if (stat(reinterpret_cast<const char*>(utf8SrcPath.data()), &s) != 0 || !S_ISDIR(s.st_mode))
    {
        return false;
    }

    auto utf8DestPath = gsl::span(reinterpret_cast<std::byte*>(GlobalCharBuffer.data()) + GlobalCharBuffer.size() / 2,
        GlobalCharBuffer.size() / 2);
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(destPath.data()), sizeof(destPath[0]) * destPath.length()}, utf8DestPath))
    {
        return false;
    }

    return rename(reinterpret_cast<const char*>(utf8SrcPath.data()),
        reinterpret_cast<const char*>(utf8DestPath.data())) == 0;
}

bool Directory::SetCurrentDirectory(std::u16string_view path)
{
    auto srcBytes = gsl::span(reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * path.length());
    auto destBytes = gsl::span(reinterpret_cast<std::byte*>(GlobalCharBuffer.data()), GlobalCharBuffer.size());
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(), srcBytes, destBytes))
    {
        return {};
    }

    return chdir(reinterpret_cast<const char*>(destBytes.data())) == 0;
}

String Directory::GetCurrentDirectory()
{
    if (getcwd(GlobalCharBuffer.data(), GlobalCharBuffer.size()) == nullptr)
    {
        return {};
    }

    auto utf16Str = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(GlobalCharBuffer.data()),
        strlen(GlobalCharBuffer.data()));
    if (!utf16Str)
    {
        return {};
    }

    return std::move(utf16Str.value());
}

std::vector<String> Directory::GetLogicalDrives()
{
    std::vector<String> logicalDrives;

    struct statfs* m = nullptr;
    for (auto i = 0; i < getmntinfo(&m, MNT_WAIT); ++i)
    {
        auto logicalDrive = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(m[i].f_mntonname),
            strlen(m[i].f_mntonname));
        if (!logicalDrive)
        {
            continue;
        }

        logicalDrives.push_back(std::move(logicalDrive.value()));
    }

    return logicalDrives;
}

bool Directory::InternalCreateDirectory(std::u16string_view path)
{
    auto srcBytes = gsl::span(reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * path.length());
    auto destBytes = gsl::span(reinterpret_cast<std::byte*>(GlobalCharBuffer.data()), GlobalCharBuffer.size());
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(), srcBytes, destBytes))
    {
        return {};
    }

    return mkdir(reinterpret_cast<const char*>(destBytes.data()), 0777) == 0;
}

CS2CPP_NAMESPACE_END