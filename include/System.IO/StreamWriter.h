#pragma once

#include "System.Text/Encoding.h"

#include "Stream.h"
#include "TextWriter.h"

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
    std::shared_ptr<class Encoding> GetEncoding() const noexcept override;
    void SetAutoFlush(bool autoFlush) noexcept;
    bool GetAutoFlush() const noexcept;
    std::shared_ptr<Stream> GetBaseStream() const noexcept;

private:
    void WriteCore(std::u16string_view value, bool appendNewLine) override;
    std::vector<char16_t>& GetCharBuffer();
    std::vector<std::byte>& GetByteBuffer();
    static std::shared_ptr<Stream> CreateFileStream(std::u16string_view path, bool append);

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

    static constexpr int32_t DefaultBufferSize = 1024;
    static constexpr int32_t DefaultFileStreamBufferSize = 4096;
};

CS2CPP_NAMESPACE_END
