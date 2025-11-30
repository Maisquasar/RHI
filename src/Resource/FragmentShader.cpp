#include "FragmentShader.h"

bool FragmentShader::Load(ResourceManager* resourceManager)
{
    return BaseShader::Load(resourceManager);
}

bool FragmentShader::SendToGPU(RHIRenderer* renderer)
{
    return true;
}

void FragmentShader::Unload()
{
}
