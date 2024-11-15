#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "shader.hpp"
#include "camera.hpp"
#include "time.hpp"
#include "model.hpp"
#include <primitive.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <mtl_parser.hpp>
#include <material.hpp>

#include <input.hpp>
#include <materials.hpp>
#include <entity.hpp>
#include <entity_manager.hpp>


constexpr unsigned int WINDOW_WIDTH = 1980;
constexpr unsigned int WINDOW_HEIGHT = 1080;


void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar* message, const void* userParam) {
    std::cerr << "OpenGL Debug Message: " << message << std::endl;
}

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

#ifdef DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_Window* window = SDL_CreateWindow(
        "OpenGL Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
    if (SDL_GL_MakeCurrent(window, context) != 0) {
        std::cerr << "SDL_GL_MakeCurrent::ERROR::" << SDL_GetError() << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

#ifdef VSYNC
    SDL_GL_SetSwapInterval(1);
#endif

#ifdef DEBUG
    if (gladLoadGL()) {  // Ensure GLAD loaded OpenGL
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {  // Check if debug context is active
            std::cout << "GL_CONTEXT::DEBUG::ACTIVATED" << std::endl;
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openglDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    } else {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
    }
#endif

    const GLubyte* version = glGetString(GL_VERSION);
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    } else {
        std::cerr << "Unable to retrieve OpenGL version." << std::endl;
    }

    int currentWindowWidth, currentWindowHeight;
    SDL_GetWindowSize(window, &currentWindowWidth, &currentWindowHeight);
    float currrentWindowRatio = static_cast<float>(currentWindowWidth) / currentWindowHeight;

    glViewport(0, 0, currentWindowWidth, currentWindowHeight);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();

    ShaderEngine shaderEngineLighting;
    ShaderEngine shaderEngineLight;

    ShaderFactory shaderFactory;

    Shader lightingVertexShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\lighting_vertex.glsl", GL_VERTEX_SHADER);
    shaderEngineLighting.addShader(lightingVertexShader);
    Shader lightingFragmentShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\lighting_fragment.glsl", GL_FRAGMENT_SHADER);
    shaderEngineLighting.addShader(lightingFragmentShader);
    shaderEngineLighting.compile();

    Shader lightVertexShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\light_vertex.glsl", GL_VERTEX_SHADER);
    shaderEngineLight.addShader(lightVertexShader);
    Shader lightFragmentShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\light_fragment.glsl", GL_FRAGMENT_SHADER);
    shaderEngineLight.addShader(lightFragmentShader);
    shaderEngineLight.compile();

    glEnable(GL_DEPTH_TEST);

    Cube cube(1.0f);
    // cube.setTexture("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\res\\box.bmp", TextureType::DIFFUSE);
    cube.setShaderEngine(shaderEngineLight);
    Sphere sphere(1.0f);

    Cube cubeLighting(1.0f);
    cubeLighting.setShaderEngine(shaderEngineLighting);
    cubeLighting.setTexture("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\res\\box.bmp", TextureType::DIFFUSE);
    cubeLighting.setTexture("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\res\\box_specular_map.png", TextureType::SPECULAR);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    Model model("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\res\\teapot.fbx");
    // model.setShaderEngine(shaderEngineLighting);

    Camera camera; 

    InputHandler::CursorMovementCallback callback = std::bind(&Camera::computeCursorCameraMovements, &camera, std::placeholders::_1, std::placeholders::_2);
    InputHandlerFactory::createInputHandler(callback);

    Material goldMaterial = MaterialManager::getInstance()->getMaterial(MaterialType::GOLD);
    Material silverMaterial = MaterialManager::getInstance()->getMaterial(MaterialType::SILVER);
    Material copperMaterial = MaterialManager::getInstance()->getMaterial(MaterialType::COPPER);

    Entity entity = EntityFactory::createEntity();

    while (InputSystem::getInstance()->shouldStop()) {

        Time::getInstance().computeDeltaTime();
        InputSystem::getInstance()->update(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Handle platform windows for multi-viewports
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        std::vector<Action> actions = getActions(keystate);
        camera.computeActions(actions);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        // modelMatrix = glm::rotate(modelMatrix, (float)SDL_GetTicks()/256, glm::vec3(0.0f, 1.0f, 0.0f));
        // modelMatrix = glm::rotate(modelMatrix, (float)SDL_GetTicks64()/128 * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view(1.0f);
        view = camera.getViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f),
            currrentWindowRatio, 0.1f, 100.0f);

        shaderEngineLight.use();
        shaderEngineLight.setMat4("projection", projection);
        shaderEngineLight.setMat4("view", view);
        modelMatrix = glm::translate(modelMatrix, lightPos);
        glm::vec3 lightPosition = glm::vec3(modelMatrix[3]);

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        shaderEngineLight.setMat4("model", modelMatrix);

        cube.draw();

        modelMatrix = glm::mat4(1.0f);

        shaderEngineLighting.use();
        shaderEngineLighting.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        shaderEngineLighting.setFloat("material.shininess", 64.0f);

        shaderEngineLighting.setVec3("light.position", lightPosition.x,
            lightPosition.y, lightPosition.z);

        shaderEngineLighting.setVec3("light.ambient", 0.2f, 0.2f, 0.2f); 
        shaderEngineLighting.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        shaderEngineLighting.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        shaderEngineLighting.setVec3("cameraPosition", camera.getPosition());

        shaderEngineLighting.setMat4("model", modelMatrix);
        shaderEngineLighting.setMat4("view", view);
        shaderEngineLighting.setMat4("projection", projection);
        cubeLighting.draw();

        // model.draw();
        glBindVertexArray(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
