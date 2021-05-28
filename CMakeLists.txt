# SS6Player for Cpp (C++/ssfb2)
# 
# Copyright(C) Web Technology Corp.
#                      All rights reserved.

cmake_minimum_required (VERSION 3.14)

# Project-Name
project ("SS6PlayerForCpp_Sample")

# command options
option(ENABLE_CCACHE "Enable ccache?" OFF)

#
# ccache
find_program(CCACHE_EXE ccache)
if(CCACHE_EXE)
    if(ENABLE_CCACHE)
        message(STATUS "Enable ccache")
        if(CMAKE_C_COMPILER_LAUNCHER)
            set(CMAKE_C_COMPILER_LAUNCHER "${CMAKE_C_COMPILER_LAUNCHER}" "${CCACHE_EXE}")
        else()
            set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_EXE}")
        endif()
        if(CMAKE_CXX_COMPILER_LAUNCHER)
            set(CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CXX_COMPILER_LAUNCHER}" "${CCACHE_EXE}")
        else()
            set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXE}")
        endif()
    endif()
endif()

if (APPLE AND (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    set(CMAKE_MACOSX_RPATH 1)
endif()

include(FetchContent)
FetchContent_Declare(
        flatbuffers
        GIT_REPOSITORY https://github.com/google/flatbuffers.git
        GIT_TAG        6df40a2471737b27271bdd9b900ab5f3aec746c7 # 1.12.0
)
FetchContent_MakeAvailable(flatbuffers)

if ((NOT DEFINED ENGINE) OR (ENGINE STREQUAL "OPENGL"))
    FetchContent_Declare(
            glew
            GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
            GIT_TAG        f456deace7b408655109aaeff71421ef2d3858c6 # glew-cmake-2.2.0
    )
    
    FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG        814b7929c5add4b0541ccad26fb81f28b71dc4d8 # 3.3.4
    )

    FetchContent_Declare(
            stb
            GIT_REPOSITORY https://github.com/nothings/stb.git
            GIT_TAG        master
    )
    FetchContent_MakeAvailable(glew glfw stb)
endif()

# Composition
add_subdirectory("src")                 # Static-Library: libSS6PlayerForCpp
add_subdirectory("src/Application")     # Sample: Application-Main

