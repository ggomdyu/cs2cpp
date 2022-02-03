#include <array>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "System.IO/Directory.h"
#include "System.IO/DirectoryInfo.h"
#include "System.IO/Path.h"
#include "System.Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

static std::optional<struct stat> CreateStat(std::u16string_view path)
{
    std::optional<std::vector<std::byte>> utf8Path = Encoding::UTF8().GetBytes(path);
    if (!utf8Path)
    {
        return std::nullopt;
    }

    struct stat s{};
    if (stat(reinterpret_cast<const char*>(utf8Path->data()), &s) != 0)
    {
        return std::nullopt;
    }

    return s;
}

static bool IsDotDirectory(std::string_view path) noexcept
{
    return (path.length() == 1 && path[0] == '.') || (path.length() == 2 && path == "..");
}

static std::string GetFullPath(std::string_view basePath, std::string_view fileName)
{
    std::string fullPath(basePath);
    bool hasSeparator = Path::IsDirectorySeparator(basePath.back()) || Path::IsVolumeSeparator(basePath.back());
    if (!hasSeparator)
    {
        fullPath += Path::DirectorySeparatorChar;
    }

    fullPath += fileName;

    return fullPath;
}

static bool InternalRecursiveDelete(std::string_view path)
{
    std::unique_ptr<DIR, decltype(&closedir)> dir(opendir(reinterpret_cast<const char*>(path.data())), &closedir);
    if (!dir)
    {
        return false;
    }

    struct dirent* dirEntry;
    while ((dirEntry = readdir(dir.get())) != nullptr)
    {
#if CS2CPP_PLATFORM_DARWIN
        std::string_view fileName(dirEntry->d_name, dirEntry->d_namlen);
#else
        std::string_view fileName(dirEntry->d_name);
#endif
        if (IsDotDirectory(fileName))
        {
            continue;
        }

        std::string fullPath = GetFullPath(path, fileName);
        if (dirEntry->d_type & DT_DIR)
        {
            InternalRecursiveDelete(fullPath);
        }
        else
        {
            unlink(reinterpret_cast<const char*>(fullPath.data()));
        }
    }

    return rmdir(reinterpret_cast<const char*>(path.data())) == 0;
}

bool Directory::Exists(std::u16string_view path)
{
    std::optional<struct stat> s = CreateStat(path);
    return s && S_ISDIR(s->st_mode);
}

bool Directory::Delete(std::u16string_view path, bool recursive)
{
    auto fullPath = Path::GetFullPath(path);
    auto utf8FullPath = Encoding::UTF8().GetBytes(fullPath);
    if (!utf8FullPath)
    {
        return false;
    }

    if (!recursive)
    {
        return rmdir(reinterpret_cast<const char*>(utf8FullPath->data())) == 0;
    }

    return InternalRecursiveDelete({reinterpret_cast<const char*>(utf8FullPath->data()), fullPath.length()});
}

bool Directory::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto utf8SrcFullPath = Encoding::UTF8().GetBytes(Path::GetFullPath(srcPath));
    if (!utf8SrcFullPath)
    {
        return false;
    }

    // Check whether the specified file exists.
    struct stat s{};
    if (stat(reinterpret_cast<const char*>(utf8SrcFullPath->data()), &s) != 0 || !S_ISDIR(s.st_mode))
    {
        return false;
    }

    auto utf8DestFullPath = Encoding::UTF8().GetBytes(Path::GetFullPath(destPath));
    if (!utf8DestFullPath)
    {
        return false;
    }

    return rename(reinterpret_cast<const char*>(utf8SrcFullPath->data()), reinterpret_cast<const char*>(utf8DestFullPath->data())) == 0;
}

bool Directory::InternalCreateDirectory(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    auto utf8FullPath = Encoding::UTF8().GetBytes(fullPath);
    if (!utf8FullPath)
    {
        return false;
    }

    return mkdir(reinterpret_cast<const char*>(utf8FullPath->data()), 0777) == 0;
}

CS2CPP_NAMESPACE_END