#include "Material.h"

#include <ranges>

#include "Shader.h"
#include "Debug/Log.h"

bool Material::Load(ResourceManager* resourceManager)
{
    return true;
}

bool Material::SendToGPU(RHIRenderer* renderer)
{
    return true;
}

void Material::Unload()
{}

void Material::SetShader(const SafePtr<Shader>& shader)
{
    m_shader = shader;

    auto lambda = [this]()
    {
        m_attributes.Clear();
        Uniforms uniforms = m_shader->GetUniforms();

        for (Uniform& uniform : uniforms | std::views::values)
        {
            switch (uniform.type)
            {
                case UniformType::NestedStruct:
                    for (UniformMember& member : uniform.members)
                    {
                        switch (member.type)
                        {
                            case UniformType::Float:
                            {
                                float value = m_temporaryAttributes.floatAttributes.contains(member.name) ? m_temporaryAttributes.floatAttributes[member.name].value : 0.f;
                                m_attributes.floatAttributes[member.name] = Attribute<float>(
                                    uniform.name,
                                    value
                                );
                            }
                            break;
                            case UniformType::Int:
                            {
                                int value = m_temporaryAttributes.intAttributes.contains(member.name) ? m_temporaryAttributes.intAttributes[member.name].value : 0.f;
                                m_attributes.intAttributes[member.name] = Attribute<int>(
                                    uniform.name,
                                    value
                                );
                            }
                            break;
                            case UniformType::Vec2:
                            {
                                Vec2f value = m_temporaryAttributes.vec2Attributes.contains(member.name) ? m_temporaryAttributes.vec2Attributes[member.name].value : Vec2f::Zero();
                                m_attributes.vec2Attributes[member.name] = Attribute<Vec2f>(
                                    uniform.name,
                                    value
                                );
                            }
                            break;
                            case UniformType::Vec3:
                            {
                                Vec3f value = m_temporaryAttributes.vec3Attributes.contains(member.name) ? m_temporaryAttributes.vec3Attributes[member.name].value : Vec3f::Zero();
                                m_attributes.vec3Attributes[member.name] = Attribute<Vec3f>(
                                    uniform.name,
                                    value
                                );
                            }
                            case UniformType::Vec4:
                            {
                                Vec4f value = m_temporaryAttributes.vec4Attributes.contains(member.name) ? m_temporaryAttributes.vec4Attributes[member.name].value : Vec4f::Zero();
                                m_attributes.vec4Attributes[member.name] = Attribute<Vec4f>(
                                    uniform.name,
                                    value
                                );
                            }
                            case UniformType::Mat4:
                            {
                                Mat4 value = m_temporaryAttributes.matrixAttributes.contains(member.name) ? m_temporaryAttributes.matrixAttributes[member.name].value : Mat4::Identity();
                                m_attributes.matrixAttributes[member.name] = Attribute<Mat4>(
                                    uniform.name,
                                    value
                                );
                            }
                        }
                    }
                    break;
                case UniformType::Sampler2D:
                    m_attributes.samplerAttributes[uniform.name] =
                    m_temporaryAttributes.samplerAttributes.contains(uniform.name) ? m_temporaryAttributes.samplerAttributes[uniform.name].value : SafePtr<Texture>{};
                    break;
                case UniformType::SamplerCube:
                    break;
                default:
                    PrintError("Unknown uniform type: %d", static_cast<int>(uniform.type));
                    break;
            }
        }
    };
    if (m_shader->SentToGPU())
    {
        lambda();
    }
    else
    {
        m_shader->OnSentToGPU.Bind(lambda);
    }
}

void Material::SetAttribute(const std::string& name, float attribute)
{
    if (m_attributes.floatAttributes.contains(name))
    {
        m_attributes.floatAttributes[name] = attribute;
    }
    else if (m_shader && !m_shader->SentToGPU())
    {
        m_temporaryAttributes.floatAttributes[name] = attribute;
    }
}

void Material::SetAttribute(const std::string& name, int attribute)
{
    if (m_attributes.intAttributes.contains(name))
    {
        m_attributes.intAttributes[name] = attribute;
    }
    else if (m_shader && !m_shader->SentToGPU())
    {
        m_temporaryAttributes.intAttributes[name] = attribute;
    }
}

void Material::SetAttribute(const std::string& name, const Vec2f& attribute)
{
    if (m_attributes.vec2Attributes.contains(name))
    {
        m_attributes.vec2Attributes[name] = attribute;
    }
    else if (m_shader && !m_shader->SentToGPU())
    {
        m_temporaryAttributes.vec2Attributes[name] = attribute;
    }
}

void Material::SetAttribute(const std::string& name, const Vec3f& attribute)
{
    if (m_attributes.vec3Attributes.contains(name))
    {
        m_attributes.vec3Attributes[name] = attribute;
    }
    else if (m_shader && !m_shader->SentToGPU())
    {
        m_temporaryAttributes.vec3Attributes[name] = attribute;
    }
}

void Material::SetAttribute(const std::string& name, const Vec4f& attribute)
{
    if (m_attributes.vec4Attributes.contains(name))
    {
        m_attributes.vec4Attributes[name] = attribute;
    }
    else if (m_shader && !m_shader->SentToGPU())
    {
        m_temporaryAttributes.vec4Attributes[name] = attribute;
    }
}

void Material::SetAttribute(const std::string& name, const SafePtr<Texture>& texture)
{
    if (m_attributes.samplerAttributes.contains(name))
    {
        m_attributes.samplerAttributes[name] = texture;
    }
    else if (m_shader && !m_shader->SentToGPU())
    {
        m_temporaryAttributes.samplerAttributes[name] = texture;
    }
}
void Material::SetAttribute(const std::string& name, const Mat4& attribute)
{
    if (m_attributes.matrixAttributes.contains(name))
    {
        m_attributes.matrixAttributes[name] = attribute;
    }
    else if (m_shader && !m_shader->SentToGPU())
    {
        m_temporaryAttributes.matrixAttributes[name] = attribute;
    }
}

void Material::SendAllValues(RHIRenderer* renderer)
{
    std::unordered_map<std::string, CustomAttributes> customAttributes;
    
    for (auto& attrib : m_attributes.floatAttributes)
    {
        customAttributes[attrib.second.uniformName].size += sizeof(float);
    }
    for (auto& attrib : m_attributes.intAttributes)
    {
        customAttributes[attrib.second.uniformName].size += sizeof(int);
    }
    for (auto& attrib : m_attributes.vec2Attributes)
    {
        customAttributes[attrib.second.uniformName].size += sizeof(Vec2f);
    }
    for (auto& attrib : m_attributes.vec3Attributes)
    {
        customAttributes[attrib.second.uniformName].size += sizeof(Vec3f);
    }
    for (auto& attrib : m_attributes.vec4Attributes)
    {
        customAttributes[attrib.second.uniformName].size += sizeof(Vec4f);
    }
    for (auto& attrib : m_attributes.matrixAttributes)
    {
        customAttributes[attrib.second.uniformName].size += sizeof(Mat4);
    }
    
    //TODO allocate memory and set the value to the memory for each uniforms
    
    for (auto& uniformData : customAttributes)
    {
        Uniform uniform = m_shader->GetUniform(uniformData.first);
        auto binding = UBOBinding(uniform.set, uniform.binding);
        m_shader->SendValue(binding, uniformData.second.data, uniformData.second.size, renderer);
    }
}
