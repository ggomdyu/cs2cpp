#pragma once

#include "System.Text/Encoding.h"

struct UConverter;

CS2CPP_NAMESPACE_BEGIN

class ICUEncoding final :
    public Encoding
{
public:
    ICUEncoding(UConverter* converter, int32_t codePage);
    ICUEncoding(UConverter* converter, int32_t codePage, std::shared_ptr<EncoderFallback> encoderFallback, std::shared_ptr<DecoderFallback> decoderFallback);
    ICUEncoding(const ICUEncoding& rhs) = delete;
    ICUEncoding(ICUEncoding&& rhs) noexcept = default;
    ~ICUEncoding() override;

public:
    ICUEncoding& operator=(const ICUEncoding& rhs) = delete;
    ICUEncoding& operator=(ICUEncoding&& rhs) noexcept = default;

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
    UConverter* GetInternalConverter() const noexcept;

protected:
    void RefreshEncoderFallback();
    void RefreshDecoderFallback();
    int32_t GetMinCharByte() const noexcept;
    int32_t GetMaxCharByte() const noexcept;

private:
    std::unique_ptr<UConverter, decltype(&ucnv_close)> converter_;
};

CS2CPP_NAMESPACE_END