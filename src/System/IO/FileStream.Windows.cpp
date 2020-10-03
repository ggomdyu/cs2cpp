#include "System/IO/FileStream.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

void* FileStream::NullNativeFileHandle = INVALID_HANDLE_VALUE;

int64_t FileStream::Length() const
{
    LARGE_INTEGER li;
    if (GetFileSizeEx(_nativeFileHandle, &li) == FALSE)
    {
        return -1;
    }

    int64_t length = li.QuadPart;
    if (_filePos + _writePos > length)
    {
        length = _filePos + _writePos;
    }

    return length;
}

void FileStream::FlushWriteBuffer()
{
    if (_writePos <= 0)
    {
        return;
    }

    this->InternalWrite(&_buffer[0], _writePos);

    _writePos = 0;
}

void* FileStream::InternalOpenHandle(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, FileOptions options)
{
    bool useSecurityAttributes = static_cast<int32_t>(share) & static_cast<int32_t>(FileShare::Inheritable);
    SECURITY_ATTRIBUTES securityAttributes{};
    if (useSecurityAttributes)
    {
        if ((static_cast<int32_t>(share) & static_cast<int32_t>(FileShare::Inheritable)) != 0)
        {
            securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
            securityAttributes.bInheritHandle = TRUE;
        }
    }

    auto desiredAccess =
        (access == FileAccess::Read) ? GENERIC_READ :
        (access == FileAccess::Write) ? GENERIC_WRITE : GENERIC_READ | GENERIC_WRITE;

    // Append is not a valid Win32 constant, so we must change it.
    if (mode == FileMode::Append)
    {
        mode = FileMode::OpenOrCreate;
    }

    // The named pipe on Windows allows the server to impersonate the client.
    // So we have to add the below flags because of this security vulnerability.
    DWORD flagsAndAttributes = static_cast<int32_t>(options) | SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS;

    return CreateFileW(reinterpret_cast<LPCWSTR>(path.data()), desiredAccess, static_cast<DWORD>(share),
        useSecurityAttributes ? &securityAttributes : nullptr, static_cast<DWORD>(mode), flagsAndAttributes, nullptr);
}

int32_t FileStream::InternalRead(std::byte* buffer, int32_t count)
{
    DWORD readBytes = 0;
    if (ReadFile(_nativeFileHandle, buffer, count, &readBytes, nullptr) == FALSE)
    {
        return 0;
    }

    _filePos += readBytes;
    return readBytes;
}

int32_t FileStream::InternalWrite(const std::byte* buffer, int32_t count)
{
    DWORD writtenBytes = 0;
    if (WriteFile(_nativeFileHandle, buffer, count, &writtenBytes, nullptr) == FALSE)
    {
        return 0;
    }

    _filePos += writtenBytes;
    return writtenBytes;
}

int64_t FileStream::InternalSeek(int64_t offset, SeekOrigin origin)
{
    LARGE_INTEGER distanceToMove;
    distanceToMove.QuadPart = offset;

    LARGE_INTEGER newFilePointer;
    if (SetFilePointerEx(_nativeFileHandle, distanceToMove, &newFilePointer, static_cast<DWORD>(origin)) == FALSE)
    {
        return 0;
    }

    // Validate the file pointer position.
    _filePos = newFilePointer.QuadPart;

    return newFilePointer.QuadPart;
}

void FileStream::InternalClose()
{
    if (_nativeFileHandle != NullNativeFileHandle)
    {
        CloseHandle(_nativeFileHandle);
        _nativeFileHandle = NullNativeFileHandle;
    }
}

bool FileStream::InternalSetLength(int64_t value)
{
    const auto prevFilePos = _filePos;
    if (_filePos != value)
    {
        if (this->InternalSeek(value, SeekOrigin::Begin) == 0)
        {
            return false;
        }
    }

    if (SetEndOfFile(_nativeFileHandle) == FALSE)
    {
        return false;
    }

    if (prevFilePos != value)
    {
        if (value <= prevFilePos)
        {
            // The file pointer position is now shorter than the current file length,
            // so we must set it to EOF.
            if (this->InternalSeek(0, SeekOrigin::End) == 0)
            {
                return false;
            }
        }
        else
        {
            // The file pointer position has changed, so we must roll back it.
            if (this->InternalSeek(prevFilePos, SeekOrigin::Begin))
            {
                return false;
            }
        }
    }

    return true;
}

void FileStream::InternalFlush() const
{
    FlushFileBuffers(_nativeFileHandle);
}

CS2CPP_NAMESPACE_END