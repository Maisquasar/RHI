#include <iostream>

#include "Core/Window.h"
#include "Render/RHI/RHIRenderer.h"
#include "Resource/Model.h"
#include "Resource/ResourceManager.h"
#include "Utils/Type.h"

int Run(int argc, char** argv, char** envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
    
    WindowConfig config;
    config.title = "Window Test";
    config.size = Vec2i(1920, 1080);
    std::unique_ptr<Window> window = Window::Create(WindowAPI::SDL, RenderAPI::Vulkan, config);

    if (!window)
    {
        std::cerr << "Failed to create window" << '\n';
        return -1;
    }

    std::unique_ptr<RHIRenderer> renderer = RHIRenderer::Create(RenderAPI::Vulkan, window.get());
    if (!renderer)
    {
        std::cerr << "Failed to create renderer" << '\n';
        return -1;
    }

    ThreadPool::Initialize();
    ResourceManager resourceManager;
    SafePtr<Model> cubeModel = resourceManager.Load<Model>("resources/models/Cube.obj");

    window->SetVSync(true);
    window->SetMouseCursorType(CursorType::Hand);

    while (!window->ShouldClose())
    {
        window->PollEvents();

        renderer->DrawFrame();
    }

    ThreadPool::Terminate();
    
    window->Terminate();

    return 0;
}

#if defined(_WIN32) && defined(NDEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
#else
int main(int argc, char** argv, char** envp)
#endif
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // TODO: Remove Comments To Break on leaks
    // |
    // V
    // _CrtSetBreakAlloc(173);
#ifdef NDEBUG
    int argc = __argc;
    char** argv = __argv;
    char** envp = nullptr;
#endif
#endif
    return Run(argc, argv, envp);
}
