#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4AutoDelete.hh"
#include "G4PolarizationManager.hh"

#include "G4SDManager.hh"
#include "TargetSD.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

G4int DetectorConstruction::fNofTargets = 0;

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fCheckOverlaps(true),
   fLogicalTarget(NULL)
{
  SetCopyNb(6); // The number of targets
  fLogicalTarget = new G4LogicalVolume*[fNofTargets];
}

DetectorConstruction::~DetectorConstruction()
{
  delete [] fLogicalTarget;
}  

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Geometry parameters
  G4double targetThickness = 5.*cm;
  G4double targetSizeXY  = 25.*cm;
  G4double targetSpacing  = 20.*cm;	//from target center to center
  
  if ( targetSpacing < targetThickness ) {
    G4ExceptionDescription msg;
    msg << "The targetThickness should be larger than targetSpacing.";
    G4Exception("B4DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }

  G4double envLength = (fNofTargets+1)*targetSpacing;	//the length of envelop in z axis
  G4double envSizeXY = targetSizeXY+20*cm;

  auto worldSizeXY = 1.2 * envSizeXY;
  auto worldSizeZ  = 1.2 * envLength;

  G4double firstPosition = -envLength/2 + targetSpacing;
  
  // Get materials
  G4NistManager* nist = G4NistManager::Instance();

  G4Material* worldMaterial  = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* envMaterial    = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* targetMaterial = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  
  if ( ! worldMaterial || ! envMaterial || ! targetMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("B4DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }

  //                                        
  // Visualization attributes
  //
  G4VisAttributes* targetVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* envelopeVisAtt = new G4VisAttributes(G4Colour(1.0,0.5,0.0));
   
  //     
  // World
  //
  auto worldS 
    = new G4Box("World",           // its name
                 worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size
                         
  auto worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 worldMaterial,    // its material
                 "World");         // its name
                                   
  auto worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
  //
  // Envelope
  //
  G4Box* envS
    = new G4Box("Envelope",	   // its name
		envSizeXY/2, envSizeXY/2, envLength/2); // its size

  G4LogicalVolume* envLV
    = new G4LogicalVolume(
		envS,		   // its solid
		envMaterial,	   // its material
		"Envelope");        // its name

  envLV->SetVisAttributes(envelopeVisAtt);

  new G4PVPlacement(
		0,		   // no rotation
		G4ThreeVector(),   // at(0, 0, 0)
		envLV,		   // its logical volume
		"Envelope",	   // its name
		worldLV,	   // its mather volume
		false,		   // no boolean operations
		0, 		   // copy number
		fCheckOverlaps);   // checking overlaps

  //
  // Target
  //
  for(G4int copyNb = 0; copyNb<fNofTargets; copyNb++)
  {
     G4double Zposition = firstPosition + copyNb*targetSpacing;

     G4Box* targetS
	 = new G4Box("Target",	   // its name
		     targetSizeXY/2, targetSizeXY/2, targetThickness/2); // its size

     fLogicalTarget[copyNb]  
         = new G4LogicalVolume(targetS,		// its solid
	                       targetMaterial,  // its material
			       "Target",	// its name
			       0,
			       0,
			       0);

     fLogicalTarget[copyNb]->SetVisAttributes(targetVisAtt);

     new G4PVPlacement(0,				// no rotation
		       G4ThreeVector(0, 0, Zposition),  // its position
		       fLogicalTarget[copyNb],		// its logical volume
		       "Target",     			// its name
		       envLV,				// its mather volume
		       false,				// no poolean operation
		       copyNb,				// copy number
		       fCheckOverlaps);			// checking overlaps

  // register logical Volume in PolarizationManager with zero polarization
  G4PolarizationManager * polMgr = G4PolarizationManager::GetInstance();
  polMgr->SetVolumePolarization(fLogicalTarget[copyNb], G4ThreeVector(0.,0.,0.));
  }

  //std::cout << *(G4Material::GetMaterialTable()) << std::endl; // Get material table

  //
  // Always return the physical World
  //
  return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // 
  // Sensitive detectors
  //
  TargetSD* targetSD 
    = new TargetSD("cosmicRay/TargetSD", "TargetHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(targetSD);
  SetSensitiveDetector("Target", targetSD, true);
  // Setting targetSD to all logical volumes with the same name of "Target"
}
