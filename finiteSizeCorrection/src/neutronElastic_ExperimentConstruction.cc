#include "neutronElastic_ExperimentConstruction.hh"

using namespace CLHEP;

neutronElastic_ExperimentConstruction::neutronElastic_ExperimentConstruction()
    :  worldVolume_log(0),
    worldVolume_phys(0)
{
    materialFactory = new MaterialFactory();
    detConstruction = new DetectorConstruction("bicron");
}

neutronElastic_ExperimentConstruction::~neutronElastic_ExperimentConstruction() {}

G4VPhysicalVolume* neutronElastic_ExperimentConstruction::Construct()
{
    // Create the 'world volume' of the experiment
    G4double worldX = 6.0*m; // left of beam
    G4double worldY = 1.0*m; // toward ceiling
    G4double worldZ = 6.0*m; // beam direction

    G4Material * air = materialFactory->GetMaterial("Air");

    G4Box* worldBox = new G4Box(
            "worldVol_box",
            worldX,
            worldY,
            worldZ
            );

    worldVolume_log = new G4LogicalVolume(
            worldBox,
            air,
            "worldVol_log",
            0,
            0,
            0);

    worldVolume_log->SetVisAttributes(G4VisAttributes::Invisible);

    // create the detector
    G4ThreeVector detPosition = G4ThreeVector(
            DET_DISTANCE*sin(DET_ANGLE), // x-direction
            0,                                 // y-direction
            DET_DISTANCE*cos(DET_ANGLE)  // z-direction
            );

    G4RotationMatrix* detRotation = new G4RotationMatrix();
    detRotation->rotateY(-DET_ANGLE);

    G4LogicalVolume * detector_log = detConstruction->GetADetector();

    G4PVPlacement* detector = new G4PVPlacement(
            detRotation,     // detector rotation
            detPosition,     // detector position vector
            detector_log,    // logical volume used to instantiate this detector
            "detector",      // name of detector
            worldVolume_log, // name of volume in which to place detector
            false,           // no boolean operations (?)
            0);                // new physical volume copy number

    // create the target
    G4Tubs* targetShape = new G4Tubs(
            "targetShape",  // name of target
            0.0*cm,    // inner radius of cylinder
            0.4125*cm, // outer radius of cylinder
            0.85*cm,   // half-length of cylinder
            0*deg,     // angle at which to start cylinder
            360*deg);  // angle at which to end cylinder

    G4Material* targetMaterial =  materialFactory->GetMaterial("Sn112");

    G4LogicalVolume* targetLog = new G4LogicalVolume(
            targetShape,
            targetMaterial,
            "targetLog");

    G4RotationMatrix* targetRotation = new G4RotationMatrix();
    targetRotation->rotateX(90*deg);
    G4PVPlacement* target = new G4PVPlacement(
            targetRotation,
            G4ThreeVector(0,0,0),
            targetLog,
            "target",
            worldVolume_log,
            false,
            0); 

    G4PVPlacement* world = new G4PVPlacement(
            0,
            G4ThreeVector(),
            worldVolume_log,
            "world",
            0,
            false,
            0);

    return world;
}
