#include "System/Console.h"
#include "System.IO/StreamReader.h"
#include "System.IO/StreamWriter.h"
#include "System.Text/OSEncoding.h"

CS2CPP_NAMESPACE_BEGIN

std::shared_ptr<TextReader> Console::In()
{
    auto in = std::atomic_load(&in_);
    if (!in)
    {
        in = CreateReader(OpenStandardInput());
        std::atomic_store(&in_, in);
    }

    return in;
}

std::shared_ptr<TextWriter> Console::Out()
{
    auto out = std::atomic_load(&out_);
    if (!out)
    {
        out = CreateWriter(OpenStandardOutput());
        std::atomic_store(&out_, out);
    }

    return out;
}

std::shared_ptr<TextWriter> Console::Error()
{
    auto error = std::atomic_load(&error_);
    if (!error)
    {
        error = CreateWriter(OpenStandardError());
        std::atomic_store(&error_, error);
    }

    return error;
}

void Console::SetInputEncoding(std::shared_ptr<Encoding> encoding)
{
    std::atomic_store(&inputEncoding_, std::move(encoding));
}

void Console::SetOutputEncoding(std::shared_ptr<Encoding> encoding)
{
    std::atomic_store(&outputEncoding_, std::move(encoding));
}

bool Console::SetWindowLeft(int32_t left)
{
    return SetWindowPosition(left, GetWindowTop());
}

bool Console::SetWindowTop(int32_t top)
{
    return SetWindowPosition(GetWindowLeft(), top);
}

bool Console::SetWindowWidth(int32_t width)
{
    return SetWindowSize(width, GetWindowHeight());
}

bool Console::SetWindowHeight(int32_t height)
{
    return SetWindowSize(GetWindowWidth(), height);
}

std::shared_ptr<Encoding> Console::GetInputEncoding()
{
    auto inputEncoding = std::atomic_load(&inputEncoding_);
    if (!inputEncoding)
    {
        auto codePage = GetDefaultConsoleInputCP();
        inputEncoding = Encoding::GetEncoding(codePage);
        if (!inputEncoding)
        {
            inputEncoding = std::make_shared<OSEncoding>(codePage);
        }

        std::atomic_store(&inputEncoding_, inputEncoding);
    }

    return inputEncoding;
}

std::shared_ptr<Encoding> Console::GetOutputEncoding()
{
    auto outputEncoding = std::atomic_load(&outputEncoding_);
    if (!outputEncoding)
    {
        auto codePage = GetDefaultConsoleOutputCP();
        outputEncoding = Encoding::GetEncoding(codePage);
        if (!outputEncoding)
        {
            outputEncoding = std::make_shared<OSEncoding>(codePage);
        }

        std::atomic_store(&outputEncoding_, outputEncoding);
    }

    return outputEncoding;
}

void Console::Beep()
{
    Beep(800, 200);
}

void Console::SetIn(std::shared_ptr<TextReader> newIn)
{
    std::atomic_store(&in_, std::move(newIn));
}

void Console::SetOut(std::shared_ptr<TextWriter> newOut)
{
    std::atomic_store(&out_, std::move(newOut));
}

void Console::SetError(std::shared_ptr<TextWriter> newError)
{
    std::atomic_store(&error_, std::move(newError));
}

std::u16string Console::ReadLine()
{
    return In()->ReadLine().value_or(std::u16string());
}

void Console::Write(std::u16string_view value)
{
    Out()->Write(value);
}

void Console::Write(const char16_t* value)
{
    Out()->Write(value);
}

void Console::Write(bool value)
{
    Out()->Write(value);
}

void Console::WriteLine()
{
    Out()->WriteLine();
}

void Console::WriteLine(std::u16string_view value)
{
    Out()->WriteLine(value);
}

void Console::WriteLine(const char16_t* value)
{
    Out()->WriteLine(value);
}

void Console::WriteLine(bool value)
{
    Out()->WriteLine(value);
}

std::shared_ptr<TextReader> Console::CreateReader(std::shared_ptr<Stream> stream)
{
    if (!stream)
    {
        return nullptr;
    }

    return std::make_shared<StreamReader>(std::move(stream), *GetInputEncoding(), 4096, true);
}

std::shared_ptr<TextWriter> Console::CreateWriter(std::shared_ptr<Stream> stream)
{
    if (!stream)
    {
        return nullptr;
    }

    auto writer = std::make_shared<StreamWriter>(std::move(stream), *GetOutputEncoding(), 4096, true);
    writer->SetAutoFlush(true);

    return writer;
}

CS2CPP_NAMESPACE_END
