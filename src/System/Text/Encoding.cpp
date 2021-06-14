#include <unicode/unistr.h>

#include "System/Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::encoding
{

enum CCSID
{
    UTF8 = 1208,
    UTF16LE = 1202,
    UTF16BE = 1200,
    UTF16 = 1204,
    UTF32BE = 1232,
    UTF32LE = 1234,
    UTF32 = 1236,
    BOCU_1 = 1214,
    SCSU = 1212,
    US_ASCII = 367,
    KOI8_R = 878,
    EUC_JP = 954,
    EUC_KR = 970,
};

constexpr int32_t GetCCSID(std::u16string_view strippedEncodingName) noexcept
{
    auto hash = [](std::u16string_view str) constexpr noexcept -> size_t {
        size_t hashValue = 0;
        for (char16_t c : str)
            hashValue = 65599 * hashValue + c;

        return hashValue ^ (hashValue >> 16);
    };

    switch (hash(strippedEncodingName))
    {
    case hash(u"utf8"):
    case hash(u"ibm1208"):
        return CCSID::UTF8;
    case hash(u"utf16"):
    case hash(u"ibm1204"):
        return CCSID::UTF16;
    case hash(u"utf16be"):
    case hash(u"ibm1200"):
        return CCSID::UTF16BE;
    case hash(u"utf16le"):
    case hash(u"ibm1202"):
        return CCSID::UTF16LE;
    case hash(u"utf32le"):
    case hash(u"ibm1234"):
        return CCSID::UTF32LE;
    case hash(u"utf32be"):
    case hash(u"ibm1232"):
        return CCSID::UTF32BE;
    case hash(u"utf32"):
    case hash(u"ibm1236"):
        return CCSID::UTF32;
    case hash(u"usascii"):
    case hash(u"ibm367"):
        return CCSID::US_ASCII;
    case hash(u"koi8r"):
    case hash(u"ibm878"):
        return CCSID::KOI8_R;
    case hash(u"euckr"):
    case hash(u"ibm970"):
        return CCSID::EUC_KR;
    case hash(u"eucjp"):
    case hash(u"ibm954"):
        return CCSID::EUC_JP;
    case hash(u"scsu"):
        return CCSID::SCSU;
    case hash(u"bocu1"):
    case hash(u"ibm1214"):
        return CCSID::BOCU_1;
    case hash(u"big5hkscs"):
        return 1375;
    case hash(u"iso885913"):
        return 921;
    case hash(u"iso88598"):
        return 5012;
    case hash(u"ms950hkscs"):
        return 5471;
    case hash(u"windows1250"):
        return 5346;
    case hash(u"windows1251"):
        return 5347;
    case hash(u"windows1252"):
        return 5348;
    case hash(u"windows1253"):
        return 5349;
    case hash(u"windows1254"):
        return 5350;
    case hash(u"windows1255"):
        return 9447;
    case hash(u"windows1256"):
        return 9448;
    case hash(u"windows1257"):
        return 9449;
    case hash(u"windows1258"):
        return 5354;
    case hash(u"ibm037"):
        return 37;
    case hash(u"ibm273"):
        return 273;
    case hash(u"ibm277"):
        return 277;
    case hash(u"ibm278"):
        return 278;
    case hash(u"ibm280"):
        return 280;
    case hash(u"ibm284"):
        return 284;
    case hash(u"ibm285"):
        return 285;
    case hash(u"ibm297"):
        return 297;
    case hash(u"ibm420"):
        return 420;
    case hash(u"ibm424"):
        return 424;
    case hash(u"ibm437"):
        return 437;
    case hash(u"ibm500"):
        return 500;
    case hash(u"ibm737"):
        return 737;
    case hash(u"ibm775"):
        return 775;
    case hash(u"ibm838"):
        return 838;
    case hash(u"ibm850"):
        return 850;
    case hash(u"ibm852"):
        return 852;
    case hash(u"ibm855"):
        return 855;
    case hash(u"ibm856"):
        return 856;
    case hash(u"ibm857"):
        return 857;
    case hash(u"ibm00858"):
        return 858;
    case hash(u"ibm860"):
        return 860;
    case hash(u"ibm861"):
        return 861;
    case hash(u"ibm862"):
        return 862;
    case hash(u"ibm863"):
        return 863;
    case hash(u"ibm864"):
        return 864;
    case hash(u"ibm865"):
        return 865;
    case hash(u"ibm866"):
        return 866;
    case hash(u"ibm868"):
        return 868;
    case hash(u"ibm869"):
        return 869;
    case hash(u"ibm870"):
        return 870;
    case hash(u"ibm871"):
        return 871;
    case hash(u"ibm875"):
        return 875;
    case hash(u"ibm918"):
        return 918;
    case hash(u"ibm922"):
        return 922;
    case hash(u"ibm930"):
        return 930;
    case hash(u"ibm939"):
        return 939;
    case hash(u"ibm1006"):
        return 1006;
    case hash(u"ibm1026"):
        return 1026;
    case hash(u"ibm1047"):
        return 1047;
    case hash(u"ibm1098"):
        return 1098;
    case hash(u"ibm01140"):
        return 1140;
    case hash(u"ibm01141"):
        return 1141;
    case hash(u"ibm01142"):
        return 1142;
    case hash(u"ibm01143"):
        return 1143;
    case hash(u"ibm01144"):
        return 1144;
    case hash(u"ibm01145"):
        return 1145;
    case hash(u"ibm01146"):
        return 1146;
    case hash(u"ibm01147"):
        return 1147;
    case hash(u"ibm01148"):
        return 1148;
    case hash(u"ibm01149"):
        return 1149;
    case hash(u"ibm819"):
        return 819;
    case hash(u"ibm912"):
        return 912;
    case hash(u"ibm913"):
        return 913;
    case hash(u"ibm914"):
        return 914;
    case hash(u"ibm915"):
        return 915;
    case hash(u"ibm813"):
        return 813;
    case hash(u"ibm916"):
        return 916;
    case hash(u"ibm920"):
        return 920;
    case hash(u"ibm923"):
        return 923;
    case hash(u"ibm933"):
        return 933;
    case hash(u"ibm935"):
        return 935;
    case hash(u"ibm937"):
        return 937;
    case hash(u"ibm943"):
        return 943;
    case hash(u"ibm949"):
        return 949;
    case hash(u"ibm964"):
        return 964;
    case hash(u"ibm874"):
        return 874;
    case hash(u"ibm950"):
        return 950;
    case hash(u"ibm1124"):
        return 1124;
    case hash(u"ibm1025"):
        return 1025;
    case hash(u"ibm1089"):
        return 1089;
    case hash(u"ibm1097"):
        return 1097;
    case hash(u"ibm1112"):
        return 1112;
    case hash(u"ibm1122"):
        return 1122;
    case hash(u"ibm1123"):
        return 1123;
    case hash(u"ibm1381"):
        return 1381;
    case hash(u"ibm1383"):
        return 1383;
    case hash(u"ibm33722"):
    case hash(u"ibm337221208"):
        return 33722;
    default:
        return 0;
    }
}

constexpr gsl::span<const std::byte> GetByteOrderMark(int32_t codePage) noexcept
{
    constexpr std::byte utf8ByteOrderMark[] = {std::byte('\xef'), std::byte('\xbb'), std::byte('\xbf')};
    constexpr std::byte utf16LEByteOrderMark[] = {std::byte('\xff'), std::byte('\xfe')};
    constexpr std::byte utf16BEByteOrderMark[] = {std::byte('\xfe'), std::byte('\xff')};
    constexpr std::byte utf32ByteOrderMark[] = {std::byte('\xff'), std::byte('\xfe'), std::byte(0), std::byte(0)};

    switch (codePage)
    {
    case CCSID::UTF8:
        return {utf8ByteOrderMark, std::extent_v<decltype(utf8ByteOrderMark)>};

    case CCSID::UTF16:
    case CCSID::UTF16LE:
        return {utf16LEByteOrderMark, std::extent_v<decltype(utf16LEByteOrderMark)>};

    case CCSID::UTF16BE:
        return {utf16BEByteOrderMark, std::extent_v<decltype(utf16BEByteOrderMark)>};

    case CCSID::UTF32:
        return {utf32ByteOrderMark, std::extent_v<decltype(utf32ByteOrderMark)>};

    default:
        return {};
    }
}

std::pair<std::array<char16_t, UCNV_MAX_CONVERTER_NAME_LENGTH>, size_t> GetStrippedName(std::u16string_view name) noexcept
{
    std::array<char16_t, UCNV_MAX_CONVERTER_NAME_LENGTH> strippedName{};
    size_t len = 0;

    for (auto c : name)
    {
        if (isalpha(c))
            c = tolower(c);
        else if (!isdigit(c))
            continue;

        strippedName[len++] = c;
    }

    return {strippedName, len};
}

}

