#pragma once

CS2CPP_NAMESPACE_BEGIN
    
constexpr gsl::span<const char16_t> Path::GetInvalidFileNameChars() noexcept
{
    constexpr char16_t invalidFileNameChars[] = {u'\0', u'a'};
    return invalidFileNameChars;
}

constexpr gsl::span<const char16_t> Path::GetInvalidPathChars() noexcept
{
    constexpr char16_t invalidPathChars[] = {u'\0'};
    return invalidPathChars;
}

constexpr std::u16string_view Path::GetPathRoot(std::u16string_view path) noexcept
{
    return IsDirectorySeparator(path[0]) ? DirectorySeparatorStr : std::u16string_view();
}

constexpr int32_t Path::GetRootLength(std::u16string_view path) noexcept
{
    return path.length() > 0 && Path::IsDirectorySeparator(path[0]) ? 1 : 0;
}

CS2CPP_NAMESPACE_END