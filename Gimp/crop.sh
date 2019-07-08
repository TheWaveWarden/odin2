echo 'cropping LFO syncground'

convert backplatenosync.png -crop 70x40+70+468 cropped/lfo13_sync_background.png
convert backplatenosync.png -crop 70x40+197+468 cropped/lfo24_sync_background.png

echo 'cropping bypass'

#convert -page 800x600+0+0  BypassRaw.png -page +0+0 paths/bypass_path.png  -layers flatten -background none bypass_applied.png

convert BypassRaw.png -crop 247x145+29+29 applied/osc1_bypass.png
convert BypassRaw.png -crop 247x145+277+29 applied/osc2_bypass.png
convert BypassRaw.png -crop 247x145+525+29 applied/osc3_bypass.png

convert BypassApplied.png -crop 247x133+29+176 applied/filter1_bypass.png
convert BypassApplied.png -crop 247x133+525+176 applied/filter2_bypass.png
convert BypassApplied.png -crop 247x133+277+317 applied/filter3_bypass.png



echo 'cropping buttons'

convert glaspanelsraw.png -crop 19x9+249+236 cropped/buttons/buttonup_1.png
convert glaspanelsraw.png -crop 19x9+249+266 cropped/buttons/buttonup_2.png
convert glaspanelsraw.png -crop 19x9+249+245 cropped/buttons/buttondown_1.png
convert glaspanelsraw.png -crop 19x9+249+305 cropped/buttons/buttondown_2.png




convert FilterButtonsRaw.png -crop 17x15+208+129 cropped/buttons/buttonclear_1.png
convert FilterButtonsRaw.png -crop 17x15+208+154 cropped/buttons/buttonclear_2.png
convert FilterButtonsRaw.png -crop 17x17+208+129 cropped/buttons/buttonclearlast_1.png
convert FilterButtonsRaw.png -crop 17x17+208+154 cropped/buttons/buttonclearlast_2.png


convert OdinKnobRaw.png -crop 24x11+63+374 cropped/buttons/slider_handle.png




convert OscKnobRaw.png -crop 61x19+82+58 cropped/buttons/LED_1.png
convert OscKnobRaw.png -crop 61x19+82+115 cropped/buttons/LED_2.png


convert FilterButtonsRaw.png -crop 24x22+338+256 cropped/buttons/button1_1.png
convert FilterButtonsRaw.png -crop 24x22+368+256 cropped/buttons/button1_2.png
convert FilterButtonsRaw.png -crop 24x22+398+256 cropped/buttons/button1_3.png
convert FilterButtonsRaw.png -crop 24x22+428+256 cropped/buttons/button1_4.png

convert -append cropped/buttons/button1_1.png  cropped/buttons/button1_2.png  cropped/buttons/button1_3.png  cropped/buttons/button1_4.png  cropped/buttons/button1filmstrip.png 

convert FilterButtonsRaw.png -crop 24x22+338+278 cropped/buttons/button2_1.png
convert FilterButtonsRaw.png -crop 24x22+368+278 cropped/buttons/button2_2.png
convert FilterButtonsRaw.png -crop 24x22+398+278 cropped/buttons/button2_3.png
convert FilterButtonsRaw.png -crop 24x22+428+278 cropped/buttons/button2_4.png

convert -append cropped/buttons/button2_1.png  cropped/buttons/button2_2.png  cropped/buttons/button2_3.png  cropped/buttons/button2_4.png  cropped/buttons/button2filmstrip.png 

convert FilterButtonsRaw.png -crop 24x22+338+300 cropped/buttons/button3_1.png
convert FilterButtonsRaw.png -crop 24x22+368+300 cropped/buttons/button3_2.png
convert FilterButtonsRaw.png -crop 24x22+398+300 cropped/buttons/button3_3.png
convert FilterButtonsRaw.png -crop 24x22+428+300 cropped/buttons/button3_4.png

convert -append cropped/buttons/button3_1.png  cropped/buttons/button3_2.png  cropped/buttons/button3_3.png  cropped/buttons/button3_4.png  cropped/buttons/button3filmstrip.png 

convert FilterButtonsRaw.png -crop 24x22+338+322 cropped/buttons/buttonf1_1.png
convert FilterButtonsRaw.png -crop 24x22+368+322 cropped/buttons/buttonf1_2.png
convert FilterButtonsRaw.png -crop 24x22+398+322 cropped/buttons/buttonf1_3.png
convert FilterButtonsRaw.png -crop 24x22+428+322 cropped/buttons/buttonf1_4.png

convert -append cropped/buttons/buttonf1_1.png  cropped/buttons/buttonf1_2.png  cropped/buttons/buttonf1_3.png  cropped/buttons/buttonf1_4.png  cropped/buttons/buttonf1filmstrip.png 



