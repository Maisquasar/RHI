#pragma once
#include "IComponent.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Resource/ComputeShader.h"

class Material;
class Mesh;

struct InstanceData
{
    Vec4f position = Vec4f::One();
    Vec4f color = Vec4f::One();
};

class ParticleSystemComponent : public IComponent
{
public:
    DECLARE_COMPONENT_TYPE(ParticleSystemComponent);
    
    void Describe(ClassDescriptor& d) override;
    void OnCreate() override;
    void OnUpdate(float deltaTime) override;
    void OnRender(RHIRenderer* renderer) override;
    void OnDestroy() override;
    
    void SetParticleCount(int count);
    void SetMesh(SafePtr<Mesh> mesh);

private:
    
private:
    std::unique_ptr<ComputeDispatch> m_compute;
    
    // Instancing
    std::vector<std::unique_ptr<VulkanBuffer>> m_buffersToCleanup;
    std::unique_ptr<VulkanBuffer> m_instanceBuffer;
    std::unique_ptr<VulkanBuffer> m_instanceStaging;
    std::vector<Vec3f> m_positions;
    std::vector<Vec4f> m_colors;
    
    std::unique_ptr<VulkanBuffer> m_gpuBuffer;
    std::unique_ptr<VulkanBuffer> m_stagingBuffer;
    
    VkFence m_computeFence;
    void* m_mappedStagingMemory = nullptr;
    
    int m_particleCount = 50000;
    
    SafePtr<Mesh> m_mesh;
    SafePtr<Material> m_material;
};
