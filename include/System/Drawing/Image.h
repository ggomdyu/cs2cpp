#pragma once

#include <gsl/span>
#include <memory>
#include <optional>

CS2CPP_NAMESPACE_BEGIN

enum class ImageFormat
{
    Unknown = 0,
    Bmp,
    Emf,
    Exif,
    Gif,
    Icon,
    Jpeg,
    MemoryBmp,
    Png,
    Tiff,
    Wmf,
};

enum class RotateFlipType
{
    Rotate180FlipXY = 0,
    RotateNoneFlipNone = 0,
    Rotate270FlipXY = 1,
    Rotate90FlipNone = 1,
    Rotate180FlipNone = 2,
    RotateNoneFlipXY = 2,
    Rotate270FlipNone = 3,
    Rotate90FlipXY = 3,
    Rotate180FlipY = 4,
    RotateNoneFlipX = 4,
    Rotate270FlipY = 5,
    Rotate90FlipX = 5,
    Rotate180FlipX = 6,
    RotateNoneFlipY = 6,
    Rotate270FlipX = 7,
    Rotate90FlipY = 7,
};

enum class PixelFormat
{
    Unknown = 0,
    RGBA8888,
    RGB888,
    RGBA4444,
    R8,
};

class Image final
{
private:
    Image(std::unique_ptr<std::byte[]> imageData, int32_t width, int32_t height, PixelFormat pixelFormat);

public:
    [[nodiscard]] bool operator==(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] bool operator!=(std::nullptr_t rhs) const noexcept;
    [[nodiscard]] std::byte& operator[](int32_t index);
    [[nodiscard]] std::byte operator[](int32_t index) const;

public:
    [[nodiscard]] static std::optional<Image> Create(const char16_t* filePath);
    [[nodiscard]] static std::optional<Image> Create(gsl::span<const std::byte> bytes);
    [[nodiscard]] std::byte* GetImageData() noexcept;
    [[nodiscard]] const std::byte* GetImageData() const noexcept;
    [[nodiscard]] int32_t GetWidth() const noexcept;
    [[nodiscard]] int32_t GetHeight() const noexcept;
    [[nodiscard]] constexpr PixelFormat GetPixelFormat() const noexcept;
    bool Save(const char* filePath, ImageFormat format) const;
    void RotateFlip(RotateFlipType rotateFlipType);

private:
    std::unique_ptr<std::byte[]> _imageData;
    int32_t _width;
    int32_t _height;
    static constexpr PixelFormat InternalPixelFormat = PixelFormat::RGBA8888;
};

constexpr PixelFormat Image::GetPixelFormat() const noexcept
{
    return InternalPixelFormat;
}

CS2CPP_NAMESPACE_END