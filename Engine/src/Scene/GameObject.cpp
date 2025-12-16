#include "GameObject.h"

#include <utility>

GameObject::GameObject(Scene& scene): m_scene(scene)
{
    m_transform = AddComponent<TransformComponent>();
}

std::vector<SafePtr<IComponent>> GameObject::GetComponents() const
{
    return m_scene.GetComponents(this);
}

void GameObject::RemoveComponent(Core::UUID compId) const
{
    m_scene.RemoveComponent(std::move(compId));
}

bool GameObject::HasParent() const
{
    return m_parentUUID != UUID_INVALID;
}

SafePtr<GameObject> GameObject::GetParent() const
{
    if (!HasParent())
        return {};
    return m_scene.GetGameObject(m_parentUUID);
}

std::vector<SafePtr<GameObject>> GameObject::GetChildren() const
{
    std::vector<SafePtr<GameObject>> children;
    children.reserve(m_childrenUUID.size());
    for (auto& childUUID : m_childrenUUID)
    {
        children.push_back(m_scene.GetGameObject(childUUID));
    }
    return children;
}