convert FilterButtonsRaw.png -crop 24x24+338+349 cropped/buttons/buttonright_1.png
convert FilterButtonsRaw.png -crop 24x24+368+349 cropped/buttons/buttonright_2.png
convert FilterButtonsRaw.png -crop 24x24+398+349 cropped/buttons/buttonright_3.png
convert FilterButtonsRaw.png -crop 24x24+428+349 cropped/buttons/buttonright_4.png

convert -append cropped/buttons/buttonright_1.png  cropped/buttons/buttonright_2.png  cropped/buttons/buttonright_3.png  cropped/buttons/buttonright_4.png  cropped/buttons/buttonrightfilmstrip.png 

convert FilterButtonsRaw.png -crop 24x24+338+374 cropped/buttons/buttonleft_1.png
convert FilterButtonsRaw.png -crop 24x24+368+374 cropped/buttons/buttonleft_2.png
convert FilterButtonsRaw.png -crop 24x24+398+374 cropped/buttons/buttonleft_3.png
convert FilterButtonsRaw.png -crop 24x24+428+374 cropped/buttons/buttonleft_4.png

convert -append cropped/buttons/buttonleft_1.png  cropped/buttons/buttonleft_2.png  cropped/buttons/buttonleft_3.png  cropped/buttons/buttonleft_4.png  cropped/buttons/buttonleftfilmstrip.png 

convert FilterButtonsRaw.png -crop 114x19+211+256 cropped/buttons/buttonlfo13_1.png
convert FilterButtonsRaw.png -crop 114x19+211+276 cropped/buttons/buttonlfo13_2.png
convert FilterButtonsRaw.png -crop 114x19+211+296 cropped/buttons/buttonlfo13_3.png
convert FilterButtonsRaw.png -crop 114x19+211+316 cropped/buttons/buttonlfo13_4.png

convert -append cropped/buttons/buttonlfo13_1.png  cropped/buttons/buttonlfo13_2.png  cropped/buttons/buttonlfo13_3.png  cropped/buttons/buttonlfo13_4.png  cropped/buttons/buttonlfo13filmstrip.png 

convert FilterButtonsRaw.png -crop 114x19+211+336 cropped/buttons/buttonlfo24_1.png
convert FilterButtonsRaw.png -crop 114x19+211+356 cropped/buttons/buttonlfo24_2.png
convert FilterButtonsRaw.png -crop 114x19+211+376 cropped/buttons/buttonlfo24_3.png
convert FilterButtonsRaw.png -crop 114x19+211+396 cropped/buttons/buttonlfo24_4.png

convert -append cropped/buttons/buttonlfo24_1.png  cropped/buttons/buttonlfo24_2.png  cropped/buttons/buttonlfo24_3.png  cropped/buttons/buttonlfo24_4.png  cropped/buttons/buttonlfo24filmstrip.png 

convert FilterButtonsRaw.png -crop 114x19+459+256 cropped/buttons/buttonenv24_1.png
convert FilterButtonsRaw.png -crop 114x19+459+276 cropped/buttons/buttonenv24_2.png
convert FilterButtonsRaw.png -crop 114x19+459+296 cropped/buttons/buttonenv24_3.png
convert FilterButtonsRaw.png -crop 114x19+459+316 cropped/buttons/buttonenv24_4.png

convert -append cropped/buttons/buttonenv13_1.png  cropped/buttons/buttonenv24_2.png  cropped/buttons/buttonenv13_3.png  cropped/buttons/buttonenv13_4.png  cropped/buttons/buttonenv13filmstrip.png 

convert FilterButtonsRaw.png -crop 114x19+459+336 cropped/buttons/buttonenv13_1.png
convert FilterButtonsRaw.png -crop 114x19+459+356 cropped/buttons/buttonenv13_2.png
convert FilterButtonsRaw.png -crop 114x19+459+376 cropped/buttons/buttonenv13_3.png
convert FilterButtonsRaw.png -crop 114x19+459+396 cropped/buttons/buttonenv13_4.png

convert -append cropped/buttons/buttonenv24_1.png  cropped/buttons/buttonenv13_2.png  cropped/buttons/buttonenv24_3.png  cropped/buttons/buttonenv24_4.png  cropped/buttons/buttonenv24filmstrip.png 

convert FilterButtonsRaw.png -crop 63x19+273+233 cropped/buttons/buttondistortion_1.png
convert FilterButtonsRaw.png -crop 63x19+338+233 cropped/buttons/buttondistortion_2.png
convert FilterButtonsRaw.png -crop 63x19+403+233 cropped/buttons/buttondistortion_3.png
convert FilterButtonsRaw.png -crop 63x19+468+233 cropped/buttons/buttondistortion_4.png

