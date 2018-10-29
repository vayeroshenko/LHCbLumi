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
    LSD = new L_SensitiveDetector("LSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(LSD);

    LPlaneLog->SetSensitiveDetector(LSD);



//    G4cout << "_____________________________________________Detectors are made" << G4endl;
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

    BPMaterial = man->FindOrBuildMaterial("G4_Be");

    Vacuum = new G4Material( "Galactic", z=1., a=1.01*g/mole, density= universe_mean_density,
                             kStateGas, 2.73*kelvin, 3.e-18*pascal );

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




    G4VSolid *BPSolid = new G4Tubs("BeamPipe",			// name
            LConst::BPInnerRadius,						// inner radius
            LConst::BPOuterRadius,						// outer radius
            LConst::worldSizeZ/2.,						// dZ/2
            0,											// theta start
            twopi);										// theta of sector

    G4LogicalVolume *BPLogical = new G4LogicalVolume(BPSolid,
            BPMaterial,
            "BeamPipe");
    G4VPhysicalVolume *BPPhysical =  new G4PVPlacement(
            new G4RotationMatrix(),
            G4ThreeVector(0.,0.,0.),
            BPLogical,
            "BeamPipe",
            worldLogical,
            false,
            0);


    G4VSolid *BPVSolid = new G4Tubs("BeamPipeVacuum",               // name
            0.,                                                     // inner radius
            LConst::BPInnerRadius,                              	// outer radius
            LConst::worldSizeZ/2.,                                  // dZ/2
            0,                                                      // theta start
            twopi);                                                 // theta of sector

    G4LogicalVolume *BPVLogical = new G4LogicalVolume(BPVSolid,
            Vacuum,
            "BeamPipeVacuum");
    G4VPhysicalVolume *BPVPhysical =  new G4PVPlacement(
            new G4RotationMatrix(),
            G4ThreeVector(0.,0.,0.),
            BPVLogical,
            "BeamPipe",
            worldLogical,
            false,
            0);



    G4VSolid *LSolid= new G4Box("LPlane",
            100*cm,
            100*cm,
            1*mm);
    LPlaneLog = new G4LogicalVolume(LSolid,
            Vacuum,
            "LPlane");
    G4VPhysicalVolume *LPlanePhys =  new G4PVPlacement(
            new G4RotationMatrix(),
            G4ThreeVector(0.,0.,LConst::LpozZ),
            LPlaneLog,
            "LPlane",
            worldLogical,
            false,
            0);





	G4cout << "_____________________________________________Volumes are made" << G4endl;
	return worldPhysical;
}

