#include "VulkanDescriptorSetLayout.h"

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
    Cleanup();
}

void VulkanDescriptorSetLayout::Create(const std::vector<DescriptorBindingInfo>& bindingsInfo)
{
    Cleanup();

    m_bindings.clear();
    for (auto& info : bindingsInfo)
    {
        VkDescriptorSetLayoutBinding binding{};
        binding.binding = info.binding;
        binding.descriptorType = info.type;
        binding.descriptorCount = info.descriptorCount;
        binding.stageFlags = info.stageFlags;
        binding.pImmutableSamplers = nullptr;
        m_bindings.push_back(binding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(m_bindings.size());
    layoutInfo.pBindings = m_bindings.data();

    if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor set layout!");
    }
}

void VulkanDescriptorSetLayout::Cleanup()
{
    if (m_layout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
        m_layout = VK_NULL_HANDLE;
    }
    m_bindings.clear();
}