convert -append cropped/buttons/buttondistortion_1.png  cropped/buttons/buttondistortion_2.png  cropped/buttons/buttondistortion_3.png  cropped/buttons/buttondistortion_4.png  cropped/buttons/buttondistortionfilmstrip.png 

convert FilterButtonsRaw.png -crop 62x19+305+420 cropped/buttons/buttonflanger_1.png
convert FilterButtonsRaw.png -crop 62x19+305+445 cropped/buttons/buttonflanger_2.png
convert FilterButtonsRaw.png -crop 62x19+305+470 cropped/buttons/buttonflanger_3.png
convert FilterButtonsRaw.png -crop 62x19+305+495 cropped/buttons/buttonflanger_4.png

convert -append cropped/buttons/buttonflanger_1.png  cropped/buttons/buttonflanger_2.png  cropped/buttons/buttonflanger_3.png  cropped/buttons/buttonflanger_4.png  cropped/buttons/buttonflangerfilmstrip.png 

convert FilterButtonsRaw.png -crop 62x19+367+420 cropped/buttons/buttondelay_1.png
convert FilterButtonsRaw.png -crop 62x19+367+445 cropped/buttons/buttondelay_2.png
convert FilterButtonsRaw.png -crop 62x19+367+470 cropped/buttons/buttondelay_3.png
convert FilterButtonsRaw.png -crop 62x19+367+495 cropped/buttons/buttondelay_4.png

convert -append cropped/buttons/buttondelay_1.png  cropped/buttons/buttondelay_2.png  cropped/buttons/buttondelay_3.png  cropped/buttons/buttondelay_4.png  cropped/buttons/buttondelayfilmstrip.png 

convert FilterButtonsRaw.png -crop 62x19+429+420 cropped/buttons/buttonchorus_1.png
convert FilterButtonsRaw.png -crop 62x19+429+445 cropped/buttons/buttonchorus_2.png
convert FilterButtonsRaw.png -crop 62x19+429+470 cropped/buttons/buttonchorus_3.png
convert FilterButtonsRaw.png -crop 62x19+429+495 cropped/buttons/buttonchorus_4.png

convert -append cropped/buttons/buttonchorus_1.png  cropped/buttons/buttonchorus_2.png  cropped/buttons/buttonchorus_3.png  cropped/buttons/buttonchorus_4.png  cropped/buttons/buttonchorusfilmstrip.png 

convert FilterButtonsRaw.png -crop 62x19+491+420 cropped/buttons/buttonphaser_1.png
convert FilterButtonsRaw.png -crop 62x19+491+445 cropped/buttons/buttonphaser_2.png
convert FilterButtonsRaw.png -crop 62x19+491+470 cropped/buttons/buttonphaser_3.png
convert FilterButtonsRaw.png -crop 62x19+491+495 cropped/buttons/buttonphaser_4.png

convert -append cropped/buttons/buttonphaser_1.png  cropped/buttons/buttonphaser_2.png  cropped/buttons/buttonphaser_3.png  cropped/buttons/buttonphaser_4.png  cropped/buttons/buttonphaserfilmstrip.png 

convert FilterButtonsRaw.png -crop 60x12+557+420 cropped/buttons/buttonfxon_1.png
convert FilterButtonsRaw.png -crop 60x12+557+445 cropped/buttons/buttonfxon_2.png
convert FilterButtonsRaw.png -crop 60x12+557+470 cropped/buttons/buttonfxon_3.png
convert FilterButtonsRaw.png -crop 60x12+557+495 cropped/buttons/buttonfxon_4.png

convert -append cropped/buttons/buttonfxon_1.png  cropped/buttons/buttonfxon_2.png  cropped/buttons/buttonfxon_3.png  cropped/buttons/buttonfxon_4.png  cropped/buttons/buttonfxonfilmstrip.png 

convert FilterButtonsRaw.png -crop 67x19+590+252 cropped/buttons/buttonarpeggiator_1.png
convert FilterButtonsRaw.png -crop 67x19+590+277 cropped/buttons/buttonarpeggiator_2.png
convert FilterButtonsRaw.png -crop 67x19+590+302 cropped/buttons/buttonarpeggiator_3.png
convert FilterButtonsRaw.png -crop 67x19+590+327 cropped/buttons/buttonarpeggiator_4.png

convert -append cropped/buttons/buttonarpeggiator_1.png  cropped/buttons/buttonarpeggiator_2.png  cropped/buttons/buttonarpeggiator_3.png  cropped/buttons/buttonarpeggiator_4.png  cropped/buttons/buttonarpeggiatorfilmstrip.png 


