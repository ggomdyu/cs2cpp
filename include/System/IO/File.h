#pragma once

#include "System/DateTime.h"
#include "System/FunctionTraits.h"
#include "System/IO/StreamReader.h"
#include "System/IO/StreamWriter.h"
#include "System/Text/Encoding.h"

#include "FileAttributes.h"
#include "FileStream.h"
#include "SafeFilePointer.h"

CS2CPP_NAMESPACE_BEGIN

class File final
{
public:
    File() = delete;

public:
    static bool Copy(std::u16string_view srcPath, std::u16string_view destPath, bool overwrite = false);
    static bool Delete(std::u16string_view path);
    static bool Exists(std::u16string_view path);
    static bool Move(std::u16string_view srcPath, std::u16string_view destPath);
    static bool Decrypt(std::u16string_view path);
    static bool Encrypt(std::u16string_view path);
    static bool SetAttributes(std::u16string_view path, FileAttributes fileAttributes);
    static bool SetCreationTime(std::u16string_view path, DateTime creationTime);
    static bool SetCreationTimeUtc(std::u16string_view path, DateTime creationTimeUtc);
    static bool SetLastAccessTime(std::u16string_view path, DateTime lastAccessTime);
    static bool SetLastAccessTimeUtc(std::u16string_view path, DateTime lastAccessTimeUtc);
    static bool SetLastWriteTime(std::u16string_view path, DateTime lastWriteTime);
    static bool SetLastWriteTimeUtc(std::u16string_view path, DateTime lastWriteTimeUtc);
    [[nodiscard]] static std::optional<FileAttributes> GetAttributes(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetCreationTime(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetCreationTimeUtc(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetLastAccessTime(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetLastAccessTimeUtc(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetLastWriteTime(std::u16string_view path);
    [[nodiscard]] static std::optional<DateTime> GetLastWriteTimeUtc(std::u16string_view path);
    [[nodiscard]] static std::shared_ptr<FileStream> Create(std::u16string_view path);
    [[nodiscard]] static std::shared_ptr<FileStream> Create(std::u16string_view path, int32_t bufferSize);
    [[nodiscard]] static std::shared_ptr<FileStream> Create(std::u16string_view path, int32_t bufferSize, FileOptions options);
    [[nodiscard]] static std::shared_ptr<FileStream> Open(std::u16string_view path, FileMode mode);
    [[nodiscard]] static std::shared_ptr<FileStream> Open(std::u16string_view path, FileMode mode, FileAccess access);
    [[nodiscard]] static std::shared_ptr<FileStream> Open(std::u16string_view path, FileMode mode, FileAccess access, FileShare share);
    [[nodiscard]] static std::shared_ptr<FileStream> OpenRead(std::u16string_view path);
    [[nodiscard]] static std::shared_ptr<FileStream> OpenWrite(std::u16string_view path);
    [[nodiscard]] static std::optional<StreamReader> OpenText(std::u16string_view path);
    [[nodiscard]] static std::optional<StreamWriter> CreateText(std::u16string_view path);
    [[nodiscard]] static std::optional<StreamWriter> AppendText(std::u16string_view path);
    [[nodiscard]] static std::optional<std::u16string> ReadAllText(std::u16string_view path);
    [[nodiscard]] static std::optional<std::u16string> ReadAllText(std::u16string_view path, const Encoding& encoding);
    static bool WriteAllText(std::u16string_view path, std::u16string_view contents);
    static bool WriteAllText(std::u16string_view path, std::u16string_view contents, const Encoding& encoding);
    [[nodiscard]] static std::optional<std::vector<std::byte>> ReadAllBytes(std::u16string_view path);
    static bool WriteAllBytes(std::u16string_view path, gsl::span<const std::byte> bytes);
    template <typename F>
    static bool ReadLines(std::u16string_view path, const F& callback);
    [[nodiscard]] static std::optional<std::vector<std::u16string>> ReadAllLines(std::u16string_view path);
    [[nodiscard]] static std::optional<std::vector<std::u16string>> ReadAllLines(std::u16string_view path, const Encoding& encoding);
    template <typename F>
    static bool ReadLines(std::u16string_view path, const Encoding& encoding, const F& callback);
    static bool WriteAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents);
    static bool WriteAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents, const Encoding& encoding);
    static bool AppendAllText(std::u16string_view path, std::u16string_view contents);
    static bool AppendAllText(std::u16string_view path, std::u16string_view contents, const Encoding& encoding);
    static bool AppendAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents);
    static bool AppendAllLines(std::u16string_view path, gsl::span<std::u16string_view> contents, const Encoding& encoding);
    static bool Replace(std::u16string_view srcPath, std::u16string_view destPath, const std::u16string_view* destBackupPath, bool ignoreMetadataErrors = false);

private:
    static std::vector<std::byte> InternalReadAllText(std::u16string_view path);
    static SafeFilePointer InternalFileOpen(std::u16string_view path, std::u16string_view mode);
    static void InternalWriteAllLines(StreamWriter& writer, gsl::span<std::u16string_view> contents);
    static void InternalWriteAllText(StreamWriter& writer, std::u16string_view contents);
    static const Encoding& DetectEncoding(gsl::span<const std::byte> bytes) noexcept;
    static gsl::span<const std::byte> RemoveByteOrderMark(gsl::span<const std::byte> bytes, const Encoding& encoding) noexcept;
};

template <typename F>
bool File::ReadLines(std::u16string_view path, const F& callback)
{
    return ReadLines(path, Encoding::UTF8(), callback);
}

template <typename F>
bool File::ReadLines(std::u16string_view path, const Encoding& encoding, const F& callback)
{
    auto reader = StreamReader::Create(path, encoding);
    if (!reader)
        return false;

    while (true)
    {
        auto line = reader->ReadLine();
        if (!line)
            break;

        if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
        {
            if (!callback(std::move(*line)))
                break;
        }
        else
            callback(std::move(*line));
    }

    return true;
}

CS2CPP_NAMESPACE_END
