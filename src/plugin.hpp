#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
extern Model* modelCompare;
extern Model* modelMultiplexr;
extern Model* modelClock;
extern Model* modelSequence;
extern Model* modelDivide;