convert FilterButtonsRaw.png -crop 42x19+662+252 cropped/buttons/buttonstep_1.png
convert FilterButtonsRaw.png -crop 42x19+662+277 cropped/buttons/buttonstep_2.png
convert FilterButtonsRaw.png -crop 42x19+662+302 cropped/buttons/buttonstep_3.png
convert FilterButtonsRaw.png -crop 42x19+662+327 cropped/buttons/buttonstep_4.png

convert -append cropped/buttons/buttonstep_1.png  cropped/buttons/buttonstep_2.png  cropped/buttons/buttonstep_3.png  cropped/buttons/buttonstep_4.png  cropped/buttons/buttonstepfilmstrip.png 


convert FilterButtonsRaw.png -crop 36x19+712+252 cropped/buttons/buttonnoise_1.png
convert FilterButtonsRaw.png -crop 36x19+712+277 cropped/buttons/buttonnoise_2.png
convert FilterButtonsRaw.png -crop 36x19+712+302 cropped/buttons/buttonnoise_3.png
convert FilterButtonsRaw.png -crop 36x19+712+327 cropped/buttons/buttonnoise_4.png

convert -append cropped/buttons/buttonnoise_1.png  cropped/buttons/buttonnoise_2.png  cropped/buttons/buttonnoise_3.png  cropped/buttons/buttonnoise_4.png  cropped/buttons/buttonnoisefilmstrip.png 


convert FilterButtonsRaw.png -crop 33x18+353+179 cropped/buttons/buttonsave_1.png
convert FilterButtonsRaw.png -crop 33x18+353+209 cropped/buttons/buttonsave_2.png

convert -append cropped/buttons/buttonsave_1.png  cropped/buttons/buttonsave_2.png cropped/buttons/buttonsavefilmstrip.png 

convert FilterButtonsRaw.png -crop 33x18+403+179 cropped/buttons/buttonload_1.png
convert FilterButtonsRaw.png -crop 33x18+403+209 cropped/buttons/buttonload_2.png

convert -append cropped/buttons/buttonload_1.png  cropped/buttons/buttonload_2.png cropped/buttons/buttonloadfilmstrip.png 

convert FilterButtonsRaw.png -crop 46x18+444+179 cropped/buttons/buttonreset_global_1.png
convert FilterButtonsRaw.png -crop 46x18+444+209 cropped/buttons/buttonreset_global_2.png

convert -append cropped/buttons/buttonreset_global_1.png  cropped/buttons/buttonreset_global_2.png cropped/buttons/buttonreset_globalfilmstrip.png 

convert FilterButtonsRaw.png -crop 46x18+494+179 cropped/buttons/buttonrandom_1.png
convert FilterButtonsRaw.png -crop 46x18+494+209 cropped/buttons/buttonrandom_2.png

convert -append cropped/buttons/buttonrandom_1.png  cropped/buttons/buttonrandom_2.png cropped/buttons/buttonrandomfilmstrip.png 

convert FilterButtonsRaw.png -crop 24x23+234+126 cropped/buttons/buttonapply_1.png
convert FilterButtonsRaw.png -crop 24x23+234+151 cropped/buttons/buttonapply_2.png
convert FilterButtonsRaw.png -crop 24x23+234+176 cropped/buttons/buttonapply_3.png
convert FilterButtonsRaw.png -crop 24x23+234+201 cropped/buttons/buttonapply_4.png

convert FilterButtonsRaw.png -crop 24x23+293+126 cropped/buttons/buttonpleb_1.png
convert FilterButtonsRaw.png -crop 24x23+293+151 cropped/buttons/buttonpleb_2.png
convert FilterButtonsRaw.png -crop 24x23+293+176 cropped/buttons/buttonpleb_3.png
convert FilterButtonsRaw.png -crop 24x23+293+201 cropped/buttons/buttonpleb_4.png

convert -append cropped/buttons/buttonapply_1.png  cropped/buttons/buttonapply_2.png cropped/buttons/buttonapply_3.png  cropped/buttons/buttonapply_4.png cropped/buttons/buttonapplyfilmstrip.png 

convert FilterButtonsRaw.png -crop 26x19+261+126 cropped/buttons/buttonreset_osc_1.png
convert FilterButtonsRaw.png -crop 26x19+261+151 cropped/buttons/buttonreset_osc_2.png
convert FilterButtonsRaw.png -crop 26x19+261+176 cropped/buttons/buttonreset_osc_3.png
convert FilterButtonsRaw.png -crop 26x19+261+201 cropped/buttons/buttonreset_osc_4.png

