#!/bin/bash

source /cvmfs/sft.cern.ch/lcg/views/LCG_95/x86_64-centos7-gcc8-opt/setup.sh

#source /cvmfs/sft.cern.ch/lcg/views/LCG_95/x86_64-centos7-gcc8-opt/bin/geant4.sh
#export LD_LIBRARY_PATH=/cvmfs/sft.cern.ch/lcg/releases/java/8u91-ae32f/x86_64-centos7-gcc8-opt/jre/lib/amd64:/cvmfs/sft.cern.ch/lcg/views/LCG_95/x86_64-centos7-gcc8-opt/lib64:/cvmfs/sft.cern.ch/lcg/views/LCG_95/x86_64-centos7-gcc8-opt/lib:/cvmfs/sft.cern.ch/lcg/releases/gcc/8.2.0-3fa06/x86_64-centos7/lib:/cvmfs/sft.cern.ch/lcg/releases/gcc/8.2.0-3fa06/x86_64-centos7/lib64:/cvmfs/sft.cern.ch/lcg/releases/binutils/2.30-e5b21/x86_64-centos7/lib:/lib:/afs/cern.ch/sw/lcg/external/Python/2.7.3/x86_64-slc6-gcc48-opt/lib::/opt/rh/python27/root/usr/lib64

#export G4LEVELGAMMADATA=/cvmfs/geant4.cern.ch/share/data/PhotonEvaporation5.3
#export G4LEDATA=/cvmfs/geant4.cern.ch/share/data/G4EMLOW7.7
#export G4NEUTRONHPDATA=/cvmfs/geant4.cern.ch/share/data/G4NDL4.5
#export G4ENSDFSTATEDATA=/cvmfs/geant4.cern.ch/share/data/G4ENSDFSTATE2.2
#export G4RADIOACTIVEDATA=/cvmfs/geant4.cern.ch/share/data/RadioactiveDecay5.3
#export G4ABLADATA=/cvmfs/geant4.cern.ch/share/data/G4ABLA3.1
#export G4PIIDATA=/cvmfs/geant4.cern.ch/share/data/G4PII1.3
#export G4PARTICLEXSDATA=/cvmfs/geant4.cern.ch/share/data/G4PARTICLEXS1.1
#export G4SAIDXSDATA=/cvmfs/geant4.cern.ch/share/data/G4SAIDDATA2.0
#export G4REALSURFACEDATA=/cvmfs/geant4.cern.ch/share/data/RealSurface2.1.1
#export G4INCLDATA=/cvmfs/geant4.cern.ch/share/data/G4INCL1.0

/afs/cern.ch/work/v/vyeroshe/PLUME_G4/LHCbLumi/build/LHCbLumi vis_file.mac data_$1.root >> ap_output.txt
