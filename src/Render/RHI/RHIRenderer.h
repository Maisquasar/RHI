#pragma once
#include <memory>

#include "RHIIndexBuffer.h"
#include "RHITexture.h"
#include "RHIVertexBuffer.h"
#include "Utils/Type.h"

class Texture;
class Window;

namespace ImageLoader
{
    struct Image;
}

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

    virtual bool IsInitialized() const { return p_initialized; }

    static std::unique_ptr<RHIRenderer> Create(RenderAPI api, Window* window);

    virtual bool Initialize(Window* window) = 0;
    virtual void WaitForGPU() = 0;
    virtual void Cleanup() = 0;
    
    virtual void WaitUntilFrameFinished() = 0;
    virtual bool BeginFrame() = 0;
    virtual void Update() = 0;
    virtual void EndFrame() = 0;
    virtual bool MultiThreadSendToGPU() = 0;
    
    virtual void DrawFrame() {}
    
    virtual std::unique_ptr<RHITexture> CreateTexture(const ImageLoader::Image& image) = 0;
    virtual std::unique_ptr<RHIVertexBuffer> CreateVertexBuffer(const float* data, uint32_t size, uint32_t floatPerVertex) = 0;
    virtual std::unique_ptr<RHIIndexBuffer> CreateIndexBuffer(const uint32_t* data, uint32_t size) = 0;
    // virtual std::unique_ptr<RHI
    
    virtual void SetDefaultTexture(const SafePtr<Texture>& texture) = 0;
    
protected:
    RenderAPI p_renderAPI;
    bool p_initialized = false;
    
};
