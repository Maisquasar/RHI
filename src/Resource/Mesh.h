#pragma once
#ifdef RENDER_API_VULKAN

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <array>

// Simple vertex structure
struct Vertex
{
    float position[3];
    float color[3];
    float texCoord[2];
    float normal[3];

    static VkVertexInputBindingDescription GetBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        // Position
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        // Color
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        // Texture coordinates
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        // Normal
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, normal);

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