#pragma once

CS2CPP_NAMESPACE_BEGIN

class PosixPath
{
public:
    PosixPath() = delete;

public:
    [[nodiscard]] static constexpr bool IsPathRooted(std::string_view path) noexcept;
    [[nodiscard]] static std::string Combine(std::string_view path1, std::string_view path2);
    [[nodiscard]] static constexpr bool IsDirectorySeparator(char16_t c) noexcept;

private:
    [[nodiscard]] static constexpr bool IsValidDriveChar(char c) noexcept;

public:
    static constexpr char DirectorySeparatorChar = '/';
    static constexpr char AltDirectorySeparatorChar = '/';
    static constexpr char VolumeSeparatorChar = '/';
    static constexpr char PathSeparator = ':';
};

using PlatformPath = PosixPath;

constexpr bool PosixPath::IsPathRooted(std::string_view path) noexcept
{
    return (path.length() >= 1 && PosixPath::IsDirectorySeparator(path[0])) ||
        (path.length() >= 2 && IsValidDriveChar(path[0]) && path[1] == PosixPath::VolumeSeparatorChar);
}

inline std::string PosixPath::Combine(std::string_view path1, std::string_view path2)
{
    if (path2.length() == 0)
        return std::string(path1);

    if (path1.length() == 0 || IsPathRooted(path2))
        return std::string(path2);

    std::string ret(path1);
    const auto path1Separator = path1[path1.length() - 1];
    const auto path2Separator = path2[0];
    const auto hasSeparator = (path1Separator == PosixPath::DirectorySeparatorChar) || (path1Separator == PosixPath::AltDirectorySeparatorChar)
        || (path2Separator == PosixPath::DirectorySeparatorChar) || (path2Separator == PosixPath::AltDirectorySeparatorChar);
    if (!hasSeparator)
        ret += PosixPath::DirectorySeparatorChar;
    
    ret += path2;

    return ret;
}

constexpr bool PosixPath::IsDirectorySeparator(char16_t c) noexcept
{
    return c == AltDirectorySeparatorChar || c == DirectorySeparatorChar;
}

constexpr bool PosixPath::IsValidDriveChar(char c) noexcept
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

CS2CPP_NAMESPACE_END