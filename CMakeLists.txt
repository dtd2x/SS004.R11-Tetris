cmake_minimum_required(VERSION 3.28)
project(Tetris LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(Tetris
    src/main.cpp
    src/Paths.cpp
    src/core/Game.cpp
    src/core/Piece.cpp
    src/core/HighScore.cpp
    src/input/Input.cpp
    src/render/Renderer.cpp
    src/render/Theme.cpp
    src/render/Particles.cpp
    src/audio/Audio.cpp
)
target_link_libraries(Tetris PRIVATE sfml-graphics sfml-audio)
target_compile_features(Tetris PRIVATE cxx_std_17)
if(MSVC)
    target_compile_options(Tetris PRIVATE /utf-8)
endif()

# Chep font, am thanh ra canh file exe (moi lan build)
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Tetris>/assets
    VERBATIM)
add_dependencies(Tetris copy_assets)

if(WIN32)
    add_custom_command(TARGET Tetris PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
                $<TARGET_FILE_DIR:Tetris>
        VERBATIM)
endif()
