#!/bin/sh
clear
export PATH="$HOME/Qt/Tools/CMake/bin:$PATH"
echo $PATH
# configure
$HOME/Qt/Tools/CMake/bin/cmake -DCMAKE_PREFIX_PATH="$HOME/Qt/6.10.2/gcc_64" -S "$HOME/Documents/GitHub/ExamWebBrowser/sources" -B "$HOME/Documents/GitHub/ExamWebBrowser/build/linux"
# compile
cmake --build "$HOME/Documents/GitHub/ExamWebBrowser/build/linux" --target all
