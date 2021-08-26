# SS6Player for Cpp (C++/ssfb2)
# 
# Copyright(C) Web Technology Corp.
#                      All rights reserved.

# ------------------------------------------------------- Usage: CMake Command-Line Options
# -DTARGET=[ gl32 | gl64 ]
# -DCMAKE_TOOLCHAIN_FILE=[ path to this file ]
# -DDIRECTORY_DATA_RUNTIME=[ path to Datas for Runtime ]
#     *) none : Application has no datas.
#        Relative-path : Reference-Dir. is "ss6playerforcpp/"
#        Absolute-path
# 
# ------------------------------------------------------- Requirements for Build
# - Install GLEW under "src/Platform/OpenGL/OSS/GLEW".
# - Install GLFW under "src/Platform/OpenGL/OSS/GLFW".
# - Install stb_image.h "under src/Platform/OpenGL/OSS/stb".
# 
# -------------------------------------------------------

# System-Information
#
# set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(ENGINE "OPENGL")
set(CMAKE_HOST_SYSTEM_NAME Windows)
set(CMAKE_HOST_SYSTEM_VERSION 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_CROSSCOMPILING TRUE)

# Build-Types
#
if(MSVC)
  set(TARGET_ENV_WINDOWS 1)
  set(NAME_TARGET_SPEC "x86")
  set(NAME_TARGET_SPEC_GLEW "Win32")
else()
endif()
if(${TARGET} MATCHES "win32")
  set(TARGET_ENV_WINDOWS 1)
  set(NAME_TARGET_SPEC "x86")
  set(NAME_TARGET_SPEC_GLEW "Win32")
elseif(${TARGET} MATCHES "win64")
  set(TARGET_ENV_WINDOWS 1)
  set(NAME_TARGET_SPEC "x64")
  set(NAME_TARGET_SPEC_GLEW "x64")
else()
endif()

if(SS6PCPP_NAME_COMPILEENV MATCHES "VS2017")
  set(NAME_TARGET_SPEC_GLFW "vc2017")
elseif(SS6PCPP_NAME_COMPILEENV MATCHES "VS2019")
  set(NAME_TARGET_SPEC_GLFW "vc2019")
endif()

# Directories: System-Root & Misc.s
#
if(TARGET_ENV_WINDOWS EQUAL 1)
  set(PATH_DEVTOOLS_BASE "${SS6PCPP_VISUALSTUDIO}/Tools/MSVC/${SS6PCPP_VISUALSTUDIO_VERSION}")
  set(PATH_DEVTOOLS_HOST "${PATH_DEVTOOLS_BASE}/bin/Host${NAME_TARGET_SPEC}")
  set(PATH_DEVTOOLS "${PATH_DEVTOOLS_HOST}/${NAME_TARGET_SPEC}")

  set(PATH_WINDOWSKIT_BASE "${SS6PCPP_WINDOWSKITS_PATH}/${SS6PCPP_WINDOWSKITS_VERSION_MAJOR}")
  set(VERSIONFULL_WINDOWSKIT "${SS6PCPP_WINDOWSKITS_VERSION_MAJOR}.${SS6PCPP_WINDOWSKITS_VERSION_MINOR}")
else()
endif()


# Build-Target
#

# C/C++ Compilers
#
if(TARGET_ENV_WINDOWS EQUAL 1)
  set(CMAKE_C_COMPILER "${PATH_DEVTOOLS}/cl.exe")
  set(CMAKE_CXX_COMPILER "${PATH_DEVTOOLS}/cl.exe")
else()
endif()

# Directories: Include
#
if(TARGET_ENV_WINDOWS EQUAL 1)
  include_directories("${PATH_DEVTOOLS_BASE}/include")
  include_directories("${PATH_DEVTOOLS_BASE}/atlmfc/include")
  include_directories("${SS6PCPP_VISUALSTUDIO}/Auxiliary/VS/include")
  include_directories("${PATH_WINDOWSKIT_BASE}/Include/${VERSIONFULL_WINDOWSKIT}/ucrt")
  include_directories("${PATH_WINDOWSKIT_BASE}/Include/${VERSIONFULL_WINDOWSKIT}/um")
  include_directories("${PATH_WINDOWSKIT_BASE}/Include/${VERSIONFULL_WINDOWSKIT}/shared")
  include_directories("${PATH_WINDOWSKIT_BASE}/Include/${VERSIONFULL_WINDOWSKIT}/winrt")
  include_directories("${PATH_WINDOWSKIT_BASE}/Include/${VERSIONFULL_WINDOWSKIT}/cppwinrt")
  include_directories("${PATH_WINDOWSKIT_BASE}/NETFXSDK/${SS6PCPP_WINDOWSKITS_FRAMEWORK}/Include/um")
else()
endif()

# Directories: Link-Objects
if(TARGET_ENV_WINDOWS EQUAL 1)
  link_directories("${PATH_DEVTOOLS_BASE}/lib/${NAME_TARGET_SPEC}")
  link_directories("${PATH_DEVTOOLS_BASE}/atlmfc/lib/${NAME_TARGET_SPEC}")
  link_directories("${PATH_DEVTOOLS_BASE}/Auxiliary/VS/lib/${NAME_TARGET_SPEC}")
  link_directories("${PATH_WINDOWSKIT_BASE}/lib/${VERSIONFULL_WINDOWSKIT}/ucrt/${NAME_TARGET_SPEC}")
  link_directories("${PATH_WINDOWSKIT_BASE}/lib/${VERSIONFULL_WINDOWSKIT}/um/${NAME_TARGET_SPEC}")
  link_directories("${PATH_WINDOWSKIT_BASE}/NETFXSDK/${SS6PCPP_WINDOWSKITS_FRAMEWORK}/lib/um/${NAME_TARGET_SPEC}")
else()
endif()

# Options: Compiler
#

# Options: Linker
#

# Options: Librarian
#

# Options: Authorizer
#

# Root-Paths
#
# search for programs in the build host directories
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# enable ccache
# find_program(CCACHE_EXE ccache)
# if(CCACHE_EXE)
#     if(ENABLE_CCACHE)
#         message(STATUS "Enable ccache")
#         if(CMAKE_C_COMPILER_LAUNCHER)
#             set(CMAKE_C_COMPILER_LAUNCHER "${CMAKE_C_COMPILER_LAUNCHER}" "${CCACHE_EXE}")
#         else()
#             set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_EXE}")
#         endif()
#         if(CMAKE_CXX_COMPILER_LAUNCHER)
#             set(CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CXX_COMPILER_LAUNCHER}" "${CCACHE_EXE}")
#         else()
#             set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXE}")
#         endif()
#     endif()
# endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++14)
endif()

if(WIN32)
elseif(APPLE)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++14)
endif()
