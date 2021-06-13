/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

void OdinAudioProcessorEditor::setGUIBig() {

	m_tuning.setTopLeftPosition(1.5f * TUNING_POS_X, 1.5f * TUNING_POS_Y);

	juce::Image dropdown_button1 =
	    ImageCache::getFromMemory(BinaryData::buttondropdown_1_150_png, BinaryData::buttondropdown_1_150_pngSize);
	juce::Image dropdown_button2 =
	    ImageCache::getFromMemory(BinaryData::buttondropdown_2_150_png, BinaryData::buttondropdown_2_150_pngSize);

	juce::DrawableImage dropdown_draw1;
	juce::DrawableImage dropdown_draw2;

	dropdown_draw1.setImage(dropdown_button1);
	dropdown_draw2.setImage(dropdown_button2);

	m_osc1_dropdown.setImages(&dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2);

	m_osc1_dropdown.setBounds(OdinHelper::c150(DROPDOWN_OSC1_POS_X),
	                          OdinHelper::c150(DROPDOWN_POS_Y),
	                          dropdown_button1.getWidth(),
	                          dropdown_button1.getHeight());

	m_osc2_dropdown.setImages(&dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2);

	m_osc2_dropdown.setBounds(OdinHelper::c150(DROPDOWN_OSC2_POS_X),
	                          OdinHelper::c150(DROPDOWN_POS_Y),
	                          dropdown_button1.getWidth(),
	                          dropdown_button1.getHeight());

	m_osc3_dropdown.setImages(&dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2);
	m_osc3_dropdown.setBounds(OdinHelper::c150(DROPDOWN_OSC3_POS_X),
	                          OdinHelper::c150(DROPDOWN_POS_Y),
	                          dropdown_button1.getWidth(),
	                          dropdown_button1.getHeight());
	m_filter1_dropdown.setImages(&dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2);
	m_filter1_dropdown.setBounds(OdinHelper::c150(DROPDOWN_FILTER1_POS_X),
	                             OdinHelper::c150(DROPDOWN_FILTER1_POS_Y),
	                             dropdown_button1.getWidth(),
	                             dropdown_button1.getHeight());
	m_filter2_dropdown.setImages(&dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2);
	m_filter2_dropdown.setBounds(OdinHelper::c150(DROPDOWN_FILTER2_POS_X),
	                             OdinHelper::c150(DROPDOWN_FILTER2_POS_Y),
	                             dropdown_button1.getWidth(),
	                             dropdown_button1.getHeight());

	m_filter3_dropdown.setImages(&dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2);
	m_filter3_dropdown.setBounds(OdinHelper::c150(DROPDOWN_FILTER3_POS_X),
	                             OdinHelper::c150(DROPDOWN_FILTER3_POS_Y),
	                             dropdown_button1.getWidth(),
	                             dropdown_button1.getHeight());

	m_fx_buttons_section.setTopLeftPosition(OdinHelper::c150(FX_BUTTON_X) + 2, OdinHelper::c150(FX_BUTTON_Y) + 3);

	juce::Image fx_on_1 =
	    ImageCache::getFromMemory(BinaryData::buttonfxon_1_150_png, BinaryData::buttonfxon_1_150_pngSize);
	juce::Image fx_on_2 =
	    ImageCache::getFromMemory(BinaryData::buttonfxon_2_150_png, BinaryData::buttonfxon_2_150_pngSize);
	juce::Image fx_on_3 =
	    ImageCache::getFromMemory(BinaryData::buttonfxon_3_150_png, BinaryData::buttonfxon_3_150_pngSize);
	juce::Image fx_on_4 =
	    ImageCache::getFromMemory(BinaryData::buttonfxon_4_150_png, BinaryData::buttonfxon_4_150_pngSize);

	juce::DrawableImage fx_on_draw1;
	juce::DrawableImage fx_on_draw2;
	juce::DrawableImage fx_on_draw3;
	juce::DrawableImage fx_on_draw4;

	fx_on_draw1.setImage(fx_on_1);
	fx_on_draw2.setImage(fx_on_2);
	fx_on_draw3.setImage(fx_on_3);
	fx_on_draw4.setImage(fx_on_4);

	m_phaser_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_phaser_on_button.setBounds(
	    OdinHelper::c150(FX_ON_BUTTON_X), OdinHelper::c150(FX_ON_BUTTON_Y), fx_on_1.getWidth(), fx_on_1.getHeight());

	m_flanger_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_flanger_on_button.setBounds(OdinHelper::c150(FX_ON_BUTTON_X) + OdinHelper::c150(FX_BUTTON_OFFSET),
	                              OdinHelper::c150(FX_ON_BUTTON_Y),
	                              fx_on_1.getWidth(),
	                              fx_on_1.getHeight());

	m_chorus_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_chorus_on_button.setBounds(OdinHelper::c150(FX_ON_BUTTON_X) + 2 * OdinHelper::c150(FX_BUTTON_OFFSET),
	                             OdinHelper::c150(FX_ON_BUTTON_Y),
	                             fx_on_1.getWidth(),
	                             fx_on_1.getHeight());
	m_chorus_on_button.setTriggeredOnMouseDown(true);
	m_chorus_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_delay_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_delay_on_button.setClickingTogglesState(true);
	m_delay_on_button.setBounds(OdinHelper::c150(FX_ON_BUTTON_X) + 3 * OdinHelper::c150(FX_BUTTON_OFFSET),
	                            OdinHelper::c150(FX_ON_BUTTON_Y),
	                            fx_on_1.getWidth(),
	                            fx_on_1.getHeight());
	addAndMakeVisible(m_delay_on_button);
	m_delay_on_button.setAlwaysOnTop(true);

	m_reverb_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_reverb_on_button.setClickingTogglesState(true);
	m_reverb_on_button.setBounds(OdinHelper::c150(FX_ON_BUTTON_X) + 3 * OdinHelper::c150(FX_BUTTON_OFFSET),
	                            OdinHelper::c150(FX_ON_BUTTON_Y),
	                            fx_on_1.getWidth(),
	                            fx_on_1.getHeight());
	addAndMakeVisible(m_reverb_on_button);
	m_reverb_on_button.setAlwaysOnTop(true);


	m_reverb_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_reverb_on_button.setClickingTogglesState(true);
	m_reverb_on_button.setBounds(OdinHelper::c150(FX_ON_BUTTON_X) + 3 * OdinHelper::c150(FX_BUTTON_OFFSET),
	                             OdinHelper::c150(FX_ON_BUTTON_Y),
	                             fx_on_1.getWidth(),
	                             fx_on_1.getHeight());
	addAndMakeVisible(m_reverb_on_button);
	m_reverb_on_button.setAlwaysOnTop(true);

	juce::Image filter_button1_1 =
	    ImageCache::getFromMemory(BinaryData::button1_1_150_png, BinaryData::button1_1_150_pngSize);
	juce::Image filter_button1_2 =
	    ImageCache::getFromMemory(BinaryData::button1_2_150_png, BinaryData::button1_2_150_pngSize);
	juce::Image filter_button1_3 =
	    ImageCache::getFromMemory(BinaryData::button1_3_150_png, BinaryData::button1_3_150_pngSize);
	juce::Image filter_button1_4 =
	    ImageCache::getFromMemory(BinaryData::button1_4_150_png, BinaryData::button1_4_150_pngSize);

	juce::DrawableImage filter_button1_draw1;
	juce::DrawableImage filter_button1_draw2;
	juce::DrawableImage filter_button1_draw3;
	juce::DrawableImage filter_button1_draw4;

	filter_button1_draw1.setImage(filter_button1_1);
	filter_button1_draw2.setImage(filter_button1_2);
	filter_button1_draw3.setImage(filter_button1_3);
	filter_button1_draw4.setImage(filter_button1_4);

	m_filleft_button1.setImages(&filter_button1_draw2,
	                            &filter_button1_draw2,
	                            &filter_button1_draw1,
	                            &filter_button1_draw1,
	                            &filter_button1_draw4,
	                            &filter_button1_draw4,
	                            &filter_button1_draw3,
	                            &filter_button1_draw3);
	m_filleft_button1.setBounds(OdinHelper::c150(BUTTON_1_LEFT_POS_X),
	                            OdinHelper::c150(BUTTON_1_LEFT_POS_Y),
	                            filter_button1_1.getWidth(),
	                            filter_button1_1.getHeight());
	m_filright_button1.setImages(&filter_button1_draw2,
	                             &filter_button1_draw2,
	                             &filter_button1_draw1,
	                             &filter_button1_draw1,
	                             &filter_button1_draw4,
	                             &filter_button1_draw4,
	                             &filter_button1_draw3,
	                             &filter_button1_draw3);
	m_filright_button1.setBounds(OdinHelper::c150(BUTTON_2_RIGHT_POS_X),
	                             OdinHelper::c150(BUTTON_2_RIGHT_POS_Y) - filter_button1_1.getHeight(),
	                             filter_button1_1.getWidth(),
	                             filter_button1_1.getHeight());

	juce::Image question_button_1 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_1_150_png, BinaryData::buttonpleb_1_150_pngSize);
	juce::Image question_button_2 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_2_150_png, BinaryData::buttonpleb_2_150_pngSize);
	juce::Image question_button_3 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_3_150_png, BinaryData::buttonpleb_3_150_pngSize);
	juce::Image question_button_4 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_4_150_png, BinaryData::buttonpleb_4_150_pngSize);

	juce::DrawableImage question_button_draw1;
	juce::DrawableImage question_button_draw2;
	juce::DrawableImage question_button_draw3;
	juce::DrawableImage question_button_draw4;

	question_button_draw1.setImage(question_button_1);
	question_button_draw2.setImage(question_button_2);
	question_button_draw3.setImage(question_button_3);
	question_button_draw4.setImage(question_button_4);

	m_question_button.setImages(&question_button_draw2,
	                            &question_button_draw2,
	                            &question_button_draw1,
	                            &question_button_draw1,
	                            &question_button_draw4,
	                            &question_button_draw4,
	                            &question_button_draw3,
	                            &question_button_draw3);
	m_question_button.setBounds(OdinHelper::c150(QUESTION_POS_X),
	                            OdinHelper::c150(QUESTION_POS_Y),
	                            question_button_1.getWidth(),
	                            question_button_1.getHeight());

	juce::Image filter_button2_1 =
	    ImageCache::getFromMemory(BinaryData::button2_1_150_png, BinaryData::button2_1_150_pngSize);
	juce::Image filter_button2_2 =
	    ImageCache::getFromMemory(BinaryData::button2_2_150_png, BinaryData::button2_2_150_pngSize);
	juce::Image filter_button2_3 =
	    ImageCache::getFromMemory(BinaryData::button2_3_150_png, BinaryData::button2_3_150_pngSize);
	juce::Image filter_button2_4 =
	    ImageCache::getFromMemory(BinaryData::button2_4_150_png, BinaryData::button2_4_150_pngSize);

	juce::DrawableImage filter_button2_draw1;
	juce::DrawableImage filter_button2_draw2;
	juce::DrawableImage filter_button2_draw3;
	juce::DrawableImage filter_button2_draw4;

	filter_button2_draw1.setImage(filter_button2_1);
	filter_button2_draw2.setImage(filter_button2_2);
	filter_button2_draw3.setImage(filter_button2_3);
	filter_button2_draw4.setImage(filter_button2_4);

	m_filleft_button2.setImages(&filter_button2_draw2,
	                            &filter_button2_draw2,
	                            &filter_button2_draw1,
	                            &filter_button2_draw1,
	                            &filter_button2_draw4,
	                            &filter_button2_draw4,
	                            &filter_button2_draw3,
	                            &filter_button2_draw3);
	m_filleft_button2.setBounds(OdinHelper::c150(BUTTON_1_LEFT_POS_X),
	                            OdinHelper::c150(BUTTON_1_LEFT_POS_Y) + filter_button2_1.getHeight(),
	                            filter_button2_1.getWidth(),
	                            filter_button2_1.getHeight());

	m_filright_button2.setImages(&filter_button2_draw2,
	                             &filter_button2_draw2,
	                             &filter_button2_draw1,
	                             &filter_button2_draw1,
	                             &filter_button2_draw4,
	                             &filter_button2_draw4,
	                             &filter_button2_draw3,
	                             &filter_button2_draw3);
	m_filright_button2.setBounds(OdinHelper::c150(BUTTON_2_RIGHT_POS_X),
	                             OdinHelper::c150(BUTTON_2_RIGHT_POS_Y),
	                             filter_button2_1.getWidth(),
	                             filter_button2_1.getHeight());

	juce::Image filter_button3_1 =
	    ImageCache::getFromMemory(BinaryData::button3_1_150_png, BinaryData::button3_1_150_pngSize);
	juce::Image filter_button3_2 =
	    ImageCache::getFromMemory(BinaryData::button3_2_150_png, BinaryData::button3_2_150_pngSize);
	juce::Image filter_button3_3 =
	    ImageCache::getFromMemory(BinaryData::button3_3_150_png, BinaryData::button3_3_150_pngSize);
	juce::Image filter_button3_4 =
	    ImageCache::getFromMemory(BinaryData::button3_4_150_png, BinaryData::button3_4_150_pngSize);

	juce::DrawableImage filter_button3_draw1;
	juce::DrawableImage filter_button3_draw2;
	juce::DrawableImage filter_button3_draw3;
	juce::DrawableImage filter_button3_draw4;

	filter_button3_draw1.setImage(filter_button3_1);
	filter_button3_draw2.setImage(filter_button3_2);
	filter_button3_draw3.setImage(filter_button3_3);
	filter_button3_draw4.setImage(filter_button3_4);

	m_filleft_button3.setImages(&filter_button3_draw2,
	                            &filter_button3_draw2,
	                            &filter_button3_draw1,
	                            &filter_button3_draw1,
	                            &filter_button3_draw4,
	                            &filter_button3_draw4,
	                            &filter_button3_draw3,
	                            &filter_button3_draw3);
	m_filleft_button3.setBounds(OdinHelper::c150(BUTTON_1_LEFT_POS_X),
	                            OdinHelper::c150(BUTTON_1_LEFT_POS_Y) + 2 * filter_button3_1.getHeight(),
	                            filter_button3_1.getWidth(),
	                            filter_button3_1.getHeight());
	m_filright_button3.setImages(&filter_button3_draw2,
	                             &filter_button3_draw2,
	                             &filter_button3_draw1,
	                             &filter_button3_draw1,
	                             &filter_button3_draw4,
	                             &filter_button3_draw4,
	                             &filter_button3_draw3,
	                             &filter_button3_draw3);
	m_filright_button3.setBounds(OdinHelper::c150(BUTTON_2_RIGHT_POS_X),
	                             OdinHelper::c150(BUTTON_2_RIGHT_POS_Y) + filter_button3_1.getHeight(),
	                             filter_button3_1.getWidth(),
	                             filter_button3_1.getHeight());

	juce::Image filter_buttonf1_1 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_1_150_png, BinaryData::buttonf1_1_150_pngSize);
	juce::Image filter_buttonf1_2 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_2_150_png, BinaryData::buttonf1_2_150_pngSize);
	juce::Image filter_buttonf1_3 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_3_150_png, BinaryData::buttonf1_3_150_pngSize);
	juce::Image filter_buttonf1_4 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_4_150_png, BinaryData::buttonf1_4_150_pngSize);

	juce::DrawableImage filter_buttonf1_draw1;
	juce::DrawableImage filter_buttonf1_draw2;
	juce::DrawableImage filter_buttonf1_draw3;
	juce::DrawableImage filter_buttonf1_draw4;

	filter_buttonf1_draw1.setImage(filter_buttonf1_1);
	filter_buttonf1_draw2.setImage(filter_buttonf1_2);
	filter_buttonf1_draw3.setImage(filter_buttonf1_3);
	filter_buttonf1_draw4.setImage(filter_buttonf1_4);

	m_filright_buttonf1.setImages(&filter_buttonf1_draw2,
	                              &filter_buttonf1_draw2,
	                              &filter_buttonf1_draw1,
	                              &filter_buttonf1_draw1,
	                              &filter_buttonf1_draw4,
	                              &filter_buttonf1_draw4,
	                              &filter_buttonf1_draw3,
	                              &filter_buttonf1_draw3);
	m_filright_buttonf1.setBounds(OdinHelper::c150(BUTTON_2_RIGHT_POS_X),
	                              OdinHelper::c150(BUTTON_2_RIGHT_POS_Y) + 2 * filter_button3_1.getHeight(),
	                              filter_buttonf1_1.getWidth(),
	                              filter_buttonf1_1.getHeight());
	// load the knob strips
	juce::Image metal_knob_small =
	    ImageCache::getFromMemory(BinaryData::metal_knob_small_150_png, BinaryData::metal_knob_small_150_pngSize);

	juce::Image metal_knob_big =
	    ImageCache::getFromMemory(BinaryData::metal_knob_big_150_png, BinaryData::metal_knob_big_150_pngSize);

	//juce::Image black_knob_mid =
	//    ImageCache::getFromMemory(BinaryData::black_knob_mid_150_png, BinaryData::black_knob_mid_150_pngSize);

	// load backplates for osc and filters

	juce::Image bypass_osc1_plate =
	    ImageCache::getFromMemory(BinaryData::osc1_bypass_150_png, BinaryData::osc1_bypass_150_pngSize);
	juce::Image bypass_osc2_plate =
	    ImageCache::getFromMemory(BinaryData::osc2_bypass_150_png, BinaryData::osc2_bypass_150_pngSize);
	juce::Image bypass_osc3_plate =
	    ImageCache::getFromMemory(BinaryData::osc3_bypass_150_png, BinaryData::osc3_bypass_150_pngSize);

	juce::Image bypass_fil1_plate =
	    ImageCache::getFromMemory(BinaryData::filter1_bypass_150_png, BinaryData::filter1_bypass_150_pngSize);
	juce::Image bypass_fil2_plate =
	    ImageCache::getFromMemory(BinaryData::filter2_bypass_150_png, BinaryData::filter2_bypass_150_pngSize);
	juce::Image bypass_fil3_plate =
	    ImageCache::getFromMemory(BinaryData::filter3_bypass_150_png, BinaryData::filter3_bypass_150_pngSize);

	juce::Image black_knob_small = ImageCache::getFromMemory(BinaryData::black_knob_very_small_150_png,
	                                                         BinaryData::black_knob_very_small_150_pngSize);

	m_unison_detune.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_unison_width.setStrip(black_knob_small, N_KNOB_FRAMES);

	m_glide.setStrip(metal_knob_small, N_KNOB_FRAMES);
	m_master.setStrip(metal_knob_small, N_KNOB_FRAMES);

	juce::Image pitchwheel = ImageCache::getFromMemory(BinaryData::modwheel_150_png, BinaryData::modwheel_150_pngSize);

	m_pitchwheel.setStrip(pitchwheel, N_KNOB_FRAMES, false);
	m_pitchwheel.setBounds(OdinHelper::c150(PITCHWHEEL_X),
	                       OdinHelper::c150(WHEEL_Y),
	                       pitchwheel.getWidth() / N_KNOB_FRAMES,
	                       pitchwheel.getHeight());

	m_modwheel.setStrip(pitchwheel, N_KNOB_FRAMES, false);
	m_modwheel.setBounds(OdinHelper::c150(MODWHEEL_X),
	                     OdinHelper::c150(WHEEL_Y),
	                     pitchwheel.getWidth() / N_KNOB_FRAMES,
	                     pitchwheel.getHeight());

	m_osc1.setBackgroundBypass(bypass_osc1_plate);
	m_osc2.setBackgroundBypass(bypass_osc2_plate);
	m_osc3.setBackgroundBypass(bypass_osc3_plate);

	m_fil1_component.setBackgroundBypass(bypass_fil1_plate);
	m_fil2_component.setBackgroundBypass(bypass_fil2_plate);
	m_fil3_component.setBackgroundBypass(bypass_fil3_plate);

	juce::Image flanger_image =
	    ImageCache::getFromMemory(BinaryData::flangernosync_150_png, BinaryData::flangernosync_150_pngSize);
	juce::Image flanger_image_sync =
	    ImageCache::getFromMemory(BinaryData::flangersync_150_png, BinaryData::flangersync_150_pngSize);
	m_flanger.setImage(flanger_image_sync, true);
	m_flanger.setImage(flanger_image, false);

	juce::Image phaser_image =
	    ImageCache::getFromMemory(BinaryData::phasernosync_150_png, BinaryData::phasernosync_150_pngSize);
	juce::Image phaser_image_sync =
	    ImageCache::getFromMemory(BinaryData::phasersync_150_png, BinaryData::phasersync_150_pngSize);
	m_phaser.setImage(phaser_image_sync, true);
	m_phaser.setImage(phaser_image, false);

	juce::Image chorus_image =
	    ImageCache::getFromMemory(BinaryData::chorusnosync_150_png, BinaryData::chorusnosync_150_pngSize);
	juce::Image chorus_image_sync =
	    ImageCache::getFromMemory(BinaryData::chorussync_150_png, BinaryData::chorussync_150_pngSize);
	m_chorus.setImage(chorus_image_sync, true);
	m_chorus.setImage(chorus_image, false);

	juce::Image delay_image;
	if ((float)m_value_tree.state.getChildWithName("fx")["delay_sync"] < 0.5f) {
		delay_image = ImageCache::getFromMemory(BinaryData::delaynosync_150_png, BinaryData::delaynosync_150_pngSize);
	} else {
		delay_image = ImageCache::getFromMemory(BinaryData::delaysync_150_png, BinaryData::delaysync_150_pngSize);
	}
	m_delay.setImage(delay_image);

	juce::Image env13_left =
	    ImageCache::getFromMemory(BinaryData::buttonenv13_1_150_png, BinaryData::buttonenv13_1_150_pngSize);
	juce::Image env13_right =
	    ImageCache::getFromMemory(BinaryData::buttonenv13_3_150_png, BinaryData::buttonenv13_3_150_pngSize);
	m_env_13_button.setImage(env13_left, 1);
	m_env_13_button.setImage(env13_right, 2);
	m_env_13_button.setBounds(
	    OdinHelper::c150(ENV13_POS_X), OdinHelper::c150(ENV13_POS_Y), env13_left.getWidth(), env13_left.getHeight());

	juce::Image env24_left =
	    ImageCache::getFromMemory(BinaryData::buttonenv24_1_150_png, BinaryData::buttonenv24_1_150_pngSize);
	juce::Image env24_right =
	    ImageCache::getFromMemory(BinaryData::buttonenv24_3_150_png, BinaryData::buttonenv24_3_150_pngSize);
	m_env_24_button.setImage(env24_left, 1);
	m_env_24_button.setImage(env24_right, 2);
	m_env_24_button.setBounds(
	    OdinHelper::c150(ENV24_POS_X), OdinHelper::c150(ENV24_POS_Y), env24_left.getWidth(), env24_left.getHeight());

	juce::Image lfo13_left =
	    ImageCache::getFromMemory(BinaryData::buttonlfo13_1_150_png, BinaryData::buttonlfo13_1_150_pngSize);
	juce::Image lfo13_right =
	    ImageCache::getFromMemory(BinaryData::buttonlfo13_3_150_png, BinaryData::buttonlfo13_3_150_pngSize);
	m_lfo_13_button.setImage(lfo13_left, 1);
	m_lfo_13_button.setImage(lfo13_right, 2);
	m_lfo_13_button.setBounds(
	    OdinHelper::c150(LFO13_POS_X), OdinHelper::c150(LFO13_POS_Y), lfo13_left.getWidth(), lfo13_left.getHeight());

	juce::Image select_arp_button_left  = ImageCache::getFromMemory(BinaryData::button_select_arpeggiator_1_150_png,
                                                                   BinaryData::button_select_arpeggiator_1_150_pngSize);
	juce::Image select_arp_button_right = ImageCache::getFromMemory(
	    BinaryData::button_select_arpeggiator_2_150_png, BinaryData::button_select_arpeggiator_2_150_pngSize);
	juce::DrawableImage select_arp_button_left_draw;
	select_arp_button_left_draw.setImage(select_arp_button_left);
	juce::DrawableImage select_arp_button_right_draw;
	select_arp_button_right_draw.setImage(select_arp_button_right);
	m_select_arp_button.setImages(&select_arp_button_right_draw,
	                              &select_arp_button_right_draw,
	                              &select_arp_button_left_draw,
	                              &select_arp_button_left_draw,
	                              &select_arp_button_left_draw,
	                              &select_arp_button_left_draw,
	                              &select_arp_button_left_draw);
	m_select_arp_button.setBounds(OdinHelper::c150(ARP_MATRIX_BUTTON_POS_X),
	                              OdinHelper::c150(ARP_MATRIX_BUTTON_POS_Y),
	                              select_arp_button_left.getWidth(),
	                              select_arp_button_left.getHeight());

	juce::Image select_modmatrix_button_left = ImageCache::getFromMemory(
	    BinaryData::button_select_modmatrix_1_150_png, BinaryData::button_select_modmatrix_1_150_pngSize);
	juce::Image select_modmatrix_button_right = ImageCache::getFromMemory(
	    BinaryData::button_select_modmatrix_2_150_png, BinaryData::button_select_modmatrix_2_150_pngSize);
	juce::DrawableImage select_modmatrix_button_left_draw;
	select_modmatrix_button_left_draw.setImage(select_modmatrix_button_left);
	juce::DrawableImage select_modmatrix_button_right_draw;
	select_modmatrix_button_right_draw.setImage(select_modmatrix_button_right);
	m_select_modmatrix_button.setImages(&select_modmatrix_button_right_draw,
	                                    &select_modmatrix_button_right_draw,
	                                    &select_modmatrix_button_left_draw,
	                                    &select_modmatrix_button_left_draw,
	                                    &select_modmatrix_button_left_draw,
	                                    &select_modmatrix_button_left_draw,
	                                    &select_modmatrix_button_left_draw);
	m_select_modmatrix_button.setBounds(SELECT_BUTTON_WIDTH_150 + OdinHelper::c150(ARP_MATRIX_BUTTON_POS_X),
	                                    OdinHelper::c150(ARP_MATRIX_BUTTON_POS_Y),
	                                    select_modmatrix_button_left.getWidth(),
	                                    select_modmatrix_button_left.getHeight());

	juce::Image select_presets_button_left  = ImageCache::getFromMemory(BinaryData::button_select_presets_1_150_png,
                                                                       BinaryData::button_select_presets_1_150_pngSize);
	juce::Image select_presets_button_right = ImageCache::getFromMemory(
	    BinaryData::button_select_presets_2_150_png, BinaryData::button_select_presets_2_150_pngSize);
	juce::DrawableImage select_presets_button_left_draw;
	select_presets_button_left_draw.setImage(select_presets_button_left);
	juce::DrawableImage select_presets_button_right_draw;
	select_presets_button_right_draw.setImage(select_presets_button_right);
	m_select_presets_button.setImages(&select_presets_button_right_draw,
	                                  &select_presets_button_right_draw,
	                                  &select_presets_button_left_draw,
	                                  &select_presets_button_left_draw,
	                                  &select_presets_button_left_draw,
	                                  &select_presets_button_left_draw,
	                                  &select_presets_button_left_draw);
	m_select_presets_button.setBounds(SELECT_BUTTON_WIDTH_150 * 2 + OdinHelper::c150(ARP_MATRIX_BUTTON_POS_X),
	                                  OdinHelper::c150(ARP_MATRIX_BUTTON_POS_Y),
	                                  select_presets_button_left.getWidth(),
	                                  select_presets_button_left.getHeight());

	juce::Image lfo24_left =
	    ImageCache::getFromMemory(BinaryData::buttonlfo24_1_150_png, BinaryData::buttonlfo24_1_150_pngSize);
	juce::Image lfo24_right =
	    ImageCache::getFromMemory(BinaryData::buttonlfo24_3_150_png, BinaryData::buttonlfo24_3_150_pngSize);
	m_lfo_24_button.setImage(lfo24_left, 1);
	m_lfo_24_button.setImage(lfo24_right, 2);
	m_lfo_24_button.setBounds(
	    OdinHelper::c150(LFO24_POS_X), OdinHelper::c150(LFO24_POS_Y), lfo24_left.getWidth(), lfo24_left.getHeight());

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_150_png, BinaryData::glaspanel_midbig_150_pngSize);
	m_mono_poly_legato_dropdown.setBounds(
	    OdinHelper::c150(LEGATO_POS_X), OdinHelper::c150(LEGATO_POS_Y), glas_panel.getWidth(), glas_panel.getHeight());
	m_mono_poly_legato_dropdown.setImage(glas_panel);
	m_mono_poly_legato_dropdown.setInlay(1);

	juce::Image gui_size_left =
	    ImageCache::getFromMemory(BinaryData::buttonguisize_1_150_png, BinaryData::buttonguisize_1_150_pngSize);
	juce::Image gui_size_right =
	    ImageCache::getFromMemory(BinaryData::buttonguisize_3_150_png, BinaryData::buttonguisize_3_150_pngSize);
	m_gui_size_button.setImage(gui_size_left, 1);
	m_gui_size_button.setImage(gui_size_right, 2);
	m_gui_size_button.setBounds(OdinHelper::c150(GUI_SIZE_POS_X),
	                            OdinHelper::c150(GUI_SIZE_POS_Y),
	                            gui_size_left.getWidth(),
	                            gui_size_left.getHeight());

	juce::Image reset_1 =
	    ImageCache::getFromMemory(BinaryData::buttonresetsynth_3_150_png, BinaryData::buttonresetsynth_3_150_pngSize);
	juce::Image reset_2 =
	    ImageCache::getFromMemory(BinaryData::buttonresetsynth_2_150_png, BinaryData::buttonresetsynth_2_150_pngSize);

	juce::DrawableImage reset_draw1;
	juce::DrawableImage reset_draw2;

	reset_draw1.setImage(reset_1);
	reset_draw2.setImage(reset_2);

	m_reset.setImages(
	    &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1, &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1);
	m_reset.setBounds(RESET_SYNTH_POS_X_150, RESET_SYNTH_POS_Y_150, reset_1.getWidth(), reset_1.getHeight());

	juce::Image lfo13_sync_background = ImageCache::getFromMemory(BinaryData::lfo13_sync_background_150_png,
	                                                              BinaryData::lfo13_sync_background_150_pngSize);

	juce::Image lfo24_sync_background = ImageCache::getFromMemory(BinaryData::lfo24_sync_background_150_png,
	                                                              BinaryData::lfo24_sync_background_150_pngSize);
	m_lfo_1.setBounds(OdinHelper::c150(LFO_LEFT_POS_X),
	                  OdinHelper::c150(LFO_LEFT_POS_Y),
	                  OdinHelper::c150(LFO_SIZE_X),
	                  OdinHelper::c150(LFO_SIZE_Y));
	m_lfo_1.setSyncOverdraw(lfo13_sync_background);
	m_lfo_2.setBounds(OdinHelper::c150(LFO_LEFT_POS_X),
	                  OdinHelper::c150(LFO_LEFT_POS_Y),
	                  OdinHelper::c150(LFO_SIZE_X),
	                  OdinHelper::c150(LFO_SIZE_Y));
	m_lfo_2.setSyncOverdraw(lfo24_sync_background);
	m_lfo_3.setBounds(OdinHelper::c150(LFO_RIGHT_POS_X),
	                  OdinHelper::c150(LFO_RIGHT_POS_Y),
	                  OdinHelper::c150(LFO_SIZE_X),
	                  OdinHelper::c150(LFO_SIZE_Y));
	m_lfo_3.setSyncOverdraw(lfo13_sync_background);
	m_lfo_4.setBounds(OdinHelper::c150(LFO_RIGHT_POS_X),
	                  OdinHelper::c150(LFO_RIGHT_POS_Y),
	                  OdinHelper::c150(LFO_SIZE_X),
	                  OdinHelper::c150(LFO_SIZE_Y));
	m_lfo_4.setSyncOverdraw(lfo24_sync_background);

	m_pitch_amount.setTopLeftPosition(OdinHelper::c150(PITCH_AMOUNT_X), OdinHelper::c150(PITCH_AMOUNT_Y));
	m_unison_selector.setTopLeftPosition(OdinHelper::c150(UNISON_SELECTOR_X), OdinHelper::c150(UNISON_SELECTOR_Y) + 1);

	m_value_input.setBounds(0, 0, OdinHelper::c150(INPUT_LABEL_SIZE_X), OdinHelper::c150(INPUT_LABEL_SIZE_Y));

	Rectangle<int> area_osc_1(OdinHelper::c150(OSC_SIZE_X), OdinHelper::c150(OSC_SIZE_Y));
	Rectangle<int> area_osc_2(OdinHelper::c150(OSC_SIZE_X), OdinHelper::c150(OSC_SIZE_Y));
	Rectangle<int> area_osc_3(OdinHelper::c150(OSC_SIZE_X), OdinHelper::c150(OSC_SIZE_Y));
	Rectangle<int> area_fil_1(OdinHelper::c150(FILTER_SIZE_X), OdinHelper::c150(FILTER_SIZE_Y));
	Rectangle<int> area_midsection(OdinHelper::c150(MIDSECTION_SIZE_X), OdinHelper::c150(MIDSECTION_SIZE_Y));

	area_osc_1.setPosition(OdinHelper::c150(OSC1_POS_X), OdinHelper::c150(OSC_POS_Y));
	area_osc_2.setPosition(OdinHelper::c150(OSC2_POS_X), OdinHelper::c150(OSC_POS_Y));
	area_osc_3.setPosition(OdinHelper::c150(OSC3_POS_X), OdinHelper::c150(OSC_POS_Y));
	area_fil_1.setPosition(OdinHelper::c150(FIL1_POS_X), OdinHelper::c150(FIL1_POS_Y));
	area_midsection.setPosition(OdinHelper::c150(MIDSECTION_POS_X), OdinHelper::c150(MIDSECTION_POS_Y));

	m_glide.setBounds(OdinHelper::c150(GLIDE_POS_X) - OdinHelper::c150(METAL_KNOB_SMALL_OFFSET_X),
	                  OdinHelper::c150(GLIDE_POS_Y) - OdinHelper::c150(METAL_KNOB_SMALL_OFFSET_Y),
	                  OdinHelper::c150(METAL_KNOB_SMALL_SIZE_X),
	                  OdinHelper::c150(METAL_KNOB_SMALL_SIZE_Y));

	m_master.setBounds(OdinHelper::c150(MASTER_POS_X) - OdinHelper::c150(METAL_KNOB_SMALL_OFFSET_X),
	                   OdinHelper::c150(MASTER_POS_Y) - OdinHelper::c150(METAL_KNOB_SMALL_OFFSET_Y),
	                   OdinHelper::c150(METAL_KNOB_SMALL_SIZE_X),
	                   OdinHelper::c150(METAL_KNOB_SMALL_SIZE_Y));

	m_unison_detune.setBounds(OdinHelper::c150(UNISON_DETUNE_X),
	                          OdinHelper::c150(UNISON_DETUNE_Y),
	                          OdinHelper::c150(BLACK_KNOB_SMALL_SIZE_X),
	                          OdinHelper::c150(BLACK_KNOB_SMALL_SIZE_Y));
	m_unison_width.setBounds(OdinHelper::c150(UNISON_STEREO_X),
	                         OdinHelper::c150(UNISON_STEREO_Y),
	                         OdinHelper::c150(BLACK_KNOB_SMALL_SIZE_X),
	                         OdinHelper::c150(BLACK_KNOB_SMALL_SIZE_Y));

	m_osc1.setBounds(area_osc_1);
	m_osc2.setBounds(area_osc_2);
	m_osc3.setBounds(area_osc_3);

	m_fil1_component.setBounds(area_fil_1);
	m_fil2_component.setBounds(OdinHelper::c150(FIL2_POS_X),
	                           OdinHelper::c150(FIL2_POS_Y),
	                           OdinHelper::c150(FILTER_SIZE_X),
	                           OdinHelper::c150(FILTER_SIZE_Y));
	m_fil3_component.setBounds(OdinHelper::c150(FIL3_POS_X),
	                           OdinHelper::c150(FIL3_POS_Y),
	                           OdinHelper::c150(FILTER_SIZE_X),
	                           OdinHelper::c150(FILTER_SIZE_Y));
	m_midsection.setBounds(area_midsection);

	m_adsr_1.setBounds(OdinHelper::c150(ADSR_LEFT_POS_X),
	                   OdinHelper::c150(ADSR_LEFT_POS_Y),
	                   OdinHelper::c150(ADSR_SIZE_X),
	                   OdinHelper::c150(ADSR_SIZE_Y));
	m_adsr_2.setBounds(OdinHelper::c150(ADSR_RIGHT_POS_X),
	                   OdinHelper::c150(ADSR_RIGHT_POS_Y),
	                   OdinHelper::c150(ADSR_SIZE_X),
	                   OdinHelper::c150(ADSR_SIZE_Y));
	m_adsr_3.setBounds(OdinHelper::c150(ADSR_LEFT_POS_X),
	                   OdinHelper::c150(ADSR_LEFT_POS_Y),
	                   OdinHelper::c150(ADSR_SIZE_X),
	                   OdinHelper::c150(ADSR_SIZE_Y));
	m_adsr_4.setBounds(OdinHelper::c150(ADSR_RIGHT_POS_X),
	                   OdinHelper::c150(ADSR_RIGHT_POS_Y),
	                   OdinHelper::c150(ADSR_SIZE_X),
	                   OdinHelper::c150(ADSR_SIZE_Y));

	m_flanger.setBounds(OdinHelper::c150(FX_AREA_POS_X),
	                    OdinHelper::c150(FX_AREA_POS_Y),
	                    OdinHelper::c150(FX_AREA_SIZE_X),
	                    OdinHelper::c150(FX_AREA_SIZE_Y));
	m_phaser.setBounds(OdinHelper::c150(FX_AREA_POS_X),
	                   OdinHelper::c150(FX_AREA_POS_Y),
	                   OdinHelper::c150(FX_AREA_SIZE_X),
	                   OdinHelper::c150(FX_AREA_SIZE_Y));
	m_chorus.setBounds(OdinHelper::c150(FX_AREA_POS_X),
	                   OdinHelper::c150(FX_AREA_POS_Y),
	                   OdinHelper::c150(FX_AREA_SIZE_X),
	                   OdinHelper::c150(FX_AREA_SIZE_Y));
	m_delay.setBounds(OdinHelper::c150(FX_AREA_POS_X),
	                  OdinHelper::c150(FX_AREA_POS_Y),
	                  OdinHelper::c150(FX_AREA_SIZE_X),
	                  OdinHelper::c150(FX_AREA_SIZE_Y));
	m_reverb.setBounds(OdinHelper::c150(FX_AREA_POS_X),
	                   OdinHelper::c150(FX_AREA_POS_Y),
	                   OdinHelper::c150(FX_AREA_SIZE_X),
	                   OdinHelper::c150(FX_AREA_SIZE_Y));

	m_mod_matrix.setBounds(
	    MATRIX_POS_X_150, MATRIX_POS_Y_150, OdinHelper::c150(MATRIX_SIZE_X), OdinHelper::c150(MATRIX_SIZE_Y));
	m_arp.setBounds(OdinHelper::c150(ARPEGGIATOR_POS_X),
	                OdinHelper::c150(ARPEGGIATOR_POS_Y),
	                OdinHelper::c150(MATRIX_SIZE_X),
	                OdinHelper::c150(MATRIX_SIZE_Y) + 5);
	m_patch_browser.setBounds(BROWSER_POS_X_150, BROWSER_POS_Y_150, BROWSER_SIZE_X_150, BROWSER_SIZE_Y_150);
	//m_save_load.setBounds(OdinHelper::c150(SAVE_LOAD_POS_X),
	//                      OdinHelper::c150(SAVE_LOAD_POS_Y),
	//                      OdinHelper::c150(SAVE_LOAD_SIZE_X),
	//                      OdinHelper::c150(SAVE_LOAD_SIZE_Y));
	m_xy_section.setBounds(OdinHelper::c150(XY_COMPONENT_POS_X),
	                       OdinHelper::c150(XY_COMPONENT_POS_Y),
	                       OdinHelper::c150(XY_COMPONENT_SIZE_X),
	                       OdinHelper::c150(XY_COMPONENT_SIZE_Y));

	m_tooltip.setBounds(900, 100, 100, 100);
	m_tooltip.setAlwaysOnTop(true);

	setOsc1Plate(m_value_tree.state.getChildWithName("osc")["osc1_type"]);
	setOsc2Plate(m_value_tree.state.getChildWithName("osc")["osc2_type"]);
	setOsc3Plate(m_value_tree.state.getChildWithName("osc")["osc3_type"]);
	setFilter1Plate(m_value_tree.state.getChildWithName("misc")["fil1_type"]);
	setFilter2Plate(m_value_tree.state.getChildWithName("misc")["fil2_type"]);
	setFilter3Plate(m_value_tree.state.getChildWithName("misc")["fil3_type"]);

	forceValueTreeOntoComponents(false);
	//m_save_load.resetPatchText();

	setSize(ODIN_EDITOR_SIZE_150_X, ODIN_EDITOR_SIZE_150_Y);
	m_odin_backdrop =
	    ImageCache::getFromMemory(BinaryData::odin_backdrop_150_png, BinaryData::odin_backdrop_150_pngSize);
}