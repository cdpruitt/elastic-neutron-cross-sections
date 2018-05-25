#ifndef neutronElastic_Primary_h
#define neutronElastic_Primary_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include <time.h>
#include <string>
#include "TRandom3.h"
#include <fstream>
#include "TF2.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraphErrors.h"
#include "TProfile2D.h"

const G4double BEAM_SPOT_DIAMETER = 0*cm;   // in cm
const G4double BEAM_START_UNCERTAINTY = 0*cm; // in cm
const G4double BEAM_START_POSITION = -10*cm; // in cm

class neutronElastic_Primary : public G4VUserPrimaryGeneratorAction
{
    public:
        neutronElastic_Primary(G4String);
        ~neutronElastic_Primary();
        void GeneratePrimaries(G4Event* anEvent);
        inline float Getaa() {return aa;}
        inline float Getbb() {return bb;}
        inline float Getcc() {return cc;}
        inline std::string GetRunname() {return runname;}
        inline int GetRunno() {return runno;}

    private:
        G4ParticleGun* particleGun;
        TRandom3 * rand;
        Double_t theta, phi;
        Double_t thetaf, phif;
        float aa,bb,cc;
        float nubar;
        std::string runname;
        TF1 * angularDistribution;
        TGraphErrors* inputDistributionGraph;
        TH1F * nudist;
        TH2F * fangdist;
        TH2F * nangdist;
        TH2F * check_n_ang_en;
        TH1F * sim_nudist;
        int runno;
        static const int primary_proj_bin_width = 2;
        int evno;
        bool isC6D6;
        TH1F * neng;
        TF1 * d2o_en_ang;
        float beam_energy_width;
        bool polarized_beam;

        const double MIN_ANGLE = 29;
        const double MAX_ANGLE = 31;

        TH1D* inputTheta;
        double distributionIntegral = 0;
};

#endif


