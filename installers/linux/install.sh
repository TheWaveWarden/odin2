#!/bin/bash

#get location of script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

echo "Create VST3 directory"
sudo mkdir -p /usr/lib/vst3

echo "Create LV2 directory"
sudo mkdir -p /usr/lib/lv2

echo "Copy Odin2 to VST3 directory"
sudo cp $DIR/Odin2.vst3 /usr/lib/vst3

echo "Copy Odin2 to LV2 directory"
sudo cp $DIR/Odin2.lv2 /usr/lib/lv2

#echo "Remove old Odin2 VST2 version (if it exists)"
#sudo rm /usr/lib/vst/odin2

echo "Create Odin2 soundbank and config file folder"
sudo mkdir -p /opt/odin2/Soundbanks

echo "Copy Soundbanks"
sudo cp -r $DIR/Soundbanks/* /opt/odin2/Soundbanks/

echo "Adjust write permissions for soundbank folder"
sudo chmod -R 777 /opt/odin2

echo "Done!"