convert -append cropped/buttons/buttonreset_osc_1.png  cropped/buttons/buttonreset_osc_2.png cropped/buttons/buttonreset_osc_3.png  cropped/buttons/buttonreset_osc_4.png cropped/buttons/buttonreset_oscfilmstrip.png 

convert FilterButtonsRaw.png -crop 23x14+167+148 cropped/buttons/buttondropdown_1.png
convert FilterButtonsRaw.png -crop 23x14+167+173 cropped/buttons/buttondropdown_2.png

convert -append cropped/buttons/buttondropdown_1.png  cropped/buttons/buttondropdown_2.png cropped/buttons/buttondropdownfilmstrip.png 

convert FilterButtonsRaw.png -crop 49x19+102+256 cropped/buttons/buttonexplin_1.png
convert FilterButtonsRaw.png -crop 49x19+102+281 cropped/buttons/buttonexplin_2.png
convert FilterButtonsRaw.png -crop 49x19+102+306 cropped/buttons/buttonexplin_3.png
convert FilterButtonsRaw.png -crop 49x19+102+331 cropped/buttons/buttonexplin_4.png

convert -append cropped/buttons/buttonexplin_1.png  cropped/buttons/buttonexplin_2.png cropped/buttons/buttonexplin_3.png  cropped/buttons/buttonexplin_4.png cropped/buttons/buttonexplinfilmstrip.png 

convert FilterButtonsRaw.png -crop 71x19+7+256 cropped/buttons/buttonlegato_1.png
convert FilterButtonsRaw.png -crop 71x19+7+281 cropped/buttons/buttonlegato_2.png
convert FilterButtonsRaw.png -crop 71x19+7+306 cropped/buttons/buttonlegato_3.png
convert FilterButtonsRaw.png -crop 71x19+7+331 cropped/buttons/buttonlegato_4.png

convert -append cropped/buttons/buttonlegato_1.png  cropped/buttons/buttonlegato_2.png cropped/buttons/buttonlegato_3.png  cropped/buttons/buttonlegato_4.png cropped/buttons/buttonlegatofilmstrip.png 

convert FilterButtonsRaw.png -crop 44x19+157+256 cropped/buttons/buttonplusminus_1.png
convert FilterButtonsRaw.png -crop 44x19+157+281 cropped/buttons/buttonplusminus_2.png
convert FilterButtonsRaw.png -crop 44x19+157+306 cropped/buttons/buttonplusminus_3.png
convert FilterButtonsRaw.png -crop 44x19+157+331 cropped/buttons/buttonplusminus_4.png

convert -append cropped/buttons/buttonplusminus_1.png  cropped/buttons/buttonplusminus_2.png cropped/buttons/buttonplusminus_3.png  cropped/buttons/buttonplusminus_4.png cropped/buttons/buttonplusminusfilmstrip.png 

convert FilterButtonsRaw.png -crop 46x18+113+371 cropped/buttons/buttonreset_lfo_1.png
convert FilterButtonsRaw.png -crop 46x18+113+396 cropped/buttons/buttonreset_lfo_2.png
convert FilterButtonsRaw.png -crop 46x18+113+421 cropped/buttons/buttonreset_lfo_3.png
convert FilterButtonsRaw.png -crop 46x18+113+446 cropped/buttons/buttonreset_lfo_4.png

convert -append cropped/buttons/buttonreset_lfo_1.png  cropped/buttons/buttonreset_lfo_2.png cropped/buttons/buttonreset_lfo_3.png  cropped/buttons/buttonreset_lfo_4.png cropped/buttons/buttonreset_lfofilmstrip.png 

convert FilterButtonsRaw.png -crop 33x18+223+419 cropped/buttons/buttonsync_1.png
convert FilterButtonsRaw.png -crop 33x18+223+444 cropped/buttons/buttonsync_2.png
convert FilterButtonsRaw.png -crop 33x18+223+469 cropped/buttons/buttonsync_3.png
convert FilterButtonsRaw.png -crop 33x18+223+494 cropped/buttons/buttonsync_4.png

convert -append cropped/buttons/buttonsync_1.png  cropped/buttons/buttonsync_2.png cropped/buttons/buttonsync_3.png  cropped/buttons/buttonsync_4.png cropped/buttons/buttonsyncfilmstrip.png 

convert FilterButtonsRaw.png -crop 37x12+260+421 cropped/buttons/buttonloop_1.png
convert FilterButtonsRaw.png -crop 37x12+260+446 cropped/buttons/buttonloop_2.png
convert FilterButtonsRaw.png -crop 37x12+260+471 cropped/buttons/buttonloop_3.png
convert FilterButtonsRaw.png -crop 37x12+260+496 cropped/buttons/buttonloop_4.png

