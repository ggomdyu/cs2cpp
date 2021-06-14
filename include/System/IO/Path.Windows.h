#pragma once

CS2CPP_NAMESPACE_BEGIN

class WindowsPath
{
public:
    WindowsPath() = delete;

protected:
    static constexpr char16_t DirectorySeparatorChar = u'\\';
    static constexpr char16_t AltDirectorySeparatorChar = u'/';
    static constexpr char16_t VolumeSeparatorChar = u':';
    static constexpr char16_t PathSeparator = u';';
};

using PlatformPath = WindowsPath;

CS2CPP_NAMESPACE_END