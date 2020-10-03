#include "System/IO/ConsoleStream.h"

CS2CPP_NAMESPACE_BEGIN

ConsoleStream::ConsoleStream(FileAccess access) noexcept :
    _canRead(access == FileAccess::Read || access == FileAccess::ReadWrite),
    _canWrite(access == FileAccess::Write || access == FileAccess::ReadWrite)
{
}

bool ConsoleStream::CanSeek() const noexcept
{
    return false;
}

bool ConsoleStream::CanRead() const noexcept
{
    return _canRead;
}

bool ConsoleStream::CanWrite() const noexcept
{
    return _canWrite;
}

bool ConsoleStream::SetLength(int64_t value)
{
    return false;
}

int64_t ConsoleStream::Length() const
{
    return -1;
}

int64_t ConsoleStream::Position() const
{
    return -1;
}

int64_t ConsoleStream::Seek(int64_t offset, SeekOrigin origin)
{
    return -1;
}

void ConsoleStream::Close()
{
}

void ConsoleStream::Flush()
{
}

CS2CPP_NAMESPACE_END
