#!/bin/csh
if( ! $?LD_LIBRARY_PATH ) then
  setenv LD_LIBRARY_PATH ../hepmcbuild/lib
else
  setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:../hepmcbuild/lib
endif
