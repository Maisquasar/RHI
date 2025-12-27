#include "ParticleSystemComponent.h"

#include <utility>

#include "Core/Engine.h"
#include "Render/Vulkan/VulkanIndexBuffer.h"
#include "Render/Vulkan/VulkanRenderer.h"
#include "Render/Vulkan/VulkanVertexBuffer.h"
#include "Scene/GameObject.h"
#include "Utils/Color.h"
#include "Utils/Random.h"

void ParticleSystemComponent::Describe(ClassDescriptor& d)
{
    {
        auto& property = d.AddInt("particleCount", m_particleCount);
        property.setter = [this](void* data)
        {
            SetParticleCount(*static_cast<int*>(data));
        };
    }
    
    auto& property = d.AddProperty("Mesh", PropertyType::Mesh, &m_mesh);
    property.setter = [this](void* data)
    {
        SetMesh(*static_cast<SafePtr<Mesh>*>(data));
    };
}

void ParticleSystemComponent::OnCreate()
{
    auto resourceManager = Engine::Get()->GetResourceManager();
    auto renderer = Engine::Get()->GetRenderer();
    // auto shader = resourceManager->Load<Shader>(RESOURCE_PATH"/shaders/Compute/multiply.shader");

    auto instancingShader = resourceManager->Load<Shader>(RESOURCE_PATH"/shaders/Instancing/instancing.shader");
    m_material = resourceManager->CreateMaterial("Instancing");
    m_material->SetShader(instancingShader);

    m_mesh = resourceManager->Load<Mesh>(RESOURCE_PATH"/models/Cube.obj/Cube");

    SetParticleCount(50000);
}

void ParticleSystemComponent::OnUpdate(float deltaTime)
{
    if (!m_buffersToCleanup.empty())
    {
        auto vulkanRenderer = Cast<VulkanRenderer>(Engine::Get()->GetRenderer());
        vkDeviceWaitIdle(vulkanRenderer->GetDevice()->GetDevice());
        
        for (auto& buffer : m_buffersToCleanup)
        {
            buffer->Cleanup();
        }
        m_buffersToCleanup.clear();
    }
    
    if (!m_instanceBuffer) return;

    auto renderer = Cast<VulkanRenderer>(Engine::Get()->GetRenderer());
    auto device = renderer->GetDevice();

    std::vector<InstanceData> stagingData(m_particleCount);
    for (int i = 0; i < m_particleCount; i++)
    {
        stagingData[i].position = m_positions[i];
        stagingData[i].color = m_colors[i];
    }

    VkDeviceSize bufferSize = sizeof(InstanceData) * m_particleCount;
    m_instanceStaging->CopyData(stagingData.data(), bufferSize);

    VkCommandBuffer commandBuffer = renderer->GetCommandBuffer();

    VkBufferMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    barrier.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.buffer = m_instanceBuffer->GetBuffer();
    barrier.offset = 0;
    barrier.size = bufferSize;

    vkCmdPipelineBarrier(commandBuffer,
                         VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                         VK_PIPELINE_STAGE_TRANSFER_BIT,
                         0, 0, nullptr, 1, &barrier, 0, nullptr);

    m_instanceBuffer->CopyFrom(commandBuffer, m_instanceStaging.get(), bufferSize);

    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    vkCmdPipelineBarrier(commandBuffer,
                         VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                         0, 0, nullptr, 1, &barrier, 0, nullptr);

    if (!m_mesh)
        return;

    CameraData cameraData = p_gameObject->GetScene()->GetCameraData();
    auto transform = p_gameObject->GetTransform();
    Mat4 VP = cameraData.VP;

    m_material->SetAttribute("viewProj", VP);
}

void ParticleSystemComponent::OnRender(RHIRenderer* renderer)
{
    auto vulkanRenderer = Cast<VulkanRenderer>(renderer);
    VkCommandBuffer commandBuffer = vulkanRenderer->GetCommandBuffer();
    uint32_t frameIndex = vulkanRenderer->GetFrameIndex();

    if (!vulkanRenderer->BindShader(m_material->GetShader().getPtr()))
        return;

    m_material->SendAllValues(renderer);
    if (!vulkanRenderer->BindMaterial(m_material.getPtr()))
        return;

    auto vertex = Cast<VulkanVertexBuffer>(m_mesh->GetVertexBuffer());
    auto index = Cast<VulkanIndexBuffer>(m_mesh->GetIndexBuffer());

    VkBuffer vertexBuffers[] = {vertex->GetBuffer(), m_instanceBuffer->GetBuffer()};
    VkDeviceSize offsets[] = {0, 0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 2, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, index->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(commandBuffer, index->GetIndexCount(), static_cast<uint32_t>(m_particleCount), 0, 0, 0);
}

void ParticleSystemComponent::OnDestroy()
{
    auto vulkanRenderer = Cast<VulkanRenderer>(Engine::Get()->GetRenderer());
    if (vulkanRenderer)
        vkDeviceWaitIdle(vulkanRenderer->GetDevice()->GetDevice());
    
    // Clean up any pending buffers
    for (auto& buffer : m_buffersToCleanup)
    {
        if (buffer)
            buffer->Cleanup();
    }
    m_buffersToCleanup.clear();
    
    if (m_instanceBuffer)
        m_instanceBuffer->Cleanup();
    if (m_instanceStaging)
        m_instanceStaging->Cleanup();

}

void ParticleSystemComponent::SetParticleCount(int count)
{
    if (count <= 0)
        return;
    m_particleCount = count;
    
    auto instancingShader = m_material->GetShader();
    auto renderer = Engine::Get()->GetRenderer();
    instancingShader->EOnSentToGPU.Bind([this, renderer]()
    {
        auto vulkanRenderer = Cast<VulkanRenderer>(renderer);
        auto device = vulkanRenderer->GetDevice();
        VkDeviceSize size = sizeof(InstanceData) * static_cast<VkDeviceSize>(m_particleCount);
        
        // Move old buffers to cleanup list instead of destroying immediately
        if (m_instanceStaging || m_instanceBuffer)
        {
            if (m_instanceStaging)
                m_buffersToCleanup.push_back(std::move(m_instanceStaging));
            if (m_instanceBuffer)
                m_buffersToCleanup.push_back(std::move(m_instanceBuffer));
        }
        
        m_instanceStaging = std::make_unique<VulkanBuffer>();
        m_instanceStaging->Initialize(device, size,
                                  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        m_instanceBuffer = std::make_unique<VulkanBuffer>();
        m_instanceBuffer->Initialize(device, size,
                                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        
        m_positions.resize(m_particleCount);
        m_colors.resize(m_particleCount);
        
        float t = 0.01f;
        for (int i = 0; i < m_particleCount; i++) {
            m_positions[i] = Random::PointOnSphere(10.f);
            m_colors[i] = Color::FromHSV(t, 1.f, 1.f);
            
            t += (1.f / m_particleCount) * 360.f;
        }
    });
}

void ParticleSystemComponent::SetMesh(SafePtr<Mesh> mesh)
{
    m_mesh = std::move(mesh);
}
