# 2.0.10 alpha
- fixed VST3 project restore bug
- changed company name in plugin
# 2.0.9 alpha
- fixed load functionality for draw-oscs, comb polarity and formant vowels
- changed backdrop panels for korg and diode filters
- fixed issue with bright formant panel on mac
# 2.0.8 alpha
- fixed load functionality in modmatrix, lfo- & fx sync, distortion panel, lfo wave
- made several parameters non-audio
- fixed .odin not being appended to filenames
- set main-font to runestyle
- changed various panel colors
- changed modmatrix and XY pad colors
# 2.0.7 alpha
- increased modulation depth on phaser-, flanger- and chorus- rate
- fixed bug where flanger would glitch on high sample rates
# 2.0.6 alpha
- major overhaul of save-load system
- backwards compatibility for patches now guaranteed
- removed amp velocity as modulation target
# 2.0.5 alpha
- replaced amp velocity with amp width
- linux now using AVX but no AVX2
# 2.0.4 alpha
- fixed modamount component crashing on win build
- taken measures to avoid denormals
# 2.0.3 alpha
- using optimization option for Win build
- added flag for using AVX in Win build
- using link time optimization in Win build
- fixed missing tooltip for left routing button
- fixed ringmod in filter3 not working
- introduced patchversion 1
# 2.0.2 alpha
- newer patch-versions than current can not be loaded anymore
- changed distortion panel to stripe pattern
# 2.0.1 alpha
- fixed crash in specdraw osc
# 2.0.0 alpha
- added patches folder and first patch
# 1.9.18
- changed vector, multi, distortion panels
- fixed formant filter not working
- code / peformance improvement (removed unnecessary "virtual" classifiers)
# 1.9.17
- Valuetree & patches now with version number
- fixed sync-backrops in LFO
- Korg HP now has distinct panel
# 1.9.16
- performance improvements filters and phaser
# 1.9.15
- big performance improvement for all VA Filters
# 1.9.14
- performance improvement in linterpol
- performance improvement in all filters
# 1.9.13
- major performance improvement for ALL oscs
# 1.9.12
- added modsource dropdown to wavetable oscillator
# 1.9.11
- WavetableOsc now has native modenv control
- performance improvement for oscs and filters
- fixed issue with modamount on big display
# 1.9.10
- major performance upgrade for ALL filters
# 1.9.9
- major performance upgrade for multiosc
# 1.9.8
- all notes of can no longer trigger MIDI learn
- destortion now has two panels for on/off
- chipdraw now interpolating correctly on fast draws
# 1.9.7
- now dynamically deciding which interpolation function to use depending on desktop scale
# 1.9.6
- complete rewrite of the midi learn functionality
- thereby crashes when midi learn is active and GUI is not
# 1.9.5
- show only .odin files in save and load window
- automatically append .odin on file save
# 1.9.4
- now saving last save/load location
- default save location was changed to "Documents" Folder
# 1.9.3
- fixed issue where LFO and FX plates were not loaded correctly on GUI start
- fixed issue where patch label wasn't loaded correctly on GUI start
