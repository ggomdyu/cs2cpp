#pragma once

#include <array>
#include <cstring>
#include <deque>
#include <dirent.h>
#include <fnmatch.h>
#include <memory>
#include <unistd.h>

#include "System/FunctionTraits.h"
#include "System.IO/Path.h"
#include "System.Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

template <typename F>
void InternalEnumerateAllDirectories(const char* path, const char* searchPattern, uint8_t filter, const F& callback)
{
//    std::deque<std::string> directories(1, path);
//
//    while (!directories.empty())
//    {
//        std::string currentPath = std::move(directories.front());
//        directories.pop_front();
//
//        auto dir = std::unique_ptr<DIR, decltype(&closedir)>(opendir(currentPath.data()), closedir);
//        if (!dir)
//        {
//            continue;
//        }
//
//        struct dirent* ent;
//        while (true)
//        {
//            ent = readdir(dir.get());
//            if (!ent)
//            {
//                break;
//            }
//
//#if CS2CPP_PLATFORM_DARWIN
//            std::string_view entryName(ent->d_name, ent->d_namlen);
//#else
//            std::string_view entryName(ent->d_name);
//#endif
//
//            if (ent->d_type & DT_DIR)
//            {
//                // Ignore the . and ..
//                if ((entryName.size() == 1 && entryName[0] == '.') ||
//                    (entryName.size() == 2 && !strcmp(entryName.data(), "..")))
//                {
//                    continue;
//                }
//
//                directories.push_back(std::filesystem::path(currentPath) /= entryName);
//            }
//
//            if (ent->d_type & filter)
//            {
//                // Ignore the file that the filename doesn't match with wildcards.
//                auto newPath = (ent->d_type & DT_DIR) ? directories.back() : (std::filesystem::path(currentPath) /= entryName).string();
//                if (fnmatch(searchPattern, newPath.data(), 0) != 0)
//                {
//                    continue;
//                }
//
//                auto utf16NewPath = Encoding::UTF8().GetString({reinterpret_cast<const std::byte*>(newPath.data()),
//                    static_cast<int32_t>(newPath.length())});
//                if (!utf16NewPath)
//                {
//                    continue;
//                }
//
//                if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
//                {
//                    if (!callback(std::move(utf16NewPath.value())))
//                    {
//                        return;
//                    }
//                }
//                else
//                {
//                    callback(std::move(utf16NewPath.value()));
//                }
//            }
//        }
//    }
}

template <typename F>
void InternalEnumerateTopDirectoryOnly(const char* path, const char* searchPattern, uint8_t filter, const F& callback)
{
//    auto dir = std::unique_ptr<DIR, decltype(&closedir)>(opendir(path), closedir);
//    if (!dir)
//    {
//        return;
//    }
//
//    struct dirent* ent;
//    while (true)
//    {
//        ent = readdir(dir.get());
//        if (!ent)
//        {
//            break;
//        }
//
//        if (!(ent->d_type & filter))
//        {
//            continue;
//        }
//
//#if CS2CPP_PLATFORM_DARWIN
//        std::string_view entryName(ent->d_name, ent->d_namlen);
//#else
//        std::string_view entryName(ent->d_name);
//#endif
//
//        // Ignore the . and ..
//        if ((entryName.length() == 1 && entryName[0] == '.') ||
//            (entryName.length() == 2 && !strcmp(entryName.data(), "..")))
//        {
//            continue;
//        }
//
//        // Ignore the file that the filename doesn't matched with wildcards.
//        auto newPath = std::filesystem::path(path) /= entryName;
//        if (fnmatch(searchPattern, newPath.c_str(), 0) != 0)
//        {
//            continue;
//        }
//
//        if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
//        {
//            if (!callback(std::move(newPath.u16string())))
//            {
//                break;
//            }
//        }
//        else
//        {
//            callback(std::move(newPath.u16string()));
//        }
//    }
}

template <typename F>
void InternalEnumerate(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, uint8_t filter, const F& callback)
{
    auto fullPath = Path::GetFullPath(path);

    auto utf8FullPath = Encoding::UTF8().GetBytes(fullPath);
    if (!utf8FullPath)
    {
        return;
    }

    auto utf8SearchPattern = Encoding::UTF8().GetBytes(searchPattern);
    if (!utf8SearchPattern)
    {
        return;
    }

    if (searchOption == SearchOption::AllDirectories)
    {
        InternalEnumerateAllDirectories(reinterpret_cast<const char*>(utf8FullPath->data()),
            reinterpret_cast<const char*>(utf8SearchPattern->data()), filter, callback);
    }
    else
    {
        InternalEnumerateTopDirectoryOnly(reinterpret_cast<const char*>(utf8FullPath->data()),
            reinterpret_cast<const char*>(utf8SearchPattern->data()), filter, callback);
    }
}

}

template <typename F>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    InternalEnumerate(path, searchPattern, searchOption, DT_DIR, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    InternalEnumerate(path, searchPattern, searchOption, DT_REG, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    InternalEnumerate(path, searchPattern, searchOption, DT_DIR | DT_REG, callback);
}

CS2CPP_NAMESPACE_END