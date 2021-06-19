#pragma once

#include <gsl/span>
#include <string>

#if CS2CPP_PLATFORM_WINDOWS
#    include "Path.Windows.h"
#elif CS2CPP_SUPPORT_POSIX
#    include "Path.Posix.h"
#endif

CS2CPP_NAMESPACE_BEGIN

class Path final :
    private PlatformPath
{
    friend class Directory;

public:
    Path() = delete;

public:
    [[nodiscard]] static std::u16string Combine(std::u16string_view path1, std::u16string_view path2);
    [[nodiscard]] static constexpr bool IsPathRooted(std::u16string_view path) noexcept;
    [[nodiscard]] static constexpr std::u16string_view GetExtension(std::u16string_view path) noexcept;
    [[nodiscard]] static constexpr std::u16string_view GetFileName(std::u16string_view path) noexcept;
    [[nodiscard]] static constexpr std::u16string_view GetFileNameWithoutExtension(std::u16string_view path) noexcept;
    [[nodiscard]] static std::u16string GetDirectoryName(std::u16string_view path);
    [[nodiscard]] static std::u16string ChangeExtension(std::u16string_view path, std::u16string_view extension);
    [[nodiscard]] static constexpr bool HasExtension(std::u16string_view path) noexcept;
    [[nodiscard]] static std::u16string GetFullPath(std::u16string_view path);
    [[nodiscard]] static std::u16string GetFullPath(std::u16string_view path, std::u16string_view basePath);
    [[nodiscard]] static std::u16string GetPathRoot(std::u16string_view path);
    [[nodiscard]] static std::u16string GetRandomFileName();
    [[nodiscard]] static std::u16string GetTempPath();
    [[nodiscard]] static constexpr gsl::span<const char16_t> GetInvalidFileNameChars() noexcept;
    [[nodiscard]] static constexpr gsl::span<const char16_t> GetInvalidPathChars() noexcept;
    [[nodiscard]] static constexpr bool IsDirectorySeparator(char16_t c) noexcept;

private:
    static constexpr int32_t GetRootLength(std::u16string_view path) noexcept;
    static constexpr bool IsValidDriveChar(char16_t c) noexcept;
    static std::u16string RemoveRelativeSegments(std::u16string_view path);

public:
    static constexpr char16_t AltDirectorySeparatorChar = PlatformPath::AltDirectorySeparatorChar;
    static constexpr char16_t DirectorySeparatorChar = PlatformPath::DirectorySeparatorChar;
    static constexpr char16_t PathSeparator = PlatformPath::PathSeparator;
    static constexpr char16_t VolumeSeparatorChar = PlatformPath::VolumeSeparatorChar;

private:
    static constexpr char16_t AltDirectorySeparatorStr[] = {AltDirectorySeparatorChar, u'\0'};
    static constexpr char16_t DirectorySeparatorStr[] = {DirectorySeparatorChar, u'\0'};
};

constexpr bool Path::IsPathRooted(std::u16string_view path) noexcept
{
    return (path.length() >= 1 && IsDirectorySeparator(path[0]))
        || (path.length() >= 2 && IsValidDriveChar(path[0]) && path[1] == VolumeSeparatorChar);
}

constexpr std::u16string_view Path::GetExtension(std::u16string_view path) noexcept
{
    auto index = path.length();
    while (index-- > 0)
    {
        if (path[index] == '.')
            return path.substr(index);
    }

    return {};
}

constexpr std::u16string_view Path::GetFileName(std::u16string_view path) noexcept
{
    auto index = path.length();
    while (index-- > 0)
    {
        if (IsDirectorySeparator(path[index]))
            return path.substr(index + 1, path.length() - index);
    }

    return path;
}

constexpr std::u16string_view Path::GetFileNameWithoutExtension(std::u16string_view path) noexcept
{
    auto extensionStartIndex = path.length();

    auto index = extensionStartIndex;
    while (index-- > 0)
    {
        if (IsDirectorySeparator(path[index]))
            return path.substr(index + 1, extensionStartIndex - (index + 1));

        if (index == 0)
            return path.substr(0, extensionStartIndex);

        if (path[index] == '.' && (extensionStartIndex == path.length()))
            extensionStartIndex = index;
    }

    return path;
}

constexpr bool Path::HasExtension(std::u16string_view path) noexcept
{
    auto index = path.length();
    while (index-- > 0)
    {
        if (path[index] == u'.')
            return path.length() > (index + 1);
    }

    return false;
}

constexpr bool Path::IsDirectorySeparator(char16_t c) noexcept
{
    return c == AltDirectorySeparatorChar || c == DirectorySeparatorChar;
}

constexpr bool Path::IsValidDriveChar(char16_t c) noexcept
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

CS2CPP_NAMESPACE_END

#if CS2CPP_PLATFORM_WINDOWS
#    include "Path.Windows.inl"
#elif CS2CPP_SUPPORT_POSIX
#    include "Path.Posix.inl"
#endif
