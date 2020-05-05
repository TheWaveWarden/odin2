
#include "ArpComponent.h"
#include <JuceHeader.h>

ArpComponent::ArpComponent(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts) :
    m_processor(p_processor), m_value_tree(vts), m_step_0(vts, 0), m_step_1(vts, 1), m_step_2(vts, 2), m_step_3(vts, 3),
    m_step_4(vts, 4), m_step_5(vts, 5), m_step_6(vts, 6), m_step_7(vts, 7), m_step_8(vts, 8), m_step_9(vts, 9),
    m_step_10(vts, 10), m_step_11(vts, 11), m_step_12(vts, 12), m_step_13(vts, 13), m_step_14(vts, 14),
    m_step_15(vts, 15)

{
	addAndMakeVisible(m_step_0);
	addAndMakeVisible(m_step_1);
	addAndMakeVisible(m_step_2);
	addAndMakeVisible(m_step_3);
	addAndMakeVisible(m_step_4);
	addAndMakeVisible(m_step_5);
	addAndMakeVisible(m_step_6);
	addAndMakeVisible(m_step_7);
	addAndMakeVisible(m_step_8);
	addAndMakeVisible(m_step_9);
	addAndMakeVisible(m_step_10);
	addAndMakeVisible(m_step_11);
	addAndMakeVisible(m_step_12);
	addAndMakeVisible(m_step_13);
	addAndMakeVisible(m_step_14);
	addAndMakeVisible(m_step_15);

	m_octave_selector.OnValueChange = [&](int p_new_value) {
		//todo
		//m_value_tree.state.getChildWithName("misc").setProperty("unison_voices", p_new_value, nullptr);
	};
	m_octave_selector.valueToText = [](int p_value) {
		if (p_value > 1) {
			return std::to_string(p_value) + " Octaves";
		} else {
			return std::to_string(p_value) + " Octave";
		}
	};
	m_octave_selector.setLegalValues({1, 2, 3, 4});

	addAndMakeVisible(m_octave_selector);
	m_octave_selector.setMouseDragDivisor(20.f);
	m_octave_selector.setColor(Colour(10, 40, 50));
	m_octave_selector.setTooltip("TODO");

	m_steps_selector.OnValueChange = [&](int p_new_value) {
		setNumberLEDsToShow(p_new_value);
		//todo
		//m_value_tree.state.getChildWithName("misc").setProperty("unison_voices", p_new_value, nullptr);
	};
	m_steps_selector.valueToText = [](int p_value) {
		if (p_value > 1) {
			return std::to_string(p_value) + " Steps";
		} else {
			return std::to_string(p_value) + " Step";
		}
	};
	m_steps_selector.setLegalValues({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});

	addAndMakeVisible(m_steps_selector);
	m_steps_selector.setMouseDragDivisor(20.f);
	m_steps_selector.setColor(Colour(10, 40, 50));
	m_steps_selector.setTooltip("TODO");

	m_direction.OnValueChange = [&](int p_new_value) {
		//todo
		//m_value_tree.state.getChildWithName("misc").setProperty("unison_voices", p_new_value, nullptr);
	};
	m_direction.valueToText = [&](int p_value) {
		return OdinArpeggiator::ArpPatternToString((OdinArpeggiator::ArpPattern)p_value).toStdString();
	};
	m_direction.setLegalValues({(int)OdinArpeggiator::ArpPattern::Up,
	                            (int)OdinArpeggiator::ArpPattern::Down,
	                            (int)OdinArpeggiator::ArpPattern::UpAndDown,
	                            (int)OdinArpeggiator::ArpPattern::DownAndUp,
	                            (int)OdinArpeggiator::ArpPattern::Random});
	addAndMakeVisible(m_direction);
	m_direction.setMouseDragDivisor(20.f);
	m_direction.setColor(Colour(10, 40, 50));
	m_direction.setTooltip("TODO");

	m_gate.OnValueChange = [&](int p_new_value) {
		//todo
		//m_value_tree.state.getChildWithName("misc").setProperty("unison_voices", p_new_value, nullptr);
	};
	m_gate.valueToText = [&](int p_value) {
		if (p_value < 100) {
			return "Gate: " + std::to_string(p_value) + "%";
		} else {
			return "Gate " + std::to_string(p_value) + "%";
		}
	};
	m_gate.setLegalValues({10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 125, 150, 175, 200});
	addAndMakeVisible(m_gate);
	m_gate.setMouseDragDivisor(20.f);
	m_gate.setColor(Colour(10, 40, 50));
	m_gate.setTooltip("TODO");

	m_sync_time.OnValueChange = [&](int p_left, int p_right) {
		//todo
	};
	m_sync_time.setTooltip("TODO");
	addAndMakeVisible(m_sync_time);

	//todo start and stop timer when component is shown
	startTimer(10);

	//todo remove
	m_steps_selector.setValue(16);
	m_octave_selector.setValue(2);
	m_direction.setValue(1);
	m_gate.setValue(50);
}

ArpComponent::~ArpComponent() {
}

void ArpComponent::paint(Graphics &g) {

	g.setColour(Colours::darkgrey);
	g.drawRect(getLocalBounds(), 1); // draw an outline around the component
}

void ArpComponent::resized() {
}

