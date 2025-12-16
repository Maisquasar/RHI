#pragma once
#include <vector>

#include "IResource.h"

class Mesh;

class Model : public IResource
{
public:
    DECLARE_RESOURCE_TYPE(Model)

    bool Load(ResourceManager* resourceManager) override;
    bool SendToGPU(RHIRenderer* renderer) override;
    void Unload() override;
    
    std::vector<Mesh*> GetMeshes() const { return m_meshes; }
private:
    std::vector<Mesh*> m_meshes;
};
