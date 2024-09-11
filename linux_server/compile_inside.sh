#! /usr/bin/bash

echo "runnin cmake script"
rm -r ./cmake-build-debug & cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" -S ./ -B ./cmake-build-debug && cmake --build ./cmake-build-debug -- -j 8
echo "finished cmake script"
