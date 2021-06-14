#pragma once

#include "System/Text/Encoding.h"

#include "Stream.h"
#include "TextReader.h"

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
    [[nodiscard]] static std::optional<StreamReader> Create(std::u16string_view path);
    [[nodiscard]] static std::optional<StreamReader> Create(std::u16string_view path, bool detectEncodingFromByteOrderMarks);
    [[nodiscard]] static std::optional<StreamReader> Create(std::u16string_view path, const Encoding& encoding);
    [[nodiscard]] static std::optional<StreamReader> Create(std::u16string_view path, const Encoding& encoding, bool detectEncodingFromByteOrderMarks);
    [[nodiscard]] static std::optional<StreamReader> Create(std::u16string_view path, const Encoding& encoding, bool detectEncodingFromByteOrderMarks, int32_t bufferSize);
    [[nodiscard]] const Encoding& GetCurrentEncoding() const noexcept;
    [[nodiscard]] std::shared_ptr<Stream> GetBaseStream() const noexcept;
    [[nodiscard]] bool IsEndOfStream() const noexcept;
    void Close() override;
    int32_t Peek() override;
    int32_t Read() override;

private:
    [[nodiscard]] Encoding CloneEncoding(const Encoding& encoding) const;
    int32_t ReadBuffer();
    bool DetectPreamble(const Encoding& encoding, int32_t readByteCount);
    void CompressBuffer(int32_t offset);
    void DetectEncoding(int32_t readByteCount);
    std::vector<std::byte>& GetByteBuffer();
    std::vector<char16_t>& GetCharBuffer();

private:
    std::shared_ptr<Stream> _stream;
    std::vector<std::byte> _byteBuffer;
    std::vector<char16_t> _charBuffer;
    std::unique_ptr<std::vector<std::byte>> _fallbackBuffer;
    int32_t _byteBufferSize;
    Encoding _encoding;
    bool _detectEncoding;
    bool _leaveOpen;
    int32_t _charPos;
    int32_t _charLen;
    int32_t _bytePos;

    static constexpr int32_t DefaultBufferSize = 1024;
    static constexpr int32_t DefaultFileStreamBufferSize = 4096;
};

CS2CPP_NAMESPACE_END
