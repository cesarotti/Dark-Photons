##!/bin/bash
# Run annihilation

cd darkPhotonBuild
cmake -DGEANT4_BUILD_MULTITHREADED=OFF -DGeant4_DIR=/usr/local/geant4.10.00.p01/lib64/Geant4-10.0.0 -DHEPMC_INCLUDE_DIR=../../madgraph/hepmcbuild/include/ -DHEPMC_LIBRARIES=../../madgraph/hepmcbuild/lib/libHepMC.so ../darkPhoton
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
