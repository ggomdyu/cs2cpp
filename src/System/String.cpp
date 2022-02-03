// #include <unicode/locid.h>
// #include <unicode/normlzr.h>
// #include <unicode/ustring.h>

// #include "System/String.h"

// CS2CPP_NAMESPACE_BEGIN

// constexpr char16_t WhiteSpaceTable[] = {u' ', u'\u2000', u'\f', u'\n', u'\r', u'\t', u'\v'};

// constexpr UNormalizationMode ConvertNormalizationFormToNative(NormalizationForm normalizationForm) noexcept
// {
//     constexpr std::array<UNormalizationMode, 4> nativeNormalizationFormTable{
//         UNORM_NFC,
//         UNORM_NFD,
//         UNORM_NFKC,
//         UNORM_NFKD,
//     };

//     return nativeNormalizationFormTable[static_cast<int32_t>(normalizationForm)];
// }

// constexpr int32_t Sign(int32_t value) noexcept
// {
//     return value > 0 ? 1 : value < 0 ? -1 : 0;
// }

// const String String::Empty;

// String::String(std::u16string_view value) :
//     String(value.data(), static_cast<int32_t>(value.length()))
// {
// }

// String::String(std::u32string_view value) :
//     String(value.data(), static_cast<int32_t>(value.length()))
// {
// }

// String::String(const char16_t* value) :
//     String(value, std::char_traits<char16_t>::length(value))
// {
// }

// String::String(const char16_t* value, int32_t length) :
//     _str(value, length)
// {
// }

// String::String(const char32_t* value) :
//     String(value, std::char_traits<char32_t>::length(value))
// {
// }

// String::String(const char32_t* value, int32_t length) :
//     _str(icu::UnicodeString::fromUTF32(reinterpret_cast<const UChar32*>(value), length))
// {
// }

// String::String(char32_t c, int32_t count) :
//     _str(count, c, count)
// {
// }

// String::String(icu::UnicodeString value) noexcept :
//     _str(std::move(value))
// {
// }

// String& String::operator=(const char16_t* rhs)
// {
//     return operator=(std::u16string_view(rhs));
// }

// String& String::operator=(std::u16string_view rhs)
// {
//     _str.setTo(false, rhs.data(), static_cast<int32_t>(rhs.length()));
//     return *this;
// }

// String& String::operator=(String&& rhs) noexcept
// {
//     _str = std::move(rhs._str);
//     return *this;
// }

// String& String::operator+=(char16_t rhs)
// {
//     _str.append(rhs);
//     return *this;
// }

// String& String::operator+=(std::u16string_view rhs)
// {
//     _str.append(rhs.data(), static_cast<int32_t>(rhs.length()));
//     return *this;
// }

// const char16_t& String::operator[](int32_t index) const noexcept
// {
//     return _str.getTerminatedBuffer()[index];
// }

// String::operator std::u16string_view() const noexcept
// {
//     return std::u16string_view(&operator[](0), static_cast<int32_t>(_str.length()));
// }

// String String::Join(std::u16string_view separator, std::initializer_list<std::u16string_view> values)
// {
//     if (std::empty(values))
//     {
//         return {};
//     }

//     icu::UnicodeString ret;
//     for (auto it = std::cbegin(values); it != std::cend(values) - 1; ++it)
//     {
//         ret.append(it->data(), static_cast<int32_t>(it->length()));
//         ret.append(separator.data(), static_cast<int32_t>(separator.length()));
//     }

//     const auto it = std::cend(values) - 1;
//     ret.append(it->data(), static_cast<int32_t>(it->length()));

//     return String(std::move(ret));
// }

// bool String::Equals(std::u16string_view value) const noexcept
// {
//     return Equals(*this, value);
// }

// std::vector<char16_t> String::ToCharArray() const
// {
//     return ToCharArray(0, _str.length());
// }

