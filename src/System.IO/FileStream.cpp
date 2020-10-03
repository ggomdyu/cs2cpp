#include "System.IO/FileStream.h"
#include "System.IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

FileStream::FileStream(void* fileHandle, std::u16string path, FileAccess access, int32_t bufferSize) :
    fileHandle_(fileHandle),
    bufferSize_(bufferSize),
    readPos_(0),
    readLen_(0),
    writePos_(0),
    filePos_(0),
    access_(access),
    path_(std::move(path))
{
}

FileStream::FileStream(FileStream&& rhs) noexcept :
    fileHandle_(rhs.fileHandle_),
    buffer_(std::move(rhs.buffer_)),
    bufferSize_(rhs.bufferSize_),
    readPos_(rhs.readPos_),
    readLen_(rhs.readLen_),
    writePos_(rhs.writePos_),
    filePos_(rhs.filePos_),
    access_(rhs.access_),
    path_(std::move(rhs.path_))
{
    rhs.fileHandle_ = nullptr;
    rhs.bufferSize_ = 0;
    rhs.readPos_ = 0;
    rhs.readLen_ = 0;
    rhs.writePos_ = 0;
    rhs.filePos_ = 0;
}

FileStream::~FileStream()
{
    InternalClose();
}

FileStream& FileStream::operator=(FileStream&& rhs) noexcept
{
    std::swap(fileHandle_, rhs.fileHandle_);
    std::swap(buffer_, rhs.buffer_);
    std::swap(path_, rhs.path_);

    bufferSize_ = rhs.bufferSize_;
    readPos_ = rhs.readPos_;
    readLen_ = rhs.readLen_;
    writePos_ = rhs.writePos_;
    filePos_ = rhs.filePos_;
    access_ = rhs.access_;

    return *this;
}

bool FileStream::operator==(const FileStream& rhs) const noexcept
{
    return fileHandle_ == rhs.fileHandle_;
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
    {
        return nullptr;
    }

    auto fullPath = Path::GetFullPath(path);
    auto fileHandle = InternalOpenHandle(fullPath, mode, access, share, options);
    if (fileHandle == NullFileHandle)
    {
        return nullptr;
    }

    auto fileStream = std::make_shared<FileStream>(fileHandle, std::move(fullPath), access, bufferSize);
    if (mode == FileMode::Append)
    {
        fileStream->Seek(0, SeekOrigin::End);
    }

    return std::move(fileStream);
}

bool FileStream::CanRead() const noexcept
{
    return !IsClosed() && (size_t(access_) & size_t(FileAccess::Read)) != 0;
}

bool FileStream::CanSeek() const noexcept
{
    return !IsClosed();
}

bool FileStream::CanWrite() const noexcept
{
    return !IsClosed() && (size_t(access_) & size_t(FileAccess::Write)) != 0;
}

bool FileStream::SetLength(int64_t value)
{
    if (value < 0 || IsClosed() || !CanSeek() || !CanWrite())
    {
        return false;
    }

    // If the write buffer is not empty
    if (writePos_ > 0)
    {
        FlushWriteBuffer();
    }
    // If the read buffer is not empty
    else if (readPos_ < readLen_)
    {
        FlushReadBuffer();
    }

    return InternalSetLength(value);
}

int64_t FileStream::Position() const
{
    return filePos_ + writePos_;
}

int64_t FileStream::Seek(int64_t offset, SeekOrigin origin)
{
    if (!CanSeek())
    {
        return -1;
    }

    // If the write buffer is not empty
    if (writePos_ > 0)
    {
        FlushWriteBuffer();
    }
    else if (origin == SeekOrigin::Current)
    {
        // If we've read the buffer once before, then the seek offset is automatically moved to the end of the buffer.
        // So we must adjust the offset to set the seek offset as required.
        offset -= static_cast<int64_t>(readLen_) - readPos_;
    }

    readPos_ = readLen_ = 0;

    return InternalSeek(offset, origin);
}

