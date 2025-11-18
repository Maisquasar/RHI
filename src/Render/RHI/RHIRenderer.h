#pragma once
#include <memory>

#include "RHITexture.h"
#include "Resource/Loader/ImageLoader.h"

class Window;

enum class RenderAPI
{
    None,
    Vulkan,
    DirectX,
    OpenGL,
    Metal,
};

class RHIRenderer
{
public:
    RHIRenderer() = default;
    virtual ~RHIRenderer();

    static std::unique_ptr<RHIRenderer> Create(RenderAPI api, Window* window);

    virtual bool Initialize(Window* window) = 0;
    virtual void Cleanup() = 0;
    
    virtual void DrawFrame() = 0;
    virtual bool MultiThreadSendToGPU() = 0;
    virtual std::unique_ptr<RHITexture> CreateTexture(const ImageLoader::Image& image) = 0;
    
private:
    RenderAPI m_renderAPI;
};
