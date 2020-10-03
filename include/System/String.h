#pragma once

#include <fmt/format.h>
#include <gsl/span>
#include <string_view>
#include <unicode/unistr.h>
#include <unicode/utf16.h>
#include <unicode/utypes.h>
#include <vector>

CS2CPP_NAMESPACE_BEGIN

enum class NormalizationForm
{
    FormC,
    FormD,
    FormKC,
    FormKD,
};

enum class StringSplitOptions
{
    None,
    RemoveEmptyEntries,
};

class String final
{
    friend class Directory;

public:
    String() = default;
    String(std::u16string_view value);
    String(std::u32string_view value);
    String(const char16_t* value);
    String(const char16_t* value, int32_t length);
    String(const char32_t* value);
    String(const char32_t* value, int32_t length);
    String(char32_t c, int32_t count);
    String(const String& rhs) = default;
    String(String&& rhs) noexcept = default;
private:
    String(icu::UnicodeString value) noexcept;

public:
    ~String() = default;

public:
    String& operator=(const char16_t* rhs);
    String& operator=(std::u16string_view rhs);
    String& operator=(const String& rhs) = default;
    String& operator=(String&& rhs) noexcept;
    String& operator+=(char16_t rhs);
    String& operator+=(std::u16string_view rhs);
    [[nodiscard]] const char16_t& operator[](int32_t index) const noexcept;
    [[nodiscard]] operator std::u16string_view() const noexcept;

public:
    [[nodiscard]] static String Join(std::u16string_view separator, std::initializer_list<std::u16string_view> values);
    [[nodiscard]] static constexpr bool Equals(std::u16string_view strA, std::u16string_view strB) noexcept;
    [[nodiscard]] bool Equals(std::u16string_view value) const noexcept;
    [[nodiscard]] std::vector<char16_t> ToCharArray() const;
    [[nodiscard]] std::vector<char16_t> ToCharArray(int32_t startIndex, int32_t length) const;
    [[nodiscard]] static constexpr bool IsNullOrEmpty(std::u16string_view value) noexcept;
    [[nodiscard]] static constexpr bool IsNullOrWhiteSpace(std::u16string_view value) noexcept;
    [[nodiscard]] int32_t GetHashCode() const noexcept;
    [[nodiscard]] int32_t Length() const noexcept;
    [[nodiscard]] std::vector<String> Split(char16_t separator, StringSplitOptions options = StringSplitOptions::None) const;
    [[nodiscard]] std::vector<String> Split(char16_t separator, int32_t count, StringSplitOptions options = StringSplitOptions::None) const;
    [[nodiscard]] std::vector<String> Split(std::initializer_list<char16_t> separator, StringSplitOptions options = StringSplitOptions::None) const;
    [[nodiscard]] std::vector<String> Split(std::initializer_list<char16_t> separator, int32_t count, StringSplitOptions options = StringSplitOptions::None) const;
    [[nodiscard]] String Substring(int32_t startIndex) const;
    [[nodiscard]] String Substring(int32_t startIndex, int32_t length) const;
    [[nodiscard]] String Trim() const;
    [[nodiscard]] String Trim(char16_t trimChar) const;
    [[nodiscard]] String Trim(std::initializer_list<char16_t> trimChars) const;
    [[nodiscard]] String TrimStart() const;
    [[nodiscard]] String TrimStart(char16_t trimChar) const;
    [[nodiscard]] String TrimStart(std::initializer_list<char16_t> trimChars) const;
    [[nodiscard]] String TrimEnd() const;
    [[nodiscard]] String TrimEnd(char16_t trimChar) const;
    [[nodiscard]] String TrimEnd(std::initializer_list<char16_t> trimChars) const;
    [[nodiscard]] bool IsNormalized(NormalizationForm normalizationForm = NormalizationForm::FormC) const noexcept;
    [[nodiscard]] String Normalize(NormalizationForm normalizationForm = NormalizationForm::FormC) const;
    [[nodiscard]] static int32_t Compare(std::u16string_view strA, std::u16string_view strB) noexcept;
    [[nodiscard]] static int32_t Compare(std::u16string_view strA, std::u16string_view strB, bool ignoreCase) noexcept;
    [[nodiscard]] static constexpr int32_t CompareOrdinal(std::u16string_view strA, std::u16string_view strB) noexcept;
    [[nodiscard]] int32_t CompareTo(std::u16string_view value) const noexcept;
    [[nodiscard]] bool Contains(std::u16string_view value) const noexcept;
    [[nodiscard]] bool Contains(char16_t value) const noexcept;
    [[nodiscard]] bool StartsWith(std::u16string_view value) const noexcept;
    [[nodiscard]] bool StartsWith(std::u16string_view value, bool ignoreCase) const noexcept;
    [[nodiscard]] bool EndsWith(std::u16string_view value) const noexcept;
    [[nodiscard]] bool EndsWith(std::u16string_view value, bool ignoreCase) const noexcept;
    [[nodiscard]] int32_t IndexOf(char16_t c) const noexcept;
    [[nodiscard]] int32_t IndexOf(char16_t c, int32_t startIndex) const noexcept;
    [[nodiscard]] int32_t IndexOf(char16_t c, int32_t startIndex, int32_t count) const noexcept;
    [[nodiscard]] int32_t IndexOf(std::u16string_view value) const noexcept;
    [[nodiscard]] int32_t IndexOf(std::u16string_view value, int32_t startIndex) const noexcept;
    [[nodiscard]] int32_t IndexOf(std::u16string_view value, int32_t startIndex, int32_t count) const noexcept;
    [[nodiscard]] int32_t IndexOfAny(std::initializer_list<char16_t> anyOf) const noexcept;
    [[nodiscard]] int32_t IndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex) const noexcept;
    [[nodiscard]] int32_t IndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept;
    [[nodiscard]] int32_t LastIndexOf(char16_t value) const noexcept;
    [[nodiscard]] int32_t LastIndexOf(char16_t value, int32_t startIndex) const noexcept;
    [[nodiscard]] int32_t LastIndexOf(char16_t value, int32_t startIndex, int32_t count) const noexcept;
    [[nodiscard]] int32_t LastIndexOf(std::u16string_view value) const noexcept;
    [[nodiscard]] int32_t LastIndexOf(std::u16string_view value, int32_t startIndex) const noexcept;
    [[nodiscard]] int32_t LastIndexOf(std::u16string_view value, int32_t startIndex, int32_t count) const noexcept;
    [[nodiscard]] int32_t LastIndexOfAny(std::initializer_list<char16_t> anyOf) const noexcept;
    [[nodiscard]] int32_t LastIndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex) const noexcept;
    [[nodiscard]] int32_t LastIndexOfAny(std::initializer_list<char16_t> anyOf, int32_t startIndex, int32_t count) const noexcept;
    [[nodiscard]] String PadLeft(int32_t totalWidth) const;
    [[nodiscard]] String PadLeft(int32_t totalWidth, char16_t paddingChar) const;
    [[nodiscard]] String PadRight(int32_t totalWidth) const;
    [[nodiscard]] String PadRight(int32_t totalWidth, char16_t paddingChar) const;
    [[nodiscard]] String ToLower() const;
    [[nodiscard]] String ToLowerInvariant() const;
    [[nodiscard]] String ToUpper() const;
    [[nodiscard]] String ToUpperInvariant() const;
    [[nodiscard]] String ToString() const;
    [[nodiscard]] String Insert(int32_t startIndex, std::u16string_view value) const;
    [[nodiscard]] String Replace(char16_t oldChar, char16_t newChar) const;
    [[nodiscard]] String Replace(std::u16string_view oldValue, std::u16string_view newValue) const;
    [[nodiscard]] String Remove(int32_t startIndex, int32_t count) const;
    void CopyTo(int32_t sourceIndex, gsl::span<char16_t> destination, int32_t count) const;
    template <typename... _Types>
    [[nodiscard]] static String Format(std::u16string_view format, _Types&&... args);
    template <typename T, typename... Ts>
    [[nodiscard]] static String Concat(const T& arg, Ts&&... args);

