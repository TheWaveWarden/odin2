#!/bin/bash

#get location of script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

echo "Create VST3 directory"
sudo mkdir -p /usr/lib/vst3

echo "Create LV2 directory"
sudo mkdir -p /usr/lib/lv2

echo "Create CLAP directory"
sudo mkdir -p /usr/lib/clap

echo "Copy Odin2 to VST3 directory"
sudo cp -r $DIR/Odin2.vst3 /usr/lib/vst3

echo "Copy Odin2 to LV2 directory"
sudo cp -r $DIR/Odin2.lv2 /usr/lib/lv2

echo "Copy Odin2 to LV2 directory"
sudo cp -r $DIR/Odin2.clap /usr/lib/clap

if [ ! -d "$HOME/.local/share/odin2/Soundbanks" ]; then
    echo "Migrate Soundbanks from old location:"
    mkdir -p "$HOME/.local/share/odin2/Soundbanks/User Patches"
    cp -r /opt/odin2/Soundbanks/* $HOME/.local/share/odin2/Soundbanks/
    sudo chown -R $USER:$USER $HOME/.local/share/odin2
fi

echo "Done!"

