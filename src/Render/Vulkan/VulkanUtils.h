#pragma once
#ifdef RENDER_API_VULKAN
#include <vulkan/vulkan_core.h>

#include "Render/RHI/RHIVertex.h"

namespace VulkanUtils
{
    inline VkVertexInputRate To(RHIVertexInputRate in)
    {
        switch (in)
        {
            case RHIVertexInputRate::VERTEX_INPUT_RATE_VERTEX:
                return VK_VERTEX_INPUT_RATE_VERTEX;
            case RHIVertexInputRate::VERTEX_INPUT_RATE_INSTANCE:
                return VK_VERTEX_INPUT_RATE_INSTANCE;
            case RHIVertexInputRate::VERTEX_INPUT_RATE_MAX_ENUM:
                return VK_VERTEX_INPUT_RATE_MAX_ENUM;
            default:
                return VK_VERTEX_INPUT_RATE_VERTEX;
        }
    }

    inline VkVertexInputBindingDescription To(RHIBindingDescription in)
    {
        VkVertexInputBindingDescription out;
        out.binding = in.binding;
        out.stride = in.stride;
        out.inputRate = To(in.inputRate);
        return out;
    }

    inline VkFormat To(RHIFormat in)
    {
        switch (in)
        {
            case RHIFormat::R32_F:
                return VK_FORMAT_R32_SFLOAT;
            case RHIFormat::R32G32_F:
                return VK_FORMAT_R32G32_SFLOAT;
            case RHIFormat::R32G32B32_F:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case RHIFormat::R32G32B32A32_F:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            default:
                return VK_FORMAT_R32_SFLOAT;
        }
    }

    inline VkVertexInputAttributeDescription To(RHIVertexInputAttributeDescription in)
    {
        VkVertexInputAttributeDescription out;
        out.location = in.location;
        out.binding = in.binding;
        out.format = To(in.format);
        out.offset = in.offset;
        return out;
    }
}
#endif