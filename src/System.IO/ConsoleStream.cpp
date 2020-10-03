#include <array>

#include "System.IO/ConsoleStream.h"

CS2CPP_NAMESPACE_BEGIN

ConsoleStream::~ConsoleStream()
{
    InternalClose();
}

bool ConsoleStream::CanSeek() const noexcept
{
    return false;
}

bool ConsoleStream::CanRead() const noexcept
{
    return canRead_;
}

bool ConsoleStream::CanWrite() const noexcept
{
    return canWrite_;
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

int32_t ConsoleStream::ReadByte()
{
    std::array<std::byte, 1> bytes{};
    if (Read(bytes) == 0)
    {
        return -1;
    }

    return static_cast<int32_t>(bytes[0]);
}

bool ConsoleStream::WriteByte(std::byte value)
{
    return Write(std::array{value});
}

int64_t ConsoleStream::Seek(int64_t offset, SeekOrigin origin)
{
    return -1;
}

void ConsoleStream::Close()
{
    InternalClose();
}

void ConsoleStream::Flush()
{
}

void ConsoleStream::InternalClose()
{
    handle_ = nullptr;
}

CS2CPP_NAMESPACE_END
