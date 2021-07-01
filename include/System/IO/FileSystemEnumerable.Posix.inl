#pragma once

#include <array>
#include <deque>
#include <dirent.h>
#include <fnmatch.h>
#include <memory>
#include <unistd.h>

#include "System/FunctionTraits.h"
#include "System/Text/Encoding.h"

#include "Path.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::filesystem_enumerable
{

template <typename F>
void InternalEnumerateAllDirectories(const char* path, const char* searchPattern, uint8_t filterType, const F& callback)
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

            if (ent->d_type & filterType)
            {
                // Ignore the file that the filename doesn't matched with wildcards.
                if (fnmatch(searchPattern, ent->d_name, FNM_PATHNAME) != 0)
                    continue;

                auto utf8Path = PosixPath::Combine(currentPath, {ent->d_name, ent->d_namlen});
                auto utf16Path = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(utf8Path.data()),
                    utf8Path.length());
                if (!utf16Path)
                    continue;

                if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
                {
                    if (!callback(std::move(utf16Path.value())))
                        return;
                }
                else
                    callback(std::move(utf16Path.value()));
            }
        }
    }
}

template <typename F>
void InternalEnumerateTopDirectoryOnly(const char* path, const char* searchPattern, uint8_t filterType, const F& callback)
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

        if (!(ent->d_type & filterType))
            continue;

        // Ignore the . and ..
        if ((ent->d_namlen == 1 && ent->d_name[0] == '.')
            || (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
            continue;

        // Ignore the file that the filename doesn't matched with wildcards.
        if (fnmatch(searchPattern, ent->d_name, FNM_PATHNAME) != 0)
            continue;

        auto utf8Path = PosixPath::Combine(path, {ent->d_name, ent->d_namlen});
        auto utf16Path = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(utf8Path.data()),
            utf8Path.length());
        if (!utf16Path)
            continue;

        if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
        {
            if (!callback(std::move(utf16Path.value())))
                break;
        }
        else
            callback(std::move(utf16Path.value()));
    }
}

}

template <typename F>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    using namespace detail::filesystem_enumerable;

    std::array<char, 2048> utf8Path{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), path.length() * sizeof(path[0])},
        {reinterpret_cast<std::byte*>(utf8Path.data()), utf8Path.size()}))
        return;

    std::array<char, 2048> utf8SearchPattern{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(searchPattern.data()), searchPattern.length() * sizeof(searchPattern[0])},
        {reinterpret_cast<std::byte*>(utf8SearchPattern.data()), utf8SearchPattern.size()}))
        return;

    if (searchOption == SearchOption::AllDirectories)
        InternalEnumerateAllDirectories(utf8Path.data(), utf8SearchPattern.data(), DT_DIR, callback);
    else
        InternalEnumerateTopDirectoryOnly(utf8Path.data(), utf8SearchPattern.data(), DT_DIR, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    using namespace detail::filesystem_enumerable;

    std::array<char, 2048> utf8Path{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), path.length() * sizeof(path[0])},
        {reinterpret_cast<std::byte*>(utf8Path.data()), utf8Path.size()}))
        return;

    std::array<char, 2048> utf8SearchPattern{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(searchPattern.data()), searchPattern.length() * sizeof(searchPattern[0])},
        {reinterpret_cast<std::byte*>(utf8SearchPattern.data()), utf8SearchPattern.size()}))
        return;

    if (searchOption == SearchOption::AllDirectories)
        InternalEnumerateAllDirectories(utf8Path.data(), utf8SearchPattern.data(), DT_REG, callback);
    else
        InternalEnumerateTopDirectoryOnly(utf8Path.data(), utf8SearchPattern.data(), DT_REG, callback);
}

template <typename F>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const F& callback)
{
    using namespace detail::filesystem_enumerable;

    std::array<char, 2048> utf8Path{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), path.length() * sizeof(path[0])},
        {reinterpret_cast<std::byte*>(utf8Path.data()), utf8Path.size()}))
        return;

    std::array<char, 2048> utf8SearchPattern{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(searchPattern.data()), searchPattern.length() * sizeof(searchPattern[0])},
        {reinterpret_cast<std::byte*>(utf8SearchPattern.data()), utf8SearchPattern.size()}))
        return;

    if (searchOption == SearchOption::AllDirectories)
        InternalEnumerateAllDirectories(utf8Path.data(), utf8SearchPattern.data(), DT_DIR | DT_REG, callback);
    else
        InternalEnumerateTopDirectoryOnly(utf8Path.data(), utf8SearchPattern.data(), DT_DIR | DT_REG, callback);
}

CS2CPP_NAMESPACE_END