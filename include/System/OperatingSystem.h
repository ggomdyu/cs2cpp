#pragma once

#include "PlatformID.h"
#include "Version.h"

CS2CPP_NAMESPACE_BEGIN

class OperatingSystem final
{
public:
    OperatingSystem(PlatformID platform, Version version, std::u16string_view servicePack) noexcept;

public:
    bool operator==(const OperatingSystem& rhs) const noexcept;
    bool operator!=(const OperatingSystem& rhs) const noexcept;

public:
    PlatformID GetPlatform() const noexcept;
    const Version& GetVersion() const noexcept;
    std::u16string_view GetServicePack() const noexcept;
    std::u16string_view GetVersionString() const noexcept;
    std::u16string_view ToString() const;

private:
    static std::u16string CreateVersionString(PlatformID platform, Version version, std::u16string_view servicePack);

private:
    PlatformID platform_;
    Version version_;
    std::u16string servicePack_;
    std::u16string versionString_;
};

CS2CPP_NAMESPACE_END
