#include <algorithm>
#include <array>

#include "System/Environment.h"
#include "System/Random.h"
#include "System.IO/Directory.h"
#include "System.IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

std::u16string Path::ChangeExtension(std::u16string_view path, std::u16string_view extension)
{
    if (path.empty())
    {
        return {};
    }

    size_t dotIndex = path.rfind('.');
    if (dotIndex == decltype(path)::npos)
    {
        dotIndex = path.length();
    }

    std::u16string ret(path.substr(0, dotIndex));
    bool foundDot = !extension.empty() && extension[0] == u'.';
    if (!foundDot)
    {
        ret += u'.';
    }

    ret += extension;

    return ret;
}

std::u16string Path::GetFullPath(std::u16string_view path, std::u16string_view basePath)
{
    return GetFullPath(Combine(basePath, path));
}

std::u16string Path::GetDirectoryName(std::u16string_view path)
{
    size_t rootLen = Path::GetRootLength(path);
    size_t index = path.length();
    while (rootLen < index && !IsDirectorySeparator(path[--index]));

    std::u16string ret(path.substr(0, index));
#if CS2CPP_PLATFORM_WINDOWS
    std::replace(ret.begin(), ret.end(), AltDirectorySeparatorChar, DirectorySeparatorChar);
#endif
    return ret;
}

std::u16string Path::GetRandomFileName()
{
    Random r;

    std::array<char16_t, 12> buffer = {};
    for (char16_t& c : buffer)
    {
        c = static_cast<char16_t>(r.Next(0, 2) == 0
            ? r.Next(u'a', u'z' + 1)
            : r.Next(u'0', u'9' + 1));
    }

    buffer[8] = u'.';

    return {buffer.data(), buffer.size()};
}

void Path::InternalCombine(std::u16string_view path, std::u16string& dest)
{
    if (path.empty())
    {
        return;
    }

    if (IsPathRooted(path))
    {
        dest = path;
        return;
    }

    if (!dest.empty())
    {
        bool hasSeparator = IsDirectorySeparator(dest.back()) || IsVolumeSeparator(dest.back());
        if (!hasSeparator)
        {
            dest += DirectorySeparatorChar;
        }
    }

    dest += path;
}

std::u16string Path::RemoveRelativeSegments(std::u16string_view path)
{
    std::u16string ret;
    for (size_t i = 0; i < path.length(); ++i)
    {
        char16_t c = path[i];
        if (IsDirectorySeparator(c) && i + 1 < path.length())
        {
            if (IsDirectorySeparator(path[i + 1]))
            {
                continue;
            }

            if (i + 2 >= path.length() || (path[i + 1] == u'.' && IsDirectorySeparator(path[i + 2])))
            {
                ++i;
                continue;
            }

            if (i + 3 < path.length() && path[i + 1] == u'.' && path[i + 2] == u'.' && IsDirectorySeparator(path[i + 3]))
            {
                int32_t j = static_cast<int32_t>(ret.size()) - 1;
                for (; j >= 0; --j)
                {
                    if (IsDirectorySeparator(ret[j]))
                    {
                        ret.resize(j);
                        break;
                    }
                }

                if (j == 0)
                {
                    ret.clear();
                }

                i += 2;
                continue;
            }
        }

        // Normalize the directory separator.
        if (c == AltDirectorySeparatorChar)
        {
            c = DirectorySeparatorChar;
        }

        ret.push_back(c);
    }

    return ret;
}

CS2CPP_NAMESPACE_END
