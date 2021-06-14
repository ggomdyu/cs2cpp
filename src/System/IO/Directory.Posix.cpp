#include <array>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>

#include "System/IO/Directory.h"
#include "System/IO/DirectoryInfo.h"
#include "System/IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::directory
{

std::optional<struct stat> CreateStat(std::u16string_view path)
{
    auto utf8Path = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * path.length());
    if (!utf8Path)
        return {};

    struct stat s{};
    if (stat(reinterpret_cast<const char*>(utf8Path->data()), &s) != 0)
        return {};

    return s;
}

bool InternalRecursiveDelete(std::string_view path)
{
    DIR* dir = opendir(reinterpret_cast<const char*>(path.data()));
    if (!dir)
        return false;

    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr)
    {
        // Ignore the . and ..
        if ((ent->d_namlen == 1 && ent->d_name[0] == '.') ||
            (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
            continue;

        auto fullPath = PosixPath::Combine(path, {ent->d_name, ent->d_namlen});
        if (ent->d_type & DT_DIR)
            InternalRecursiveDelete(fullPath);
        else
            unlink(reinterpret_cast<const char*>(fullPath.data()));
    }
    closedir(dir);

    return rmdir(reinterpret_cast<const char*>(path.data())) == 0;
}

}

bool Directory::Exists(std::u16string_view path)
{
    auto s = detail::directory::CreateStat(path);
    if (!s || !S_ISDIR(s->st_mode))
        return false;

    return true;
}

bool Directory::Delete(std::u16string_view path, bool recursive)
{
    auto utf8Path = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * path.length());
    if (!utf8Path)
        return false;

    if (!recursive)
        return rmdir(reinterpret_cast<const char*>(utf8Path->data())) == 0;

    return detail::directory::InternalRecursiveDelete({reinterpret_cast<const char*>(utf8Path->data()), utf8Path->size()});
}

bool Directory::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto utf8SrcPath = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        reinterpret_cast<const std::byte*>(srcPath.data()), static_cast<int32_t>(sizeof(srcPath[0]) * srcPath.length()));
    if (!utf8SrcPath)
        return false;

    // Check whether the specified file exists.
    struct stat s{};
    if (stat(reinterpret_cast<const char*>(utf8SrcPath->data()), &s) != 0 || !S_ISDIR(s.st_mode))
        return false;

    auto utf8DestPath = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        reinterpret_cast<const std::byte*>(destPath.data()), static_cast<int32_t>(sizeof(destPath[0]) * destPath.length()));
    if (!utf8DestPath)
        return false;

    return rename(reinterpret_cast<const char*>(utf8SrcPath->data()), reinterpret_cast<const char*>(utf8DestPath->data())) == 0;
}

bool Directory::SetCurrentDirectory(std::u16string_view path)
{
    auto utf8Path = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        reinterpret_cast<const std::byte*>(path.data()), static_cast<int32_t>(sizeof(path[0]) * path.length()));
    if (!utf8Path)
        return false;

    return chdir(reinterpret_cast<const char*>(utf8Path->data())) == 0;
}

std::u16string Directory::GetCurrentDirectory()
{
    std::array<char, 4096> buffer{};
    if (getcwd(buffer.data(), buffer.size()) == nullptr)
        return {};

    auto utf16Str = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(buffer.data()),
        static_cast<int32_t>(strlen(buffer.data())));
    if (!utf16Str)
        return {};

    return std::move(utf16Str.value());
}

std::vector<std::u16string> Directory::GetLogicalDrives()
{
    std::vector<std::u16string> logicalDrives;

    struct statfs* m = nullptr;
    for (auto i = 0; i < getmntinfo(&m, MNT_WAIT); ++i)
    {
        auto logicalDrive = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(m[i].f_mntonname),
            static_cast<int32_t>(strlen(m[i].f_mntonname)));
        if (!logicalDrive)
            continue;

        logicalDrives.push_back(std::move(logicalDrive.value()));
    }

    return logicalDrives;
}

bool Directory::InternalCreateDirectory(std::u16string_view path)
{
    auto utf8Path = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        reinterpret_cast<const std::byte*>(path.data()), static_cast<int32_t>(sizeof(path[0]) * path.length()));
    if (!utf8Path)
        return false;

    return mkdir(reinterpret_cast<const char*>(utf8Path->data()), 0777) == 0;
}

CS2CPP_NAMESPACE_END
