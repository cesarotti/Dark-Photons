#! /bin/sh
# test/testHepMCIteration.sh.  Generated from testHepMCIteration.sh.in by configure.
# you can compare testHepMCIteration.out to ./testHepMCIteration.dat
# and testHepMCIterationExtended.out to ./testHepMCIterationExtended.dat,
# but the particles within a vertex are printed in "random" order

./testHepMCIteration \
 | diff -q -b - ./testHepMCIteration.output > /dev/null

OS=`uname`
case "$OS" in
CYGWIN*)
  #don't compare these on Windows (hopelessly different default output)
  cmd1=
  ;;
Darwin*)
  # MacOSX fix
  cmd1=`sed 's/(-0,0)/(0,0)/g'  testHepMCIteration.dat  | \
  	diff -q -b - ./testHepMCIteration.dat`
  # dealing with different ordering - can't compare
  ;;
*)
  cmd1=`sed 's/(-0,0)/(0,0)/g'  testHepMCIteration.dat  | \
  	diff -q -b - ./testHepMCIteration.dat`
esac

if [ -n "$cmd1" ]
then
  echo $cmd1
  exit 1;
fi

exit 0;
