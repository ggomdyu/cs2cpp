#pragma once

#include <optional>
#include <vector>

#include "System/DateTime.h"

#include "DirectoryInfo.h"

CS2CPP_NAMESPACE_BEGIN

class Directory final
{
public:
    Directory() = delete;

public:
    static DirectoryInfo CreateDirectory(std::u16string_view path);
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
    [[nodiscard]] static std::optional<DateTime> GetCreationTime(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetCreationTimeUtc(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetLastAccessTime(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetLastAccessTimeUtc(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetLastWriteTime(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetLastWriteTimeUtc(std::u16string_view path);
    [[nodiscard]] static String GetCurrentDirectory();
    [[nodiscard]] static String GetDirectoryRoot(std::u16string_view path);
    [[nodiscard]] static std::vector<String> GetLogicalDrives();
    [[nodiscard]] static DirectoryInfo GetParent(std::u16string_view path);
    [[nodiscard]] static std::vector<String> GetDirectories(std::u16string_view path, std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly);
    [[nodiscard]] static std::vector<String> GetFiles(std::u16string_view path, std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly);
    [[nodiscard]] static std::vector<String> GetFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly);
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

private:
    static bool InternalCreateDirectory(std::u16string_view path);
};

template <typename Pred>
void Directory::EnumerateDirectories(std::u16string_view path, const Pred& callback)
{
    return FileSystemEnumerable::EnumerateDirectories(path, callback);
}

template <typename Pred>
void Directory::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, const Pred& callback)
{
    return FileSystemEnumerable::EnumerateDirectories(path, searchPattern, callback);
}

template <typename Pred>
void Directory::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback)
{
    return FileSystemEnumerable::EnumerateDirectories(path, searchPattern, searchOption, callback);
}

template <typename Pred>
void Directory::EnumerateFiles(std::u16string_view path, const Pred& callback)
{
    return FileSystemEnumerable::EnumerateFiles(path, callback);
}

template <typename Pred>
void Directory::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, const Pred& callback)
{
    return FileSystemEnumerable::EnumerateFiles(path, searchPattern, callback);
}

template <typename Pred>
void Directory::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback)
{
    return FileSystemEnumerable::EnumerateFiles(path, searchPattern, searchOption, callback);
}

template <typename Pred>
void Directory::EnumerateFileSystemEntries(std::u16string_view path, const Pred& callback)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, callback);
}

template <typename Pred>
void Directory::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, const Pred& callback)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, callback);
}

template <typename Pred>
void Directory::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, searchOption, callback);
}

CS2CPP_NAMESPACE_END