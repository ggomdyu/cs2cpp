#include <array>
#include <sys/stat.h>
#include <unistd.h>

#include "System.IO/Directory.h"
#include "System.IO/DirectoryInfo.h"
#include "System.IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

extern std::optional<struct stat> CreateStat(std::u16string_view path);

namespace
{

bool InternalRecursiveDelete(std::string_view path)
{
    DIR* dir = opendir(reinterpret_cast<const char*>(path.data()));
    if (!dir)
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
    auto s = CreateStat(path);
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