#pragma once
#ifdef RENDER_API_VULKAN
#include <vector>
#include <vulkan/vulkan.h>

class VulkanCommandBuffer;
class VulkanDevice;

class VulkanDepthBuffer
{
public:
    VulkanDepthBuffer() = default;
    ~VulkanDepthBuffer();

    bool Initialize(VulkanDevice* device, VkExtent2D extent);
    void Cleanup();

    VkImageView GetImageView() const { return m_depthImageView; }
    VkFormat GetDepthFormat() const { return m_depthFormat; }

private:
    VkFormat FindDepthFormat(VulkanDevice* device);
    VkFormat FindSupportedFormat(VulkanDevice* device,
                                 const std::vector<VkFormat>& candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features);
    
    bool HasStencilComponent(VkFormat format);
    
    void CreateImage(VulkanDevice* device, uint32_t width, uint32_t height,
                    VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkImage& image,
                    VkDeviceMemory& imageMemory);
    
    VkImageView CreateImageView(VulkanDevice* device, VkImage image,
                                VkFormat format, VkImageAspectFlags aspectFlags);
    
    void TransitionImageLayout(VkCommandPool cmdPool, VkImage image, VkFormat format,
                               VkImageLayout oldLayout, VkImageLayout newLayout);

private:
    VulkanDevice* m_device = nullptr;
    VkImage m_depthImage = VK_NULL_HANDLE;
    VkDeviceMemory m_depthImageMemory = VK_NULL_HANDLE;
    VkImageView m_depthImageView = VK_NULL_HANDLE;
    VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;
};
#endif