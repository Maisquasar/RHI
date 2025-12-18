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
    void ShowMaterials(const Property& property) const;
    void ShowMesh(const Property& property) const;
    void ShowTransform(const Property& property) const;
    
private:
    SceneHolder* m_sceneHolder;
    
    Core::UUID m_selectedObject;
};
