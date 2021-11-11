#!/usr/bin/env pwsh

$SCRIPTDIR = $PSScriptRoot
$BASEDIR = "${SCRIPTDIR}\.."

$BUILD_TYPE = "Debug"
if ($args[0] -ne "") {
    $BUILD_TYPE = $args[0]
}
$ENABLE_CCACHE = "ON"
if ($BUILD_TYPE -ne "Debug") {
    $ENABLE_CCACHE = "OFF"
}

pushd $BASEDIR
$BUILDDIR = "buildwin"
if (Test-Path -Path $BUILDDIR) {
    rm -r -fo $BUILDDIR
}
mkdir $BUILDDIR

pushd $BUILDDIR
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DWITH_CI_BUILD=ON ..
cmake --build . --config $BUILD_TYPE --parallel

popd 
popd
