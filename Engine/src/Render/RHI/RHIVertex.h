#pragma once
#include <cstdint>

enum class RHIVertexInputRate
{
    VERTEX_INPUT_RATE_VERTEX = 0,
    VERTEX_INPUT_RATE_INSTANCE = 1,
    VERTEX_INPUT_RATE_MAX_ENUM
};

struct RHIBindingDescription
{
    uint32_t binding;
    uint32_t stride;
    RHIVertexInputRate inputRate;
};

enum class RHIFormat
{
    R32_F,
    R32G32_F,
    R32G32B32_F,
    R32G32B32A32_F,
};

struct RHIVertexInputAttributeDescription
{
    uint32_t location;
    uint32_t binding;
    RHIFormat format;
    uint32_t offset;
};
