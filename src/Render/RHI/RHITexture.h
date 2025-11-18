#pragma once
#include <cstdint>

class RHITexture
{
public:
    RHITexture() = default;
    RHITexture& operator=(const RHITexture& other) = default;
    RHITexture(const RHITexture&) = default;
    RHITexture(RHITexture&&) noexcept = default;
    virtual ~RHITexture() = default;
    
    uint32_t GetWidth() const { return p_width; }
    uint32_t GetHeight() const { return p_height; }
protected:
    uint32_t p_width = 0;
    uint32_t p_height = 0;
};
