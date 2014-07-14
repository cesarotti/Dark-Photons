#!/bin/bash
# Run annihilation

cd /home/local1/Desktop/standard-build
cmake -DGEANT4_BUILD_MULTITHREADED=ON -DGeant4_DIR=/usr/local/geant4.10.00.p01/lib64/Geant4-10.0.0 /home/local1/Desktop/standard
make clean
make -j4
./darkPhoton
mkdir ../../standardPlots
mkdir ../../standardPlots/`date +%Y:%m:%d#%H:%M:%S`
mv *.root ../../standardPlots/`date +%Y:%m:%d#%H:%M:%S`
rm -rf *.root
cd ../../standardPlots/`date +%Y:%m:%d#%H:%M:%S`
hadd complete.root *.root
mkdir threadfiles
mv *.root threadfiles
cd threadfiles
mv complete.root ../
