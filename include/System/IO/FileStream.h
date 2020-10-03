#pragma once

#include <vector>

#include "Stream.h"
#include "System/String.h"

CS2CPP_NAMESPACE_BEGIN

class FileStream :
    public Stream
{
public:
    FileStream(const FileStream& rhs) = delete;
    FileStream(FileStream&& rhs) noexcept;
protected:
    FileStream(void* nativeFileHandle, std::u16string_view path, FileAccess access, int32_t bufferSize);

public:
    ~FileStream() override;

public:
    FileStream& operator=(const FileStream& rhs) = delete;
    FileStream& operator=(FileStream&& rhs) noexcept;
    [[nodiscard]] bool operator==(const FileStream& rhs) const noexcept;
    [[nodiscard]] bool operator!=(const FileStream& rhs) const noexcept;

public:
    using Stream::Write;
    using Stream::Read;

    [[nodiscard]] static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode);
    [[nodiscard]] static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode, FileAccess access);
    [[nodiscard]] static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode, FileAccess access, FileShare share);
    [[nodiscard]] static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, int32_t bufferSize);
    [[nodiscard]] static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, FileOptions options, int32_t bufferSize);
    [[nodiscard]] bool CanRead() const noexcept override;
    [[nodiscard]] bool CanSeek() const noexcept override;
    [[nodiscard]] bool CanWrite() const noexcept override;
    [[nodiscard]] int64_t Length() const override;
    [[nodiscard]] int64_t Position() const override;
    [[nodiscard]] const String& Name() const noexcept;
    [[nodiscard]] bool IsClosed() const noexcept;
    int32_t Read(std::byte* bytes, int32_t count) override;
    int32_t ReadByte() override;
    bool Write(const std::byte* bytes, int32_t count) override;
    bool WriteByte(std::byte value) override;
    bool SetLength(int64_t value) override;
    int64_t Seek(int64_t offset, SeekOrigin origin) override;
    void Close() override;
    void Flush() override;
    void Flush(bool flushToDisk);

private:
    [[nodiscard]] std::vector<std::byte>& GetBuffer();
    void FlushWriteBuffer();
    void FlushReadBuffer();
    [[nodiscard]] static void* InternalOpenHandle(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, FileOptions options);
    int32_t InternalRead(std::byte* bytes, int32_t count);
    int32_t InternalWrite(const std::byte* bytes, int32_t count);
    int64_t InternalSeek(int64_t offset, SeekOrigin origin);
    void InternalClose();
    void InternalFlush() const;
    [[nodiscard]] bool InternalSetLength(int64_t value);

private:
    static constexpr FileShare DefaultShare = FileShare::Read;
    static constexpr FileOptions DefaultFileOption = FileOptions::None;
    static constexpr int DefaultBufferSize = 4096;
    static void* NullNativeFileHandle;

    void* _nativeFileHandle;
    std::vector<std::byte> _buffer;
    int32_t _bufferSize;
    int32_t _readPos;
    int32_t _readLen;
    int32_t _writePos;
    int64_t _filePos;
    FileAccess _access;
    String _path;
};

CS2CPP_NAMESPACE_END