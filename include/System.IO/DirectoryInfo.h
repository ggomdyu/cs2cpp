#pragma once

#include <vector>
#include <optional>

#include "System.IO.Enumeration/FileSystemEnumerable.h"

CS2CPP_NAMESPACE_BEGIN

struct FullPathTag {};

class DirectoryInfo final
{
public:
    explicit DirectoryInfo(std::u16string_view path);
    DirectoryInfo(std::u16string path, FullPathTag);

public:
    void Create();
    bool Delete(bool recursive = false);
    bool MoveTo(std::u16string_view destDirName);
    std::optional<DirectoryInfo> CreateSubdirectory(std::u16string_view path) const;
    bool IsExists() const;
    std::u16string GetName() const noexcept;
    std::vector<std::u16string> GetDirectories(std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly) const;
    std::vector<std::u16string> GetFiles(std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly) const;
    std::vector<std::u16string> GetFileSystemEntries(std::u16string_view searchPattern = u"*", SearchOption searchOption = SearchOption::TopDirectoryOnly) const;
    std::optional<DirectoryInfo> GetParent() const;
    DirectoryInfo GetRoot() const;
    std::u16string_view ToString() const noexcept;
    template <typename F>
    void EnumerateDirectories(const F& callback) const;
    template <typename F>
    void EnumerateDirectories(std::u16string_view searchPattern, const F& callback) const;
    template <typename F>
    void EnumerateDirectories(std::u16string_view searchPattern, SearchOption searchOption, const F& callback) const;
    template <typename F>
    void EnumerateFiles(const F& callback) const;
    template <typename F>
    void EnumerateFiles(std::u16string_view searchPattern, const F& callback) const;
    template <typename F>
    void EnumerateFiles(std::u16string_view searchPattern, SearchOption searchOption, const F& callback) const;
    template <typename F>
    void EnumerateFileSystemEntries(const F& callback);
    template <typename F>
    void EnumerateFileSystemEntries(std::u16string_view searchPattern, const F& callback);
    template <typename F>
    void EnumerateFileSystemEntries(std::u16string_view searchPattern, SearchOption searchOption, const F& callback);

private:
    std::u16string fullPath_;
};

template <typename F>
void DirectoryInfo::EnumerateDirectories(const F& callback) const
{
    FileSystemEnumerable::EnumerateDirectories(&fullPath_[0], callback);
}

template <typename F>
void DirectoryInfo::EnumerateDirectories(std::u16string_view searchPattern, const F& callback) const
{
    FileSystemEnumerable::EnumerateDirectories(&fullPath_[0], searchPattern, callback);
}

template <typename F>
void DirectoryInfo::EnumerateDirectories(std::u16string_view searchPattern, SearchOption searchOption, const F& callback) const
{
    FileSystemEnumerable::EnumerateDirectories(&fullPath_[0], searchPattern, searchOption, callback);
}

template <typename F>
void DirectoryInfo::EnumerateFiles(const F& callback) const
{
    FileSystemEnumerable::EnumerateFiles(&fullPath_[0], callback);
}

template <typename F>
void DirectoryInfo::EnumerateFiles(std::u16string_view searchPattern, const F& callback) const
{
    FileSystemEnumerable::EnumerateFiles(&fullPath_[0], searchPattern, callback);
}

template <typename F>
void DirectoryInfo::EnumerateFiles(std::u16string_view searchPattern, SearchOption searchOption, const F& callback) const
{
    FileSystemEnumerable::EnumerateFiles(&fullPath_[0], searchPattern, searchOption, callback);
}

template <typename F>
void DirectoryInfo::EnumerateFileSystemEntries(const F& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(&fullPath_[0], callback);
}

template <typename F>
void DirectoryInfo::EnumerateFileSystemEntries(std::u16string_view searchPattern, const F& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(&fullPath_[0], searchPattern, callback);
}

template <typename F>
void DirectoryInfo::EnumerateFileSystemEntries(std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(&fullPath_[0], searchPattern, searchOption, callback);
}

CS2CPP_NAMESPACE_END
