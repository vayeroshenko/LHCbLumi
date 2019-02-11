#!/bin/bash


for i in {1..2}
do
	cd ../build
	name="data_$1.root"
	./LHCbLumi vis_file.mac $name > /dev/null #| grep Random:seed
	cp data.root ../analysis
	cd ../analysis
	# rm dataNew.root
	root -b -q -l group.cc  > /dev/null
	root -b -q -l ana2.cc | grep Mean | awk '{print $3}' | tee -a log.txt
done
