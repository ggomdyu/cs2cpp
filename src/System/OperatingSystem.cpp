#include "System/OperatingSystem.h"

CS2CPP_NAMESPACE_BEGIN

OperatingSystem::OperatingSystem(PlatformID platform, Version version, std::u16string_view servicePack) noexcept :
    platform_(platform),
    version_(version),
    servicePack_(servicePack),
    versionString_(CreateVersionString(platform, version, servicePack))
{
}

bool OperatingSystem::operator==(const OperatingSystem& rhs) const noexcept
{
    return platform_ == rhs.platform_
        && version_ == rhs.version_
        && servicePack_ == rhs.servicePack_
        && versionString_ == rhs.versionString_;
}

bool OperatingSystem::operator!=(const OperatingSystem& rhs) const noexcept
{
    return !operator==(rhs);
}

PlatformID OperatingSystem::GetPlatform() const noexcept
{
    return platform_;
}

const Version& OperatingSystem::GetVersion() const noexcept
{
    return version_;
}

std::u16string_view OperatingSystem::GetServicePack() const noexcept
{
    return servicePack_;
}

std::u16string_view OperatingSystem::GetVersionString() const noexcept
{
    return versionString_;
}

std::u16string_view OperatingSystem::ToString() const
{
    return GetVersionString();
}

std::u16string OperatingSystem::CreateVersionString(PlatformID platform, Version version, std::u16string_view servicePack)
{
    std::u16string_view platformName;
    switch (platform)
    {
    case PlatformID::Win32NT:
        platformName = u"Microsoft Windows NT ";
        break;

    case PlatformID::Win32Windows:
        platformName = (version.GetMajor() > 4) || ((version.GetMajor() == 4) && (version.GetMinor() > 0))
            ? u"Microsoft Windows 98 "
            : u"Microsoft Windows 95 ";
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

    std::u16string ret;
    if (servicePack.empty())
    {
        auto versionStr = version.ToString();

        ret.reserve(platformName.length() + versionStr.length());
        ret += platformName;
        ret += versionStr;
    }
    else
    {
        auto versionStr = version.ToString(3);
        
        ret.reserve(platformName.length() + versionStr.length() + 1 + servicePack.length());
        ret += platformName;
        ret += versionStr;
        ret += u" ";
        ret += servicePack;
    }

    return ret;
}

CS2CPP_NAMESPACE_END
