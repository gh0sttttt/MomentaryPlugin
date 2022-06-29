#include "plugin.hpp"
#include "dsp/digital.hpp"

#define CLOCK_TIME 1e-3f


struct Clock : Module {
	enum ParamId {
		TEMPO_KNOB_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		CLOCK_OUT_1_OUTPUT,
		CLOCK_OUT_2_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		CLOCK_LED_1_LIGHT,
		CLOCK_LED_2_LIGHT,
		LIGHTS_LEN
	};

	// Init Pulse generator object

	dsp::PulseGenerator pulseGenerator;
	float counter, length;

	Clock() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(TEMPO_KNOB_PARAM, 30.0, 360.0, 120.0, "Tempo", " BPM");
		configOutput(CLOCK_OUT_1_OUTPUT, "");
		configOutput(CLOCK_OUT_2_OUTPUT, "");

		counter = 0.f;
		length = 0.f;
	}

	void process(const ProcessArgs& args) override {
		// Set BPM value to Tempo Knob value 
		float BPM = params[TEMPO_KNOB_PARAM].getValue();
		// samples
		length = 60.f * args.sampleRate/BPM; 

		if(counter > length) {
			pulseGenerator.trigger(CLOCK_TIME);
			counter -= length; 
		}

		counter++;
		// init clock outputs
		float clockOutput1 = pulseGenerator.process(args.sampleTime);
		float clockOutput2 = pulseGenerator.process(args.sampleTime);
		// Set pulse rate for Output 1 & 2
		// 2nd output is half speed of clock output 
		outputs[CLOCK_OUT_1_OUTPUT].setVoltage(10.f * clockOutput1);
		outputs[CLOCK_OUT_2_OUTPUT].setVoltage(10.f * clockOutput2);
		// Set Output LEDs
			lights[CLOCK_LED_1_LIGHT].setSmoothBrightness(clockOutput1, 5e-6f);
			lights[CLOCK_LED_2_LIGHT].setSmoothBrightness(clockOutput2, 5e-6f);

	}
};

struct ClockWidget : ModuleWidget {
	ClockWidget(Clock* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Clock.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundLargeBlackKnob>(mm2px(Vec(15.7, 38.894)), module, Clock::TEMPO_KNOB_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.875, 70.204)), module, Clock::CLOCK_OUT_1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.875, 94.017)), module, Clock::CLOCK_OUT_2_OUTPUT));

		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(15.875, 79.64)), module, Clock::CLOCK_LED_1_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(16.193, 102.824)), module, Clock::CLOCK_LED_2_LIGHT));
	}
};


Model* modelClock = createModel<Clock, ClockWidget>("Clock");