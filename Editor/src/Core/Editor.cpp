#include "Editor.h"

#include "Core/Engine.h"

Editor::Editor()
{
}

void Editor::Initialize()
{
    WindowConfig config;
    config.title = "Window Test";
    config.size = Vec2i(1280, 720);
    config.attributes = static_cast<WindowAttributes>(VSync);
    m_window = Window::Create(WindowAPI::GLFW, RenderAPI::Vulkan, config);
    
    EngineDesc desc = {
        .window = m_window.get(),
    };
    
    m_engine = Engine::Create();
    m_engine->Initialize(desc);
    m_imguiHandler = std::make_unique<ImGuiHandler>();
    m_imguiHandler->Initialize(m_window.get(), m_engine->GetRenderer());
    //
    // m_engine->OnRender += [&]()
    // {
    //     m_imguiHandler->BeginFrame();
    //     OnRender();
    // };
    //
    // m_engine->OnEndFrame += [&]()
    // {
    //     m_imguiHandler->EndFrame();
    // };
    //
    // m_engine->OnCleanup += [&]()
    // {
    //     m_imguiHandler->Cleanup();
    // };
}

void Editor::Run()
{
    // m_engine->Run();
    
    while (!m_window->ShouldClose())
    {
        m_window->PollEvents();
        
        if (!m_engine->BeginFrame())
            continue;
        
        m_engine->Update();
        
        m_engine->Render();
        
        m_imguiHandler->BeginFrame();
        OnRender();
        m_imguiHandler->EndFrame();
        
        m_engine->EndFrame();
    }
}

void Editor::OnRender()
{
    ImGui::ShowDemoWindow();
}

void Editor::Cleanup()
{
    m_imguiHandler->Cleanup();
    m_engine->Cleanup();
}
