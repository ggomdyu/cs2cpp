#pragma once

#include <mutex>
#include <optional>
#include <string_view>

#include "System.Text/CodePageDataItem.h"
#include "System.Text/EncodingInfo.h"

CS2CPP_NAMESPACE_BEGIN

class EncodingTable final
{
public:
    EncodingTable() = delete;

public:
    static std::vector<EncodingInfo> GetEncodings();
    static std::optional<int32_t> GetCCSIDFromCodePage(int32_t codePage) noexcept;
    static std::optional<int32_t> GetCodePageFromName(std::u16string_view name);
    static const CodePageDataItem& GetCodePageDataItem(int32_t codePage);

private:
    static std::optional<int32_t> InternalGetCodePageFromName(std::u16string_view item) noexcept;
};

CS2CPP_NAMESPACE_END