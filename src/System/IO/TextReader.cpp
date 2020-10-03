#include "System/IO/TextReader.h"

CS2CPP_NAMESPACE_BEGIN

int32_t TextReader::Read(gsl::span<char16_t> bytes)
{
    return Read(bytes.data(), static_cast<int32_t>(bytes.size()));
}

int32_t TextReader::Read(char16_t* bytes, int32_t count)
{
    int32_t n = 0;
    for (; n < count; ++n)
    {
        const auto c = this->Read();
        if (c == -1)
        {
            break;
        }

        bytes[n] = c;
    }

    return n;
}

std::optional<String> TextReader::ReadLine()
{
    String ret;
    while (true)
    {
        const auto c = this->Read();
        if (c == -1)
        {
            break;
        }

        if (c == u'\r' || c == u'\n')
        {
            if (c == u'\r' && this->Peek() == u'\n')
            {
                this->Read();
            }

            return std::move(ret);
        }

        ret += static_cast<char16_t>(c);
    }

    if (ret.Length() > 0)
    {
        return std::move(ret);
    }

    return {};
}

String TextReader::ReadToEnd()
{
    String ret;

    std::array<char16_t, 4096> buffer{};
    int32_t readLen;
    while ((readLen = this->Read(buffer)) != 0)
    {
        ret += std::u16string_view(buffer.data(), static_cast<size_t>(readLen));
    }

    return ret;
}

CS2CPP_NAMESPACE_END