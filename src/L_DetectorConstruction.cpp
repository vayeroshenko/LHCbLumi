/*
 * L_DetectorConstruction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_DetectorConstruction.h"


L_DetectorConstruction::L_DetectorConstruction() {
    //    DefineMateials();

}

L_DetectorConstruction::~L_DetectorConstruction() {

}

G4VPhysicalVolume* L_DetectorConstruction::Construct() {
    LSD = new L_SensitiveDetector("LSD", _eventAction);
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
// Defining materials
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
    G4Element* Si =
            new G4Element("Silicon", symbol = "Si", z = 14., a = 28.09*g/mole);
    Air =
            new G4Material("Air", density = 0.000290*mg/cm3, ncomponents = 2);
    Air->AddElement(N, fractionmass = 0.7);
    Air->AddElement(O, fractionmass = 0.3);
    worldMaterial = Air;

    G4Material* Scint = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    ScintMaterial = Scint;

    INOX = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    Beryllium = man->FindOrBuildMaterial("G4_Be");
    Copper = man->FindOrBuildMaterial("G4_Cu");

    BPMaterial = Copper;
    //    BPMaterial = Beryllium;


    Vacuum = new G4Material( "Galactic", z=1., a=1.01*g/mole, density= universe_mean_density,
                             kStateGas, 2.73*kelvin, 3.e-18*pascal );

    // Quartz Material (SiO2)
    SiO2 =
            new G4Material("quartz", density = 2.200*g/cm3, ncomponents = 2);
    SiO2->AddElement(Si, natoms = 1);
    SiO2->AddElement(O , natoms = 2);



    //
    // Generate and Add Material Properties Table
    //
    const G4int num = 36;
    G4double WaveLength[num];
    G4double Absorption[num];      // Default value for absorption
    G4double AirAbsorption[num];
    G4double AirRefractiveIndex[num];
    G4double PhotonEnergy[num];

    // Absorption of quartz per 1m
    G4double QuartzAbsorption[num] =
    {0.999572036,0.999544661,0.999515062,0.999483019,0.999448285,
     0.999410586,0.999369611,0.999325013,0.999276402,0.999223336,
     0.999165317,0.999101778,0.999032079,0.998955488,0.998871172,
     0.998778177,0.99867541 ,0.998561611,0.998435332,0.998294892,
     0.998138345,0.997963425,0.997767484,0.997547418,0.99729958 ,
     0.99701966 ,0.99670255 ,0.996342167,0.995931242,0.995461041,
     0.994921022,0.994298396,0.993577567,0.992739402,0.991760297,
     0.990610945};

    for (int i=0; i<num; i++) {
        WaveLength[i] = (300 + i*10)*nanometer;
        Absorption[i] = 100*m;      // Fake number for no absorption
        AirAbsorption[i] = 10.*cm;   // If photon hits air, kill it
        AirRefractiveIndex[i] = 1.; // Rough air refraction
        PhotonEnergy[num - (i+1)] = twopi*hbarc/WaveLength[i];
        /* Absorption is given per length and G4 needs mean free path
         length, calculate it here
         mean free path length - taken as probablility equal 1/e
         that the photon will be absorbed */
        QuartzAbsorption[i] = (-1)/log(QuartzAbsorption[i])*100*cm;
        //EpotekAbsorption[i] = (-1)/log(EpotekAbsorption[i])*
        //epotekBarJoint.thickness;
    }

    G4double QuartzRefractiveIndex[num] =
    {1.456535,1.456812,1.4571  ,1.457399,1.457712,1.458038,
     1.458378,1.458735,1.459108,1.4595  ,1.459911,1.460344,
     1.460799,1.46128 ,1.461789,1.462326,1.462897,1.463502,
     1.464146,1.464833,1.465566,1.46635 ,1.46719 ,1.468094,
     1.469066,1.470116,1.471252,1.472485,1.473826,1.475289,
     1.476891,1.478651,1.480592,1.482739,1.485127,1.487793};



    // Assign absorption and refraction to materials

    // Quartz
    G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
    QuartzMPT->AddProperty("RINDEX", PhotonEnergy, QuartzRefractiveIndex, num);
    QuartzMPT->AddProperty("ABSLENGTH", PhotonEnergy, QuartzAbsorption, num);

    SiO2->SetMaterialPropertiesTable(QuartzMPT);

    // Air
    G4MaterialPropertiesTable* AirMPT = new G4MaterialPropertiesTable();
    AirMPT->AddProperty("RINDEX", PhotonEnergy, AirRefractiveIndex, num);
    AirMPT->AddProperty("ABSLENGTH", PhotonEnergy, AirAbsorption, num);

    // Assign these properties to the world volume
    Air->SetMaterialPropertiesTable(AirMPT);
    // In our rough assumption
    Vacuum->SetMaterialPropertiesTable(AirMPT);
}

