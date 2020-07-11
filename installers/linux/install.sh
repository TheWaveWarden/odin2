#!/bin/bash

#get location of script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

echo "Create VST directory"
sudo mkdir -p /usr/lib/vst

echo "Copy Odin2 to VST directory"
sudo cp $DIR/Odin2.so /usr/lib/vst

echo "Create Odin2 soundbank and config file folder"
sudo mkdir -p /opt/odin2/Soundbanks

echo "Copy Soundbanks"
sudo cp -r $DIR/Soundbanks/* /opt/odin2/Soundbanks/

echo "Adjust write permissions for soundbank folder"
sudo chmod -R 777 /opt/odin2

echo "Done!"

