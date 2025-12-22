#pragma once
#include "IComponent.h"

class ParticleSystemComponent : public IComponent
{
public:
    DECLARE_COMPONENT_TYPE(ParticleSystemComponent);
    
    void OnUpdate(float deltaTime) override;
    
};
