#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"  
#include "G4SubtractionSolid.hh" 
#include "G4RotationMatrix.hh"


namespace TC {

    DetectorConstruction::DetectorConstruction() {
        // Default shield parameters

      DefineMaterials();
        G4NistManager* nist = G4NistManager::Instance();
     
       // For glass

        // Define elements

        G4Element* Bi = new G4Element("Bismuth", "Bi", 83., 208.98 * g / mole);
        G4Element* Ba = new G4Element("Barium", "Ba", 56., 137.33 * g / mole);
        G4Element* B = new G4Element("Boron", "B", 5., 10.81 * g / mole);
        G4Element* O = new G4Element("Oxygen", "O", 8., 16.00 * g / mole);
        G4Element* Si = new G4Element("Silicon", "Si", 14., 28.00 * g / mole);
        G4Element* Na = new G4Element("Sodium", "Na", 11., 23.00 * g / mole);
        G4Element* Al = new G4Element("Aluminium", "Al", 13., 27.00 * g / mole);
        G4Element* K = new G4Element("Potassium", "K", 19., 39.00 * g / mole);
        

          //G4Material* B2O3 = nist->FindOrBuildMaterial("G4_BORON_OXIDE");

        G4Material* Bi2O3 = new G4Material("Bi2O3", 8.96 * g / cm3, 2);
        Bi2O3->AddElement(Bi, 2);
        Bi2O3->AddElement(O, 3);

          G4Material* BaO = new G4Material("BaO", 5.72 * g / cm3, 2);
         BaO->AddElement(Ba, 1);
         BaO->AddElement(O, 1);

        G4Material* SiO2 = new G4Material("SiO2", 2.65 * g / cm3, 2);
        SiO2->AddElement(Si, 1);
        SiO2->AddElement(O, 2);


        G4Material* B2O3 = new G4Material("B2O3", 2.5 * g / cm3, 2);
        B2O3->AddElement(B, 2);
       B2O3->AddElement(O, 3);

     /*  
       
        G4Material* Na2O = new G4Material("Na2O", 2.27 * g / cm3, 2);
       Na2O->AddElement(Na, 2);
       Na2O->AddElement(O, 1);

        G4Material* Al2O3 = new G4Material("Al2O3", 3.99 * g / cm3, 2);
       Al2O3->AddElement(Al, 2);
       Al2O3->AddElement(O, 3);

        G4Material* K2O = new G4Material("K2O", 2.35 * g / cm3, 2);
       K2O->AddElement(K, 2);
       K2O->AddElement(O, 1);

        G4Material* borosilicate = new G4Material("borosilicate", 2.55 * g / cm3, 5);
       borosilicate->AddMaterial(B2O3, 20.20 * perCent);
        borosilicate->AddMaterial(Na2O, 8.21 * perCent);
       borosilicate->AddMaterial(Al2O3, 17.35 * perCent);
        borosilicate->AddMaterial(SiO2, 48.51 * perCent);
       borosilicate->AddMaterial(K2O, 5.73 * perCent);


       glass = new G4Material("glass", 4.210 * g / cm3, 3);
       glass->AddMaterial(Bi2O3, 20 * perCent);
       glass->AddMaterial(BaO, 50 * perCent);
       glass->AddMaterial(borosilicate, 30 * perCent);
        
       
       */
       

        // Define material
       
        glass = new G4Material("glass", 4.502 * g / cm3, 3);
        glass->AddMaterial(Bi2O3, 65 * perCent);
        glass->AddMaterial(BaO, 2 * perCent);
        glass->AddMaterial(B2O3, 33 * perCent);

        

        
        
        



        fShieldThickness = 1.*cm;
    }

void DetectorConstruction::DefineMaterials() {
        G4NistManager* nist = G4NistManager::Instance();
        
        // Define NaI material
        G4Element* Na = nist->FindOrBuildElement("Na");
        G4Element* I = nist->FindOrBuildElement("I");
        
        fNaIMaterial = new G4Material("NaI", 3.67*g/cm3, 2);
        fNaIMaterial->AddElement(Na, 1);
        fNaIMaterial->AddElement(I, 1);
        
        // Add optical properties for scintillation if needed
        const G4int nEntries = 2;
        G4double photonEnergy[nEntries] = {2.034*eV, 4.136*eV};
        
        // Refractive index
        G4double refractiveIndex[nEntries] = {1.85, 1.85};
        
        G4MaterialPropertiesTable* mptNaI = new G4MaterialPropertiesTable();
        mptNaI->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries, true);
        
