#!/bin/bash


#package version can be passed as first argument for CI, if not it must be input
if [ "$#" -eq 1 ]; then
    package_version="$1"
else 
    read -p "Enter Odin Package Version (i.e. 2.2-0): "  package_version
fi

echo "package version set to $package_version"

#get name for package

package_name="Odin_${package_version}"

echo "Creating package ${package_name}"

#recreate directory
sudo rm -rf $package_name
mkdir $package_name

#build tree for vst3 && lv2 plugin
mkdir $package_name/usr/
mkdir $package_name/usr/lib
mkdir $package_name/usr/lib/vst3
mkdir $package_name/usr/lib/lv2

#build tree for preset library
mkdir $package_name/opt
mkdir $package_name/opt/odin2
mkdir $package_name/opt/odin2/Soundbanks

#build Odin2 from scratch
pushd ../../Builds/LinuxMakefile/
../../../JUCE/Projucer --resave ../../Odin.jucer
#add command to build LV2 as well
echo -e "include ../../LV2.mak" >> Makefile
make clean
make CONFIG=Release -j 12

#copy it over and go back to installer dir
cp -r build/Odin2.vst3 ../../installers/linux/$package_name/usr/lib/vst3/Odin2.vst3
cp -r build/Odin2_.lv2 ../../installers/linux/$package_name/usr/lib/lv2/Odin2.lv2
popd

#cp Soundbanks
cp -r ../../Soundbanks/* $package_name/opt/odin2/Soundbanks/
#remove empty .gitignore
rm $package_name/opt/odin2/Soundbanks/User\ Patches/.gitignore 

mkdir $package_name/DEBIAN

#create config file
echo "Package: Odin" >> $package_name/DEBIAN/control
echo "Version: ${package_version}" >> $package_name/DEBIAN/control
echo "Section: base" >> $package_name/DEBIAN/control
echo "Priority: optional" >> $package_name/DEBIAN/control
echo "Architecture: amd64" >> $package_name/DEBIAN/control
echo "Depends:" >> $package_name/DEBIAN/control
echo "Maintainer: TheWaveWarden info@thewavewarden.com" >> $package_name/DEBIAN/control
echo "Description: Odin 2" >> $package_name/DEBIAN/control
echo "  Semimodular VST3 and LV2 Synthesizer Plugin" >> $package_name/DEBIAN/control

#create postinst file
echo "chmod -R 777 /opt/odin2" >> $package_name/DEBIAN/postinst
sudo chmod -R 775 $package_name/DEBIAN/postinst

#build package
dpkg-deb --build $package_name

#remove temp files
rm -rf $package_name



