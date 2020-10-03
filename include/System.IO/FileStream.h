#pragma once

#include <string>
#include <vector>

#include "System.IO/FileAccess.h"
#include "System.IO/FileMode.h"
#include "System.IO/FileOptions.h"
#include "System.IO/FileShare.h"
#include "System.IO/Stream.h"

CS2CPP_NAMESPACE_BEGIN

class FileStream final :
    public Stream
{
public:
    FileStream(void* nativeFileHandle, std::u16string path, FileAccess access, int32_t bufferSize);
    FileStream(const FileStream& rhs) = delete;
    FileStream(FileStream&& rhs) noexcept;
    ~FileStream() override;
    
public:
    FileStream& operator=(const FileStream& rhs) = delete;
    FileStream& operator=(FileStream&& rhs) noexcept;
    bool operator==(const FileStream& rhs) const noexcept;
    bool operator!=(const FileStream& rhs) const noexcept;

public:
    static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode);
    static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode, FileAccess access);
    static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode, FileAccess access, FileShare share);
    static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, int32_t bufferSize);
    static std::shared_ptr<FileStream> Create(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, FileOptions options, int32_t bufferSize);
    bool CanRead() const noexcept override;
    bool CanSeek() const noexcept override;
    bool CanWrite() const noexcept override;
    int64_t Length() const override;
    int64_t Position() const override;
    std::u16string_view Name() const noexcept;
    bool IsClosed() const noexcept;
    int32_t Read(Span<std::byte> bytes) override;
    int32_t ReadByte() override;
    bool Write(ReadOnlySpan<std::byte> bytes) override;
    bool WriteByte(std::byte value) override;
    bool SetLength(int64_t value) override;
    int64_t Seek(int64_t offset, SeekOrigin origin) override;
    void Close() override;
    void Flush() override;
    void Flush(bool flushToDisk);

private:
    std::vector<std::byte>& GetBuffer();
    void FlushWriteBuffer();
    void FlushReadBuffer();
    static void* InternalOpenHandle(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, FileOptions options);
    int32_t InternalRead(Span<std::byte> bytes);
    int32_t InternalWrite(ReadOnlySpan<std::byte> bytes);
    int64_t InternalSeek(int64_t offset, SeekOrigin origin);
    void InternalClose();
    void InternalFlush() const;
    bool InternalSetLength(int64_t value);

private:
    static constexpr FileShare DefaultShare = FileShare::Read;
    static constexpr FileOptions DefaultFileOption = FileOptions::None;
    static constexpr int DefaultBufferSize = 4096;
    static void* const NullFileHandle;

    void* fileHandle_;
    std::vector<std::byte> buffer_;
    int32_t bufferSize_;
    int32_t readPos_;
    int32_t readLen_;
    int32_t writePos_;
    int64_t filePos_;
    FileAccess access_;
    std::u16string path_;
};

CS2CPP_NAMESPACE_END
