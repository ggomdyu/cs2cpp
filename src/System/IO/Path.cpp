#include <array>

#include "System/Environment.h"
#include "System/IO/Directory.h"
#include "System/IO/Path.h"
#include "System/Random.h"

CS2CPP_NAMESPACE_BEGIN

String Path::Combine(std::u16string_view path1, std::u16string_view path2)
{
    if (path2.length() == 0)
    {
        return String(path1);
    }

    if (path1.length() == 0 || IsPathRooted(path2))
    {
        return String(path2);
    }

    String ret(path1);
    const auto path1Separator = path1[path1.length() - 1];
    const auto path2Separator = path2[0];
    const auto hasSeparator = (path1Separator == DirectorySeparatorChar) || (path1Separator == AltDirectorySeparatorChar)
        || (path2Separator == DirectorySeparatorChar) || (path2Separator == AltDirectorySeparatorChar);
    if (!hasSeparator)
    {
        ret += DirectorySeparatorChar;
    }
    ret += path2;

    return ret;
}

String Path::ChangeExtension(std::u16string_view path, std::u16string_view extension)
{
    if (path.length() == 0)
    {
        return String::Empty;
    }

    auto extensionStartIndex = path.rfind('.');
    if (extensionStartIndex == decltype(path)::npos)
    {
        extensionStartIndex = path.length();
    }

    String ret(path.substr(0, extensionStartIndex));
    const auto extensionHasDot = extension.length() > 0 && extension[0] == u'.';
    if (!extensionHasDot)
    {
        ret += u'.';
    }
    ret += extension;

    return ret;
}

String Path::GetFullPath(std::u16string_view path)
{
    if (path.length() <= 0)
    {
        return String::Empty;
    }

    const auto collapsedString = IsPathRooted(path) ?
        RemoveRelativeSegments(path) :
        RemoveRelativeSegments(Combine(Directory::GetCurrentDirectory(), path));

    return collapsedString.Length() == 0 ? u"/" : collapsedString;
}

String Path::GetFullPath(std::u16string_view UPath, std::u16string_view basePath)
{
    return GetFullPath(Combine(basePath, UPath));
}

String Path::GetRandomFileName()
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

    return String(buffer.data(), static_cast<int32_t>(buffer.size()));
}

String Path::GetTempPath()
{
    auto tempEnvVarValue = Environment::GetEnvironmentVariable(u"TMPDIR");
    if (!tempEnvVarValue)
    {
        return u"/tmp/";
    }

    if (!IsDirectorySeparator((*tempEnvVarValue)[tempEnvVarValue->Length() - 1]))
    {
        (*tempEnvVarValue) += DirectorySeparatorChar;
    }

    return std::move(*tempEnvVarValue);
}

String Path::RemoveRelativeSegments(std::u16string_view path)
{
    String ret;
    for (decltype(path.length()) i = 0; i < path.length(); ++i)
    {
        auto c = path[i];
        if (IsDirectorySeparator(c) && i + 1 < path.length())
        {
            // Ignore the '/'
            if (IsDirectorySeparator(path[i + 1]))
            {
                continue;
            }

            // Ignore the "./"
            if (i + 2 >= path.length() || (IsDirectorySeparator(path[i + 2]) && path[i + 1] == u'.'))
            {
                ++i;
                continue;
            }

            if (i + 3 < path.length() && IsDirectorySeparator(path[i + 3]) && path[i + 2] == u'.' && path[i + 1] == u'.')
            {
                auto j = ret.Length() == 0 ? 0 : ret.Length() - 1;
                for (; j >= 0; --j)
                {
                    if (IsDirectorySeparator(ret[j]))
                    {
                        ret = ret.Substring(0, j);
                        break;
                    }
                }

                if (j == 0)
                {
                    ret = String::Empty;
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

        ret += c;
    }

    return ret;
}

CS2CPP_NAMESPACE_END
