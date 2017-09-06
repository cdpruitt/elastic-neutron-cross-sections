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
#include "TH1F.h"
#include "TProfile2D.h"

const double BEAM_SPOT_DIAMETER = 0;   // in cm
const double BEAM_START_UNCERTAINTY = 0; // in cm
const double BEAM_START_POSITION = -2; // in cm

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
};

#endif

