#ifndef HISTOS_H
#define HISTOS_H

#include <string>
#include "TH1.h"
#include "Detector.h"

int histos(std::string inputFileName, std::string outputFileName);
TH1D* correctForEfficiency(TH1D* histo, const Detector& d);

#endif /* HISTOS_H */
