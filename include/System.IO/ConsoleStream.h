#pragma once

#include "System.IO/FileAccess.h"
#include "System.IO/Stream.h"

CS2CPP_NAMESPACE_BEGIN

class ConsoleStream final :
    public Stream
{
public:
    explicit ConsoleStream(void* handle, FileAccess access, bool useFileAPIs) noexcept;
    ConsoleStream(const ConsoleStream& rhs) = delete;
    ConsoleStream(ConsoleStream&& rhs) noexcept = default;
    ~ConsoleStream() override;

public:
    ConsoleStream& operator=(const ConsoleStream& rhs) = delete;
    ConsoleStream& operator=(ConsoleStream&& rhs) noexcept = default;

public:
    bool CanRead() const noexcept override;
    bool CanSeek() const noexcept override;
    bool CanWrite() const noexcept override;
    bool SetLength(int64_t value) override;
    int64_t Length() const override;
    int64_t Position() const override;
    int32_t Read(Span<std::byte> bytes) override;
    int32_t ReadByte() override;
    bool Write(ReadOnlySpan<std::byte> bytes) override;
    bool WriteByte(std::byte value) override;
    int64_t Seek(int64_t offset, SeekOrigin origin) override;
    void Close() override;
    void Flush() override;

private:
    void InternalClose();

private:
    void* handle_;
    bool canRead_ = false;
    bool canWrite_ = false;
    bool isPipe_ = false;
    bool useFileAPIs_ = false;
};

CS2CPP_NAMESPACE_END