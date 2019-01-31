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
    LSD = new L_SensitiveDetector("LSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(LSD);
    DefineMateials();
    return DefineVolumes();
}

void L_DetectorConstruction::ConstructSDandField() {


//    L1PlaneLogInner->SetSensitiveDetector(LSD);
//    L2PlaneLogInner->SetSensitiveDetector(LSD);
//    L1PlaneLogOuter->SetSensitiveDetector(LSD);
//    L2PlaneLogOuter->SetSensitiveDetector(LSD);


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
    INOX = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    Vacuum = new G4Material( "Galactic", z=1., a=1.01*g/mole, density= universe_mean_density,
                             kStateGas, 2.73*kelvin, 3.e-18*pascal );


}

G4VPhysicalVolume* L_DetectorConstruction::DefineVolumes(){

    G4VSolid *worldSolid = new G4Box("World",
                                     LConst::worldSizeX/2,
                                     LConst::worldSizeY/2,
                                     LConst::worldSizeZ/2);

    G4LogicalVolume *worldLogical = new G4LogicalVolume(worldSolid,
                                                        Vacuum, // worldMaterial,
                                                        "World");

    G4VPhysicalVolume *worldPhysical = new G4PVPlacement(0,
                                                         G4ThreeVector(),
                                                         worldLogical,
                                                         "World",
                                                         0,
                                                         false,
                                                         0);

    G4VSolid *ExtSolid = new G4Tubs("ExtTube",	// name
                                    0.,											// inner radius
                                    LConst::BPOuterRadius,						// outer radius
                                    LConst::worldSizeZ/2.,							// dZ/2
                                    0,											// theta start
                                    twopi);										// theta of sector


    G4Transform3D Tr(G4RotationMatrix(), G4ThreeVector(0,0,0));




    G4VSolid *BPSolid = new G4Tubs("BeamPipe",			// name
                                   LConst::BPInnerRadius,						// inner radius
                                   LConst::BPOuterRadius,						// outer radius
                                   (LConst::worldSizeZ/2. + LConst::BeamStart)/2.,  // dZ/2
                                   0,											// theta start
                                   twopi);										// theta of sector

    G4LogicalVolume *BPLogical = new G4LogicalVolume(BPSolid,
                                                     BPMaterial,
                                                     "BeamPipe");
    G4VPhysicalVolume *BPPhysical =  new G4PVPlacement(
                new G4RotationMatrix(),
                G4ThreeVector(0.,0., (- LConst::worldSizeZ/2. + LConst::BeamStart)/2.),
                BPLogical,
                "BeamPipe",
                worldLogical,
                false,
                0);

    G4VSolid *VELOsphere = new G4Sphere("VELOsphere",
                                        LConst::sphereInnerR,
                                        LConst::sphereOuterR,
                                        0,
                                        twopi, //LConst::sphereTheta*2.,
                                        0,
                                        LConst::sphereTheta);
    G4SubtractionSolid *VELOsphereSolid = new G4SubtractionSolid("VELOsphere", VELOsphere, ExtSolid);

    G4LogicalVolume *VELOsphereLog = new G4LogicalVolume(VELOsphereSolid,
                                                         INOX,
                                                         "VELOsphere");

    G4RotationMatrix *Rm = new G4RotationMatrix();
    Rm->rotateX(pi);

    G4VPhysicalVolume *VELOspherePhys =  new G4PVPlacement(
                Rm,
                G4ThreeVector(0.,0., (LConst::sphereCenter)),
                VELOsphereLog,
                "VELOsphere",
                worldLogical,
                false,
                0);




    //    G4VSolid *BPVSolid = new G4Tubs("BeamPipeVacuum",               // name
    //            0.,                                                     // inner radius
    //            LConst::BPInnerRadius,                              	// outer radius
    //            (LConst::worldSizeZ/2. + LConst::BeamStart)/2.,                                  // dZ/2
    //            0,                                                      // theta start
    //            twopi);                                                 // theta of sector

    //    G4LogicalVolume *BPVLogical = new G4LogicalVolume(BPVSolid,
    //            Vacuum,
    //            "BeamPipeVacuum");
    //    G4VPhysicalVolume *BPVPhysical =  new G4PVPlacement(
    //            new G4RotationMatrix(),
    //            G4ThreeVector(0.,0.,(LConst::worldSizeZ/2. + LConst::BeamStart)/2.),
    //            BPVLogical,
    //            "BeamPipe",
    //            worldLogical,
    //            false,
    //            0);


    ///////////////////////////////////////////////////////
    G4VSolid *L1SolidPlane= new G4Box("L1Plane",
                                      100*cm,
                                      100*cm,
                                      1*mm);
    G4SubtractionSolid *L1Solid = new G4SubtractionSolid("L1Plane",
                                                         L1SolidPlane,
                                                         ExtSolid);
    L1PlaneLogOuter = new G4LogicalVolume(L1Solid,
                                          Vacuum,
                                          "L1PlaneOuter");
    //    G4VPhysicalVolume *L1PlanePhysOuter =  new G4PVPlacement(
    //                new G4RotationMatrix(),
    //                G4ThreeVector(0.,0.,LConst::L1pozZ),
    //                L1PlaneLogOuter,
    //                "L1PlaneOuter",
    //                worldLogical,
    //                false,
    //                0);
    ///////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////
    G4VSolid *L1SolidIn= new G4Tubs("L1Plane",
                                    0.,
                                    LConst::BPOuterRadius,
                                    1*mm,
                                    0,
                                    twopi);
    L1PlaneLogInner = new G4LogicalVolume(L1SolidIn,
                                          Vacuum,
                                          "L1PlaneInner");
    //        G4VPhysicalVolume *L1PlanePhysInner =  new G4PVPlacement(
    //                    new G4RotationMatrix(),
    //                    G4ThreeVector(0.,0.,LConst::L1pozZ),
    //                    L1PlaneLogInner,
    //                    "L1PlaneInner",
    //                    worldLogical,
    //                    false,
    //                    0);
    ////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    G4VSolid *L2SolidPlane= new G4Box("L2Plane",
                                      100*cm,
                                      100*cm,
                                      1*mm);
    G4SubtractionSolid *L2Solid = new G4SubtractionSolid("L2Plane",
                                                         L2SolidPlane,
                                                         ExtSolid);
    L2PlaneLogOuter = new G4LogicalVolume(L2Solid,
                                          Vacuum,
                                          "L2PlaneOuter");
