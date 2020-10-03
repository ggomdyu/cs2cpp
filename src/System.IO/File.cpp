#include "System/TimeZoneInfo.h"
#include "System.IO/File.h"

CS2CPP_NAMESPACE_BEGIN

namespace
{

DateTime GetUtcDateTime(DateTime dateTime)
{
    if (dateTime.Kind() == DateTimeKind::Unspecified)
    {
        return DateTime::SpecifyKind(dateTime, DateTimeKind::Utc);
    }

    return dateTime.ToUniversalTime();
}

}

bool File::AppendAllLines(std::u16string_view path, ReadOnlySpan<std::u16string_view> contents)
{
    auto sw = StreamWriter::Create(path, true);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::AppendAllLines(std::u16string_view path, ReadOnlySpan<std::u16string_view> contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, true, encoding);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::AppendAllText(std::u16string_view path, std::u16string_view contents)
{
    auto sw = StreamWriter::Create(path, true);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllText(*sw, contents);
    return true;
}

bool File::AppendAllText(std::u16string_view path, std::u16string_view contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, true, encoding);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllText(*sw, contents);
    return true;
}

std::optional<StreamWriter> File::AppendText(std::u16string_view path)
{
    return StreamWriter::Create(path, true);
}

std::shared_ptr<FileStream> File::Create(std::u16string_view path)
{
    return FileStream::Create(path, FileMode::Create, FileAccess::ReadWrite, FileShare::None);
}

std::shared_ptr<FileStream> File::Create(std::u16string_view path, int32_t bufferSize)
{
    return FileStream::Create(path, FileMode::Create, FileAccess::ReadWrite, FileShare::None, bufferSize);
}

std::shared_ptr<FileStream> File::Create(std::u16string_view path, int32_t bufferSize, FileOptions options)
{
    return FileStream::Create(path, FileMode::Create, FileAccess::ReadWrite, FileShare::None, options, bufferSize);
}

std::optional<StreamWriter> File::CreateText(std::u16string_view path)
{
    return StreamWriter::Create(path, false);
}

bool File::SetCreationTimeUtc(std::u16string_view path, DateTime creationTimeUtc)
{
    return SetCreationTime(path, GetUtcDateTime(creationTimeUtc));
}

bool File::SetLastAccessTimeUtc(std::u16string_view path, DateTime lastAccessTimeUtc)
{
    return SetLastAccessTime(path, GetUtcDateTime(lastAccessTimeUtc));
}

bool File::SetLastWriteTimeUtc(std::u16string_view path, DateTime lastWriteTimeUtc)
{
    return SetLastWriteTime(path, GetUtcDateTime(lastWriteTimeUtc));
}

std::optional<DateTime> File::GetCreationTime(std::u16string_view path)
{
    auto ret = GetCreationTimeUtc(path);
    if (!ret)
    {
        return std::nullopt;
    }

    return *ret + TimeZoneInfo::Local().BaseUtcOffset();
}

std::optional<DateTime> File::GetLastAccessTime(std::u16string_view path)
{
    auto ret = GetLastAccessTimeUtc(path);
    if (!ret)
    {
        return std::nullopt;
    }

    return *ret + TimeZoneInfo::Local().BaseUtcOffset();
}

std::optional<DateTime> File::GetLastWriteTime(std::u16string_view path)
{
    auto ret = GetLastWriteTimeUtc(path);
    if (!ret)
    {
        return std::nullopt;
    }

    return *ret + TimeZoneInfo::Local().BaseUtcOffset();
}

std::shared_ptr<FileStream> File::Open(std::u16string_view path, FileMode mode)
{
    return Open(path, mode, mode == FileMode::Append ? FileAccess::Write : FileAccess::ReadWrite, FileShare::None);
}

std::shared_ptr<FileStream> File::Open(std::u16string_view path, FileMode mode, FileAccess access)
{
    return Open(path, mode, access, FileShare::None);
}