Encoding::Encoding(UConverter* converter) :
    _converter(converter, ucnv_close),
    _encodingName(GetUnicodeEncodingName(converter)),
    _preamble(detail::encoding::GetByteOrderMark(GetCodePage()))
{
    SetDefaultFallback();
}

Encoding::~Encoding()
{
}

bool Encoding::operator==(const Encoding& rhs) const noexcept
{
    return _converter == rhs._converter;
}

bool Encoding::operator!=(const Encoding& rhs) const noexcept
{
    return !(*this == rhs);
}

std::optional<Encoding> Encoding::Create(int32_t codePage)
{
    auto converter = CreateUConverter(codePage);
    if (!converter)
        return {};

    return Encoding(converter);
}

std::optional<Encoding> Encoding::Create(std::u16string_view encodingName)
{
    auto converter = CreateUConverter(encodingName);
    if (!converter)
        return {};

    return Encoding(converter);
}

const Encoding* Encoding::GetEncoding(int32_t codePage)
{
    auto it = _encodingTable.find(codePage);
    if (it != _encodingTable.cend())
        return &it->second;

    auto encoding = Encoding::Create(codePage);
    if (!encoding)
        return nullptr;

    return &_encodingTable.insert(it, {codePage, std::move(encoding.value())})->second;
}

