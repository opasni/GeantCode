!# /bin/bash

for i in `seq 0 0.1 1.5`; do
  echo $i | ./project3 run1.mac
  echo $i >> data;
  root -q -b getNumber.c | grep "Elektron\|Proton" >> data;
done
