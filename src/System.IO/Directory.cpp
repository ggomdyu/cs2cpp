#include "System/Environment.h"
#include "System.IO/Directory.h"
#include "System.IO/File.h"
#include "System.IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

std::optional<DirectoryInfo> Directory::CreateDirectory(std::u16string_view path)
{
    if (path.empty())
    {
        return std::nullopt;
    }

    std::u16string fullPath = Path::GetFullPath(path);
    if (InternalCreateDirectory(fullPath))
    {
        return DirectoryInfo(std::move(fullPath), FullPathTag{});
    }

    // Maybe the path is invalid because the path contains directories that don't exist.
    if (fullPath.length() > 0 && !Path::IsDirectorySeparator(fullPath[fullPath.length() - 1]))
    {
        fullPath += Path::DirectorySeparatorChar;
    }

    for (size_t i = Path::IsDirectorySeparator(fullPath[0]) ? 1 : 0; i < fullPath.size(); ++i)
    {
        char16_t c = fullPath[i];
        if (Path::IsDirectorySeparator(c))
        {
            fullPath[i] = 0;
            InternalCreateDirectory({&fullPath[0], i + 1});
            fullPath[i] = c;
        }
    }

    // Remove a temporarily added directory separator.
    if (Path::IsDirectorySeparator(fullPath.back()))
    {
        fullPath.pop_back();
    }

    return DirectoryInfo(std::move(fullPath), FullPathTag{});
}

bool Directory::SetCurrentDirectory(std::u16string_view path)
{
    return Environment::SetCurrentDirectory(Path::GetFullPath(path));
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

std::u16string Directory::GetCurrentDirectory()
{
    return Environment::GetCurrentDirectory();
}

std::u16string Directory::GetDirectoryRoot(std::u16string_view path)
{
    return Path::GetPathRoot(Path::GetFullPath(path));
}

std::vector<std::u16string> Directory::GetLogicalDrives()
{
    return Environment::GetLogicalDrives();
}

std::optional<DirectoryInfo> Directory::GetParent(std::u16string_view path)
{
    if (path.empty())
    {
        return std::nullopt;
    }

    std::u16string fullPath = Path::GetFullPath(path);
    return DirectoryInfo(Path::GetDirectoryName(fullPath));
}

std::vector<std::u16string> Directory::GetDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<std::u16string> ret;
    for (std::u16string_view path : EnumerateDirectories(path, searchPattern, searchOption))
    {
        ret.emplace_back(path);
    }

    return ret;
}

std::vector<std::u16string> Directory::GetFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<std::u16string> ret;
    for (std::u16string_view path : EnumerateFiles(path, searchPattern, searchOption))
    {
        ret.emplace_back(path);
    }

    return ret;
}

std::vector<std::u16string> Directory::GetFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    std::vector<std::u16string> ret;
    for (std::u16string_view path : EnumerateFileSystemEntries(path, searchPattern, searchOption))
    {
        ret.emplace_back(path);
    }

    return ret;
}

DirectoryIterator Directory::EnumerateDirectories(std::u16string_view path)
{
    return FileSystemEnumerable::EnumerateDirectories(path);
}

DirectoryIterator Directory::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern)
{
    return FileSystemEnumerable::EnumerateDirectories(path, searchPattern);
}

DirectoryIterator Directory::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    return FileSystemEnumerable::EnumerateDirectories(path, searchPattern, searchOption);
}

DirectoryIterator Directory::EnumerateFiles(std::u16string_view path)
{
    return FileSystemEnumerable::EnumerateFiles(path);
}

DirectoryIterator Directory::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern)
{
    return FileSystemEnumerable::EnumerateFiles(path, searchPattern);
}

DirectoryIterator Directory::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    return FileSystemEnumerable::EnumerateFiles(path, searchPattern, searchOption);
}

DirectoryIterator Directory::EnumerateFileSystemEntries(std::u16string_view path)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path);
}

DirectoryIterator Directory::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern);
}

DirectoryIterator Directory::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, searchOption);
}

CS2CPP_NAMESPACE_END