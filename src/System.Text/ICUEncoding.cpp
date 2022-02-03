#include <mutex>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <unordered_map>

#include "System.Text/DecoderReplacementFallback.h"
#include "System.Text/EncoderReplacementFallback.h"
#include "System.Text/EncodingTable.h"
#include "System.Text/ICUEncoding.h"

CS2CPP_NAMESPACE_BEGIN

static UConverter* CreateInternalConverter(int32_t codePage)
{
    UErrorCode status = U_ZERO_ERROR;
    return ucnv_openCCSID(EncodingTable::GetCCSIDFromCodePage(codePage).value_or(-1), UCNV_IBM, &status);
}

static std::shared_ptr<ICUEncoding> CreateDefaultEncoding(int32_t codePage)
{
    UConverter* converter = CreateInternalConverter(codePage);
    if (!converter)
    {
        return nullptr;
    }

    return std::make_shared<ICUEncoding>(converter, codePage);
}

static std::optional<int32_t> InternalConvert(const ICUEncoding& srcEncoding, const ICUEncoding& destEncoding, ReadOnlySpan<std::byte> srcBytes, Span<std::byte> destBytes)
{
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString converter(reinterpret_cast<const char*>(begin(srcBytes)), srcBytes.Length(), srcEncoding.GetInternalConverter(), status);
    if (U_FAILURE(status))
    {
        return std::nullopt;
    }

    int32_t convertedByteCount = converter.extract(reinterpret_cast<char*>(begin(destBytes)), destBytes.Length(),
        destEncoding.GetInternalConverter(), status);

    // U_BUFFER_OVERFLOW_ERROR occurs when we pass an empty destBytes, but it's not an error.
    if (U_FAILURE(status) && !(destBytes.IsEmpty() && status == U_BUFFER_OVERFLOW_ERROR))
    {
        return std::nullopt;
    }

    return convertedByteCount;
}

template <typename T>
static std::optional<T> InternalConvert(const ICUEncoding& srcEncoding, const ICUEncoding& destEncoding, ReadOnlySpan<std::byte> bytes)
{
    T fakeSizeBuffer;

    // For the performance, we resize the encoding result buffer by expected encoded byte size, so it might not
    // be the same as the real encoded byte size.
    int32_t expectedByteCount =
        destEncoding.GetMaxByteCount(srcEncoding.GetMaxCharCount(bytes.Length()) + 1);
    fakeSizeBuffer.resize(expectedByteCount / sizeof(fakeSizeBuffer[0]));

    std::optional<int> convertedByteCount = InternalConvert(srcEncoding, destEncoding, bytes,
        {reinterpret_cast<std::byte*>(fakeSizeBuffer.data()), static_cast<int32_t>(fakeSizeBuffer.size() * sizeof(fakeSizeBuffer[0]))});
    if (!convertedByteCount)
    {
        return std::nullopt;
    }

    fakeSizeBuffer.resize(*convertedByteCount / sizeof(fakeSizeBuffer[0]));

    return std::optional<T>(std::move(fakeSizeBuffer));
}

std::shared_ptr<Encoding> Encoding::GetEncoding(int32_t codePage)
{
    static std::unordered_map<int32_t, std::shared_ptr<Encoding>> encodings;
    static std::mutex mutex;
    [[maybe_unused]] auto lock = std::lock_guard(mutex);

    auto it = encodings.find(codePage);
    if (it != encodings.cend())
    {
        return it->second;
    }

    std::shared_ptr<ICUEncoding> encoding = CreateDefaultEncoding(codePage);
    if (!encoding)
    {
        return nullptr;
    }

    return encodings.emplace_hint(it, codePage, std::move(encoding))->second;
}

ICUEncoding::ICUEncoding(UConverter* converter, int32_t codePage) :
    ICUEncoding(converter, codePage, EncoderFallback::ReplacementFallback, DecoderFallback::ReplacementFallback)
{
}

ICUEncoding::ICUEncoding(UConverter* converter, int32_t codePage, std::shared_ptr<EncoderFallback> encoderFallback, std::shared_ptr<DecoderFallback> decoderFallback) :
    Encoding(codePage, &EncodingTable::GetCodePageDataItem(codePage), true, std::move(encoderFallback), std::move(decoderFallback)),
    converter_(converter, ucnv_close)
{
    RefreshEncoderFallback();
    RefreshDecoderFallback();
}

ICUEncoding::~ICUEncoding() = default;

std::shared_ptr<Encoding> ICUEncoding::Clone() const noexcept
{
    std::shared_ptr<ICUEncoding> ret = std::make_shared<ICUEncoding>(CreateInternalConverter(codePage_), codePage_, GetEncoderFallback(), GetDecoderFallback());

    // Cloned Encoding is writable.
    ret->isReadOnly_ = false;

    return ret;
}

bool ICUEncoding::IsSingleByte() const noexcept
{
    return GetMinCharByte() == 1 && GetMaxCharByte() == 1;
}

