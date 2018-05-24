#include "DetectorDataTree.hh"

// Access to Analysis pointer! (see MargotSD.cc EndOfEvent() for Example)
DetectorDataTree* DetectorDataTree::pointer;

DetectorDataTree::DetectorDataTree() : 
    // Initialized Values
    eng_int(0), theta_int(0), phi_int(0), theStartPos(0.,0.,0.),
    KEOfEvent(0), TOFOfEvent(0), PathLengthEvent(0),
    event_counter(0), number_detected(0)
{
    /* Constructor */
    // When Pointer is constructed, assigns address of this class to it.
    pointer = this;

    // Create pointers to ROOT Analysis Tree and Branches

    const Char_t* evt_file = "dataTree.root";

    file = new TFile(evt_file,"RECREATE");
    tree = new TTree("tree","Data From Energy Deposited in the Det");

    tree->Branch("Energy",&KEOfEvent,"energy/D");
    tree->Branch("TOF",&TOFOfEvent,"TOF/D");
}

DetectorDataTree::~DetectorDataTree()
{
    /* Destructor, Close root file */

    file->Write(); 
    file->Close();
    std::cout << "############################################################" << std::endl;
    std::cout << "Created Root Tree File = \"dataTree.root\"" << std::endl;
    std::cout << "Got the ROOT Tree File from Data Record Routine!" << std::endl; 
    delete file;
}

// Member functions of DetectorDataTree class

/*void DetectorDataTree::senddataPG(double value1,G4ThreeVector StartPos)
{
    eng_int = value1;
    event_counter++;
    theta_int = 0.;
    phi_int = 0.;

    theStartPos = StartPos;
}*/

void DetectorDataTree::ShowDataFromEvent()
{
    G4cout << "====================================================" << G4endl;
    G4cout << "The Energy of the Initial Particle was :    " << G4BestUnit(eng_int,"Energy") << G4endl;
    G4cout << "KE of event was  :    " << G4BestUnit(KEOfEvent,"Energy")  << G4endl;
    G4cout << "TOF of particle was :    " << G4BestUnit(TOFOfEvent,"Time") << G4endl;
    G4cout << "Total Detected So Far                  :    " << number_detected << G4endl;
    G4cout << G4endl;     
}

void DetectorDataTree::GetParticleTotals()
{
    G4cout << "The Initial Number of Beam Particles was           : " << event_counter << G4endl;
    G4cout << "Number of decay Particles Detected                 : " << number_detected << G4endl;
    G4cout << G4endl;
}

void DetectorDataTree::FillTree()
{
    tree->Fill();

    number_detected++;
}
