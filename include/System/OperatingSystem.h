#pragma once

#include "PlatformID.h"
#include "String.h"
#include "Version.h"

CS2CPP_NAMESPACE_BEGIN

class OperatingSystem final
{
public:
    OperatingSystem(PlatformID platform, Version version) noexcept;
    OperatingSystem(PlatformID platform, Version version, String servicePack) noexcept;

public:
    [[nodiscard]] PlatformID GetPlatform() const noexcept;
    [[nodiscard]] const String& GetServicePack() const noexcept;
    [[nodiscard]] const Version& GetVersion() const noexcept;
    [[nodiscard]] const String& ToString() const;
    [[nodiscard]] const String& GetVersionString() const;

private:
    PlatformID _platform;
    Version _version;
    String _servicePack;
    mutable String _versionString;
};

inline OperatingSystem::OperatingSystem(PlatformID platform, Version version) noexcept :
    OperatingSystem(platform, version, {})
{
}

inline OperatingSystem::OperatingSystem(PlatformID platform, Version version, String servicePack) noexcept :
    _platform(platform),
    _version(version),
    _servicePack(std::move(servicePack))
{
}

inline PlatformID OperatingSystem::GetPlatform() const noexcept
{
    return _platform;
}

inline const String& OperatingSystem::GetServicePack() const noexcept
{
    return _servicePack;
}

inline const Version& OperatingSystem::GetVersion() const noexcept
{
    return _version;
}

inline const String& OperatingSystem::ToString() const
{
    return this->GetVersionString();
}

inline const String& OperatingSystem::GetVersionString() const
{
    if (_versionString.Length() > 0)
    {
        return _versionString;
    }

    String versionString;
    switch (_platform)
    {
    case PlatformID::Win32NT:
        versionString = String(u"Microsoft Windows NT ");
        break;

    case PlatformID::Win32Windows:
        versionString = (_version.GetMajor() > 4) || ((_version.GetMajor() == 4) && (_version.GetMinor() > 0)) ? String(u"Microsoft Windows 98 ") : String(u"Microsoft Windows 95 ");
        break;

    case PlatformID::Win32S:
        versionString = String(u"Microsoft Win32S ");
        break;

    case PlatformID::WinCE:
        versionString = String(u"Microsoft Windows CE ");
        break;

    case PlatformID::MacOSX:
        versionString = String(u"Mac OS X ");
        break;

    default:
        versionString = String(u"<unknown> ");
        break;
    }

    if (_servicePack.Length() == 0)
    {
        versionString += _version.ToString();
    }
    else
    {
        versionString += String::Format(u"{}{}{}", _version.ToString(3), u" ", _servicePack);
    }

    _versionString = std::move(versionString);

    return _versionString;
}

CS2CPP_NAMESPACE_END