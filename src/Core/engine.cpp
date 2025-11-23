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

void Engine::initSDL(const EngineConfig& cfg)
{
    Logger::Log(LogLevel::Info, "Initializing SDL...", "Engine");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        Logger::Log(LogLevel::Error,
            std::string("Failed to initialize SDL: ") + SDL_GetError(), "Engine");
        throw std::runtime_error("SDL_Init failed");
    }

#ifdef DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    m_Window = SDL_CreateWindow(
        cfg.title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        cfg.width, cfg.height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (!m_Window)
    {
        Logger::Log(LogLevel::Error,
            std::string("Failed to create window: ") + SDL_GetError(), "Engine");
        SDL_Quit();
        throw std::runtime_error("SDL_CreateWindow failed");
    }

    m_Context = SDL_GL_CreateContext(m_Window);
    if (!m_Context)
    {
        Logger::Log(LogLevel::Error,
            std::string("Failed to create OpenGL context: ") + SDL_GetError(), "Engine");
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
        throw std::runtime_error("SDL_GL_CreateContext failed");
    }

    if (SDL_GL_MakeCurrent(m_Window, m_Context) != 0)
    {
        Logger::Log(LogLevel::Error,
            std::string("SDL_GL_MakeCurrent error: ") + SDL_GetError(), "Engine");
        SDL_GL_DeleteContext(m_Context);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
        throw std::runtime_error("SDL_GL_MakeCurrent failed");
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        Logger::Log(LogLevel::Error,
            "Failed to initialize OpenGL context with gladLoadGLLoader", "Engine");
        SDL_GL_DeleteContext(m_Context);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
        throw std::runtime_error("gladLoadGLLoader failed");
    }

#ifdef VSYNC
    SDL_GL_SetSwapInterval(1);
    Logger::Log(LogLevel::Info, "VSYNC enabled (SDL_GL_SetSwapInterval(1))", "Engine");
#else
    Logger::Log(LogLevel::Info, "VSYNC disabled", "Engine");
#endif

    Logger::Log(LogLevel::Info, "SDL initialization successful.", "Engine");
}



void Engine::initOpenGL()
{
    Logger::Log(LogLevel::Info, "Initializing OpenGL state...", "Engine");

#ifdef DEBUG
    if (gladLoadGL())
    {
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            Logger::Log(LogLevel::Info, "GL_CONTEXT::DEBUG::ACTIVATED", "Engine");
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openglDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                                  GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    }
    else
    {
        Logger::Log(LogLevel::Error, "Failed to initialize GLAD in initOpenGL()", "Engine");
    }
#endif

    const GLubyte *version = glGetString(GL_VERSION);
    if (version)
    {
        Logger::Log(
            LogLevel::Info,
            std::string("OpenGL Version: ") +
                reinterpret_cast<const char*>(version), "Engine");
    }
    else
    {
        Logger::Log(LogLevel::Warning, "Unable to retrieve OpenGL version.", "Engine");
    }

    int currentWindowWidth, currentWindowHeight;
    SDL_GetWindowSize(m_Window, &currentWindowWidth, &currentWindowHeight);
    m_AspectRatio = static_cast<float>(currentWindowWidth) /
                    static_cast<float>(currentWindowHeight);

    glViewport(0, 0, currentWindowWidth, currentWindowHeight);

    Logger::Log(
        LogLevel::Info,
        "OpenGL viewport initialized: " +
            std::to_string(currentWindowWidth) + "x" +
            std::to_string(currentWindowHeight), "Engine");
}

void Engine::initImGui()
{
    Logger::Log(LogLevel::Info, "Initializing ImGui...", "Engine");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_Window, m_Context);
    ImGui_ImplOpenGL3_Init();

    Logger::Log(LogLevel::Info, "ImGui initialization successful.", "Engine");
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
        Logger::Log(LogLevel::Error, "Engine::Run received null IGame.", "Engine");
        throw std::runtime_error("Engine::Run received null IGame.");
    }

    Logger::Log(LogLevel::Info, "Engine::Run() starting", "Engine");
    Logger::Log(LogLevel::Info, "Calling game->OnInit()", "Engine");
    game->OnInit(*this);

    bool running = true;
    int frameCount = 0;

    while (running)
    {
        InputSystem::getInstance()->update(m_Window);
        if (InputSystem::getInstance()->shouldStop()) {
            Logger::Log(LogLevel::Info,
                        "InputSystem requested stop, leaving main loop", "Engine");
            running = false;
            break;
        }

        Time::getInstance().computeDeltaTime();
        float dt = Time::getInstance().getDeltaTime();

        if ((frameCount++ % 300) == 0) {
            Logger::Log(
                LogLevel::Info,
                "Frame " + std::to_string(frameCount) +
                " dt=" + std::to_string(dt), "Engine");
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        game->OnUpdate(*this, dt);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT);

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glBindVertexArray(0);

        game->OnRender(*this);

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

        SDL_GL_SwapWindow(m_Window);
    }

    Logger::Log(LogLevel::Info, "Engine::Run() exiting main loop", "Engine");
}


Engine::~Engine() {
    Logger::Log(LogLevel::Info, "Engine destructor: shutting down subsystems.", "Engine");
    shutdownImGui();
    shutdownSDL();
    Logger::Log(LogLevel::Info, "Engine shutdown complete.", "Engine");
}
