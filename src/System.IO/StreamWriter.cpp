#include "System.IO/StreamWriter.h"
#include "System.IO/FileStream.h"

CS2CPP_NAMESPACE_BEGIN

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
    auto fs = CreateFileStream(path, append);
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
    auto fs = CreateFileStream(path, append);
    if (!fs)
    {
        return std::nullopt;
    }

    bool haveWrittenPreamble = fs->CanSeek() && fs->Position() > 0;
    return StreamWriter(std::move(fs), encoding, bufferSize, false, haveWrittenPreamble);
}

void StreamWriter::Write(std::u16string_view value)
{
    WriteCore(value, false);
}

void StreamWriter::WriteLine(std::u16string_view value)
{
    WriteCore(value, true);
}

std::shared_ptr<Encoding> StreamWriter::GetEncoding() const noexcept
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

    auto& byteBuffer = GetByteBuffer();
    auto convertedBytes = encoding_->GetBytes(ReadOnlySpan(GetCharBuffer().data(), charPos_), byteBuffer);
    if (convertedBytes && *convertedBytes > 0)
    {
        stream_->Write({byteBuffer.data(), *convertedBytes});
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

std::vector<std::byte>& StreamWriter::GetByteBuffer()
{
    if (byteBuffer_.empty())
    {
        byteBuffer_.resize(size_t(encoding_->GetMaxByteCount(bufferSize_)));
    }

    return byteBuffer_;
}

void StreamWriter::WriteCore(std::u16string_view value, bool appendNewLine)
{
    auto& charBuffer = GetCharBuffer();

    auto writeCore = [&](std::u16string_view value)
    {
        while (!value.empty())
        {
            if (charPos_ == charBuffer.size())
            {
                Flush();
            }

            auto leftBufferSize = charBuffer.size() - charPos_;
            auto copyAvailableSize = std::min(value.size(), leftBufferSize);
            memcpy(charBuffer.data() + charPos_, value.data(), copyAvailableSize * sizeof(char16_t));

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

std::vector<char16_t>& StreamWriter::GetCharBuffer()
{
    if (charBuffer_.empty())
    {
        charBuffer_.resize(static_cast<size_t>(bufferSize_));
    }

    return charBuffer_;
}

std::shared_ptr<Stream> StreamWriter::CreateFileStream(std::u16string_view path, bool append)
{
    return FileStream::Create(path, append ? FileMode::Append : FileMode::Create, FileAccess::Write,
        FileShare::Read, FileOptions::SequentialScan, DefaultFileStreamBufferSize);
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