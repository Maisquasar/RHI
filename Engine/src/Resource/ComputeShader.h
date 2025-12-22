#pragma once
#include "Shader.h"

class ComputeShader : public BaseShader
{
public:
    ComputeShader(std::filesystem::path path) : BaseShader(std::move(path)) {}
    ComputeShader(const ComputeShader&) = delete;
    ComputeShader(ComputeShader&&) = delete;
    ComputeShader& operator=(const ComputeShader&) = delete;
    virtual ~ComputeShader() override {}
    
    ResourceType GetResourceType() const override { return ResourceType::ComputeShader; }
    
    ShaderType GetShaderType() const override { return ShaderType::Compute; }
    
    bool SendToGPU(RHIRenderer* renderer) override;
};
