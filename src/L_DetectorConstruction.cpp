/*
 * L_DetectorConstruction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_DetectorConstruction.h"


L_DetectorConstruction::L_DetectorConstruction() {
	DefineMateials();

}

L_DetectorConstruction::~L_DetectorConstruction() {

}

G4VPhysicalVolume* L_DetectorConstruction::Construct() {
	DefineMateials();
	return DefineVolumes();
}

void L_DetectorConstruction::ConstructSDandField() {
    HSD = new L_SensitiveDetector("HSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(HSD);

	HStationB2Log->SetSensitiveDetector(HSD);
	HStationB1Log->SetSensitiveDetector(HSD);
	HStationB0Log->SetSensitiveDetector(HSD);
	HStationF2Log->SetSensitiveDetector(HSD);
	HStationF1Log->SetSensitiveDetector(HSD);



	G4cout << "_____________________________________________Detectors are made" << G4endl;
}

void L_DetectorConstruction::DefineMateials() {
	G4String symbol;
	G4double a, z, density;
	G4int ncomponents, natoms;
	G4double fractionmass;

	G4NistManager* man = G4NistManager::Instance();

	G4Element* C =
			new G4Element("Carbon", symbol = "C", z = 6., a = 12.01*g/mole);
	G4Element* N =
			new G4Element("Nitrogen", symbol = "N", z = 7., a = 14.01*g/mole);
	G4Element* O =
			new G4Element("Oxygen", symbol = "O", z = 8., a = 16.00*g/mole);

	G4Material* Air =
			new G4Material("Air", density = 0.000290*mg/cm3, ncomponents = 2);
	Air->AddElement(N, fractionmass = 0.7);
	Air->AddElement(O, fractionmass = 0.3);
	worldMaterial = Air;

	G4Material* Scint = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	ScintMaterial = Scint;

}

G4VPhysicalVolume* L_DetectorConstruction::DefineVolumes(){

	G4VSolid *worldSolid = new G4Box("World",
            LConst::worldSizeX/2,
            LConst::worldSizeY/2,
            LConst::worldSizeZ/2);

	G4LogicalVolume *worldLogical = new G4LogicalVolume(worldSolid,
			worldMaterial,
			"World");

	G4VPhysicalVolume *worldPhysical = new G4PVPlacement(0,
			G4ThreeVector(),
			worldLogical,
			"World",
			0,
			false,
			0);

	G4VSolid *HStationB0Solid = new G4Box("HStationB0",
            LConst::StB0Width/2,
            LConst::StB0Height/2,
            LConst::StB0Thickness/2);
	HStationB0Log = new G4LogicalVolume(HStationB0Solid,
			ScintMaterial,
			"HStationB0");
	G4VPhysicalVolume *HStationB0Phys =  new G4PVPlacement(
			new G4RotationMatrix(),
            G4ThreeVector(0.,0.,LConst::StB0PozZ),
			HStationB0Log,
			"HStationB0",
			worldLogical,
			false,
			0);

	G4VSolid *HStationB1Solid = new G4Box("HStationB1",
            LConst::StB1Width/2,
            LConst::StB1Height/2,
            LConst::StB1Thickness/2);
	HStationB1Log = new G4LogicalVolume(HStationB1Solid,
			ScintMaterial,
			"HStationB1");
	G4VPhysicalVolume *HStationB1Phys =  new G4PVPlacement(
			new G4RotationMatrix(),
            G4ThreeVector(0.,0.,LConst::StB1PozZ),
			HStationB1Log,
			"HStationB1",
			worldLogical,
			false,
			0);

	G4VSolid *HStationB2Solid = new G4Box("HStationB2",
            LConst::StB2Width/2,
            LConst::StB2Height/2,
            LConst::StB2Thickness/2);
	HStationB2Log = new G4LogicalVolume(HStationB2Solid,
			ScintMaterial,
			"HStationB2");
	G4VPhysicalVolume *HStationB2Phys =  new G4PVPlacement(
			new G4RotationMatrix(),
            G4ThreeVector(0.,0.,LConst::StB2PozZ),
			HStationB2Log,
			"HStationB2",
			worldLogical,
			false,
			0);

	G4VSolid *HStationF1Solid = new G4Box("HStationF1",
            LConst::StF1Width/2,
            LConst::StF1Height/2,
            LConst::StF1Thickness/2);
	HStationF1Log = new G4LogicalVolume(HStationF1Solid,
			ScintMaterial,
			"HStationF1");
	G4VPhysicalVolume *HStationF1Phys =  new G4PVPlacement(
			new G4RotationMatrix(),
            G4ThreeVector(0.,0.,LConst::StF1PozZ),
			HStationF1Log,
			"HStationF1",
			worldLogical,
			false,
			0);

	G4VSolid *HStationF2Solid = new G4Box("HStationF2",
            LConst::StF2Width/2,
            LConst::StF2Height/2,
            LConst::StF2Thickness/2);
	HStationF2Log = new G4LogicalVolume(HStationF2Solid,
			ScintMaterial,
			"HStationF2");
	G4VPhysicalVolume *HStationF2Phys =  new G4PVPlacement(
			new G4RotationMatrix(),
            G4ThreeVector(0.,0.,LConst::StF2PozZ),
			HStationF2Log,
			"HStationF2",
			worldLogical,
			false,
			0);

	G4cout << "_____________________________________________Volumes are made" << G4endl;
	return worldPhysical;
}

