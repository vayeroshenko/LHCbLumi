#!/bin/bash

rm log.txt
cd ../build
cmake ../ > /dev/null
make > /dev/null
cd ../analysis

for i in {1..5}
do
	screen -S "Lumi_$i" -L -d -m ./runMT.sh $i
done

cd ../analysis

while [ $(screen -ls | grep Lumi | wc -l) -ne 0 ]
do
	sleep 10
done

python2 draw.py

