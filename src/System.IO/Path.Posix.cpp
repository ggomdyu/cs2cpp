#include "System/Environment.h"
#include "System.IO/Directory.h"
#include "System.IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

std::u16string Path::GetFullPath(std::u16string_view path)
{
    if (path.empty())
    {
        return {};
    }

    auto collapsedString = IsPathRooted(path)
        ? RemoveRelativeSegments(path)
        : RemoveRelativeSegments(Combine(Directory::GetCurrentDirectory(), path));

    return collapsedString.length() == 0 ? DirectorySeparatorStr : collapsedString;
}

std::u16string Path::GetPathRoot(std::u16string_view path)
{
    return IsDirectorySeparator(path[0]) ? DirectorySeparatorStr : std::u16string();
}

std::u16string Path::GetTempPath()
{
    auto tempPath = Environment::GetEnvironmentVariable(u"TMPDIR");
    if (!tempPath)
    {
        return u"/tmp/";
    }

    if (!tempPath->empty() && !IsDirectorySeparator((*tempPath)[tempPath->length() - 1]))
    {
        *tempPath += DirectorySeparatorChar;
    }

    return std::move(*tempPath);
}

CS2CPP_NAMESPACE_END
