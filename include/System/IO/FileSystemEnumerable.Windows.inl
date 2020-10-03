#pragma once

#include <deque>

#include "System/FunctionTraits.h"
#include "System/Windows/SafeFindHandle.h"
#include "System/Windows/Windows.h"

#include "Path.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::filesystem_enumerable
{

template <typename Pred>
void InternalEnumerateAllDirectories(std::u16string_view path, std::u16string_view searchPattern, DWORD filterType, const Pred& callback)
{
    std::deque<String> directories(1, Path::Combine(path, searchPattern));

    while (!directories.empty())
    {
        const auto& currentPath = directories.front();

        WIN32_FIND_DATAW findData;
        const SafeFindHandle handle(FindFirstFileW(reinterpret_cast<const wchar_t*>(&currentPath[0]), &findData));
        if (handle == nullptr)
        {
            break;
        }

        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Ignore the . and ..
                if (findData.cFileName[0] == L'.' && (findData.cFileName[1] == L'\0' || (findData.cFileName[1] == L'.'
                    && findData.cFileName[2] == L'\0')))
                {
                    continue;
                }

                auto newPath = Path::Combine(currentPath.Substring(0, currentPath.Length() -
                    static_cast<int32_t>(searchPattern.size())), reinterpret_cast<const char16_t*>(findData.cFileName));
                newPath += Path::DirectorySeparatorChar;
                newPath += searchPattern;

                directories.push_back(std::move(newPath));
            }

            if (findData.dwFileAttributes & filterType)
            {
                auto newPath = Path::Combine(currentPath.Substring(0, currentPath.Length() - searchPattern.size()),
                    reinterpret_cast<const char16_t*>(findData.cFileName));
                if constexpr (std::is_same_v<typename FunctionTraits<Pred>::Return, bool>)
                {
                    if (!callback(std::move(newPath)))
                    {
                        return;
                    }
                }
                else
                {
                    callback(std::move(newPath));
                }
            }
        } while (FindNextFileW(handle, &findData) == TRUE);

        directories.pop_front();
    }
}

template <typename Pred>
void InternalEnumerateTopDirectoryOnly(std::u16string_view path, std::u16string_view searchPattern, DWORD filterType, const Pred& callback)
{
    const auto combinedPath = Path::Combine(path, searchPattern);

    WIN32_FIND_DATAW findData;
    const SafeFindHandle handle(FindFirstFileW(reinterpret_cast<const wchar_t*>(&combinedPath[0]), &findData));
    if (handle == nullptr)
    {
        return;
    }

    do
    {
        if (findData.dwFileAttributes & filterType)
        {
            // Ignore the . and ..
            if (findData.cFileName[0] == L'.' && (findData.cFileName[1] == L'\0' || (findData.cFileName[1] == L'.'
                && findData.cFileName[2] == L'\0')))
            {
                continue;
            }

            auto newPath = Path::Combine(path, reinterpret_cast<const char16_t*>(findData.cFileName));
            if constexpr (std::is_same_v<typename FunctionTraits<Pred>::Return, bool>)
            {
                if (!callback(std::move(newPath)))
                {
                    return;
                }
            }
            else
            {
                callback(std::move(newPath));
            }
        }
    } while (FindNextFileW(handle, &findData) == TRUE);
}

}

template <typename Pred>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback)
{
    if (searchOption == SearchOption::AllDirectories)
    {
        detail::filesystem_enumerable::InternalEnumerateAllDirectories(path, searchPattern,
            FILE_ATTRIBUTE_DIRECTORY, callback);
    }
    else
    {
        detail::filesystem_enumerable::InternalEnumerateTopDirectoryOnly(path, searchPattern,
            FILE_ATTRIBUTE_DIRECTORY, callback);
    }
}

template <typename Pred>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback)
{
    if (searchOption == SearchOption::AllDirectories)
    {
        detail::filesystem_enumerable::InternalEnumerateAllDirectories(path, searchPattern,
            FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL, callback);
    }
    else
    {
        detail::filesystem_enumerable::InternalEnumerateTopDirectoryOnly(path, searchPattern,
            FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL, callback);
    }
}

template <typename Pred>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& callback)
{
    if (searchOption == SearchOption::AllDirectories)
    {
        detail::filesystem_enumerable::InternalEnumerateAllDirectories(path, searchPattern,
            std::numeric_limits<DWORD>::max(), callback);
    }
    else
    {
        detail::filesystem_enumerable::InternalEnumerateTopDirectoryOnly(path, searchPattern,
            std::numeric_limits<DWORD>::max(), callback);
    }
}

CS2CPP_NAMESPACE_END