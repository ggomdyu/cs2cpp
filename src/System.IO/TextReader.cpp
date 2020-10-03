#include <array>

#include "System.IO/TextReader.h"

CS2CPP_NAMESPACE_BEGIN

int32_t TextReader::Read(Span<char16_t> bytes)
{
    int32_t n = 0;
    for (; n < bytes.Length(); ++n)
    {
        auto c = Read();
        if (c == -1)
        {
            break;
        }

        bytes[n] = c;
    }

    return n;
}

std::optional<std::u16string> TextReader::ReadLine()
{
    std::u16string ret;
    while (true)
    {
        auto c = Read();
        if (c == -1)
        {
            break;
        }

        if (c == u'\r' || c == u'\n')
        {
            if (c == u'\r' && Peek() == u'\n')
            {
                Read();
            }

            return {std::move(ret)};
        }

        ret.push_back(c);
    }

    if (ret.empty())
    {
        return std::nullopt;
    }

    return {std::move(ret)};
}

std::u16string TextReader::ReadToEnd()
{
    std::u16string ret;

    std::array<char16_t, 4096> buffer{};
    int32_t readLen;
    while ((readLen = Read(buffer)) != 0)
    {
        ret.insert(ret.cend(), buffer.data(), buffer.data() + readLen);
    }

    return ret;
}

CS2CPP_NAMESPACE_END
