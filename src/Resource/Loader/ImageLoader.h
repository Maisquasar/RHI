#pragma once
#include <filesystem>
#include <galaxymath/Maths.h>

namespace ImageLoader
{
    struct Image
    {
        Vec2i size;
        unsigned char* data;
        int channels;
    };

    void FlipVerticalOnLoad(bool flagTrueIfShouldFlip);

    bool Load(char const* filename, Image& image, int req_comp = 4);
    bool Load(const std::filesystem::path& path, Image& image, int req_comp = 4);
    Image* LoadIco(char const* filename, int* count);

    Image LoadFromMemory(unsigned char* data, int len);

    void SaveImage(const char* filename, const Image& image);

    void ImageFree(void* data);
    void ImageFree(const Image& image);
}
