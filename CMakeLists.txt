cmake_minimum_required(VERSION 3.16)
project(MyEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(GFX_ENABLE_LOGGING "Enable logging (spdlog)" ON)
option(GFX_BUILD_EXAMPLES "Build examples" ON)

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

if(GFX_ENABLE_LOGGING)
    add_compile_definitions(GFX_ENABLE_LOGGING)
endif()

add_subdirectory(${THIRD_PARTY_DIR}/glfw)
add_subdirectory(${THIRD_PARTY_DIR}/glm)
add_subdirectory(${THIRD_PARTY_DIR}/spdlog)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
add_subdirectory(${THIRD_PARTY_DIR}/assimp)

find_package(slang REQUIRED PATHS ${THIRD_PARTY_DIR}/slang-2026.12-windows-x86_64 NO_DEFAULT_PATH)

add_subdirectory(src/gfx)

if(GFX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
