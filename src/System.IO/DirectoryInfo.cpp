#include "System.IO/DirectoryInfo.h"
#include "System.IO/Directory.h"

CS2CPP_NAMESPACE_BEGIN

DirectoryInfo::DirectoryInfo(std::u16string_view path) :
    fullPath_(Path::GetFullPath(path))
{
}

DirectoryInfo::DirectoryInfo(std::u16string path, FullPathTag) :
    fullPath_(std::move(path))
{
}

bool DirectoryInfo::IsExists() const
{
    return Directory::Exists(&fullPath_[0]);
}

std::u16string DirectoryInfo::GetName() const noexcept
{
    return Path::GetDirectoryName(fullPath_);
}

std::optional<DirectoryInfo> DirectoryInfo::GetParent() const
{
    return Directory::GetParent(fullPath_);
}

DirectoryInfo DirectoryInfo::GetRoot() const
{
    return DirectoryInfo(Directory::GetDirectoryRoot(fullPath_), FullPathTag{});
}

std::vector<std::u16string> DirectoryInfo::GetDirectories(std::u16string_view searchPattern, SearchOption searchOption) const
{
    return Directory::GetDirectories(&fullPath_[0], searchPattern, searchOption);
}

std::vector<std::u16string> DirectoryInfo::GetFiles(std::u16string_view searchPattern, SearchOption searchOption) const
{
    return Directory::GetFiles(&fullPath_[0], searchPattern, searchOption);
}

std::vector<std::u16string> DirectoryInfo::GetFileSystemEntries(std::u16string_view searchPattern, SearchOption searchOption) const
{
    return Directory::GetFileSystemEntries(&fullPath_[0], searchPattern, searchOption);
}

void DirectoryInfo::Create()
{
    Directory::CreateDirectory(&fullPath_[0]);
}

std::optional<DirectoryInfo> DirectoryInfo::CreateSubdirectory(std::u16string_view path) const
{
    auto combinedPath = Path::Combine(fullPath_, path);
    return Directory::CreateDirectory(combinedPath);
}

bool DirectoryInfo::MoveTo(std::u16string_view destDirName)
{
    return Directory::Move(&fullPath_[0], destDirName);
}

bool DirectoryInfo::Delete(bool recursive)
{
    return Directory::Delete(&fullPath_[0], recursive);
}

std::u16string_view DirectoryInfo::ToString() const noexcept
{
    return fullPath_;
}

CS2CPP_NAMESPACE_END
