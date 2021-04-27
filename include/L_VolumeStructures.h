#pragma once

//G4
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Point3D.hh"
#include "G4TwoVector.hh"
#include "globals.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4AssemblyVolume.hh"
#include "G4VSensitiveDetector.hh"

#include "TMath.h"

//My
#include "LConst.hh"

// Base structure
struct VolumeStruct {
    G4String            name;
    G4Material*         material;
    G4VSolid*           solid;
    G4LogicalVolume*    logical;
    VolumeStruct() :
        material(0),
        solid(0),
        logical(0),
        name("")
    {;}
    void SetMaterial(G4Material* mat){
        material = mat;
    }
    void ConstructLogical(){
        logical = new G4LogicalVolume(solid,
                                      material,
                                      name);
    }
    ~VolumeStruct() {
        delete solid, material, logical;
    }
};

struct PMT_window: VolumeStruct {
    const G4double radius;
    const G4double thickness;
    PMT_window():
        radius(LConst::window_radius),
        thickness(LConst::window_thickness)
    {
        solid = new G4Tubs(
                    "sectorOut",
                    0.,
                    radius,
                    thickness / 2.,
                    0.,
                    twopi);
    }
};

struct PMT_tablet: VolumeStruct {
    const G4double radius;
    const G4double thickness;
    PMT_tablet():
        radius(LConst::window_radius),
        thickness(LConst::pmt_detector_thickness)
    {
        solid = new G4Tubs(
                    "sectorOut",
                    0.,
                    radius,
                    thickness / 2.,
                    0.,
                    twopi);
    }
};

struct PMT_body: VolumeStruct {
    const G4double radius;
    const G4double thickness;
    PMT_body():
        radius(LConst::window_radius),
        thickness(LConst::pmt_body_thickness)
    {
        solid = new G4Tubs(
                    "bodyOut",
                    0.,
                    radius,
                    thickness / 2.,
                    0.,
                    twopi);
    }
};

struct PMT_detector: VolumeStruct {
    const G4double radius;
    const G4double thickness;
    PMT_detector():
        radius(LConst::window_radius),
        thickness(LConst::pmt_detector_thickness)
    {
        solid = new G4Tubs(
                    "sectorOut",
                    0.,
                    radius,
                    thickness / 2.,
                    0.,
                    twopi);
    }
};

////// ALL PMTs ARE THE SAME

struct Assembly {
    G4int ID = -1;
    PMT_detector    *detector;
    PMT_window      *window;
    PMT_tablet      *tablet;
    PMT_body        *body;
    G4double angle;
    G4double pos_z;
    G4double phi;

    G4double pmt_center_rad;
    G4double pmt_detector_rad;
    G4double pmt_body_rad;

    G4VSensitiveDetector* sensitive;

    Assembly() {
        detector = new PMT_detector;
        window =   new PMT_window;
        tablet =   new PMT_tablet;
        body =     new PMT_body;
    }
//    Assembly() {
//        detector    = det_glob;
//        window      = win_glob;
//        tablet      = tab_glob;
//    }
    void SetIdZThetaPhi(Int_t Id, Double_t Z, Double_t Theta, Double_t Phi){
        angle = Theta;
        phi = Phi;
        pos_z = Z;
        ID = Id+1;

        detector->name = "detector out " + std::to_string(ID);
        window->name = "sector out " + std::to_string(ID);
        tablet->name = "tablet out " + std::to_string(ID);
    }
    void Place(G4AssemblyVolume *assembly) {
        if (ID == -1) return;

        detector->ConstructLogical();
        window->ConstructLogical();
        tablet->ConstructLogical();
        body->ConstructLogical();

        window->logical->SetSensitiveDetector(sensitive);

        pmt_center_rad = pos_z * tan(angle);
        pmt_detector_rad = pmt_center_rad + window->thickness / 2. + detector->thickness / 2.;
        pmt_body_rad = pmt_center_rad + window->thickness / 2. + detector->thickness + 5*mm;


        G4RotationMatrix RTilt = G4RotationMatrix();
        RTilt.rotateX(angle + 90*deg);

        G4ThreeVector *Ta = new G4ThreeVector(0.,0.,0.);
        G4RotationMatrix *Ra = new G4RotationMatrix();



        G4Transform3D Tr;

        G4String name;


        Ta = new G4ThreeVector(0.,0.,0.);
        Ra = new G4RotationMatrix();

        //        Ra->rotateX(90.*deg);
        Ra->rotateY(-phi + 90*deg);
        *Ra = *Ra * RTilt;

        Ta->setX(pmt_center_rad * TMath::Cos(phi));
        Ta->setY(pos_z);
        Ta->setZ(pmt_center_rad * TMath::Sin(phi));

        Tr = G4Transform3D(*Ra, *Ta);
        //        if (j == 0)
        assembly->AddPlacedVolume(window->logical, Tr);


        /////////// outer detector ///////

        Ta = new G4ThreeVector(0.,0.,0.);
        Ra = new G4RotationMatrix();

        Ra->rotateY(-phi + 90.*deg);

        Ta->setX((pmt_detector_rad) * TMath::Cos(phi));
        Ta->setY(pos_z);
        Ta->setZ((pmt_detector_rad) * TMath::Sin(phi));


        *Ta -= G4ThreeVector( pmt_center_rad * TMath::Cos(phi),
                              pos_z,
                              pmt_center_rad * TMath::Sin(phi));

        *Ta = (*Ra * (RTilt * (Ra->inverse() * (*Ta))));

        *Ta += G4ThreeVector( pmt_center_rad * TMath::Cos(phi),
                              pos_z,
                              pmt_center_rad * TMath::Sin(phi));

        *Ra = *Ra * RTilt;
        Tr = G4Transform3D(*Ra,*Ta);

        assembly->AddPlacedVolume(detector->logical, Tr);

        /////////// PMT body ///////

        Ta = new G4ThreeVector(0.,0.,0.);
        Ra = new G4RotationMatrix();

        Ra->rotateY(-phi + 90.*deg);

        Ta->setX((pmt_body_rad) * TMath::Cos(phi));
        Ta->setY(pos_z);
        Ta->setZ((pmt_body_rad) * TMath::Sin(phi));


        *Ta -= G4ThreeVector( pmt_center_rad * TMath::Cos(phi),
                              pos_z,
                              pmt_center_rad * TMath::Sin(phi));

        *Ta = (*Ra * (RTilt * (Ra->inverse() * (*Ta))));

        *Ta += G4ThreeVector( pmt_center_rad * TMath::Cos(phi),
                              pos_z,
                              pmt_center_rad * TMath::Sin(phi));

        *Ra = *Ra * RTilt;
        Tr = G4Transform3D(*Ra,*Ta);

        assembly->AddPlacedVolume(body->logical, Tr);

    }
};


