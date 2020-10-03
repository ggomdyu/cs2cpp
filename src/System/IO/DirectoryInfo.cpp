#include "System/IO/DirectoryInfo.h"
#include "System/IO/Directory.h"

CS2CPP_NAMESPACE_BEGIN

DirectoryInfo::DirectoryInfo(std::u16string_view path) :
    _fullPath(Path::GetFullPath(path))
{
}

DirectoryInfo::DirectoryInfo(String path, FullPathTag) :
    _fullPath(std::move(path))
{
}

bool DirectoryInfo::IsExists() const
{
    return Directory::Exists(&_fullPath[0]);
}

std::u16string_view DirectoryInfo::GetName() const noexcept
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

std::vector<String> DirectoryInfo::GetDirectories(std::u16string_view searchPattern, SearchOption searchOption) const
{
    return Directory::GetDirectories(&_fullPath[0], searchPattern, searchOption);
}

std::vector<String> DirectoryInfo::GetFiles(std::u16string_view searchPattern, SearchOption searchOption) const
{
    return Directory::GetFiles(&_fullPath[0], searchPattern, searchOption);
}

std::vector<String> DirectoryInfo::GetFileSystemEntries(std::u16string_view searchPattern, SearchOption searchOption) const
{
    return Directory::GetFileSystemEntries(&_fullPath[0], searchPattern, searchOption);
}

void DirectoryInfo::Create() const
{
    Directory::CreateDirectory(&_fullPath[0]);
}

DirectoryInfo DirectoryInfo::CreateSubdirectory(std::u16string_view path) const
{
    const auto combinedPath = Path::Combine(_fullPath, path);
    return Directory::CreateDirectory(combinedPath);
}

bool DirectoryInfo::MoveTo(std::u16string_view destDirName) const
{
    return Directory::Move(&_fullPath[0], destDirName);
}

bool DirectoryInfo::Delete(bool recursive) const
{
    return Directory::Delete(&_fullPath[0], recursive);
}

const String& DirectoryInfo::ToString() const noexcept
{
    return _fullPath;
}

CS2CPP_NAMESPACE_END