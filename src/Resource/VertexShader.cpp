#include "VertexShader.h"

#include "Debug/Log.h"
#include "Render/RHI/RHIRenderer.h"

bool VertexShader::Load(ResourceManager* resourceManager)
{
    return BaseShader::Load(resourceManager);
}

bool VertexShader::SendToGPU(RHIRenderer* renderer)
{
    return BaseShader::SendToGPU(renderer);
}

void VertexShader::Unload()
{
    BaseShader::Unload();
}
