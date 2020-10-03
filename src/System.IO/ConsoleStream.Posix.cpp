#include "System.IO/ConsoleStream.h"

CS2CPP_NAMESPACE_BEGIN

ConsoleStream::ConsoleStream(void* handle, FileAccess access, bool useFileAPIs) noexcept :
    handle_(handle),
    canRead_(access == FileAccess::Read || access == FileAccess::ReadWrite),
    canWrite_(access == FileAccess::Write || access == FileAccess::ReadWrite),
    isPipe_(false),
    useFileAPIs_(useFileAPIs)
{
}

int32_t ConsoleStream::Read(Span<std::byte> bytes)
{
    if (bytes.IsEmpty() || !CanRead())
    {
        return 0;
    }

    auto buffer = reinterpret_cast<char*>(&bytes[0]);
    if (!fgets(buffer, bytes.Length(), static_cast<FILE*>(handle_)))
    {
        return 0;
    }

    return static_cast<int32_t>(strlen(buffer));
}

bool ConsoleStream::Write(ReadOnlySpan<std::byte> bytes)
{
    if (bytes.IsEmpty() || !CanWrite())
    {
        return false;
    }

    return fwrite(bytes, 1, bytes.Length(), static_cast<FILE*>(handle_)) == bytes.Length();
}

CS2CPP_NAMESPACE_END