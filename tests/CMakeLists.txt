cmake_minimum_required(VERSION 3.23)

find_package(GTest CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Get source files from the main project (excluding main.cpp)
file(GLOB_RECURSE ENGINE_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Test sources
set(TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/tests/MinimalTest.cpp"
    "${CMAKE_SOURCE_DIR}/tests/ActionTest.cpp"
    "${CMAKE_SOURCE_DIR}/tests/InputHandlerFactoryTest.cpp"
    "${CMAKE_SOURCE_DIR}/tests/InputSystemTest.cpp"
    "${CMAKE_SOURCE_DIR}/tests/stb_impl.cpp"
)

# Create the tests executable with custom main
add_executable(LambEngineTests ${TEST_SOURCES} ${ENGINE_SOURCES})

# Include directories
target_include_directories(LambEngineTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${SRC_SUBDIRS}
    ${Stb_INCLUDE_DIR}
)

# Link libraries - use gtest without gtest_main, we'll provide our own main
target_link_libraries(LambEngineTests PRIVATE
    GTest::gtest
    GTest::gmock
    SDL2::SDL2
    glad::glad
    OpenGL::GL
    assimp::assimp
    glm::glm
    imgui::imgui
    nlohmann_json::nlohmann_json
)

# Register tests
add_test(NAME AllTests COMMAND LambEngineTests)
