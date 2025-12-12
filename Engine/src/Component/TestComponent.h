#pragma once
#include "IComponent.h"

class TestComponent : public IComponent
{
public:
    using IComponent::IComponent;
    
    void OnUpdate(float deltaTime) override;
};
