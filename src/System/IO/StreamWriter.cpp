#include "System/IO/StreamWriter.h"
#include "System/IO/FileStream.h"

CS2CPP_NAMESPACE_BEGIN

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream) noexcept :
    StreamWriter(std::move(stream), Encoding::UTF8(), DefaultBufferSize, false, false)
{
}

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream, const Encoding& encoding) noexcept :
    StreamWriter(std::move(stream), encoding, DefaultBufferSize)
{
}

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream, const Encoding& encoding, int32_t bufferSize) noexcept :
    StreamWriter(std::move(stream), encoding, DefaultBufferSize, false)
{
}

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream, const Encoding& encoding, int32_t bufferSize, bool leaveOpen) noexcept :
    StreamWriter(std::move(stream), encoding, bufferSize, leaveOpen, true)
{
}

StreamWriter::StreamWriter(std::shared_ptr<Stream> stream, const Encoding& encoding, int32_t bufferSize, bool leaveOpen, bool haveWrittenPreamble) noexcept :
    _stream(std::move(stream)),
    _encoding(*Encoding::Create(encoding.GetCodePage())),
    _bufferSize(bufferSize),
    _charPos(0),
    _leaveOpen(leaveOpen),
    _haveWrittenPreamble(haveWrittenPreamble)
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
        return {};

    // Don't write preamble.
    return StreamWriter(std::move(fs), Encoding::UTF8(), DefaultBufferSize, false, true);
}

std::optional<StreamWriter> StreamWriter::Create(std::u16string_view path, bool append, const Encoding& encoding)
{
    return Create(path, append, encoding, DefaultBufferSize);
}

std::optional<StreamWriter> StreamWriter::Create(std::u16string_view path, bool append, const Encoding& encoding, int32_t bufferSize)
{
    auto fs = CreateFileStream(path, append);
    if (!fs)
        return {};

    bool haveWrittenPreamble = fs->CanSeek() && fs->Position() > 0;
    return StreamWriter(std::move(fs), encoding, bufferSize, false, haveWrittenPreamble);
}

const Encoding& StreamWriter::GetEncoding() const noexcept
{
    return _encoding;
}

std::shared_ptr<Stream> StreamWriter::GetBaseStream() const noexcept
{
    return _stream;
}

void StreamWriter::Write(char16_t value)
{
    auto& charBuffer = GetCharBuffer();
    if (_charPos == charBuffer.size())
        Flush();

    charBuffer[_charPos++] = value;
}

void StreamWriter::Flush()
{
    if (!_stream)
        return;

    if (!_haveWrittenPreamble)
    {
        _haveWrittenPreamble = true;
        if (!_encoding.GetPreamble().empty())
            _stream->Write(_encoding.GetPreamble());
    }

    auto& byteBuffer = GetByteBuffer();

    auto convertedBytes = Encoding::Convert(Encoding::Unicode(), _encoding,
        {reinterpret_cast<const std::byte*>(GetCharBuffer().data()), _charPos * sizeof(char16_t)}, byteBuffer);
    if (convertedBytes && *convertedBytes > 0)
        _stream->Write(byteBuffer.data(), *convertedBytes);

    _stream->Flush();
    _charPos = 0;
}

void StreamWriter::Close()
{
    if (!_stream || _leaveOpen)
        return;

    Flush();
    _stream->Close();
}

std::vector<std::byte>& StreamWriter::GetByteBuffer()
{
    if (_byteBuffer.empty())
    {
        auto maxBytesPerBuffer = _encoding.GetMaxByteCount(_bufferSize);
        _byteBuffer.resize(static_cast<size_t>(maxBytesPerBuffer));
    }

    return _byteBuffer;
}

std::vector<char16_t>& StreamWriter::GetCharBuffer()
{
    if (_charBuffer.empty())
        _charBuffer.resize(static_cast<size_t>(_bufferSize));

    return _charBuffer;
}

std::shared_ptr<Stream> StreamWriter::CreateFileStream(std::u16string_view path, bool append)
{
    return FileStream::Create(path, append ? FileMode::Append : FileMode::Create, FileAccess::Write,
        FileShare::Read, FileOptions::SequentialScan, DefaultFileStreamBufferSize);
}

CS2CPP_NAMESPACE_END