#include "System.IO/FileStream.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

void* const FileStream::NullFileHandle = INVALID_HANDLE_VALUE;

int64_t FileStream::Length() const
{
    LARGE_INTEGER li;
    if (GetFileSizeEx(fileHandle_, &li) == FALSE)
    {
        return -1;
    }

    int64_t length = li.QuadPart;
    if (filePos_ + writePos_ > length)
    {
        length = filePos_ + writePos_;
    }

    return length;
}

void FileStream::FlushWriteBuffer()
{
    if (writePos_ <= 0)
    {
        return;
    }

    InternalWrite({&buffer_[0], writePos_});

    writePos_ = 0;
}

void* FileStream::InternalOpenHandle(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, FileOptions options)
{
    SECURITY_ATTRIBUTES securityAttributes{};
    bool useSecurityAttributes = static_cast<int32_t>(share) & static_cast<int32_t>(FileShare::Inheritable);
    if (useSecurityAttributes && (static_cast<int32_t>(share) & static_cast<int32_t>(FileShare::Inheritable)) != 0)
    {
        securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
        securityAttributes.bInheritHandle = TRUE;
    }

    auto desiredAccess = (access == FileAccess::Read)
        ? GENERIC_READ : (access == FileAccess::Write)
        ? GENERIC_WRITE : GENERIC_READ | GENERIC_WRITE;

    // Append is not a valid constant in Win32, so we must change it.
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

int32_t FileStream::InternalRead(Span<std::byte> bytes)
{
    DWORD readBytes = 0;
    if (ReadFile(fileHandle_, &bytes[0], bytes.Length(), &readBytes, nullptr) == FALSE)
    {
        return 0;
    }

    filePos_ += readBytes;
    return static_cast<int32_t>(readBytes);
}

int32_t FileStream::InternalWrite(ReadOnlySpan<std::byte> bytes)
{
    DWORD writtenBytes = 0;
    if (WriteFile(fileHandle_, &bytes[0], bytes.Length(), &writtenBytes, nullptr) == FALSE)
    {
        return 0;
    }

    filePos_ += writtenBytes;
    return static_cast<int32_t>(writtenBytes);
}

int64_t FileStream::InternalSeek(int64_t offset, SeekOrigin origin)
{
    LARGE_INTEGER distanceToMove;
    distanceToMove.QuadPart = offset;

    LARGE_INTEGER newFilePointer;
    if (SetFilePointerEx(fileHandle_, distanceToMove, &newFilePointer, static_cast<DWORD>(origin)) == FALSE)
    {
        return 0;
    }

    // Validate the file pointer position.
    filePos_ = newFilePointer.QuadPart;

    return newFilePointer.QuadPart;
}

void FileStream::InternalClose()
{
    if (fileHandle_ == NullFileHandle)
    {
        return;
    }

    CloseHandle(fileHandle_);
    fileHandle_ = NullFileHandle;
}

bool FileStream::InternalSetLength(int64_t value)
{
    auto prevFilePos = filePos_;
    if (filePos_ != value && InternalSeek(value, SeekOrigin::Begin) == 0)
    {
        return false;
    }

    if (SetEndOfFile(fileHandle_) == FALSE)
    {
        return false;
    }

    if (prevFilePos != value)
    {
        if (value <= prevFilePos)
        {
            // The file pointer position is now shorter than the current file length, so we must set it to EOF.
            if (InternalSeek(0, SeekOrigin::End) == 0)
            {
                return false;
            }
        }
        else
        {
            // The file pointer position has been changed, so roll back it.
            if (InternalSeek(prevFilePos, SeekOrigin::Begin))
            {
                return false;
            }
        }
    }

    return true;
}

void FileStream::InternalFlush() const
{
    FlushFileBuffers(fileHandle_);
}

CS2CPP_NAMESPACE_END