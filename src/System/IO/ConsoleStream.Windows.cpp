#include "System/IO/ConsoleStream.Windows.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

WindowsConsoleStream::WindowsConsoleStream(void* handle, FileAccess access, bool useFileAPIs) :
    ConsoleStream(access),
    _handle(handle),
    _isPipe(GetFileType(handle) == FILE_TYPE_PIPE),
    _useFileAPIs(useFileAPIs)
{
}

int32_t WindowsConsoleStream::Read(std::byte* bytes, int32_t count)
{
    if (bytes == nullptr || count == 0 || !this->CanRead())
    {
        return 0;
    }

    DWORD readNum;
    if ((_useFileAPIs ?
        ReadFile(_handle, bytes, count, &readNum, nullptr) :
        ReadConsole(_handle, bytes, count / sizeof(wchar_t), &readNum, nullptr)) == FALSE)
    {
        return 0;
    }

    if (!_useFileAPIs)
    {
        readNum *= sizeof(wchar_t);
    }

    return static_cast<int32_t>(readNum);
}

int32_t WindowsConsoleStream::ReadByte()
{
    std::array<std::byte, 1> byte{};
    if (this->Read(byte.data(), 1) == 0)
    {
        return -1;
    }

    return static_cast<int32_t>(byte[0]);
}

bool WindowsConsoleStream::Write(const std::byte* bytes, int32_t count)
{
    if (bytes == nullptr || count == 0 || !this->CanWrite())
    {
        return false;
    }

    DWORD writtenNum;
    return !!(_useFileAPIs ?
        WriteFile(_handle, bytes, count, &writtenNum, nullptr) :
        WriteConsoleW(_handle, bytes, count / sizeof(wchar_t), &writtenNum, nullptr)
    );
}

bool WindowsConsoleStream::WriteByte(std::byte value)
{
    std::array<std::byte, 1> byte{value};
    return Write(byte.data(), static_cast<int32_t>(byte.size()));
}

CS2CPP_NAMESPACE_END