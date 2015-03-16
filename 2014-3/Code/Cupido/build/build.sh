#!/bin/bash

FOLDER=${PWD##*/}

MODE=$(echo $FOLDER | sed 's/[^ ]\+/\L\u&/g')
echo $MODE

(cd ../../../Clarus/build/$FOLDER/ && ./build.sh)

(cd ../../../Cight/build/$FOLDER/ && ./build.sh)

cmake -DCMAKE_BUILD_TYPE=$MODE ../..

make -j2
