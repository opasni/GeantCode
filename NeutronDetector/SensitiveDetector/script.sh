!# /bin/bash

setmacro="runT.mac"
# Inverse beta Off
setfile="InvBetaOff"
#
#
endfile=$setfile
endfile+=".root"
noeF="numberofevents"
#
cut -d' ' -f1 $setfile  | ./project3 $setmacro
# cat $setmacro | grep beamOn | awk 'NF>1{print $NF}' > $noe
noe=$(cat runT.mac | grep beamOn | awk 'NF>1{print $NF}')
echo $noe >> $noeF
echo $endfile >> $noeF
cat $noeF | root -q -b joinPR.c
setfolder=$setfile
setfolder+="F"
mkdir $setfolder
mv Project* $setfolder
mv $noeF $setfolder
mv $endfile $setfolder
#
#
# Inverse beta On
setfile="InvBetaOn"
#
#
endfile=$setfile
endfile+=".root"
noeF="numberofevents"
#
cut -d' ' -f1 $setfile  | ./project3 $setmacro
# cat $setmacro | grep beamOn | awk 'NF>1{print $NF}' > $noe
noe=$(cat runT.mac | grep beamOn | awk 'NF>1{print $NF}')
echo $noe >> $noeF
echo $endfile >> $noeF
cat $noeF | root -q -b joinPR.c
setfolder=$setfile
setfolder+="F"
mkdir $setfolder
mv Project* $setfolder
mv $noeF $setfolder
mv $endfile $setfolder
