#!/bin/csh -f

# Defining colours for shell
set GREEN  = "\033[1;32m"
set RED    = "\033[1;31m"
set PINK   = "\033[1;35m"
set BLUE   = "\033[1;34m"
set YELLOW = "\033[1;33m"
set CYAN   = "\033[1;36m"
set NORMAL = "\033[0;39m"

# Configuring MA5 environment variable
setenv MA5_BASE /media/sf_darkphotons/madanalysis/madanalysis5

# Configuring PATH environment variable
if ( $?PATH ) then
setenv PATH "$PATH":/usr/local/root/5.34.09/bin
else
setenv PATH /usr/local/root/5.34.09/bin
endif

# Configuring LD_LIBRARY_PATH environment variable
if ( $?LD_LIBRARY_PATH ) then
setenv LD_LIBRARY_PATH $MA5_BASE/tools/SampleAnalyzer/Lib/:"$LD_LIBRARY_PATH":/usr/local/root/5.34.09/lib/root:/usr/lib64
else
setenv LD_LIBRARY_PATH $MA5_BASE/tools/SampleAnalyzer/Lib/:/usr/local/root/5.34.09/lib/root:/usr/lib64
endif

# Checking that all environment variables are defined
if ( $?MA5_BASE && $?PATH && $?LD_LIBRARY_PATH ) then
echo $YELLOW"--------------------------------------------------------"
echo "    Your environment is properly configured for MA5     "
echo "--------------------------------------------------------"$NORMAL
endif
