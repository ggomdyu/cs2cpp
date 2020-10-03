#pragma once

#include <deque>
#include <shlwapi.h>

#include "System/FunctionTraits.h"
#include "System/Windows/SafeFindHandle.h"
#include "System/Windows/Windows.h"

#pragma comment(lib, "Shlwapi.lib")

CS2CPP_NAMESPACE_BEGIN

inline bool IsMatchWithPattern(std::u16string_view path, std::u16string_view searchPattern)
{
    if (searchPattern == u"*")
    {
        return true;
    }

    return PathMatchSpecW(reinterpret_cast<LPCWSTR>(path.data()), reinterpret_cast<LPCWSTR>(searchPattern.data())) == TRUE;
}

template <typename F>
void InternalEnumerateAllDirectories(std::u16string_view path, std::u16string_view searchPattern, DWORD filter, const F& callback)
{
    std::deque<std::u16string> directories(1, Path::Combine(path, u"*"));

    while (!directories.empty())
    {
        const auto& currentPath = directories.front();

        WIN32_FIND_DATAW findData;
        SafeFindHandle handle(FindFirstFileW(reinterpret_cast<LPCWSTR>(&currentPath[0]), &findData));
        if (!handle)
        {
            break;
        }

        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Ignore the . and ..
                if (findData.cFileName[0] == L'.' &&
                   (findData.cFileName[1] == L'\0' || (findData.cFileName[1] == L'.' && findData.cFileName[2] == L'\0')))
                {
                    continue;
                }

                directories.push_back(Path::Combine(currentPath.substr(0, currentPath.length() - 1), reinterpret_cast<const char16_t*>(findData.cFileName), u"*"));
            }

            if (findData.dwFileAttributes & filter)
            {
                auto newPath = Path::Combine(currentPath.substr(0, currentPath.length() - 1), reinterpret_cast<const char16_t*>(findData.cFileName));
                if (!IsMatchWithPattern(newPath, searchPattern))
                {
                    continue;
                }

                if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
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

template <typename F>
void InternalEnumerateTopDirectoryOnly(std::u16string_view path, std::u16string_view searchPattern, DWORD filter, const F& callback)
{
    auto combinedPath = Path::Combine(path, searchPattern);

    WIN32_FIND_DATAW findData;
    auto handle = SafeFindHandle(FindFirstFileW(reinterpret_cast<LPCWSTR>(&combinedPath[0]), &findData));
    if (!handle)
    {
        return;
    }

    do
    {
        if (findData.dwFileAttributes & filter)
        {
            // Ignore the . and ..
            if (findData.cFileName[0] == L'.' &&
               (findData.cFileName[1] == L'\0' || (findData.cFileName[1] == L'.' && findData.cFileName[2] == L'\0')))
            {
                continue;
            }

            auto newPath = Path::Combine(path, reinterpret_cast<const char16_t*>(findData.cFileName));
            if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
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

template <typename F>
void InternalEnumerate(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, DWORD filter, const F& callback)
{
    if (searchOption == SearchOption::AllDirectories)
    {
        InternalEnumerateAllDirectories(path, searchPattern, filter, callback);
    }
    else
    {
        InternalEnumerateTopDirectoryOnly(path, searchPattern, filter, callback);
    }
}

template <typename F>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    InternalEnumerate(path, searchPattern, searchOption, FILE_ATTRIBUTE_DIRECTORY, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    InternalEnumerate(path, searchPattern, searchOption, FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    InternalEnumerate(path, searchPattern, searchOption, std::numeric_limits<DWORD>::max(), callback);
}

CS2CPP_NAMESPACE_END