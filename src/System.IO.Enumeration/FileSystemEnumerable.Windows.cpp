#include "System/Windows/Windows.h"
#include "System.IO.Enumeration/FileSystemEnumerable.h"

CS2CPP_NAMESPACE_BEGIN

DirectoryIterator FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    return {path, searchPattern, FILE_ATTRIBUTE_DIRECTORY, searchOption};
}

DirectoryIterator FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption)
{
    return {path, searchPattern, FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL, searchOption};
}

CS2CPP_NAMESPACE_END
