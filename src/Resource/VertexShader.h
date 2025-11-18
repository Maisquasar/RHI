#pragma once
#include "IResource.h"

class VertexShader : public IResource
{
public:
    VertexShader(std::filesystem::path path) : IResource(std::move(path)) {}
    VertexShader(const VertexShader&) = delete;
    VertexShader(VertexShader&&) = delete;
    VertexShader& operator=(const VertexShader&) = delete;
    virtual ~VertexShader() override = default;
    
    bool Load(ResourceManager* resourceManager) override;
    bool SendToGPU(RHIRenderer* renderer) override;
    void Unload() override;
private:
};
