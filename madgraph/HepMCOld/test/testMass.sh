#! /bin/sh
# test/testMass.sh.  Generated from testMass.sh.in by configure.

./testMass

OS=`uname`
case "$OS" in
CYGWIN*)
  cmd1=`sed 's/e+0/e+/g'  testHepMCExtended.dat  | \
     sed 's/e-0/e-/g'  -  | \
	diff -q -b - ./testHepMCExtended.dat`
  ;;
*)
  cmd1=`diff -q -b testHepMCExtended.dat ./testHepMCExtended.dat`
esac

if [ -n "$cmd1" ]
then
  echo $cmd1
  exit 1;
fi

if ( ! `diff -q -b testMass.dat testHepMCExtended.dat` )
then
  echo "comparison of testMass.dat failed"
  exit 1;
fi

exit 0;
