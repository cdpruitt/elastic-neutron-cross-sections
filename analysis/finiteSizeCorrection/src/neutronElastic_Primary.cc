#include "neutronElastic_ExperimentConstruction.hh"
#include "neutronElastic_Primary.hh"
#include <string>

#include "TFile.h"
#include "TH1D.h"
#include "TGraphErrors.h"

using namespace CLHEP;

neutronElastic_Primary::neutronElastic_Primary(G4String filename) 
{
    std::ifstream infile(filename.data());
    G4String particle;

    infile >> aa >> bb >> cc >> nubar >> runname >> runno;  //load angular distribution
    infile.close();

    particleGun = new G4ParticleGun(1); // generate one neutron at a time

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;

    particleGun->SetParticleDefinition(particleTable->FindParticle("neutron"));
    particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));

    rand = new TRandom3(0);
    gRandom = rand;
    // conic angular distribution for neutron source
    /*angularDistribution = new TF1( //starting angular distribution
            "angularDistribution", // name of angular distribution
            //"0",
            "[0]*TMath::Gaus(x,[1],[2])",
            -pi/2,
            pi/2
            );

    angularDistribution->SetParameter(0,aa);
    angularDistribution->SetParameter(1,bb);
    angularDistribution->SetParameter(2,cc);
    angularDistribution->SetNpx(100);
    */

    //calculated by fitting rkin output at 15.0 MeV
    /*d2o_en_ang = new TF1("d2o_en_ang","pol3",0,180);
    d2o_en_ang->SetParameter(0,7.26197);
    d2o_en_ang->SetParameter(1,-0.000177025);
    d2o_en_ang->SetParameter(2,-0.000173958);
    d2o_en_ang->SetParameter(3,6.78406e-07);
    */

    //SET THE BEAM ENERGY WIDTH
    //beam_energy_width = 0.250; //used for pfoct14 c6d6 @ 15.0 MeV

    string inputDistributionFileName = "configuration/literatureData.root";
    TFile* inputDistributionFile = new TFile(inputDistributionFileName.c_str(), "READ");

    if(!inputDistributionFile->IsOpen())
    {
        cerr << "Error: failed to open input distribution file " << inputDistributionFileName << endl;
        exit(1);
    }

    string inputDistributionGraphName = "Sn112, 4M, 2018";
    inputDistributionGraph = (TGraphErrors*)inputDistributionFile->Get(inputDistributionGraphName.c_str());
    if(!inputDistributionGraph)
    {
        cerr << "Error: failed to find " << inputDistributionGraphName << " in " << inputDistributionFileName << endl;
        exit(1);
    }

    inputDistributionGraph = (TGraphErrors*)(inputDistributionGraph->Clone());

    inputDistributionFile->Close();

    // normalize input distribution histo
    int numberOfPoints = inputDistributionGraph->GetN();

    /*vector<double> x(numberOfPoints);
    vector<double> y(numberOfPoints);

    for(int i=0; i<numberOfPoints; i++)
    {
        inputDistributionGraph->GetPoint(i, x[i], y[i]);
    }

    for(int i=0; i<numberOfPoints; i++)
    {
        distributionIntegral += 0.5*(y[i]+y[i-1])*(x[i]-x[i-1]);
    }
    */

    distributionIntegral = 0.5*(inputDistributionGraph->Eval(MIN_ANGLE)
            +inputDistributionGraph->Eval(MAX_ANGLE))
        *(MAX_ANGLE-MIN_ANGLE);

    cout << "distribution integral = " << distributionIntegral << endl;

    inputTheta = new TH1D("inputTheta","inputTheta", (MAX_ANGLE+2-MIN_ANGLE)*10, MIN_ANGLE-1, MAX_ANGLE+1);

    evno = 0;
}

neutronElastic_Primary::~neutronElastic_Primary()
{
    TFile* inputThetaFile = new TFile("configuration/inputThetaResults.root", "RECREATE");
    inputTheta->Write();
    inputThetaFile->Close();

    delete particleGun;
}

void neutronElastic_Primary::GeneratePrimaries(G4Event* anEvent)
{
    evno++;

    if(evno%10000==0)
    {
        G4cout << evno << G4endl;
    }

    //set position
    G4double xx, yy, zz;

    const G4double targetDiameter = 0.0001*cm;
    const G4double targetHeight = 1.37*cm;
    const G4double DETECTOR_DIAMETER = 12.68*cm;

    //pick initial x and y
    do {
        xx = targetDiameter*(rand->Rndm()-0.5);
        zz = targetDiameter*(rand->Rndm()-0.5);
    } while(sqrt(pow(xx,2)+pow(zz,2)) > targetDiameter/2.0);

    //pick initial y, assume uniformly distributed over the target
    //zz = ORIGIN_OFFSET + BEAM_START_POSITION + BEAM_START_UNCERTAINTY*(rand->Rndm()-0.5);
    yy = targetHeight*(rand->Rndm()-0.5);

    particleGun->SetParticlePosition(G4ThreeVector(xx,yy,zz));

    // get angle
    double theta, phi, phi0, nen;
    double probability;

    do
    {
        theta = MIN_ANGLE+(MAX_ANGLE-MIN_ANGLE)*(rand->Rndm());
        probability = inputDistributionGraph->Eval(theta);
    } while(probability<distributionIntegral*rand->Rndm());

    inputTheta->Fill(theta);

    phi0 = atan(yy/DET_DISTANCE);
    phi = phi0+atan(DETECTOR_DIAMETER*(rand->Rndm()-0.5)/DET_DISTANCE);

    // set momentum direction
    double pxp,pyp,pzp;
    pxp = sin(theta)*cos(phi);
    pyp = sin(theta)*sin(phi);
    pzp = cos(theta);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(pxp,pyp,pzp));

    //set energy    
    float eg = 17.0;
    //double nen_orig;
    //nen = d2o_en_ang->Eval(thetan*180.0/3.1415926)*(1 + (nubar - eg)/(eg-2.2));
    //nen_orig = nen;
    //do {
    //    nen = rand->Gaus(nen,beam_energy_width);
    //} while(nen - nen_orig > nen_orig+2*beam_energy_width);

    particleGun->SetParticleEnergy(eg*MeV);
    particleGun->GeneratePrimaryVertex(anEvent); 
}
