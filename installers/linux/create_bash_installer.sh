#!/bin/bash

# remove old instances
rm Odin2_linux_x64.zip

# copy builds over
cp -r ../../build/Odin2_artefacts/Release/VST3/Odin2.vst3 Odin2.vst3
#cp -r ../../build/Odin2_artefacts/Release/LV2/Odin2.lv2 Odin2.lv2
cp -r ../../build/Odin2_artefacts/Release/CLAP/Odin2.clap Odin2.clap

# create zip
zip -r Odin2_linux_x64.zip Odin2.vst3 Odin2.lv2 install.sh README.txt

# remove intermediate files
rm Odin2.vst3 -rf
rm Odin2.lv2 -rf
rm Odin2.clap -rf
