#!/bin/bash
mkdir -p "$PWD"/out

docker run -it \
   --name fluxcapacitor \
   --rm \
   -v "$PWD"/src:/library/src \
   -v "$PWD"/examples:/library/examples \
   -v "$PWD"/out:/library/out \
   -v "$PWD"/library.properties:/library/library.properties \
   -v "$PWD"/doxygen/doxygen.conf:/doxygen/doxygen.conf \
   superjonotron/doxygen $@