#include "plugin.hpp"
#include "dsp/digital.hpp"

struct Sequence : Module {
	enum ParamId {
		STEP_1_PARAM,
		STEP_2_PARAM,
		STEP_3_PARAM,
		STEP_4_PARAM,
		STEP_5_PARAM,
		STEP_6_PARAM,
		STEP_7_PARAM,
		STEP_8_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		CLOCK_IN_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		SEQUENCE_OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		STEP_1_LED_LIGHT,
		STEP_2_LED_LIGHT,
		STEP_3_LED_LIGHT,
		STEP_4_LED_LIGHT,
		STEP_5_LED_LIGHT,
		STEP_6_LED_LIGHT,
		STEP_7_LED_LIGHT,
		STEP_8_LED_LIGHT,
		LIGHTS_LEN
	};

	// Init new Schmitt Trigger instance
	dsp::SchmittTrigger clockDetector;
	int step = 0;

	Sequence() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		// Config LEDs to step param
		for (int l = 0; l < LIGHTS_LEN; l++)
		{
			configParam(STEP_1_PARAM+l, 0.0, 8.0, 1.0);
		}

		configInput(CLOCK_IN_INPUT, "Clock Input");
		configOutput(SEQUENCE_OUT_OUTPUT, "Sequence");
	}

	void process(const ProcessArgs& args) override {
		// Detects trigger from clock input
		if(clockDetector.process(inputs[CLOCK_IN_INPUT].getVoltage())) 
		{
			// advances step 
			step = (step + 1) & 7;
		}
		// Sets LED for active step 
		for (int i = 0; i < LIGHTS_LEN; i++)
		{
			lights[i].setSmoothBrightness(i == step, 5e-6f);
		}
		// Sends sequence to output 
		outputs[SEQUENCE_OUT_OUTPUT].setVoltage(params[step].getValue());

	}
};


struct SequenceWidget : ModuleWidget {
	SequenceWidget(Sequence* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Sequence.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(30.163, 26.194)), module, Sequence::STEP_1_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(10.848, 35.454)), module, Sequence::STEP_2_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(30.163, 46.038)), module, Sequence::STEP_3_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(10.848, 56.356)), module, Sequence::STEP_4_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(29.898, 67.469)), module, Sequence::STEP_5_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(10.583, 77.788)), module, Sequence::STEP_6_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(30.163, 88.503)), module, Sequence::STEP_7_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(10.319, 98.425)), module, Sequence::STEP_8_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.758, 17.992)), module, Sequence::CLOCK_IN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(29.633, 109.363)), module, Sequence::SEQUENCE_OUT_OUTPUT));

		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(30.427, 34.396)), module, Sequence::STEP_1_LED_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(10.848, 44.979)), module, Sequence::STEP_2_LED_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(30.163, 56.356)), module, Sequence::STEP_3_LED_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(10.848, 66.94)), module, Sequence::STEP_4_LED_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(29.898, 78.317)), module, Sequence::STEP_5_LED_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(10.583, 87.842)), module, Sequence::STEP_6_LED_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(29.898, 98.425)), module, Sequence::STEP_7_LED_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(10.054, 108.479)), module, Sequence::STEP_8_LED_LIGHT));
	}
};


Model* modelSequence = createModel<Sequence, SequenceWidget>("Sequence");