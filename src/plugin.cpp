#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	// Add modules here
	p->addModel(modelCompare);
	p->addModel(modelMultiplexr);
	p->addModel(modelClock);
	p->addModel(modelSequence);
	p->addModel(modelDivide);
}