const Encoding* Encoding::GetEncoding(std::u16string_view encodingName)
{
    using namespace detail::encoding;

    auto strippedEncodingName = GetStrippedName(encodingName);
    return GetEncoding(GetCCSID({strippedEncodingName.first.data(), strippedEncodingName.second}));
}

std::optional<std::vector<std::byte>> Encoding::Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::byte* bytes, int32_t count)
{
    auto status = U_ZERO_ERROR;
    auto str = icu::UnicodeString(reinterpret_cast<const char*>(bytes), count, srcEncoding._converter.get(), status);
    if (U_FAILURE(status))
        return {};

    auto encodedStrByteCount = str.extract(nullptr, 0, destEncoding._converter.get(), status);
    if (encodedStrByteCount == 0)
        return {};

    std::vector<std::byte> ret(encodedStrByteCount);
    status = U_ZERO_ERROR;
    str.extract(reinterpret_cast<char*>(&ret[0]), int32_t(ret.size()), destEncoding._converter.get(), status);

    return ret;
}

std::optional<std::vector<std::byte>> Encoding::Convert(const Encoding& srcEncoding, const Encoding& destEncoding, gsl::span<const std::byte> srcBytes)
{
    return Convert(srcEncoding, destEncoding, srcBytes.data(), static_cast<int32_t>(srcBytes.size()));
}

