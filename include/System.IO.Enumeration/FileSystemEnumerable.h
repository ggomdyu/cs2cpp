#pragma once

#include "System.IO.Enumeration/DirectoryIterator.h"

CS2CPP_NAMESPACE_BEGIN

class FileSystemEnumerable final
{
public:
    FileSystemEnumerable() = delete;

public:
    static DirectoryIterator EnumerateDirectories(std::u16string_view path);
    static DirectoryIterator EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern);
    static DirectoryIterator EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption);
    static DirectoryIterator EnumerateFiles(std::u16string_view path);
    static DirectoryIterator EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern);
    static DirectoryIterator EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption);
    static DirectoryIterator EnumerateFileSystemEntries(std::u16string_view path);
    static DirectoryIterator EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern);
    static DirectoryIterator EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption);
};

CS2CPP_NAMESPACE_END
