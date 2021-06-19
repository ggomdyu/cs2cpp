#pragma once

CS2CPP_NAMESPACE_BEGIN

class PosixPath
{
public:
    PosixPath() = delete;

public:
    [[nodiscard]] static constexpr bool IsPathRooted(std::string_view path) noexcept;
    [[nodiscard]] static std::string Combine(std::string_view path1, std::string_view path2);

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
    return (path.length() >= 1 && path[0] == DirectorySeparatorChar)
        || (path.length() >= 2 && IsValidDriveChar(path[0]) && path[1] == PosixPath::VolumeSeparatorChar);
}

constexpr bool PosixPath::IsValidDriveChar(char c) noexcept
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

CS2CPP_NAMESPACE_END