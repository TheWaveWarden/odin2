#!/bin/bash

rm Odin2_linux_x64.zip
cp -r ../../Builds/LinuxMakefile/build/Odin2.vst3 Odin2.vst3
cp -r ../../Builds/LinuxMakefile/build/Odin2_.lv2 Odin2.lv2
zip -r Odin2_linux_x64.zip Odin2.vst3 Odin2.lv2 install.sh README.txt
rm -rf Soundbanks
rm Odin2.vst3 -rf
rm Odin2.lv2 -rf