std::shared_ptr<FileStream> File::Open(std::u16string_view path, FileMode mode, FileAccess access, FileShare share)
{
    return FileStream::Create(path, mode, access, share);
}

std::shared_ptr<FileStream> File::OpenRead(std::u16string_view path)
{
    return FileStream::Create(path, FileMode::Open, FileAccess::Read, FileShare::Read);
}

std::optional<StreamReader> File::OpenText(std::u16string_view path)
{
    return StreamReader::Create(path);
}

std::shared_ptr<FileStream> File::OpenWrite(std::u16string_view path)
{
    return FileStream::Create(path, FileMode::OpenOrCreate, FileAccess::Write, FileShare::None);
}

std::optional<std::vector<std::byte>> File::ReadAllBytes(std::u16string_view path)
{
    auto fs = FileStream::Create(path, FileMode::Open, FileAccess::Read, FileShare::Read, 512);
    if (!fs)
    {
        return std::nullopt;
    }

    std::vector<std::byte> ret(static_cast<size_t>(fs->Length()));
    fs->Read(ret);

    return {std::move(ret)};
}

std::optional<std::vector<std::u16string>> File::ReadAllLines(std::u16string_view path)
{
    return ReadAllLines(path, Encoding::UTF8());
}

std::optional<std::vector<std::u16string>> File::ReadAllLines(std::u16string_view path, const Encoding& encoding)
{
    std::vector<std::u16string> lines;
    if (!ReadLines(path, encoding, [&](std::u16string&& line) { lines.push_back(std::move(line)); }))
    {
        return std::nullopt;
    }

    return {std::move(lines)};
}

std::optional<std::u16string> File::ReadAllText(std::u16string_view path)
{
    auto reader = StreamReader::Create(path);
    if (!reader)
    {
        return std::nullopt;
    }

    return reader->ReadToEnd();
}

std::optional<std::u16string> File::ReadAllText(std::u16string_view path, const Encoding& encoding)
{
    auto bytes = ReadAllBytes(path);
    if (!bytes)
    {
        return std::nullopt;
    }

    return encoding.GetString(RemoveByteOrderMark(*bytes, encoding));
}

bool File::WriteAllBytes(std::u16string_view path, ReadOnlySpan<std::byte> bytes)
{
    auto fs = FileStream::Create(path, FileMode::Create, FileAccess::Write, FileShare::Read);
    if (!fs)
    {
        return false;
    }

    fs->Write(bytes);
    fs->Close();
    return true;
}

bool File::WriteAllLines(std::u16string_view path, ReadOnlySpan<std::u16string_view> contents)
{
    auto sw = StreamWriter::Create(path);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::WriteAllLines(std::u16string_view path, ReadOnlySpan<std::u16string_view> contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, false, encoding);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::WriteAllText(std::u16string_view path, std::u16string_view contents)
{
    auto sw = StreamWriter::Create(path);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllText(*sw, contents);
    return true;
}

bool File::WriteAllText(std::u16string_view path, std::u16string_view contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, false, encoding);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllText(*sw, contents);
    return true;
}

void File::InternalWriteAllLines(StreamWriter& writer, ReadOnlySpan<std::u16string_view> contents)
{
    for (decltype(auto) content : contents)
    {
        writer.WriteLine(content);
    }

    writer.Close();
}

void File::InternalWriteAllText(StreamWriter& writer, std::u16string_view contents)
{
    writer.Write(contents);
    writer.Close();
}

ReadOnlySpan<std::byte> File::RemoveByteOrderMark(ReadOnlySpan<std::byte> bytes, const Encoding& encoding) noexcept
{
    auto preamble = encoding.Preamble();
    if (preamble.Length() > bytes.Length())
    {
        return bytes;
    }

    int32_t j = 0;
    for (int32_t i = 0; i < preamble.Length(); ++i)
    {
        if (preamble[i] != std::byte(bytes[i]))
        {
            break;
        }

        ++j;
    }

    return bytes.Slice(j);
}

CS2CPP_NAMESPACE_END
