#include <array>

#include "System.IO/ConsoleStream.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

ConsoleStream::ConsoleStream(void* handle, FileAccess access, bool useFileAPIs) noexcept :
    handle_(handle),
    canRead_(access == FileAccess::Read || access == FileAccess::ReadWrite),
    canWrite_(access == FileAccess::Write || access == FileAccess::ReadWrite),
    isPipe_(GetFileType(handle) == FILE_TYPE_PIPE),
    useFileAPIs_(useFileAPIs)
{
}

int32_t ConsoleStream::Read(Span<std::byte> bytes)
{
    if (bytes.IsEmpty() || !CanRead())
    {
        return 0;
    }

    DWORD readBytes;
    if ((useFileAPIs_
        ? ReadFile(handle_, &bytes[0], bytes.Length(), &readBytes, nullptr)
        : ReadConsoleW(handle_, &bytes[0], bytes.Length() / sizeof(wchar_t), &readBytes, nullptr)) == FALSE)
    {
        return 0;
    }

    if (!useFileAPIs_)
    {
        readBytes *= sizeof(wchar_t);
    }

    return static_cast<int32_t>(readBytes);
}

bool ConsoleStream::Write(ReadOnlySpan<std::byte> bytes)
{
    if (bytes.IsEmpty() || !CanWrite())
    {
        return false;
    }

    DWORD writtenNum;
    return !!(useFileAPIs_
        ? WriteFile(handle_, &bytes[0], bytes.Length(), &writtenNum, nullptr)
        : WriteConsoleW(handle_, &bytes[0], bytes.Length() / sizeof(wchar_t), &writtenNum, nullptr)
    );
}

CS2CPP_NAMESPACE_END