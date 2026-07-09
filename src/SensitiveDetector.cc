#include "SensitiveDetector.hh"

#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"

#include "RunAction.hh"

namespace TC
{

G4Mutex mutex = G4MUTEX_INITIALIZER;

SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), fHitsCollection(nullptr)
{
  collectionName.insert(hitsCollectionName);
  G4int fPhotonCount;
}

void SensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection = new HitsCollection(SensitiveDetectorName, collectionName[0]);
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // ~~~ REMOVE THIS firststepin volume FILTER ~

    // Only process gamma photons
    if (aStep->GetTrack()->GetDefinition() != G4Gamma::GammaDefinition()) {
      return false;
    }

    // Reject secondary gammas
    if (aStep->GetTrack()->GetParentID() != 0) {
      return false;
    }

    // Reject backscattered photons
    G4ThreeVector momentumDir = aStep->GetPreStepPoint()->GetMomentumDirection();
    if (momentumDir.z() <= 0) {
      return false;
    }

    // ~~~ REMOVE THE ENERGY FILTER (For B calculation, keep it commented) ~~~
   
    // Create hit and mark as gamma
    TCHit* hit = new TCHit();
    hit->MarkAsGamma();
   

    
    // ~~~ CORRECT AND SIMPLIFIED METHOD ~~~
    // Check if this is the photon's first step (meaning it has never interacted anywhere)
    if (aStep->GetTrack()->GetCurrentStepNumber() == 1) {
      hit->MarkAsUncollided();  // This is the correct way to identify uncollided photons
    }

    

    // All other photons that enter the detector are scored as "total" (collided + uncollided)

    fHitsCollection->insert(hit);
    return true;
  
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  G4AutoLock lock(&mutex);  // Thread-safe output

  G4int gammaCount = fHitsCollection->CountGammaHits();
  G4int uncollidedCount = fHitsCollection->CountUncollidedHits();

  G4cout << "Gamma photons detected in this event: " << gammaCount << G4endl;
  G4cout << "Uncollided photons detected in this event: " << uncollidedCount << G4endl;

  // Update RunAction
  const G4UserRunAction* userAction = G4RunManager::GetRunManager()->GetUserRunAction();
  RunAction* runAction = const_cast<RunAction*>(static_cast<const RunAction*>(userAction));

  if (runAction) {
    if (GetName() == "PreDetector") {
      runAction->AddPrePhoton(gammaCount);
    }
    else if (GetName() == "PostDetector") {
      runAction->AddPostPhoton(gammaCount);
      runAction->AddUncollidedPhoton(uncollidedCount);
    }
  }
}
}  // namespace TC