private:
    [[nodiscard]] static constexpr int32_t GetTrimStartOffset(std::u16string_view value, std::initializer_list<char16_t> trimChars) noexcept;
    [[nodiscard]] static constexpr int32_t GetTrimEndOffset(std::u16string_view value, std::initializer_list<char16_t> trimChars) noexcept;
    template <typename... Ts>
    static void InternalConcat(icu::UnicodeString& str, char16_t value, Ts&&... args);
    template <typename... Ts>
    static void InternalConcat(icu::UnicodeString& str, std::u16string_view value, Ts&&... args);

public:
    static const String Empty;

private:
    static constexpr auto WhiteSpaceTable = {u' ', u'\u2000', u'\f', u'\n', u'\r', u'\t', u'\v'};
    mutable icu::UnicodeString _str;
};

constexpr bool String::Equals(std::u16string_view strA, std::u16string_view strB) noexcept
{
    return strA == strB;
}

constexpr bool String::IsNullOrEmpty(std::u16string_view value) noexcept
{
    return value.length() == 0;
}

constexpr bool String::IsNullOrWhiteSpace(std::u16string_view value) noexcept
{
    if (IsNullOrEmpty(value))
    {
        return true;
    }

    if (GetTrimStartOffset(value, WhiteSpaceTable) == static_cast<int32_t>(value.length()) ||
        GetTrimEndOffset(value, WhiteSpaceTable) == 0)
    {
        return true;
    }

    return false;
}

