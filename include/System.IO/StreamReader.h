#pragma once

#include "System.IO/Stream.h"
#include "System.IO/TextReader.h"
#include "System.Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

class StreamReader final :
    public TextReader
{
public:
    explicit StreamReader(std::shared_ptr<Stream> stream) noexcept;
    StreamReader(std::shared_ptr<Stream> stream, const Encoding& encoding) noexcept;
    StreamReader(std::shared_ptr<Stream> stream, const Encoding& encoding, bool detectEncodingFromByteOrderMarks) noexcept;
    StreamReader(std::shared_ptr<Stream> stream, const Encoding& encoding, bool detectEncodingFromByteOrderMarks, int32_t bufferSize) noexcept;
    StreamReader(std::shared_ptr<Stream> stream, const Encoding& encoding, bool detectEncodingFromByteOrderMarks, int32_t bufferSize, bool leaveOpen) noexcept;
    StreamReader(std::shared_ptr<Stream> stream, bool detectEncodingFromByteOrderMarks) noexcept;
    StreamReader(const StreamReader& rhs) = delete;
    StreamReader(StreamReader&& rhs) noexcept = default;
    ~StreamReader() override = default;

public:
    StreamReader& operator=(const StreamReader& rhs) = delete;
    StreamReader& operator=(StreamReader&& rhs) noexcept = default;

public:
    using TextReader::Read;
    static std::optional<StreamReader> Create(std::u16string_view path);
    static std::optional<StreamReader> Create(std::u16string_view path, bool detectEncodingFromByteOrderMarks);
    static std::optional<StreamReader> Create(std::u16string_view path, const Encoding& encoding);
    static std::optional<StreamReader> Create(std::u16string_view path, const Encoding& encoding, bool detectEncodingFromByteOrderMarks);
    static std::optional<StreamReader> Create(std::u16string_view path, const Encoding& encoding, bool detectEncodingFromByteOrderMarks, int32_t bufferSize);
    std::shared_ptr<Encoding> CurrentEncoding() const noexcept;
    std::shared_ptr<Stream> BaseStream() const noexcept;
    bool IsEndOfStream() const noexcept;
    int32_t Peek() override;
    void Close() override;
    int32_t Read() override;

private:
    std::shared_ptr<Encoding> CloneEncoding(const Encoding& encoding);
    int32_t ReadBuffer();
    bool DetectPreamble(const Encoding& encoding, int32_t readByteCount);
    void CompressBuffer(int32_t offset);
    void DetectEncoding(int32_t readByteCount);
    static const Encoding& GetCorrespondingEncoding(ReadOnlySpan<std::byte> byteBuffer);
    std::vector<std::byte>& GetByteBuffer();
    std::vector<char16_t>& GetCharBuffer();

private:
    std::shared_ptr<Stream> stream_;
    std::vector<std::byte> byteBuffer_;
    std::vector<char16_t> charBuffer_;
    std::vector<std::byte> bytesUnknown_;
    int32_t byteBufferSize_;
    std::shared_ptr<Encoding> encoding_;
    bool detectEncoding_;
    bool leaveOpen_;
    int32_t charPos_;
    int32_t charLen_;
    int32_t bytePos_;

    static constexpr int32_t DefaultBufferSize = 1024;
    static constexpr int32_t DefaultFileStreamBufferSize = 4096;
};

CS2CPP_NAMESPACE_END