// std::vector<char16_t> String::ToCharArray(int32_t startIndex, int32_t length) const
// {
//     std::vector<char16_t> ret(length);
//     for (int32_t i = startIndex, j = 0; i < startIndex + length; ++i, ++j)
//     {
//         ret[j] = _str.getBuffer()[j];
//     }

//     return ret;
// }

// int32_t String::GetHashCode() const noexcept
// {
//     if (_str.length() == 0)
//     {
//         return 0;
//     }

//     const auto* buffer = _str.getBuffer();
//     int32_t hashValue = 0;
//     for (int32_t i = 0; i < _str.length(); ++i)
//     {
//         hashValue = 65599 * hashValue + buffer[i];
//     }

//     return hashValue ^ (hashValue >> 16);
// }

// int32_t String::Length() const noexcept
// {
//     return _str.length();
// }

// std::vector<String> String::Split(char16_t separator, StringSplitOptions options) const
// {
//     return Split(separator, _str.length(), options);
// }

// std::vector<String> String::Split(char16_t separator, int32_t count, StringSplitOptions options) const
// {
//     return Split({separator}, _str.length(), options);
// }

// std::vector<String> String::Split(std::initializer_list<char16_t> separator, StringSplitOptions options) const
// {
//     return Split(separator, _str.length(), options);
// }

// std::vector<String> String::Split(std::initializer_list<char16_t> separator, int32_t count, StringSplitOptions options) const
// {
//     std::vector<String> ret;
//     auto prevIndex = 0;
//     while (true)
//     {
//         const auto currentIndex = IndexOfAny(separator, prevIndex, count);
//         if (currentIndex == -1)
//         {
//             const auto substrLen = static_cast<size_t>(_str.length()) - prevIndex;
//             if (!(substrLen == 0 && options == StringSplitOptions::RemoveEmptyEntries))
//             {
//                 const std::u16string_view substr(&_str.getBuffer()[prevIndex], substrLen);
//                 ret.emplace_back(substr);
//             }
//             break;
//         }

//         const auto substrLen = static_cast<size_t>(currentIndex) - prevIndex;
//         if (!(substrLen == 0 && options == StringSplitOptions::RemoveEmptyEntries))
//         {
//             const std::u16string_view substr(&_str.getBuffer()[prevIndex], substrLen);
//             ret.emplace_back(substr);
//         }
//         prevIndex = currentIndex + 1;
//     }

//     return ret;
// }

// String String::Substring(int32_t startIndex) const
// {
//     return Substring(startIndex, Length() - startIndex);
// }

// String String::Substring(int32_t startIndex, int32_t length) const
// {
//     return String(_str.tempSubString(startIndex, length));
// }

// String String::Trim() const
// {
//     return Trim(WhiteSpaceTable);
// }

// String String::Trim(char16_t trimChar) const
// {
//     return Trim({trimChar});
// }

// String String::Trim(std::initializer_list<char16_t> trimChars) const
// {
//     const auto trimStartOffset = GetTrimStartOffset(*this, trimChars);
//     const auto trimEndOffset = GetTrimEndOffset(*this, trimChars);

//     return String(_str.getBuffer() + trimStartOffset, trimEndOffset - trimStartOffset);
// }

// String String::TrimStart() const
// {
//     return TrimStart(WhiteSpaceTable);
// }

// String String::TrimStart(char16_t trimChar) const
// {
//     return TrimStart({trimChar});
// }

// String String::TrimStart(std::initializer_list<char16_t> trimChars) const
// {
//     const auto trimStartOffset = GetTrimStartOffset(*this, trimChars);
//     return String(_str.getBuffer() + trimStartOffset, _str.length() - trimStartOffset);
// }

// String String::TrimEnd() const
// {
//     return TrimEnd(WhiteSpaceTable);
// }

// String String::TrimEnd(char16_t trimChar) const
// {
//     return TrimEnd({trimChar});
// }

// String String::TrimEnd(std::initializer_list<char16_t> trimChars) const
// {
//     const auto trimEndOffset = GetTrimEndOffset(*this, trimChars);
//     return String(_str.getBuffer(), trimEndOffset);
// }

