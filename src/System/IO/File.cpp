#include <cstdio>

#include "System/IO/File.h"
#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

bool File::SetCreationTime(std::u16string_view path, DateTime creationTime)
{
    return SetCreationTimeUtc(path, creationTime);
}

bool File::SetLastAccessTime(std::u16string_view path, DateTime lastAccessTime)
{
    return SetLastAccessTimeUtc(path, lastAccessTime);
}

bool File::SetLastWriteTime(std::u16string_view path, DateTime lastWriteTime)
{
    return SetLastWriteTimeUtc(path, lastWriteTime);
}

std::optional<DateTime> File::GetCreationTime(std::u16string_view path)
{
    auto ret = GetCreationTimeUtc(path);
    if (!ret)
    {
        return {};
    }

    return *ret + TimeZoneInfo::Local().GetBaseUtcOffset();
}

std::optional<DateTime> File::GetLastAccessTime(std::u16string_view path)
{
    auto ret = GetLastAccessTimeUtc(path);
    if (!ret)
    {
        return {};
    }

    return *ret + TimeZoneInfo::Local().GetBaseUtcOffset();
}

std::optional<DateTime> File::GetLastWriteTime(std::u16string_view path)
{
    auto ret = GetLastWriteTimeUtc(path);
    if (!ret)
    {
        return {};
    }

    return *ret + TimeZoneInfo::Local().GetBaseUtcOffset();
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

std::shared_ptr<FileStream> File::OpenWrite(std::u16string_view path)
{
    return FileStream::Create(path, FileMode::OpenOrCreate, FileAccess::Write, FileShare::None);
}

std::optional<StreamReader> File::OpenText(std::u16string_view path)
{
    return StreamReader::Create(path);
}

std::optional<StreamWriter> File::CreateText(std::u16string_view path)
{
    return StreamWriter::Create(path, false);
}

std::optional<StreamWriter> File::AppendText(std::u16string_view path)
{
    return StreamWriter::Create(path, true);
}

std::optional<String> File::ReadAllText(std::u16string_view path)
{
    auto fileData = InternalReadAllText(path);
    const auto& encoding = DetectEncoding(fileData);

    return encoding.GetString(RemoveByteOrderMark(fileData, encoding));
}

std::optional<String> File::ReadAllText(std::u16string_view path, const Encoding& encoding)
{
    auto fileData = InternalReadAllText(path);
    return encoding.GetString(RemoveByteOrderMark(fileData, encoding));
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

std::unique_ptr<std::byte[]> File::ReadAllBytes(std::u16string_view path, ReturnPointerTag)
{
    SafeFilePointer fp = InternalFileOpen(path, u"rb");
    if (fp == nullptr)
    {
        return nullptr;
    }

    fseek(fp, 0, SEEK_END);
    const auto fileSize = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    auto fileData = std::make_unique<std::byte[]>(static_cast<size_t>(fileSize));
#ifdef _MSC_VER
    fread_s(&fileData[0], fileSize, 1, fileSize, fp);
#else
    fread(&fileData[0], 1, fileSize, fp);
#endif

    return fileData;
}

std::optional<std::vector<std::byte>> File::ReadAllBytes(std::u16string_view path, ReturnVectorTag)
{
    SafeFilePointer fp = InternalFileOpen(path, u"rb");
    if (fp == nullptr)
    {
        return {};
    }

    fseek(fp, 0, SEEK_END);
    const auto fileSize = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    std::vector<std::byte> fileData(fileSize);
#ifdef _MSC_VER
    fread_s(&fileData[0], fileSize, 1, fileSize, fp);
#else
    fread(&fileData[0], 1, fileSize, fp);
#endif

    return fileData;
}

bool File::WriteAllBytes(std::u16string_view path, gsl::span<const std::byte> bytes)
{
    auto fs = FileStream::Create(path, FileMode::Create, FileAccess::Write, FileShare::Read);
    if (fs == nullptr)
    {
        return false;
    }

    fs->Write(bytes);
    return true;
}

std::optional<std::vector<String>> File::ReadAllLines(std::u16string_view path)
{
    return ReadAllLines(path, Encoding::UTF8());
}

std::optional<std::vector<String>> File::ReadAllLines(std::u16string_view path, const Encoding& encoding)
{
    std::vector<String> lines;
    auto pred = [&](String&& line)
    {
        lines.push_back(std::move(line));
    };

    if (!ReadLines(path, encoding, pred))
    {
        return {};
    }

    return std::move(lines);
}

bool File::WriteAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents)
{
    auto sw = StreamWriter::Create(path);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::WriteAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, false, encoding);
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

bool File::AppendAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents)
{
    auto sw = StreamWriter::Create(path, true);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::AppendAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, true, encoding);
    if (!sw)
    {
        return false;
    }

    InternalWriteAllLines(*sw, contents);
    return true;
}

std::vector<std::byte> File::InternalReadAllText(std::u16string_view path)
{
    SafeFilePointer fp = InternalFileOpen(path, u"r");
    if (fp == nullptr)
    {
        return {};
    }

    fseek(fp, 0, SEEK_END);
    const auto fileSize = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    std::vector<std::byte> fileData(fileSize);
#ifdef _MSC_VER
    fread_s(&fileData[0], fileSize, 1, fileSize, fp);
#else
    fread(&fileData[0], 1, fileSize, fp);
#endif

    return fileData;
}

void File::InternalWriteAllText(StreamWriter& writer, std::u16string_view contents)
{
    writer.Write(contents);
    writer.Close();
}

void File::InternalWriteAllLines(StreamWriter& writer, gsl::span<std::u16string_view> contents)
{
    for (decltype(auto) content : contents)
    {
        writer.WriteLine(content);
    }

    writer.Close();
}

const Encoding& File::DetectEncoding(gsl::span<const std::byte> bytes) noexcept
{
    if (bytes.data() == nullptr || bytes.size() < 2)
    {
        return Encoding::UTF8();
    }

    if (bytes[0] == std::byte(0xff) && bytes[1] == std::byte(0xFE))
    {
        return bytes.size() < 4 || bytes[2] != std::byte(0) || bytes[3] != std::byte(0) ?
            Encoding::Unicode() :
            Encoding::UTF32();
    }

    if (bytes[0] == std::byte(0xfe) && bytes[1] == std::byte(0xff))
    {
        return Encoding::BigEndianUnicode();
    }

    if (bytes.size() >= 4 && bytes[0] == std::byte(0) && bytes[1] == std::byte(0) && bytes[2] == std::byte(0xfe) &&
        bytes[3] == std::byte(0xff))
    {
        return *Encoding::GetEncoding(u"UTF32BE");
    }

    return Encoding::UTF8();
}

gsl::span<const std::byte> File::RemoveByteOrderMark(gsl::span<const std::byte> bytes, const Encoding& encoding) noexcept
{
    const auto preamble = encoding.GetPreamble();
    if (preamble.size() > bytes.size())
    {
        return bytes;
    }

    size_t startIndex = 0;
    for (size_t i = 0; i < preamble.size(); ++i)
    {
        if (preamble[i] != std::byte(bytes[i]))
        {
            break;
        }

        ++startIndex;
    }

    return gsl::span(bytes).subspan(startIndex);
}

CS2CPP_NAMESPACE_END