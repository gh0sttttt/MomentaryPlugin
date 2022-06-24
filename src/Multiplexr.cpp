#include "plugin.hpp"


struct Multiplexr : Module {
	enum ParamId {
		INPUT_SELECTOR_PARAM,
		OUTPUT_SELECTOR_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUT2_INPUT,
		INPUT3_INPUT,
		INPUT1_INPUT,
		INPUT4_INPUT,
		DEMUX_INPUT,
		INPUTS_LEN,
		N_INPUT = INPUT4_INPUT,
	};
	enum OutputId {
		OUTPUT2_OUTPUT,
		OUTPUT3_OUTPUT,
		OUTPUT1_OUTPUT,
		OUTPUT4_OUTPUT,
		MUX_OUTPUT,
		OUTPUTS_LEN,
		N_OUTPUT = OUTPUT4_OUTPUT,
	};
	enum LightId {
		INPUT_LIGHT2_LIGHT,
		INPUT_LIGHT3_LIGHT,
		INPUT_LIGHT1_LIGHT,
		INPUT_LIGHT4_LIGHT,
		OUTPUT_LIGHT2_LIGHT,
		OUTPUT_LIGHT3_LIGHT,
		OUTPUT_LIGHT1_LIGHT,
		OUTPUT_LIGHT4_LIGHT,
		LIGHTS_LEN
	};

	unsigned int muxSelector, demuxSelector;

	Multiplexr() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(INPUT_SELECTOR_PARAM, 0.0, 3.0, 0.0, "Mux Selector");
		configParam(OUTPUT_SELECTOR_PARAM, 0.0, 3.0, 0.0, "Demux Selector");
		// Init selectors
		muxSelector = 0;
		demuxSelector = 0;

		configInput(INPUT2_INPUT, "");
		configInput(INPUT3_INPUT, "");
		configInput(INPUT1_INPUT, "");
		configInput(INPUT4_INPUT, "");
		configInput(MUX_OUTPUT, "");
		configInput(DEMUX_INPUT, "");
		configOutput(OUTPUT2_OUTPUT, "");
		configOutput(OUTPUT3_OUTPUT, "");
		configOutput(OUTPUT1_OUTPUT, "");
		configOutput(OUTPUT4_OUTPUT, "");
	}


	void process(const ProcessArgs& args) override {

		// Mux
		// Set light to 0 before checking for input
		lights[muxSelector].setBrightness(0.f);
		// Cast input value to int 
		muxSelector = (unsigned int)clamp((int)params[INPUT_SELECTOR_PARAM].getValue(), 0, N_INPUT );
		//  Toggle light on when an input is selected with knob 
		lights[muxSelector].setBrightness(1.f);
		// Check for output connection
		if(outputs[MUX_OUTPUT].isConnected()) 
		{
			// Check for input connection
			if(inputs[muxSelector].isConnected()) 
			{
				outputs[MUX_OUTPUT].setVoltage(inputs[muxSelector].getVoltage());
			}
		}

		// Demux
		lights[demuxSelector+N_INPUT+1].setBrightness(0.f);
		demuxSelector = (unsigned int)clamp((int)params[OUTPUT_SELECTOR_PARAM].getValue(), 0, N_OUTPUT+1);
		lights[demuxSelector+N_INPUT+1].setBrightness(1.f);

		if(inputs[DEMUX_INPUT].isConnected())
		{
			if(outputs[demuxSelector].isConnected())
			{
				outputs[demuxSelector].setVoltage(inputs[DEMUX_INPUT].getVoltage());
			}
		}
	}
};


struct MultiplexrWidget : ModuleWidget {
	MultiplexrWidget(Multiplexr* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Multiplexr.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundLargeBlackKnob>(mm2px(Vec(20.463, 38.894)), module, Multiplexr::INPUT_SELECTOR_PARAM));
		addParam(createParamCentered<RoundLargeBlackKnob>(mm2px(Vec(20.577, 88.965)), module, Multiplexr::OUTPUT_SELECTOR_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.35, 28.31)), module, Multiplexr::INPUT2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(31.635, 28.6)), module, Multiplexr::INPUT3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.41, 49.587)), module, Multiplexr::INPUT1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(31.696, 49.877)), module, Multiplexr::INPUT4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(20.523, 108.06)), module, Multiplexr::DEMUX_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.465, 78.381)), module, Multiplexr::OUTPUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(31.75, 78.671)), module, Multiplexr::OUTPUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.525, 99.658)), module, Multiplexr::OUTPUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(31.81, 99.948)), module, Multiplexr::OUTPUT4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.523, 57.525)), module, Multiplexr::MUX_OUTPUT));

		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(13.908, 31.95)), module, Multiplexr::INPUT_LIGHT2_LIGHT));
		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(27.667, 32.479)), module, Multiplexr::INPUT_LIGHT3_LIGHT));
		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(13.644, 46.238)), module, Multiplexr::INPUT_LIGHT1_LIGHT));
		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(26.873, 46.238)), module, Multiplexr::INPUT_LIGHT4_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(14.023, 82.021)), module, Multiplexr::OUTPUT_LIGHT2_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(27.781, 82.55)), module, Multiplexr::OUTPUT_LIGHT3_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(13.758, 96.308)), module, Multiplexr::OUTPUT_LIGHT1_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(26.988, 96.308)), module, Multiplexr::OUTPUT_LIGHT4_LIGHT));
	}
};


Model* modelMultiplexr = createModel<Multiplexr, MultiplexrWidget>("Multiplexr");