std::optional<int32_t> Encoding::Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::byte* srcBytes, int32_t srcByteCount, std::byte* destBytes, int32_t destByteCount)
{
    auto status = U_ZERO_ERROR;
    auto str = icu::UnicodeString(reinterpret_cast<const char*>(srcBytes), srcByteCount, srcEncoding._converter.get(), status);
    if (U_FAILURE(status))
    {
        if (destByteCount >= destEncoding.GetMinCharByte())
            memset(destBytes, 0, destEncoding.GetMinCharByte());

        return {};
    }

    auto encodedStrByteCount = str.extract(nullptr, 0, destEncoding._converter.get(), status);
    if (encodedStrByteCount > destByteCount || encodedStrByteCount == 0)
    {
        if (destByteCount >= destEncoding.GetMinCharByte())
            memset(destBytes, 0, destEncoding.GetMinCharByte());

        return {};
    }

    status = U_ZERO_ERROR;
    str.extract(reinterpret_cast<char*>(destBytes), destByteCount, destEncoding._converter.get(), status);

    return encodedStrByteCount;
}

std::optional<int32_t> Encoding::Convert(const Encoding& srcEncoding, const Encoding& destEncoding, gsl::span<const std::byte> srcBytes, gsl::span<std::byte> destBytes)
{
    return Convert(srcEncoding, destEncoding, &srcBytes[0], srcBytes.size(), &destBytes[0], destBytes.size());
}

void Encoding::SetEncoderFallback(EncoderFallback fallback)
{
    auto status = U_ZERO_ERROR;
    ucnv_setFromUCallBack(_converter.get(), fallback.callback, fallback.context, nullptr, nullptr, &status);
}

void Encoding::SetDecoderFallback(DecoderFallback fallback)
{
    auto status = U_ZERO_ERROR;
    ucnv_setToUCallBack(_converter.get(), fallback.callback, fallback.context, nullptr, nullptr, &status);
}

std::optional<std::vector<char32_t>> Encoding::GetChars(const std::byte* bytes, int32_t count) const
{
    std::vector<char32_t> ret;

    auto status = U_ZERO_ERROR;
    auto currentIt = reinterpret_cast<const char*>(bytes);
    auto endIt = reinterpret_cast<const char*>(bytes + count);
    while (currentIt < endIt)
    {
        auto c = ucnv_getNextUChar(_converter.get(), &currentIt, currentIt + count, &status);
        if (U_FAILURE(status))
            return {};

        if (c == 0)
            break;

        ret.push_back(static_cast<char32_t>(c));
    }

    return std::move(ret);
}

std::optional<std::vector<char32_t>> Encoding::GetChars(gsl::span<const std::byte> bytes) const
{
    return GetChars(bytes.data(), static_cast<int32_t>(bytes.size()));
}

std::optional<std::u16string> Encoding::GetString(const std::byte* bytes, int32_t count) const
{
    if (GetCodePage() == Unicode().GetCodePage())
        return std::u16string(reinterpret_cast<const char16_t*>(bytes), count / 2);

    auto convertedBytes = Convert(*this, Unicode(), bytes, count);
    if (convertedBytes)
        return std::u16string(reinterpret_cast<const char16_t*>(convertedBytes->data()), convertedBytes->size() / 2);

    return {};
}

std::optional<std::u16string> Encoding::GetString(gsl::span<const std::byte> bytes) const
{
    return GetString(bytes.data(), bytes.size());
}

gsl::span<const std::byte> Encoding::GetPreamble() const noexcept
{
    return _preamble;
}

std::optional<int32_t> Encoding::GetCharCount(const std::byte* bytes, int32_t count) const
{
    int32_t ret = 0;

    auto status = U_ZERO_ERROR;
    auto currentIt = reinterpret_cast<const char*>(&bytes[0]);
    auto endIt = reinterpret_cast<const char*>(bytes + count);
    while (currentIt < endIt)
    {
        auto c = ucnv_getNextUChar(_converter.get(), &currentIt, currentIt + count, &status);
        if (U_FAILURE(status))
            return {};

        if (c == UChar32{})
            break;

        ++ret;
    }

    return ret;
}