        // Scintillation properties
        G4double scintilFast[nEntries] = {1.00, 1.00};
        //mptNaI->AddProperty("FASTCOMPONENT", photonEnergy, scintilFast, nEntries, true);
        //mptNaI->AddConstProperty("SCINTILLATIONYIELD", 38000./MeV, true);
        mptNaI->AddConstProperty("RESOLUTIONSCALE", 1.0, true);
        mptNaI->AddConstProperty("FASTTIMECONSTANT", 250.*ns, true);
        mptNaI->AddConstProperty("YIELDRATIO", 1.0, true);
        
        fNaIMaterial->SetMaterialPropertiesTable(mptNaI);
    }

    

    G4VPhysicalVolume* DetectorConstruction::Construct() {
        G4NistManager* nist = G4NistManager::Instance();
        
        // World volume
        G4double worldSize = 50*cm;
        G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
        
        auto solidWorld = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
        auto logicWorld = new G4LogicalVolume(solidWorld, vacuum, "World");
        auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld,
                                         "World", nullptr, false, 0);

        // Detector parameters
        G4double detXY = 5.*cm;
        G4double detZ = 5.*cm;
         G4double detZ_pre = 1. *cm;
        G4double detXY_pre = 5. *cm;

          
        G4Material* det_Mat = vacuum;

        // Create shield --------------------------------------------------------
        G4double shXY = 10.*cm;
        G4double shieldZpos = 5.*cm;  // Center position of shield
        
        auto solidShield = new G4Box("Shield", shXY/2, shXY/2, fShieldThickness/2);

              auto logicShield = new G4LogicalVolume(solidShield, glass, "Shield");
        new G4PVPlacement(nullptr, G4ThreeVector(0,0,shieldZpos), logicShield,
                        "Shield", logicWorld, false, 0);

        // Create detectors ------------------------------------------------------
        // Pre-detector (before shield)

        G4double preDetZpos = shieldZpos - fShieldThickness/2 - 8.*cm;
        auto preSolid = new G4Box("PreDetector", detXY_pre, detXY_pre, detZ_pre);
        fPreDetectorLogic = new G4LogicalVolume(preSolid, fNaIMaterial, "PreDetector");
        new G4PVPlacement(nullptr, G4ThreeVector(0,0,preDetZpos), fPreDetectorLogic,
                        "PreDetector", logicWorld, false, 0);



        // Post-detector (after shield)
        G4double postDetZpos = shieldZpos + fShieldThickness/2 + 10.*cm;
        auto postSolid = new G4Box("PostDetector", detXY, detXY, detZ);
        fPostDetectorLogic = new G4LogicalVolume(postSolid, fNaIMaterial, "PostDetector");
        new G4PVPlacement(nullptr, G4ThreeVector(0,0,postDetZpos), fPostDetectorLogic,
                        "PostDetector", logicWorld, false, 0);
        
       /* 

        //Collimator parameters
 G4Material* lead = nist->FindOrBuildMaterial("G4_Pb");
    G4double collimatorThickness = 2.0*cm;
    G4double collimatorRadius = 5.0*cm;  // Outer radius
    G4double holeRadius = 0.5*cm;       // Aperture radius

    // Position collimators 5 cm from each detector
    G4double collimatorPreZ = preDetZpos - 3.*cm;
    
    // Solid collimator (full cylinder)
    auto solidCollimatorFull = new G4Tubs("CollimatorFull", 
                                        0, 
                                        collimatorRadius, 
                                        collimatorThickness/2, 
                                        0, 
                                        360*deg);

    // Solid hole (to be subtracted)
    auto solidHole = new G4Tubs("CollimatorHole", 
                              0, 
                              holeRadius, 
                              collimatorThickness,             
                              0, 
                              360*deg);
 // Subtracted solid (creates collimator with hole)
    auto solidCollimator = new G4SubtractionSolid("Collimator",
                                                solidCollimatorFull,
                                                solidHole,
                                                new G4RotationMatrix(),
                                                G4ThreeVector());

    // Logical volume
    auto logicCollimator = new G4LogicalVolume(solidCollimator, 
                                             glass, 
                                             "Collimator");

    // Place PRE-shield collimator
    new G4PVPlacement(nullptr, 
                     G4ThreeVector(0,0,collimatorPreZ),
                     logicCollimator,
                     "PreCollimator",
                     logicWorld,
                     false,
                     0);

                     
*/

        return physWorld;
    }

    void DetectorConstruction::ConstructSDandField() {
        // Create two sensitive detectors with different names
        auto preSD = new SensitiveDetector("PreDetector", "PreHitsCollection");
        auto postSD = new SensitiveDetector("PostDetector", "PostHitsCollection");

        G4SDManager::GetSDMpointer()->AddNewDetector(preSD);
        G4SDManager::GetSDMpointer()->AddNewDetector(postSD);

        // Assign SDs to logical volumes
       SetSensitiveDetector(fPreDetectorLogic, preSD);
        SetSensitiveDetector(fPostDetectorLogic, postSD);
    }
}