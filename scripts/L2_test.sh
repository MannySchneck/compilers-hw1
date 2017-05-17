#!/bin/bash

passed=0 ;
failed=0 ;
cd tests/L2 ; 
for i in *.L2 ; do

  # If the output already exists, skip the current test
  if ! test -f ${i}.out ; then
    continue ;
  fi
  echo $i ;

  # Generate the binary
  pushd ./ > /dev/null 2>&1;
  cd ../.. ;
  ./L2/L2c tests/L2/${i} ;
  ./a.out &> tests/L2/${i}.out.tmp ;
  cmp tests/L2/${i}.out.tmp tests/L2/${i}.out ;
  if ! test $? -eq 0 ; then
    echo "  Failed" ;
    echo;
    let failed=$failed+1 ;
  else
    echo "  Passed" ;
    echo;
    let passed=$passed+1 ;
  fi
  popd > /dev/null 2>&1; 
done
let total=$passed+$failed ;

echo "########## SUMMARY" ;
echo "Test passed: $passed out of $total"
