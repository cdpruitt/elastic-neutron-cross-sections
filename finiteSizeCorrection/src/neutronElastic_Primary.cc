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
    angularDistribution = new TF2( //starting angular distribution
            "angularDistribution", // name of angular distribution
            "sin(x)*([0]+[1]*pow(sin(x),2)+[2]*pow(sin(2*x),2)-cos(2*y)*([1]*pow(sin(x),2)+[2]*pow(sin(2*x),2)))",
            0,
            pi,
            0,
            2*pi
            );

    angularDistribution->SetParameter(0,aa);
    angularDistribution->SetParameter(1,bb);
    angularDistribution->SetParameter(2,cc);
    angularDistribution->SetNpx(1000);
    angularDistribution->SetNpy(1000);

    //calculated by fitting rkin output at 15.0 MeV
    d2o_en_ang = new TF1("d2o_en_ang","pol3",0,180);
    d2o_en_ang->SetParameter(0,7.26197);
    d2o_en_ang->SetParameter(1,-0.000177025);
    d2o_en_ang->SetParameter(2,-0.000173958);
    d2o_en_ang->SetParameter(3,6.78406e-07);

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

    //radius is THE BEAM SPOT RADIUS, assume it is like 10.5 mm FOR 10 MM collimator
    //double beam_spot_dia = 10.5/10.0; //in cm
    //radius is THE BEAM SPOT RADIUS, assume it is like 20 mm FOR 3/4" collimator
    double beam_spot_dia = 20.0/10.0; //in cm

    //pick initial x and y
    do {
        xx = rand->Rndm()*beam_spot_dia-beam_spot_dia/2.0;
        yy = rand->Rndm()*beam_spot_dia-beam_spot_dia/2.0;
    } while(sqrt(pow(xx,2)+pow(yy,2)) > beam_spot_dia/2.0);

    //pick initial z, assume uniformly distributed over the target
    double zz;
    //length is 3.7 cm
    zz = rand->Rndm()*3.7 - 3.7/2.0;

    particleGun->SetParticlePosition(G4ThreeVector(xx*cm,yy*cm,zz*cm));

    //get angle
    double thetan, phin, nen;
    double pxp,pyp,pzp;
    double nen_orig;
    angularDistribution->GetRandom2(thetan,phin);

    //set momentum direction
    pxp = sin(thetan)*cos(phin);
    pyp = sin(thetan)*sin(phin);
    pzp = cos(thetan);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(pxp,pyp,pzp));

    //set energy    
    float eg = 15.0;
    nen = d2o_en_ang->Eval(thetan*180.0/3.1415926)*(1 + (nubar - eg)/(eg-2.2));
    nen_orig = nen;
    do {
        nen = rand->Gaus(nen,beam_energy_width);
    } while(nen - nen_orig > nen_orig+2*beam_energy_width);
    particleGun->SetParticleEnergy(nen*MeV);

    particleGun->GeneratePrimaryVertex(anEvent); 
}
