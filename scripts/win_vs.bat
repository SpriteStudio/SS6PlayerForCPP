set BUILDTREE_PATH="../build_win"

cmake -DTARGET=win64 -DCMAKE_BUILD_TYPE=%1 -DCMAKE_TOOLCHAIN_FILE=../conf/OpenGL/ToolChain.cmake -B %BUILDTREE_PATH% -S ..
cmake --build %BUILDTREE_PATH% --parallel 
