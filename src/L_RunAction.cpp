/*
 * L_RunAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_RunAction.h"


L_RunAction::L_RunAction() {
    //	_outputFileName = "data.root";
    timer = new G4Timer();


    G4cout << "Run action constructor" << G4endl;
}

L_RunAction::~L_RunAction() {
    delete tree;
    delete hfile;
}

void L_RunAction::BeginOfRunAction(const G4Run* run)
{
    //inform the runManager to save random number seed
    //	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    G4cout << "BeginOfRunAction" << G4endl;

    timer->Start();

    // Histogramming

    hfile = new TFile(_outputFileName, "RECREATE", "LHCb Luminometer Simulation Data", 1);

    G4cout << "Output file created" << G4endl;
    if (hfile->IsZombie()) exit(-1);
    tree = new TTree("T", "LHCb Luminometer Data Tree");
    //	tree->SetAutoSave(1000000);

    // Create new event
    TTree::SetBranchStyle(0);

    //Hits

    tree->Branch("nPart", &_nPart, "nPart/I");
    tree->Branch("TrackID", _TrackID, "TrackID[nPart]/I");
    tree->Branch("ParentID", _ParentID, "ParentID[nPart]/I");
    tree->Branch("Energy", _Energy, "Energy[nPart]/D");
    tree->Branch("Time", _Time, "Time[nPart]/D");
    tree->Branch("PdgID",_PdgID, "PdgID[nPart]/I");
    tree->Branch("StationID", _StationID, "StationID[nPart]/I");
    tree->Branch("X", _X, "X[nPart]/D");
    tree->Branch("Y", _Y, "Y[nPart]/D");
    tree->Branch("Z", _Z, "Z[nPart]/D");
    tree->Branch("Momentum", _Momentum, "Momentum[nPart]/D");
    tree->Branch("Px", _Px, "Px[nPart]/D");
    tree->Branch("Py", _Py, "Py[nPart]/D");
    tree->Branch("Pz", _Pz, "Pz[nPart]/D");

    tree->Branch("birthX", _birthX, "birthX[nPart]/D");
    tree->Branch("birthY", _birthY, "birthY[nPart]/D");
    tree->Branch("birthZ", _birthZ, "birthZ[nPart]/D");


    tree->Branch("grannyID", _grannyID, "grannyID[nPart]/I");
    tree->Branch("isPrimary", _isPrimary, "isPrimary[nPart]/O");

    G4cout << "BeginOfRunAction end" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void L_RunAction::EndOfRunAction(const G4Run* )
{
    //	hfile = tree->GetCurrentFile();

    G4cout << "End of run action" << G4endl;
    tree->Write();
    hfile->Write();
    tree->Print();
    timer->Stop();
    hfile->Close();

    G4cout << "Time: " << *timer << G4endl;

}
