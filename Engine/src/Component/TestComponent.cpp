#include "TestComponent.h"

#include "TransformComponent.h"
#include "Scene/GameObject.h"
void TestComponent::OnUpdate(float deltaTime)
{
    auto transform = GetGameObject()->GetComponent<TransformComponent>();
    
    auto rotation = transform->GetLocalRotation();
    transform->SetLocalRotation(rotation * Quat::AngleAxis(deltaTime * 60.f, Vec3f::Up()));
}
