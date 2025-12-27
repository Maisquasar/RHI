#pragma once
#include "EditorWindow.h"
#include "Core/UUID.h"
#include "Scene/GameObject.h"
#include "Utils/Event.h"

class ImGuiHandler;
class SceneHolder;

class Hierarchy : public EditorWindow
{
public:
    Hierarchy(Engine* engine, ImGuiHandler* handler);

    void OnRender() override;
    
    Event<Core::UUID> EOnObjectSelected;

private:
    void DisplayObject(GameObject* object, uint64_t& index, bool display = true);
private:
    SceneHolder* m_sceneHolder;
    
    Core::UUID m_selectedObject = UUID_INVALID;
    
    Core::UUID m_renameObject = UUID_INVALID;
    bool m_openRename;
};