convert -append cropped/buttons/buttonloop_1.png  cropped/buttons/buttonloop_2.png cropped/buttons/buttonloop_3.png  cropped/buttons/buttonloop_4.png cropped/buttons/buttonloopfilmstrip.png 


#rm cropped/buttons/buttonf1_*
#rm cropped/buttons/button1_*
#rm cropped/buttons/button2_*
#rm cropped/buttons/button3_*
#rm cropped/buttons/buttonright_*
#rm cropped/buttons/buttonleft_*
#rm cropped/buttons/buttonlfo13_*
#rm cropped/buttons/buttonlfo24_*
#rm cropped/buttons/buttonenv13_*
#rm cropped/buttons/buttonenv24_*
#rm cropped/buttons/buttondistortion_*
#rm cropped/buttons/buttonloop_*
#rm cropped/buttons/buttonsync_*
#rm cropped/buttons/buttonreset_lfo*
#rm cropped/buttons/buttonexplin_*
#rm cropped/buttons/buttonplusminus_*
#rm cropped/buttons/buttonapply_*
#rm cropped/buttons/buttonreset_osc_*
#rm cropped/buttons/buttonsave_*
#rm cropped/buttons/buttonload_*
#rm cropped/buttons/buttonreset_global_*
#rm cropped/buttons/buttonrandom_*
#rm cropped/buttons/buttonarpeggiator_*
#rm cropped/buttons/buttonstep_*
#rm cropped/buttons/buttonnoise_*
#rm cropped/buttons/buttonfxon_*
#rm cropped/buttons/buttonchorus_*
#rm cropped/buttons/buttonphaser_*
#rm cropped/buttons/buttondelay_*
#rm cropped/buttons/buttonflanger_*
#rm cropped/buttons/buttondropdown_*


echo 'cropping FX panels'
convert delaysyncraw.png -crop 243x107+527+343 cropped/delaysync.png
convert delaynosyncraw.png -crop 243x107+527+343 cropped/delaynosync.png
convert phasernosyncraw.png -crop 243x107+527+343 cropped/phasernosync.png
convert flangernosyncraw.png -crop 243x107+527+343 cropped/flangernosync.png
convert chorusnosyncraw.png -crop 243x107+527+343 cropped/chorusnosync.png
convert phasersyncraw.png -crop 243x107+527+343 cropped/phasersync.png
convert flangersyncraw.png -crop 243x107+527+343 cropped/flangersync.png
convert chorussyncraw.png -crop 243x107+527+343 cropped/chorussync.png



echo 'cropping oscillator panels'

convert OscRaw.png -crop 247x145+29+1 cropped/analog_cropped.png
convert OscRaw.png -crop 247x145+277+1 cropped/chiptune_cropped.png
convert OscRaw.png -crop 247x145+525+1 cropped/noise_cropped.png
convert OscRaw.png -crop 247x145+29+151 cropped/wavetable_cropped.png
convert OscRaw.png -crop 247x145+277+151 cropped/vector_cropped.png
convert OscRaw.png -crop 247x145+525+151 cropped/fm_cropped.png
convert OscRaw.png -crop 247x145+525+451 cropped/pm_cropped.png
convert OscRaw.png -crop 247x145+29+301 cropped/specdraw_cropped.png
convert OscRaw.png -crop 247x145+277+301 cropped/wavedraw_cropped.png
convert OscRaw.png -crop 247x145+525+301 cropped/chipdraw_cropped.png
convert OscRaw.png -crop 247x145+277+451 cropped/multi_cropped.png

echo 'cropping oscillator knobs'

convert OscKnobRaw.png -crop 247x145+29+1 cropped/analog_knob_cropped.png
convert OscKnobRaw.png -crop 247x145+277+1 cropped/chiptune_knob_cropped.png
convert OscKnobRaw.png -crop 247x145+525+1 cropped/noise_knob_cropped.png
convert OscKnobRaw.png -crop 247x145+29+151 cropped/wavetable_knob_cropped.png
convert OscKnobRaw.png -crop 247x145+277+151 cropped/vector_knob_cropped.png
convert OscKnobRaw.png -crop 247x145+525+151 cropped/fm_knob_cropped.png
convert OscKnobRaw.png -crop 247x145+29+301 cropped/specdraw_knob_cropped.png
convert OscKnobRaw.png -crop 247x145+277+301 cropped/wavedraw_knob_cropped.png
convert OscKnobRaw.png -crop 247x145+525+301 cropped/chipdraw_knob_cropped.png
convert OscKnobRaw.png -crop 247x145+277+451 cropped/multi_knob_cropped.png

