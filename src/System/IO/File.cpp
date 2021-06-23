#include "System/IO/File.h"
#include "System/TimeZoneInfo.h"

CS2CPP_NAMESPACE_BEGIN

bool File::AppendAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents)
{
    auto sw = StreamWriter::Create(path, true);
    if (!sw)
        return false;

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::AppendAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, true, encoding);
    if (!sw)
        return false;

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::AppendAllText(std::u16string_view path, std::u16string_view contents)
{
    auto sw = StreamWriter::Create(path, true);
    if (!sw)
        return false;

    InternalWriteAllText(*sw, contents);
    return true;
}

bool File::AppendAllText(std::u16string_view path, std::u16string_view contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, true, encoding);
    if (!sw)
        return false;

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
        return {};

    return *ret + TimeZoneInfo::Local().GetBaseUtcOffset();
}

std::optional<DateTime> File::GetLastAccessTime(std::u16string_view path)
{
    auto ret = GetLastAccessTimeUtc(path);
    if (!ret)
        return {};

    return *ret + TimeZoneInfo::Local().GetBaseUtcOffset();
}

std::optional<DateTime> File::GetLastWriteTime(std::u16string_view path)
{
    auto ret = GetLastWriteTimeUtc(path);
    if (!ret)
        return {};

    return *ret + TimeZoneInfo::Local().GetBaseUtcOffset();
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
    SafeFilePointer fp = InternalFileOpen(path, u"rb");
    if (!fp)
        return {};

    fseek(fp, 0, SEEK_END);
    auto fileSize = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    std::vector<std::byte> fileData(fileSize);
#ifdef _MSC_VER
    fread_s(&fileData[0], fileSize, 1, fileSize, fp);
#else
    fread(&fileData[0], 1, fileSize, fp);
#endif

    return fileData;
}

std::optional<std::vector<std::u16string>> File::ReadAllLines(std::u16string_view path)
{
    return ReadAllLines(path, Encoding::UTF8());
}

std::optional<std::vector<std::u16string>> File::ReadAllLines(std::u16string_view path, const Encoding& encoding)
{
    std::vector<std::u16string> lines;
    if (!ReadLines(path, encoding, [&](std::u16string&& line) { lines.push_back(std::move(line)); }))
        return {};

    return std::optional<std::vector<std::u16string>>(std::move(lines));
}

std::optional<std::u16string> File::ReadAllText(std::u16string_view path)
{
    auto fileData = InternalReadAllText(path);
    const auto& encoding = DetectEncoding(fileData);

    return encoding.GetString(RemoveByteOrderMark(fileData, encoding));
}

std::optional<std::u16string> File::ReadAllText(std::u16string_view path, const Encoding& encoding)
{
    auto fileData = InternalReadAllText(path);
    return encoding.GetString(RemoveByteOrderMark(fileData, encoding));
}

bool File::WriteAllBytes(std::u16string_view path, gsl::span<const std::byte> bytes)
{
    auto fs = FileStream::Create(path, FileMode::Create, FileAccess::Write, FileShare::Read);
    if (!fs)
        return false;

    fs->Write(bytes);
    fs->Close();
    return true;
}

bool File::WriteAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents)
{
    auto sw = StreamWriter::Create(path);
    if (!sw)
        return false;

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::WriteAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, false, encoding);
    if (!sw)
        return false;

    InternalWriteAllLines(*sw, contents);
    return true;
}

bool File::WriteAllText(std::u16string_view path, std::u16string_view contents)
{
    auto sw = StreamWriter::Create(path);
    if (!sw)
        return false;

    InternalWriteAllText(*sw, contents);
    return true;
}

bool File::WriteAllText(std::u16string_view path, std::u16string_view contents, const Encoding& encoding)
{
    auto sw = StreamWriter::Create(path, false, encoding);
    if (!sw)
        return false;

    InternalWriteAllText(*sw, contents);
    return true;
}

std::vector<std::byte> File::InternalReadAllText(std::u16string_view path)
{
    SafeFilePointer fp = InternalFileOpen(path, u"r");
    if (!fp)
        return {};

    fseek(fp, 0, SEEK_END);
    auto fileSize = static_cast<size_t>(ftell(fp));
    fseek(fp, 0, SEEK_SET);

    std::vector<std::byte> fileData(fileSize);
#ifdef _MSC_VER
    fread_s(&fileData[0], fileSize, 1, fileSize, fp);
#else
    fread(&fileData[0], 1, fileSize, fp);
#endif

    return fileData;
}

void File::InternalWriteAllLines(StreamWriter& writer, gsl::span<std::u16string_view> contents)
{
    for (decltype(auto) content : contents)
        writer.WriteLine(content);

    writer.Close();
}

void File::InternalWriteAllText(StreamWriter& writer, std::u16string_view contents)
{
    writer.Write(contents);
    writer.Close();
}

const Encoding& File::DetectEncoding(gsl::span<const std::byte> bytes) noexcept
{
    if (bytes.data() == nullptr || bytes.size() < 2)
        return Encoding::UTF8();

    if (bytes[0] == std::byte(0xff) && bytes[1] == std::byte(0xfe))
        return bytes.size() < 4 || bytes[2] != std::byte(0) || bytes[3] != std::byte(0) ?
            Encoding::Unicode() :
            Encoding::UTF32();

    if (bytes[0] == std::byte(0xfe) && bytes[1] == std::byte(0xff))
        return Encoding::BigEndianUnicode();

    if (bytes.size() >= 4 && bytes[0] == std::byte(0) && bytes[1] == std::byte(0) && bytes[2] == std::byte(0xfe) &&
        bytes[3] == std::byte(0xff))
        return *Encoding::GetEncoding(u"UTF32BE");

    return Encoding::UTF8();
}

gsl::span<const std::byte> File::RemoveByteOrderMark(gsl::span<const std::byte> bytes, const Encoding& encoding) noexcept
{
    auto preamble = encoding.GetPreamble();
    if (preamble.size() > bytes.size())
        return bytes;

    size_t j = 0;
    for (size_t i = 0; i < preamble.size(); ++i)
    {
        if (preamble[i] != std::byte(bytes[i]))
            break;

        ++j;
    }

    return gsl::span(bytes).subspan(j);
}

CS2CPP_NAMESPACE_END