// bool String::IsNormalized(NormalizationForm normalizationForm) const noexcept
// {
//     auto errorCode = U_ZERO_ERROR;
//     if (const auto ret = icu::Normalizer::isNormalized(_str,
//         detail::string::ConvertNormalizationFormToNative(normalizationForm), errorCode); errorCode == U_ZERO_ERROR)
//     {
//         return !!ret;
//     }

//     return false;
// }

// String String::Normalize(NormalizationForm normalizationForm) const
// {
//     icu::UnicodeString ret;

//     auto errorCode = U_ZERO_ERROR;
//     icu::Normalizer::normalize(_str, detail::string::ConvertNormalizationFormToNative(normalizationForm), 0, ret,
//         errorCode);

//     if (errorCode != U_ZERO_ERROR)
//     {
//         return {};
//     }

//     return std::move(ret);
// }

// int32_t String::Compare(std::u16string_view strA, std::u16string_view strB) noexcept
// {
//     const auto length = int32_t(std::min(strA.length(), strB.length()));
//     for (int32_t i = 0; i < length; ++i)
//     {
//         if (strA[i] == strB[i])
//         {
//             continue;
//         }

//         auto c = u_tolower(strA[i]) == u_tolower(strB[i]) ? -1 : 1;
//         return strA[i] > strB[i] ? c : -c;
//     }

//     return strA.length() > strB.length() ? 1 : strA.length() < strB.length() ? -1 : 0;
// }

// int32_t String::Compare(std::u16string_view strA, std::u16string_view strB, bool ignoreCase) noexcept
// {
//     if (!ignoreCase)
//     {
//         return Compare(strA, strB);
//     }

//     auto errorCode = U_ZERO_ERROR;
//     return detail::string::Sign(u_strCaseCompare(strA.data(), static_cast<int32_t>(strA.length()), strB.data(),
//         static_cast<int32_t>(strB.length()), U_COMPARE_IGNORE_CASE, &errorCode));
// }

// int32_t String::CompareTo(std::u16string_view value) const noexcept
// {
//     return Compare(*this, value);
// }

// bool String::Contains(std::u16string_view value) const noexcept
// {
//     return IndexOf(value) >= 0;
// }

// bool String::Contains(char16_t value) const noexcept
// {
//     return IndexOf(value) >= 0;
// }

// bool String::StartsWith(std::u16string_view value) const noexcept
// {
//     return _str.startsWith(value.data(), 0, static_cast<int32_t>(value.length()));
// }

// bool String::StartsWith(std::u16string_view value, bool ignoreCase) const noexcept
// {
//     if (!ignoreCase)
//     {
//         return StartsWith(value);
//     }

//     return _str.caseCompare(0, static_cast<int32_t>(value.length()), value.data(), 0,
//         static_cast<int32_t>(value.length()), 0) == 0;
// }

// bool String::EndsWith(std::u16string_view value) const noexcept
// {
//     return _str.endsWith(value.data(), 0, static_cast<int32_t>(value.length()));
// }

// bool String::EndsWith(std::u16string_view value, bool ignoreCase) const noexcept
// {
//     if (!ignoreCase)
//     {
//         return EndsWith(value);
//     }

//     return _str.caseCompare(static_cast<int32_t>(_str.length() - value.length()),
//         static_cast<int32_t>(value.length()), value.data(), 0, static_cast<int32_t>(value.length()), 0) == 0;
// }

// int32_t String::IndexOf(char16_t c) const noexcept
// {
//     return IndexOf(c, 0);
// }

// int32_t String::IndexOf(char16_t c, int32_t startIndex) const noexcept
// {
//     return IndexOf(c, startIndex, _str.length() - startIndex);
// }

// int32_t String::IndexOf(char16_t c, int32_t startIndex, int32_t count) const noexcept
// {
//     return _str.indexOf(c, startIndex, count);
// }

// int32_t String::IndexOf(std::u16string_view value) const noexcept
// {
//     return IndexOf(value, 0);
// }

