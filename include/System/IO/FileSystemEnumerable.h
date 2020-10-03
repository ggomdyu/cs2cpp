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
    template <typename Pred>
    static void EnumerateDirectories(std::u16string_view path, const Pred& callback);
    template <typename Pred>
    static void EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, const Pred& callback);
    template <typename Pred>
    static void EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback);
    template <typename Pred>
    static void EnumerateFiles(std::u16string_view path, const Pred& callback);
    template <typename Pred>
    static void EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, const Pred& callback);
    template <typename Pred>
    static void EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback);
    template <typename Pred>
    static void EnumerateFileSystemEntries(std::u16string_view path, const Pred& callback);
    template <typename Pred>
    static void EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, const Pred& callback);
    template <typename Pred>
    static void EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback);
};

template <typename Pred>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, const Pred& callback)
{
    FileSystemEnumerable::EnumerateDirectories(path, u"*", callback);
}

template <typename Pred>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, const Pred& callback)
{
    FileSystemEnumerable::EnumerateDirectories(path, searchPattern, SearchOption::TopDirectoryOnly, callback);
}

template <typename Pred>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, const Pred& callback)
{
    FileSystemEnumerable::EnumerateFiles(path, u"*", callback);
}

template <typename Pred>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, const Pred& callback)
{
    FileSystemEnumerable::EnumerateFiles(path, searchPattern, SearchOption::TopDirectoryOnly, callback);
}

template <typename Pred>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, const Pred& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(path, u"*", callback);
}

template <typename Pred>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, const Pred& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, SearchOption::TopDirectoryOnly, callback);
}

CS2CPP_NAMESPACE_END

#if CS2CPP_PLATFORM_WINDOWS
#    include "FileSystemEnumerable.Windows.inl"
#elif CS2CPP_PLATFORM_MACOS
#    include "FileSystemEnumerable.Posix.inl"
#endif