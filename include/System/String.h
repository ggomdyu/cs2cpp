//#pragma once
//
//#include "System/NormalizationForm.h"
//#include "System/ReadOnlySpan.h"
//#include "System/StringSplitOptions.h"
//
//CS2CPP_NAMESPACE_BEGIN
//
//class StringView;
//
//class String final
//{
//public:
//    String(const String& rhs);
//    String(const char16_t* value);
//    String(const char16_t* value, int32_t length);
//    String(char16_t c, int32_t count);
//    String(StringView value);
//
//public:
//    bool operator==(const String& rhs) noexcept;
//    bool operator!=(const String& rhs) noexcept;
//    const char16_t& operator[](int32_t index) const noexcept;
//
//public:
//    static String Join(StringView separator, ReadOnlySpan<StringView> value);
//    static String Join(StringView separator, ReadOnlySpan<StringView> value, int32_t startIndex, int32_t count);
//    bool Equals(StringView value) const noexcept;
//    static bool Equals(StringView a, StringView b) noexcept;
//    void CopyTo(int32_t sourceIndex, Span<char16_t> dest, int32_t destinationIndex, int32_t count) const;
//    std::vector<char16_t> ToCharArray() const;
//    std::vector<char16_t> ToCharArray(int32_t startIndex, int32_t length) const;
//    static constexpr bool IsNullOrEmpty(StringView value) noexcept;
//    static constexpr bool IsNullOrWhiteSpace(StringView value) noexcept;
//    int32_t GetHashCode() const noexcept;
//    int32_t Length() const noexcept;
//    std::vector<String> Split(char16_t separator, StringSplitOptions options = StringSplitOptions::None) const;
//    std::vector<String> Split(ReadOnlySpan<char16_t> separator, StringSplitOptions options = StringSplitOptions::None) const;
//    std::vector<String> Split(ReadOnlySpan<char16_t> separator, int32_t count, StringSplitOptions options = StringSplitOptions::None) const;
//    String Substring(int32_t startIndex) const;
//    String Substring(int32_t startIndex, int32_t length) const;
//    String Trim() const;
//    String Trim(char16_t trimChar) const;
//    String Trim(ReadOnlySpan<char16_t> trimChars) const;
//    String TrimStart() const;
//    String TrimStart(char16_t trimChar) const;
//    String TrimStart(ReadOnlySpan<char16_t> trimChars) const;
//    String TrimEnd() const;
//    String TrimEnd(char16_t trimChar) const;
//    String TrimEnd(ReadOnlySpan<char16_t> trimChars) const;
//    bool IsNormalized() const noexcept;
//    bool IsNormalized(NormalizationForm normalizationForm) const noexcept;
//    String Normalize() const;
//    String Normalize(NormalizationForm normalizationForm) const;
//    static int32_t Compare(StringView strA, StringView strB) noexcept;
//    static int32_t Compare(StringView strA, StringView strB, bool ignoreCase) noexcept;
//    static int32_t Compare(StringView strA, StringView strB, int32_t length) noexcept;
//    static int32_t Compare(StringView strA, StringView strB, int32_t length, bool ignoreCase) noexcept;
//    int32_t CompareTo(StringView str) const noexcept;
//    static int32_t CompareOrdinal(StringView strA, StringView strB) noexcept;
//    static int32_t CompareOrdinal(StringView strA, StringView strB, bool ignoreCase) noexcept;
//    bool Contains(char16_t value) const noexcept;
//    bool Contains(StringView value) const noexcept;
//    bool EndsWith(StringView value) const noexcept;
//    bool EndsWith(StringView value, bool ignoreCase) const noexcept;
//    int32_t IndexOf(char16_t c) const noexcept;
//    int32_t IndexOf(char16_t c, int32_t startIndex) const noexcept;
//    int32_t IndexOf(char16_t c, int32_t startIndex, int32_t count) const noexcept;
//    int32_t IndexOfAny(ReadOnlySpan<char16_t> anyOf) const noexcept;
//    int32_t IndexOfAny(ReadOnlySpan<char16_t> anyOf, int32_t startIndex) const noexcept;
//    int32_t IndexOfAny(ReadOnlySpan<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept;
//    int32_t IndexOf(StringView value) const noexcept;
//    int32_t IndexOf(StringView value, int32_t startIndex) const noexcept;
//    int32_t IndexOf(StringView value, int32_t startIndex, int32_t count) const noexcept;
//    int32_t LastIndexOf(char16_t value) const noexcept;
//    int32_t LastIndexOf(char16_t value, int32_t startIndex) const noexcept;
//    int32_t LastIndexOf(char16_t value, int32_t startIndex, int32_t count) const noexcept;
//    int32_t LastIndexOfAny(ReadOnlySpan<char16_t> anyOf) const noexcept;
//    int32_t LastIndexOfAny(ReadOnlySpan<char16_t> anyOf, int32_t startIndex) const noexcept;
//    int32_t LastIndexOfAny(ReadOnlySpan<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept;
//    int32_t LastIndexOf(StringView value) const noexcept;
//    int32_t LastIndexOf(StringView value, int32_t startIndex) const noexcept;
//    int32_t LastIndexOf(StringView value, int32_t startIndex, int32_t count) const noexcept;
//    String PadLeft(int32_t totalWidth) const;
//    String PadLeft(int32_t totalWidth, char16_t paddingChar) const;
//    String PadRight(int32_t totalWidth) const;
//    String PadRight(int32_t totalWidth, char16_t paddingChar) const;
//    bool StartsWith(StringView value) const noexcept;
//    bool StartsWith(StringView value, bool ignoreCase) const noexcept;
//    String ToLower() const;
//    String ToLowerInvariant() const;
//    String ToUpper() const;
//    String ToUpperInvariant() const;
//    String ToString() const;
//    String Trim() const;
//    String Insert(int32_t startIndex, StringView value) const;
//    String Replace(char16_t oldChar, char16_t newChar) const;
//    String Replace(StringView oldValue, StringView newValue) const;
//    String Remove(int32_t startIndex, int32_t count) const;
//    template <typename... Ts>
//    static String Format(StringView format, Ts&&... args);
//    template <typename... Ts>
//    static String Concat(Ts&&... args);
//
//private:
//    static constexpr int32_t GetTrimStartOffset(StringView value, std::initializer_list<char16_t> trimChars) noexcept;
//    static constexpr int32_t GetTrimEndOffset(StringView value, std::initializer_list<char16_t> trimChars) noexcept;
//    template <typename... Ts>
//    static void InternalConcat(icu::UnicodeString& str, char16_t value, Ts&&... args);
//    template <typename... Ts>
//    static void InternalConcat(icu::UnicodeString& str, StringView value, Ts&&... args);
//
//public:
//    static const String Empty;
//
//private:
//    static constexpr char16_t WhiteSpaceTable[] = {u' ', u'\u2000', u'\f', u'\n', u'\r', u'\t', u'\v'};
//    static constexpr int32_t StackBufferSize = 16;
//
//    union
//    {
//        char16_t* str_;
//        char16_t buffer_[SSOBufferSize];
//    } buffer_;
//    int32_t length_;
//};
//
//constexpr bool String::Equals(StringView strA, StringView strB) noexcept
//{
//    return strA == strB;
//}
//
//constexpr bool String::IsNullOrEmpty(StringView value) noexcept
//{
//    return value.Length() == 0;
//}
//
//constexpr bool String::IsNullOrWhiteSpace(StringView value) noexcept
//{
//    if (IsNullOrEmpty(value))
//    {
//        return true;
//    }
//
//    if (GetTrimStartOffset(value, WhiteSpaceTable) == static_cast<int32_t>(value.Length()) ||
//        GetTrimEndOffset(value, WhiteSpaceTable) == 0)
//    {
//        return true;
//    }
//
//    return false;
//}
//
//constexpr int32_t String::CompareOrdinal(StringView strA, StringView strB) noexcept
//{
//    const auto* a = strA.data();
//    const auto* b = strB.data();
//    if (a == b && strA.length() == strB.length())
//    {
//        return 0;
//    }
//
//    if (strA.length() == 0)
//    {
//        return -1;
//    }
//
//    if (strB.length() == 0)
//    {
//        return 1;
//    }
//
//    int32_t length = std::min(strA.length(), strB.length());
//    int32_t diffOffset = -1;
//
//    while (length >= 10)
//    {
//        if (*reinterpret_cast<const int32_t*>(a) != *reinterpret_cast<const int32_t*>(b))
//        {
//            diffOffset = 0;
//            break;
//        }
//
//        if (*reinterpret_cast<const int32_t*>(a + 2) != *reinterpret_cast<const int32_t*>(b + 2))
//        {
//            diffOffset = 2;
//            break;
//        }
//
//        if (*reinterpret_cast<const int32_t*>(a + 4) != *reinterpret_cast<const int32_t*>(b + 4))
//        {
//            diffOffset = 4;
//            break;
//        }
//
//        if (*reinterpret_cast<const int32_t*>(a + 6) != *reinterpret_cast<const int32_t*>(b + 6))
//        {
//            diffOffset = 6;
//            break;
//        }
//
//        if (*reinterpret_cast<const int32_t*>(a + 8) != *reinterpret_cast<const int32_t*>(b + 8))
//        {
//            diffOffset = 8;
//            break;
//        }
//        a += 10;
//        b += 10;
//        length -= 10;
//    }
//
//    if (diffOffset != -1)
//    {
//        a += diffOffset;
//        b += diffOffset;
//        const auto order = *a - *b;
//        if (order != 0)
//        {
//            return order;
//        }
//        return *(a + 1) - *(b + 1);
//    }
//
//    while (length > 0)
//    {
//        if (*reinterpret_cast<const int32_t*>(a) != *reinterpret_cast<const int32_t*>(b))
//        {
//            break;
//        }
//        a += 2;
//        b += 2;
//        length -= 2;
//    }
//
//    if (length > 0)
//    {
//        const auto c = *a - *b;
//        if (c != 0)
//        {
//            return c;
//        }
//        return *(a + 1) - *(b + 1);
//    }
//
//    return int32_t(strA.length() - strB.length());
//}
//
//template <typename... Ts>
//String String::Format(StringView format, Ts&&... args)
//{
//    fmt::basic_memory_buffer<char16_t> buffer;
//    fmt::format_to(buffer, format, std::forward<Ts>(args)...);
//
//    return icu::UnicodeString(buffer.data(), static_cast<int32_t>(buffer.size()));
//}
//
//template <typename T, typename... Ts>
//String String::Concat(const T& arg, Ts&&... args)
//{
//    icu::UnicodeString ret;
//    InternalConcat(ret, arg, std::forward<Ts>(args)...);
//
//    return std::move(ret);
//}
//
//constexpr int32_t String::GetTrimStartOffset(StringView value, std::initializer_list<char16_t> trimChars) noexcept
//{
//    const char16_t* array = value.data();
//    int32_t length = static_cast<int32_t>(value.length());
//    int32_t startOffset = 0;
//    int32_t i = 0;
//    char32_t c = 0;
//
//    do
//    {
//        startOffset = i;
//        if (i >= length)
//        {
//            break;
//        }
//        U16_NEXT(array, i, length, c);
//
//    } while (std::find(trimChars.begin(), trimChars.end(), c) != trimChars.end());
//
//    return startOffset;
//}
//
//constexpr int32_t String::GetTrimEndOffset(StringView value, std::initializer_list<char16_t> trimChars) noexcept
//{
//    const char16_t* array = value.data();
//    int32_t oldLength = static_cast<int32_t>(value.length());
//    int32_t i = oldLength, length = 0;
//    char32_t c = 0;
//
//    do
//    {
//        length = i;
//        if (i <= 0)
//        {
//            break;
//        }
//        U16_PREV(array, 0, i, c);
//    } while (std::find(trimChars.begin(), trimChars.end(), c) != trimChars.end());
//
//    return (length < oldLength) ? length : oldLength;
//}
//
//template <typename... Ts>
//void String::InternalConcat(icu::UnicodeString& str, char16_t value, Ts&&... args)
//{
//    str.append(value);
//    if constexpr (sizeof...(Ts) > 0)
//    {
//        InternalConcat(str, std::forward<Ts>(args)...);
//    }
//}
//
//template <typename... Ts>
//void String::InternalConcat(icu::UnicodeString& str, StringView value, Ts&&... args)
//{
//    str.append(value.data(), static_cast<int32_t>(value.length()));
//    if constexpr (sizeof...(Ts) > 0)
//    {
//        InternalConcat(str, std::forward<Ts>(args)...);
//    }
//}
//
//inline String operator+(const String& lhs, StringView rhs)
//{
//    return String(lhs) += rhs;
//}
//
//inline String operator+(const String& lhs, const char16_t* rhs)
//{
//    return operator+(lhs, StringView(rhs));
//}
//
//inline String operator+(StringView lhs, const String& rhs)
//{
//    return String(lhs) += rhs;
//}
//
//inline String operator+(const char16_t* lhs, const String& rhs)
//{
//    return operator+(StringView(lhs), rhs);
//}
//
//inline String operator+(const String& lhs, const String& rhs)
//{
//    return String(lhs) += rhs;
//}
//
//inline bool operator==(const String& lhs, StringView rhs) noexcept
//{
//    return lhs.Equals(rhs);
//}
//
//inline bool operator==(const String& lhs, const char16_t* rhs) noexcept
//{
//    return lhs.Equals(rhs);
//}
//
//inline bool operator==(StringView lhs, const String& rhs) noexcept
//{
//    return rhs.Equals(lhs);
//}
//
//inline bool operator==(const char16_t* lhs, const String& rhs) noexcept
//{
//    return rhs.Equals(lhs);
//}
//
//inline bool operator==(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.Equals(rhs);
//}
//
//inline bool operator!=(const String& lhs, StringView rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//inline bool operator!=(const String& lhs, const char16_t* rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//inline bool operator!=(StringView lhs, const String& rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//inline bool operator!=(const char16_t* lhs, const String& rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//inline bool operator!=(const String& lhs, const String& rhs) noexcept
//{
//    return !operator==(lhs, rhs);
//}
//
//inline bool operator<(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.GetHashCode() < rhs.GetHashCode();
//}
//
//inline bool operator<=(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.GetHashCode() <= rhs.GetHashCode();
//}
//
//inline bool operator>(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.GetHashCode() > rhs.GetHashCode();
//}
//
//inline bool operator>=(const String& lhs, const String& rhs) noexcept
//{
//    return lhs.GetHashCode() >= rhs.GetHashCode();
//}
//
//inline const char16_t* begin(const String& str) noexcept
//{
//    return &str[0];
//}
//
//inline const char16_t* end(const String& str) noexcept
//{
//    return begin(str) + str.Length();
//}
//
//CS2CPP_NAMESPACE_END
//
//// inline const char16_t* begin(const String& value)
//// {
////     value.IsLargeString();
////     return static_cast<T*>(span);
//// }
//
//// inline const T* end(const String& value)
//// {
////     return static_cast<T*>(span) + span.Length();
//// }
//
////basic_stringstream<char16_t>& operator<<(basic_stringstream<char16_t>& lhs, const auto& rhs)
////{
////}
