/*
 * L_RunAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#ifndef SRC_L_RUNACTION_H_
#define SRC_L_RUNACTION_H_

#include <G4UserRunAction.hh>

#include "G4Run.hh"
#include "G4RunManager.hh"

//my
#include "HitDataStructure.hh"
#include "EventDataStructure.hh"
#include "LConst.hh"

//G4
#include "G4Timer.hh"
#include "globals.hh"
#include "G4UserRunAction.hh"

//root
#include "TTree.h"
#include "TFile.h"


class L_RunAction: public G4UserRunAction {
public:
    L_RunAction();
    virtual ~L_RunAction();
public:
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void   EndOfRunAction(const G4Run* run);

public:
    void SetOutputFileName(G4String fileName) {_outputFileName = fileName;}
    G4String GetOutputFileName() { return _outputFileName;}
    TTree* tree;
    HitData HitInfo;
    EventData EventInfo;
    G4int _nPart;
    G4int _EventID;
    G4int _nSec;
    static const G4int _nPartMax = 200000;
    G4int _TrackID[_nPartMax];
    G4int _ParentID[_nPartMax];
    G4int _PdgID[_nPartMax];
    G4int _StationID[_nPartMax];
    G4double _Energy[_nPartMax];
    G4double _Time[_nPartMax];
    G4double _X[_nPartMax];
    G4double _Y[_nPartMax];
    G4double _Z[_nPartMax];
    G4double _Px[_nPartMax];
    G4double _Py[_nPartMax];
    G4double _Pz[_nPartMax];
    G4double _Momentum[_nPartMax];
    G4int _nPhot[LConst::nSectors];


    G4double _birthX[_nPartMax];
    G4double _birthY[_nPartMax];
    G4double _birthZ[_nPartMax];

    G4bool _isPrimary[_nPartMax];
    G4double _exitAngles[_nPartMax];
    G4double _entranceAngles[_nPartMax];
    G4double _enttranceAngles[_nPartMax];
    G4int _nRefl[_nPartMax];



private:
    G4Timer* timer;
    TFile* hfile;
    G4String _outputFileName = "data.root";
};

#endif /* SRC_L_RUNACTION_H_ */
