#pragma once
#ifdef RENDER_API_VULKAN

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <array>
#include <galaxymath/Maths.h>

#include "Render/RHI/RHIVertex.h"

// Simple vertex structure
struct Vertex
{
    Vec3f position;
    Vec2f texCoord;
    Vec3f normal;
    Vec3f tangent;

    static RHIBindingDescription GetBindingDescription()
    {
        RHIBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = RHIVertexInputRate::VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<RHIVertexInputAttributeDescription, 4> GetAttributeDescriptions()
    {
        std::array<RHIVertexInputAttributeDescription, 4> attributeDescriptions{};

        // Position
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = RHIFormat::R32G32B32_F;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        // Texture coordinates
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 2;
        attributeDescriptions[1].format = RHIFormat::R32G32_F;
        attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

        // Normal
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 3;
        attributeDescriptions[2].format = RHIFormat::R32G32B32_F;
        attributeDescriptions[2].offset = offsetof(Vertex, normal);
        
        // Tangent
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 4;
        attributeDescriptions[3].format = RHIFormat::R32G32B32_F;
        attributeDescriptions[3].offset = offsetof(Vertex, tangent);

        return attributeDescriptions;
    }
};

class VulkanDevice;
class VulkanBuffer;

class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    bool Initialize(VulkanDevice* device, const std::vector<Vertex>& vertices,
                   const std::vector<uint32_t>& indices);
    void Cleanup();

    void Bind(VkCommandBuffer commandBuffer);
    void Draw(VkCommandBuffer commandBuffer);

    uint32_t GetVertexCount() const { return static_cast<uint32_t>(m_vertices.size()); }
    uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_indices.size()); }

    // Helper to create simple shapes
    static Mesh* CreateTriangle(VulkanDevice* device);
    static Mesh* CreateQuad(VulkanDevice* device);
    static Mesh* CreateCube(VulkanDevice* device);

private:
    bool CreateVertexBuffer();
    bool CreateIndexBuffer();
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    VulkanDevice* m_device = nullptr;
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    std::unique_ptr<VulkanBuffer> m_vertexBuffer;
    std::unique_ptr<VulkanBuffer> m_indexBuffer;
};

#endif