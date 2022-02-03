#include <cassert>

#include "System.IO/FileStream.h"
#include "System.IO/StreamReader.h"

CS2CPP_NAMESPACE_BEGIN

class StreamDecoderFallback final :
    public DecoderFallback
{
public:
    explicit StreamDecoderFallback(std::vector<std::byte>& bytesUnknown);

public:
    void Fallback(ReadOnlySpan<std::byte> bytesUnknown) override;

public:
    std::vector<std::byte>& bytesUnknown_;
};

StreamDecoderFallback::StreamDecoderFallback(std::vector<std::byte>& bytesUnknown) :
    bytesUnknown_(bytesUnknown)
{
}

void StreamDecoderFallback::Fallback(ReadOnlySpan<std::byte> bytesUnknown)
{
    bytesUnknown_.insert(bytesUnknown_.end(), begin(bytesUnknown), end(bytesUnknown));
}


StreamReader::StreamReader(std::shared_ptr<Stream> stream) noexcept :
    StreamReader(std::move(stream), true)
{
}

StreamReader::StreamReader(std::shared_ptr<Stream> stream, const Encoding& encoding) noexcept :
    StreamReader(std::move(stream), encoding, true, DefaultBufferSize, false)
{
}

StreamReader::StreamReader(std::shared_ptr<Stream> stream, const Encoding& encoding, bool detectEncodingFromByteOrderMarks) noexcept :
    StreamReader(std::move(stream), encoding, detectEncodingFromByteOrderMarks, DefaultBufferSize)
{
}

StreamReader::StreamReader(std::shared_ptr<Stream> stream, const Encoding& encoding, bool detectEncodingFromByteOrderMarks, int32_t bufferSize) noexcept :
    StreamReader(std::move(stream), encoding, detectEncodingFromByteOrderMarks, bufferSize, false)
{
}

StreamReader::StreamReader(std::shared_ptr<Stream> stream, const Encoding& encoding, bool detectEncodingFromByteOrderMarks, int32_t bufferSize, bool leaveOpen) noexcept :
    stream_(std::move(stream)),
    byteBufferSize_(bufferSize),
    encoding_(CloneEncoding(encoding)),
    detectEncoding_(detectEncodingFromByteOrderMarks),
    leaveOpen_(leaveOpen),
    charPos_(0),
    charLen_(0),
    bytePos_(0)
{
}

StreamReader::StreamReader(std::shared_ptr<Stream> stream, bool detectEncodingFromByteOrderMarks) noexcept :
    StreamReader(std::move(stream), Encoding::UTF8(), detectEncodingFromByteOrderMarks)
{
}

std::optional<StreamReader> StreamReader::Create(std::u16string_view path)
{
    return Create(path, true);
}

std::optional<StreamReader> StreamReader::Create(std::u16string_view path, bool detectEncodingFromByteOrderMarks)
{
    return Create(path, Encoding::UTF8(), detectEncodingFromByteOrderMarks, DefaultBufferSize);
}

std::optional<StreamReader> StreamReader::Create(std::u16string_view path, const Encoding& encoding)
{
    return Create(path, encoding, true, DefaultBufferSize);
}

std::optional<StreamReader> StreamReader::Create(std::u16string_view path, const Encoding& encoding, bool detectEncodingFromByteOrderMarks)
{
    return Create(path, encoding, detectEncodingFromByteOrderMarks, DefaultBufferSize);
}

std::optional<StreamReader> StreamReader::Create(std::u16string_view path, const Encoding& encoding, bool detectEncodingFromByteOrderMarks, int32_t bufferSize)
{
    if (bufferSize <= 0)
    {
        return std::nullopt;
    }

    auto stream = FileStream::Create(path, FileMode::Open, FileAccess::Read, FileShare::Read, FileOptions::SequentialScan,
        DefaultFileStreamBufferSize);
    if (!stream)
    {
        return std::nullopt;
    }

    return StreamReader(std::move(stream), encoding, detectEncodingFromByteOrderMarks, bufferSize);
}

std::shared_ptr<Encoding> StreamReader::CurrentEncoding() const noexcept
{
    return encoding_;
}

std::shared_ptr<Stream> StreamReader::BaseStream() const noexcept
{
    return stream_;
}

bool StreamReader::IsEndOfStream() const noexcept
{
    return stream_->Position() == stream_->Length();
}

int32_t StreamReader::Peek()
{
    if (charPos_ == charLen_ && ReadBuffer() == 0)
    {
        return -1;
    }

    return GetCharBuffer()[charPos_];
}

void StreamReader::Close()
{
    if (!leaveOpen_)
    {
        stream_->Close();
    }
}

int32_t StreamReader::Read()
{
    if (charPos_ == charLen_)
    {
        if (ReadBuffer() == 0)
        {
            return -1;
        }
    }

    return GetCharBuffer()[charPos_++];
}

