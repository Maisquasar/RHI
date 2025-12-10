#include "MeshComponent.h"

#include "Core/Engine.h"
#include "Render/Vulkan/VulkanRenderer.h"

void MeshComponent::OnUpdate(float deltaTime)
{
    RHIRenderer* renderer = Engine::Get()->GetRenderer();
    
    Vec2i windowSize = Engine::Get()->GetWindow()->GetSize();
    
    Vec3f camPos = Vec3f(2.0f, 2.0f, 2.0f);
    Vec3f camTarget = Vec3f(0.0f, 0.0f, 0.0f);
    Vec3f camUp = Vec3f(0.0f, 1.0f, 0.0f);
    auto view = Mat4::LookAtRH(camPos, camTarget, camUp);

    auto projection = Mat4::CreateProjectionMatrix(
        45.f, (float)windowSize.x / (float)windowSize.y, 0.1f, 10.0f);
    projection[1][1] *= -1;
    Mat4 VP = projection * view;

    for (auto& material : m_materials)
    {
        {
            Uniform uniform = material->GetShader()->GetUniform("ubo");
            auto binding = UBOBinding(uniform.set, uniform.binding);
            material->GetShader()->SendValue(binding, &VP, sizeof(VP), renderer);
        }
        {
            Vec4f color = Vec4f::One();
            Uniform uniform = material->GetShader()->GetUniform("material");
            auto binding = UBOBinding(uniform.set, uniform.binding);
            material->GetShader()->SendValue(binding, &color, sizeof(color), renderer);
        }
    }
}

void MeshComponent::OnRender(RHIRenderer* renderer)
{
    if (!m_mesh || !m_mesh->IsLoaded() || !m_mesh->SentToGPU() || !m_mesh->GetVertexBuffer() || !m_mesh->GetIndexBuffer())
        return;
    
    float distanceInFront = 5.f;
    Vec3f camPos = Vec3f(2.0f, 2.0f, 2.0f);
    Vec3f camTarget = Vec3f(0.0f, 0.0f, 0.0f);
    Vec3f forward = Vec3f::Normalize(camTarget - camPos);
    Vec3f cubePosition = camPos + forward * distanceInFront;

    float angle = 90.f;
    Mat4 model = Mat4::CreateTransformMatrix(cubePosition, Vec3f(0.f, angle, 0.f), Vec3f(1.f, 1.f, 1.f));
    
    for (auto& material : m_materials)
    {
        auto shader = material->GetShader().get().get();
        renderer->BindShader(shader);

        renderer->BindVertexBuffers(m_mesh->GetVertexBuffer(), m_mesh->GetIndexBuffer());

        PushConstant pushConstant = shader->GetPushConstants()[ShaderType::Vertex];
        renderer->SendPushConstants(&model, sizeof(model), shader, pushConstant);
        
        renderer->DrawVertex(m_mesh->GetVertexBuffer(), m_mesh->GetIndexBuffer());
    }
}

void MeshComponent::SetMesh(const SafePtr<Mesh>& mesh)
{
    m_mesh = mesh;
}

void MeshComponent::AddMaterial(const SafePtr<Material>& material)
{
    m_materials.push_back(material);
}
