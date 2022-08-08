# 2.3.4
- fixed unitialized editor pointer
# 2.3.3
- added CLAP plugin support
- added native M1 Mac support
- much improved UI appearance when using OS display scaling
- switched build system to CMake
- made WinInstaller suitable for systems not mounted on C:
- fixed symbol name clashes for "Knob" and "Filter"
- fixed issue where renaming a soundbank would select the bank above
# 2.3.2
- fixed issue where multiple instances of reverb would produce noise
# 2.3.1
- debian package: fixed data file being owned by root (introduced in 2.3.0)
- LFO Waves now selectable from a dropdown
- Reversed order of LFO waves to make them align with dropdown
# 2.3.0
- new FX Module: Reverb (ported from zita-rev1)
- added microtuning functionality: Scala and KeyboardMapping files (Credits to Team Surge with their FOSS tuning-library)
- Added MIDI Breath (CC 2) as modulation source
- ModMatrix amounts now range up to [-300, 300]
- default GUI size is now 150%
- Init Patch now has the LP-Filter fully open
- Retrig playmode now restarts the ADSR Attack from the current value, rather than zero
- FM- PM Osc ratio ranges increased to [1,16]
- added indicator triangles to dropdowns to make them more obvious
- factory presets are now contained in the binary
- all paths opened with patchbrowsers are now remembered after reopening plugin and shared among different plugin instances
- fixed Filters not being modulatable beyond +150 in modmatrix
- updated to JUCE 6.0.8
- renamed debian package from odin to odin2-synth to avoid conflicts with repositories
- linux: removed unnessecary dependency to libcurl4
- linux: enabled link-time optimization
- created Azure-CI pipelines for Ubuntu-18.04, MacOs 10.14 and Windows2019 to create automated builds
- updated Manual to 2.3.0
- major code cleanups
# 2.2.4
- fixed Legato carrying current value over to Attack rather than to current section
- ChipDraw tables now available as LFO waves
- XY-Pad automation can now be recorded in DAW
- added background fill to chipdraw and wavedraw editors
- disabled "Made with JUCE" splashscreen
- fixed permissions path in mac installer script when copying file over from old soundbank location
- added new patches to factory sound bank
- fixed help description for Playmode-dropdown
- git commit hash is now displayed in the help-section
# 2.2.3
- upgraded to JUCE 6
- Removed automatic analytics-collection by JUCE
- Removed linux VST2 version
- Added linux VST3 version
- Added linux LV2 version
- New Playmode: Retrig
- Playmodes Legato and Retrig will now play linguring notes again after the current key is released
- Improved opening time for GUI
- added scroll bars to patch-browser
- changed MAC file location
- debian package now correctly packed for amd64 rather than i386
# 2.2.2
- fixed plugin validation in Logic
# 2.2.1
- fixed some verisons only playing sine-waves
# 2.2.0 beta
- new feature: patch-browser
- Odin 2 now ships with installers for all platforms
- Odin 2 now ships with a Factory Soundbank
- ping-pong delay will now alwmays start left, regardless of amp-pan
- added XY-Pad X & Y as modulation destinations
- changed location of config file on Win & Mac
- drastically imrpoved patch loading times
- fixed issue where Filter2 would use the wrong right vowel
- fixed issue where glide time was dependent on samplerate
- fixed issue where comb filter would produce smear effect on first note
- fixed issue which would crash pluginval
- renamed OH-12 filter to SEM-12
- renamed KO-35 filter to KRG-35
# 2.1.0 beta
- implemented Arpeggiator
- new modulation sources: Arp Mod 1 & 2
- new modulation tagets: Arp Speed & Arp Gate
- all sync-time selectors now show a dropdown with most common times on click
- unison selector now shows dropdown rather than being click-draggable
- fixed issue where unison+legato would produce clicks
- changed default value for Delay-Feedback to 0.45
- changed default value for Delay-Wet to 0.7
- implemented MIDI-AllNotesOff
- implemented MIDI-allSoundOff
- modwheel and pitchbend are no longer loaded with patches
- drastically reduced filesize for patches without draw-oscs
# 2.0.22 beta
- fixed issue where wave-/chip-/spec-draw oscs used default waveform before opening the GUI in some hosts
# 2.0.21 beta
- increased number of voices from 12 to 24
- patch-display now has up and down button to browse through patches in same folder
- clicking the patch-display now reveals a list of patches in the same folder
- reworked how volume- and modulation works for osc-vol, filter-gain, amp-gain and master-gain
- updated voicing priority: voices in release are prioritized when stealing notes
- now applying random phase at note start when using unison or multiosc (overwritten by osc-reset)
- changed range for volume of oscs and filters to [-Inf dB, 12dB]
- changed range for main filters to [20Hz,20kHz]
- fixed FM carrier ratio not being loaded
- fixed FM modulator ratio not being loaded
- fixed pitchbend amount not being loaded
- FM carrier ratio, FM modulator ratio and pitchbend amount are no longer modulatable by the host
- fixed issue where FM-osc moddepth was dependent on pitch-controls
- fixed ".odin" file still being saved when canceling patch-saving
- fixed Decay and Release parameters not automatically updating after change in samplerate
- removed 12-voice option from unison
- removed "Poly Aftertouch" from modulation sources
- fixed LFO-sync button always being off on GUI-load
- fixed reversed sawtooth icon in small GUI LFO
- adjusted sync-time-selector font justification
- removed saturation modulation target for OH-12 filter
# 2.0.20 beta
- fixed legato not being enabled before the GUI was opened
# 2.0.19 beta
- implemented Unison
- added "Unison index" modulation source
- removed amp-width parameter
- added amp-velocity parameter
- split "Aftertouch" modsource into "Aftertouch" and "Channel Pressure"
- changed default master-volume to -7dB
- increased oscillator-octave range to [-4, 4]
- fixed specdraw crosshair allignment
# 2.0.18 beta
- made amplifier and distortion polyphonic
- moved amp-envelope application after distortion
- fixed oscillator as polyphonic modulation source
- fixed glide in multi-oscillator
- lowered the minimum volume for osc-volume to -60dB
- lowered the minimum volume for filter gain to -60dB
- lowered the minimum volume for amp gain to -30dB
- lowered the minimum volume for master to -30dB
# 2.0.17 beta
- fixed Global modulation sources crashing plugin on some machines
- dropped requirement for AVX instructions
# 2.0.16 beta
- GUI-size now toggleable between 100% and 150%
- decreased volume of init-patch
# 2.0.15 beta
- fixed a deadlock when assigning modsources in some hosts
- fixed a deadlock when loading synth in some hosts
- adjusted behaviour of ADSR as modulation targets
- removed a lot of linux-dependencies, which caused problems in some hosts
# 2.0.14 beta
- added AudioUnit version
- fixed phaser not working
- modulation amounts are now resetable by ctrl-click
# 2.0.13 beta
- fixed issue where FLStudio would freeze upon loading Odin
- added thewavewarden logo to XY-pad
# 2.0.12 alpha
- fixed issue where osc volume would not show up as mod destination for noise osc
- added better render for knob graphics
# 2.0.11 alpha
- fixed issue where multiple plugin instances would share the same wave-/chip-/specdraw data
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
