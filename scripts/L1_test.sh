#!/bin/bash
tests_dir=tests/L1;
passed=0 ;
failed=0 ;
cd ${tests_dir} ; 
for i in *.L1 ; do

  # If the output already exists, skip the current test
  if ! test -f ${i}.out ; then
    continue ;
  fi
  echo $i ;

  # Generate the binary
  pushd ./ ;
  cd ../.. ;
  ./L1c ${tests_dir}/${i} ;
  ./a.out &> ${tests_dir}/${i}.out.tmp ;
  cmp ${tests_dir}/${i}.out.tmp ${tests_dir}/${i}.out ;
  if ! test $? -eq 0 ; then
    echo "test ${i}  Failed" ;
    return -1
    let failed=$failed+1 ;
  else
    echo "  Passed" ;
    let passed=$passed+1 ;
  fi
  popd ; 
done
let total=$passed+$failed ;

echo "########## SUMMARY" ;
echo "Test passed: $passed out of $total"
