#pragma once

CS2CPP_NAMESPACE_BEGIN

constexpr ReadOnlySpan<char16_t> Path::GetInvalidFileNameChars() noexcept
{
    constexpr char16_t invalidFileNameChars[] = {
        u'\"', u'<', u'>', u'|', u'\0', static_cast<char16_t>(1), static_cast<char16_t>(2), static_cast<char16_t>(3),
        static_cast<char16_t>(4), static_cast<char16_t>(5), static_cast<char16_t>(6), static_cast<char16_t>(7), static_cast<char16_t>(8),
        static_cast<char16_t>(9), static_cast<char16_t>(10), static_cast<char16_t>(11), static_cast<char16_t>(12),
        static_cast<char16_t>(13), static_cast<char16_t>(14), static_cast<char16_t>(15), static_cast<char16_t>(16),
        static_cast<char16_t>(17), static_cast<char16_t>(18), static_cast<char16_t>(19), static_cast<char16_t>(20),
        static_cast<char16_t>(21), static_cast<char16_t>(22), static_cast<char16_t>(23), static_cast<char16_t>(24),
        static_cast<char16_t>(25), static_cast<char16_t>(26), static_cast<char16_t>(27), static_cast<char16_t>(28),
        static_cast<char16_t>(29), static_cast<char16_t>(30), static_cast<char16_t>(31), ':', '*', '?', '\\', '/'};

    return invalidFileNameChars;
}

constexpr ReadOnlySpan<char16_t> Path::GetInvalidPathChars() noexcept
{
    constexpr char16_t invalidPathChars[] = {
        u'|', u'\0', static_cast<char16_t>(1), static_cast<char16_t>(2), static_cast<char16_t>(3), static_cast<char16_t>(4),
        static_cast<char16_t>(5), static_cast<char16_t>(6), static_cast<char16_t>(7), static_cast<char16_t>(8), static_cast<char16_t>(9),
        static_cast<char16_t>(10), static_cast<char16_t>(11), static_cast<char16_t>(12), static_cast<char16_t>(13),
        static_cast<char16_t>(14), static_cast<char16_t>(15), static_cast<char16_t>(16), static_cast<char16_t>(17),
        static_cast<char16_t>(18), static_cast<char16_t>(19), static_cast<char16_t>(20), static_cast<char16_t>(21),
        static_cast<char16_t>(22), static_cast<char16_t>(23), static_cast<char16_t>(24), static_cast<char16_t>(25),
        static_cast<char16_t>(26), static_cast<char16_t>(27), static_cast<char16_t>(28), static_cast<char16_t>(29),
        static_cast<char16_t>(30), static_cast<char16_t>(31)};

    return invalidPathChars;
}

constexpr int32_t Path::GetRootLength(std::u16string_view path) noexcept
{
    if (path.length() <= 1)
    {
        return path.length() == 1 && IsDirectorySeparator(path[0]) ? 1 : 0;
    }

    size_t len = 2;
    if (IsDirectorySeparator(path[0]))
    {
        // Check for \\server or \\server\share
        if (IsDirectorySeparator(path[1]))
        {
            // Get \\server
            while (len < path.length() && !IsDirectorySeparator(path[len]))
            {
                ++len;
            }

            // Get \share
            if (len < path.length())
            {
                ++len;
                while (len < path.length() && !IsDirectorySeparator(path[len]))
                {
                    ++len;
                }
            }
        }
        else
        {
            // The path starts with '\' or '/'.
            return 1;
        }
    }
    else if (path[1] == VolumeSeparatorChar)
    {
        // The path starts with "C:/" or "C:\\".
        if (path.length() >= 3 && IsDirectorySeparator(path[2]))
        {
            ++len;
        }
    }
    else
    {
        return 0;
    }

    return static_cast<int32_t>(len);
}

CS2CPP_NAMESPACE_END