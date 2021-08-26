set SS6PCPP_NAME_COMPILEENV="VS2019"
set SS6PCPP_VISUALSTUDIO="D:/Application/Microsoft/Visual Studio/2019/Community/VC"
set SS6PCPP_VISUALSTUDIO_VERSION="14.29.30133"
set SS6PCPP_WINDOWSKITS_PATH="C:/Program Files (x86)/Windows Kits"
set SS6PCPP_WINDOWSKITS_VERSION_MAJOR="10"
set SS6PCPP_WINDOWSKITS_VERSION_MINOR="0.19041.0"
set SS6PCPP_WINDOWSKITS_FRAMEWORK="4.8"

set BUILDTREE_PATH="./build_win"

cmake -DTARGET=win64 -DCMAKE_BUILD_TYPE=%1 -DMSVC=true -DCMAKE_TOOLCHAIN_FILE=./conf/OpenGL/ToolChain.cmake -B %BUILDTREE_PATH%
cmake --build %BUILDTREE_PATH% --parallel