std::optional<int32_t> Encoding::GetCharCount(gsl::span<const std::byte> bytes) const
{
    return GetCharCount(&bytes[0], static_cast<int32_t>(bytes.size()));
}

std::u16string_view Encoding::GetEncodingName() const noexcept
{
    return _encodingName;
}

EncoderFallback Encoding::GetEncoderFallback() const noexcept
{
    EncoderFallback fallback{};
    ucnv_getFromUCallBack(_converter.get(), &fallback.callback, &fallback.context);

    return fallback;
}

DecoderFallback Encoding::GetDecoderFallback() const noexcept
{
    DecoderFallback fallback{};
    ucnv_getToUCallBack(_converter.get(), &fallback.callback, &fallback.context);

    return fallback;
}

int32_t Encoding::GetCodePage() const noexcept
{
    auto errorCode = U_ZERO_ERROR;
    return ucnv_getCCSID(_converter.get(), &errorCode);
}

int32_t Encoding::GetHashCode() const noexcept
{
    return GetCodePage();
}

int32_t Encoding::GetMaxByteCount(int32_t charCount) const noexcept
{
    return GetMaxCharByte() * (charCount + 1);
}

int32_t Encoding::GetMaxCharCount(int32_t byteCount) const noexcept
{
    // Didn't care about supplementary..
    return byteCount / GetMinCharByte() + 1;
}

const Encoding& Encoding::UTF8() noexcept
{
    static const auto& encoding = *GetEncoding(detail::encoding::UTF8);
    return encoding;
}

const Encoding& Encoding::Unicode() noexcept
{
    static const auto& encoding = *GetEncoding(detail::encoding::UTF16LE);
    return encoding;
}

const Encoding& Encoding::BigEndianUnicode() noexcept
{
    static const auto& encoding = *GetEncoding(detail::encoding::UTF16BE);
    return encoding;
}

const Encoding& Encoding::UTF32() noexcept
{
    static const auto& encoding = *GetEncoding(detail::encoding::UTF32);
    return encoding;
}

bool Encoding::IsSingleByte() const noexcept
{
    return GetMinCharByte() == 1 && GetMaxCharByte() == 1;
}

UConverter* Encoding::CreateUConverter(std::u16string_view encodingName)
{
    auto status = U_ZERO_ERROR;
    auto converter = ucnv_openU(encodingName.data(), &status);
    if (U_FAILURE(status))
        return nullptr;

    return converter;
}

UConverter* Encoding::CreateUConverter(int32_t codePage)
{
    auto status = U_ZERO_ERROR;
    auto converter = ucnv_openCCSID(codePage, UCNV_IBM, &status);
    if (U_FAILURE(status))
        return nullptr;

    return converter;
}

std::u16string Encoding::GetUnicodeEncodingName(UConverter* converter)
{
    auto status = U_ZERO_ERROR;
    auto beginIt = ucnv_getName(converter, &status);
    auto endIt = beginIt + strlen(beginIt);

    std::array<char16_t, UCNV_MAX_CONVERTER_NAME_LENGTH> newEncodingName{};
    std::copy(beginIt, endIt, newEncodingName.begin());

    return std::u16string(newEncodingName.data(), endIt - beginIt);
}

int32_t Encoding::GetMinCharByte() const noexcept
{
    return ucnv_getMinCharSize(_converter.get());
}

int32_t Encoding::GetMaxCharByte() const noexcept
{
    return ucnv_getMaxCharSize(_converter.get());
}

void Encoding::SetDefaultFallback() const noexcept
{
    auto status = U_ZERO_ERROR;
    ucnv_setToUCallBack(_converter.get(), UCNV_TO_U_CALLBACK_SKIP, nullptr, nullptr, nullptr, &status);
    ucnv_setFromUCallBack(_converter.get(), UCNV_FROM_U_CALLBACK_STOP, nullptr, nullptr, nullptr, &status);
}

CS2CPP_NAMESPACE_END
