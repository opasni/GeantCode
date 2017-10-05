!# /bin/bash

settfile = "TarMatOn"

# cut -d' ' -f1 TarMatOn  | ./project3 run1.mac
# mkdir TarMatOnF
# mv Project* TarMatOnF

for i in `seq 0 0.5 1`; do
  echo $i | ./project3 run1.mac
  root -q -b getNumber.c  | grep 'Elektron\|Proton' >> data;
done
