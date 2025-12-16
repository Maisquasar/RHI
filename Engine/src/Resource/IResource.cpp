#include "IResource.h"

#include "ResourceManager.h"
IResource::IResource(const std::filesystem::path& path)
{
    p_path = ResourceManager::SanitizePath(path);
}

std::string IResource::GetName() const
{
    return p_path.filename().stem().generic_string().c_str();
}
