#pragma once

#include <string>

#include "PlatformID.h"
#include "Version.h"

CS2CPP_NAMESPACE_BEGIN

class OperatingSystem final
{
public:
    OperatingSystem(PlatformID platform, Version version, std::u16string servicePack) noexcept;

public:
    [[nodiscard]] PlatformID GetPlatform() const noexcept;
    [[nodiscard]] const std::u16string& GetServicePack() const noexcept;
    [[nodiscard]] const Version& GetVersion() const noexcept;
    [[nodiscard]] const std::u16string& ToString() const;
    [[nodiscard]] const std::u16string& GetVersionString() const;

private:
    PlatformID _platform;
    Version _version;
    std::u16string _servicePack;
    mutable std::u16string _versionString;
};

inline OperatingSystem::OperatingSystem(PlatformID platform, Version version, std::u16string servicePack) noexcept :
    _platform(platform),
    _version(version),
    _servicePack(std::move(servicePack))
{
}

inline PlatformID OperatingSystem::GetPlatform() const noexcept
{
    return _platform;
}

inline const std::u16string& OperatingSystem::GetServicePack() const noexcept
{
    return _servicePack;
}

inline const Version& OperatingSystem::GetVersion() const noexcept
{
    return _version;
}

inline const std::u16string& OperatingSystem::ToString() const
{
    return GetVersionString();
}

inline const std::u16string& OperatingSystem::GetVersionString() const
{
    if (!_versionString.empty())
        return _versionString;

    auto platformName = u"";
    switch (_platform)
    {
    case PlatformID::Win32NT:
        platformName = u"Microsoft Windows NT ";
        break;

    case PlatformID::Win32Windows:
        platformName = (_version.GetMajor() > 4) || ((_version.GetMajor() == 4) && (_version.GetMinor() > 0)) ?
            u"Microsoft Windows 98 " : u"Microsoft Windows 95 ";
        break;

    case PlatformID::Win32S:
        platformName = u"Microsoft Win32S ";
        break;

    case PlatformID::WinCE:
        platformName = u"Microsoft Windows CE ";
        break;

    case PlatformID::MacOSX:
        platformName = u"Mac OS X ";
        break;

    default:
        platformName = u"<unknown> ";
        break;
    }

    if (_servicePack.empty())
    {
        _versionString += platformName;
        _versionString += _version.ToString();
    }
    else
        _versionString = fmt::format(u"{}{}{}{}", platformName, _version.ToString(3), u" ", _servicePack);

    return _versionString;
}

CS2CPP_NAMESPACE_END
