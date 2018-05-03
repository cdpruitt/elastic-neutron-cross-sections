#include "neutronElastic_ExperimentConstruction.hh"
#include "neutronElastic_Primary.hh"
#include <string>

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
    angularDistribution = new TF1( //starting angular distribution
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

    //calculated by fitting rkin output at 15.0 MeV
    /*d2o_en_ang = new TF1("d2o_en_ang","pol3",0,180);
    d2o_en_ang->SetParameter(0,7.26197);
    d2o_en_ang->SetParameter(1,-0.000177025);
    d2o_en_ang->SetParameter(2,-0.000173958);
    d2o_en_ang->SetParameter(3,6.78406e-07);
    */

    //SET THE BEAM ENERGY WIDTH
    beam_energy_width = 0.250; //used for pfoct14 c6d6 @ 15.0 MeV

    evno = 0;
}

neutronElastic_Primary::~neutronElastic_Primary()
{
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
    double xx, yy;

    //pick initial x and y
    do {
        xx = BEAM_SPOT_DIAMETER*(rand->Rndm()-0.5);
        yy = BEAM_SPOT_DIAMETER*(rand->Rndm()-0.5);
    } while(sqrt(pow(xx,2)+pow(yy,2)) > BEAM_SPOT_DIAMETER/2.0);

    //pick initial z, assume uniformly distributed over the target
    double zz;
    zz = ORIGIN_OFFSET + BEAM_START_POSITION + BEAM_START_UNCERTAINTY*(rand->Rndm()-0.5);

    particleGun->SetParticlePosition(G4ThreeVector(xx,yy,zz));

    // get angle
    double theta, phi, nen;
    theta = angularDistribution->GetRandom();
    phi = 2*pi*(rand->Rndm()-0.5);

    // set momentum direction
    double pxp,pyp,pzp;
    pxp = sin(theta)*cos(phi);
    pyp = sin(theta)*sin(phi);
    pzp = cos(theta);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(pxp,pyp,pzp));

    //set energy    
    float eg = 11.0;
    //double nen_orig;
    //nen = d2o_en_ang->Eval(thetan*180.0/3.1415926)*(1 + (nubar - eg)/(eg-2.2));
    //nen_orig = nen;
    //do {
    //    nen = rand->Gaus(nen,beam_energy_width);
    //} while(nen - nen_orig > nen_orig+2*beam_energy_width);

    particleGun->SetParticleEnergy(eg*MeV);
    particleGun->GeneratePrimaryVertex(anEvent); 
}
