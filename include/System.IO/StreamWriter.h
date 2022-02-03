#pragma once

#include "System.IO/Stream.h"
#include "System.IO/TextWriter.h"
#include "System.Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

class StreamWriter final :
    public TextWriter
{
public:
    explicit StreamWriter(std::shared_ptr<Stream> stream) noexcept;
    StreamWriter(std::shared_ptr<Stream> stream, const class Encoding& encoding) noexcept;
    StreamWriter(std::shared_ptr<Stream> stream, const class Encoding& encoding, int32_t bufferSize) noexcept;
    StreamWriter(std::shared_ptr<Stream> stream, const class Encoding& encoding, int32_t bufferSize, bool leaveOpen) noexcept;
    StreamWriter(const StreamWriter& rhs) = delete;
    StreamWriter(StreamWriter&& rhs) noexcept = default;
    ~StreamWriter() override = default;
    
private:
    StreamWriter(std::shared_ptr<Stream> stream, const class Encoding& encoding, int32_t bufferSize, bool leaveOpen, bool haveWrittenPreamble) noexcept;

public:
    StreamWriter& operator=(const StreamWriter& rhs) = delete;
    StreamWriter& operator=(StreamWriter&& rhs) noexcept = default;

public:
    using TextWriter::Write;
    using TextWriter::WriteLine;
    static std::optional<StreamWriter> Create(std::u16string_view path);
    static std::optional<StreamWriter> Create(std::u16string_view path, bool append);
    static std::optional<StreamWriter> Create(std::u16string_view path, bool append, const class Encoding& encoding);
    static std::optional<StreamWriter> Create(std::u16string_view path, bool append, const class Encoding& encoding, int32_t bufferSize);
    void Write(std::u16string_view value) override;
    void WriteLine(std::u16string_view value) override;
    void Flush() override;
    void Close() override;
    std::shared_ptr<class Encoding> Encoding() const noexcept override;
    void SetAutoFlush(bool autoFlush) noexcept;
    bool GetAutoFlush() const noexcept;
    std::shared_ptr<Stream> GetBaseStream() const noexcept;

private:
    void InternalWrite(std::u16string_view value, bool appendNewLine) override;
    Span<char16_t> GetCharBuffer();
    Span<std::byte> GetByteBuffer();

private:
    std::shared_ptr<Stream> stream_;
    std::shared_ptr<class Encoding> encoding_;
    std::vector<char16_t> charBuffer_;
    std::vector<std::byte> byteBuffer_;
    int32_t bufferSize_;
    int32_t charPos_;
    bool leaveOpen_;
    bool haveWrittenPreamble_;
    bool autoFlush_;
};

CS2CPP_NAMESPACE_END
