#!/bin/csh
if( ! $?LD_LIBRARY_PATH ) then
  setenv LD_LIBRARY_PATH /home/local1/project/Dark-Photons/madgraph/hepmcbuild/lib
else
  setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/home/local1/project/Dark-Photons/madgraph/hepmcbuild/lib
endif
setenv PYTHIA8DATA ${PYTHIA8_HOME}/xmldoc
