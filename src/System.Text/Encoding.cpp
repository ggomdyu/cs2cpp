#include "System.Text/Encoding.h"
#include "System.Text/EncodingData.h"
#include "System.Text/EncodingTable.h"

CS2CPP_NAMESPACE_BEGIN

Encoding::Encoding(int32_t codePage, const CodePageDataItem* dataItem, bool isReadOnly, std::shared_ptr<EncoderFallback> encoderFallback, std::shared_ptr<DecoderFallback> decoderFallback) :
    codePage_(codePage),
    dataItem_(dataItem),
    isReadOnly_(isReadOnly),
    encoderFallback_(std::move(encoderFallback)),
    decoderFallback_(std::move(decoderFallback))
{
}

bool Encoding::operator==(const Encoding& rhs) const noexcept
{
    return Equals(rhs);
}

bool Encoding::operator!=(const Encoding& rhs) const noexcept
{
    return !Equals(rhs);
}

std::optional<int32_t> Encoding::Convert(const Encoding& srcEncoding, const Encoding& destEncoding, ReadOnlySpan<std::byte> srcBytes, Span<std::byte> destBytes)
{
    auto characters = srcEncoding.GetChars(srcBytes);
    if (!characters)
    {
        return std::nullopt;
    }

    return destEncoding.GetBytes(*characters, destBytes);
}

std::optional<std::vector<std::byte>> Encoding::Convert(const Encoding& srcEncoding, const Encoding& destEncoding, ReadOnlySpan<std::byte> bytes)
{
    auto characters = srcEncoding.GetChars(bytes);
    if (!characters)
    {
        return std::nullopt;
    }

    return destEncoding.GetBytes(*characters);
}

std::shared_ptr<Encoding> Encoding::GetEncoding(std::u16string_view encodingName)
{
    auto codePage = EncodingTable::GetCodePageFromName(encodingName);
    if (!codePage)
    {
        return nullptr;
    }

    return GetEncoding(*codePage);
}

std::shared_ptr<Encoding> Encoding::GetEncoding(int32_t codePage, std::shared_ptr<EncoderFallback> encoderFallback, std::shared_ptr<DecoderFallback> decoderFallback)
{
    auto encoding = GetEncoding(codePage);
    if (!encoding)
    {
        return nullptr;
    }

    auto clonedEncoding = encoding->Clone();
    clonedEncoding->SetEncoderFallback(std::move(encoderFallback));
    clonedEncoding->SetDecoderFallback(std::move(decoderFallback));

    return clonedEncoding;
}

std::shared_ptr<Encoding> Encoding::GetEncoding(std::u16string_view encodingName, std::shared_ptr<EncoderFallback> encoderFallback, std::shared_ptr<DecoderFallback> decoderFallback)
{
    auto codePage = EncodingTable::GetCodePageFromName(encodingName);
    if (!codePage)
    {
        return nullptr;
    }

    return GetEncoding(*codePage, std::move(encoderFallback), std::move(decoderFallback));
}

std::vector<EncodingInfo> Encoding::GetEncodings()
{
    return EncodingTable::GetEncodings();
}

const Encoding& Encoding::ASCII() noexcept
{
    static auto encoding = GetEncoding(CodePageASCII);
    return *encoding;
}

const Encoding& Encoding::Latin1() noexcept
{
    static auto encoding = GetEncoding(CodePageLatin1);
    return *encoding;
}

const Encoding& Encoding::UTF8() noexcept
{
    static auto encoding = GetEncoding(CodePageUTF8);
    return *encoding;
}

const Encoding& Encoding::Unicode() noexcept
{
    static auto encoding = GetEncoding(CodePageUTF16);
    return *encoding;
}

const Encoding& Encoding::BigEndianUnicode() noexcept
{
    static auto encoding = GetEncoding(CodePageUTF16BE);
    return *encoding;
}

const Encoding& Encoding::UTF32() noexcept
{
    static auto encoding = GetEncoding(CodePageUTF32);
    return *encoding;
}

const Encoding& Encoding::BigEndianUnicode32() noexcept
{
    static auto encoding = GetEncoding(CodePageUTF32BE);
    return *encoding;
}

ReadOnlySpan<std::byte> Encoding::Preamble() const noexcept
{
    return dataItem_->Preamble();
}

std::u16string_view Encoding::BodyName() const noexcept
{
    return dataItem_->BodyName();
}

std::u16string_view Encoding::EncodingName() const noexcept
{
    return dataItem_->DisplayName();
}

std::u16string_view Encoding::HeaderName() const noexcept
{
    return dataItem_->HeaderName();
}

std::u16string_view Encoding::WebName() const noexcept
{
    return dataItem_->WebName();
}

int32_t Encoding::WindowsCodePage() const noexcept
{
    return dataItem_->uiFamilyCodePage_;
}

bool Encoding::IsBrowserDisplay() const noexcept
{
    return (dataItem_->Flags() & EncodingData::MIMECONTF_BROWSER) != 0;
}

bool Encoding::IsBrowserSave() const noexcept
{
    return (dataItem_->Flags() & EncodingData::MIMECONTF_SAVABLE_BROWSER) != 0;
}

bool Encoding::IsMailNewsDisplay() const noexcept
{
    return (dataItem_->Flags() & EncodingData::MIMECONTF_MAILNEWS) != 0;
}

bool Encoding::IsMailNewsSave() const noexcept
{
    return (dataItem_->Flags() & EncodingData::MIMECONTF_SAVABLE_MAILNEWS) != 0;
}

bool Encoding::IsReadOnly() const noexcept
{
    return isReadOnly_;
}

int32_t Encoding::CodePage() const noexcept
{
    return codePage_;
}

int32_t Encoding::GetHashCode() const noexcept
{
    return codePage_;
}

std::shared_ptr<EncoderFallback> Encoding::GetEncoderFallback() const noexcept
{
    return encoderFallback_;
}

std::shared_ptr<DecoderFallback> Encoding::GetDecoderFallback() const noexcept
{
    return decoderFallback_;
}

bool Encoding::Equals(const Encoding& value) const noexcept
{
    return codePage_ == value.codePage_
        && GetEncoderFallback() == value.GetEncoderFallback()
        && GetDecoderFallback() == value.GetDecoderFallback();
}

CS2CPP_NAMESPACE_END