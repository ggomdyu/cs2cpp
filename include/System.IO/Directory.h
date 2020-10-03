#pragma once

#include <optional>
#include <vector>

#include "System/DateTime.h"
#include "System.IO/DirectoryInfo.h"

CS2CPP_NAMESPACE_BEGIN

class Directory final
{
public:
    Directory() = delete;

public:
    static std::optional<DirectoryInfo> CreateDirectory(std::u16string_view path);
    static bool Delete(std::u16string_view path, bool recursive = false);
    static bool Exists(std::u16string_view path);
    static bool Move(std::u16string_view srcPath, std::u16string_view destPath);
    static bool SetCurrentDirectory(std::u16string_view path);
    static bool SetCreationTime(std::u16string_view path, DateTime creationTime);
    static bool SetCreationTimeUtc(std::u16string_view path, DateTime creationTimeUtc);
    static bool SetLastAccessTime(std::u16string_view path, DateTime lastAccessTime);
    static bool SetLastAccessTimeUtc(std::u16string_view path, DateTime lastAccessTimeUtc);
    static bool SetLastWriteTime(std::u16string_view path, DateTime lastWriteTime);
    static bool SetLastWriteTimeUtc(std::u16string_view path, DateTime lastWriteTimeUtc);
    static std::optional<DateTime> GetCreationTime(std::u16string_view path);
    static std::optional<DateTime> GetCreationTimeUtc(std::u16string_view path);
    static std::optional<DateTime> GetLastAccessTime(std::u16string_view path);
    static std::optional<DateTime> GetLastAccessTimeUtc(std::u16string_view path);
    static std::optional<DateTime> GetLastWriteTime(std::u16string_view path);
    static std::optional<DateTime> GetLastWriteTimeUtc(std::u16string_view path);
    static std::u16string GetCurrentDirectory();
    static std::u16string GetDirectoryRoot(std::u16string_view path);
    static std::vector<std::u16string> GetLogicalDrives();
    static std::optional<DirectoryInfo> GetParent(std::u16string_view path);
    static std::vector<std::u16string> GetDirectories(std::u16string_view path, std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly);
    static std::vector<std::u16string> GetFiles(std::u16string_view path, std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly);
    static std::vector<std::u16string> GetFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly);
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

private:
    static bool InternalCreateDirectory(std::u16string_view path);
};

template <typename F>
void Directory::EnumerateDirectories(std::u16string_view path, const F& callback)
{
    return FileSystemEnumerable::EnumerateDirectories(path, callback);
}

template <typename F>
void Directory::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, const F& callback)
{
    return FileSystemEnumerable::EnumerateDirectories(path, searchPattern, callback);
}

template <typename F>
void Directory::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    return FileSystemEnumerable::EnumerateDirectories(path, searchPattern, searchOption, callback);
}

template <typename F>
void Directory::EnumerateFiles(std::u16string_view path, const F& callback)
{
    return FileSystemEnumerable::EnumerateFiles(path, callback);
}

template <typename F>
void Directory::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, const F& callback)
{
    return FileSystemEnumerable::EnumerateFiles(path, searchPattern, callback);
}

template <typename F>
void Directory::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    return FileSystemEnumerable::EnumerateFiles(path, searchPattern, searchOption, callback);
}

template <typename F>
void Directory::EnumerateFileSystemEntries(std::u16string_view path, const F& callback)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, callback);
}

template <typename F>
void Directory::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, const F& callback)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, callback);
}

template <typename F>
void Directory::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, searchOption, callback);
}

CS2CPP_NAMESPACE_END
