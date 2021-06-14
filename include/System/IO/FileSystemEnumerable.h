#pragma once

#include <string_view>

CS2CPP_NAMESPACE_BEGIN

enum class SearchOption
{
    TopDirectoryOnly,
    AllDirectories,
};

class FileSystemEnumerable
{
public:
    FileSystemEnumerable() = delete;

public:
    template <typename F>
    static void EnumerateDirectories(std::u16string_view path, const F& callback);
    template <typename F>
    static void EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, const F& callback);
    template <typename F>
    static void EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback);
    template <typename F>
    static void EnumerateFiles(std::u16string_view path, const F& callback);
    template <typename F>
    static void EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, const F& callback);
    template <typename F>
    static void EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback);
    template <typename F>
    static void EnumerateFileSystemEntries(std::u16string_view path, const F& callback);
    template <typename F>
    static void EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, const F& callback);
    template <typename F>
    static void EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback);
};

template <typename F>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, const F& callback)
{
    FileSystemEnumerable::EnumerateDirectories(path, u"*", callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, const F& callback)
{
    FileSystemEnumerable::EnumerateDirectories(path, searchPattern, SearchOption::TopDirectoryOnly, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, const F& callback)
{
    FileSystemEnumerable::EnumerateFiles(path, u"*", callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, const F& callback)
{
    FileSystemEnumerable::EnumerateFiles(path, searchPattern, SearchOption::TopDirectoryOnly, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, const F& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(path, u"*", callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, const F& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, SearchOption::TopDirectoryOnly, callback);
}

CS2CPP_NAMESPACE_END

#if CS2CPP_PLATFORM_WINDOWS
#    include "FileSystemEnumerable.Windows.inl"
#elif CS2CPP_PLATFORM_MACOS
#    include "FileSystemEnumerable.Posix.inl"
#endif