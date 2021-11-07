set BUILDTREE_PATH="../build_win"

<<<<<<< HEAD
cmake -DTARGET=win64 -DCMAKE_BUILD_TYPE=%1 -DCMAKE_TOOLCHAIN_FILE=../conf/OpenGL/ToolChain.cmake -B %BUILDTREE_PATH% -S ..
=======
set BUILDTREE_PATH="../build_win"

cmake -DTARGET=win64 -DCMAKE_BUILD_TYPE=%1 -DMSVC=true -DCMAKE_TOOLCHAIN_FILE=./conf/OpenGL/ToolChain.cmake -B %BUILDTREE_PATH% -S ..
>>>>>>> 8e7205b7f24218fcef4a1e33668657ef097ae696
cmake --build %BUILDTREE_PATH% --parallel 
