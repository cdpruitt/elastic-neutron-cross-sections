#!/bin/bash

./calculateCS Sn112
./calculateCS Sn124

./readDataToGraph experimentalData/Sn112 crossSections.root
./readDataToGraph experimentalData/Sn124 crossSections.root

cd prettyPrint
root .x literatureSn_11MeV.cpp
