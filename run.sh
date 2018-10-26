cd build/
cmake ../
make -j8

cd ../Pythia/
make main -j8
./main > /dev/null

cp events.root ../build/
cd ../build
./herschel vis_file.mac

