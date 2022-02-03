//#include <array>
//#include <unicode/locid.h>
//#include <unicode/normlzr.h>
//#include <unicode/ustring.h>
//
//#include "System/StringView.h"
//
//CS2CPP_NAMESPACE_BEGIN
//
//constexpr UNormalizationMode ConvertNormalizationFormToNative(NormalizationForm normalizationForm) noexcept
//{
//    constexpr std::pair<NormalizationForm, UNormalizationMode> table[] =
//    {
//        { NormalizationForm::FormC, UNORM_NFC, },
//        { NormalizationForm::FormD, UNORM_NFD, },
//        { NormalizationForm::FormKC, UNORM_NFKC, },
//        { NormalizationForm::FormKD, UNORM_NFKD, },
//    };
//
//    return nativeNormalizationFormTable[static_cast<int32_t>(normalizationForm)];
//}
//
//constexpr int32_t Sign(int32_t value) noexcept
//{
//    return value > 0 ? 1 : value < 0 ? -1 : 0;
//}
//
//StringView::StringView(const char16_t* value) :
//    StringView(value, std::char_traits<char16_t>::length(value))
//{
//}
//
//StringView::StringView(const char16_t* value, int32_t length) :
//    str_(value),
//    length_(length)
//{
//}
//
//const char16_t& StringView::operator[](int32_t index) const noexcept
//{
//    return str_[index];
//}
//
//String StringView::Join(StringView separator, ReadOnlySpan<StringView> values)
//{
//    if (values.IsEmpty())
//    {
//        return String::Empty;
//    }
//
//    icu::UnicodeString ret;
//    for (auto it = std::begin(values); it != std::end(values) - 1; ++it)
//    {
//        ret.append(it->data(), static_cast<int32_t>(it->length()));
//        ret.append(separator.data(), static_cast<int32_t>(separator.Length()));
//    }
//
//    auto it = std::end(values) - 1;
//    ret.append(it->data(), static_cast<int32_t>(it->length()));
//
//    return {std::move(ret)};
//}
//
//bool StringView::Equals(StringView value) const noexcept
//{
//    return Equals(*this, value);
//}
//
//std::vector<char16_t> StringView::ToCharArray() const
//{
//    return ToCharArray(0, length_);
//}
//
//std::vector<char16_t> StringView::ToCharArray(int32_t startIndex, int32_t length) const
//{
//    return std::vector<char16_t>(&str_[startIndex], &str_[length]);
//}
//
//std::vector<String> StringView::Split(char16_t separator, StringSplitOptions options) const
//{
//    return Split(separator, length_, options);
//}
//
//std::vector<String> StringView::Split(char16_t separator, int32_t count, StringSplitOptions options) const
//{
//    return Split({separator}, length_, options);
//}
//
//std::vector<String> StringView::Split(std::initializer_list<char16_t> separator, StringSplitOptions options) const
//{
//    return Split(separator, length_, options);
//}
//
//std::vector<String> StringView::Split(std::initializer_list<char16_t> separator, int32_t count, StringSplitOptions options) const
//{
//    std::vector<String> ret;
//    auto prevIndex = 0;
//    while (true)
//    {
//        int32_t currentIndex = IndexOfAny(separator, prevIndex, count);
//        if (currentIndex == -1)
//        {
//            int32_t substrLen = length_ - prevIndex;
//            if (!(substrLen == 0 && options == StringSplitOptions::RemoveEmptyEntries))
//            {
//                StringView substr(&str_[prevIndex], substrLen);
//                ret.emplace_back(substr);
//            }
//            break;
//        }
//
//        int32_t substrLen = currentIndex - prevIndex;
//        if (!(substrLen == 0 && options == StringSplitOptions::RemoveEmptyEntries))
//        {
//            StringView substr(&str_[prevIndex], substrLen);
//            ret.emplace_back(substr);
//        }
//        prevIndex = currentIndex + 1;
//    }
//
//    return ret;
//}
//
//String StringView::Substring(int32_t startIndex) const
//{
//    return Substring(startIndex, Length() - startIndex);
//}
//
//String StringView::Substring(int32_t startIndex, int32_t length) const
//{
//    return StringView(str_.tempSubString(startIndex, length));
//}
//
//String StringView::Trim() const
//{
//    return Trim(WhiteSpaceTable);
//}
//
//String StringView::Trim(char16_t trimChar) const
//{
//    return Trim({trimChar});
//}
//
//String StringView::Trim(std::initializer_list<char16_t> trimChars) const
//{
//    const auto trimStartOffset = GetTrimStartOffset(*this, trimChars);
//    const auto trimEndOffset = GetTrimEndOffset(*this, trimChars);
//
//    return StringView(str_.getBuffer() + trimStartOffset, trimEndOffset - trimStartOffset);
//}
//
//String StringView::TrimStart() const
//{
//    return TrimStart(WhiteSpaceTable);
//}
//
//String StringView::TrimStart(char16_t trimChar) const
//{
//    return TrimStart({trimChar});
//}
//
//String StringView::TrimStart(std::initializer_list<char16_t> trimChars) const
//{
//    const auto trimStartOffset = GetTrimStartOffset(*this, trimChars);
//    return StringView(str_.getBuffer() + trimStartOffset, length_ - trimStartOffset);
//}
//
//String StringView::TrimEnd() const
//{
//    return TrimEnd(WhiteSpaceTable);
//}
//
//String StringView::TrimEnd(char16_t trimChar) const
//{
//    return TrimEnd({trimChar});
//}
//
//String StringView::TrimEnd(std::initializer_list<char16_t> trimChars) const
//{
//    const auto trimEndOffset = GetTrimEndOffset(*this, trimChars);
//    return StringView(str_.getBuffer(), trimEndOffset);
//}
//
//bool StringView::IsNormalized(NormalizationForm normalizationForm) const noexcept
//{
//    UErrorCode errorCode = U_ZERO_ERROR;
//   return icu::Normalizer::isNormalized(str_, ConvertNormalizationFormToNative(normalizationForm), errorCode); errorCode == U_ZERO_ERROR;
//}
//
//String StringView::Normalize(NormalizationForm normalizationForm) const
//{
//    icu::UnicodeString ret;
//
//    auto errorCode = U_ZERO_ERROR;
//    icu::Normalizer::normalize(str_, detail::string::ConvertNormalizationFormToNative(normalizationForm), 0, ret,
//        errorCode);
//
//    if (errorCode != U_ZERO_ERROR)
//    {
//        return {};
//    }
//
//    return std::move(ret);
//}
//
//int32_t StringView::Compare(StringView strA, StringView strB) noexcept
//{
//    const auto length = int32_t(std::min(strA.Length(), strB.Length()));
//    for (int32_t i = 0; i < length; ++i)
//    {
//        if (strA[i] == strB[i])
//        {
//            continue;
//        }
//
//        auto c = u_tolower(strA[i]) == u_tolower(strB[i]) ? -1 : 1;
//        return strA[i] > strB[i] ? c : -c;
//    }
//
//    return strA.Length() > strB.Length() ? 1 : strA.Length() < strB.Length() ? -1 : 0;
//}
//
//int32_t StringView::Compare(StringView strA, StringView strB, bool ignoreCase) noexcept
//{
//    if (!ignoreCase)
//    {
//        return Compare(strA, strB);
//    }
//
//    auto errorCode = U_ZERO_ERROR;
//    return Sign(u_strCaseCompare(&strA[0], static_cast<int32_t>(strA.Length()), &strB[0], static_cast<int32_t>(strB.Length()), U_COMPARE_IGNORE_CASE, &errorCode));
//}
//
//int32_t StringView::CompareTo(StringView value) const noexcept
//{
//    return Compare(*this, value);
//}
//
//bool StringView::Contains(StringView value) const noexcept
//{
//    return IndexOf(value) >= 0;
//}
//
//bool StringView::Contains(char16_t value) const noexcept
//{
//    return IndexOf(value) >= 0;
//}
//
//bool StringView::StartsWith(StringView value) const noexcept
//{
//    return str_.startsWith(value.data(), 0, static_cast<int32_t>(value.Length()));
//}
//
//bool StringView::StartsWith(StringView value, bool ignoreCase) const noexcept
//{
//    if (!ignoreCase)
//    {
//        return StartsWith(value);
//    }
//
//    return str_.caseCompare(0, static_cast<int32_t>(value.Length()), value.data(), 0,
//        static_cast<int32_t>(value.Length()), 0) == 0;
//}
//
//bool StringView::EndsWith(StringView value) const noexcept
//{
//    return str_.endsWith(value.data(), 0, static_cast<int32_t>(value.Length()));
//}
//
//bool StringView::EndsWith(StringView value, bool ignoreCase) const noexcept
//{
//    if (!ignoreCase)
//    {
//        return EndsWith(value);
//    }
//
//    return str_.caseCompare(static_cast<int32_t>(length_ - value.Length()),
//        static_cast<int32_t>(value.Length()), value.data(), 0, static_cast<int32_t>(value.Length()), 0) == 0;
//}
//
//int32_t StringView::IndexOf(char16_t c) const noexcept
//{
//    return IndexOf(c, 0);
//}
//
//int32_t StringView::IndexOf(char16_t c, int32_t startIndex) const noexcept
//{
//    return IndexOf(c, startIndex, length_ - startIndex);
//}
//
//int32_t StringView::IndexOf(char16_t c, int32_t startIndex, int32_t count) const noexcept
//{
//    return str_.indexOf(c, startIndex, count);
//}
//
//int32_t StringView::IndexOf(StringView value) const noexcept
//{
//    return IndexOf(value, 0);
//}
//
//int32_t StringView::IndexOf(StringView value, int32_t startIndex) const noexcept
//{
//    return IndexOf(value, startIndex, length_ - startIndex);
//}
//
//int32_t StringView::IndexOf(StringView value, int32_t startIndex, int32_t count) const noexcept
//{
//    return str_.indexOf(&value[0], static_cast<int32_t>(value.Length()), startIndex, count);
//}
//
//int32_t StringView::IndexOfAny(std::initializer_list<char16_t> anyOf) const noexcept
//{
//    return IndexOfAny(anyOf, 0);
//}
//
//int32_t StringView::IndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex) const noexcept
//{
//    return IndexOfAny(anyOf, startIndex, length_ - startIndex);
//}
//
//int32_t StringView::IndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept
//{
//    int32_t prevIndex = startIndex;
//    int32_t currentIndex = startIndex;
//    char32_t c;
//    while (currentIndex < count)
//    {
//        U16_NEXT(str_.getBuffer(), currentIndex, count, c);
//
//        if (std::find(anyOf.begin(), anyOf.end(), c) != anyOf.end())
//        {
//            return prevIndex;
//        }
//
//        prevIndex = currentIndex;
//    }
//
//    return -1;
//}
//
//int32_t StringView::LastIndexOf(char16_t value) const noexcept
//{
//    return LastIndexOf(value, static_cast<int32_t>(length_) - 1);
//}
//
//int32_t StringView::LastIndexOf(char16_t value, int32_t startIndex) const noexcept
//{
//    return LastIndexOf(value, startIndex, startIndex + 1);
//}
//
//int32_t StringView::LastIndexOf(char16_t value, int32_t startIndex, int32_t count) const noexcept
//{
//    return str_.lastIndexOf(value, (startIndex + 1) - count, count);
//}
//
//int32_t StringView::LastIndexOf(StringView value) const noexcept
//{
//    return LastIndexOf(value, static_cast<int32_t>(length_) - 1);
//}
//
//int32_t StringView::LastIndexOf(StringView value, int32_t startIndex) const noexcept
//{
//    return LastIndexOf(value, startIndex, startIndex + 1);
//}
//
//int32_t StringView::LastIndexOf(StringView value, int32_t startIndex, int32_t count) const noexcept
//{
//    return str_.lastIndexOf(value.data(), static_cast<int32_t>(value.Length()), (startIndex + 1) - count, count);
//}
//
//int32_t StringView::LastIndexOfAny(std::initializer_list<char16_t> anyOf) const noexcept
//{
//    return LastIndexOfAny(anyOf, static_cast<int32_t>(length_) - 1);
//}
//
//int32_t StringView::LastIndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex) const noexcept
//{
//    return LastIndexOfAny(anyOf, startIndex, startIndex + 1);
//}
//
//int32_t StringView::LastIndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept
//{
//    int32_t currentIndex = startIndex + 1;
//    int32_t i = count;
//    char32_t c;
//    while (currentIndex >= 0 && i-- > 0)
//    {
//        U16_PREV(str_.getBuffer(), count, currentIndex, c);
//
//        if (std::find(anyOf.begin(), anyOf.end(), c) != anyOf.end())
//        {
//            return currentIndex;
//        }
//    }
//
//    return -1;
//}
//
//String StringView::PadLeft(int32_t totalWidth) const
//{
//    return PadLeft(totalWidth, ' ');
//}
//
//String StringView::PadLeft(int32_t totalWidth, char16_t paddingChar) const
//{
//    auto ret = str_;
//    ret.padLeading(totalWidth, paddingChar);
//
//    return std::move(ret);
//}
//
//String StringView::PadRight(int32_t totalWidth) const
//{
//    return PadRight(totalWidth, ' ');
//}
//
//String StringView::PadRight(int32_t totalWidth, char16_t paddingChar) const
//{
//    auto ret = str_;
//    ret.padTrailing(totalWidth, paddingChar);
//
//    return std::move(ret);
//}
//
//String StringView::ToLower() const
//{
//    return icu::UnicodeString(str_).toLower();
//}
//
//String StringView::ToLowerInvariant() const
//{
//    return icu::UnicodeString(str_).toLower(icu::Locale::getEnglish());
//}
//
//String StringView::ToUpper() const
//{
//    return icu::UnicodeString(str_).toUpper();
//}
//
//String StringView::ToUpperInvariant() const
//{
//    return icu::UnicodeString(str_).toLower(icu::Locale::getEnglish());
//}
//
//String StringView::ToString() const
//{
//    return str_;
//}
//
//String StringView::Insert(int32_t startIndex, StringView value) const
//{
//    auto ret = str_;
//    ret.insert(startIndex, value.data(), static_cast<int32_t>(value.size()));
//
//    return StringView(std::move(ret));
//}
//
//String StringView::Replace(char16_t oldChar, char16_t newChar) const
//{
//    auto ret = str_;
//    const auto index = ret.indexOf(oldChar);
//    if (index >= 0)
//    {
//        ret.replace(index, 1, newChar);
//    }
//
//    return StringView(std::move(ret));
//}
//
//String StringView::Replace(StringView oldValue, StringView newValue) const
//{
//    auto ret = str_;
//    const auto index = ret.indexOf(oldValue.data(), static_cast<int32_t>(oldValue.Length()), 0);
//    if (index >= 0)
//    {
//        ret.replace(index, static_cast<int32_t>(oldValue.Length()), newValue.data(),
//            static_cast<int32_t>(newValue.Length()));
//    }
//
//    return StringView(std::move(ret));
//}
//
//String StringView::Remove(int32_t startIndex, int32_t count) const
//{
//    auto ret = str_;
//    ret.remove(startIndex, count);
//
//    return StringView(std::move(ret));
//}
//
//void StringView::CopyTo(int32_t sourceIndex, Span<char16_t> destination, int32_t count) const
//{
//    memcpy(destination.data(), &str_.getBuffer()[sourceIndex], sizeof(char16_t) * count);
//}
//
//CS2CPP_NAMESPACE_END