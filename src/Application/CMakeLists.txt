# SS6Player for Cpp (C++/ssfb2)
# 
# Copyright(C) Web Technology Corp.
#                      All rights reserved.

cmake_minimum_required (VERSION 3.15)
project(SS6PlayerForCpp_AppliationMain)

# command options
# TODO: 
# option(ENABLE_CCACHE "Enable ccache?" ON)

# platform detect
# TODO:
if (MSVC)
    set(DEFAULT_STATIC_OPTION YES)
else()
    set(DEFAULT_STATIC_OPTION NO)
endif()

#
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# header-file directories
set(headerDirectories_SS6player
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/Entity"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/Library"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/PrimitiveBatcher"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/Project"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/Sequencer"
    "${CMAKE_CURRENT_SOURCE_DIR}/../OSS"
)
include_directories(${headerDirectories_SS6player})

# config.h
set(NAME_RUNTIME_FILE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../DataRuntime/OpenGL")
configure_file(./OpenGL/config.h.in ./OpenGL/config.h)
message(STATUS ${CMAKE_CURRENT_BINARY_DIR}/)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/OpenGL)

# platform
if ((NOT DEFINED ENGINE) OR (ENGINE STREQUAL "OPENGL"))
  ## OpenGL
  set(SS6PlayerForCppApp_srcs ${SS6PlayerForCppApp_srcs}
          OpenGL/main.cpp
          OpenGL/application.cpp
          OpenGL/application.h
          )

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Platform/OpenGL) # [SS6Player for CPP] Platform
  include_directories(${glew_SOURCE_DIR}/include)                     # GLEW
  include_directories(${glfw_SOURCE_DIR}/include)                     # GLFW
  include_directories(${stb_SOURCE_DIR})                              # STB
endif()

# link
add_executable(SS6PlayerForCpp_AppliationMain ${SS6PlayerForCppApp_srcs})
target_link_libraries(SS6PlayerForCpp_AppliationMain SS6PlayerForCpp)
if ((NOT DEFINED ENGINE) OR (ENGINE STREQUAL "OPENGL"))
  # dependency libraries
  add_dependencies(SS6PlayerForCpp_AppliationMain glfw)
  add_dependencies(SS6PlayerForCpp_AppliationMain libglew_static)
  target_link_libraries(SS6PlayerForCpp_AppliationMain glfw)
  target_link_libraries(SS6PlayerForCpp_AppliationMain libglew_static)
  if(WIN32)
     target_link_libraries(SS6PlayerForCpp_AppliationMain opengl32)
   elseif(APPLE AND (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
     # macOS
     target_link_libraries(SS6PlayerForCpp_AppliationMain "-framework Cocoa")
     target_link_libraries(SS6PlayerForCpp_AppliationMain "-framework IOKit")
     target_link_libraries(SS6PlayerForCpp_AppliationMain "-framework OpenGL")
   endif()
endif()

# export application required files
if (ENGINE STREQUAL "OPENGL")
    add_custom_command(TARGET SS6PlayerForCpp_AppliationMain POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/../Shader/OpenGL/src ${PROJECT_SOURCE_DIR}/../../DataRuntime/OpenGL/shader
	)
endif()

