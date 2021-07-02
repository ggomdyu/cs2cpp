#include <cassert>

#include "System/IO/StreamReader.h"
#include "System/IO/FileStream.h"

CS2CPP_NAMESPACE_BEGIN

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
    _stream(std::move(stream)),
    _fallbackBuffer(std::make_unique<std::vector<std::byte>>()),
    _byteBufferSize(bufferSize),
    _encoding(CloneEncoding(encoding)),
    _detectEncoding(detectEncodingFromByteOrderMarks),
    _leaveOpen(leaveOpen),
    _charPos(0),
    _charLen(0),
    _bytePos(0)
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
        return {};

    auto fileStream = FileStream::Create(path, FileMode::Open, FileAccess::Read, FileShare::Read, FileOptions::SequentialScan,
        DefaultFileStreamBufferSize);
    if (!fileStream)
        return {};

    return StreamReader(std::move(fileStream), encoding, detectEncodingFromByteOrderMarks, bufferSize);
}

const Encoding& StreamReader::GetCurrentEncoding() const noexcept
{
    return _encoding;
}

std::shared_ptr<Stream> StreamReader::GetBaseStream() const noexcept
{
    return _stream;
}

bool StreamReader::IsEndOfStream() const noexcept
{
    return _stream->Position() == _stream->Length();
}

void StreamReader::Close()
{
    if (!_leaveOpen)
        _stream->Close();
}

int32_t StreamReader::Peek()
{
    if (_charPos == _charLen)
    {
        if (ReadBuffer() == 0)
            return -1;
    }

    return GetCharBuffer()[_charPos];
}

int32_t StreamReader::Read()
{
    if (_charPos == _charLen)
    {
        if (ReadBuffer() == 0)
            return -1;
    }

    return GetCharBuffer()[_charPos++];
}

Encoding StreamReader::CloneEncoding(const Encoding& encoding) const
{
    auto newEncoding = Encoding::Create(encoding.GetCodePage());

    newEncoding->SetDecoderFallback({
        [](const void* context, [[maybe_unused]] UConverterToUnicodeArgs* toArgs, const char* codeUnits,
            int32_t length, UConverterCallbackReason reason, UErrorCode* err) {
            if (reason > UCNV_IRREGULAR)
                return;

            auto buffer = const_cast<std::vector<std::byte>*>(static_cast<const std::vector<std::byte>*>(context));
            for (int32_t i = 0; i < length; ++i)
                buffer->push_back(static_cast<std::byte>(codeUnits[i]));

            *err = U_ZERO_ERROR;
        }, _fallbackBuffer.get()}
    );

    return std::move(*newEncoding);
}

int32_t StreamReader::ReadBuffer()
{
    _charPos = 0;
    _charLen = 0;
    _bytePos = 0;

    decltype(auto) byteBuffer = GetByteBuffer();

    // If character bytes are overlapped at the boundary of byteBuffer, it will occur fallback while decoding.
    // Therefore, when the stream is read again, we need to fill byteBuffer with _fallbackBuffer.
    auto fallbackBufferSize = static_cast<int32_t>(_fallbackBuffer->size());
    if (!_fallbackBuffer->empty())
    {
        assert(byteBuffer.size() > _fallbackBuffer->size());
        for (size_t i = 0; i < _fallbackBuffer->size(); ++i)
            byteBuffer[i] = (*_fallbackBuffer)[i];

        _fallbackBuffer->clear();
    }

    // If the stream indicates EOF, then escape the function.
    int32_t readByteCount;
    if ((readByteCount = _stream->Read(&byteBuffer[fallbackBufferSize],
        static_cast<int32_t>(byteBuffer.size()) - fallbackBufferSize)) == 0)
        return 0;

    readByteCount += fallbackBufferSize;

    if (_detectEncoding && byteBuffer.size() >= 2)
    {
        DetectEncoding(readByteCount);
        _detectEncoding = false;
    }

    decltype(auto) charBuffer = GetCharBuffer();

    auto convertedBytes = Encoding::Convert(_encoding, Encoding::Unicode(),
        &byteBuffer[_bytePos], readByteCount - _bytePos, reinterpret_cast<std::byte*>(charBuffer.data()),
        static_cast<int32_t>(charBuffer.size() * sizeof(charBuffer[0])));
    if (!convertedBytes && !_fallbackBuffer->empty())
    {
        // The stream read too short data, which causes fallback.
        // So we should read buffer again.
        return ReadBuffer();
    }

    _charLen = *convertedBytes / sizeof(char16_t);

    return _charLen;
}

bool StreamReader::DetectPreamble(const Encoding& encoding, int32_t readByteCount)
{
    if (static_cast<size_t>(readByteCount) < encoding.GetPreamble().size())
        return false;

    decltype(auto) byteBuffer = GetByteBuffer();
    for (size_t i = 0; i < encoding.GetPreamble().size(); ++i)
    {
        if (byteBuffer[i] != encoding.GetPreamble()[i])
            return false;
    }

    return true;
}

void StreamReader::DetectEncoding(int32_t readByteCount)
{
    decltype(auto) byteBuffer = GetByteBuffer();

    decltype(auto) newEncoding = &Encoding::UTF8();
    if (byteBuffer[0] == std::byte(0xff) && byteBuffer[1] == std::byte(0xFE))
        newEncoding = byteBuffer.size() < 4 || byteBuffer[2] != std::byte(0) || byteBuffer[3] != std::byte(0) ?
            &Encoding::Unicode() : &Encoding::UTF32();
    else if (byteBuffer[0] == std::byte(0xfe) && byteBuffer[1] == std::byte(0xff))
        newEncoding = &Encoding::BigEndianUnicode();
    else if (byteBuffer.size() >= 4 && byteBuffer[0] == std::byte(0) && byteBuffer[1] == std::byte(0) &&
        byteBuffer[2] == std::byte(0xfe) && byteBuffer[3] == std::byte(0xff))
        newEncoding = Encoding::GetEncoding(u"UTF32BE");

    // If the default reader's encoding is different from detected encoding, then
    // refresh _charBuffer and _encoding, etc.
    if (newEncoding->GetCodePage() != _encoding.GetCodePage())
    {
        auto newMaxCharsPerBuffer = newEncoding->GetMaxCharCount(static_cast<int32_t>(byteBuffer.size()));
        _charBuffer.resize(static_cast<size_t>(newMaxCharsPerBuffer) * 2);
        _encoding = CloneEncoding(*newEncoding);
    }

    // Remove the byte order mark.
    if (DetectPreamble(*newEncoding, readByteCount))
        CompressBuffer(static_cast<int32_t>(newEncoding->GetPreamble().size()));
}

void StreamReader::CompressBuffer(int32_t offset)
{
    _bytePos += offset;
}

std::vector<std::byte>& StreamReader::GetByteBuffer()
{
    if (_byteBuffer.empty())
        _byteBuffer.resize(_byteBufferSize);

    return _byteBuffer;
}

std::vector<char16_t>& StreamReader::GetCharBuffer()
{
    if (_charBuffer.empty())
    {
        auto maxCharsPerBuffer = _encoding.GetMaxCharCount(_byteBufferSize);
        _charBuffer.resize(static_cast<size_t>(maxCharsPerBuffer));
    }

    return _charBuffer;
}

CS2CPP_NAMESPACE_END