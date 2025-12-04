#include <iostream>

#include "Core/Engine.h"
#include "Core/Window.h"
#include "Debug/Log.h"

#include "Render/RHI/RHIRenderer.h"
#include "Render/Vulkan/VulkanRenderer.h"
#include "Resource/Mesh.h"

#include "Resource/ResourceManager.h"
#include "Resource/Model.h"
#include "Resource/Texture.h"

#include "Utils/Type.h"

int Run(int argc, char** argv, char** envp)
{
    (void)argc;
    (void)argv;
    (void)envp;

    Engine engine;
    engine.Initialize();
    
    engine.Run();
    
    engine.Cleanup();

    return 0;
}


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

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