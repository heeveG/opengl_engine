cmake_minimum_required(VERSION 3.17)
project(opengl_engine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
add_compile_definitions(GLEW_STATIC)
add_subdirectory(external_sources "external_sources")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

configure_file(config/root_directory.h.in config/root_directory.h)

include_directories(
        ${CMAKE_SOURCE_DIR}/src/gl
        ${CMAKE_SOURCE_DIR}/src/light
        ${CMAKE_SOURCE_DIR}/src/model
        ${CMAKE_SOURCE_DIR}/src/renderer
        ${CMAKE_SOURCE_DIR}/src/util
        ${CMAKE_SOURCE_DIR}/external_sources/glfw-3.3.8/include
        ${CMAKE_SOURCE_DIR}/external_sources/glew-2.1.0/include
        ${CMAKE_SOURCE_DIR}/external_sources/assimp-5.2.5/include
        ${CMAKE_SOURCE_DIR}/external_sources/glm
        ${CMAKE_SOURCE_DIR}/external_sources/stb_image

        ${CMAKE_BINARY_DIR}/config
)

file(GLOB_RECURSE PROJECT_SOURCES
        src/*.cpp
        external_sources/stb_image/*.cpp
     )

file(GLOB_RECURSE PROJECT_HEADERS
        src/*.h
        external_sources/stb_image/*.h
     )

file(GLOB_RECURSE  PROJECT_SHADERS
        res/shaders/*      
     )

file(GLOB PROJECT_CONFIGS 
        CMakeLists.txt
        README.md
        .gitignore
    )

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Configs" FILES ${PROJECT_CONFIGS})

add_definitions(-DGLFW_INCLUDE_NONE -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(opengl_engine ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_SHADERS})

target_link_libraries(opengl_engine ${OPENGL_LIBRARY} GLEW_LIB assimp glfw ${GLFW_LIBRARIES})

# choose blurring approach (BLUR_SCISSOR/BLUR_MASK/BLUR_BRANCH). BLUR_BRANCH used by default.
target_compile_definitions(opengl_engine PUBLIC BLUR_SCISSOR)

# set VS startup project.
if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT opengl_engine)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
endif()
