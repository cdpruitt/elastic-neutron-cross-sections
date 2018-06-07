#ifndef PLOT_HISTOS_H
#define PLOT_HISTOS_H

#include <vector>

#include "ReferenceCS.h"

int plotHistos();
int plotDiffs();
int plotReference(const std::vector<std::vector<ReferenceCS>>& references);

#endif /* PLOT_HISTOS_H */
