#! /bin/sh
# test/testHepMC.sh.  Generated from testHepMC.sh.in by configure.

./testHepMC >& testHepMC.cout

OS=`uname`
case "$OS" in
CYGWIN*)
  #don't compare these on Windows (hopelessly different default output)
  cmd1=`sed 's/e+0/e+/g' testHepMC.out   | \
       sed 's/e-0/e-/g' -   | \
	diff -q -b - ./testHepMC.dat`
  cmd2=
  ;;
Darwin*)
  # MacOSX fix
  cmd2=`sed 's/e-00/e+00/g'  ./testHepMCParticle.output | \
	diff -q -b - testHepMCParticle.out`
  cmd1=`diff -q -b testHepMC.out ./testHepMC.dat`
  ;;
*)
  cmd2=`diff -q -b testHepMCParticle.out ./testHepMCParticle.output`
  cmd1=`diff -q -b testHepMC.out ./testHepMC.dat`
esac

if [ `diff -q -b testHepMC.cout ./testHepMC.output > /dev/null` ]; then
  echo "testHepMC.cout differs"
  exit 1;
fi

if [ -n "$cmd1" ]
then
  echo $cmd1
  exit 1;
fi

if [ -n "$cmd2" ]
then
  echo $cmd2
  exit 1;
fi

exit 0;
