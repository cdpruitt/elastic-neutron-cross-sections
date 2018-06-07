#ifndef CALCULATE_REFERENCE_H
#define CALCULATE_REFERENCE_H

#include "ReferenceCS.h"
#include "Detector.h"
#include <string>

int calculateReference(std::string experiment, ReferenceCS& reference, Detector d);
ReferenceCS combineReferences(std::vector<ReferenceCS> references);

#endif /* CALCULATE_REFERENCE_H */
