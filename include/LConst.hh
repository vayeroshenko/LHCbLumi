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

	const G4double worldSizeX = 2*m;
	const G4double worldSizeY = 2*m;
	const G4double worldSizeZ = 250*m;

	const G4double StB2Height = 600*mm;
	const G4double StB2Width = 600*mm;
	const G4double StB2Thickness = 5*mm;
	const G4double StB2PozZ = -114 * m;

	const G4double StB1Height = 600*mm;
	const G4double StB1Width = 600*mm;
	const G4double StB1Thickness = 5*mm;
	const G4double StB1PozZ = -19.7 * m;

	const G4double StB0Height = 600*mm;
	const G4double StB0Width = 600*mm;
	const G4double StB0Thickness = 5*mm;
	const G4double StB0PozZ = -7.5 * m;

	const G4double StF1Height = 600*mm;
	const G4double StF1Width = 600*mm;
	const G4double StF1Thickness = 5*mm;
	const G4double StF1PozZ = 20 * m;

	const G4double StF2Height = 600*mm;
	const G4double StF2Width = 600*mm;
	const G4double StF2Thickness = 5*mm;
	const G4double StF2PozZ = 114 * m;



}










#endif /* INCLUDE_LCONST_HH_ */
