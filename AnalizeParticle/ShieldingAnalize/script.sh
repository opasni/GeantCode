!# /bin/bash
for i in `seq 0.1 0.2 2`; do

  echo $i | ./project3 run2.mac
  root -q -b getNumber.c >> data;
done
