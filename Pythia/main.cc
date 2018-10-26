// Stdlib header file for input and output.
#include <iostream>

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"


// ROOT, for interactive graphics.
#include "TROOT.h"
#include "TH1.h"
#include "TH2D.h"
#include "TVirtualPad.h"
#include "TPad.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TNtuple.h"


const int arraySize = 20000;
static const Double_t c_light = 299.7924580000;         //mm/ns

typedef struct {
  Float_t index[arraySize];
  Int_t status[arraySize];
  Bool_t IsFinal[arraySize];
  Int_t ID[arraySize];
  Float_t firstDau[arraySize];
  Float_t lastDau[arraySize];
  Int_t Mindex[arraySize];
  Float_t xprod[arraySize];
  Float_t yprod[arraySize];
  Float_t zprod[arraySize];
  Float_t xdecay[arraySize];
  Float_t ydecay[arraySize];
  Float_t zdecay[arraySize];
  Float_t Px[arraySize];
  Float_t Py[arraySize];
  Float_t Pz[arraySize];
  Float_t E[arraySize];
  Float_t mass[arraySize];
  Float_t charge[arraySize];
  Float_t tau[arraySize];
  Int_t Nparticle;
  Int_t Nproj;
  Int_t Ntarg;
  Int_t Ncoll;
  Int_t iEvent;
  Int_t ParentID[arraySize];
} OUTPUTEVENT;
static OUTPUTEVENT ETTree; // TTree for pythia output

using namespace Pythia8;
using namespace std;


int main() {
  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;
  Event& event = pythia.event;
  pythia.readFile("main.cmnd");
  TString outRootFileName = "events.root";

  int    nEvent    = pythia.mode("Main:numberOfEvents");
  //int    nList     = pythia.mode("Main:numberToList");
  //int    nShow     = pythia.mode("Main:timesToShow");
  int    nAbort    = pythia.mode("Main:timesAllowErrors");
  bool   showCS    = pythia.flag("Main:showChangedSettings");
  bool   showAS    = pythia.flag("Main:showAllSettings");
  bool   showCPD   = pythia.flag("Main:showChangedParticleData");
  bool   showAPD   = pythia.flag("Main:showAllParticleData");
  bool   showAStat = pythia.flag("Main:showAllStatistics");
  cout<<"showAStat "<<showAStat<<endl;

  pythia.init();

  TTree* outputTTree = new TTree("Tree","tree from pythia");

  outputTTree->Branch("EventID",  &ETTree.iEvent,  "EventID/I");
  outputTTree->Branch("nParticles",  &ETTree.Nparticle,  "nParticles/I");
  
  outputTTree->Branch("X", ETTree.xdecay, "X[nParticles]/F");
  outputTTree->Branch("Y", ETTree.ydecay, "Y[nParticles]/F");
  outputTTree->Branch("Z", ETTree.zdecay, "Z[nParticles]/F");

  outputTTree->Branch("Px", ETTree.Px, "Px[nParticles]/F");
  outputTTree->Branch("Py", ETTree.Py, "Py[nParticles]/F");
  outputTTree->Branch("Pz", ETTree.Pz, "Pz[nParticles]/F");


  outputTTree->Branch("PDGid", ETTree.ID, "PDGid[nParticles]/I");

  Long64_t nFills = 0;
  int iAbort = 0;
  int i = 0; 

  for(ETTree.iEvent = 0; ETTree.iEvent < nEvent; ETTree.iEvent++) {
    if (!pythia.next()) continue;
    Long64_t particleID = 0;
    for (int i = 3; i < event.size(); ++i){
      if (!event[i].isFinal()) continue;

      ETTree.IsFinal[particleID] = event[i].isFinal();
      ETTree.ID[particleID] = event[i].id();
      ETTree.firstDau[particleID] = event[i].daughter1();
      ETTree.lastDau[particleID] = event[i].daughter2();
      ETTree.Mindex[particleID] = event[i].mother1();
      ETTree.xprod[particleID] = event[i].xProd();
      ETTree.yprod[particleID] = event[i].yProd();
      ETTree.zprod[particleID] = event[i].zProd();
      ETTree.xdecay[particleID] = event[i].xDec();
      ETTree.ydecay[particleID] = event[i].yDec();
      ETTree.zdecay[particleID] = event[i].zDec();
      ETTree.Px[particleID] = event[i].px();
      ETTree.Py[particleID] = event[i].py();
      ETTree.Pz[particleID] = event[i].pz();
      ETTree.E[particleID] = event[i].e();
      ETTree.mass[particleID] = event[i].m();
      ETTree.charge[particleID] = event[i].charge();
      ETTree.tau[particleID] = event[i].tau()/c_light;
      
      particleID++;
    }
    ETTree.Nparticle = particleID;

    outputTTree->Fill();
    nFills++;
  // End of event loop. Statistics. Histogram. Done.
  }

  TFile *outFile = new TFile(outRootFileName.Data(),"RECREATE");

  outputTTree->Write();
  outFile->Close();

  delete outFile;
  delete outputTTree;

  pythia.stat();
  return 0;
}