constexpr int32_t String::CompareOrdinal(std::u16string_view strA, std::u16string_view strB) noexcept
{
    const auto* a = strA.data();
    const auto* b = strB.data();
    if (a == b && strA.length() == strB.length())
    {
        return 0;
    }

    if (strA.length() == 0)
    {
        return -1;
    }

    if (strB.length() == 0)
    {
        return 1;
    }

    auto length = int32_t(std::min(strA.length(), strB.length()));
    int32_t diffOffset = -1;

    while (length >= 10)
    {
        if (*reinterpret_cast<const int32_t*>(a) != *reinterpret_cast<const int32_t*>(b))
        {
            diffOffset = 0;
            break;
        }

        if (*reinterpret_cast<const int32_t*>(a + 2) != *reinterpret_cast<const int32_t*>(b + 2))
        {
            diffOffset = 2;
            break;
        }

        if (*reinterpret_cast<const int32_t*>(a + 4) != *reinterpret_cast<const int32_t*>(b + 4))
        {
            diffOffset = 4;
            break;
        }

        if (*reinterpret_cast<const int32_t*>(a + 6) != *reinterpret_cast<const int32_t*>(b + 6))
        {
            diffOffset = 6;
            break;
        }

        if (*reinterpret_cast<const int32_t*>(a + 8) != *reinterpret_cast<const int32_t*>(b + 8))
        {
            diffOffset = 8;
            break;
        }
        a += 10;
        b += 10;
        length -= 10;
    }

    if (diffOffset != -1)
    {
        a += diffOffset;
        b += diffOffset;
        const auto order = *a - *b;
        if (order != 0)
        {
            return order;
        }
        return *(a + 1) - *(b + 1);
    }

    while (length > 0)
    {
        if (*reinterpret_cast<const int32_t*>(a) != *reinterpret_cast<const int32_t*>(b))
        {
            break;
        }
        a += 2;
        b += 2;
        length -= 2;
    }

    if (length > 0)
    {
        const auto c = *a - *b;
        if (c != 0)
        {
            return c;
        }
        return *(a + 1) - *(b + 1);
    }

    return int32_t(strA.length() - strB.length());
}

template <typename... Ts>
String String::Format(std::u16string_view format, Ts&&... args)
{
    fmt::basic_memory_buffer<char16_t> buffer;
    fmt::format_to(buffer, format, std::forward<Ts>(args)...);

    return icu::UnicodeString(buffer.data(), static_cast<int32_t>(buffer.size()));
}

template <typename T, typename... Ts>
String String::Concat(const T& arg, Ts&&... args)
{
    icu::UnicodeString ret;
    InternalConcat(ret, arg, std::forward<Ts>(args)...);

    return std::move(ret);
}

constexpr int32_t String::GetTrimStartOffset(std::u16string_view value, std::initializer_list<char16_t> trimChars) noexcept
{
    const auto* array = value.data();
    const auto length = static_cast<int32_t>(value.length());
    int32_t i = 0;
    int32_t startOffset = 0;
    char32_t c = 0;

    while (true)
    {
        startOffset = i;
        if (i >= length)
        {
            break;
        }
        U16_NEXT(array, i, length, c);

        if (std::find(trimChars.begin(), trimChars.end(), c) == trimChars.end())
        {
            break;
        }
    }

    return startOffset;
}

