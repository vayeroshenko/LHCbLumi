#!/bin/bash

rm log.txt
cd ../build
cmake ../ > /dev/null
make > /dev/null
cd ../analysis

rm dataNew*
rm data*


for i in {1..8}
do
	mkdir "temp_$i"
	cp groupMT.cc "temp_$i"
	cp ana2MT.cc "temp_$i"
	cp runMT.sh "temp_$i"
	cd "temp_$i"
	screen -S "Lumi_$i" -L -d -m ./runMT.sh $i
	cd ../
done



while [ $(screen -ls | grep Lumi | wc -l) -ne 0 ]
do
	sleep 1
done

touch log.txt

for i in {1..8}
do
	cat "temp_$i/log.txt">>log.txt
done

rm -rf temp*

python2 draw.py

