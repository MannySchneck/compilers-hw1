#!/bin/bash

cd tests ; 
for i in *.L2 ; do

  # If the output already exists, skip the current test
  if test -f ${i}.out ; then
    continue ;
  fi
  echo $i ;

  # Generate the binary
  pushd ./ ;
  cd ../ ;
  ./L2c tests/${i} ;
  ./a.out &> tests/${i}.out ;
  popd ; 
done
