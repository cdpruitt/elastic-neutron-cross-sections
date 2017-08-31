#include "TH1I.h"

#include "../include/scalerData.h"

using namespace std;

TH1I* subtractBackground(TH1I* targetHist, TH1I* blankHist, unsigned int targetScaling, unsigned int blankScaling)
{
    TH1I* targetHist = (TH1I*)targetHist->Clone();
    TH1I* blankHist = (TH1I*)blankHist->Clone();

    blankHist->Scale(targetScaling/blankScaling);

    TH1I* difference = (TH1I*)targetHist->Clone();
    difference->Add(blankHist,-1);
}
