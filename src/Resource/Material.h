#pragma once
#include "IResource.h"

#include "Utils/Type.h"

class Shader;

class Material : public IResource
{
public:
    Material() = default;
    Material(const Material&) = delete;
    Material(Material&&) = delete;
    Material& operator=(const Material&) = delete;
    ~Material() override = default;
    
    bool Load(ResourceManager* resourceManager) override;
    bool SendToGPU(RHIRenderer* renderer) override;
    void Unload() override;
private:
    SafePtr<Shader> m_shader = nullptr;
};