echo 'cropping filter panels'

convert FiltersRaw.png -crop 247x134+29+33 cropped/LP12_cropped.png
convert FiltersRaw.png -crop 247x134+277+33 cropped/BP12_cropped.png
convert FiltersRaw.png -crop 247x134+525+33 cropped/HP12_cropped.png
convert FiltersRaw.png -crop 247x134+29+175 cropped/LP24_cropped.png
convert FiltersRaw.png -crop 247x134+277+175 cropped/BP24_cropped.png
convert FiltersRaw.png -crop 247x134+525+175 cropped/HP24_cropped.png
convert FiltersRaw.png -crop 247x134+29+317 cropped/formant_cropped.png
convert FiltersRaw.png -crop 247x134+277+317 cropped/comb_cropped.png
convert FiltersRaw.png -crop 247x134+525+317 cropped/SEM12_cropped.png
convert FiltersRaw.png -crop 247x134+29+459 cropped/diode_cropped.png
convert FiltersRaw.png -crop 247x134+277+459 cropped/korg_cropped.png
convert FiltersRaw.png -crop 247x134+525+459 cropped/SEM24_cropped.png
convert OscRaw.png -crop 247x134+29+459 cropped/ringmod_cropped.png


echo 'cropping filter knobs'

convert FilterKnobsRaw.png -crop 247x134+29+33 cropped/LP12_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+277+33 cropped/BP12_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+525+33 cropped/HP12_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+29+175 cropped/LP24_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+277+175 cropped/BP24_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+525+175 cropped/HP24_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+29+317 cropped/formant_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+277+317 cropped/comb_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+525+317 cropped/SEM12_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+29+459 cropped/diode_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+277+459 cropped/korg_knobs_cropped.png
convert FilterKnobsRaw.png -crop 247x134+525+459 cropped/SEM24_knobs_cropped.png

echo 'cropping glass panels'

convert glaspanelsraw.png -crop 88x15+140+177 cropped/modpanelbig_left_up.png
convert glaspanelsraw.png -crop 88x15+271+177 cropped/modpanelbig_mid_up.png
convert glaspanelsraw.png -crop 88x15+402+177 cropped/modpanelbig_right_up.png
convert glaspanelsraw.png -crop 88x15+140+192 cropped/modpanelbig_left_mid.png
convert glaspanelsraw.png -crop 88x15+271+192 cropped/modpanelbig_mid_mid.png
convert glaspanelsraw.png -crop 88x15+402+192 cropped/modpanelbig_right_mid.png
convert glaspanelsraw.png -crop 88x16+140+207 cropped/modpanelbig_left_down.png
convert glaspanelsraw.png -crop 88x16+271+207 cropped/modpanelbig_mid_down.png
convert glaspanelsraw.png -crop 88x16+402+207 cropped/modpanelbig_right_down.png

convert glaspanelsraw.png -crop 43x15+228+177 cropped/modpanelsmall_up.png
convert glaspanelsraw.png -crop 43x15+228+192 cropped/modpanelsmall_mid.png
convert glaspanelsraw.png -crop 43x16+228+207 cropped/modpanelsmall_down.png

convert glaspanelsraw.png -crop 99x85+197+82 cropped/vectorpanel.png
convert glaspanelsraw.png -crop 205x104+273+234 cropped/drawpanel.png

convert glaspanelsraw.png -crop 92x20+151+235 cropped/glaspanel_big.png
convert glaspanelsraw.png -crop 65x18+177+326 cropped/glaspanel_midbig.png
convert glaspanelsraw.png -crop 42x20+201+265 cropped/glaspanel_mid.png
convert glaspanelsraw.png -crop 28x20+215+295 cropped/glaspanel_small.png

convert glaspanelsraw.png -crop 19x18+249+235 cropped/buttons/selectbutton0.png
convert glaspanelsraw.png -crop 19x18+249+265 cropped/buttons/selectbutton1.png
convert glaspanelsraw.png -crop 19x18+249+295 cropped/buttons/selectbutton2.png

convert -append cropped/buttons/selectbutton0.png cropped/buttons/selectbutton1.png cropped/buttons/selectbutton2.png cropped/buttons/select_filmstrip.png 
rm cropped/buttons/selectbutt*


echo 'cropping knob frames'

