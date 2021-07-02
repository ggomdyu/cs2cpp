#include <cstdio>

#include "System/Diagnostics/Debug.h"
#include "System/IO/File.h"
#include "System/IO/FileStream.h"

CS2CPP_NAMESPACE_BEGIN

void* const FileStream::NullNativeFileHandle = nullptr;

namespace detail::filestream
{

constexpr const char* ConvertFileModeAccessToNative(FileMode mode, FileAccess access)
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

}

int64_t FileStream::Length() const
{
    FILE* nativeFileHandle = reinterpret_cast<FILE*>(_nativeFileHandle);

    auto prevSeekOffset = ftell(nativeFileHandle);
    fseek(nativeFileHandle, 0, SEEK_END);

    auto length = ftell(nativeFileHandle);
    fseek(nativeFileHandle, prevSeekOffset, SEEK_SET);

    if (_filePos + _writePos > length)
        length = _filePos + _writePos;

    return length;
}

void FileStream::FlushWriteBuffer()
{
    if (_writePos <= 0)
        return;
    
    InternalWrite(&_buffer[0], _writePos);
    fflush(reinterpret_cast<FILE*>(_nativeFileHandle));

    _writePos = 0;
}

void* FileStream::InternalOpenHandle(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, FileOptions options)
{
    if (mode == FileMode::OpenOrCreate && !File::Exists(path))
        mode = FileMode::Create;

    auto utf8Path = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * path.length()});

    return fopen(reinterpret_cast<const char*>(utf8Path->data()), detail::filestream::ConvertFileModeAccessToNative(mode, access));
}

int32_t FileStream::InternalRead(std::byte* bytes, int32_t count)
{
    auto readBytes = fread(bytes, 1, count, reinterpret_cast<FILE*>(_nativeFileHandle));
    if (readBytes == static_cast<decltype(readBytes)>(-1))
        return 0;

    _filePos += readBytes;
    return static_cast<int32_t>(readBytes);
}

int32_t FileStream::InternalWrite(const std::byte* buffer, int32_t count)
{
    auto writtenBytes = fwrite(buffer, 1, count, reinterpret_cast<FILE*>(_nativeFileHandle));
    if (writtenBytes == static_cast<decltype(writtenBytes)>(-1))
        return 0;

    _filePos += writtenBytes;
    return static_cast<int32_t>(writtenBytes);
}

int64_t FileStream::InternalSeek(int64_t offset, SeekOrigin origin)
{
    auto nativeFileHandle = reinterpret_cast<FILE*>(_nativeFileHandle);
    auto result = fseek(nativeFileHandle, offset, static_cast<int>(origin));
    if (result != 0)
        return 0;

    int64_t newFilePos = ftell(nativeFileHandle);
    if (newFilePos == -1)
        return 0;

    _filePos = newFilePos;

    return static_cast<int32_t>(newFilePos);
}

void FileStream::InternalClose()
{
    if (_nativeFileHandle == NullNativeFileHandle)
        return;

    fclose(reinterpret_cast<FILE*>(_nativeFileHandle));
    _nativeFileHandle = NullNativeFileHandle;
}

void FileStream::InternalFlush() const
{
    fflush(reinterpret_cast<FILE*>(_nativeFileHandle));
}

bool FileStream::InternalSetLength([[maybe_unused]] int64_t value)
{
    return false;
}

CS2CPP_NAMESPACE_END
