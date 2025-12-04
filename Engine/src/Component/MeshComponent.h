#pragma once
#include "IComponent.h"
#include "Resource/Mesh.h"
#include "Utils/Type.h"

class Material;

class MeshComponent : public IComponent
{
public:
    MeshComponent() = default;
    MeshComponent& operator=(const MeshComponent& other) = default;
    MeshComponent(const MeshComponent&) = default;
    MeshComponent(MeshComponent&&) noexcept = default;
    ~MeshComponent() override = default;
    
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    
private:
    std::vector<SafePtr<Material>> m_materials;
    SafePtr<Mesh> m_mesh;
};
