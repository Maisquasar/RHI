#include "Model.h"

#include "Mesh.h"
#include "ResourceManager.h"
#include "Loader/OBJLoader.h"

bool Model::Load(ResourceManager* resourceManager)
{
    if (p_path.extension() == ".obj")
    {
        OBJLoader::Model model;
        if (!OBJLoader::Load(p_path, model))
        {
            throw std::runtime_error("Failed to load model");
            return false;
        }
        
        for (auto& mesh : model.meshes)
        {
            SafePtr<Mesh> meshResource = resourceManager->AddResource(std::make_shared<Mesh>(p_path / mesh.name));
            
            this->m_meshes.push_back(meshResource.get().get());
            
            for (const Vec3i& idx : mesh.indices)
            {
                int posIdx = idx.x;
                int uvIdx = idx.y;
                int normIdx = idx.z;


                Vertex v{};


                if (posIdx >= 0 && static_cast<size_t>(posIdx) < mesh.positions.size())
                    v.position = mesh.positions[static_cast<size_t>(posIdx)];
                else
                    v.position = Vec3f(0.0f, 0.0f, 0.0f);


                if (uvIdx >= 0 && static_cast<size_t>(uvIdx) < mesh.textureUVs.size())
                    v.texCoord = mesh.textureUVs[static_cast<size_t>(uvIdx)];
                else
                    v.texCoord = Vec2f(0.0f, 0.0f);


                if (normIdx >= 0 && static_cast<size_t>(normIdx) < mesh.normals.size())
                    v.normal = mesh.normals[static_cast<size_t>(normIdx)];
                else
                    v.normal = Vec3f(0.0f, 0.0f, 0.0f);


                if (posIdx >= 0 && static_cast<size_t>(posIdx) < mesh.tangents.size())
                    v.tangent = mesh.tangents[static_cast<size_t>(posIdx)];
                else
                    v.tangent = Vec3f(0.0f, 0.0f, 0.0f);
                
                meshResource->m_vertices.push_back(v);
                
                meshResource->m_indices.push_back(idx.x);
                meshResource->m_indices.push_back(idx.y);
                meshResource->m_indices.push_back(idx.z);
            }
        }
        return true;
    }
    else
    {
        throw std::runtime_error("Unsupported file format" + p_path.extension().generic_string());
        return false;
    }
}

bool Model::SendToGPU(RHIRenderer* renderer)
{
    return true;
}

void Model::Unload()
{
}
