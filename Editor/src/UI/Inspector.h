#pragma once
#include "EditorWindow.h"
#include "Core/UUID.h"
#include "Scene/ComponentDescriptor.h"
#include "Utils/Type.h"

class SceneHolder;

class Inspector : public EditorWindow
{
public:
    Inspector(Engine* engine);
    
    void OnRender() override;
    
    void SetSelectedObject(Core::UUID uuid);

    void ShowComponent(const ComponentDescriptor& descriptor) const;
private:
    SceneHolder* m_sceneHolder;
    
    Core::UUID m_selectedObject;
};
