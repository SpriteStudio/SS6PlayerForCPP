set BUILDTREE_PATH="../build_win"

cmake -DCMAKE_BUILD_TYPE=%1 -DCMAKE_TOOLCHAIN_FILE=../conf/OpenGL/ToolChain.cmake -B %BUILDTREE_PATH% -S ..
cmake --build %BUILDTREE_PATH% --target SS6PlayerForCpp_AppliationMain --parallel 
