#include "System/Environment.h"
#include "System/IO/TextWriter.h"

CS2CPP_NAMESPACE_BEGIN

void TextWriter::Write(std::u16string_view value)
{
    for (auto c : value)
        Write(c);
}

void TextWriter::WriteLine(char16_t value)
{
    Write(value);
    WriteLine();
}

void TextWriter::WriteLine(std::u16string_view value)
{
    Write(value);
    WriteLine();
}

std::u16string_view TextWriter::GetNewLine() const noexcept
{
    return Environment::GetNewLine();
}

void TextWriter::WriteLine()
{
    Write(GetNewLine());
}

CS2CPP_NAMESPACE_END