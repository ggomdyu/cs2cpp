#include "System.IO.Enumeration/FileSystemEnumerable.h"
#include "System.IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

DirectoryIterator FileSystemEnumerable::EnumerateDirectories(std::u16string_view path)
{
    return FileSystemEnumerable::EnumerateDirectories(Path::GetFullPath(path), u"*");
}

DirectoryIterator FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern)
{
    return FileSystemEnumerable::EnumerateDirectories(Path::GetFullPath(path), searchPattern, SearchOption::TopDirectoryOnly);
}

DirectoryIterator FileSystemEnumerable::EnumerateFiles(std::u16string_view path)
{
    return FileSystemEnumerable::EnumerateFiles(Path::GetFullPath(path), u"*");
}

DirectoryIterator FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern)
{
    return FileSystemEnumerable::EnumerateFiles(Path::GetFullPath(path), searchPattern, SearchOption::TopDirectoryOnly);
}

DirectoryIterator FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(Path::GetFullPath(path), u"*");
}

DirectoryIterator FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(Path::GetFullPath(path), searchPattern, SearchOption::TopDirectoryOnly);
}

DirectoryIterator FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    return {path, searchPattern, std::numeric_limits<uint32_t>::max(), searchOption};
}

CS2CPP_NAMESPACE_END
