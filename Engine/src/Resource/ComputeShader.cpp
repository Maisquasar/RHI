#include "ComputeShader.h"

bool ComputeShader::SendToGPU(RHIRenderer* renderer)
{
    if (!BaseShader::SendToGPU(renderer))
        return false;
    
    return true;
}
