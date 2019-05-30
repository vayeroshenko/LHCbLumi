/*
 * LConst.hh
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#ifndef INCLUDE_LCONST_HH_
#define INCLUDE_LCONST_HH_

#include "TMath.h"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"

namespace LConst{

const G4double worldSizeX = 3*m;
const G4double worldSizeY = 3*m;
const G4double worldSizeZ = 8*m;


const G4double BPOuterRadius = 31*mm;
const G4double BPInnerRadius = 30*mm;

  //const G4double L1pozZ = - 2172.*mm;
//    const G4double L1pozZ = - 2172.*mm + 400*mm;
const G4double L1pozZ = - 2172.*mm + 800*mm;

//    const G4double L2pozZ = - 2172.*mm + 800*mm;

//    const G4double BeamStart = - 879.*mm;

// VELO Sphere
const G4double capHeight = 280.*mm;
const G4double capBaseR = 1000./2. *mm;
const G4double sphereStart = - 500.*mm;
const G4double sphereThickness = 5.*mm;

const G4double sphereR = (capHeight*capHeight + capBaseR*capBaseR)/2. / capHeight;
const G4double sphereTheta = TMath::ATan(capBaseR/2. / (sphereR - capHeight));
const G4double sphereOuterR = sphereR;
const G4double sphereInnerR = sphereR - sphereThickness;
const G4double sphereCenter = sphereStart - capHeight + sphereR;

const G4double BeamStart = sphereStart - capHeight + 32.*mm;

const G4double VeloLeft = -175*mm;
const G4double VeloRight = 350*mm;





///////////////////////// Detector inside beampipe ///////////////////////////////////////////////////
const G4double sectorThicknessIn = 1*cm;

const G4double innerRadIn = 10*mm;
const G4double outerRadIn = BPInnerRadius;
const G4int nSecIn = 12;

const G4double centerRadIn = (innerRadIn * TMath::Cos(TMath::Pi() / nSecIn) +
                              outerRadIn * TMath::Cos(TMath::Pi() / nSecIn)) / 2.;
const G4double absInnerSideIn = 0.1*mm;
const G4double absOuterSideIn = absInnerSideIn * outerRadIn / innerRadIn;
const G4double VertHeightIn = outerRadIn - innerRadIn;
const G4double innerSideIn = 2. * innerRadIn * TMath::Sin(TMath::Pi() / nSecIn) - absInnerSideIn;
const G4double outerSideIn = 2. * outerRadIn * TMath::Sin(TMath::Pi() / nSecIn) - absOuterSideIn;
/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////// Detoctor outside beampipe ///////////////////////////////////////////////
const G4double sectorThicknessOut = 1*cm;


 const G4double innerRadOut = BPOuterRadius;
 const G4double outerRadOut = 10*cm;
//const G4double innerRadOut = 160*mm;
//const G4double outerRadOut = 100*cm;
//const G4double innerRadOut = 100*cm;
//const G4double outerRadOut = 110*cm;


 const G4int nSecOut = 100;
// const G4int nSecOut = 50;

const G4double angleOut = 0.*deg;

const G4double centerRadOut = (innerRadOut * TMath::Cos(TMath::Pi() / nSecOut) +
                               outerRadOut * TMath::Cos(TMath::Pi() / nSecOut)) / 2.;
const G4double absInnerSideOut = 0.01*mm;
const G4double absOuterSideOut = absInnerSideOut * outerRadOut / innerRadOut * 0.8;
const G4double VertHeightOut = outerRadOut - innerRadOut;
const G4double innerSideOut = 2. * (centerRadOut - VertHeightOut * TMath::Cos(angleOut)/2.) * TMath::Sin(TMath::Pi() / nSecOut) - absInnerSideOut*40;
const G4double outerSideOut = 2. * (centerRadOut + VertHeightOut * TMath::Cos(angleOut)/2.) * TMath::Sin(TMath::Pi() / nSecOut) - absOuterSideOut*20;


const G4double lmLength = 40.*cm;
const G4double lmPixelSize = 5.*mm;


//////////////////////////////////////////////////////////////////////////////////////////////////////////



}










#endif /* INCLUDE_LCONST_HH_ */
