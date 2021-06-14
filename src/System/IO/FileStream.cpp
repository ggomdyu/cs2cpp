#include "System/IO/FileStream.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::filestream
{

class FileStream final :
    public CS2CPP_NAMESPACE::FileStream
{
public:
    FileStream(void* nativeFileHandle, std::u16string_view path, FileAccess access, int32_t bufferSize);
};

FileStream::FileStream(void* nativeFileHandle, std::u16string_view path, FileAccess access, int32_t bufferSize) :
    CS2CPP_NAMESPACE::FileStream(nativeFileHandle, path, access, bufferSize)
{
}

}

FileStream::FileStream(void* nativeFileHandle, std::u16string_view path, FileAccess access, int32_t bufferSize) :
    _nativeFileHandle(nativeFileHandle),
    _bufferSize(bufferSize),
    _readPos(0),
    _readLen(0),
    _writePos(0),
    _filePos(0),
    _access(access),
    _path(path)
{
}

FileStream::FileStream(FileStream&& rhs) noexcept :
    _nativeFileHandle(rhs._nativeFileHandle),
    _buffer(std::move(rhs._buffer)),
    _bufferSize(rhs._bufferSize),
    _readPos(rhs._readPos),
    _readLen(rhs._readLen),
    _writePos(rhs._writePos),
    _filePos(rhs._filePos),
    _access(rhs._access),
    _path(std::move(rhs._path))
{
    rhs._nativeFileHandle = nullptr;
    rhs._bufferSize = 0;
    rhs._readPos = 0;
    rhs._readLen = 0;
    rhs._writePos = 0;
    rhs._filePos = 0;
}

FileStream::~FileStream()
{
    InternalClose();
}

FileStream& FileStream::operator=(FileStream&& rhs) noexcept
{
    std::swap(_nativeFileHandle, rhs._nativeFileHandle);
    std::swap(_buffer, rhs._buffer);
    std::swap(_path, rhs._path);

    _bufferSize = rhs._bufferSize;
    _readPos = rhs._readPos;
    _readLen = rhs._readLen;
    _writePos = rhs._writePos;
    _filePos = rhs._filePos;
    _access = rhs._access;

    return *this;
}

bool FileStream::operator==(const FileStream& rhs) const noexcept
{
    return _nativeFileHandle == rhs._nativeFileHandle;
}

bool FileStream::operator!=(const FileStream& rhs) const noexcept
{
    return !operator==(rhs);
}

std::shared_ptr<FileStream> FileStream::Create(std::u16string_view path, FileMode mode)
{
    return Create(path, mode, (mode == FileMode::Append ? FileAccess::Write : FileAccess::ReadWrite), DefaultShare,
        DefaultFileOption, DefaultBufferSize);
}

std::shared_ptr<FileStream> FileStream::Create(std::u16string_view path, FileMode mode, FileAccess access)
{
    return Create(path, mode, access, DefaultShare, DefaultFileOption, DefaultBufferSize);
}

std::shared_ptr<FileStream> FileStream::Create(std::u16string_view path, FileMode mode, FileAccess access, FileShare share)
{
    return Create(path, mode, access, share, DefaultFileOption, DefaultBufferSize);
}

std::shared_ptr<FileStream> FileStream::Create(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, int32_t bufferSize)
{
    return Create(path, mode, access, share, DefaultFileOption, bufferSize);
}

std::shared_ptr<FileStream> FileStream::Create(std::u16string_view path, FileMode mode, FileAccess access, FileShare share, FileOptions options, int32_t bufferSize)
{
    if (bufferSize <= 0)
        return {};

    auto nativeFileHandle = InternalOpenHandle(path, mode, access, share, options);
    if (nativeFileHandle == NullNativeFileHandle)
        return {};

    auto fileStream = std::make_shared<detail::filestream::FileStream>(nativeFileHandle, path, access, bufferSize);
    if (mode == FileMode::Append)
        fileStream->Seek(0, SeekOrigin::End);

    return std::move(fileStream);
}

bool FileStream::CanRead() const noexcept
{
    return !IsClosed() && (size_t(_access) & size_t(FileAccess::Read)) != 0;
}

bool FileStream::CanSeek() const noexcept
{
    return !IsClosed();
}

bool FileStream::CanWrite() const noexcept
{
    return !IsClosed() && (size_t(_access) & size_t(FileAccess::Write)) != 0;
}

