#!/bin/bash

for k in {1..10}
do
	. runMT.screen.sh
	root -q group.cc
	filenameNew="newData_$k.root"
	filename="data_$k.root"
	mv -v data.root $filename
	mv -v dataNew.root $filenameNew
done
