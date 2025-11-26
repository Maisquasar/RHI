#pragma once
#include <filesystem>
#include <galaxymath/Maths.h>

class MTLLoader
{
public:
    MTLLoader() = default;

    struct Material
    {
        std::filesystem::path name;
        Vec3f ambient;
        Vec3f diffuse;
        Vec3f specular;
        Vec4f emissive;
        float transparency;
        std::optional<std::filesystem::path> albedo;
    };

    static std::vector<Material> Load(const std::filesystem::path& path);

private:
};
