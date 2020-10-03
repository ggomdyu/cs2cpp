#include "System.Text/OSEncoding.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

std::optional<int32_t> OSEncoding::GetBytes(ReadOnlySpan<char16_t> chars, Span<std::byte> bytes) const
{
    return WideCharToMultiByte(codePage_, 0,
        reinterpret_cast<LPCWCH>(&chars[0]), static_cast<int>(chars.Length()),
        reinterpret_cast<LPSTR>(&bytes[0]), static_cast<int>(bytes.Length()), nullptr, nullptr);
}

std::optional<int32_t> OSEncoding::GetByteCount(ReadOnlySpan<char16_t> chars) const
{
    auto byteCount = WideCharToMultiByte(codePage_, 0, reinterpret_cast<LPCWCH>(&chars[0]), static_cast<int>(chars.Length()),
        nullptr, 0, nullptr, nullptr);
    if (byteCount <= 0)
    {
        return std::nullopt;
    }

    return byteCount;
}

std::optional<int32_t> OSEncoding::GetChars(ReadOnlySpan<std::byte> bytes, Span<char16_t> chars) const
{
    return MultiByteToWideChar(codePage_, 0,
        reinterpret_cast<LPCCH>(&bytes[0]), static_cast<int>(bytes.Length()),
        reinterpret_cast<LPWSTR>(&chars[0]), static_cast<int>(chars.Length()));
}

std::optional<int32_t> OSEncoding::GetCharCount(ReadOnlySpan<std::byte> bytes) const
{
    auto charCount = MultiByteToWideChar(codePage_, 0, reinterpret_cast<LPCCH>(&bytes[0]), static_cast<int>(bytes.Length()),
        nullptr, 0);
    if (charCount <= 0)
    {
        return std::nullopt;
    }

    return charCount;
}

CS2CPP_NAMESPACE_END
