#pragma once
#ifdef RENDER_API_VULKAN

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

struct DescriptorBindingInfo
{
    uint32_t binding;
    VkDescriptorType type;
    VkShaderStageFlags stageFlags;
    uint32_t descriptorCount = 1;
};

class VulkanDescriptorSetLayout
{
public:
    VulkanDescriptorSetLayout(VkDevice device)
        : m_device(device), m_layout(VK_NULL_HANDLE) {}

    ~VulkanDescriptorSetLayout();

    void Create(const std::vector<DescriptorBindingInfo>& bindingsInfo);

    void Cleanup();

    VkDescriptorSetLayout GetLayout() const { return m_layout; }

private:
    VkDevice m_device;
    VkDescriptorSetLayout m_layout;
    std::vector<VkDescriptorSetLayoutBinding> m_bindings;
};

#endif
