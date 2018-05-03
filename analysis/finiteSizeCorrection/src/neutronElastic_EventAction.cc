#include "neutronElastic_EventAction.hh"

using namespace CLHEP;

neutronElastic_EventAction::neutronElastic_EventAction()
{}

neutronElastic_EventAction::~neutronElastic_EventAction()
{}

void neutronElastic_EventAction::BeginOfEventAction(const G4Event*)
{}

void neutronElastic_EventAction::resetVectors()
{
    curEdep.clear();
    curLo.clear();
    curNeng.clear();
    curHitTime.clear();
    earliestHitTime.clear();
    parentPrimaryID.clear();
    xint.clear();
    yint.clear();
    zint.clear();

    for(int i=0; i<ndet; i++)
    {
        curEdep.push_back(0);
        curLo.push_back(0);
        curNeng.push_back(0);
        curHitTime.push_back(0);
        earliestHitTime.push_back(0);
        parentPrimaryID.push_back(-1);
        xint.push_back(0);
        yint.push_back(0);
        zint.push_back(0);
    }
}

void neutronElastic_EventAction::EndOfEventAction(const G4Event* evt)
{
    resetVectors();

    int multiplicity=0;

    std::vector<int> parentID;

    G4ThreeVector intpos;

    fHCoE = evt->GetHCofThisEvent();

    if(fHCoE)
    {
        fCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("detectorCollection");
        fHitsCollection = (DetectorHitsCollection*)fHCoE->GetHC( fCollectionID );

        if( fHitsCollection )
        {
            for(int i=0; i<fHitsCollection->entries(); i++)
            {

                if(i==0)
                {
                    parentID.push_back((*fHitsCollection)[i]->GetParentID());
                    multiplicity++;
                }

                else if(std::find(parentID.begin(),parentID.end(),(*fHitsCollection)[i]->GetParentID())==parentID.end())
                {
                    //vectorize edep, LO over all detectors
                    //fill all once at the end, incl overall neutron multiplicity detected	  
                    multiplicity++;
                    parentID.push_back((*fHitsCollection)[i]->GetParentID());
                }		

                //fill all vectors

                //this picks the hit time to be the earliest collision in the ndet.  
                if((*fHitsCollection)[i]->GetHitTime() > 0 &&
                        ((*fHitsCollection)[i]->GetHitTime() < earliestHitTime[0] ||
                         earliestHitTime[0] < .0001 ))
                {
                    earliestHitTime[0] = (*fHitsCollection)[i]->GetHitTime()/ns;
                    curNeng[0] = (*fHitsCollection)[i]->GetNeng()/MeV;
                    intpos = (*fHitsCollection)[i]->GetPos();
                    xint[0] = intpos.getX();
                    yint[0] = intpos.getY();
                    zint[0] = intpos.getZ();
                }

                curEdep[0]+=((*fHitsCollection)[i]->GetEdep())/MeV;

                //fill parentPrimaryID vector
                if(parentPrimaryID[0] == -1)
                {
                    parentPrimaryID[0] = (*fHitsCollection)[i]->GetParentID();
                }

                else if(parentPrimaryID[0] != (*fHitsCollection)[i]->GetParentID())
                {
                    if(parentPrimaryID[0]%10 != (*fHitsCollection)[i]->GetParentID() &&
                            (int)(parentPrimaryID[0]/10) != (*fHitsCollection)[i]->GetParentID() &&
                            (int)(parentPrimaryID[0]/100) != (*fHitsCollection)[i]->GetParentID())
                    {
                        //	    G4cout << parentPrimaryID[0] << " " << (*fHitsCollection)[i]->GetParentID() << G4endl;
                        parentPrimaryID[0]*=10;
                        parentPrimaryID[0]+=(*fHitsCollection)[i]->GetParentID();
                        //	    G4cout << parentPrimaryID[0] << G4endl;
                    }
                }
            }

            //use earliest hit time or biggest LO hit time here
            for(int i = 0; i < (int)curHitTime.size(); i++)
            {
                curHitTime[i] = earliestHitTime[i];
            }
        }
    }
}
