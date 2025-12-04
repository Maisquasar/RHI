#include "VulkanShaderBuffer.h"

#include "Debug/Log.h"

bool VulkanShaderBuffer::Initialize(VulkanDevice* device, const std::string& code)
{
    m_device = device;
    if (code.empty() || (code.size() % 4) != 0)
    {
        PrintError("Invalid SPIR-V code size");
        return false;
    }

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule = VK_NULL_HANDLE;
    VkResult result = vkCreateShaderModule(device->GetDevice(), &createInfo, nullptr, &shaderModule);
    if (result != VK_SUCCESS)
    {
        PrintError("Failed to create shader module. VkResult: %s", std::to_string(result).c_str()); 
        return false;
    }

    m_module = shaderModule;
    return true;
}

void VulkanShaderBuffer::CleanUp()
{
    if (m_module != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(m_device->GetDevice(), m_module, nullptr);
        m_module = VK_NULL_HANDLE;
    }
}
