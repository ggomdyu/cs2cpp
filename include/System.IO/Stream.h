#pragma once

#include <memory>

#include "System.IO/SeekOrigin.h"
#include "System/ReadOnlySpan.h"

CS2CPP_NAMESPACE_BEGIN

class Stream
{
public:
    Stream() noexcept = default;
    Stream(const Stream& rhs) = delete;
    Stream(Stream&& rhs) noexcept = default;
    virtual ~Stream() = default;

public:
    Stream& operator=(const Stream& rhs) = delete;
    Stream& operator=(Stream&& rhs) noexcept = default;

public:
    virtual bool CanRead() const noexcept = 0;
    virtual bool CanSeek() const noexcept = 0;
    virtual bool CanWrite() const noexcept = 0;
    virtual bool SetLength(int64_t value) = 0;
    virtual int64_t Length() const = 0;
    virtual int64_t Position() const = 0;
    virtual int32_t Read(Span<std::byte> bytes) = 0;
    virtual int32_t ReadByte() = 0;
    virtual bool Write(ReadOnlySpan<std::byte> bytes) = 0;
    virtual bool WriteByte(std::byte value) = 0;
    virtual int64_t Seek(int64_t offset, SeekOrigin origin) = 0;
    virtual void Close() = 0;
    virtual void Flush() = 0;
};

CS2CPP_NAMESPACE_END
