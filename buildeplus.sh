#!/bin/bash
mkdir runs
cd project-build/eplus
cmake -DGEANT4_BUILD_MULTITHREADED=ON -DGeant4_DIR=/usr/local/geant4.10.00.p01/lib64/Geant4-10.0.0 ../../eplus
make clean
make
./darkPhoton
mkdir ../../runs/`date +%Y:%m:%d#%H:%M:%S`
mv *.root ../../runs/`date +%Y:%m:%d#%H:%M:%S`
rm -rf *.root
cd ../../runs/`date +%Y:%m:%d#%H:%M:%S`
hadd complete.root *.root
mkdir threadfiles
mv *.root threadfiles
cd threadfiles
mv complete.root ../
