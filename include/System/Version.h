#pragma once

#include <fmt/format.h>
#include <string>

CS2CPP_NAMESPACE_BEGIN

struct Version final
{
public:
    constexpr Version(int32_t major, int32_t minor, int32_t build, int32_t revision) noexcept;
    constexpr Version(int32_t major, int32_t minor, int32_t build) noexcept;
    constexpr Version(int32_t major, int32_t minor) noexcept;

public:
    constexpr bool operator==(const Version& rhs) const noexcept;
    constexpr bool operator!=(const Version& rhs) const noexcept;
    constexpr bool operator<(const Version& rhs) const noexcept;
    constexpr bool operator>(const Version& rhs) const noexcept;
    constexpr bool operator<=(const Version& rhs) const noexcept;
    constexpr bool operator>=(const Version& rhs) const noexcept;

public:
    [[nodiscard]] constexpr int32_t GetMajor() const noexcept;
    [[nodiscard]] constexpr int32_t GetMinor() const noexcept;
    [[nodiscard]] constexpr int32_t GetBuild() const noexcept;
    [[nodiscard]] constexpr int32_t GetRevision() const noexcept;
    [[nodiscard]] constexpr int16_t GetMajorRevision() const noexcept;
    [[nodiscard]] constexpr int16_t GetMinorRevision() const noexcept;
    [[nodiscard]] constexpr int32_t GetHashCode() const noexcept;
    [[nodiscard]] constexpr int32_t CompareTo(const Version& rhs) const noexcept;
    [[nodiscard]] std::u16string ToString() const;
    [[nodiscard]] std::u16string ToString(int32_t fieldCount) const;

private:
    int32_t _major = 0;
    int32_t _minor = 0;
    int32_t _build = -1;
    int32_t _revision = -1;
};

constexpr Version::Version(int32_t major, int32_t minor, int32_t build, int32_t revision) noexcept :
    _major(major),
    _minor(minor),
    _build(build),
    _revision(revision)
{
}

constexpr Version::Version(int32_t major, int32_t minor, int32_t build) noexcept :
    _major(major),
    _minor(minor),
    _build(build)
{
}

constexpr Version::Version(int32_t major, int32_t minor) noexcept :
    _major(major),
    _minor(minor)
{
}

constexpr bool Version::operator==(const Version& rhs) const noexcept
{
    return CompareTo(rhs) == 0;
}

constexpr bool Version::operator!=(const Version& rhs) const noexcept
{
    return !operator==(rhs);
}

constexpr bool Version::operator<(const Version& rhs) const noexcept
{
    return CompareTo(rhs) < 0;
}

constexpr bool Version::operator<=(const Version& rhs) const noexcept
{
    return CompareTo(rhs) <= 0;
}

constexpr bool Version::operator>(const Version& rhs) const noexcept
{
    return rhs < *this;
}

constexpr bool Version::operator>=(const Version& rhs) const noexcept
{
    return rhs <= *this;
}

constexpr int32_t Version::GetMajor() const noexcept
{
    return _major;
}

constexpr int32_t Version::GetMinor() const noexcept
{
    return _minor;
}

constexpr int32_t Version::GetBuild() const noexcept
{
    return _build;
}

constexpr int32_t Version::GetRevision() const noexcept
{
    return _revision;
}

constexpr int16_t Version::GetMajorRevision() const noexcept
{
    return _revision >> 16;
}

constexpr int16_t Version::GetMinorRevision() const noexcept
{
    return _revision & 0xffff;
}

constexpr int32_t Version::GetHashCode() const noexcept
{
    return ((_major & 0x0000000f) << 28) | ((_minor & 0x000000ff) << 20) | ((_build & 0x000000ff) << 12) |
        ((_revision & 0x00000fff));
}

constexpr int32_t Version::CompareTo(const Version& rhs) const noexcept
{
    if (_major != rhs._major)
        return _major > rhs._major ? 1 : -1;

    if (_minor != rhs._minor)
        return _minor > rhs._minor ? 1 : -1;

    if (_build != rhs._build)
        return _build > rhs._build ? 1 : -1;

    if (_revision != rhs._revision)
        return _revision > rhs._revision ? 1 : -1;

    return 0;
}

inline std::u16string Version::ToString() const
{
    if (_build == -1)
        return ToString(2);

    if (_revision == -1)
        return ToString(3);

    return ToString(4);
}

inline std::u16string Version::ToString(int32_t fieldCount) const
{
    switch (fieldCount)
    {
    case 0:
        return {};

    case 1:
        return fmt::format(u"{}", _major);

    case 2:
        return fmt::format(u"{}.{}", _major, _minor);

    default:
        if (_build == -1)
            return {};

        if (fieldCount == 3)
            return fmt::format(u"{}.{}.{}", _major, _minor, _build);

        if (_revision == -1)
            return {};

        if (fieldCount == 4)
            return fmt::format(u"{}.{}.{}.{}", _major, _minor, _build, _revision);

        return {};
    }
}

CS2CPP_NAMESPACE_END
