#pragma once

#include <string_view>

CS2CPP_NAMESPACE_BEGIN

class EncodingInfo final
{
public:
    EncodingInfo(int32_t codePage, std::u16string_view name, std::u16string_view displayName) noexcept;

public:
    bool operator==(const EncodingInfo& rhs) const noexcept;
    bool operator!=(const EncodingInfo& rhs) const noexcept;

public:
    const class Encoding* GetEncoding() const noexcept;
    bool Equals(const EncodingInfo& value) const noexcept;
    int32_t GetHashCode() const noexcept;

public:
    const int32_t CodePage;
    const std::u16string Name;
    const std::u16string DisplayName;
};

CS2CPP_NAMESPACE_END