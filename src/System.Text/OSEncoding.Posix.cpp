#include "System.Text/OSEncoding.h"

CS2CPP_NAMESPACE_BEGIN

std::optional<int32_t> OSEncoding::GetBytes(ReadOnlySpan<char16_t> chars, Span<std::byte> bytes) const
{
    return std::nullopt;
}

std::optional<int32_t> OSEncoding::GetByteCount(ReadOnlySpan<char16_t> chars) const
{
    return std::nullopt;
}

std::optional<int32_t> OSEncoding::GetChars(ReadOnlySpan<std::byte> bytes, Span<char16_t> chars) const
{
    return std::nullopt;
}

std::optional<int32_t> OSEncoding::GetCharCount(ReadOnlySpan<std::byte> bytes) const
{
    return std::nullopt;
}

CS2CPP_NAMESPACE_END
