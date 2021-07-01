#include "System/IO/Directory.h"
#include "System/IO/File.h"

CS2CPP_NAMESPACE_BEGIN

DirectoryInfo Directory::CreateDirectory(std::u16string_view path)
{
    if (path.length() == 0)
        return DirectoryInfo(u"", FullPathTag{});

    auto fullPath = Path::GetFullPath(path);
    if (InternalCreateDirectory(fullPath))
        return DirectoryInfo(std::move(fullPath), FullPathTag{});

    // Maybe the path is invalid because the path contains directories that don't exist.
    if (fullPath.length() > 0 && !Path::IsDirectorySeparator(fullPath[fullPath.length() - 1]))
        fullPath += Path::DirectorySeparatorChar;

    auto it = std::next(fullPath.begin(), Path::IsDirectorySeparator(fullPath[0]) ? 1 : 0);
    while (it != fullPath.end())
    {
        auto c = *it;
        if (Path::IsDirectorySeparator(c))
        {
            *it = 0;
            InternalCreateDirectory(&fullPath[0]);
            *it = c;
        }

        ++it;
    }

    // Remove a temporarily added directory separator.
    if (Path::IsDirectorySeparator(fullPath.back()))
        fullPath.pop_back();

    return DirectoryInfo(std::move(fullPath), FullPathTag{});
}

DirectoryInfo Directory::GetParent(std::u16string_view path)
{
    if (path.length() == 0)
        return DirectoryInfo(u"", FullPathTag{});

    auto fullPath = Path::GetFullPath(path);
    return DirectoryInfo(Path::GetDirectoryName(fullPath));
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
    return Path::GetPathRoot(Path::GetFullPath(path));
}

std::vector<std::u16string> Directory::GetDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<std::u16string> ret;

    auto callback = [&](std::u16string&& str) { ret.push_back(std::move(str)); };
    EnumerateDirectories(path, searchPattern, searchOption, callback);

    return ret;
}

std::vector<std::u16string> Directory::GetFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<std::u16string> ret;

    auto callback = [&](std::u16string&& str) { ret.push_back(std::move(str)); };
    EnumerateFiles(path, searchPattern, searchOption, callback);

    return ret;
}

std::vector<std::u16string> Directory::GetFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<std::u16string> ret;

    auto callback = [&](std::u16string&& str) { ret.push_back(std::move(str)); };
    EnumerateFileSystemEntries(path, searchPattern, searchOption, callback);

    return ret;
}

CS2CPP_NAMESPACE_END
