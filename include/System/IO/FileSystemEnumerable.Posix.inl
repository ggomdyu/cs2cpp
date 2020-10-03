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

template <typename Pred>
void InternalEnumerateAllDirectories(const char* path, const char* searchPattern, uint8_t filterType, const Pred& handler)
{
    std::deque<std::string> directories(1, path);

    while (!directories.empty())
    {
        std::string currentPath = std::move(directories.front());
        directories.pop_front();

        auto dir = std::unique_ptr<DIR, decltype(&closedir)>(opendir(currentPath.data()), closedir);
        if (dir == nullptr)
        {
            continue;
        }

        struct dirent* ent;
        while ((ent = readdir(dir.get())) != nullptr)
        {
            if (ent->d_type & DT_DIR)
            {
                // Ignore the . and ..
                if ((ent->d_namlen == 1 && ent->d_name[0] == '.') ||
                    (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
                {
                    continue;
                }

                directories.push_back(PosixPath::Combine(currentPath, {ent->d_name, ent->d_namlen}));
            }

            if (ent->d_type & filterType)
            {
                // Ignore the file that the filename doesn't matched with wildcards.
                if (fnmatch(searchPattern, ent->d_name, FNM_PATHNAME) != 0)
                {
                    continue;
                }

                auto utf8FullPath = PosixPath::Combine(currentPath, {ent->d_name, ent->d_namlen});
                auto utf16FullPath = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(utf8FullPath.data()),
                    utf8FullPath.length());
                if (!utf16FullPath)
                {
                    continue;
                }

                if constexpr (std::is_same_v<typename FunctionTraits<Pred>::Return, bool>)
                {
                    if (!handler(std::move(utf16FullPath.value())))
                    {
                        return;
                    }
                }
                else
                {
                    handler(std::move(utf16FullPath.value()));
                }
            }
        }
    }
}

template <typename Pred>
void InternalEnumerateTopDirectoryOnly(const char* path, const char* searchPattern, uint8_t filterType, const Pred& handler)
{
    auto dir = std::unique_ptr<DIR, decltype(&closedir)>(opendir(path), closedir);
    if (dir == nullptr)
    {
        return;
    }

    struct dirent* ent;
    while ((ent = readdir(dir.get())) != nullptr)
    {
        if (!(ent->d_type & filterType))
        {
            continue;
        }

        // Ignore the . and ..
        if ((ent->d_namlen == 1 && ent->d_name[0] == '.') ||
            (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
        {
            continue;
        }

        // Ignore the file that the filename doesn't matched with wildcards.
        if (fnmatch(searchPattern, ent->d_name, FNM_PATHNAME) != 0)
        {
            continue;
        }

        auto utf8FullPath = PosixPath::Combine(path, {ent->d_name, ent->d_namlen});
        auto utf16FullPath = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(utf8FullPath.data()),
            utf8FullPath.length());
        if (!utf16FullPath)
        {
            continue;
        }

        if constexpr (std::is_same_v<typename FunctionTraits<Pred>::Return, bool>)
        {
            if (!handler(std::move(utf16FullPath.value())))
            {
                break;
            }
        }
        else
        {
            handler(std::move(utf16FullPath.value()));
        }
    }
}

}

template <typename Pred>
void FileSystemEnumerable::EnumerateDirectories(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& handler)
{
    std::array<char, 2048> utf8Path{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), path.length() * sizeof(path[0])},
        {reinterpret_cast<std::byte*>(utf8Path.data()), utf8Path.size()}))
    {
        return;
    }

    std::array<char, 2048> utf8SearchPattern{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(searchPattern.data()), searchPattern.length() * sizeof(searchPattern[0])},
        {reinterpret_cast<std::byte*>(utf8SearchPattern.data()), utf8SearchPattern.size()}))
    {
        return;
    }

    using namespace detail::filesystem_enumerable;
    if (searchOption == SearchOption::AllDirectories)
    {
        InternalEnumerateAllDirectories(utf8Path.data(), utf8SearchPattern.data(), DT_DIR, handler);
    }
    else
    {
        InternalEnumerateTopDirectoryOnly(utf8Path.data(), utf8SearchPattern.data(), DT_DIR, handler);
    }
}

template <typename Pred>
void FileSystemEnumerable::EnumerateFiles(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& handler)
{
    std::array<char, 2048> utf8Path{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), path.length() * sizeof(path[0])},
        {reinterpret_cast<std::byte*>(utf8Path.data()), utf8Path.size()}))
    {
        return;
    }

    std::array<char, 2048> utf8SearchPattern{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(searchPattern.data()), searchPattern.length() * sizeof(searchPattern[0])},
        {reinterpret_cast<std::byte*>(utf8SearchPattern.data()), utf8SearchPattern.size()}))
    {
        return;
    }

    using namespace detail::filesystem_enumerable;
    if (searchOption == SearchOption::AllDirectories)
    {
        InternalEnumerateAllDirectories(utf8Path.data(), utf8SearchPattern.data(), DT_REG, handler);
    }
    else
    {
        InternalEnumerateTopDirectoryOnly(utf8Path.data(), utf8SearchPattern.data(), DT_REG, handler);
    }
}

template <typename Pred>
void FileSystemEnumerable::EnumerateFileSystemEntries(std::u16string_view path, std::u16string_view searchPattern, SearchOption searchOption, const Pred& handler)
{
    std::array<char, 2048> utf8Path{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), path.length() * sizeof(path[0])},
        {reinterpret_cast<std::byte*>(utf8Path.data()), utf8Path.size()}))
    {
        return;
    }

    std::array<char, 2048> utf8SearchPattern{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(searchPattern.data()), searchPattern.length() * sizeof(searchPattern[0])},
        {reinterpret_cast<std::byte*>(utf8SearchPattern.data()), utf8SearchPattern.size()}))
    {
        return;
    }

    using namespace detail::filesystem_enumerable;
    if (searchOption == SearchOption::AllDirectories)
    {
        InternalEnumerateAllDirectories(utf8Path.data(), utf8SearchPattern.data(), DT_DIR | DT_REG, handler);
    }
    else
    {
        InternalEnumerateTopDirectoryOnly(utf8Path.data(), utf8SearchPattern.data(), DT_DIR | DT_REG, handler);
    }
}

CS2CPP_NAMESPACE_END