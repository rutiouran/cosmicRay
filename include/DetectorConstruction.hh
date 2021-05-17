#ifndef B4cDetectorConstruction_h
#define B4cDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetCopyNb(G4int copyNb) {fNofTargets = copyNb;};
    static G4int GetCopyNb() {return fNofTargets;};
     
  private:
    // data members
    //

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    static  G4int   fNofTargets;     // number of layers

    G4LogicalVolume** fLogicalTarget;	//pointer to the logical target 
};

#endif
