#pragma once
#include "IResource.h"
#include "Utils/Type.h"

class FragmentShader;
class VertexShader;

class Shader : public IResource
{
public:
    Shader(std::filesystem::path path) : IResource(std::move(path)) {}
    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(const Shader&) = delete;
    virtual ~Shader() override = default;
    
    bool Load(ResourceManager* resourceManager) override;
    bool SendToGPU(RHIRenderer* renderer) override;
    void Unload() override;

private:
    SafePtr<VertexShader> m_vertexShader;
    SafePtr<FragmentShader> m_fragmentShader;
};
