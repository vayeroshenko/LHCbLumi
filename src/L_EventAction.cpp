/*
 * L_EventAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_EventAction.h"


#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

//G4
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "globals.hh"

L_EventAction::L_EventAction(L_RunAction* runact,
        L_SteppingAction* steppingAction) :
		runAction(runact), _steppingAction(steppingAction), printModulo(100)
{
	//  theCollectionID = -1;
}

L_EventAction::~L_EventAction() {
}

void L_EventAction::BeginOfEventAction(const G4Event* event)
{

//    G4cout << "BeginOfEventAction" << G4endl;
    G4int eventNum = event->GetEventID();


    // Printing an event number
	if (eventNum%printModulo == 0) {
		G4cout << "\n---> Begin of Event: " << eventNum << G4endl;
	}

    // Setting uo the hit collection to be get in the end of event
	G4String colName;
	theCollectionID =
			G4SDManager::GetSDMpointer()->GetCollectionID("Collection");


    // Reset stepping
	_steppingAction->Reset();
	_steppingAction->ResetPerEvent();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void L_EventAction::EndOfEventAction(const G4Event* event)
{

//    G4cout << "End of event" << G4endl;
	// Print info about end of the event
	G4int eventNum = event->GetEventID();

	//	  if (theCollectionID < 0) return;

	// Get the Hit Collection
	G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    L_HitsCollection * THC = 0;

	G4int nHit = 0;

	if (HCE){
        THC = (L_HitsCollection*)(HCE->GetHC(theCollectionID));
	}

	if (0 == THC) return;

	nHit = THC->entries();

	for (G4int i = 0; i < nHit; i++) {
		runAction->_TrackID[i] = (*THC)[i]->myData.TrackID;
		runAction->_ParentID[i] = (*THC)[i]->myData.ParentID;
        runAction->_Energy[i] = (*THC)[i]->myData.Energy / MeV;
        runAction->_Time[i] = (*THC)[i]->myData.Time / ps;
		runAction->_PdgID[i] = (*THC)[i]->myData.PdgID;
		runAction->_StationID[i] = (*THC)[i]->myData.StationID;
        runAction->_X[i] = (*THC)[i]->myData.X / mm;
        runAction->_Y[i] = (*THC)[i]->myData.Y / mm;
        runAction->_Z[i] = (*THC)[i]->myData.Z / mm;
        runAction->_Px[i] = (*THC)[i]->myData.Px / MeV;
        runAction->_Py[i] = (*THC)[i]->myData.Py / MeV;
        runAction->_Pz[i] = (*THC)[i]->myData.Pz / MeV;
        runAction->_Momentum[i] = (*THC)[i]->myData.Momentum / MeV;

        runAction->_birthX[i] = (*THC)[i]->myData.birthX / mm;
        runAction->_birthY[i] = (*THC)[i]->myData.birthY / mm;
        runAction->_birthZ[i] = (*THC)[i]->myData.birthZ / mm;

        runAction->_isPrimary[i] = (*THC)[i]->myData.isPrimary;
    }

	runAction->_EventID = eventNum;
	runAction->_nPart = nHit;

	runAction->tree->Fill();

	//	G4cout << "End of event" << G4endl;
}