// int32_t String::IndexOf(std::u16string_view value, int32_t startIndex) const noexcept
// {
//     return IndexOf(value, startIndex, _str.length() - startIndex);
// }

// int32_t String::IndexOf(std::u16string_view value, int32_t startIndex, int32_t count) const noexcept
// {
//     return _str.indexOf(value.data(), static_cast<int32_t>(value.length()), startIndex, count);
// }

// int32_t String::IndexOfAny(std::initializer_list<char16_t> anyOf) const noexcept
// {
//     return IndexOfAny(anyOf, 0);
// }

// int32_t String::IndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex) const noexcept
// {
//     return IndexOfAny(anyOf, startIndex, _str.length() - startIndex);
// }

// int32_t String::IndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept
// {
//     int32_t prevIndex = startIndex;
//     int32_t currentIndex = startIndex;
//     char32_t c;
//     while (currentIndex < count)
//     {
//         U16_NEXT(_str.getBuffer(), currentIndex, count, c);

//         if (std::find(anyOf.begin(), anyOf.end(), c) != anyOf.end())
//         {
//             return prevIndex;
//         }

//         prevIndex = currentIndex;
//     }

//     return -1;
// }

// int32_t String::LastIndexOf(char16_t value) const noexcept
// {
//     return LastIndexOf(value, static_cast<int32_t>(_str.length()) - 1);
// }

// int32_t String::LastIndexOf(char16_t value, int32_t startIndex) const noexcept
// {
//     return LastIndexOf(value, startIndex, startIndex + 1);
// }

// int32_t String::LastIndexOf(char16_t value, int32_t startIndex, int32_t count) const noexcept
// {
//     return _str.lastIndexOf(value, (startIndex + 1) - count, count);
// }

// int32_t String::LastIndexOf(std::u16string_view value) const noexcept
// {
//     return LastIndexOf(value, static_cast<int32_t>(_str.length()) - 1);
// }

// int32_t String::LastIndexOf(std::u16string_view value, int32_t startIndex) const noexcept
// {
//     return LastIndexOf(value, startIndex, startIndex + 1);
// }

// int32_t String::LastIndexOf(std::u16string_view value, int32_t startIndex, int32_t count) const noexcept
// {
//     return _str.lastIndexOf(value.data(), static_cast<int32_t>(value.length()), (startIndex + 1) - count, count);
// }

// int32_t String::LastIndexOfAny(std::initializer_list<char16_t> anyOf) const noexcept
// {
//     return LastIndexOfAny(anyOf, static_cast<int32_t>(_str.length()) - 1);
// }

// int32_t String::LastIndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex) const noexcept
// {
//     return LastIndexOfAny(anyOf, startIndex, startIndex + 1);
// }

// int32_t String::LastIndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept
// {
//     int32_t currentIndex = startIndex + 1;
//     int32_t i = count;
//     char32_t c;
//     while (currentIndex >= 0 && i-- > 0)
//     {
//         U16_PREV(_str.getBuffer(), count, currentIndex, c);

//         if (std::find(anyOf.begin(), anyOf.end(), c) != anyOf.end())
//         {
//             return currentIndex;
//         }
//     }

//     return -1;
// }

// String String::PadLeft(int32_t totalWidth) const
// {
//     return PadLeft(totalWidth, ' ');
// }

// String String::PadLeft(int32_t totalWidth, char16_t paddingChar) const
// {
//     auto ret = _str;
//     ret.padLeading(totalWidth, paddingChar);

//     return std::move(ret);
// }

// String String::PadRight(int32_t totalWidth) const
// {
//     return PadRight(totalWidth, ' ');
// }

// String String::PadRight(int32_t totalWidth, char16_t paddingChar) const
// {
//     auto ret = _str;
//     ret.padTrailing(totalWidth, paddingChar);

//     return std::move(ret);
// }

// String String::ToLower() const
// {
//     return icu::UnicodeString(_str).toLower();
// }

