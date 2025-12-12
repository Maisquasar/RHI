#pragma once
#ifdef RENDER_API_VULKAN

#include "VulkanMaterial.h"
#include "VulkanPipeline.h"
#include "VulkanDevice.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorPool.h"
#include "VulkanTexture.h"
#include "Resource/Texture.h"
#include "Debug/Log.h"

VulkanMaterial::VulkanMaterial(VulkanPipeline* pipeline)
    : m_pipeline(pipeline)
    , m_device(pipeline->GetDevice())
    , m_maxFramesInFlight(pipeline->GetMaxFramesInFlight())
{
    // Copy uniform information from pipeline
    m_uniformsBySet = pipeline->GetUniformsBySet();
}

VulkanMaterial::~VulkanMaterial()
{
    Cleanup();
}

bool VulkanMaterial::Initialize(uint32_t maxFramesInFlight, Texture* defaultTexture, VulkanPipeline* pipeline)
{
    try
    {
        auto descriptorTypeCounts = pipeline->GetDescriptorTypeCounts();
        auto descriptorSetLayouts = pipeline->GetDescriptorSetLayouts();
        
        // --- Create Shared Descriptor Pool ---
        std::vector<VkDescriptorPoolSize> poolSizes;
        poolSizes.reserve(descriptorTypeCounts.size());
        for (const auto& [type, count] : descriptorTypeCounts)
        {
            poolSizes.push_back({
                .type = type, 
                .descriptorCount = count * maxFramesInFlight
            }); 
        }

        m_descriptorPool = std::make_unique<VulkanDescriptorPool>();
        if (!m_descriptorPool->Initialize(m_device, poolSizes, 
            static_cast<uint32_t>(maxFramesInFlight * descriptorSetLayouts.size())))
        {
            PrintError("Failed to initialize shared descriptor pool!");
            return false;
        }
        
        // --- Create Uniform Buffers ---
        // Iterate through all uniforms and create buffers for UBOs
        for (const auto& [set, uniforms] : m_uniformsBySet)
        {
            for (const auto& uniform : uniforms)
            {
                if (uniform.type == UniformType::NestedStruct)
                {
                    UBOBinding key = {uniform.set, uniform.binding};
                    
                    auto ubo = std::make_unique<VulkanUniformBuffer>();
                    if (!ubo->Initialize(m_device, uniform.size, m_maxFramesInFlight))
                    {
                        PrintError("Failed to initialize uniform buffer for set %u binding %u", 
                                   uniform.set, uniform.binding);
                        return false;
                    }
                    ubo->MapAll();
                    
                    m_uniformBuffers[key] = std::move(ubo);
                }
            }
        }

        // --- Allocate Descriptor Sets ---
        const auto& layouts = m_pipeline->GetDescriptorSetLayouts();
        m_descriptorSets.reserve(layouts.size());

        for (size_t i = 0; i < layouts.size(); ++i)
        {
            auto descriptorSet = std::make_unique<VulkanDescriptorSet>();
            if (!descriptorSet->Initialize(m_device, 
                                          m_descriptorPool->GetPool(),
                                          layouts[i]->GetLayout(),
                                          m_maxFramesInFlight))
            {
                PrintError("Failed to initialize descriptor set %zu", i);
                return false;
            }
            m_descriptorSets.push_back(std::move(descriptorSet));
        }

        // --- Update Descriptor Sets with Default Values ---
        for (uint32_t frameIdx = 0; frameIdx < m_maxFramesInFlight; ++frameIdx)
        {
            for (const auto& [set, uniforms] : m_uniformsBySet)
            {
                std::vector<VkWriteDescriptorSet> writes;

                for (const auto& uniform : uniforms)
                {
                    VkWriteDescriptorSet write{};
                    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    write.dstSet = m_descriptorSets[set]->GetDescriptorSet(frameIdx);
                    write.dstBinding = uniform.binding;
                    write.dstArrayElement = 0;
                    write.descriptorCount = 1;

                    if (uniform.type == UniformType::NestedStruct)
                    {
                        // Uniform buffer
                        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        
                        UBOBinding key = {uniform.set, uniform.binding};
                        auto* ubo = m_uniformBuffers[key].get();
                        
                        VkDescriptorBufferInfo* bufferInfo = new VkDescriptorBufferInfo{};
                        bufferInfo->buffer = ubo->GetBuffer(frameIdx);
                        bufferInfo->offset = 0;
                        bufferInfo->range = uniform.size;
                        
                        write.pBufferInfo = bufferInfo;
                    }
                    else if (uniform.type == UniformType::Sampler2D || 
                             uniform.type == UniformType::SamplerCube)
                    {
                        // Texture sampler
                        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                        
                        VkDescriptorImageInfo* imageInfo = new VkDescriptorImageInfo{};
                        imageInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        
                        // Use default texture initially
                        if (defaultTexture)
                        {
                            auto* vulkanTexture = dynamic_cast<VulkanTexture*>(defaultTexture);
                            imageInfo->imageView = vulkanTexture->GetImageView();
                            imageInfo->sampler = vulkanTexture->GetSampler();
                        }
                        
                        write.pImageInfo = imageInfo;
                    }

                    writes.push_back(write);
                }

                // Perform batch update
                vkUpdateDescriptorSets(m_device->GetDevice(), 
                                      static_cast<uint32_t>(writes.size()), 
                                      writes.data(), 
                                      0, nullptr);

                // Clean up allocated info structures
                for (auto& write : writes)
                {
                    if (write.pBufferInfo) delete write.pBufferInfo;
                    if (write.pImageInfo) delete write.pImageInfo;
                }
            }
        }

        return true;
    }
    catch (const std::exception& e)
    {
        PrintError("VulkanMaterial initialization failed: %s", e.what());
        Cleanup();
        return false;
    }
}

