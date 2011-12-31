#!/bin/bash

basedir="../.."
file=Build/cmake/ContribFileList.cmake
src='set(Contriblib_SRC_FILES'
hdr='set(Contriblib_HDR_FILES'
pubhdr='set(Contriblib_PUB_HDR_FILES'
srcdir='${PROJECT_SOURCE_DIR}'
srcpath=Contrib/Source
hdrpath=Contrib/Include/Rocket

printfiles() {
    # Print headers
    echo ${hdr/lib/$1} >>$file
    find  $srcpath/$1/ -maxdepth 1 -iname "*.h" -exec echo '    '$srcdir/{} \; >>$file
    echo -e ')\n' >>$file
    # Print public headers
    echo ${pubhdr/lib/$1} >>$file
    find  $hdrpath/$1/ -maxdepth 1 -iname "*.h" -exec echo '    '$srcdir/{} \; >>$file
    # Print main public header
    echo '    '$srcdir/Contrib/Include/Rocket/$1.h >>$file
    echo -e ')\n' >>$file
    # Print source files
    echo ${src/lib/$1} >>$file
    find  $srcpath/$1/ -maxdepth 1 -iname "*.cpp" -exec echo '    '$srcdir/{} \; >>$file
    echo -e ')\n' >>$file
}

pushd $basedir
echo -e "# This file was auto-generated with gen_contrib.sh\n" >$file
for lib in "ProgressBar"; do
    printfiles $lib
done

popd

