#!/bin/bash

rm Odin2_linux_x64.zip
cp -r ../../Builds/LinuxMakefile/build/Odin2.vst3 Odin2.vst3
cp -r ../../Builds/LinuxMakefile/build/Odin2_.lv2 Odin2.lv2
mkdir Soundbanks
cp -r ../../Soundbanks/* Soundbanks/
rm Soundbanks/User\ Patches/.gitignore 
zip -r Odin2_linux_x64.zip Odin2.vst3 Odin2.lv2 Soundbanks install.sh README.txt
rm -rf Soundbanks
rm Odin2.vst3 -rf
rm Odin2.lv2 -rf
