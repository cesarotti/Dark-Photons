#!/bin/sh
if [ ! $?LD_LIBRARY_PATH ]; then
  export LD_LIBRARY_PATH=/home/local1/project/Dark-Photons/madgraph/hepmcbuild/lib
fi
if [ $?LD_LIBRARY_PATH ]; then
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/local1/project/Dark-Photons/madgraph/hepmcbuild/lib
fi
export PYTHIA8DATA=${PYTHIA8_HOME}/xmldoc