//            G4VPhysicalVolume *L2PlanePhysOuter =  new G4PVPlacement(
//                        new G4RotationMatrix(),
//                        G4ThreeVector(0.,0.,LConst::L2pozZ),
//                        L2PlaneLogOuter,
//                        "L2PlaneOuter",
//                        worldLogical,
//                        false,
//                        0);
    ////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////
    G4VSolid *L2SolidIn= new G4Tubs("L2Plane",
                                    0.,
                                    LConst::BPOuterRadius,
                                    1*mm,
                                    0,
                                    twopi);
    L2PlaneLogInner = new G4LogicalVolume(L2SolidIn,
                                          Vacuum,
                                          "L2PlaneInner");
    //        G4VPhysicalVolume *L2PlanePhysInner =  new G4PVPlacement(
    //                    new G4RotationMatrix(),
    //                    G4ThreeVector(0.,0.,LConst::L2pozZ),
    //                    L2PlaneLogInner,
    //                    "L2PlaneInner",
    //                    worldLogical,
    //                    false,
    //                    0);
    ////////////////////////////////////////////////////////


    G4ThreeVector *Ta = new G4ThreeVector(0.,0.,0.);
    G4RotationMatrix *Ra = new G4RotationMatrix();

    G4Trd *secIn = new G4Trd(
                "sectorIn",
                sectorIn.shortSide/2.,
                sectorIn.longSide/2.,
                sectorIn.thickness/2.,
                sectorIn.thickness/2.,
                sectorIn.height/2.
                );

    G4Trd *secOut = new G4Trd(
                "sectorOut",
                sectorOut.shortSide/2.,
                sectorOut.longSide/2.,
                sectorOut.thickness/2.,
                sectorOut.thickness/2.,
                sectorOut.height/2.
                );

    G4Trd *absIn = new G4Trd(
                "absIn",
                absorberIn.shortSide/2.,
                absorberIn.longSide/2.,
                absorberIn.thickness/2.,
                absorberIn.thickness/2.,
                absorberIn.height/2.
                );

    G4Trd *absOut = new G4Trd(
                "absOut",
                absorberOut.shortSide/2.,
                absorberOut.longSide/2.,
                absorberOut.thickness/2.,
                absorberOut.thickness/2.,
                absorberOut.height/2.
                );

    G4VisAttributes *sectorVisAtt = new G4VisAttributes;
    sectorVisAtt->SetColor(G4Color(0,0,1.));

    for (int j = 0; j < LConst::nSecIn; ++j) {
        /////////// sector /////////////
        G4String name = "sector in ";
        name += std::to_string(j+1);
        LSectorIn[j] = new G4LogicalVolume(secIn,
                                           Vacuum,
                                           name);
        LSectorIn[j]->SetSensitiveDetector(LSD);
        Ta = new G4ThreeVector(0.,0.,0.);
        Ra = new G4RotationMatrix();

        Ra->rotateX(-90.*deg);
        Ra->rotateY(- 360./LConst::nSecIn*j *deg - 90.*deg);
        Ta->setX(LConst::centerRadIn * TMath::Cos(360./LConst::nSecIn*j *deg));
        Ta->setZ(LConst::L1pozZ);
        Ta->setY(LConst::centerRadIn * TMath::Sin(360./LConst::nSecIn*j *deg));


        G4VPhysicalVolume *PSecIn =  new G4PVPlacement(
                    Ra,
                    *Ta,
                    LSectorIn[j],
                    name,
                    worldLogical,
                    false,
                    0);
        LSectorIn[j]->SetVisAttributes(sectorVisAtt);
    }


    for (int j = 0; j < LConst::nSecOut; ++j) {
        /////////// sector /////////////
        G4String name = "sector out ";
        name += std::to_string(j+1);
        LSectorOut[j] = new G4LogicalVolume(secOut,
                                            Vacuum,
                                            name);
        LSectorOut[j]->SetSensitiveDetector(LSD);
        Ta = new G4ThreeVector(0.,0.,0.);
        Ra = new G4RotationMatrix();

        Ra->rotateX(-90.*deg);
        Ra->rotateY(- 360./LConst::nSecOut*j *deg - 90*deg);
        Ta->setX(LConst::centerRadOut * TMath::Cos(360./LConst::nSecOut*j *deg));
        Ta->setZ(LConst::L1pozZ);
        Ta->setY(LConst::centerRadOut * TMath::Sin(360./LConst::nSecOut*j *deg));

        G4VPhysicalVolume *PSecOut =  new G4PVPlacement(
                    Ra,
                    *Ta,
                    LSectorOut[j],
                    name,
                    worldLogical,
                    false,
                    0);
        LSectorOut[j]->SetVisAttributes(sectorVisAtt);

    }


    G4cout << "_____________________________________________Volumes are made" << G4endl;
    return worldPhysical;
}

