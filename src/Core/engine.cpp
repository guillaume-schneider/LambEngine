#include "engine.hpp"
#include "time.hpp"
#include "input.hpp"
#include "iostream"
#include "IGame.hpp"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "log.hpp"

void GLAPIENTRY openglDebugCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar *message,
                                    const void *userParam)
{
    std::cerr << "GL CALLBACK: ";
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        std::cerr << "** GL ERROR ** ";
    }

    std::cerr << "type = 0x" << std::hex << type
              << ", severity = 0x" << severity
              << ", message = " << message << std::endl;
}

void Engine::initSDL(const EngineConfig& cfg) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

#ifdef DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    m_Window = SDL_CreateWindow(
        cfg.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        cfg.width, cfg.height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!m_Window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return;
    }

    m_Context = SDL_GL_CreateContext(m_Window);
    if (!m_Context)
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
        return;
    }

    if (SDL_GL_MakeCurrent(m_Window, m_Context) != 0)
    {
        std::cerr << "SDL_GL_MakeCurrent::ERROR::" << SDL_GetError() << std::endl;
        SDL_GL_DeleteContext(m_Context);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
        return;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        SDL_GL_DeleteContext(m_Context);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
        return;
    }

#ifdef VSYNC
    SDL_GL_SetSwapInterval(1);
#endif

};


void Engine::initOpenGL() {
    #ifdef DEBUG
    if (gladLoadGL())
    { // Ensure GLAD loaded OpenGL
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        { // Check if debug context is active
            std::cout << "GL_CONTEXT::DEBUG::ACTIVATED" << std::endl;
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openglDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    }
    else
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
    }
#endif

    const GLubyte *version = glGetString(GL_VERSION);
    if (version)
    {
        std::cout << "OpenGL Version: " << version << std::endl;
    }
    else
    {
        std::cerr << "Unable to retrieve OpenGL version." << std::endl;
    }

    int currentWindowWidth, currentWindowHeight;
    SDL_GetWindowSize(m_Window, &currentWindowWidth, &currentWindowHeight);
    float currrentWindowRatio = static_cast<float>(currentWindowWidth) / currentWindowHeight;

    glViewport(0, 0, currentWindowWidth, currentWindowHeight);
}

void Engine::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_Window, m_Context);
    ImGui_ImplOpenGL3_Init();
}


void Engine::shutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Engine::shutdownSDL() {
    if (m_Context) SDL_GL_DeleteContext(m_Context);
    if (m_Window) SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

Engine::Engine(const EngineConfig& cfg) {
    m_Config = cfg;

    initSDL(m_Config);
    initOpenGL();
    initImGui();
}

void Engine::Run(IGame* game)
{
    if (!game) {
        Logger::Log(LogLevel::Error, "Engine::Run received null IGame.");
        throw std::runtime_error("Engine::Run received null IGame.");
    }

    Logger::Log(LogLevel::Info, "Engine::Run() starting");
    Logger::Log(LogLevel::Info, "Calling game->OnInit()");

    game->OnInit(*this);

    bool running = true;
    int frameCount = 0;

    while (running)
    {
        InputSystem::getInstance()->update(m_Window);
        if (InputSystem::getInstance()->shouldStop()) {
            Logger::Log(LogLevel::Info, "InputSystem requested stop, leaving main loop");
            running = false;
            break;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        Time::getInstance().computeDeltaTime();
        float dt = Time::getInstance().getDeltaTime();

        InputSystem::getInstance()->update(m_Window);

        game->OnUpdate(*this, dt);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glBindVertexArray(0);

        game->OnRender(*this);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(m_Window);
    }

    Logger::Log(LogLevel::Info, "Engine::Run() exiting main loop");
}

Engine::~Engine() {
    shutdownImGui();
    shutdownSDL();
    std::cout << "Engine shutdown.";
}