G4VPhysicalVolume* L_DetectorConstruction::DefineVolumes(){

    //////////////////////// World /////////////////////////////
    G4VSolid *worldSolid = new G4Box("World",
                                     LConst::worldSizeX/2,
                                     LConst::worldSizeY/2,
                                     LConst::worldSizeZ/2);

    worldLogical = new G4LogicalVolume(worldSolid,
                                       Vacuum /*Air*/, // worldMaterial,
                                       "World");

    G4VPhysicalVolume *worldPhysical = new G4PVPlacement(0,
                                                         G4ThreeVector(),
                                                         worldLogical,
                                                         "World",
                                                         0,
                                                         false,
                                                         0);
    ///////////////////////////////////////////////////////////


    // The solid to be extracted from VELO vessel upstream cap
    // in order to avoid overlap with beampipe
    G4VSolid *ExtSolid = new G4Tubs("ExtTube",	// name
                                    0.,											// inner radius
                                    LConst::BPOuterRadius,						// outer radius
                                    LConst::worldSizeZ/2.,							// dZ/2
                                    0,											// theta start
                                    twopi);										// theta of sector


    ////////////////////// Beampipe ////////////////////////////
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
    ////////////////////////////////////////////////////////////


    ////////////////////// VELO cap ////////////////////////////
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
    ////////////////////////////////////////////////////////////


    G4ThreeVector *Ta = new G4ThreeVector(0.,0.,0.);
    G4RotationMatrix *Ra = new G4RotationMatrix();

    ////////////// Trapeze sectors and absorbers ///////////////


    G4Tubs *secOut = new G4Tubs(
                "sectorOut",
                0.,
                pmt_window.radius,
                pmt_window.thickness / 2.,
                0.,
                twopi);



    ////////////////////////////////////////////////////////////

    ///////////// Photon detector at the surface ///////////////

    G4VSolid *detectorOut = new G4Tubs(
                "sectorOut",
                0.,
                pmt_detector.radius,
                pmt_detector.thickness / 2.,
                0.,
                twopi);

    ////////////////////////////////////////////////////////////


    G4RotationMatrix RTilt = G4RotationMatrix();
    RTilt.rotateX(LConst::pmt_angle);


    G4Transform3D Tr;

    // Assembly
    G4AssemblyVolume *assembly = new G4AssemblyVolume();
    G4String name;


    // Loop for sectors in order to place them with detectors and absorbers
    for (int j = 0; j < LConst::pmt_n_channels; ++j) {
        /////////// sector /////////////
        name = "sector out ";
        name += std::to_string(j+1);
        LSectorOut[j] = new G4LogicalVolume(secOut,
                                            /*Vacuum*/SiO2,
                                            name);
        LSectorOut[j]->SetSensitiveDetector(LSD);
        Ta = new G4ThreeVector(0.,0.,0.);
        Ra = new G4RotationMatrix();

//        Ra->rotateX(90.*deg);
        Ra->rotateY(- 360./LConst::pmt_n_channels*j *deg + 90*deg);
        *Ra = *Ra * RTilt;

        Ta->setX(LConst::pmt_center_rad * TMath::Cos(360./LConst::pmt_n_channels*j *deg));
        Ta->setY(LConst::pmt_window_pos_z);
        Ta->setZ(LConst::pmt_center_rad * TMath::Sin(360./LConst::pmt_n_channels*j *deg));

        Tr = G4Transform3D(*Ra, *Ta);
        //        if (j == 0)
        assembly->AddPlacedVolume(LSectorOut[j], Tr);


        /////////// outer detector ///////

        name = "detector out ";
        name += std::to_string(j+1);

        Ta = new G4ThreeVector(0.,0.,0.);
        Ra = new G4RotationMatrix();

        Ra->rotateY(- 360./LConst::pmt_n_channels*j *deg + 90.*deg);


//        Ta->setX((LConst::pmt_detector_rad *TMath::Cos(TMath::Pi() / LConst::pmt_n_channels)) * TMath::Cos(360./LConst::pmt_n_channels*j *deg));
//        Ta->setY(LConst::L1pozZ);
//        Ta->setZ((LConst::pmt_detector_rad*TMath::Cos(TMath::Pi() / LConst::pmt_n_channels)) * TMath::Sin(360./LConst::pmt_n_channels*j *deg));

        Ta->setX((LConst::pmt_detector_rad) * TMath::Cos(360./LConst::pmt_n_channels*j *deg));
        Ta->setY(LConst::pmt_window_pos_z);
        Ta->setZ((LConst::pmt_detector_rad) * TMath::Sin(360./LConst::pmt_n_channels*j *deg));


        *Ta -= G4ThreeVector( LConst::pmt_center_rad * TMath::Cos(360./LConst::pmt_n_channels*j *deg),
                              LConst::pmt_window_pos_z,
                              LConst::pmt_center_rad * TMath::Sin(360./LConst::pmt_n_channels*j *deg));

        *Ta = (*Ra * (RTilt * (Ra->inverse() * (*Ta))));

        *Ta += G4ThreeVector( LConst::pmt_center_rad * TMath::Cos(360./LConst::pmt_n_channels*j *deg),
                              LConst::pmt_window_pos_z,
                              LConst::pmt_center_rad * TMath::Sin(360./LConst::pmt_n_channels*j *deg));

        *Ra = *Ra * RTilt;
        Tr = G4Transform3D(*Ra,*Ta);

        LDetectorOut[j] = new G4LogicalVolume(detectorOut,
                                              /*Vacuum*/SiO2,
                                              name);
        //        if (j == 0)
        assembly->AddPlacedVolume(LDetectorOut[j],Tr);



        /////////// sector /////////////
        name = "sector out ";
        name += std::to_string(-j-1);
        LSectorOut[LConst::pmt_n_channels+j] = new G4LogicalVolume(secOut,
                                            /*Vacuum*/SiO2,
                                            name);
        LSectorOut[LConst::pmt_n_channels+j]->SetSensitiveDetector(LSD);
        Ta = new G4ThreeVector(0.,0.,0.);
        Ra = new G4RotationMatrix();

//        Ra->rotateX(90.*deg);
        Ra->rotateY(- 360./LConst::pmt_n_channels*j *deg + 90*deg);
        *Ra = *Ra * RTilt;

        Ta->setX(LConst::pmt_center_rad_1 * TMath::Cos(360./LConst::pmt_n_channels*j *deg));
        Ta->setY(LConst::pmt_window_pos_z_1);
        Ta->setZ(LConst::pmt_center_rad_1 * TMath::Sin(360./LConst::pmt_n_channels*j *deg));

        Tr = G4Transform3D(*Ra, *Ta);
        //        if (j == 0)
        assembly->AddPlacedVolume(LSectorOut[LConst::pmt_n_channels+j], Tr);


        /////////// outer detector ///////

        name = "detector out ";
        name += std::to_string(-j-1);

        Ta = new G4ThreeVector(0.,0.,0.);
        Ra = new G4RotationMatrix();

        Ra->rotateY(- 360./LConst::pmt_n_channels*j *deg + 90.*deg);


//        Ta->setX((LConst::pmt_detector_rad *TMath::Cos(TMath::Pi() / LConst::pmt_n_channels)) * TMath::Cos(360./LConst::pmt_n_channels*j *deg));
//        Ta->setY(LConst::L1pozZ);
//        Ta->setZ((LConst::pmt_detector_rad*TMath::Cos(TMath::Pi() / LConst::pmt_n_channels)) * TMath::Sin(360./LConst::pmt_n_channels*j *deg));

        Ta->setX((LConst::pmt_detector_rad_1) * TMath::Cos(360./LConst::pmt_n_channels*j *deg));
        Ta->setY(LConst::pmt_window_pos_z_1);
        Ta->setZ((LConst::pmt_detector_rad_1) * TMath::Sin(360./LConst::pmt_n_channels*j *deg));


        *Ta -= G4ThreeVector( LConst::pmt_center_rad_1 * TMath::Cos(360./LConst::pmt_n_channels*j *deg),
                              LConst::pmt_window_pos_z_1,
                              LConst::pmt_center_rad_1 * TMath::Sin(360./LConst::pmt_n_channels*j *deg));

        *Ta = (*Ra * (RTilt * (Ra->inverse() * (*Ta))));

        *Ta += G4ThreeVector( LConst::pmt_center_rad_1 * TMath::Cos(360./LConst::pmt_n_channels*j *deg),
                              LConst::pmt_window_pos_z_1,
                              LConst::pmt_center_rad_1 * TMath::Sin(360./LConst::pmt_n_channels*j *deg));

        *Ra = *Ra * RTilt;
        Tr = G4Transform3D(*Ra,*Ta);

        LDetectorOut[LConst::pmt_n_channels+j] = new G4LogicalVolume(detectorOut,
                                              /*Vacuum*/SiO2,
                                              name);
        //        if (j == 0)
        assembly->AddPlacedVolume(LDetectorOut[LConst::pmt_n_channels+j],Tr);



    }

    Ra = new G4RotationMatrix();
    Ta = new G4ThreeVector();


    // Managing the final position of the assembly

    Ra->rotateY(270.0*deg);
    Ra->rotateX(90.0*deg);

    Ta->setX(0.);
    Ta->setY(0.);
    Ta->setZ(0.);

    //    Ta += G4ThreeVector(fTOFConst::centerRad * TMath::Cos(360./fTOFConst::nSec*i *deg),
    //                        0,
    //                        fTOFConst::centerRad * TMath::Sin(360./fTOFConst::nSec*i *deg));

    Ta->rotateZ(270*deg);

    Tr = G4Transform3D(*Ra,*Ta);

    assembly->MakeImprint(worldLogical, Tr, 0, true);

    DefineOpticalBorders();
    SetVisAttributes();


    G4cout << "_____________________________________________Volumes are made" << G4endl;
    return worldPhysical;
}


