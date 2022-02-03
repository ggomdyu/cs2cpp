#include "System.IO/StreamWriter.h"
#include "System.IO/FileStream.h"

CS2CPP_NAMESPACE_BEGIN

constexpr int32_t DefaultBufferSize = 1024;
constexpr int32_t DefaultFileStreamBufferSize = 4096;

static std::shared_ptr<FileStream> CreateFileStream(std::u16string_view path, bool append)
{
    FileMode fileMode = append ? FileMode::Append : FileMode::Create;
    return FileStream::Create(path, fileMode, FileAccess::Write, FileShare::Read, FileOptions::SequentialScan, DefaultFileStreamBufferSize);
}

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream) noexcept :
    StreamWriter(std::move(stream), Encoding::UTF8(), DefaultBufferSize, false, false)
{
}

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream, const class Encoding& encoding) noexcept :
    StreamWriter(std::move(stream), encoding, DefaultBufferSize)
{
}

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream, const class Encoding& encoding, int32_t bufferSize) noexcept :
    StreamWriter(std::move(stream), encoding, DefaultBufferSize, false)
{
}

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream, const class Encoding& encoding, int32_t bufferSize, bool leaveOpen) noexcept :
    StreamWriter(std::move(stream), encoding, bufferSize, leaveOpen, true)
{
}

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream, const class Encoding& encoding, int32_t bufferSize, bool leaveOpen, bool haveWrittenPreamble) noexcept :
    stream_(std::move(stream)),
    encoding_(encoding.Clone()),
    bufferSize_(bufferSize),
    charPos_(0),
    leaveOpen_(leaveOpen),
    haveWrittenPreamble_(haveWrittenPreamble),
    autoFlush_(false)
{
}

std::optional<StreamWriter> StreamWriter::Create(std::u16string_view path)
{
    return Create(path, false);
}

std::optional<StreamWriter> StreamWriter::Create(std::u16string_view path, bool append)
{
    std::shared_ptr<FileStream> fs = CreateFileStream(path, append);
    if (!fs)
    {
        return std::nullopt;
    }

    // Don't write preamble.
    return StreamWriter(std::move(fs), Encoding::UTF8(), DefaultBufferSize, false, true);
}

std::optional<StreamWriter> StreamWriter::Create(std::u16string_view path, bool append, const class Encoding& encoding)
{
    return Create(path, append, encoding, DefaultBufferSize);
}

std::optional<StreamWriter> StreamWriter::Create(std::u16string_view path, bool append, const class Encoding& encoding, int32_t bufferSize)
{
    std::shared_ptr<FileStream> fs = CreateFileStream(path, append);
    if (!fs)
    {
        return std::nullopt;
    }

    bool haveWrittenPreamble = fs->CanSeek() && fs->Position() > 0;
    return StreamWriter(std::move(fs), encoding, bufferSize, false, haveWrittenPreamble);
}

void StreamWriter::Write(std::u16string_view value)
{
    InternalWrite(value, false);
}

void StreamWriter::WriteLine(std::u16string_view value)
{
    InternalWrite(value, true);
}

std::shared_ptr<Encoding> StreamWriter::Encoding() const noexcept
{
    return encoding_;
}

std::shared_ptr<Stream> StreamWriter::GetBaseStream() const noexcept
{
    return stream_;
}

void StreamWriter::Flush()
{
    if (charPos_ == 0 || !stream_)
    {
        return;
    }

    if (!haveWrittenPreamble_)
    {
        haveWrittenPreamble_ = true;
        if (!encoding_->Preamble().IsEmpty())
        {
            stream_->Write(encoding_->Preamble());
        }
    }

    Span<std::byte> byteBuffer = GetByteBuffer();
    std::optional<int> convertedBytes = encoding_->GetBytes(ReadOnlySpan(&GetCharBuffer()[0], charPos_), Span(byteBuffer));
    if (convertedBytes && *convertedBytes > 0)
    {
        stream_->Write({&byteBuffer[0], *convertedBytes});
    }

    stream_->Flush();
    charPos_ = 0;
}

void StreamWriter::Close()
{
    if (!stream_ || leaveOpen_)
    {
        return;
    }

    Flush();
    stream_->Close();
}

Span<std::byte> StreamWriter::GetByteBuffer()
{
    if (byteBuffer_.empty())
    {
        byteBuffer_.resize(size_t(encoding_->GetMaxByteCount(bufferSize_)));
    }

    return byteBuffer_;
}

void StreamWriter::InternalWrite(std::u16string_view value, bool appendNewLine)
{
    Span<char16_t> charBuffer = GetCharBuffer();

    auto writeCore = [&](std::u16string_view value)
    {
        while (!value.empty())
        {
            if (charPos_ == charBuffer.Length())
            {
                Flush();
            }

            auto leftBufferSize = charBuffer.Length() - charPos_;
            auto copyAvailableSize = std::min(static_cast<int32_t>(value.size()), leftBufferSize);
            memcpy(&charBuffer[0] + charPos_, value.data(), copyAvailableSize * sizeof(char16_t));

            value = value.substr(copyAvailableSize);
            charPos_ += static_cast<int32_t>(copyAvailableSize);
        }
    };

    writeCore(value);
    if (appendNewLine)
    {
        writeCore(GetNewLine());
    }

    if (autoFlush_)
    {
        Flush();
    }
}

Span<char16_t> StreamWriter::GetCharBuffer()
{
    if (charBuffer_.empty())
    {
        charBuffer_.resize(static_cast<size_t>(bufferSize_));
    }

    return charBuffer_;
}

void StreamWriter::SetAutoFlush(bool autoFlush) noexcept
{
    autoFlush_ = autoFlush;
    if (autoFlush)
    {
        Flush();
    }
}

bool StreamWriter::GetAutoFlush() const noexcept
{
    return autoFlush_;
}

CS2CPP_NAMESPACE_END