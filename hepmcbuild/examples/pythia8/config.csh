#!/bin/csh
if( ! $?LD_LIBRARY_PATH ) then
  setenv LD_LIBRARY_PATH /usr/local/lib
else
  setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/usr/local/lib
endif
setenv PYTHIA8DATA ${PYTHIA8_HOME}/xmldoc
