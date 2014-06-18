#!/bin/bash
cd project-build/skeleton
cmake -DGEANT4_BUILD_MULTITHREADED=ON -DGeant4_DIR=/usr/local/geant4.10.00.p01/lib64/Geant4-10.0.0 ../../skeleton
make clean
make
./darkPhoton
