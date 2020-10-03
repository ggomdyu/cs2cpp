#pragma once

#include <optional>
#include <vector>

#include "System.Text/CodePageDataItem.h"
#include "System.Text/DecoderFallback.h"
#include "System.Text/EncoderFallback.h"
#include "System.Text/EncodingInfo.h"

CS2CPP_NAMESPACE_BEGIN

class Encoding
{
public:
    Encoding(int32_t codePage, const CodePageDataItem* dataItem, bool isReadOnly, std::shared_ptr<EncoderFallback> encoderFallback, std::shared_ptr<DecoderFallback> decoderFallback);
    Encoding(const Encoding& rhs) = delete;
    Encoding(Encoding&& rhs) noexcept = default;
    virtual ~Encoding() = default;

public:
    Encoding& operator=(const Encoding& rhs) = delete;
    Encoding& operator=(Encoding&& rhs) noexcept = default;
    bool operator==(const Encoding& rhs) const noexcept;
    bool operator!=(const Encoding& rhs) const noexcept;

public:
    static std::optional<int32_t> Convert(const Encoding& srcEncoding, const Encoding& destEncoding, ReadOnlySpan<std::byte> srcBytes, Span<std::byte> destBytes);
    static std::optional<std::vector<std::byte>> Convert(const Encoding& srcEncoding, const Encoding& destEncoding, ReadOnlySpan<std::byte> bytes);
    static std::shared_ptr<Encoding> GetEncoding(int32_t codePage);
    static std::shared_ptr<Encoding> GetEncoding(std::u16string_view encodingName);
    static std::shared_ptr<Encoding> GetEncoding(int32_t codePage, std::shared_ptr<EncoderFallback> encoderFallback, std::shared_ptr<DecoderFallback> decoderFallback);
    static std::shared_ptr<Encoding> GetEncoding(std::u16string_view encodingName, std::shared_ptr<EncoderFallback> encoderFallback, std::shared_ptr<DecoderFallback> decoderFallback);
    static std::vector<EncodingInfo> GetEncodings();
    static const Encoding& ASCII() noexcept;
    static const Encoding& Latin1() noexcept;
    static const Encoding& UTF8() noexcept;
    static const Encoding& Unicode() noexcept;
    static const Encoding& BigEndianUnicode() noexcept;
    static const Encoding& UTF32() noexcept;
    static const Encoding& BigEndianUnicode32() noexcept;
    ReadOnlySpan<std::byte> Preamble() const noexcept;
    std::u16string_view BodyName() const noexcept;
    std::u16string_view EncodingName() const noexcept;
    std::u16string_view HeaderName() const noexcept;
    std::u16string_view WebName() const noexcept;
    int32_t WindowsCodePage() const noexcept;
    bool IsBrowserDisplay() const noexcept;
    bool IsBrowserSave() const noexcept;
    bool IsMailNewsDisplay() const noexcept;
    bool IsMailNewsSave() const noexcept;
    bool IsReadOnly() const noexcept;
    int32_t CodePage() const noexcept;
    int32_t GetHashCode() const noexcept;
    virtual std::shared_ptr<Encoding> Clone() const noexcept = 0;
    virtual bool IsSingleByte() const noexcept = 0;
    virtual bool SetEncoderFallback(std::shared_ptr<EncoderFallback> fallback) = 0;
    virtual bool SetDecoderFallback(std::shared_ptr<DecoderFallback> fallback) = 0;
    std::shared_ptr<EncoderFallback> GetEncoderFallback() const noexcept;
    std::shared_ptr<DecoderFallback> GetDecoderFallback() const noexcept;
    virtual std::optional<int32_t> GetBytes(ReadOnlySpan<char16_t> chars, Span<std::byte> bytes) const = 0;
    virtual std::optional<std::vector<std::byte>> GetBytes(ReadOnlySpan<char16_t> chars) const = 0;
    virtual std::optional<int32_t> GetChars(ReadOnlySpan<std::byte> bytes, Span<char16_t> chars) const = 0;
    virtual std::optional<std::vector<char16_t>> GetChars(ReadOnlySpan<std::byte> bytes) const = 0;
    virtual std::optional<int32_t> GetByteCount(ReadOnlySpan<char16_t> chars) const = 0;
    virtual std::optional<int32_t> GetCharCount(ReadOnlySpan<std::byte> bytes) const = 0;
    virtual std::optional<std::u16string> GetString(ReadOnlySpan<std::byte> bytes) const = 0;
    virtual int32_t GetMaxByteCount(int32_t charCount) const noexcept = 0;
    virtual int32_t GetMaxCharCount(int32_t byteCount) const noexcept = 0;
    virtual bool Equals(const Encoding& value) const noexcept;

protected:
    static constexpr int32_t CodePageASCII = 20127;
    static constexpr int32_t CodePageLatin1 = 28591;
    static constexpr int32_t CodePageUTF8 = 65001;
    static constexpr int32_t CodePageUTF16 = 1200;
    static constexpr int32_t CodePageUTF16BE = 1201;
    static constexpr int32_t CodePageUTF32 = 12000;
    static constexpr int32_t CodePageUTF32BE = 12001;

    int32_t codePage_;
    const CodePageDataItem* dataItem_;
    bool isReadOnly_;
    std::shared_ptr<EncoderFallback> encoderFallback_;
    std::shared_ptr<DecoderFallback> decoderFallback_;
};

CS2CPP_NAMESPACE_END
