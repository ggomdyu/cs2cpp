#pragma once

#include <vector>

#include "FileSystemEnumerable.h"
#include "System/String.h"

CS2CPP_NAMESPACE_BEGIN

struct FullPathTag
{
};

class DirectoryInfo final
{
public:
    explicit DirectoryInfo(std::u16string_view path);
    DirectoryInfo(String path, FullPathTag);

public:
    void Create() const;
    [[nodiscard]] DirectoryInfo CreateSubdirectory(std::u16string_view path) const;
    bool Delete(bool recursive = false) const;
    [[nodiscard]] bool IsExists() const;
    bool MoveTo(std::u16string_view destDirName) const;
    [[nodiscard]] std::u16string_view GetName() const noexcept;
    [[nodiscard]] DirectoryInfo GetRoot() const;
    [[nodiscard]] DirectoryInfo GetParent() const;
    [[nodiscard]] std::vector<String> GetDirectories(std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly) const;
    [[nodiscard]] std::vector<String> GetFiles(std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly) const;
    [[nodiscard]] std::vector<String> GetFileSystemEntries(std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly) const;
    template <typename Pred>
    void EnumerateDirectories(const Pred& callback) const;
    template <typename Pred>
    void EnumerateDirectories(std::u16string_view searchPattern, const Pred& callback) const;
    template <typename Pred>
    void EnumerateDirectories(std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback) const;
    template <typename Pred>
    void EnumerateFiles(const Pred& callback) const;
    template <typename Pred>
    void EnumerateFiles(std::u16string_view searchPattern, const Pred& callback) const;
    template <typename Pred>
    void EnumerateFiles(std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback) const;
    template <typename Pred>
    void EnumerateFileSystemEntries(const Pred& callback);
    template <typename Pred>
    void EnumerateFileSystemEntries(std::u16string_view searchPattern, const Pred& callback);
    template <typename Pred>
    void EnumerateFileSystemEntries(std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback);
    [[nodiscard]] const String& ToString() const noexcept;

private:
    String _fullPath;
};

template <typename Pred>
void DirectoryInfo::EnumerateDirectories(const Pred& callback) const
{
    FileSystemEnumerable::EnumerateDirectories(&_fullPath[0], callback);
}

template <typename Pred>
void DirectoryInfo::EnumerateDirectories(std::u16string_view searchPattern, const Pred& callback) const
{
    FileSystemEnumerable::EnumerateDirectories(&_fullPath[0], searchPattern, callback);
}

template <typename Pred>
void DirectoryInfo::EnumerateDirectories(std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback) const
{
    FileSystemEnumerable::EnumerateDirectories(&_fullPath[0], searchPattern, searchOption, callback);
}

template <typename Pred>
void DirectoryInfo::EnumerateFiles(const Pred& callback) const
{
    FileSystemEnumerable::EnumerateFiles(&_fullPath[0], callback);
}

template <typename Pred>
void DirectoryInfo::EnumerateFiles(std::u16string_view searchPattern, const Pred& callback) const
{
    FileSystemEnumerable::EnumerateFiles(&_fullPath[0], searchPattern, callback);
}

template <typename Pred>
void DirectoryInfo::EnumerateFiles(std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback) const
{
    FileSystemEnumerable::EnumerateFiles(&_fullPath[0], searchPattern, searchOption, callback);
}

template <typename Pred>
void DirectoryInfo::EnumerateFileSystemEntries(const Pred& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(&_fullPath[0], callback);
}

template <typename Pred>
void DirectoryInfo::EnumerateFileSystemEntries(std::u16string_view searchPattern, const Pred& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(&_fullPath[0], searchPattern, callback);
}

template <typename Pred>
void DirectoryInfo::EnumerateFileSystemEntries(std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(&_fullPath[0], searchPattern, searchOption, callback);
}

CS2CPP_NAMESPACE_END