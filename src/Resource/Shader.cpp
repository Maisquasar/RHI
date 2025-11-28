#include "Shader.h"

#include "Debug/Log.h"

bool BaseShader::Load(ResourceManager* resourceManager)
{
    UNUSED(resourceManager);
    return true;
}

bool BaseShader::SendToGPU(RHIRenderer* renderer)
{
    UNUSED(renderer);
    return true;
}

bool Shader::Load(ResourceManager* resourceManager)
{
    UNUSED(resourceManager);
    return true;
}

bool Shader::SendToGPU(RHIRenderer* renderer)
{
    UNUSED(renderer);
    return true;
}

void Shader::Unload()
{
}
