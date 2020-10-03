#pragma once

#include <optional>
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
    static constexpr std::optional<Version> Parse(std::u16string_view input) noexcept;
    constexpr int32_t GetMajor() const noexcept;
    constexpr int32_t GetMinor() const noexcept;
    constexpr int32_t GetBuild() const noexcept;
    constexpr int32_t GetRevision() const noexcept;
    constexpr int16_t GetMajorRevision() const noexcept;
    constexpr int16_t GetMinorRevision() const noexcept;
    constexpr int32_t GetHashCode() const noexcept;
    constexpr int32_t CompareTo(const Version& rhs) const noexcept;
    std::u16string ToString() const;
    std::u16string ToString(int32_t fieldCount) const;

private:
    int32_t major_ = 0;
    int32_t minor_ = 0;
    int32_t build_ = -1;
    int32_t revision_ = -1;
};

constexpr Version::Version(int32_t major, int32_t minor, int32_t build, int32_t revision) noexcept :
    major_(major),
    minor_(minor),
    build_(build),
    revision_(revision)
{
}

constexpr Version::Version(int32_t major, int32_t minor, int32_t build) noexcept :
    major_(major),
    minor_(minor),
    build_(build)
{
}

constexpr Version::Version(int32_t major, int32_t minor) noexcept :
    major_(major),
    minor_(minor)
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

constexpr std::optional<Version> Version::Parse(std::u16string_view input) noexcept
{
    int32_t major = 0;
    int32_t minor = 0;
    int32_t build = 0;
    int32_t revision = 0;

    int32_t number = 0;
    int32_t state = 0;

    auto applyNumber = [&]()
    {
        switch (state)
        {
        case 0:
            major = number;
            break;
        case 1:
            minor = number;
            break;
        case 2:
            build = number;
            break;
        case 3:
            revision = number;
            break;
        }
    };

    for (auto c : input)
    {
        if (u'0' <= c && c <= u'9')
        {
            number = (number * 10) + (c - u'0');
            continue;
        }
        else if (c == u'.')
        {
            applyNumber();
            number = 0;
            ++state;
        }
        else
        {
            return std::nullopt;
        }
    }

    applyNumber();

    return Version(major, minor, build, revision);
}

constexpr int32_t Version::GetMajor() const noexcept
{
    return major_;
}

constexpr int32_t Version::GetMinor() const noexcept
{
    return minor_;
}

constexpr int32_t Version::GetBuild() const noexcept
{
    return build_;
}

constexpr int32_t Version::GetRevision() const noexcept
{
    return revision_;
}

constexpr int16_t Version::GetMajorRevision() const noexcept
{
    return static_cast<int16_t>(revision_ >> 16);
}

constexpr int16_t Version::GetMinorRevision() const noexcept
{
    return static_cast<int16_t>(revision_ & 0xffff);
}

constexpr int32_t Version::GetHashCode() const noexcept
{
    return ((major_ & 0x0000000f) << 28) | ((minor_ & 0x000000ff) << 20) |
           ((build_ & 0x000000ff) << 12) | ((revision_ & 0x00000fff));
}

constexpr int32_t Version::CompareTo(const Version& rhs) const noexcept
{
    if (major_ != rhs.major_)
    {
        return major_ > rhs.major_ ? 1 : -1;
    }

    if (minor_ != rhs.minor_)
    {
        return minor_ > rhs.minor_ ? 1 : -1;
    }

    if (build_ != rhs.build_)
    {
        return build_ > rhs.build_ ? 1 : -1;
    }

    if (revision_ != rhs.revision_)
    {
        return revision_ > rhs.revision_ ? 1 : -1;
    }

    return 0;
}

CS2CPP_NAMESPACE_END
