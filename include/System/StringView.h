//#pragma once
//
//#include <vector>
//
//#include "System/ReadOnlySpan.h"
//#include "System/String.h"
//
//CS2CPP_NAMESPACE_BEGIN
//
//class StringView final
//{
//public:
//    constexpr StringView(const char16_t* value);
//    constexpr StringView(const char16_t* value, int32_t length);
//
//public:
//    constexpr bool operator==(const StringView& rhs) noexcept;
//    constexpr bool operator!=(const StringView& rhs) noexcept;
//    constexpr const char16_t& operator[](int32_t index) const noexcept;
//
//public:
//    static String Join(StringView separator, ReadOnlySpan<StringView> value);
//    static String Join(StringView separator, ReadOnlySpan<StringView> value, int32_t startIndex, int32_t count);
//    constexpr bool Equals(StringView value) const noexcept;
//    static constexpr bool Equals(StringView a, StringView b) noexcept;
//    void CopyTo(int32_t sourceIndex, Span<char16_t> dest, int32_t destinationIndex, int32_t count) const;
//    std::vector<char16_t> ToCharArray() const;
//    std::vector<char16_t> ToCharArray(int32_t startIndex, int32_t length) const;
//    constexpr int32_t GetHashCode() const noexcept;
//    constexpr int32_t Length() const noexcept;
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
//    constexpr bool IsNormalized() const noexcept;
//    constexpr bool IsNormalized(NormalizationForm normalizationForm) const noexcept;
//    String Normalize() const;
//    String Normalize(NormalizationForm normalizationForm) const;
//    static constexpr int32_t Compare(StringView strA, StringView strB) noexcept;
//    static constexpr int32_t Compare(StringView strA, StringView strB, bool ignoreCase) noexcept;
//    static constexpr int32_t Compare(StringView strA, StringView strB, int32_t length) noexcept;
//    static constexpr int32_t Compare(StringView strA, StringView strB, int32_t length, bool ignoreCase) noexcept;
//    constexpr int32_t CompareTo(StringView str) const noexcept;
//    static constexpr int32_t CompareOrdinal(StringView strA, StringView strB) noexcept;
//    static constexpr int32_t CompareOrdinal(StringView strA, StringView strB, bool ignoreCase) noexcept;
//    constexpr bool Contains(char16_t value) const noexcept;
//    constexpr bool Contains(StringView value) const noexcept;
//    constexpr bool EndsWith(StringView value) const noexcept;
//    constexpr bool EndsWith(StringView value, bool ignoreCase) const noexcept;
//    constexpr int32_t IndexOf(char16_t c) const noexcept;
//    constexpr int32_t IndexOf(char16_t c, int32_t startIndex) const noexcept;
//    constexpr int32_t IndexOf(char16_t c, int32_t startIndex, int32_t count) const noexcept;
//    constexpr int32_t IndexOfAny(ReadOnlySpan<char16_t> anyOf) const noexcept;
//    constexpr int32_t IndexOfAny(ReadOnlySpan<char16_t> anyOf, int32_t startIndex) const noexcept;
//    constexpr int32_t IndexOfAny(ReadOnlySpan<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept;
//    constexpr int32_t IndexOf(StringView value) const noexcept;
//    constexpr int32_t IndexOf(StringView value, int32_t startIndex) const noexcept;
//    constexpr int32_t IndexOf(StringView value, int32_t startIndex, int32_t count) const noexcept;
//    constexpr int32_t LastIndexOf(char16_t value) const noexcept;
//    constexpr int32_t LastIndexOf(char16_t value, int32_t startIndex) const noexcept;
//    constexpr int32_t LastIndexOf(char16_t value, int32_t startIndex, int32_t count) const noexcept;
//    constexpr int32_t LastIndexOfAny(ReadOnlySpan<char16_t> anyOf) const noexcept;
//    constexpr int32_t LastIndexOfAny(ReadOnlySpan<char16_t> anyOf, int32_t startIndex) const noexcept;
//    constexpr int32_t LastIndexOfAny(ReadOnlySpan<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept;
//    constexpr int32_t LastIndexOf(StringView value) const noexcept;
//    constexpr int32_t LastIndexOf(StringView value, int32_t startIndex) const noexcept;
//    constexpr int32_t LastIndexOf(StringView value, int32_t startIndex, int32_t count) const noexcept;
//    String PadLeft(int32_t totalWidth) const;
//    String PadLeft(int32_t totalWidth, char16_t paddingChar) const;
//    String PadRight(int32_t totalWidth) const;
//    String PadRight(int32_t totalWidth, char16_t paddingChar) const;
//    constexpr bool StartsWith(StringView value) const noexcept;
//    constexpr bool StartsWith(StringView value, bool ignoreCase) const noexcept;
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
//
//private:
//    static constexpr char16_t WhiteSpaceTable[] = {u' ', u'\u2000', u'\f', u'\n', u'\r', u'\t', u'\v'};
//
//    const char16_t* str_;
//    int32_t length_;
//};
//
//constexpr int32_t StringView::GetHashCode() const noexcept
//{
//    int32_t hashValue = 0;
//    for (int32_t i = 0; i < length_; ++i)
//    {
//        hashValue = 65599 * hashValue + str_[i];
//    }
//
//    return hashValue ^ (hashValue >> 16);
//}
//
//constexpr int32_t StringView::Length() const noexcept
//{
//    return length_;
//}
//
//CS2CPP_NAMESPACE_END