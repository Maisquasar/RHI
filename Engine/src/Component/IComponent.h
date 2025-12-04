#pragma once
#include "Core/UUID.h"

class IComponent
{
public:
    IComponent() = default;
    IComponent& operator=(const IComponent& other) = default;
    IComponent(const IComponent&) = default;
    IComponent(IComponent&&) noexcept = default;
    virtual ~IComponent() = default;
    
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnRender() = 0;
    
    UUID GetUUID() const { return m_uuid; }
private:
    UUID m_uuid;
};
