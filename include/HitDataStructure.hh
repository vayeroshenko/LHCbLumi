#ifndef HitDataStructure_h
#define HitDataStructure_h 1

#include "globals.hh"

struct HitData
{
    G4int TrackID;
    G4int ParentID;
    G4int PdgID;
    G4int StationID;
    G4double Energy;
    G4double Time;
    G4double X;
    G4double Y;
    G4double Z;
    G4double Px;
    G4double Py;
    G4double Pz;
    G4double Momentum;

    G4bool isPrimary;
    G4double birthX;
    G4double birthY;
    G4double birthZ;

    G4double grannyID;

};

#endif
