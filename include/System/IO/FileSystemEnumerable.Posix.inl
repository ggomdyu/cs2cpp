#pragma once

#include <array>
#include <deque>
#include <dirent.h>
#include <fnmatch.h>
#include <memory>
#include <unistd.h>

#include "System/FunctionTraits.h"
#include "System/IO/Path.h"
#include "System/Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::filesystem_enumerable
{

template <typename F>
void InternalEnumerateAllDirectories(const char* path, const char* searchPattern, uint8_t filter, const F& callback)
{
    std::deque<std::string> directories(1, path);

    while (!directories.empty())
    {
        std::string currentPath = std::move(directories.front());
        directories.pop_front();

        auto dir = std::unique_ptr<DIR, decltype(&closedir)>(opendir(currentPath.data()), closedir);
        if (!dir)
            continue;

        struct dirent* ent;
        while (true)
        {
            ent = readdir(dir.get());
            if (!ent)
                break;

            if (ent->d_type & DT_DIR)
            {
                // Ignore the . and ..
                if ((ent->d_namlen == 1 && ent->d_name[0] == '.')
                    || (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
                    continue;

                directories.push_back(PosixPath::Combine(currentPath, {ent->d_name, ent->d_namlen}));
            }

            if (ent->d_type & filter)
            {
                // Ignore the file that the filename doesn't matched with wildcards.
                auto newPath = (ent->d_type & DT_DIR) ?
                    directories.back() : PosixPath::Combine(currentPath, {ent->d_name, ent->d_namlen});
                if (fnmatch(searchPattern, newPath.data(), 0) != 0)
                    continue;

                auto utf16NewPath = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(newPath.data()),
                    static_cast<int32_t>(newPath.length()));
                if (!utf16NewPath)
                    continue;

                if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
                {
                    if (!callback(std::move(utf16NewPath.value())))
                        return;
                }
                else
                    callback(std::move(utf16NewPath.value()));
            }
        }
    }
}

template <typename F>
void InternalEnumerateTopDirectoryOnly(const char* path, const char* searchPattern, uint8_t filter, const F& callback)
{
    auto dir = std::unique_ptr<DIR, decltype(&closedir)>(opendir(path), closedir);
    if (!dir)
        return;

    struct dirent* ent;
    while (true)
    {
        ent = readdir(dir.get());
        if (!ent)
            break;

        if (!(ent->d_type & filter))
            continue;

        // Ignore the . and ..
        if ((ent->d_namlen == 1 && ent->d_name[0] == '.')
            || (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
            continue;

        // Ignore the file that the filename doesn't matched with wildcards.
        auto newPath = PosixPath::Combine(path, {ent->d_name, ent->d_namlen});
        if (fnmatch(searchPattern, newPath.data(), 0) != 0)
            continue;

        auto utf16NewPath = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(newPath.data()),
            static_cast<int32_t>(newPath.length()));
        if (!utf16NewPath)
            continue;

        if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
        {
            if (!callback(std::move(utf16NewPath.value())))
                break;
        }
        else
            callback(std::move(utf16NewPath.value()));
    }
}

template <typename F>
void InternalEnumerate(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, uint8_t filter, const F& callback)
{
    auto fullPath = Path::GetFullPath(path);

    std::array<char, 4096> utf8FullPath{};
    auto utf8FullPathLen = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(fullPath.data()), sizeof(fullPath[0]) * fullPath.length()},
        {reinterpret_cast<std::byte*>(utf8FullPath.data()), utf8FullPath.size()});
    if (!utf8FullPathLen)
        return;

    std::array<char, 4096> utf8SearchPattern{};
    auto utf8SearchPatternLen = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(searchPattern.data()), sizeof(searchPattern[0]) * searchPattern.length()},
        {reinterpret_cast<std::byte*>(utf8SearchPattern.data()), utf8SearchPattern.size()});
    if (!utf8SearchPatternLen)
        return;

    if (searchOption == SearchOption::AllDirectories)
        InternalEnumerateAllDirectories(utf8FullPath.data(), utf8SearchPattern.data(), filter, callback);
    else
        InternalEnumerateTopDirectoryOnly(utf8FullPath.data(), utf8SearchPattern.data(), filter, callback);
}

}

template <typename F>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    using namespace detail::filesystem_enumerable;
    InternalEnumerate(path, searchPattern, searchOption, DT_DIR, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    using namespace detail::filesystem_enumerable;
    InternalEnumerate(path, searchPattern, searchOption, DT_REG, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    using namespace detail::filesystem_enumerable;
    InternalEnumerate(path, searchPattern, searchOption, DT_DIR | DT_REG, callback);
}

CS2CPP_NAMESPACE_END