void ArpComponent::setGUIBig() {
	m_GUI_big = true;

	m_octave_selector.setGUIBig();
	m_steps_selector.setGUIBig();
	m_direction.setGUIBig();
	m_gate.setGUIBig();
	m_sync_time.setGUIBig();

	m_octave_selector.setTopLeftPosition(OCTAVE_SELECTOR_X, OCTAVE_SELECTOR_Y);
	m_steps_selector.setTopLeftPosition(STEP_SELECTOR_X, STEP_SELECTOR_Y);
	m_direction.setTopLeftPosition(DIRECTION_SELECTOR_X, DIRECTION_SELECTOR_Y);
	m_gate.setTopLeftPosition(GATE_SELECTOR_X, GATE_SELECTOR_Y);
	m_sync_time.setTopLeftPosition(SYNC_TIME_ARP_POS_X, SYNC_TIME_ARP_POS_Y);


	for (int step = 0; step < NUMBER_OF_STEPS; ++step) {

		m_step_0.setBounds(STEP_COMPONENT_POS_X + 0 * STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_0.setGUIBig();

		m_step_1.setBounds(STEP_COMPONENT_POS_X + 1 * STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_1.setGUIBig();

		m_step_2.setBounds(STEP_COMPONENT_POS_X + 2 * STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_2.setGUIBig();

		m_step_3.setBounds(STEP_COMPONENT_POS_X + 3 * STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_3.setGUIBig();

		m_step_4.setBounds(STEP_COMPONENT_POS_X + 4 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_4.setGUIBig();

		m_step_5.setBounds(STEP_COMPONENT_POS_X + 5 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_5.setGUIBig();

		m_step_6.setBounds(STEP_COMPONENT_POS_X + 6 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_6.setGUIBig();

		m_step_7.setBounds(STEP_COMPONENT_POS_X + 7 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_7.setGUIBig();

		m_step_8.setBounds(STEP_COMPONENT_POS_X + 8 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING * 2,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_8.setGUIBig();

		m_step_9.setBounds(STEP_COMPONENT_POS_X + 9 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING * 2,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_9.setGUIBig();

		m_step_10.setBounds(STEP_COMPONENT_POS_X + 10 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING * 2,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_10.setGUIBig();

		m_step_11.setBounds(STEP_COMPONENT_POS_X + 11 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING * 2,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_11.setGUIBig();

		m_step_12.setBounds(STEP_COMPONENT_POS_X + 12 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING * 3,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_12.setGUIBig();

		m_step_13.setBounds(STEP_COMPONENT_POS_X + 13 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING * 3,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_13.setGUIBig();

		m_step_14.setBounds(STEP_COMPONENT_POS_X + 14 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING * 3,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_14.setGUIBig();

		m_step_15.setBounds(STEP_COMPONENT_POS_X + 15 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING * 3,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_15.setGUIBig();
	}
}
void ArpComponent::setGUISmall() {
	m_GUI_big = false;
}

void ArpComponent::timerCallback() {
	int led_active = m_processor.m_step_led_active.get();

	if (led_active != -1) {
		setLEDActive(led_active);
	} else {
		setNoLEDActive();
	}
}

void ArpComponent::setLEDActive(int p_LED) {
	m_step_0.setLEDActive(p_LED);
	m_step_1.setLEDActive(p_LED);
	m_step_2.setLEDActive(p_LED);
	m_step_3.setLEDActive(p_LED);
	m_step_4.setLEDActive(p_LED);
	m_step_5.setLEDActive(p_LED);
	m_step_6.setLEDActive(p_LED);
	m_step_7.setLEDActive(p_LED);
	m_step_8.setLEDActive(p_LED);
	m_step_9.setLEDActive(p_LED);
	m_step_10.setLEDActive(p_LED);
	m_step_11.setLEDActive(p_LED);
	m_step_12.setLEDActive(p_LED);
	m_step_13.setLEDActive(p_LED);
	m_step_14.setLEDActive(p_LED);
	m_step_15.setLEDActive(p_LED);
}

void ArpComponent::setNoLEDActive() {
	m_step_0.setLEDActive(-1);
	m_step_1.setLEDActive(-1);
	m_step_2.setLEDActive(-1);
	m_step_3.setLEDActive(-1);
	m_step_4.setLEDActive(-1);
	m_step_5.setLEDActive(-1);
	m_step_6.setLEDActive(-1);
	m_step_7.setLEDActive(-1);
	m_step_8.setLEDActive(-1);
	m_step_9.setLEDActive(-1);
	m_step_10.setLEDActive(-1);
	m_step_11.setLEDActive(-1);
	m_step_12.setLEDActive(-1);
	m_step_13.setLEDActive(-1);
	m_step_14.setLEDActive(-1);
	m_step_15.setLEDActive(-1);
}

void ArpComponent::setNumberLEDsToShow(int p_number) {
	m_step_0.setShowLED(p_number);
	m_step_1.setShowLED(p_number);
	m_step_2.setShowLED(p_number);
	m_step_3.setShowLED(p_number);
	m_step_4.setShowLED(p_number);
	m_step_5.setShowLED(p_number);
	m_step_6.setShowLED(p_number);
	m_step_7.setShowLED(p_number);
	m_step_8.setShowLED(p_number);
	m_step_9.setShowLED(p_number);
	m_step_10.setShowLED(p_number);
	m_step_11.setShowLED(p_number);
	m_step_12.setShowLED(p_number);
	m_step_13.setShowLED(p_number);
	m_step_14.setShowLED(p_number);
	m_step_15.setShowLED(p_number);
}
