#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: ./build.sh [project-name]"
    echo "e.g. ./build.sh eplus"
    exit
fi

if [ ! -d "runs" ]; then
    mkdir runs
fi

if [ ! -d "runs/$1" ]; then
    mkdir runs/$1
fi

if [ ! -d "project-build/$1" ]; then
    mkdir project-build/$1
fi

cd project-build/$1
cmake -DGEANT4_BUILD_MULTITHREADED=ON -DGeant4_DIR=/usr/local/geant4.10.00.p01/lib64/Geant4-10.0.0 -DHEPMC_INCLUDE_DIR=../../madgraph/hepmcbuild/include/ -DHEPMC_LIBRARIES=../../madgraph/hepmcbuild/lib/libHepMC.so ../../$1
make clean
make -j`grep -c ^processor /proc/cpuinfo`
cd ../../$1
OUTPUT=$(basename $(find -maxdepth 1 -iname "*.cc") .cc)
cd ../project-build/$1
./$OUTPUT
mkdir ../../runs/$1/`date +%Y:%m:%d#%H:%M:%S`
mv *.root ../../runs/$1/`date +%Y:%m:%d#%H:%M:%S`
rm -rf *.root
cd ../../runs/$1/`date +%Y:%m:%d#%H:%M:%S`
hadd complete.root *.root
mkdir threadfiles
mv *.root threadfiles
cd threadfiles
mv complete.root ../
