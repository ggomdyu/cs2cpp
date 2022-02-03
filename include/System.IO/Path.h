#pragma once

#include <string>

#include "System/ReadOnlySpan.h"

CS2CPP_NAMESPACE_BEGIN

class Path final
{
    friend class Directory;

public:
    Path() = delete;

public:
    template <typename... Ts>
    static std::u16string Combine(Ts&&... paths);
    static constexpr bool IsPathRooted(std::u16string_view path) noexcept;
    static constexpr std::u16string_view GetExtension(std::u16string_view path) noexcept;
    static constexpr std::u16string_view GetFileName(std::u16string_view path) noexcept;
    static constexpr std::u16string_view GetFileNameWithoutExtension(std::u16string_view path) noexcept;
    static std::u16string GetDirectoryName(std::u16string_view path);
    static std::u16string ChangeExtension(std::u16string_view path, std::u16string_view extension);
    static constexpr bool HasExtension(std::u16string_view path) noexcept;
    static std::u16string GetFullPath(std::u16string_view path);
    static std::u16string GetFullPath(std::u16string_view path, std::u16string_view basePath);
    static std::u16string GetPathRoot(std::u16string_view path);
    static std::u16string GetRandomFileName();
    static std::u16string GetTempPath();
    static constexpr ReadOnlySpan<char16_t> GetInvalidFileNameChars() noexcept;
    static constexpr ReadOnlySpan<char16_t> GetInvalidPathChars() noexcept;
    static constexpr bool IsDirectorySeparator(char16_t c) noexcept;
    static constexpr bool IsVolumeSeparator(char16_t c) noexcept;

private:
    static void InternalCombine(std::u16string_view path, std::u16string& dest);
    static constexpr int32_t GetRootLength(std::u16string_view path) noexcept;
    static constexpr bool IsValidDriveChar(char16_t c) noexcept;
    static std::u16string RemoveRelativeSegments(std::u16string_view path);

public:
    static constexpr char16_t AltDirectorySeparatorChar = u'/';
    static constexpr char16_t PathSeparator = u';';
#if CS2CPP_PLATFORM_WINDOWS
    static constexpr char16_t VolumeSeparatorChar = u':';
    static constexpr char16_t DirectorySeparatorChar = u'\\';
#else
    static constexpr char16_t VolumeSeparatorChar = u'/';
    static constexpr char16_t DirectorySeparatorChar = u'/';
#endif

private:
    static constexpr char16_t AltDirectorySeparatorStr[] = {AltDirectorySeparatorChar, u'\0'};
    static constexpr char16_t DirectorySeparatorStr[] = {DirectorySeparatorChar, u'\0'};
};

template <typename... Ts>
inline std::u16string Path::Combine(Ts&&... paths)
{
    std::u16string ret;
    ret.reserve((std::u16string_view(paths).length() + ...) + sizeof...(paths));

    (InternalCombine(std::forward<Ts>(paths), ret), ...);

    return ret;
}

constexpr bool Path::IsPathRooted(std::u16string_view path) noexcept
{
    return (path.length() >= 1 && IsDirectorySeparator(path[0]))
        || (path.length() >= 2 && IsValidDriveChar(path[0]) && path[1] == VolumeSeparatorChar);
}

constexpr std::u16string_view Path::GetExtension(std::u16string_view path) noexcept
{
    size_t dot = path.rfind(u'.');
    return (dot == std::string::npos) ? std::u16string_view() : path.substr(dot);
}

constexpr std::u16string_view Path::GetFileName(std::u16string_view path) noexcept
{
    size_t index = path.length();
    while (index-- > 0)
    {
        if (IsDirectorySeparator(path[index]) || IsVolumeSeparator(path[index]))
        {
            return path.substr(index + 1, path.length() - index);
        }
    }

    return path;
}

constexpr std::u16string_view Path::GetFileNameWithoutExtension(std::u16string_view path) noexcept
{
    std::u16string_view fileName = GetFileName(path);
    size_t dot = fileName.rfind(u'.');
    return (dot == std::string::npos) ? fileName : fileName.substr(0, dot);
}

constexpr bool Path::HasExtension(std::u16string_view path) noexcept
{
    size_t dot = path.rfind(u'.');
    return dot != std::string::npos && path.length() > (dot + 1);
}

constexpr bool Path::IsDirectorySeparator(char16_t c) noexcept
{
    return c == AltDirectorySeparatorChar || c == DirectorySeparatorChar;
}

constexpr bool Path::IsVolumeSeparator(char16_t c) noexcept
{
    return c == VolumeSeparatorChar;
}

constexpr bool Path::IsValidDriveChar(char16_t c) noexcept
{
    return (c >= u'A' && c <= u'Z') || (c >= u'a' && c <= u'z');
}

CS2CPP_NAMESPACE_END

#if CS2CPP_PLATFORM_WINDOWS
#    include "Path.Windows.inl"
#else
#    include "Path.Posix.inl"
#endif