// Definition of absorbtion surfaces
void L_DetectorConstruction::DefineOpticalBorders()
{
    const G4int num1 = 2;
    G4double Ephoton[num1] = {1.5*eV, 5.8*eV};

    G4OpticalSurface* OpVolumeKillSurface =
            new G4OpticalSurface("VolumeKillSurface");
    OpVolumeKillSurface->SetType(dielectric_metal);
    OpVolumeKillSurface->SetFinish(polished);
    OpVolumeKillSurface->SetModel(glisur);

    G4double ReflectivityKill[num1] = {0., 0.};
    G4double EfficiencyKill[num1] = {1., 1.};
    G4MaterialPropertiesTable* VolumeKill = new G4MaterialPropertiesTable();
    VolumeKill->AddProperty("REFLECTIVITY", Ephoton, ReflectivityKill, num1);
    VolumeKill->AddProperty("EFFICIENCY",   Ephoton, EfficiencyKill,   num1);
    OpVolumeKillSurface->SetMaterialPropertiesTable(VolumeKill);

    G4OpticalSurface* quartzSurface = new G4OpticalSurface("quartzBorder");
    quartzSurface->SetType(dielectric_dielectric);

    for (int j = 0; j < LConst::pmt_n_channels; ++j) {
        new G4LogicalSkinSurface("DetectorAbsSurface",
                                 LDetectorOut[j], OpVolumeKillSurface);
        new G4LogicalSkinSurface("sectorSurface",
                                 LSectorOut[j], quartzSurface);
    }



}

// Visual attributes
void L_DetectorConstruction::SetVisAttributes()
{
    G4Color blue        = G4Color(0., 0., 1.);
    G4Color green       = G4Color(0., 1., 0.);
    G4Color red         = G4Color(1., 0., 0.);
    G4Color white       = G4Color(1., 1., 1.);
    G4Color cyan        = G4Color(0., 1., 1.);
    G4Color DircColor   = G4Color(0.0, 0.0, 1.0, 0.2);
    G4Color SensColor   = G4Color(0.0, 1.0, 1.0, 0.1);

    G4VisAttributes *sectorVisAtt = new G4VisAttributes;
    sectorVisAtt->SetColor(green);
    sectorVisAtt->SetVisibility(true);
    for (int j = 0; j < LConst::pmt_n_channels; ++j) {
        LSectorOut[j]->SetVisAttributes(sectorVisAtt);
    }
}
