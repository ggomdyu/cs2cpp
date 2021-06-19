#include "System/Environment.h.h"
#include "System/IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

std::string PosixPath::Combine(std::string_view path1, std::string_view path2)
{
    if (path2.length() == 0)
        return std::string(path1);

    if (path1.length() == 0 || IsPathRooted(path2))
        return std::string(path2);

    auto path1Separator = path1[path1.length() - 1];
    auto path2Separator = path2[0];
    auto hasSeparator = (path1Separator == PosixPath::DirectorySeparatorChar)
        || (path1Separator == PosixPath::AltDirectorySeparatorChar)
        || (path2Separator == PosixPath::DirectorySeparatorChar)
        || (path2Separator == PosixPath::AltDirectorySeparatorChar);

    std::u16string ret;
    ret.reserve(path1.length() + path2.length() + (hasSeparator ? 0 : 1));
    ret += path1;

    if (!hasSeparator)
        ret += PosixPath::DirectorySeparatorChar;

    ret += path2;

    return ret;
}

std::u16string Path::GetDirectoryName(std::u16string_view path)
{
    auto rootLen = Path::GetRootLength(path);
    auto index = path.length();
    while (rootLen < index && !IsDirectorySeparator(path[--index]));

    return std::u16string(path.substr(0, index));
}

std::u16string Path::GetPathRoot(std::u16string_view path)
{
    return IsDirectorySeparator(path[0]) ? DirectorySeparatorStr : std::u16string_view();
}

std::u16string Path::GetTempPath()
{
    auto tempPath = Environment::GetEnvironmentVariable(u"TMPDIR");
    if (!tempPath)
        return u"/tmp/";

    if (tempPath->length() > 0 && !IsDirectorySeparator((*tempPath)[tempPath->length() - 1]))
        *tempPath += DirectorySeparatorChar;

    return std::move(*tempPath);
}

CS2CPP_NAMESPACE_END
