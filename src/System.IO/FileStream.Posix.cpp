#include <cstdio>

#include "System.IO/File.h"
#include "System.IO/FileStream.h"

CS2CPP_NAMESPACE_BEGIN

void* const FileStream::NullFileHandle = nullptr;

constexpr const char* FileModeAccessToNative(FileMode mode, FileAccess access)
{
    constexpr const char* fileModeTable[][3] = {
        {nullptr, "wb", "wb+"},
        {nullptr, "wb", "wb+"},
        {"rb", "wb", "rb+"},
        {"rb", "wb", "rb+"},
        {nullptr, "wb", "wb+"},
        {nullptr, "ab", "ab+"},
    };

    return fileModeTable[static_cast<int32_t>(mode) - 1][static_cast<int32_t>(access) - 1];
}

int64_t FileStream::Length() const
{
    auto fileHandle = reinterpret_cast<FILE*>(fileHandle_);
    auto prevSeekOffset = ftell(fileHandle);
    fseek(fileHandle, 0, SEEK_END);

    auto length = ftell(fileHandle);
    fseek(fileHandle, prevSeekOffset, SEEK_SET);

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
    fflush(reinterpret_cast<FILE*>(fileHandle_));

    writePos_ = 0;
}

void* FileStream::InternalOpenHandle(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, FileOptions options)
{
    auto utf8Path = Encoding::UTF8().GetBytes({path.data(), static_cast<int32_t>(path.length())});
    if (!utf8Path)
    {
        return nullptr;
    }

    if (mode == FileMode::OpenOrCreate && !File::Exists(path))
    {
        mode = FileMode::Create;
    }

    return fopen(reinterpret_cast<const char*>(utf8Path->data()), FileModeAccessToNative(mode, access));
}

int32_t FileStream::InternalRead(Span<std::byte> bytes)
{
    auto readBytes = static_cast<int32_t>(fread(&bytes[0], 1, bytes.Length(), reinterpret_cast<FILE*>(fileHandle_)));
    if (readBytes == -1)
    {
        return 0;
    }

    filePos_ += readBytes;
    return readBytes;
}

int32_t FileStream::InternalWrite(ReadOnlySpan<std::byte> bytes)
{
    auto writtenBytes = static_cast<int32_t>(fwrite(&bytes[0], 1, bytes.Length(), reinterpret_cast<FILE*>(fileHandle_)));
    if (writtenBytes == -1)
    {
        return 0;
    }

    filePos_ += writtenBytes;
    return writtenBytes;
}

int64_t FileStream::InternalSeek(int64_t offset, SeekOrigin origin)
{
    auto fp = reinterpret_cast<FILE*>(fileHandle_);
    auto result = fseek(fp, offset, static_cast<int>(origin));
    if (result != 0)
    {
        return 0;
    }

    int64_t newFilePos = ftell(fp);
    if (newFilePos == -1)
    {
        return 0;
    }

    filePos_ = newFilePos;

    return static_cast<int32_t>(newFilePos);
}

void FileStream::InternalClose()
{
    if (fileHandle_ == NullFileHandle)
    {
        return;
    }

    fclose(reinterpret_cast<FILE*>(fileHandle_));
    fileHandle_ = NullFileHandle;
}

void FileStream::InternalFlush() const
{
    fflush(reinterpret_cast<FILE*>(fileHandle_));
}

bool FileStream::InternalSetLength([[maybe_unused]] int64_t value)
{
    return false;
}

CS2CPP_NAMESPACE_END