void VulkanMaterial::Cleanup()
{
    m_descriptorSets.clear();
    m_uniformBuffers.clear();
}

void VulkanMaterial::SetUniformData(uint32_t set, uint32_t binding, const void* data, 
                                   size_t size, uint32_t frameIndex)
{
    UBOBinding key = {set, binding};
    auto it = m_uniformBuffers.find(key);
    
    if (it != m_uniformBuffers.end())
    {
        it->second->UpdateData(data, size, frameIndex);
    }
    else
    {
        PrintError("Uniform buffer not found for set %u binding %u", set, binding);
    }
}

void VulkanMaterial::SetTexture(uint32_t set, uint32_t binding, Texture* texture, uint32_t frameIndex)
{
    if (!texture || set >= m_descriptorSets.size())
    {
        PrintError("Invalid texture or set index");
        return;
    }

    auto* vulkanTexture = dynamic_cast<VulkanTexture*>(texture);
    
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = vulkanTexture->GetImageView();
    imageInfo.sampler = vulkanTexture->GetSampler();

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = m_descriptorSets[set]->GetDescriptorSet(frameIndex);
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.descriptorCount = 1;
    write.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(m_device->GetDevice(), 1, &write, 0, nullptr);
}

void VulkanMaterial::BindDescriptorSets(VkCommandBuffer commandBuffer, uint32_t frameIndex)
{
    std::vector<VkDescriptorSet> sets;
    sets.reserve(m_descriptorSets.size());
    
    for (const auto& descriptorSet : m_descriptorSets)
    {
        sets.push_back(descriptorSet->GetDescriptorSet(frameIndex));
    }

    vkCmdBindDescriptorSets(commandBuffer, 
                           VK_PIPELINE_BIND_POINT_GRAPHICS,
                           m_pipeline->GetPipelineLayout(),
                           0, // First set
                           static_cast<uint32_t>(sets.size()),
                           sets.data(),
                           0, nullptr);
}

VulkanUniformBuffer* VulkanMaterial::GetUniformBuffer(uint32_t set, uint32_t binding) const
{
    UBOBinding key = {set, binding};
    auto it = m_uniformBuffers.find(key);
    return (it != m_uniformBuffers.end()) ? it->second.get() : nullptr;
}

VulkanDescriptorSet* VulkanMaterial::GetDescriptorSet(uint32_t set) const
{
    return (set < m_descriptorSets.size()) ? m_descriptorSets[set].get() : nullptr;
}

#endif