# SS6Player for Cpp (C++/ssfb2)
# 
# Copyright(C) Web Technology Corp.
#                      All rights reserved.

cmake_minimum_required (VERSION 3.15)
project(SS6PlayerForCpp)

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

set(flatBuffers_Library_srcs
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/base.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/code_generators.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/flatbuffers.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/flatc.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/flexbuffers.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/grpc.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/hash.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/idl.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/minireflect.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/reflection.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/reflection_generated.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/registry.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/stl_emulation.h
        ${flatbuffers_SOURCE_DIR}/include/flatbuffers/util.h
        )
include_directories("${flatbuffers_SOURCE_DIR}/include")
set(SS6PlayerForCpp_srcs ${SS6PlayerForCpp_srcs}
        ${flatBuffers_Library_srcs}
        )

# Common
## Entity
set(SS6PlayerForCpp_srcs ${SS6PlayerForCpp_srcs}
        Common/Entity/ss6player_entity.cpp
        Common/Entity/ss6player_entity.h
        Common/Entity/ss6player_entity_effect.cpp
        Common/Entity/ss6player_entity_effect.h
        Common/Entity/ss6player_entity_effect_function_animation.cpp
        Common/Entity/ss6player_entity_effect_function_animation.h
        Common/Entity/ss6player_entity_effect_function_cell.cpp
        Common/Entity/ss6player_entity_effect_function_cell.h
        Common/Entity/ss6player_entity_effect_function_misc.cpp
        Common/Entity/ss6player_entity_effect_function_misc.h
        Common/Entity/ss6player_entity_effect_function_parts.cpp
        Common/Entity/ss6player_entity_effect_function_parts.h
        Common/Entity/ss6player_entity_function_animation.cpp
        Common/Entity/ss6player_entity_function_animation.h
        Common/Entity/ss6player_entity_function_cell.cpp
        Common/Entity/ss6player_entity_function_cell.h
        Common/Entity/ss6player_entity_function_misc.cpp
        Common/Entity/ss6player_entity_function_misc.h
        Common/Entity/ss6player_entity_function_parts.cpp
        Common/Entity/ss6player_entity_function_parts.h
        Common/Entity/ss6player_entity_function_track.cpp
        Common/Entity/ss6player_entity_function_track.h
        Common/Entity/ss6player_entity_replicate.cpp
        Common/Entity/ss6player_entity_replicate.h
        )
include_directories(Common/Entity)

## Library
set(SS6PlayerForCpp_srcs ${SS6PlayerForCpp_srcs}
        Common/Library/ss6player_control.cpp
        Common/Library/ss6player_control.h
        Common/Library/ss6player_control_effect.cpp
        Common/Library/ss6player_control_effect.h
        Common/Library/ss6player_data.cpp
        Common/Library/ss6player_data.h
        Common/Library/ss6player_data_project.cpp
        Common/Library/ss6player_data_project.h
        Common/Library/ss6player_pack.cpp
        Common/Library/ss6player_pack.h
        Common/Library/ss6player_utility.cpp
        Common/Library/ss6player_utility.h
        Common/Library/ss6player_utility_random_mersennetwister.cpp
        Common/Library/ss6player_utility_random_mersennetwister.h
        Common/Library/ss6player_utility_random_xorshift32.cpp
        Common/Library/ss6player_utility_random_xorshift32.h
        )
include_directories(Common/Library)

## PrimitiveBatcher
set(SS6PlayerForCpp_srcs ${SS6PlayerForCpp_srcs}
        Common/PrimitiveBatcher/ss6player_primitive_batcher.cpp
        Common/PrimitiveBatcher/ss6player_primitive_batcher.h
        )
include_directories(Common/PrimitiveBatcher)

## Project
set(SS6PlayerForCpp_srcs ${SS6PlayerForCpp_srcs}
        Common/Project/ss6player_project.cpp
        Common/Project/ss6player_project.h
        )
include_directories(Common/Project)

## Sequencer
set(SS6PlayerForCpp_srcs ${SS6PlayerForCpp_srcs}
        Common/Sequencer/ss6player_sequencer.cpp
        Common/Sequencer/ss6player_sequencer.h
        )
include_directories(Common/Sequencer)

set(SS6PlayerForCpp_srcs ${SS6PlayerForCpp_srcs}
        Common/ssfb2_generated.h
        )
include_directories(Common)

# Platform
include_directories(Platform)
if ((NOT DEFINED ENGINE) OR (ENGINE STREQUAL "OPENGL"))
  ## OpenGL
  set(SS6PlayerForCpp_srcs ${SS6PlayerForCpp_srcs}
          Platform/OpenGL/ss6player_buffer.cpp
          Platform/OpenGL/ss6player_buffer.h
          Platform/OpenGL/ss6player_file.cpp
          Platform/OpenGL/ss6player_file.h
          Platform/OpenGL/ss6player_gpu_resource_cache.h
          Platform/OpenGL/ss6player_memory.cpp
          Platform/OpenGL/ss6player_memory.h
          Platform/OpenGL/ss6player_platform.h
          Platform/OpenGL/ss6player_renderer.cpp
          Platform/OpenGL/ss6player_renderer.h
          Platform/OpenGL/ss6player_shader.cpp
          Platform/OpenGL/ss6player_shader.h
          Platform/OpenGL/ss6player_system.cpp
          Platform/OpenGL/ss6player_system.h
          Platform/OpenGL/ss6player_texture.cpp
          Platform/OpenGL/ss6player_texture.h
          Platform/OpenGL/ss6player_types.cpp
          Platform/OpenGL/ss6player_types.h
          )

  include_directories(Platform/OpenGL)                      # [SS6Player for CPP] Platform
  include_directories(${glew_SOURCE_DIR}/include)           # GLEW
  include_directories(${glfw_SOURCE_DIR}/include)           # GLFW
  include_directories(${stb_SOURCE_DIR})                    # STB
else()
endif()

add_library(SS6PlayerForCpp STATIC ${SS6PlayerForCpp_srcs})
add_dependencies(SS6PlayerForCpp flatbuffers)
set_target_properties(SS6PlayerForCpp PROPERTIES OUTPUT_NAME "SS6PlayerForCpp")
if ((NOT DEFINED ENGINE) OR (ENGINE STREQUAL "OPENGL"))
    add_dependencies(SS6PlayerForCpp libglew_static)
endif()