bool FileStream::SetLength(int64_t value)
{
    if (value < 0 || IsClosed() || !CanSeek() || !CanWrite())
        return false;

    // If the write buffer is not empty
    if (_writePos > 0)
        FlushWriteBuffer();
    // If the read buffer is not empty
    else if (_readPos < _readLen)
        FlushReadBuffer();

    return InternalSetLength(value);
}

int64_t FileStream::Position() const
{
    return _filePos + _writePos;
}

int64_t FileStream::Seek(int64_t offset, SeekOrigin origin)
{
    if (!CanSeek())
        return -1;

    // If the write buffer is not empty
    if (_writePos > 0)
        FlushWriteBuffer();
    else if (origin == SeekOrigin::Current)
    {
        // If we've read the buffer once before, then the seek offset is automatically moved to the end of the buffer.
        // So we must adjust the offset to set the seek offset as required.
        offset -= static_cast<int64_t>(_readLen) - _readPos;
    }

    _readPos = _readLen = 0;

    return InternalSeek(offset, origin);
}

void FileStream::Close()
{
    Flush();
    InternalClose();
}

const std::u16string& FileStream::Name() const noexcept
{
    return _path;
}

bool FileStream::IsClosed() const noexcept
{
    return _nativeFileHandle == NullNativeFileHandle;
}

void FileStream::Flush()
{
    Flush(false);
}

std::vector<std::byte>& FileStream::GetBuffer()
{
    if (_buffer.empty())
        _buffer.resize(_bufferSize);

    return _buffer;
}

int32_t FileStream::Read(std::byte* bytes, int32_t count)
{
    if (!CanRead() || _bufferSize < count)
        return 0;

    auto leftReadBufferSpace = _readLen - _readPos;
    if (leftReadBufferSpace == 0)
    {
        FlushWriteBuffer();

        auto readBytes = InternalRead(&GetBuffer()[0], _bufferSize);
        _readPos = 0;
        _readLen = leftReadBufferSpace = readBytes;

        if (readBytes == 0)
            return 0;
    }

    auto copiedBytes = std::min(leftReadBufferSpace, count);
    std::memcpy(bytes, &_buffer[0] + _readPos, copiedBytes);

    _readPos += copiedBytes;

    return copiedBytes;
}

int32_t FileStream::ReadByte()
{
    if (!CanRead())
        return -1;

    int32_t leftReadBufferSpace = _readLen - _readPos;
    if (leftReadBufferSpace == 0)
    {
        FlushWriteBuffer();

        auto readBytes = InternalRead(&GetBuffer()[0], _bufferSize);
        _readPos = 0;
        _readLen = readBytes;

        if (readBytes == 0)
            return -1;
    }

    return static_cast<int32_t>(_buffer[_readPos++]);
}

bool FileStream::Write(const std::byte* bytes, int32_t count)
{
    if (!CanWrite())
        return false;

    FlushReadBuffer();

    if (_writePos > 0)
    {
        int32_t numBytes = _bufferSize - _writePos;
        if (numBytes > 0)
        {
            // If the specified buffer can be stored into the m_buffer directly
            if (count <= numBytes)
            {
                std::memcpy(&GetBuffer()[_writePos], bytes, count);
                _writePos += count;
                return true;
            }

            std::memcpy(&GetBuffer()[_writePos], bytes, static_cast<size_t>(numBytes));
            _writePos += numBytes;
            bytes += numBytes;
            count -= numBytes;
        }

        FlushWriteBuffer();
    }

    if (_bufferSize < count)
    {
        InternalWrite(bytes, count);
        return true;
    }

    std::memcpy(&GetBuffer()[_writePos], bytes, count);
    _writePos += count;

    return true;
}

bool FileStream::WriteByte(std::byte value)
{
    if (!CanWrite())
        return false;

    FlushReadBuffer();

    if (_writePos == _bufferSize)
        FlushWriteBuffer();

    GetBuffer()[_writePos++] = value;

    return true;
}

void FileStream::FlushReadBuffer()
{
    if (_writePos != 0)
        return;

    // We must rewind the seek pointer if a write occurred.
    int32_t rewindOffset = _readPos - _readLen;
    if (rewindOffset != 0)
        InternalSeek(rewindOffset, SeekOrigin::Current);

    _readLen = _readPos = 0;
}

void FileStream::Flush(bool flushToDisk)
{
    if (_writePos > 0 && CanWrite())
        FlushWriteBuffer();
    else if (_readPos < _readLen && CanSeek())
        FlushReadBuffer();

    if (flushToDisk)
        InternalFlush();
}

CS2CPP_NAMESPACE_END
