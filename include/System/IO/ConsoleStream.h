#pragma once

#include "Stream.h"

CS2CPP_NAMESPACE_BEGIN

class ConsoleStream :
    public Stream
{
public:
    explicit ConsoleStream(FileAccess access) noexcept;

public:
    [[nodiscard]] bool CanRead() const noexcept override;
    [[nodiscard]] bool CanSeek() const noexcept override;
    [[nodiscard]] bool CanWrite() const noexcept override;
    bool SetLength(int64_t value) override;
    [[nodiscard]] int64_t Length() const override;
    [[nodiscard]] int64_t Position() const override;
    int64_t Seek(int64_t offset, SeekOrigin origin) override;
    void Close() override;
    void Flush() override;

private:
    bool _canRead = false;
    bool _canWrite = false;
};

CS2CPP_NAMESPACE_END