#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

RunAction::RunAction() 
    : G4UserRunAction(), 
      fPreShieldPhotons(0),
      fPostShieldPhotons(0),
      fUncollidedPhotons(0)
{}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    // Reset counters at start of run
    fPreShieldPhotons = 0;
    fPostShieldPhotons = 0;
    fUncollidedPhotons = 0;
}


void RunAction::EndOfRunAction(const G4Run*) {
    G4cout << "\n========== Run Summary ==========" << G4endl;
    G4cout << "Photons before shield (PreDetector): " << fPreShieldPhotons << G4endl;
    G4cout << "Photons after shield (PostDetector): " << fPostShieldPhotons << G4endl;
    G4cout << "Uncollided photons after shield (PostDetector): " << fUncollidedPhotons << G4endl;

 if (fUncollidedPhotons > 0)
    {
      G4double buildupFactor = static_cast<G4double>(fPostShieldPhotons) / fUncollidedPhotons;
      G4cout << "\nBuildup Factor (B): " << buildupFactor << G4endl;
    }
    

 /*
    if (fPreShieldPhotons > 0) {
        G4double transmission = static_cast<G4double>(fPostShieldPhotons) / fPreShieldPhotons;
        G4double attenuation = -log(transmission);
       // G4double thickness = 4*cm;  // From your DetectorConstruction
       // G4double density = 11.34*g/cm3;  // For lead
       // G4double mass_attenuation = attenuation / (density * thickness);

        G4cout << "\nShield Performance:" << G4endl;
        G4cout << "Transmission fraction: " << transmission << G4endl;
        G4cout << "Attenuation: " << attenuation << G4endl;
        
        
    }


    */
    G4cout << "==================================" << G4endl;
}

void RunAction::AddPrePhoton(G4int count) {
    fPreShieldPhotons += count;
}


void RunAction::AddPostPhoton(G4int count) {
    fPostShieldPhotons += count;
}

void RunAction::AddUncollidedPhoton(G4int count) {
  fUncollidedPhotons += count;
}

