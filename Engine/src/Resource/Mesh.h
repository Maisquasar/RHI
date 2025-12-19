#pragma once
#include <vector>
#include <memory>
#include <array>

#include <galaxymath/Maths.h>

#include "IResource.h"
#include "Physic/BoundingBox.h"

#include "Render/RHI/RHIVertex.h"
#include "Render/RHI/RHIVertexBuffer.h"
#include "Render/RHI/RHIIndexBuffer.h"

struct Vertex
{
    Vec3f position;
    Vec2f texCoord;
    Vec3f normal;
    Vec3f tangent;

    static RHIBindingDescription GetBindingDescription();

    static std::array<RHIVertexInputAttributeDescription, 4> GetAttributeDescriptions();
};

struct SubMesh
{
    uint32_t startIndex;
    uint32_t count;
};

class Mesh : public IResource
{
public:
    DECLARE_RESOURCE_TYPE(Mesh)

    bool Load(ResourceManager* resourceManager) override;
    bool SendToGPU(RHIRenderer* renderer) override;
    void Unload() override;
    
    std::string GetName(bool extension = true) const override;

    RHIVertexBuffer* GetVertexBuffer() const { return m_vertexBuffer.get(); }
    RHIIndexBuffer* GetIndexBuffer() const { return m_indexBuffer.get(); }
    
    const std::vector<SubMesh>& GetSubMeshes() const { return m_subMeshes; }
    
    const BoundingBox& GetBoundingBox() const { return m_boundingBox; }
private:
    void ComputeBoundingBox(const std::vector<Vec3f>& positionVertices);
private:
    friend class Model;
    
    std::vector<float> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<SubMesh> m_subMeshes;

    std::unique_ptr<RHIVertexBuffer> m_vertexBuffer;
    std::unique_ptr<RHIIndexBuffer> m_indexBuffer;
    
    BoundingBox m_boundingBox;
};