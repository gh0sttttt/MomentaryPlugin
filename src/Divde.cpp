#include "plugin.hpp"
#include "dsp/digital.hpp"

#define TRIG_TIME 1e-3f

struct divideByTwo {
	bool update;

	divideByTwo() {update = false; }

	bool process() {
		update ^= 1;
		return update;
	}
};


struct Divide : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		CLOCK_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		DIVIDE_1_OUTPUT, // hidden output
		DIVIDE_2_OUTPUT,
		DIVIDE_4_OUTPUT,
		DIVIDE_8_OUTPUT,
		DIVIDE_16_OUTPUT,
		DIVIDE_32_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHT_1_LIGHT,
		LIGHT_2_LIGHT,
		LIGHT_4_LIGHT,
		LIGHT_8_LIGHT,
		LIGHT_16_LIGHT,
		LIGHT_32_LIGHT,
		LIGHTS_LEN
	};

	divideByTwo divider[OUTPUTS_LEN];
	dsp::PulseGenerator pgen[OUTPUTS_LEN];

	void dividerIteration(int index) {
		if (index > OUTPUTS_LEN-1) return;
		bool activate = divider[index].process();
		pgen[index].trigger(TRIG_TIME);
		if(activate) {
			dividerIteration(index+1);
		}
	}

	dsp::SchmittTrigger edgeDetect;

	Divide() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(CLOCK_INPUT, "");
		configOutput(DIVIDE_2_OUTPUT, "");
		configOutput(DIVIDE_4_OUTPUT, "");
		configOutput(DIVIDE_8_OUTPUT, "");
		configOutput(DIVIDE_16_OUTPUT, "");
		configOutput(DIVIDE_32_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
		if(edgeDetect.process(inputs[CLOCK_INPUT].getVoltage())) {
			dividerIteration(0);
		}

		for (int i = 0; i < OUTPUTS_LEN; i++) {
			float output = pgen[i].process( args.sampleTime );
			outputs[i].setVoltage(10.f * output);
			lights[i].setSmoothBrightness(output, 5e-6f);
		}
	}
};


struct DivideWidget : ModuleWidget {
	DivideWidget(Divide* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/divide.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.879, 26.104)), module, Divide::CLOCK_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.149, 41.525)), module, Divide::DIVIDE_2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.149, 56.142)), module, Divide::DIVIDE_4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.149, 70.698)), module, Divide::DIVIDE_8_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.149, 85.222)), module, Divide::DIVIDE_16_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.149, 99.803)), module, Divide::DIVIDE_32_OUTPUT));

		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.668, 42.003)), module, Divide::LIGHT_2_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.668, 56.291)), module, Divide::LIGHT_4_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.668, 71.107)), module, Divide::LIGHT_8_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.621, 85.64)), module, Divide::LIGHT_16_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.668, 100.211)), module, Divide::LIGHT_32_LIGHT));
	}
};


Model* modelDivide = createModel<Divide, DivideWidget>("divide");