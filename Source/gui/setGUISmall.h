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

void OdinAudioProcessorEditor::setGUISmall() {

	m_tuning.setTopLeftPosition(TUNING_POS_X, TUNING_POS_Y);

	juce::Image dropdown_button1 =
	    ImageCache::getFromMemory(BinaryData::buttondropdown_1_png, BinaryData::buttondropdown_1_pngSize);
	juce::Image dropdown_button2 =
	    ImageCache::getFromMemory(BinaryData::buttondropdown_2_png, BinaryData::buttondropdown_2_pngSize);

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

	m_osc1_dropdown.setBounds(
	    DROPDOWN_OSC1_POS_X, DROPDOWN_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());

	m_osc2_dropdown.setImages(&dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2);

	m_osc2_dropdown.setBounds(
	    DROPDOWN_OSC2_POS_X, DROPDOWN_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());

	m_osc3_dropdown.setImages(&dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2);
	m_osc3_dropdown.setBounds(
	    DROPDOWN_OSC3_POS_X, DROPDOWN_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());
	m_filter1_dropdown.setImages(&dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2);
	m_filter1_dropdown.setBounds(
	    DROPDOWN_FILTER1_POS_X, DROPDOWN_FILTER1_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());
	m_filter2_dropdown.setImages(&dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2);
	m_filter2_dropdown.setBounds(
	    DROPDOWN_FILTER2_POS_X, DROPDOWN_FILTER2_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());

	m_filter3_dropdown.setImages(&dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2);
	m_filter3_dropdown.setBounds(
	    DROPDOWN_FILTER3_POS_X, DROPDOWN_FILTER3_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());

	m_fx_buttons_section.setTopLeftPosition(FX_BUTTON_X, FX_BUTTON_Y);

	juce::Image fx_on_1 = ImageCache::getFromMemory(BinaryData::buttonfxon_1_png, BinaryData::buttonfxon_1_pngSize);
	juce::Image fx_on_2 = ImageCache::getFromMemory(BinaryData::buttonfxon_2_png, BinaryData::buttonfxon_2_pngSize);
	juce::Image fx_on_3 = ImageCache::getFromMemory(BinaryData::buttonfxon_3_png, BinaryData::buttonfxon_3_pngSize);
	juce::Image fx_on_4 = ImageCache::getFromMemory(BinaryData::buttonfxon_4_png, BinaryData::buttonfxon_4_pngSize);

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
	m_phaser_on_button.setBounds(FX_ON_BUTTON_X, FX_ON_BUTTON_Y, fx_on_1.getWidth(), fx_on_1.getHeight());

	m_flanger_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_flanger_on_button.setBounds(
	    FX_ON_BUTTON_X + FX_BUTTON_OFFSET, FX_ON_BUTTON_Y, fx_on_1.getWidth(), fx_on_1.getHeight());

	m_chorus_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_chorus_on_button.setBounds(
	    FX_ON_BUTTON_X + 2 * FX_BUTTON_OFFSET, FX_ON_BUTTON_Y, fx_on_1.getWidth(), fx_on_1.getHeight());
	m_chorus_on_button.setTriggeredOnMouseDown(true);
	m_chorus_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_delay_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_delay_on_button.setClickingTogglesState(true);
	m_delay_on_button.setBounds(
	    FX_ON_BUTTON_X + 3 * FX_BUTTON_OFFSET, FX_ON_BUTTON_Y, fx_on_1.getWidth(), fx_on_1.getHeight());
	addAndMakeVisible(m_delay_on_button);
	m_delay_on_button.setAlwaysOnTop(true);

	m_reverb_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_reverb_on_button.setClickingTogglesState(true);
	m_reverb_on_button.setBounds(
	    FX_ON_BUTTON_X + 3 * FX_BUTTON_OFFSET, FX_ON_BUTTON_Y, fx_on_1.getWidth(), fx_on_1.getHeight());
	addAndMakeVisible(m_reverb_on_button);
	m_reverb_on_button.setAlwaysOnTop(true);

	juce::Image filter_button1_1 = ImageCache::getFromMemory(BinaryData::button1_1_png, BinaryData::button1_1_pngSize);
	juce::Image filter_button1_2 = ImageCache::getFromMemory(BinaryData::button1_2_png, BinaryData::button1_2_pngSize);
	juce::Image filter_button1_3 = ImageCache::getFromMemory(BinaryData::button1_3_png, BinaryData::button1_3_pngSize);
	juce::Image filter_button1_4 = ImageCache::getFromMemory(BinaryData::button1_4_png, BinaryData::button1_4_pngSize);

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
	m_filleft_button1.setBounds(
	    BUTTON_1_LEFT_POS_X, BUTTON_1_LEFT_POS_Y, filter_button1_1.getWidth(), filter_button1_1.getHeight());
	m_filright_button1.setImages(&filter_button1_draw2,
	                             &filter_button1_draw2,
	                             &filter_button1_draw1,
	                             &filter_button1_draw1,
	                             &filter_button1_draw4,
	                             &filter_button1_draw4,
	                             &filter_button1_draw3,
	                             &filter_button1_draw3);
	m_filright_button1.setBounds(BUTTON_2_RIGHT_POS_X,
	                             BUTTON_2_RIGHT_POS_Y - filter_button1_1.getHeight(),
	                             filter_button1_1.getWidth(),
	                             filter_button1_1.getHeight());

	juce::Image question_button_1 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_1_png, BinaryData::buttonpleb_1_pngSize);
	juce::Image question_button_2 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_2_png, BinaryData::buttonpleb_2_pngSize);
	juce::Image question_button_3 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_3_png, BinaryData::buttonpleb_3_pngSize);
	juce::Image question_button_4 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_4_png, BinaryData::buttonpleb_4_pngSize);

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
	m_question_button.setBounds(
	    QUESTION_POS_X, QUESTION_POS_Y, question_button_1.getWidth(), question_button_1.getHeight());

	juce::Image filter_button2_1 = ImageCache::getFromMemory(BinaryData::button2_1_png, BinaryData::button2_1_pngSize);
	juce::Image filter_button2_2 = ImageCache::getFromMemory(BinaryData::button2_2_png, BinaryData::button2_2_pngSize);
	juce::Image filter_button2_3 = ImageCache::getFromMemory(BinaryData::button2_3_png, BinaryData::button2_3_pngSize);
	juce::Image filter_button2_4 = ImageCache::getFromMemory(BinaryData::button2_4_png, BinaryData::button2_4_pngSize);

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
	m_filleft_button2.setBounds(BUTTON_1_LEFT_POS_X,
	                            BUTTON_1_LEFT_POS_Y + filter_button2_1.getHeight(),
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
	m_filright_button2.setBounds(
	    BUTTON_2_RIGHT_POS_X, BUTTON_2_RIGHT_POS_Y, filter_button2_1.getWidth(), filter_button2_1.getHeight());

	juce::Image filter_button3_1 = ImageCache::getFromMemory(BinaryData::button3_1_png, BinaryData::button3_1_pngSize);
	juce::Image filter_button3_2 = ImageCache::getFromMemory(BinaryData::button3_2_png, BinaryData::button3_2_pngSize);
	juce::Image filter_button3_3 = ImageCache::getFromMemory(BinaryData::button3_3_png, BinaryData::button3_3_pngSize);
	juce::Image filter_button3_4 = ImageCache::getFromMemory(BinaryData::button3_4_png, BinaryData::button3_4_pngSize);

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
	m_filleft_button3.setBounds(BUTTON_1_LEFT_POS_X,
	                            BUTTON_1_LEFT_POS_Y + 2 * filter_button3_1.getHeight(),
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
	m_filright_button3.setBounds(BUTTON_2_RIGHT_POS_X,
	                             BUTTON_2_RIGHT_POS_Y + filter_button3_1.getHeight(),
	                             filter_button3_1.getWidth(),
	                             filter_button3_1.getHeight());

	juce::Image filter_buttonf1_1 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_1_png, BinaryData::buttonf1_1_pngSize);
	juce::Image filter_buttonf1_2 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_2_png, BinaryData::buttonf1_2_pngSize);
	juce::Image filter_buttonf1_3 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_3_png, BinaryData::buttonf1_3_pngSize);
	juce::Image filter_buttonf1_4 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_4_png, BinaryData::buttonf1_4_pngSize);

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
	m_filright_buttonf1.setBounds(BUTTON_2_RIGHT_POS_X,
	                              BUTTON_2_RIGHT_POS_Y + 2 * filter_button3_1.getHeight(),
	                              filter_buttonf1_1.getWidth(),
	                              filter_buttonf1_1.getHeight());
	// load the knob strips
	juce::Image metal_knob_small =
	    ImageCache::getFromMemory(BinaryData::metal_knob_small_png, BinaryData::metal_knob_small_pngSize);

	juce::Image metal_knob_big =
	    ImageCache::getFromMemory(BinaryData::metal_knob_big_png, BinaryData::metal_knob_big_pngSize);

	//juce::Image black_knob_mid =
	//    ImageCache::getFromMemory(BinaryData::black_knob_mid_png, BinaryData::black_knob_mid_pngSize);

	// load backplates for osc and filters

	juce::Image bypass_osc1_plate =
	    ImageCache::getFromMemory(BinaryData::osc1_bypass_png, BinaryData::osc1_bypass_pngSize);
	juce::Image bypass_osc2_plate =
	    ImageCache::getFromMemory(BinaryData::osc2_bypass_png, BinaryData::osc2_bypass_pngSize);
	juce::Image bypass_osc3_plate =
	    ImageCache::getFromMemory(BinaryData::osc3_bypass_png, BinaryData::osc3_bypass_pngSize);

	juce::Image bypass_fil1_plate =
	    ImageCache::getFromMemory(BinaryData::filter1_bypass_png, BinaryData::filter1_bypass_pngSize);
	juce::Image bypass_fil2_plate =
	    ImageCache::getFromMemory(BinaryData::filter2_bypass_png, BinaryData::filter2_bypass_pngSize);
	juce::Image bypass_fil3_plate =
	    ImageCache::getFromMemory(BinaryData::filter3_bypass_png, BinaryData::filter3_bypass_pngSize);

	juce::Image black_knob_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_very_small_png, BinaryData::black_knob_very_small_pngSize);

	m_glide.setStrip(metal_knob_small, N_KNOB_FRAMES);
	m_master.setStrip(metal_knob_small, N_KNOB_FRAMES);
	m_unison_detune.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_unison_width.setStrip(black_knob_small, N_KNOB_FRAMES);

	juce::Image pitchwheel = ImageCache::getFromMemory(BinaryData::modwheel_png, BinaryData::modwheel_pngSize);

	m_pitchwheel.setStrip(pitchwheel, N_KNOB_FRAMES, false);
	m_pitchwheel.setBounds(PITCHWHEEL_X, WHEEL_Y, pitchwheel.getWidth() / N_KNOB_FRAMES, pitchwheel.getHeight());

	m_modwheel.setStrip(pitchwheel, N_KNOB_FRAMES, false);
	m_modwheel.setBounds(MODWHEEL_X, WHEEL_Y, pitchwheel.getWidth() / N_KNOB_FRAMES, pitchwheel.getHeight());

	m_osc1.setBackgroundBypass(bypass_osc1_plate);
	m_osc2.setBackgroundBypass(bypass_osc2_plate);
	m_osc3.setBackgroundBypass(bypass_osc3_plate);

	m_fil1_component.setBackgroundBypass(bypass_fil1_plate);
	m_fil2_component.setBackgroundBypass(bypass_fil2_plate);
	m_fil3_component.setBackgroundBypass(bypass_fil3_plate);

	juce::Image flanger_image =
	    ImageCache::getFromMemory(BinaryData::flangernosync_png, BinaryData::flangernosync_pngSize);
	juce::Image flanger_image_sync =
	    ImageCache::getFromMemory(BinaryData::flangersync_png, BinaryData::flangersync_pngSize);
	m_flanger.setImage(flanger_image_sync, true);
	m_flanger.setImage(flanger_image, false);

	juce::Image phaser_image =
	    ImageCache::getFromMemory(BinaryData::phasernosync_png, BinaryData::phasernosync_pngSize);
	juce::Image phaser_image_sync =
	    ImageCache::getFromMemory(BinaryData::phasersync_png, BinaryData::phasersync_pngSize);
	m_phaser.setImage(phaser_image_sync, true);
	m_phaser.setImage(phaser_image, false);

	juce::Image chorus_image =
	    ImageCache::getFromMemory(BinaryData::chorusnosync_png, BinaryData::chorusnosync_pngSize);
	juce::Image chorus_image_sync =
	    ImageCache::getFromMemory(BinaryData::chorussync_png, BinaryData::chorussync_pngSize);
	m_chorus.setImage(chorus_image_sync, true);
	m_chorus.setImage(chorus_image, false);

	juce::Image delay_image;
	if ((float)m_value_tree.state.getChildWithName("fx")["delay_sync"] < 0.5f) {
		delay_image = ImageCache::getFromMemory(BinaryData::delaynosync_png, BinaryData::delaynosync_pngSize);
	} else {
		delay_image = ImageCache::getFromMemory(BinaryData::delaysync_png, BinaryData::delaysync_pngSize);
	}
	m_delay.setImage(delay_image);

	juce::Image env13_left =
	    ImageCache::getFromMemory(BinaryData::buttonenv13_1_png, BinaryData::buttonenv13_1_pngSize);
	juce::Image env13_right =
	    ImageCache::getFromMemory(BinaryData::buttonenv13_3_png, BinaryData::buttonenv13_3_pngSize);
	m_env_13_button.setImage(env13_left, 1);
	m_env_13_button.setImage(env13_right, 2);
	m_env_13_button.setBounds(ENV13_POS_X, ENV13_POS_Y, env13_left.getWidth(), env13_left.getHeight());

	juce::Image env24_left =
	    ImageCache::getFromMemory(BinaryData::buttonenv24_1_png, BinaryData::buttonenv24_1_pngSize);
	juce::Image env24_right =
	    ImageCache::getFromMemory(BinaryData::buttonenv24_3_png, BinaryData::buttonenv24_3_pngSize);
	m_env_24_button.setImage(env24_left, 1);
	m_env_24_button.setImage(env24_right, 2);
	m_env_24_button.setBounds(ENV24_POS_X, ENV24_POS_Y, env24_left.getWidth(), env24_left.getHeight());

	juce::Image lfo13_left =
	    ImageCache::getFromMemory(BinaryData::buttonlfo13_1_png, BinaryData::buttonlfo13_1_pngSize);
	juce::Image lfo13_right =
	    ImageCache::getFromMemory(BinaryData::buttonlfo13_3_png, BinaryData::buttonlfo13_3_pngSize);
	m_lfo_13_button.setImage(lfo13_left, 1);
	m_lfo_13_button.setImage(lfo13_right, 2);
	m_lfo_13_button.setBounds(LFO13_POS_X, LFO13_POS_Y, lfo13_left.getWidth(), lfo13_left.getHeight());

	juce::Image lfo24_left =
	    ImageCache::getFromMemory(BinaryData::buttonlfo24_1_png, BinaryData::buttonlfo24_1_pngSize);
	juce::Image lfo24_right =
	    ImageCache::getFromMemory(BinaryData::buttonlfo24_3_png, BinaryData::buttonlfo24_3_pngSize);
	m_lfo_24_button.setImage(lfo24_left, 1);
	m_lfo_24_button.setImage(lfo24_right, 2);
	m_lfo_24_button.setBounds(LFO24_POS_X, LFO24_POS_Y, lfo24_left.getWidth(), lfo24_left.getHeight());

	juce::Image select_arp_button_left  = ImageCache::getFromMemory(BinaryData::button_select_arpeggiator_1_png,
                                                                   BinaryData::button_select_arpeggiator_1_pngSize);
	juce::Image select_arp_button_right = ImageCache::getFromMemory(BinaryData::button_select_arpeggiator_2_png,
	                                                                BinaryData::button_select_arpeggiator_2_pngSize);
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
	m_select_arp_button.setBounds(ARP_MATRIX_BUTTON_POS_X,
	                              ARP_MATRIX_BUTTON_POS_Y,
	                              select_arp_button_left.getWidth(),
	                              select_arp_button_left.getHeight());

	juce::Image select_modmatrix_button_left  = ImageCache::getFromMemory(BinaryData::button_select_modmatrix_1_png,
                                                                         BinaryData::button_select_modmatrix_1_pngSize);
	juce::Image select_modmatrix_button_right = ImageCache::getFromMemory(
	    BinaryData::button_select_modmatrix_2_png, BinaryData::button_select_modmatrix_2_pngSize);
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
	m_select_modmatrix_button.setBounds(SELECT_BUTTON_WIDTH + ARP_MATRIX_BUTTON_POS_X,
	                                    ARP_MATRIX_BUTTON_POS_Y,
	                                    select_modmatrix_button_left.getWidth(),
	                                    select_modmatrix_button_left.getHeight());

	juce::Image select_presets_button_left =
	    ImageCache::getFromMemory(BinaryData::button_select_presets_1_png, BinaryData::button_select_presets_1_pngSize);
	juce::Image select_presets_button_right =
	    ImageCache::getFromMemory(BinaryData::button_select_presets_2_png, BinaryData::button_select_presets_2_pngSize);
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
	m_select_presets_button.setBounds(SELECT_BUTTON_WIDTH * 2 + ARP_MATRIX_BUTTON_POS_X,
	                                  ARP_MATRIX_BUTTON_POS_Y,
	                                  select_presets_button_left.getWidth(),
	                                  select_presets_button_left.getHeight());

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_png, BinaryData::glaspanel_midbig_pngSize);
	m_mono_poly_legato_dropdown.setBounds(LEGATO_POS_X, LEGATO_POS_Y, glas_panel.getWidth(), glas_panel.getHeight());
	m_mono_poly_legato_dropdown.setImage(glas_panel);
	m_mono_poly_legato_dropdown.setInlay(1);

	juce::Image gui_size_left =
	    ImageCache::getFromMemory(BinaryData::buttonguisize_1_png, BinaryData::buttonguisize_1_pngSize);
	juce::Image gui_size_right =
	    ImageCache::getFromMemory(BinaryData::buttonguisize_3_png, BinaryData::buttonguisize_3_pngSize);
	m_gui_size_button.setImage(gui_size_left, 1);
	m_gui_size_button.setImage(gui_size_right, 2);
	m_gui_size_button.setBounds(GUI_SIZE_POS_X, GUI_SIZE_POS_Y, gui_size_left.getWidth(), gui_size_left.getHeight());

	juce::Image lfo13_sync_background =
	    ImageCache::getFromMemory(BinaryData::lfo13_sync_background_png, BinaryData::lfo13_sync_background_pngSize);

	juce::Image lfo24_sync_background =
	    ImageCache::getFromMemory(BinaryData::lfo24_sync_background_png, BinaryData::lfo24_sync_background_pngSize);

	juce::Image reset_1 =
	    ImageCache::getFromMemory(BinaryData::buttonresetsynth_3_png, BinaryData::buttonresetsynth_3_pngSize);
	juce::Image reset_2 =
	    ImageCache::getFromMemory(BinaryData::buttonresetsynth_2_png, BinaryData::buttonresetsynth_2_pngSize);

	juce::DrawableImage reset_draw1;
	juce::DrawableImage reset_draw2;

	reset_draw1.setImage(reset_1);
	reset_draw2.setImage(reset_2);

	m_reset.setImages(
	    &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1, &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1);
	m_reset.setBounds(RESET_SYNTH_POS_X_100, RESET_SYNTH_POS_Y_100, reset_1.getWidth(), reset_1.getHeight());

	m_lfo_1.setBounds(LFO_LEFT_POS_X, LFO_LEFT_POS_Y, LFO_SIZE_X, LFO_SIZE_Y);
	m_lfo_1.setSyncOverdraw(lfo13_sync_background);
	m_lfo_2.setBounds(LFO_LEFT_POS_X, LFO_LEFT_POS_Y, LFO_SIZE_X, LFO_SIZE_Y);
	m_lfo_2.setSyncOverdraw(lfo24_sync_background);
	m_lfo_3.setBounds(LFO_RIGHT_POS_X, LFO_RIGHT_POS_Y, LFO_SIZE_X, LFO_SIZE_Y);
	m_lfo_3.setSyncOverdraw(lfo13_sync_background);
	m_lfo_4.setBounds(LFO_RIGHT_POS_X, LFO_RIGHT_POS_Y, LFO_SIZE_X, LFO_SIZE_Y);
	m_lfo_4.setSyncOverdraw(lfo24_sync_background);

	m_pitch_amount.setTopLeftPosition(PITCH_AMOUNT_X, PITCH_AMOUNT_Y);
	m_unison_selector.setTopLeftPosition(UNISON_SELECTOR_X, UNISON_SELECTOR_Y);

	m_value_input.setBounds(0, 0, INPUT_LABEL_SIZE_X, INPUT_LABEL_SIZE_Y);

	setOsc1Plate(m_value_tree.state.getChildWithName("osc")["osc1_type"]);
	setOsc2Plate(m_value_tree.state.getChildWithName("osc")["osc2_type"]);
	setOsc3Plate(m_value_tree.state.getChildWithName("osc")["osc3_type"]);
	setFilter1Plate(m_value_tree.state.getChildWithName("misc")["fil1_type"]);
	setFilter2Plate(m_value_tree.state.getChildWithName("misc")["fil2_type"]);
	setFilter3Plate(m_value_tree.state.getChildWithName("misc")["fil3_type"]);

	Rectangle<int> area_osc_1(OSC_SIZE_X, OSC_SIZE_Y);
	Rectangle<int> area_osc_2(OSC_SIZE_X, OSC_SIZE_Y);
	Rectangle<int> area_osc_3(OSC_SIZE_X, OSC_SIZE_Y);
	Rectangle<int> area_fil_1(FILTER_SIZE_X, FILTER_SIZE_Y);
	Rectangle<int> area_midsection(MIDSECTION_SIZE_X, MIDSECTION_SIZE_Y);

	area_osc_1.setPosition(OSC1_POS_X, OSC_POS_Y);
	area_osc_2.setPosition(OSC2_POS_X, OSC_POS_Y);
	area_osc_3.setPosition(OSC3_POS_X, OSC_POS_Y);
	area_fil_1.setPosition(FIL1_POS_X, FIL1_POS_Y);
	area_midsection.setPosition(MIDSECTION_POS_X, MIDSECTION_POS_Y);

	m_glide.setBounds(GLIDE_POS_X - METAL_KNOB_SMALL_OFFSET_X,
	                  GLIDE_POS_Y - METAL_KNOB_SMALL_OFFSET_Y,
	                  METAL_KNOB_SMALL_SIZE_X,
	                  METAL_KNOB_SMALL_SIZE_Y);

	m_master.setBounds(MASTER_POS_X - METAL_KNOB_SMALL_OFFSET_X,
	                   MASTER_POS_Y - METAL_KNOB_SMALL_OFFSET_Y,
	                   METAL_KNOB_SMALL_SIZE_X,
	                   METAL_KNOB_SMALL_SIZE_Y);

	m_unison_detune.setBounds(UNISON_DETUNE_X, UNISON_DETUNE_Y, BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);
	m_unison_width.setBounds(UNISON_STEREO_X, UNISON_STEREO_Y, BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);

	m_osc1.setBounds(area_osc_1);
	m_osc2.setBounds(area_osc_2);
	m_osc3.setBounds(area_osc_3);

	m_fil1_component.setBounds(area_fil_1);
	m_fil2_component.setBounds(FIL2_POS_X, FIL2_POS_Y, FILTER_SIZE_X, FILTER_SIZE_Y);
	m_fil3_component.setBounds(FIL3_POS_X, FIL3_POS_Y, FILTER_SIZE_X, FILTER_SIZE_Y);
	m_midsection.setBounds(area_midsection);

	m_adsr_1.setBounds(ADSR_LEFT_POS_X, ADSR_LEFT_POS_Y, ADSR_SIZE_X, ADSR_SIZE_Y);
	m_adsr_2.setBounds(ADSR_RIGHT_POS_X, ADSR_RIGHT_POS_Y, ADSR_SIZE_X, ADSR_SIZE_Y);
	m_adsr_3.setBounds(ADSR_LEFT_POS_X, ADSR_LEFT_POS_Y, ADSR_SIZE_X, ADSR_SIZE_Y);
	m_adsr_4.setBounds(ADSR_RIGHT_POS_X, ADSR_RIGHT_POS_Y, ADSR_SIZE_X, ADSR_SIZE_Y);

	m_flanger.setBounds(FX_AREA_POS_X, FX_AREA_POS_Y, FX_AREA_SIZE_X, FX_AREA_SIZE_Y);
	m_phaser.setBounds(FX_AREA_POS_X, FX_AREA_POS_Y, FX_AREA_SIZE_X, FX_AREA_SIZE_Y);
	m_chorus.setBounds(FX_AREA_POS_X, FX_AREA_POS_Y, FX_AREA_SIZE_X, FX_AREA_SIZE_Y);
	m_delay.setBounds(FX_AREA_POS_X, FX_AREA_POS_Y, FX_AREA_SIZE_X, FX_AREA_SIZE_Y);
	m_reverb.setBounds(FX_AREA_POS_X, FX_AREA_POS_Y, FX_AREA_SIZE_X, FX_AREA_SIZE_Y);

	m_mod_matrix.setBounds(MATRIX_POS_X_100, MATRIX_POS_Y_100, MATRIX_SIZE_X, MATRIX_SIZE_Y);
	m_arp.setBounds(ARPEGGIATOR_POS_X, ARPEGGIATOR_POS_Y, MATRIX_SIZE_X, MATRIX_SIZE_Y);
	m_patch_browser.setBounds(BROWSER_POS_X, BROWSER_POS_Y, BROWSER_SIZE_X, BROWSER_SIZE_Y);

	//m_save_load.setBounds(SAVE_LOAD_POS_X, SAVE_LOAD_POS_Y, SAVE_LOAD_SIZE_X, SAVE_LOAD_SIZE_Y);
	m_xy_section.setBounds(XY_COMPONENT_POS_X, XY_COMPONENT_POS_Y, XY_COMPONENT_SIZE_X, XY_COMPONENT_SIZE_Y);

	m_tooltip.setBounds(900, 100, 100, 100);
	m_tooltip.setAlwaysOnTop(true);

	forceValueTreeOntoComponents(false);
	//m_save_load.resetPatchText();

	setSize(ODIN_EDITOR_SIZE_X, ODIN_EDITOR_SIZE_Y);
	m_odin_backdrop = ImageCache::getFromMemory(BinaryData::odin_backdrop_png, BinaryData::odin_backdrop_pngSize);
}