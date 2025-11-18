#include "Texture.h"

#include "Loader/ImageLoader.h"
#include "Render/RHI/RHIRenderer.h"

bool Texture::Load(ResourceManager* resourceManager)
{    
    m_image = ImageLoader::Image();
    if (!ImageLoader::Load(p_path, m_image))
    {
        throw std::runtime_error("Failed to load image " + p_path.string());
        return false;
    }
    return true;
}

bool Texture::SendToGPU(RHIRenderer* renderer)
{
    m_buffer = renderer->CreateTexture(m_image);
    ImageLoader::ImageFree(m_image);
    m_image = ImageLoader::Image();
    return true;
}

void Texture::Unload()
{
    m_buffer.reset();
    if (m_image.data)
    {
        ImageLoader::ImageFree(m_image);
        m_image = ImageLoader::Image();
    }
}
