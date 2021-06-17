#include <emmintrin.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#ifdef _MSC_VER
#    define STBI_MSC_SECURE_CRT
#endif
#include <stb_image.h>
#include <stb_image_write.h>

#include "System/Drawing/Image.h"
#include "System/IO/File.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::image
{

void FlipRGBAImageX(std::byte* imageData, int32_t width, int32_t height)
{
    if (!imageData)
        return;

    using Color4b = uint32_t;

#if CS2CPP_SIMD_SSE2
    auto simdUnusedSize = (width * height) % 8;
    for (int32_t y = 0; y < height; ++y)
    {
        if (simdUnusedSize > 0)
        {
            auto* frontIt = reinterpret_cast<Color4b*>(&imageData[y * width + width / 2 - simdUnusedSize / 2]);
            auto* backIt = frontIt + simdUnusedSize - 1;
            for (; frontIt < backIt; ++frontIt, --backIt)
                std::swap(*frontIt, *backIt);
        }

        auto* frontIt = reinterpret_cast<__m128i*>(&imageData[y * width * 4]);
        auto* backIt = reinterpret_cast<__m128i*>(&imageData[(y + 1) * width * 4 - static_cast<int32_t>(sizeof(__m128i))]);
        for (; frontIt < backIt; ++frontIt, --backIt)
        {
            auto r1 = _mm_shuffle_epi32(_mm_loadu_si128(frontIt), _MM_SHUFFLE(0, 1, 2, 3));
            auto r2 = _mm_shuffle_epi32(_mm_loadu_si128(backIt), _MM_SHUFFLE(0, 1, 2, 3));
            _mm_storeu_si128(frontIt, r2);
            _mm_storeu_si128(backIt, r1);
        }
    }
#else
    auto frontIt = reinterpret_cast<Color4b*>(imageData);
    auto backIt = &(reinterpret_cast<Color4b*>(imageData))[width - 1];
    for (; frontIt < backIt; ++frontIt, --backIt)
    {
        for (int32_t col = 0; col < width * height; col += width)
        {
            std::swap(*(frontIt + col), *(backIt + col));
        }
    }
#endif
}

void FlipRGBAImageXY(std::byte* imageData, int32_t width, int32_t height)
{
    if (!imageData)
        return;

    using Color4b = uint32_t;

#if CS2CPP_SIMD_SSE2
    auto simdUnusedSize = (width * height) % 8;
    if (simdUnusedSize > 0)
    {
        auto* frontIt = reinterpret_cast<Color4b*>(&imageData[width * height / 2 - simdUnusedSize / 2]);
        auto* backIt = frontIt + simdUnusedSize - 1;
        for (; frontIt < backIt; ++frontIt, --backIt)
            std::swap(*frontIt, *backIt);
    }

    auto* frontIt = reinterpret_cast<__m128i*>(imageData);
    auto* backIt = reinterpret_cast<__m128i*>(&imageData[width * height * 4 - static_cast<int32_t>(sizeof(__m128i))]);
    for (; frontIt < backIt; ++frontIt, --backIt)
    {
        auto r1 = _mm_shuffle_epi32(_mm_loadu_si128(frontIt), _MM_SHUFFLE(0, 1, 2, 3));
        auto r2 = _mm_shuffle_epi32(_mm_loadu_si128(backIt), _MM_SHUFFLE(0, 1, 2, 3));
        _mm_storeu_si128(frontIt, r2);
        _mm_storeu_si128(backIt, r1);
    }
#else
    auto* frontIt = reinterpret_cast<Color4b*>(imageData);
    auto* backIt = &(reinterpret_cast<Color4b*>(imageData))[width * height - 1];
    for (; frontIt < backIt; ++frontIt, --backIt)
    {
        std::swap(*frontIt, *backIt);
    }
#endif
}

void FlipImageY(std::byte* imageData, int32_t width, int32_t height, int32_t bytesPerPixel)
{
    if (!imageData)
        return;

    int32_t stride = width * bytesPerPixel;
    auto tempRowBuffer = std::make_unique<std::byte[]>(static_cast<size_t>(stride));

    auto* frontIt = imageData;
    auto* backIt = &imageData[(height - 1) * stride];
    for (; frontIt < backIt; frontIt += stride, backIt -= stride)
    {
        std::copy_n(frontIt, stride, tempRowBuffer.get());
        std::copy_n(backIt, stride, frontIt);
        std::copy_n(tempRowBuffer.get(), stride, backIt);
    }
}

void RotateRGBAImageRight90Degrees(std::byte* imageData, int32_t width, int32_t height)
{
    if (!imageData)
        return;

    using Color4b = uint32_t;
    auto imageDataSize = static_cast<size_t>(width * height) * 4;
    auto tempBuffer = std::make_unique<std::byte[]>(imageDataSize);

    auto* srcRow = reinterpret_cast<Color4b*>(imageData);
    auto* destCol = &(reinterpret_cast<Color4b*>(tempBuffer.get()))[height - 1];
    for (; srcRow < &(reinterpret_cast<Color4b*>(imageData)[width * height]); srcRow += width, --destCol)
    {
        for (int32_t x = 0; x < width; ++x)
            destCol[x * height] = srcRow[x];
    }

    std::copy_n(tempBuffer.get(), imageDataSize, imageData);
}

void RotateRGBAImageLeft90Degrees(std::byte* imageData, int32_t width, int32_t height)
{
    if (!imageData)
        return;

    using Color4b = uint32_t;
    auto imageDataSize = width * height * 4;
    auto tempBuffer = std::make_unique<std::byte[]>(imageDataSize);

    auto* srcRow = reinterpret_cast<Color4b*>(imageData);
    auto* destCol = &reinterpret_cast<Color4b*>(tempBuffer.get())[(width - 1) * height];
    for (; srcRow < &(reinterpret_cast<Color4b*>(imageData)[width * height]); srcRow += width, ++destCol)
    {
        for (int32_t x = 0; x < width; ++x)
            destCol[-x * height] = srcRow[x];
    }

    std::copy_n(tempBuffer.get(), imageDataSize, imageData);
}

}

