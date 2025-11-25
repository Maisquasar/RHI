#include "ResourceManager.h"

#include "Texture.h"

void ResourceManager::Initialize(RHIRenderer* renderer)
{
    m_renderer = renderer;
}

void ResourceManager::LoadDefaultTexture(const std::filesystem::path& resourcePath)
{
    Load<Texture>(resourcePath, false);
    
    m_defaultTexture = GetHash(resourcePath);
}

std::shared_ptr<Texture> ResourceManager::GetDefaultTexture() const
{
    return GetResource<Texture>(m_defaultTexture);
}
