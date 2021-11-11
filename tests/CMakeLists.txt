# GoogleTest requires at least C++11

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(ss6player_tests)

if (WITH_CI_BUILD)
target_compile_definitions(ss6player_tests PUBLIC "SS6PLAYER_TESTS_WITH_CI_BUILD")
endif()

target_link_libraries(ss6player_tests PUBLIC
	SS6PlayerForCpp
	gtest_main
)

include(GoogleTest)
gtest_discover_tests(ss6player_tests)

# Source files
target_include_directories(ss6player_tests PRIVATE ..)

target_sources(ss6player_tests PRIVATE
main.cpp
Common/Sequencer/ss6player_sequencer.t.cpp
)