Image::Image(std::unique_ptr<std::byte[]> imageData, int32_t width, int32_t height, PixelFormat pixelFormat) :
    _imageData(std::move(imageData)),
    _width(width),
    _height(height)
{
}

bool Image::operator==(std::nullptr_t rhs) const noexcept
{
    return _imageData == nullptr;
}

bool Image::operator!=(std::nullptr_t rhs) const noexcept
{
    return _imageData != nullptr;
}

std::byte& Image::operator[](int32_t index)
{
    return _imageData[index];
}

std::byte Image::operator[](int32_t index) const
{
    return _imageData[index];
}

std::optional<Image> Image::Create(const char16_t* filePath)
{
    auto fileData = File::ReadAllBytes(filePath);
    if (!fileData)
        return {};

    return Create(*fileData);
}

std::optional<Image> Image::Create(gsl::span<const std::byte> bytes)
{
    int width = 0, height = 0;
    auto imageData = std::unique_ptr<std::byte[]>(reinterpret_cast<std::byte*>(stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(&bytes[0]), static_cast<int>(bytes.size()), &width, &height, nullptr, STBI_rgb_alpha)));
    if (!imageData)
        return {};

    return Image(std::move(imageData), static_cast<int32_t>(width), static_cast<int32_t>(height), PixelFormat::RGBA8888);
}

std::byte* Image::GetImageData() noexcept
{
    return &_imageData[0];
}

const std::byte* Image::GetImageData() const noexcept
{
    return &_imageData[0];
}

int32_t Image::GetWidth() const noexcept
{
    return _width;
}

int32_t Image::GetHeight() const noexcept
{
    return _height;
}

bool Image::Save(const char* filePath, ImageFormat format) const
{
    switch (format)
    {
    case ImageFormat::Png:
        return stbi_write_png(reinterpret_cast<const char*>(filePath), _width, _height, 4, _imageData.get(), _width * 4) != 0;

    case ImageFormat::Jpeg:
        return stbi_write_jpg(reinterpret_cast<const char*>(filePath), _width, _height, 4, _imageData.get(), 100) != 0;

    case ImageFormat::Bmp:
        return stbi_write_bmp(reinterpret_cast<const char*>(filePath), _width, _height, 4, _imageData.get()) != 0;

    default:
        return false;
    }
}

void Image::RotateFlip(RotateFlipType rotateFlipType)
{
    using namespace detail::image;

    switch (rotateFlipType)
    {
    case RotateFlipType::RotateNoneFlipNone:
        break;

    case RotateFlipType::Rotate90FlipNone:
        RotateRGBAImageRight90Degrees(_imageData.get(), _width, _height);
        std::swap(_width, _height);
        break;

    case RotateFlipType::Rotate180FlipNone:
        FlipRGBAImageXY(_imageData.get(), _width, _height);
        break;

    case RotateFlipType::Rotate270FlipNone:
        RotateRGBAImageLeft90Degrees(_imageData.get(), _width, _height);
        std::swap(_width, _height);
        break;

    case RotateFlipType::RotateNoneFlipX:
        FlipRGBAImageX(_imageData.get(), _width, _height);
        break;

    case RotateFlipType::Rotate90FlipX:
        RotateRGBAImageRight90Degrees(_imageData.get(), _width, _height);
        FlipRGBAImageX(_imageData.get(), _width, _height);
        std::swap(_width, _height);
        break;

    case RotateFlipType::RotateNoneFlipY:
        FlipImageY(_imageData.get(), _width, _height, 4);
        break;

    case RotateFlipType::Rotate90FlipY:
        RotateRGBAImageRight90Degrees(_imageData.get(), _width, _height);
        FlipImageY(_imageData.get(), _width, _height, 4);
        std::swap(_width, _height);
        break;
    }
}

CS2CPP_NAMESPACE_END
