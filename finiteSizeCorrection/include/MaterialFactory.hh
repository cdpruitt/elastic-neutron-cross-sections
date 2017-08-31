#ifndef MaterialFactory_H
#define MaterialFactory_H 1

#include "G4Material.hh"

class MaterialFactory {

    public:

        MaterialFactory();
        ~MaterialFactory();
        void GenerateElements();
        void GenerateMaterials();
        G4Material* GetMaterial(G4String materialName);

        static MaterialFactory* matfactoryPointer;
        static void safelyConstructMatfactory();

    private:

        G4double fractionMass;
        G4int nComponents;
        G4int nIsotopes;
        G4int nAtoms;

        // declare polyethylene/graphite target composition
        G4Element* elH;

        G4Isotope* isoC12;
        G4Isotope* isoC13;
        G4Element* elC;

        G4Material* matPoly;
        G4Material* matGraphite;

        // declare air composition
        G4Isotope* isoN;
        G4Element* elN;

        G4Isotope* isoO16;
        G4Isotope* isoO17;
        G4Isotope* isoO18;
        G4Element* elO;

        G4Material* matAir;

        // declare Sn target composition
        G4Isotope* isoSn112;
        G4Element* elSn112;

        G4Isotope* isoSn124;
        G4Element* elSn124;

        G4Material* matSn112;
        G4Material* matSn124;

        // declare detector composition (NE213, eq. to Bicron 501A)
        G4Material* matBC501A;
};

#endif
