#ifndef L_VOLUMESTRUCTURES_H
#define L_VOLUMESTRUCTURES_H

//G4
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Point3D.hh"
#include "G4TwoVector.hh"
#include "globals.hh"

#include "TMath.h"

//My
#include "LConst.hh"

struct VolumeStruct {
    G4Material*        material;
    G4VSolid*          solid;
    G4LogicalVolume*   logical;
    G4VPhysicalVolume* physical;
    VolumeStruct() :
        material(0),
        solid(0),
        logical(0),
        physical(0)
    {;}
    ~VolumeStruct() {;}
};

struct TrapezeSectorStructIn: VolumeStruct {
    const G4double thickness;
    const G4double height;
    const G4double shortSide;
    const G4double longSide;
    const G4double angle;
    const G4double sides;
    const G4double middleLine;
    TrapezeSectorStructIn():
        thickness(LConst::sectorThicknessIn),
        height(LConst::outerRadIn * TMath::Cos(TMath::Pi() / LConst::nSecIn) -
               LConst::innerRadIn * TMath::Cos(TMath::Pi() / LConst::nSecIn)),
        shortSide(LConst::innerSideIn),
        longSide(LConst::outerSideIn),
        angle(atan((longSide-shortSide)/2./height)),
        sides(sqrt(height*height + (longSide-shortSide)*(longSide-shortSide)/4.)),
        middleLine((longSide+shortSide)/2.)
    {;}
};

struct TrapezeSectorStructOut: VolumeStruct {
    const G4double thickness;
    const G4double height;
    const G4double shortSide;
    const G4double longSide;
    const G4double angle;
    const G4double sides;
    const G4double middleLine;
    TrapezeSectorStructOut():
        thickness(LConst::sectorThicknessOut),
        height(LConst::outerRadOut * TMath::Cos(TMath::Pi() / LConst::nSecOut) -
               LConst::innerRadOut * TMath::Cos(TMath::Pi() / LConst::nSecOut)),
        shortSide(LConst::innerSideOut),
        longSide(LConst::outerSideOut),
        angle(atan((longSide-shortSide)/2./height)),
        sides(sqrt(height*height + (longSide-shortSide)*(longSide-shortSide)/4.)),
        middleLine((longSide+shortSide)/2.)
    {;}
};

struct TrapezeAbsStructIn: VolumeStruct {
  const G4double shortSide;
  const G4double longSide;
  const G4double thickness;
  const G4double height;
  const G4double angle;
  const G4double sides;
  const G4double middleLine;
  TrapezeAbsStructIn():
    shortSide(LConst::absInnerSideIn),
    longSide(LConst::absOuterSideIn),
    thickness(LConst::sectorThicknessIn),
    height(LConst::outerRadIn -
           LConst::innerRadIn),
    angle(atan((longSide-shortSide)/2./height)),
    sides(sqrt(height*height + (longSide-shortSide)*(longSide-shortSide)/4.)),
    middleLine((longSide+shortSide)/2.)
  {;}
};

struct TrapezeAbsStructOut: VolumeStruct {
  const G4double shortSide;
  const G4double longSide;
  const G4double thickness;
  const G4double height;
  const G4double angle;
  const G4double sides;
  const G4double middleLine;
  TrapezeAbsStructOut():
    shortSide(LConst::absInnerSideOut),
    longSide(LConst::absOuterSideOut),
    thickness(LConst::sectorThicknessOut * 1.2),
    height(LConst::outerRadOut -
           LConst::innerRadOut),
    angle(atan((longSide-shortSide)/2./height)),
    sides(sqrt(height*height + (longSide-shortSide)*(longSide-shortSide)/4.)),
    middleLine((longSide+shortSide)/2.)
  {;}
};




#endif // L_VOLUMESTRUCTURES_H
