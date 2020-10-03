#include "System/IO/Directory.h"
#include "System/IO/File.h"

CS2CPP_NAMESPACE_BEGIN

DirectoryInfo Directory::CreateDirectory(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    if (fullPath.Length() > 0 && !Path::IsDirectorySeparator(fullPath[fullPath.Length() - 1]))
    {
        fullPath += Path::DirectorySeparatorChar;
    }

    for (int32_t i = Path::IsDirectorySeparator(fullPath[0]) ? 1 : 0; i < fullPath.Length(); ++i)
    {
        const auto c = fullPath[i];
        if (Path::IsDirectorySeparator(c))
        {
            fullPath._str.setCharAt(i, 0);
            InternalCreateDirectory(&fullPath[0]);

            fullPath._str.setCharAt(i, c);
        }
    }

    return DirectoryInfo(std::move(fullPath), FullPathTag{});
}

DirectoryInfo Directory::GetParent(std::u16string_view path)
{
    if (path.length() == 0)
    {
        return DirectoryInfo(u"", FullPathTag{});
    }

    String parent(path);
    if (!Path::IsDirectorySeparator(parent[parent.Length() - 1]))
    {
        parent += Path::DirectorySeparatorChar;
    }

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

String Directory::GetDirectoryRoot(std::u16string_view path)
{
    const auto fullPath = Path::GetFullPath(path);
    return fullPath.Substring(0, Path::GetRootLength(fullPath));
}

std::vector<String> Directory::GetDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<String> ret;
    EnumerateDirectories(path, searchPattern, searchOption, [&](String&& str)
    {
        ret.push_back(std::move(str));
    });

    return ret;
}

std::vector<String> Directory::GetFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<String> ret;
    EnumerateFiles(path, searchPattern, searchOption, [&](String&& str)
    {
        ret.push_back(std::move(str));
    });

    return ret;
}

std::vector<String> Directory::GetFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<String> ret;
    EnumerateFileSystemEntries(path, searchPattern, searchOption, [&](String&& str)
    {
        ret.push_back(std::move(str));
    });

    return ret;
}

CS2CPP_NAMESPACE_END