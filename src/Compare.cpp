#include "plugin.hpp"


struct Compare : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		INPUTA1_INPUT,
		INPUTB1_INPUT,
		INPUTA2_INPUT,
		INPUTB2_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT1_OUTPUT,
		OUTPUT2_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHT1_LIGHT,
		LIGHT2_LIGHT,
		LIGHTS_LEN
	};

	Compare() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(INPUTA1_INPUT, "");
		configInput(INPUTB1_INPUT, "");
		configInput(INPUTA2_INPUT, "");
		configInput(INPUTB2_INPUT, "");
		configOutput(OUTPUT1_OUTPUT, "");
		configOutput(OUTPUT2_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {


		for (int i = 0; i < OUTPUTS_LEN; i++) {
			// Check for inputs
			if (inputs[i*2].isConnected() && inputs[i*2+1].isConnected()) {
				// Get voltage and compare
				float output = inputs[i*2].getVoltage() >= inputs[i*2+1].getVoltage();
				// Set voltage for output and enable LED
				outputs[i].setVoltage(output * 10.f);
				lights[i].setBrightness(output);
			}
			else {
				// Reset to default state 
				outputs[i].setVoltage(0.f);
				lights[i].setBrightness(0);
			}
		}
	}
};


struct CompareWidget : ModuleWidget {
	CompareWidget(Compare* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Compare.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.929, 26.458)), module, Compare::INPUTA1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.929, 37.042)), module, Compare::INPUTB1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.635, 82.662)), module, Compare::INPUTA2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.635, 93.951)), module, Compare::INPUTB2_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(28.528, 48.556)), module, Compare::OUTPUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(28.635, 105.24)), module, Compare::OUTPUT2_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(35.634, 48.858)), module, Compare::LIGHT1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(35.634, 105.479)), module, Compare::LIGHT2_LIGHT));
	}
};


Model* modelCompare = createModel<Compare, CompareWidget>("Compare");