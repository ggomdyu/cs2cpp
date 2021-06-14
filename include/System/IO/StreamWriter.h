#pragma once

#include "System/Text/Encoding.h"

#include "Stream.h"
#include "TextWriter.h"

CS2CPP_NAMESPACE_BEGIN

class StreamWriter final :
    public TextWriter
{
public:
    explicit StreamWriter(std::shared_ptr<Stream> stream) noexcept;
    StreamWriter(std::shared_ptr<Stream> stream, const Encoding& encoding) noexcept;
    StreamWriter(std::shared_ptr<Stream> stream, const Encoding& encoding, int32_t bufferSize) noexcept;
    StreamWriter(std::shared_ptr<Stream> stream, const Encoding& encoding, int32_t bufferSize, bool leaveOpen) noexcept;
    StreamWriter(const StreamWriter& rhs) = delete;
    StreamWriter(StreamWriter&& rhs) noexcept = default;
    ~StreamWriter() override = default;
    
private:
    StreamWriter(std::shared_ptr<Stream> stream, const Encoding& encoding, int32_t bufferSize, bool leaveOpen, bool haveWrittenPreamble) noexcept;

public:
    StreamWriter& operator=(const StreamWriter& rhs) = delete;
    StreamWriter& operator=(StreamWriter&& rhs) noexcept = default;

public:
    using TextWriter::Write;
    [[nodiscard]] static std::optional<StreamWriter> Create(std::u16string_view path);
    [[nodiscard]] static std::optional<StreamWriter> Create(std::u16string_view path, bool append);
    [[nodiscard]] static std::optional<StreamWriter> Create(std::u16string_view path, bool append, const Encoding& encoding);
    [[nodiscard]] static std::optional<StreamWriter> Create(std::u16string_view path, bool append, const Encoding& encoding, int32_t bufferSize);
    [[nodiscard]] const Encoding& GetEncoding() const noexcept override;
    [[nodiscard]] std::shared_ptr<Stream> GetBaseStream() const noexcept;
    void Write(char16_t value) override;
    void Flush() override;
    void Close() override;

private:
    std::vector<char16_t>& GetCharBuffer();
    std::vector<std::byte>& GetByteBuffer();
    static std::shared_ptr<Stream> CreateFileStream(std::u16string_view path, bool append);

private:
    std::shared_ptr<Stream> _stream;
    Encoding _encoding;
    std::vector<char16_t> _charBuffer;
    std::vector<std::byte> _byteBuffer;
    int32_t _bufferSize;
    int32_t _charPos;
    bool _leaveOpen;
    bool _haveWrittenPreamble;

    static constexpr int32_t DefaultBufferSize = 1024;
    static constexpr int32_t DefaultFileStreamBufferSize = 4096;
};

CS2CPP_NAMESPACE_END
