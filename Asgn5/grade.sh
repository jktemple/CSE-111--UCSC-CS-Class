#!/bin/bash

#######################################################################
#
# Copyright (C) 2018-2022 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

if [ "$#" -ne 2 ]; then
    echo "USAGE: ./grade.sh asignment-no numbers"
    exit;
fi

num=$2

./check.sh $num | tee check.out
./perf.sh  $num | tee perf.out

make --no-print-directory test | tee test.out
lines=`cat test.out | grep ' lines' |  awk '{print $2}'`
functions=`cat test.out | grep ' functions' |  awk '{print $2}'`
branches=`cat test.out | grep ' branches' |  awk '{print $2}'`
rm test.out

echo ""
echo "##################################################"
echo ""
echo "CSE111 Assignment $1"
echo ""
date

ccode=0
cstr="yes"
valg=0
vstr="n/a"
if [ ! -s make.out ]
then
  echo ""
  echo -n "Checking for memory errors..."
  (( ccode = 5 ))
  cstr="none"
  > valgrind.out
  flags="--track-origins=yes --leak-check=full --show-leak-kinds=all"
  make --no-print-directory valgrind 2>&1 > /dev/null

  valg=`grep 'ERROR SUMMARY' valgrind.out | grep -v 'ERROR SUMMARY: 0' | \
    awk 'BEGIN {sum=0} {sum += $4} END { print sum }'`

  valw=`grep 'Warning: ' valgrind.out | wc -l`

  (( valg += valw ))
  echo ""
fi

pass=`grep PASS check.out | wc -l`
tests=30

pct=`cat check.out | grep 'Tests' | grep '\%' | sed 's/\%//' | awk -F "/" '{print $2}' | awk -F " " '{print $2}'`
ttotal=`echo "scale=2; $pct * 0.4" | bc -l`

ppass=`grep Capped perf.out | grep PASS | wc -l`
ptests=9

ptotal=0
if [ $ptests -ne 0 ]
then
    ptotal=`echo "scale=2; ($ppass / $ptests ) * 30.0" | bc -l`
fi

ccov=0
ccstr="n/a"
if [ $pct = "100.0" ]; then
  ccstr="<100%"
  if [ $lines = "100.0%" ]; then
    if [ $functions = "100.0%" ]; then
      if [ $branches = "100.0%" ] || [ $branches = "no" ]; then
        ccov=20
        ccstr="100%"
      fi
    fi
  fi
fi

errors=0
if (( valg > 0 ))
then
  vstr="$valg"
fi

if (( ccode == 5 && valg == 0 ))
then
  (( errors = 5 ))
  vstr="none"
fi

echo ""
printf "%20s:  %2d/%2d    %4.1f%% of %d%% %s\n" "Functional" $pass $tests $ttotal 40 "} Making it work"
printf "%20s:  %2d/%2d    %4.1f%% of %d%% %s\n" "Non-Functional" $ppass $ptests $ptotal 30 "} Making it fast"
printf "%20s:  %5s   %5.1f%% of %2d%% %s\n" "Code Coverage" $ccstr $ccov 20 "}"
printf "%20s:  %5s   %5.1f%% of %2d%% %s\n" "Compiler Warnings" $cstr $ccode 5 "} Making it right"
printf "%20s:  %5s   %5.1f%% of %2d%% %s\n" "Memory Problems" $vstr $errors 5 "}"

total=`echo "scale=2; $ttotal + $ptotal + $ccov + $ccode + $errors" | bc -l`
printf "\n%20s: %5.1f%%\n\n" "Total" $total 

rm perf.out check.out 2>/dev/null

if (( valg > 0 ))
then
  grep 'ERROR SUMMARY' valgrind.out | grep -v 'ERROR SUMMARY: 0'
  echo ""
  echo "See valgrind.out for details"
  echo ""
fi