#zero padded loop 1 to 256
for i in $(seq -f "%04g" 1 256)
do
    echo $i
    convert knobs/raw/frame$i.png -crop 33x35+32+110 cropped/knobs/metal1/frame$i.png
    convert knobs/raw/frame$i.png -crop 39x42+129+58 cropped/knobs/metal2/frame$i.png
    convert knobs/raw/frame$i.png -crop 45x48+75+55 cropped/knobs/metal3/frame$i.png
    convert knobs/raw/frame$i.png -crop 21x24+89+114 cropped/knobs/black1/frame$i.png
    convert knobs/raw/frame$i.png -crop 27x30+36+62 cropped/knobs/black2/frame$i.png
    convert knobs/raw/frame$i.png -crop 29x32+85+11 cropped/knobs/black3/frame$i.png
    convert knobs/raw/frame$i.png -crop 32x35+33+10 cropped/knobs/black4/frame$i.png
    convert knobs/raw/frame$i.png -crop 36x39+131+8 cropped/knobs/round/frame$i.png
    convert knobs/raw/frame$i.png -crop 17x78+180+36 cropped/knobs/modwheel/frame$i.png
done  

echo 'make film strips'

convert -append cropped/knobs/metal1/frame0001.png cropped/knobs/metal1/frame0002.png cropped/knobs/metal1/metal_knob_small.png 
convert -append cropped/knobs/metal2/frame0001.png cropped/knobs/metal2/frame0002.png cropped/knobs/metal2/metal_knob_mid.png 
convert -append cropped/knobs/metal3/frame0001.png cropped/knobs/metal3/frame0002.png cropped/knobs/metal3/metal_knob_big.png 
convert -append cropped/knobs/black1/frame0001.png cropped/knobs/black1/frame0002.png cropped/knobs/black1/black_knob_very_small.png 
convert -append cropped/knobs/black2/frame0001.png cropped/knobs/black2/frame0002.png cropped/knobs/black2/black_knob_small.png 
convert -append cropped/knobs/black3/frame0001.png cropped/knobs/black3/frame0002.png cropped/knobs/black3/black_knob_mid.png 
convert -append cropped/knobs/black4/frame0001.png cropped/knobs/black4/frame0002.png cropped/knobs/black4/black_knob_big.png 
convert -append cropped/knobs/round/frame0001.png cropped/knobs/round/frame0002.png cropped/knobs/round/round_knob.png 
convert +append cropped/knobs/modwheel/frame0001.png cropped/knobs/modwheel/frame0002.png cropped/knobs/modwheel/modwheel.png 


for i in $(seq -f "%04g" 3 256)
do
     echo $i    
     convert -append cropped/knobs/metal1/metal_knob_small.png cropped/knobs/metal1/frame$i.png cropped/knobs/metal1/metal_knob_small.png 
     convert -append cropped/knobs/metal2/metal_knob_mid.png cropped/knobs/metal2/frame$i.png cropped/knobs/metal2/metal_knob_mid.png 
     convert -append cropped/knobs/metal3/metal_knob_big.png cropped/knobs/metal3/frame$i.png cropped/knobs/metal3/metal_knob_big.png
     convert -append cropped/knobs/black1/black_knob_very_small.png cropped/knobs/black1/frame$i.png cropped/knobs/black1/black_knob_very_small.png 
     convert -append cropped/knobs/black2/black_knob_small.png cropped/knobs/black2/frame$i.png cropped/knobs/black2/black_knob_small.png 
     convert -append cropped/knobs/black3/black_knob_mid.png cropped/knobs/black3/frame$i.png cropped/knobs/black3/black_knob_mid.png 
     convert -append cropped/knobs/black4/black_knob_big.png cropped/knobs/black4/frame$i.png cropped/knobs/black4/black_knob_big.png 
     convert -append cropped/knobs/round/round_knob.png cropped/knobs/round/frame$i.png cropped/knobs/round/round_knob.png 
     convert +append cropped/knobs/modwheel/modwheel.png cropped/knobs/modwheel/frame$i.png cropped/knobs/modwheel/modwheel.png 
#    convert knobs/raw/frame_$i.png -crop 33x35+32+110 cropped/knobs/metal1/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 39x42+129+58 cropped/knobs/metal2/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 45x48+75+55 cropped/knobs/metal3/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 21x24+89+114 cropped/knobs/black1/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 27x30+36+62 cropped/knobs/black2/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 29x32+85+11 cropped/knobs/black3/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 32x35+33+10 cropped/knobs/black4/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 36x39+131+8 cropped/knobs/round/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 23x94+175+28 cropped/knobs/modwheel/frame_$i.png
done  


rm cropped/knobs/metal1/frame*
rm cropped/knobs/metal2/frame*
rm cropped/knobs/metal3/frame*
rm cropped/knobs/black1/frame*
rm cropped/knobs/black2/frame*
rm cropped/knobs/black3/frame*
rm cropped/knobs/black4/frame*
rm cropped/knobs/round/frame*
rm cropped/knobs/modwheel/frame*


