#include "System/IO/Directory.h"
#include "System/IO/File.h"

CS2CPP_NAMESPACE_BEGIN

DirectoryInfo Directory::CreateDirectory(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    if (fullPath.length() > 0 && !Path::IsDirectorySeparator(fullPath[fullPath.length() - 1]))
        fullPath += Path::DirectorySeparatorChar;

    for (size_t i = Path::IsDirectorySeparator(fullPath[0]) ? 1 : 0; i < fullPath.length(); ++i)
    {
        auto c = fullPath[i];
        if (Path::IsDirectorySeparator(c))
        {
            fullPath[i] = 0;
            InternalCreateDirectory(&fullPath[0]);
            fullPath[i] = c;
        }
    }

    return DirectoryInfo(std::move(fullPath), FullPathTag{});
}

DirectoryInfo Directory::GetParent(std::u16string_view path)
{
    if (path.length() == 0)
        return DirectoryInfo(u"", FullPathTag{});

    std::u16string parent(path);
    if (!Path::IsDirectorySeparator(parent[parent.length() - 1]))
        parent += Path::DirectorySeparatorChar;

    parent += u"../";

    return DirectoryInfo(std::move(parent));
}

bool Directory::SetCreationTime(std::u16string_view path, DateTime creationTime)
{
    return File::SetCreationTime(path, creationTime);
}

bool Directory::SetCreationTimeUtc(std::u16string_view path, DateTime creationTimeUtc)
{
    return File::SetCreationTimeUtc(path, creationTimeUtc);
}

bool Directory::SetLastAccessTime(std::u16string_view path, DateTime lastAccessTime)
{
    return File::SetLastAccessTime(path, lastAccessTime);
}

bool Directory::SetLastAccessTimeUtc(std::u16string_view path, DateTime lastAccessTimeUtc)
{
    return File::SetLastAccessTimeUtc(path, lastAccessTimeUtc);
}

bool Directory::SetLastWriteTime(std::u16string_view path, DateTime lastWriteTime)
{
    return File::SetLastWriteTime(path, lastWriteTime);
}

bool Directory::SetLastWriteTimeUtc(std::u16string_view path, DateTime lastWriteTimeUtc)
{
    return File::SetLastWriteTimeUtc(path, lastWriteTimeUtc);
}

std::optional<DateTime> Directory::GetCreationTime(std::u16string_view path)
{
    return File::GetCreationTime(path);
}

std::optional<DateTime> Directory::GetCreationTimeUtc(std::u16string_view path)
{
    return File::GetCreationTimeUtc(path);
}

std::optional<DateTime> Directory::GetLastAccessTime(std::u16string_view path)
{
    return File::GetLastAccessTime(path);
}

std::optional<DateTime> Directory::GetLastAccessTimeUtc(std::u16string_view path)
{
    return File::GetLastAccessTimeUtc(path);
}

std::optional<DateTime> Directory::GetLastWriteTime(std::u16string_view path)
{
    return File::GetLastWriteTime(path);
}

std::optional<DateTime> Directory::GetLastWriteTimeUtc(std::u16string_view path)
{
    return File::GetLastWriteTimeUtc(path);
}

std::u16string Directory::GetDirectoryRoot(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    return fullPath.substr(0, Path::GetRootLength(fullPath));
}

std::vector<std::u16string> Directory::GetDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<std::u16string> ret;
    EnumerateDirectories(path, searchPattern, searchOption, [&](std::u16string&& str)
    {
        ret.push_back(std::move(str));
    });

    return ret;
}

std::vector<std::u16string> Directory::GetFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<std::u16string> ret;
    EnumerateFiles(path, searchPattern, searchOption, [&](std::u16string&& str)
    {
        ret.push_back(std::move(str));
    });

    return ret;
}

std::vector<std::u16string> Directory::GetFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<std::u16string> ret;
    EnumerateFileSystemEntries(path, searchPattern, searchOption, [&](std::u16string&& str)
    {
        ret.push_back(std::move(str));
    });

    return ret;
}

CS2CPP_NAMESPACE_END
