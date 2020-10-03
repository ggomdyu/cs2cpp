#include <algorithm>

#include "System/Version.h"

CS2CPP_NAMESPACE_BEGIN

std::u16string Version::ToString() const
{
    if (build_ == -1)
    {
        return ToString(2);
    }

    if (revision_ == -1)
    {
        return ToString(3);
    }

    return ToString(4);
}

std::u16string Version::ToString(int32_t fieldCount) const
{
    constexpr size_t MaxBufferSize = 256;

    char c8Buffer[MaxBufferSize];
    size_t c8StrLen;

    switch (fieldCount)
    {
    case 0:
        return {};

    case 1:
        c8StrLen = CS2CPP_SPRINTF(c8Buffer, "%d", major_);
        break;

    case 2:
        c8StrLen = CS2CPP_SPRINTF(c8Buffer, "%d.%d", major_, minor_);
        break;

    default:
        if (build_ == -1)
        {
            return {};
        }

        if (fieldCount == 3)
        {
            c8StrLen = CS2CPP_SPRINTF(c8Buffer, "%d.%d.%d", major_, minor_, build_);
            break;
        }

        if (revision_ == -1)
        {
            return {};
        }

        if (fieldCount == 4)
        {
            c8StrLen = CS2CPP_SPRINTF(c8Buffer, "%d.%d.%d.%d", major_, minor_, build_, revision_);
            break;
        }

        return {};
    }

    char16_t c16Buffer[MaxBufferSize];
    std::copy(c8Buffer, c8Buffer + c8StrLen, c16Buffer);

    return std::u16string(c16Buffer, c8StrLen);
}

CS2CPP_NAMESPACE_END
