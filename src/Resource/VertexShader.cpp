#include "VertexShader.h"

#include "Debug/Log.h"

bool VertexShader::Load(ResourceManager* resourceManager)
{
    return BaseShader::Load(resourceManager);
}

bool VertexShader::SendToGPU(RHIRenderer* renderer)
{
    UNUSED(renderer);
    return true;
}

void VertexShader::Unload()
{
    BaseShader::Unload();
}
