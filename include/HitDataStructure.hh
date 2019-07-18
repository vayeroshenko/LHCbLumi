#ifndef HitDataStructure_h
#define HitDataStructure_h 1

#include "globals.hh"
#include <vector>

struct HitData
{
//  G4int TrackID;
//  G4int ParentID;
//  G4int PdgID;
  G4int StationID;
//  G4double Energy;
//  G4double Time;
  G4double X;
  G4double Y;
  G4double Z;

  G4int nRefl;
  std::vector<G4double> angle_tir;
  std::vector<G4double> angle_fr;
  std::vector<G4double> angle_refr;
//  std::vector<G4double> incidenceAngles;
//  G4double Px;
//  G4double Py;
//  G4double Pz;
//  G4double Momentum;
};

#endif