// String String::ToLowerInvariant() const
// {
//     return icu::UnicodeString(_str).toLower(icu::Locale::getEnglish());
// }

// String String::ToUpper() const
// {
//     return icu::UnicodeString(_str).toUpper();
// }

// String String::ToUpperInvariant() const
// {
//     return icu::UnicodeString(_str).toLower(icu::Locale::getEnglish());
// }

// String String::ToString() const
// {
//     return _str;
// }

// String String::Insert(int32_t startIndex, std::u16string_view value) const
// {
//     auto ret = _str;
//     ret.insert(startIndex, value.data(), static_cast<int32_t>(value.size()));

//     return String(std::move(ret));
// }

// String String::Replace(char16_t oldChar, char16_t newChar) const
// {
//     auto ret = _str;
//     const auto index = ret.indexOf(oldChar);
//     if (index >= 0)
//     {
//         ret.replace(index, 1, newChar);
//     }

//     return String(std::move(ret));
// }

// String String::Replace(std::u16string_view oldValue, std::u16string_view newValue) const
// {
//     auto ret = _str;
//     const auto index = ret.indexOf(oldValue.data(), static_cast<int32_t>(oldValue.length()), 0);
//     if (index >= 0)
//     {
//         ret.replace(index, static_cast<int32_t>(oldValue.length()), newValue.data(),
//             static_cast<int32_t>(newValue.length()));
//     }

//     return String(std::move(ret));
// }

// String String::Remove(int32_t startIndex, int32_t count) const
// {
//     auto ret = _str;
//     ret.remove(startIndex, count);

//     return String(std::move(ret));
// }

// void String::CopyTo(int32_t sourceIndex, gsl::span<char16_t> destination, int32_t count) const
// {
//     memcpy(destination.data(), &_str.getBuffer()[sourceIndex], sizeof(char16_t) * count);
// }

//String operator+(const String& lhs, StringView rhs)
//{
//    return String(lhs) += rhs;
//}
//
//String operator+(const String& lhs, const char16_t* rhs)
//{
//    return operator+(lhs, StringView(rhs));
//}
//
//String operator+(StringView lhs, const String& rhs)
//{
//    return String(lhs) += rhs;
//}
//
//String operator+(const char16_t* lhs, const String& rhs)
//{
//    return operator+(StringView(lhs), rhs);
//}
//
//String operator+(const String& lhs, const String& rhs)
//{
//    return String(lhs) += rhs;
//}
//
//bool operator==(const String& lhs, StringView rhs) noexcept
//{
//    return lhs.Equals(rhs);
//}
//
//bool operator==(const String& lhs, const char16_t* rhs) noexcept
//{
//    return lhs.Equals(rhs);
//}
//
//bool operator==(StringView lhs, const String& rhs) noexcept
//{
//    return rhs.Equals(lhs);
//}
//
//bool operator==(const char16_t* lhs, const String& rhs) noexcept
//{
//    return rhs.Equals(lhs);
//}
//
//bool operator==(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.Equals(rhs);
//}
//
//bool operator!=(const String& lhs, StringView rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//bool operator!=(const String& lhs, const char16_t* rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//bool operator!=(StringView lhs, const String& rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//bool operator!=(const char16_t* lhs, const String& rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//bool operator!=(const String& lhs, const String& rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//bool operator<(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.GetHashCode() < rhs.GetHashCode();
//}
//
//bool operator<=(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.GetHashCode() <= rhs.GetHashCode();
//}
//
//bool operator>(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.GetHashCode() > rhs.GetHashCode();
//}
//
//bool operator>=(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.GetHashCode() >= rhs.GetHashCode();
//}
//
//const char16_t* begin(const CS2CPP_NAMESPACE::String& str) noexcept
//{
//    return &str[0];
//}
//
//const char16_t* end(const CS2CPP_NAMESPACE::String& str) noexcept
//{
//    return &str[0] + str.Length();
//}

//CS2CPP_NAMESPACE_END
