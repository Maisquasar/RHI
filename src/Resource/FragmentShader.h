#pragma once
#include "IResource.h"

class FragmentShader : public IResource
{
public:
    FragmentShader(std::filesystem::path path) : IResource(std::move(path)) {}
    FragmentShader(const FragmentShader&) = delete;
    FragmentShader(FragmentShader&&) = delete;
    FragmentShader& operator=(const FragmentShader&) = delete;
    virtual ~FragmentShader() override = default;
    
    bool Load(ResourceManager* resourceManager) override;
    bool SendToGPU(RHIRenderer* renderer) override;
    void Unload() override;
private:
};

