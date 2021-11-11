#!/bin/bash -e

SCRIPTDIR=$(dirname $0)
SCRIPTDIR=$(cd $SCRIPTDIR && pwd -P)
BASEDIR=${SCRIPTDIR}/..
BASEDIR=$(cd ${BASEDIR} && pwd -P)

BUILDTYPE=Debug
if [ $# -ge 1 ]; then
    BUILDTYPE=$1
fi
ENABLE_CCACHE=ON
if [ $BUILDTYPE != Debug ]; then
    ENABLE_CCACHE=OFF
fi

pushd "${BASEDIR}" > /dev/null

BUILDDIR=buildmac
/bin/rm -rf "${BUILDDIR}"
/bin/mkdir -p "${BUILDDIR}"

pushd "${BUILDDIR}" > /dev/null
cmake -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DWITH_CI_BUILD=ON ..
cmake --build . -j8
ctest
popd > /dev/null # BUILDDIR

popd > /dev/null # BASEDIR