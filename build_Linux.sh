#!/bin/sh
clear
export PATH="/home/obooklage/Qt/Tools/CMake/bin:$PATH"
echo $PATH
cd ./Linux-Release
#/home/obooklage/Qt/Tools/CMake/bin/cmake -version
# cmake version 3.27.7
# configure
/home/obooklage/Qt/Tools/CMake/bin/cmake -S ~/Documents/GitHub/WebExaMod/src -B ~/Documents/GitHub/WebExaMod/build/Linux-Release
# compile
cmake --build ~/Documents/GitHub/WebExaMod/build/Linux-Release --target all