std::shared_ptr<Encoding> StreamReader::CloneEncoding(const Encoding& encoding)
{
    auto newEncoding = encoding.Clone();
    newEncoding->SetDecoderFallback(std::make_shared<StreamDecoderFallback>(bytesUnknown_));

    return newEncoding;
}

int32_t StreamReader::ReadBuffer()
{
    charPos_ = charLen_ = bytePos_ = 0;

    std::vector<std::byte>& byteBuffer = GetByteBuffer();

    // If character bytes are overlapped at the boundary of byteBuffer, it will occur fallback while decoding.
    // Therefore, when the stream is read again, we need to fill byteBuffer with fallbackBuffer_.
    auto unknownBytesSize = static_cast<int32_t>(bytesUnknown_.size());
    if (!bytesUnknown_.empty())
    {
        assert(byteBuffer.size() > bytesUnknown_.size());
        byteBuffer.assign(bytesUnknown_.begin(), bytesUnknown_.end());
        bytesUnknown_.clear();
    }

    // If the stream indicates EOF, then escape the function.
    int32_t readByteCount;
    if ((readByteCount = stream_->Read({&byteBuffer[unknownBytesSize], static_cast<int32_t>(byteBuffer.size()) - unknownBytesSize})) == 0)
    {
        return 0;
    }

    readByteCount += unknownBytesSize;

    if (detectEncoding_ && byteBuffer.size() >= 2)
    {
        DetectEncoding(readByteCount);
        detectEncoding_ = false;
    }

    std::optional<int> convertedCharCount = encoding_->GetChars(ReadOnlySpan(&byteBuffer[bytePos_], readByteCount - bytePos_), Span(GetCharBuffer()));
    if (!convertedCharCount && !bytesUnknown_.empty())
    {
        // The stream read too short data, which causes fallback.
        // So we should read buffer again.
        return ReadBuffer();
    }

    return charLen_ = *convertedCharCount;
}

bool StreamReader::DetectPreamble(const Encoding& encoding, int32_t readByteCount)
{
    if (readByteCount < encoding.Preamble().Length())
    {
        return false;
    }

    ReadOnlySpan<std::byte> byteBuffer = GetByteBuffer();
    for (int32_t i = 0; i < encoding.Preamble().Length(); ++i)
    {
        if (byteBuffer[i] != encoding.Preamble()[i])
        {
            return false;
        }
    }

    return true;
}

void StreamReader::DetectEncoding(int32_t readByteCount)
{
    std::vector<std::byte>& byteBuffer = GetByteBuffer();

    // If the default reader's encoding is different from detected encoding, then
    // refresh charBuffer_ and encoding_, etc.
    const Encoding& newEncoding = GetCorrespondingEncoding(byteBuffer);
    if (newEncoding.CodePage() != encoding_->CodePage())
    {
        auto newMaxCharsPerBuffer = newEncoding.GetMaxCharCount(static_cast<int32_t>(byteBuffer.size()));
        charBuffer_.resize(static_cast<size_t>(newMaxCharsPerBuffer) * 2);
        encoding_ = CloneEncoding(newEncoding);
    }

    // Remove the byte order mark.
    if (DetectPreamble(newEncoding, readByteCount))
    {
        CompressBuffer(newEncoding.Preamble().Length());
    }
}

const Encoding& StreamReader::GetCorrespondingEncoding(ReadOnlySpan<std::byte> byteBuffer)
{
    if (byteBuffer.Length() < 2)
    {
        return Encoding::UTF8();
    }

    if (byteBuffer[0] == std::byte(0xff) && byteBuffer[1] == std::byte(0xfe))
    {
        return Encoding::Unicode();
    }
    else if (byteBuffer[0] == std::byte(0xfe) && byteBuffer[1] == std::byte(0xff))
    {
        return Encoding::BigEndianUnicode();
    }

    if (byteBuffer.Length() < 4)
    {
        return Encoding::UTF8();
    }

    if (byteBuffer[0] == std::byte(0xff) && byteBuffer[1] == std::byte(0xfe) && byteBuffer[2] == std::byte(0x0) &&
        byteBuffer[3] == std::byte(0x0))
    {
        return Encoding::UTF32();
    }
    else if (byteBuffer[0] == std::byte(0x0) && byteBuffer[1] == std::byte(0x0) && byteBuffer[2] == std::byte(0xfe) &&
        byteBuffer[3] == std::byte(0xff))
    {
        return Encoding::BigEndianUnicode32();
    }

    return Encoding::UTF8();
}

void StreamReader::CompressBuffer(int32_t offset)
{
    bytePos_ += offset;
}

std::vector<std::byte>& StreamReader::GetByteBuffer()
{
    if (byteBuffer_.empty())
    {
        byteBuffer_.resize(byteBufferSize_);
    }

    return byteBuffer_;
}

std::vector<char16_t>& StreamReader::GetCharBuffer()
{
    if (charBuffer_.empty())
    {
        charBuffer_.resize(size_t(encoding_->GetMaxCharCount(byteBufferSize_)));
    }

    return charBuffer_;
}

CS2CPP_NAMESPACE_END