#pragma once

#include <cstdint>
#include <gsl/span>
#include <numeric>

CS2CPP_NAMESPACE_BEGIN

enum class FileMode
{
    CreateNew = 1,
    Create = 2,
    Open = 3,
    OpenOrCreate = 4,
    Truncate = 5,
    Append = 6,
};

enum class FileAccess
{
    Read = 1,
    Write = 2,
    ReadWrite = Read | Write
};

enum class FileShare
{
    None = 0,
    Read = 1,
    Write = 2,
    ReadWrite = 3,
    Delete = 4,
    Inheritable = 16
};

enum class FileOptions
{
    WriteThrough = std::numeric_limits<int>::min(),
    None = 0,
    Encrypted = 16384,
    DeleteOnClose = 67108864,
    SequentialScan = 134217728,
    RandomAccess = 268435456,
    Asynchronous = 1073741824
};

enum class SeekOrigin
{
    Begin,
    Current,
    End
};

class Stream
{
public:
    Stream() noexcept = default;
    Stream(const Stream& rhs) = delete;
    Stream(Stream&& rhs) noexcept = default;

public:
    virtual ~Stream() = default;

public:
    Stream& operator=(const Stream& rhs) = delete;
    Stream& operator=(Stream&& rhs) noexcept = default;

public:
    [[nodiscard]] virtual bool CanRead() const noexcept = 0;
    [[nodiscard]] virtual bool CanSeek() const noexcept = 0;
    [[nodiscard]] virtual bool CanWrite() const noexcept = 0;
    virtual bool SetLength(int64_t value) = 0;
    [[nodiscard]] virtual int64_t Length() const = 0;
    [[nodiscard]] virtual int64_t Position() const = 0;
    int32_t Read(gsl::span<std::byte> bytes);
    virtual int32_t Read(std::byte* bytes, int32_t count) = 0;
    virtual int32_t ReadByte() = 0;
    bool Write(gsl::span<const std::byte> bytes);
    virtual bool Write(const std::byte* bytes, int32_t count) = 0;
    virtual bool WriteByte(std::byte value) = 0;
    virtual int64_t Seek(int64_t offset, SeekOrigin origin) = 0;
    virtual void Close() = 0;
    virtual void Flush() = 0;
};

inline int32_t Stream::Read(gsl::span<std::byte> bytes)
{
    return this->Read(&bytes[0], static_cast<int32_t>(bytes.size()));
}

inline bool Stream::Write(gsl::span<const std::byte> bytes)
{
    return this->Write(&bytes[0], static_cast<int32_t>(bytes.size()));
}

CS2CPP_NAMESPACE_END