bool ICUEncoding::SetEncoderFallback(std::shared_ptr<EncoderFallback> fallback)
{
    if (!converter_ || !fallback || IsReadOnly())
    {
        return false;
    }

    encoderFallback_ = std::move(fallback);
    RefreshEncoderFallback();

    return true;
}

bool ICUEncoding::SetDecoderFallback(std::shared_ptr<DecoderFallback> fallback)
{
    if (!converter_ || !fallback || IsReadOnly()) 
    {
        return false;
    }

    decoderFallback_ = std::move(fallback);
    RefreshDecoderFallback();

    return true;
}

std::optional<int32_t> ICUEncoding::GetBytes(ReadOnlySpan<char16_t> chars, Span<std::byte> bytes) const
{
    return InternalConvert(reinterpret_cast<const ICUEncoding&>(Unicode()), *this,
        {reinterpret_cast<const std::byte*>(&chars[0]), static_cast<int32_t>(chars.Length() * sizeof(chars[0]))}, bytes);
}

std::optional<std::vector<std::byte>> ICUEncoding::GetBytes(ReadOnlySpan<char16_t> chars) const
{
    return InternalConvert<std::vector<std::byte>>(reinterpret_cast<const ICUEncoding&>(Unicode()), *this,
        {reinterpret_cast<const std::byte*>(&chars[0]), static_cast<int32_t>(chars.Length() * sizeof(chars[0]))});
}

std::optional<int32_t> ICUEncoding::GetByteCount(ReadOnlySpan<char16_t> chars) const
{
    return GetBytes(chars, {});
}

std::optional<int32_t> ICUEncoding::GetCharCount(ReadOnlySpan<std::byte> bytes) const
{
    return GetChars(bytes, {});
}

std::optional<int32_t> ICUEncoding::GetChars(ReadOnlySpan<std::byte> bytes, Span<char16_t> chars) const
{
    std::optional<int> convertedByteCount = InternalConvert(*this, reinterpret_cast<const ICUEncoding&>(Unicode()), bytes,
        {reinterpret_cast<std::byte*>(begin(chars)), static_cast<int32_t>(chars.Length() * sizeof(chars[0]))});
    if (!convertedByteCount)
    {
        return std::nullopt;
    }

    return *convertedByteCount / static_cast<int32_t>(sizeof(chars[0]));
}

std::optional<std::vector<char16_t>> ICUEncoding::GetChars(ReadOnlySpan<std::byte> bytes) const
{
    return InternalConvert<std::vector<char16_t>>(*this, reinterpret_cast<const ICUEncoding&>(Unicode()), bytes);
}

std::optional<std::u16string> ICUEncoding::GetString(ReadOnlySpan<std::byte> bytes) const
{
    auto str = InternalConvert<std::u16string>(*this, reinterpret_cast<const ICUEncoding&>(Unicode()), bytes);
    if (!str)
    {
        return std::nullopt;
    }

    // Remove unnecessary null terminate characters.
    while (!str->empty() && str->back() == '\0')
    {
        str->pop_back();
    }

    return str;
}

int32_t ICUEncoding::GetMaxByteCount(int32_t charCount) const noexcept
{
    return GetMaxCharByte() * (charCount + 1);
}

int32_t ICUEncoding::GetMaxCharCount(int32_t byteCount) const noexcept
{
    return byteCount / GetMinCharByte() + 1;
}

void ICUEncoding::RefreshEncoderFallback()
{
    UErrorCode status = U_ZERO_ERROR;
    ucnv_setFromUCallBack(converter_.get(), [](const void* context, UConverterFromUnicodeArgs* fromUArgs, const UChar* codeUnits, int32_t length, UChar32 codePoint, UConverterCallbackReason reason, UErrorCode* err)
    {
        if (reason > UCNV_IRREGULAR)
        {
            return;
        }

        *err = U_ZERO_ERROR;

        auto fallback = const_cast<EncoderFallback*>(static_cast<const EncoderFallback*>(context));
        fallback->Fallback({codeUnits, length});

    }, encoderFallback_.get(), nullptr, nullptr, &status);
}

void ICUEncoding::RefreshDecoderFallback()
{
    UErrorCode status = U_ZERO_ERROR;
    ucnv_setToUCallBack(converter_.get(), [](const void* context, UConverterToUnicodeArgs* toArgs, const char* codeUnits, int32_t length, UConverterCallbackReason reason, UErrorCode* err)
    {
        if (reason > UCNV_IRREGULAR)
        {
            return;
        }

        *err = U_ZERO_ERROR;

        auto fallback = const_cast<DecoderFallback*>(static_cast<const DecoderFallback*>(context));
        fallback->Fallback({reinterpret_cast<const std::byte*>(codeUnits), length});

    }, decoderFallback_.get(), nullptr, nullptr, &status);
}

int32_t ICUEncoding::GetMinCharByte() const noexcept
{
    return ucnv_getMinCharSize(converter_.get());
}

int32_t ICUEncoding::GetMaxCharByte() const noexcept
{
    return ucnv_getMaxCharSize(converter_.get());
}

UConverter* ICUEncoding::GetInternalConverter() const noexcept
{
    return converter_.get();
}

CS2CPP_NAMESPACE_END
