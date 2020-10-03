#pragma once
#include "System.Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

class OSEncoding final :
    public Encoding
{
public:
    explicit OSEncoding(int32_t codePage);
    OSEncoding(const OSEncoding& rhs) = delete;
    OSEncoding(OSEncoding&& rhs) noexcept = default;
    ~OSEncoding() override = default;

public:
    OSEncoding& operator=(const OSEncoding& rhs) = delete;
    OSEncoding& operator=(OSEncoding&& rhs) noexcept = default;

public:
    std::shared_ptr<Encoding> Clone() const noexcept override;
    bool IsSingleByte() const noexcept override;
    bool SetEncoderFallback(std::shared_ptr<EncoderFallback> fallback) override;
    bool SetDecoderFallback(std::shared_ptr<DecoderFallback> fallback) override;
    std::optional<int32_t> GetBytes(ReadOnlySpan<char16_t> chars, Span<std::byte> bytes) const override;
    std::optional<std::vector<std::byte>> GetBytes(ReadOnlySpan<char16_t> chars) const override;
    std::optional<int32_t> GetChars(ReadOnlySpan<std::byte> bytes, Span<char16_t> chars) const override;
    std::optional<std::vector<char16_t>> GetChars(ReadOnlySpan<std::byte> bytes) const override;
    std::optional<int32_t> GetByteCount(ReadOnlySpan<char16_t> chars) const override;
    std::optional<int32_t> GetCharCount(ReadOnlySpan<std::byte> bytes) const override;
    std::optional<std::u16string> GetString(ReadOnlySpan<std::byte> bytes) const override;
    int32_t GetMaxByteCount(int32_t charCount) const noexcept override;
    int32_t GetMaxCharCount(int32_t byteCount) const noexcept override;

private:
    std::u16string displayName_;
    CodePageDataItem dataItem_;
};

CS2CPP_NAMESPACE_END