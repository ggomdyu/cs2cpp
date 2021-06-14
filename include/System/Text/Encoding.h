#pragma once

#include <gsl/span>
#include <optional>
#include <string>
#include <unicode/ucnv.h>
#include <unordered_map>
#include <vector>

CS2CPP_NAMESPACE_BEGIN

struct DecoderFallback
{
    UConverterToUCallback callback;
    const void* context;
};

struct EncoderFallback
{
    UConverterFromUCallback callback;
    const void* context;
};

class Encoding final
{
private:
    explicit Encoding(UConverter* converter);

public:
    Encoding(const Encoding& rhs) = delete;
    Encoding(Encoding&& rhs) noexcept = default;
    ~Encoding();

public:
    Encoding& operator=(const Encoding& rhs) = delete;
    Encoding& operator=(Encoding&& rhs) noexcept = default;
    bool operator==(const Encoding& rhs) const noexcept;
    bool operator!=(const Encoding& rhs) const noexcept;

public:
    [[nodiscard]] static std::optional<Encoding> Create(int32_t codePage);
    [[nodiscard]] static std::optional<Encoding> Create(std::u16string_view encodingName);
    [[nodiscard]] static std::optional<int32_t> Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::byte* srcBytes, int32_t srcByteCount, std::byte* destBytes, int32_t destByteCount);
    [[nodiscard]] static std::optional<int32_t> Convert(const Encoding& srcEncoding, const Encoding& destEncoding, gsl::span<const std::byte> srcBytes, gsl::span<std::byte> destBytes);
    [[nodiscard]] static std::optional<std::vector<std::byte>> Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::byte* bytes, int32_t count);
    [[nodiscard]] static std::optional<std::vector<std::byte>> Convert(const Encoding& srcEncoding, const Encoding& destEncoding, gsl::span<const std::byte> srcBytes);
    [[nodiscard]] std::optional<int32_t> GetCharCount(const std::byte* bytes, int32_t count) const;
    [[nodiscard]] std::optional<int32_t> GetCharCount(gsl::span<const std::byte> bytes) const;
    [[nodiscard]] std::optional<std::vector<char32_t>> GetChars(const std::byte* bytes, int32_t count) const;
    [[nodiscard]] std::optional<std::vector<char32_t>> GetChars(gsl::span<const std::byte> bytes) const;
    [[nodiscard]] int32_t GetCodePage() const noexcept;
    [[nodiscard]] DecoderFallback GetDecoderFallback() const noexcept;
    [[nodiscard]] static const Encoding* GetEncoding(int32_t codePage);
    [[nodiscard]] static const Encoding* GetEncoding(std::u16string_view encodingName);
    [[nodiscard]] std::u16string_view GetEncodingName() const noexcept;
    [[nodiscard]] EncoderFallback GetEncoderFallback() const noexcept;
    [[nodiscard]] int32_t GetHashCode() const noexcept;
    [[nodiscard]] int32_t GetMaxByteCount(int32_t charCount) const noexcept;
    [[nodiscard]] int32_t GetMaxCharCount(int32_t byteCount) const noexcept;
    [[nodiscard]] std::optional<std::u16string> GetString(const std::byte* bytes, int32_t count) const;
    [[nodiscard]] std::optional<std::u16string> GetString(gsl::span<const std::byte> bytes) const;
    [[nodiscard]] gsl::span<const std::byte> GetPreamble() const noexcept;
    [[nodiscard]] bool IsSingleByte() const noexcept;
    void SetEncoderFallback(EncoderFallback fallback);
    void SetDecoderFallback(DecoderFallback fallback);
    [[nodiscard]] static const Encoding& Unicode() noexcept;
    [[nodiscard]] static const Encoding& UTF8() noexcept;
    [[nodiscard]] static const Encoding& UTF32() noexcept;
    [[nodiscard]] static const Encoding& BigEndianUnicode() noexcept;

private:
    static UConverter* CreateUConverter(std::u16string_view encodingName);
    static UConverter* CreateUConverter(int32_t codePage);
    static std::u16string GetUnicodeEncodingName(UConverter* converter);
    int32_t GetMinCharByte() const noexcept;
    int32_t GetMaxCharByte() const noexcept;
    void SetDefaultFallback() const noexcept;

private:
    std::unique_ptr<UConverter, decltype(&ucnv_close)> _converter;
    std::u16string _encodingName;
    gsl::span<const std::byte> _preamble{};
    inline static std::unordered_map<int32_t, Encoding> _encodingTable;
};

CS2CPP_NAMESPACE_END
