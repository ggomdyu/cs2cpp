#pragma once

#include <string>

#include "System/ReadOnlySpan.h"

CS2CPP_NAMESPACE_BEGIN

class CodePageDataItem
{
public:
    CodePageDataItem() = default;
    CodePageDataItem(int32_t uiFamilyCodePage, std::u16string_view webName, std::u16string_view headerName, std::u16string_view bodyName, std::u16string_view displayName, ReadOnlySpan<std::byte> preamble, uint32_t flags) noexcept;

public:
    int32_t UIFamilyCodePage() const noexcept;
    std::u16string_view WebName() const noexcept;
    std::u16string_view HeaderName() const noexcept;
    std::u16string_view BodyName() const noexcept;
    std::u16string_view DisplayName() const noexcept;
    ReadOnlySpan<std::byte> Preamble() const noexcept;
    uint32_t Flags() const noexcept;

public:
    int32_t uiFamilyCodePage_ = 0;
    std::u16string_view webName_;
    std::u16string_view headerName_;
    std::u16string_view bodyName_;
    std::u16string_view displayName_;
    ReadOnlySpan<std::byte> preamble_;
    uint32_t flags_ = 0;
};

CS2CPP_NAMESPACE_END