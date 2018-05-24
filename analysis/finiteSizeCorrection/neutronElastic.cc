// mandatory GEANT4 headers
#include "G4RunManager.hh"
#include "G4UImanager.hh"

// optional GEANT4 headers
#include "G4VisExecutive.hh" // manages visualization
#include "G4UIExecutive.hh"  // allows the selection of a run-time interactive session type (?)

// QGSP_BERT_HP: high-precision neutron-physics interaction list, for neutrons <20 MeV
// down to thermal energies (see GEANT4 documentation for "reference physics lists")
#include "QGSP_BERT_HP.hh"

// user-defined headers
#include "neutronElastic_ExperimentConstruction.hh"
#include "neutronElastic_Primary.hh"
#include "neutronElastic_EventAction.hh"
#include "neutronElastic_TrackingAction.hh"
#include "G4UserSpecialCuts.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4ScoringManager.hh"

#include "DetectorDataTree.hh"

#include "G4SystemOfUnits.hh" // including this header is a new requirement for GEANT-defined units for GEANT 10.0+

#include <string>
#include <math.h>

using namespace std;

int main(int argc,char* argv[])
{
  //make sure an infile is specified
  char * infilename;
  infilename = getenv("NEUTRON_ELASTIC_INFILE_PATH");

  if(infilename==NULL)
  {
      cout << "Please set env variable NEUTRON_ELASTIC_INFILE_PATH to an input file" << endl;
      return 0;
  }

  // Generate Analysis Pointer Class
  DetectorDataTree* pointer = new DetectorDataTree;

  // RNG creation
  CLHEP::HepRandom::setTheEngine(new CLHEP::HepJamesRandom);
  G4long seed=time(0);
  CLHEP::HepRandom::setTheSeed(seed);
  CLHEP::HepRandom::showEngineStatus();

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

// create scorer for reading out relevant detector events
  G4ScoringManager* scoringManager = G4ScoringManager::GetScoringManager();

  // create the experimental setup and register w/ run manager
  neutronElastic_ExperimentConstruction* experiment = new neutronElastic_ExperimentConstruction();
  runManager->SetUserInitialization(experiment);

  // register the appropriate physics w/ run manager
  G4VModularPhysicsList* physicsList = new QGSP_BERT_HP(1);
  physicsList->SetVerboseLevel(0);

  // stop tracking neutrons after a time threshold
  G4NeutronTrackingCut* neutronCut = new G4NeutronTrackingCut();
  neutronCut->SetTimeLimit(150*ns);
  physicsList->RegisterPhysics(neutronCut);

  runManager->SetUserInitialization(physicsList);

  // set mandatory user action class
  string sinfilename;
  sinfilename.assign(infilename);
  neutronElastic_Primary* gen_action = new neutronElastic_Primary(sinfilename);
  runManager->SetUserAction(gen_action);

  // create ROOT tree generator
  //neutronElastic_EventAction* ev_action = new neutronElastic_EventAction();
  //runManager->SetUserAction(ev_action);

  //neutronElastic_TrackingAction* tr_action = new neutronElastic_TrackingAction;
  //runManager->SetUserAction(tr_action);

  // Initialize G4 kernel
  runManager->Initialize();

  // Get the pointer to the UI manager and set verbosities
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif 

  G4UImanager * UImanager = G4UImanager::GetUIpointer(); 
#ifdef G4UI_USE
  G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
  UImanager->ApplyCommand("/control/execute vis.mac");     
#endif

  ui->SessionStart();
  delete ui;
#endif
     
#ifdef G4VIS_USE
      delete visManager;
#endif

  delete runManager;

  pointer->tree->Write();

  delete pointer;

  return 0;
}
