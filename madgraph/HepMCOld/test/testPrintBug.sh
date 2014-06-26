#! /bin/sh
# test/testPrintBug.sh.  Generated from testPrintBug.sh.in by configure.

./testPrintBug 

if ( ! `sed 's/e-0\([0-9][0-9]\)/e-\1/g' testPrintBug.out | \
  sed 's/e+0\([0-9][0-9]\)/e+\1/g'  | \
  diff -q -b - ./testPrintBug.output > /dev/null` )
then
  exit 1;
fi

exit 0;

