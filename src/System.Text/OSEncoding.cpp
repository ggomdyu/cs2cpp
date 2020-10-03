#include <fmt/xchar.h>

#include "System.Text/OSEncoding.h"

CS2CPP_NAMESPACE_BEGIN

OSEncoding::OSEncoding(int32_t codePage) :
    Encoding(codePage, &dataItem_, true, nullptr, nullptr),
    displayName_(fmt::format(u"Codepage - {}", codePage)),
    dataItem_(0, displayName_, u"", u"", displayName_, {}, 0)
{
}

std::shared_ptr<Encoding> OSEncoding::Clone() const noexcept
{
    auto ret = std::make_shared<OSEncoding>(codePage_);

    // Cloned Encoding is writable.
    ret->isReadOnly_ = false;

    return ret;
}

bool OSEncoding::IsSingleByte() const noexcept
{
    return false;
}

bool OSEncoding::SetEncoderFallback(std::shared_ptr<EncoderFallback> fallback)
{
    if (!fallback || IsReadOnly())
    {
        return false;
    }

    encoderFallback_ = std::move(fallback);
    return true;
}

bool OSEncoding::SetDecoderFallback(std::shared_ptr<DecoderFallback> fallback)
{
    if (!fallback || IsReadOnly())
    {
        return false;
    }

    decoderFallback_ = std::move(fallback);
    return true;
}

std::optional<std::vector<std::byte>> OSEncoding::GetBytes(ReadOnlySpan<char16_t> chars) const
{
    auto byteCount = GetByteCount(chars);
    if (!byteCount)
    {
        return std::nullopt;
    }

    std::vector<std::byte> bytes(*byteCount);
    if (!GetBytes(chars, bytes))
    {
        return std::nullopt;
    }

    return std::optional<std::vector<std::byte>>(std::move(bytes));
}

std::optional<std::vector<char16_t>> OSEncoding::GetChars(ReadOnlySpan<std::byte> bytes) const
{
    auto charCount = GetCharCount(bytes);
    if (!charCount)
    {
        return std::nullopt;
    }

    std::vector<char16_t> chars(*charCount);
    if (!GetChars(bytes, chars))
    {
        return std::nullopt;
    }

    return std::optional<std::vector<char16_t>>(std::move(chars));
}

std::optional<std::u16string> OSEncoding::GetString(ReadOnlySpan<std::byte> bytes) const
{
    auto charCount = GetCharCount(bytes);
    if (!charCount)
    {
        return std::nullopt;
    }

    std::u16string str;
    str.resize(*charCount);
    if (!GetChars(bytes, str))
    {
        return std::nullopt;
    }

    // Remove unnecessary null terminate characters.
    while (!str.empty() && str.back() == '\0')
    {
        str.pop_back();
    }

    return std::optional<std::u16string>(std::move(str));
}

int32_t OSEncoding::GetMaxByteCount(int32_t charCount) const noexcept
{
    return std::max(charCount * 14, 0);
}

int32_t OSEncoding::GetMaxCharCount(int32_t byteCount) const noexcept
{
    return std::max(byteCount * 4, 0);
}

CS2CPP_NAMESPACE_END