void FileStream::Close()
{
    Flush();
    InternalClose();
}

std::u16string_view FileStream::Name() const noexcept
{
    return path_;
}

bool FileStream::IsClosed() const noexcept
{
    return fileHandle_ == NullFileHandle;
}

void FileStream::Flush()
{
    Flush(false);
}

std::vector<std::byte>& FileStream::GetBuffer()
{
    if (buffer_.empty())
    {
        buffer_.resize(bufferSize_);
    }

    return buffer_;
}

int32_t FileStream::Read(Span<std::byte> bytes)
{
    if (!CanRead() || bufferSize_ < bytes.Length())
    {
        return 0;
    }

    auto leftReadBufferSpace = readLen_ - readPos_;
    if (leftReadBufferSpace == 0)
    {
        FlushWriteBuffer();

        auto readBytes = InternalRead({&GetBuffer()[0], bufferSize_});
        readPos_ = 0;
        readLen_ = leftReadBufferSpace = readBytes;

        if (readBytes == 0)
        {
            return 0;
        }
    }

    auto copiedBytes = std::min(leftReadBufferSpace, bytes.Length());
    memcpy(&bytes[0], &buffer_[0] + readPos_, copiedBytes);

    readPos_ += copiedBytes;

    return copiedBytes;
}

int32_t FileStream::ReadByte()
{
    if (!CanRead())
    {
        return -1;
    }

    int32_t leftReadBufferSpace = readLen_ - readPos_;
    if (leftReadBufferSpace == 0)
    {
        FlushWriteBuffer();

        auto readBytes = InternalRead({&GetBuffer()[0], bufferSize_});
        readPos_ = 0;
        readLen_ = readBytes;

        if (readBytes == 0)
        {
            return -1;
        }
    }

    return static_cast<int32_t>(buffer_[readPos_++]);
}

bool FileStream::Write(ReadOnlySpan<std::byte> bytes)
{
    if (!CanWrite())
    {
        return false;
    }

    FlushReadBuffer();

    if (writePos_ > 0)
    {
        int32_t numBytes = bufferSize_ - writePos_;
        if (numBytes > 0)
        {
            // If the specified buffer can be stored into the m_buffer directly
            if (bytes.Length() <= numBytes)
            {
                memcpy(&GetBuffer()[writePos_], bytes, bytes.Length());
                writePos_ += bytes.Length();
                return true;
            }

            memcpy(&GetBuffer()[writePos_], bytes, static_cast<size_t>(numBytes));
            writePos_ += numBytes;
            bytes = bytes.Slice(numBytes);
        }

        FlushWriteBuffer();
    }

    if (bufferSize_ < bytes.Length())
    {
        InternalWrite(bytes);
        return true;
    }

    memcpy(&GetBuffer()[writePos_], bytes, bytes.Length());
    writePos_ += bytes.Length();

    return true;
}

bool FileStream::WriteByte(std::byte value)
{
    if (!CanWrite())
    {
        return false;
    }

    FlushReadBuffer();

    if (writePos_ == bufferSize_)
    {
        FlushWriteBuffer();
    }

    GetBuffer()[writePos_++] = value;

    return true;
}

void FileStream::FlushReadBuffer()
{
    if (writePos_ != 0)
    {
        return;
    }

    // We must rewind the seek pointer if a write occurred.
    int32_t rewindOffset = readPos_ - readLen_;
    if (rewindOffset != 0)
    {
        InternalSeek(rewindOffset, SeekOrigin::Current);
    }

    readLen_ = readPos_ = 0;
}

void FileStream::Flush(bool flushToDisk)
{
    if (writePos_ > 0 && CanWrite())
    {
        FlushWriteBuffer();
    }
    else if (readPos_ < readLen_ && CanSeek())
    {
        FlushReadBuffer();
    }

    if (flushToDisk)
    {
        InternalFlush();
    }
}

CS2CPP_NAMESPACE_END
