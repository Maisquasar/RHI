#pragma once
#include "EngineAPI.h"
#include <memory>

#include "Resource/ResourceManager.h"
#include "Core/Window.h"
#include "Render/RHI/RHIRenderer.h"
#include "Scene/ComponentHandler.h"
#include "Scene/Scene.h"

#include "Utils/Event.h"

struct ENGINE_API EngineDesc
{
    Window* window;
};

class ENGINE_API Engine
{
public:
    static Engine* Create();
    
    bool Initialize(EngineDesc desc);
    bool BeginFrame();
    void Update();
    void Render();
    void EndFrame();
    // void Run();
    void Cleanup();
    
    static Engine* Get();

    Window* GetWindow() const { return m_window; }
    RHIRenderer* GetRenderer() const { return m_renderer.get(); }

    Event<> OnRender;
    Event<> OnCleanup;
    Event<> OnEndFrame;
private:
    Window* m_window;
    std::unique_ptr<RHIRenderer> m_renderer;
    std::unique_ptr<ResourceManager> m_resourceManager;
    std::unique_ptr<ComponentRegister> m_componentRegister;
    std::unique_ptr<Scene> m_scene;
    
    inline static std::unique_ptr<Engine> s_instance = nullptr;
    
    float m_deltaTime = 0.0f;
};
