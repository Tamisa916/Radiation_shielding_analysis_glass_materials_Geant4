#ifndef TC_DETECTOR_CONSTRUCTION_HH
#define TC_DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

namespace TC {
	class DetectorConstruction : public G4VUserDetectorConstruction {
	public:
		DetectorConstruction() ;
		~DetectorConstruction() = default;

		G4VPhysicalVolume* Construct() override;
		void ConstructSDandField() override;

// New parameter setters
        void SetShieldMaterial(G4Material* mat) { glass = mat; }
        void SetShieldThickness(G4double thick) { fShieldThickness = thick; }
        
    private:
        // Shield parameters
        G4Material* glass;
        G4double fShieldThickness;
        
        // Logical volumes for sensitive detectors
        G4LogicalVolume* fPreDetectorLogic;
        G4LogicalVolume* fPostDetectorLogic;

       G4Material* fNaIMaterial;
    void DefineMaterials();
	};
}
#endif // !TC_DETECTOR_CONSTRUCTION_HH