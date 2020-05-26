#!/bin/bash

# cut the mazeResolution.c file to make it suitable for students

echo "plop"

# first argument is the origin file
origin=$1

# second argument is the goal file
goal=$2

echo -n > $goal

for incl in $(cat $origin | grep '^#include "' | sed "s/ /</g")
do
  incl=$(echo "$incl" | sed "s/</ /g" | sed "s/\n//g")
  echo "$incl" >> $goal
  echo >> $goal
done

for func in $(cat $origin | grep '^void' | sed "s/ /./g")
do
  func=$(echo "$func}" | sed "s/\./ /g" | sed "s/\n//g")
  echo "$func" >> $goal
  echo >> $goal
done