constexpr int32_t String::GetTrimEndOffset(std::u16string_view value, std::initializer_list<char16_t> trimChars) noexcept
{
    const auto* array = value.data();
    const auto oldLength = static_cast<int32_t>(value.length());
    char32_t c = 0;
    int32_t i = oldLength, length = 0;

    while (true)
    {
        length = i;
        if (i <= 0)
        {
            break;
        }
        U16_PREV(array, 0, i, c);

        if (std::find(trimChars.begin(), trimChars.end(), c) == trimChars.end())
        {
            break;
        }
    }

    return (length < oldLength) ? length : oldLength;
}

template <typename... _Types>
void String::InternalConcat(icu::UnicodeString& str, char16_t value, _Types&&... args)
{
    str.append(value);
    if constexpr (sizeof...(_Types) > 0)
    {
        InternalConcat(str, std::forward<_Types>(args)...);
    }
}

template <typename... _Types>
void String::InternalConcat(icu::UnicodeString& str, std::u16string_view value, _Types&&... args)
{
    str.append(value.data(), static_cast<int32_t>(value.length()));
    if constexpr (sizeof...(_Types) > 0)
    {
        InternalConcat(str, std::forward<_Types>(args)...);
    }
}

[[nodiscard]] inline String operator+(const String& lhs, std::u16string_view rhs)
{
    return String(lhs) += rhs;
}

[[nodiscard]] inline String operator+(const String& lhs, const char16_t* rhs)
{
    return operator+(lhs, std::u16string_view(rhs));
}

[[nodiscard]] inline String operator+(std::u16string_view lhs, const String& rhs)
{
    return String(lhs) += rhs;
}

[[nodiscard]] inline String operator+(const char16_t* lhs, const String& rhs)
{
    return operator+(std::u16string_view(lhs), rhs);
}

[[nodiscard]] inline String operator+(const String& lhs, const String& rhs)
{
    return String(lhs) += rhs;
}

[[nodiscard]] inline bool operator==(const String& lhs, std::u16string_view rhs) noexcept
{
    return lhs.Equals(rhs);
}

[[nodiscard]] inline bool operator==(const String& lhs, const char16_t* rhs) noexcept
{
    return lhs.Equals(rhs);
}

[[nodiscard]] inline bool operator==(std::u16string_view lhs, const String& rhs) noexcept
{
    return rhs.Equals(lhs);
}

[[nodiscard]] inline bool operator==(const char16_t* lhs, const String& rhs) noexcept
{
    return rhs.Equals(lhs);
}

[[nodiscard]] inline bool operator==(const String& lhs, const String& rhs) noexcept
{
    return lhs.Equals(rhs);
}

[[nodiscard]] inline bool operator!=(const String& lhs, std::u16string_view rhs) noexcept
{
    return !operator==(lhs, rhs);
}

[[nodiscard]] inline bool operator!=(const String& lhs, const char16_t* rhs) noexcept
{
    return !operator==(lhs, rhs);
}

[[nodiscard]] inline bool operator!=(std::u16string_view lhs, const String& rhs) noexcept
{
    return !operator==(lhs, rhs);
}

[[nodiscard]] inline bool operator!=(const char16_t* lhs, const String& rhs) noexcept
{
    return !operator==(lhs, rhs);
}

[[nodiscard]] inline bool operator!=(const String& lhs, const String& rhs) noexcept
{
    return !operator==(lhs, rhs);
}

[[nodiscard]] inline bool operator<(const String& lhs, const String& rhs) noexcept
{
    return lhs.GetHashCode() < rhs.GetHashCode();
}

[[nodiscard]] inline bool operator<=(const String& lhs, const String& rhs) noexcept
{
    return lhs.GetHashCode() <= rhs.GetHashCode();
}

[[nodiscard]] inline bool operator>(const String& lhs, const String& rhs) noexcept
{
    return lhs.GetHashCode() > rhs.GetHashCode();
}

[[nodiscard]] inline bool operator>=(const String& lhs, const String& rhs) noexcept
{
    return lhs.GetHashCode() >= rhs.GetHashCode();
}

CS2CPP_NAMESPACE_END

namespace std
{

[[nodiscard]] inline const char16_t* begin(const CS2CPP_NAMESPACE::String& str) noexcept
{
    return &str[0];
}

[[nodiscard]] inline const char16_t* end(const CS2CPP_NAMESPACE::String& str) noexcept
{
    return begin(str) + str.Length();
}

}