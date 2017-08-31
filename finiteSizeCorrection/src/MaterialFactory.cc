#include "MaterialFactory.hh"

using namespace CLHEP;

MaterialFactory::MaterialFactory() 
{
    GenerateElements();
    GenerateMaterials();
}

MaterialFactory::~MaterialFactory() {}

G4Material* MaterialFactory::GetMaterial(G4String materialName)
{
    G4Material *theMaterial = G4Material::GetMaterial(materialName);

    if( !theMaterial )
    {
        G4cerr << "   AllMaterials::GetMaterial - Error! The material \"" << materialName << "\" does not exist." << G4endl;
    }

    return theMaterial;
}

void MaterialFactory::GenerateElements() {

    // for poly target and graphite target definition
    elH = new G4Element("Hydrogen", "H", 1, 1.008*(g/mole));

    isoC12 = new G4Isotope("Carbon-12", 6, 12);
    isoC13 = new G4Isotope("Carbon-13", 6, 13);
    elC    = new G4Element("Carbon", "C", nIsotopes=2);
    elC->AddIsotope(isoC12, 0.9893);
    elC->AddIsotope(isoC13, 0.0107);

    // for air definition
    elN = new G4Element("Nitrogen", "N", 7, 14.01*(g/mole));

    isoO16 = new G4Isotope("Oxygen-16", 8, 16);
    isoO17 = new G4Isotope("Oxygen-17", 8, 17);
    isoO18 = new G4Isotope("Oxygen-18", 8, 18);
    elO    = new G4Element("Oxygen", "O", nIsotopes=3);
    elO->AddIsotope(isoO16, 0.9976);
    elO->AddIsotope(isoO17, 0.0004);
    elO->AddIsotope(isoO18, 0.0020);

    // for Sn target definitions
    isoSn112 = new G4Isotope("Tin-112", 50, 112);
    elSn112  = new G4Element("Tin-112-element","Sn112", nIsotopes=1);
    elSn112->AddIsotope(isoSn112,1);

    isoSn124 = new G4Isotope("Tin-124", 50, 124);
    elSn124  = new G4Element("Tin-124-element","Sn124", nIsotopes=1);
    elSn124->AddIsotope(isoSn124,1);
}

void MaterialFactory::GenerateMaterials()
{
    // define air
    matAir = new G4Material("Air", 1.275*(kg/m3), nComponents=2);
    matAir->AddElement(elN, fractionMass=0.79);
    matAir->AddElement(elO, fractionMass=0.21);

    // define scintillator for detectors (NE213, eq. to Bicron 501A)
    matBC501A = new G4Material("BC501A", 0.874*(g/cm3), nComponents=2);
    matBC501A->AddElement(elH, nAtoms=1212);
    matBC501A->AddElement(elC, nAtoms=1000);

    // define polyethylene target
    matPoly = new G4Material("Poly", 0.95*(g/cm3), nComponents=2);
    matPoly->AddElement(elC, nAtoms=1);
    matPoly->AddElement(elH, nAtoms=2);

    // define carbon target
    matGraphite = new G4Material("Graphite",2.266*(g/cm3), nComponents=1);
    matGraphite->AddElement(elC, nAtoms=1);

    // define Sn112 target
    matSn112 = new G4Material("Sn112",6.81*(g/cm3),nComponents=1);
    matSn112->AddElement(elSn112,fractionMass=1.0);

    // define Sn124 target
    matSn124 = new G4Material("Sn124",7.57*(g/cm3),nComponents=1);
    matSn124->AddElement(elSn124,fractionMass=1.0);
}
