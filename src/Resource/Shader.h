#pragma once
#include "IResource.h"
#include "Utils/Type.h"

#include "Render/RHI/RHIShaderBuffer.h"

class FragmentShader;
class VertexShader;

enum class ShaderType
{
    Vertex,
    Fragment
};

class BaseShader : public IResource
{
public:
    BaseShader(std::filesystem::path path) : IResource(std::move(path)) {}
    BaseShader(const BaseShader&) = delete;
    BaseShader(BaseShader&&) = delete;
    BaseShader& operator=(const BaseShader&) = delete;
    virtual ~BaseShader() override;
    
    virtual bool Load(ResourceManager* resourceManager) override;
    virtual bool SendToGPU(RHIRenderer* renderer) override;
    virtual void Unload() override {}
    
    virtual ShaderType GetShaderType() const = 0;
    std::string GetContent() const { return p_content; }
    RHIShaderBuffer* GetBuffer() const { return p_buffer.get(); }
private:
    std::string p_content;
    std::unique_ptr<RHIShaderBuffer> p_buffer;
};

enum class UniformType
{
    None,
    Unknown,
    Float,
    Int,
    UInt,
    Bool,
    Vec2,
    Vec3,
    Vec4,
    Mat2,
    Mat3,
    Mat4,
    NestedStruct,
    Sampler2D,
    SamplerCube
};

struct Uniform
{
    std::string name;
    UniformType type;
    uint32_t binding;
    uint32_t set;
    uint32_t size;
    std::vector<Uniform> members;
};

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

    std::vector<Uniform> GetUniforms() const { return m_uniforms; }
    VertexShader* GetVertexShader() const { return m_vertexShader.get().get(); }
    FragmentShader* GetFragmentShader() const { return m_fragmentShader.get().get(); }
private:
    void OnShaderSent();
private:
    SafePtr<VertexShader> m_vertexShader;
    SafePtr<FragmentShader> m_fragmentShader;
    
    std::vector<Uniform> m_uniforms;
};
