#include "System/IO/DirectoryInfo.h"
#include "System/IO/Directory.h"

CS2CPP_NAMESPACE_BEGIN

DirectoryInfo::DirectoryInfo(std::u16string_view path) :
    _fullPath(Path::GetFullPath(path))
{
}

DirectoryInfo::DirectoryInfo(std::u16string path, FullPathTag) :
    _fullPath(std::move(path))
{
}

bool DirectoryInfo::IsExists() const
{
    return Directory::Exists(&_fullPath[0]);
}

std::u16string DirectoryInfo::GetName() const noexcept
{
    return Path::GetDirectoryName(_fullPath);
}

DirectoryInfo DirectoryInfo::GetParent() const
{
    return Directory::GetParent(_fullPath);
}

DirectoryInfo DirectoryInfo::GetRoot() const
{
    return DirectoryInfo(Directory::GetDirectoryRoot(_fullPath), FullPathTag{});
}

std::vector<std::u16string> DirectoryInfo::GetDirectories(std::u16string_view searchPattern, SearchOption searchOption) const
{
    return Directory::GetDirectories(&_fullPath[0], searchPattern, searchOption);
}

std::vector<std::u16string> DirectoryInfo::GetFiles(std::u16string_view searchPattern, SearchOption searchOption) const
{
    return Directory::GetFiles(&_fullPath[0], searchPattern, searchOption);
}

std::vector<std::u16string> DirectoryInfo::GetFileSystemEntries(std::u16string_view searchPattern, SearchOption searchOption) const
{
    return Directory::GetFileSystemEntries(&_fullPath[0], searchPattern, searchOption);
}

void DirectoryInfo::Create()
{
    Directory::CreateDirectory(&_fullPath[0]);
}

DirectoryInfo DirectoryInfo::CreateSubdirectory(std::u16string_view path) const
{
    auto combinedPath = Path::Combine(_fullPath, path);
    return Directory::CreateDirectory(combinedPath);
}

bool DirectoryInfo::MoveTo(std::u16string_view destDirName)
{
    return Directory::Move(&_fullPath[0], destDirName);
}

bool DirectoryInfo::Delete(bool recursive)
{
    return Directory::Delete(&_fullPath[0], recursive);
}

const std::u16string& DirectoryInfo::ToString() const noexcept
{
    return _fullPath;
}

CS2CPP_NAMESPACE_END
