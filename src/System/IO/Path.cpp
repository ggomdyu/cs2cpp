#include <array>

#include "System/Environment.h"
#include "System/IO/Directory.h"
#include "System/Random.h"

CS2CPP_NAMESPACE_BEGIN

std::u16string Path::Combine(std::u16string_view path1, std::u16string_view path2)
{
    if (path2.length() == 0)
        return std::u16string(path1);

    if (path1.length() == 0 || IsPathRooted(path2))
        return std::u16string(path2);

    auto path1Separator = path1[path1.length() - 1];
    auto path2Separator = path2[0];
    auto hasSeparator = (path1Separator == DirectorySeparatorChar)
        || (path1Separator == AltDirectorySeparatorChar)
        || (path2Separator == DirectorySeparatorChar)
        || (path2Separator == AltDirectorySeparatorChar);

    std::u16string ret;
    ret.reserve(path1.length() + path2.length() + (hasSeparator ? 0 : 1));
    ret += path1;
    
    if (!hasSeparator)
        ret += DirectorySeparatorChar;

    ret += path2;

    return ret;
}

std::u16string Path::ChangeExtension(std::u16string_view path, std::u16string_view extension)
{
    if (path.length() == 0)
        return {};

    auto extensionStartIndex = path.rfind('.');
    if (extensionStartIndex == decltype(path)::npos)
        extensionStartIndex = path.length();

    std::u16string ret(path.substr(0, extensionStartIndex));
    auto extensionHasDot = extension.length() > 0 && extension[0] == u'.';
    if (!extensionHasDot)
        ret += u'.';

    ret += extension;

    return ret;
}

std::u16string Path::GetFullPath(std::u16string_view path)
{
    if (path.length() <= 0)
        return {};

    auto collapsedString = IsPathRooted(path) ?
        RemoveRelativeSegments(path) :
        RemoveRelativeSegments(Combine(Directory::GetCurrentDirectory(), path));

    return collapsedString.length() == 0 ? u"/" : collapsedString;
}

std::u16string Path::GetFullPath(std::u16string_view path, std::u16string_view basePath)
{
    return GetFullPath(Combine(basePath, path));
}

std::u16string Path::GetRandomFileName()
{
    auto r = Random();

    std::array<char16_t, 12> buffer{};
    for (auto& c : buffer)
    {
        c = static_cast<char16_t>(r.Next(0, 2) == 0 ?
            r.Next(u'a', u'z' + 1) :
            r.Next(u'0', u'9' + 1));
    }

    buffer[8] = u'.';

    return {buffer.data(), buffer.size()};
}

std::u16string Path::RemoveRelativeSegments(std::u16string_view path)
{
    std::u16string ret;
    for (size_t i = 0; i < path.length(); ++i)
    {
        auto c = path[i];
        if (IsDirectorySeparator(c) && i + 1 < path.length())
        {
            // Ignore the '/'
            if (IsDirectorySeparator(path[i + 1]))
                continue;

            // Ignore the "./"
            if (i + 2 >= path.length() || (IsDirectorySeparator(path[i + 2]) && path[i + 1] == u'.'))
            {
                ++i;
                continue;
            }

            if (i + 3 < path.length() && IsDirectorySeparator(path[i + 3]) && path[i + 2] == u'.' && path[i + 1] == u'.')
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
                    ret.clear();

                i += 2;
                continue;
            }
        }

        // Normalize the directory separator.
        if (c == AltDirectorySeparatorChar)
            c = DirectorySeparatorChar;

        ret.push_back(c);
    }

    return ret;
}

CS2CPP_NAMESPACE_END
