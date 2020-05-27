#get name for package
read -p "Enter Odin Package Version (i.e. 2.2-0): "  package_version

package_name="Odin_${package_version}"

echo "Creating package ${package_name}"

#recreate directory
rm -rf $package_name
mkdir $package_name

#build tree for vst2 plugin
mkdir $package_name/usr/
mkdir $package_name/usr/lib
mkdir $package_name/usr/lib/vst

#build Odin2 from scratch
pushd ../../Builds/LinuxMakefile/
../../../JUCE/Projucer --resave ../../Odin.jucer
make clean
make CONFIG=Release -j 12

#copy it over and go back to installer dir
cp build/Odin2.so ../../installers/linux/$package_name/usr/lib/vst/Odin2.so
popd

#create config file
mkdir $package_name/DEBIAN
echo "Package: Odin" >> $package_name/DEBIAN/control
echo "Version: ${package_version}" >> $package_name/DEBIAN/control
echo "Section: base" >> $package_name/DEBIAN/control
echo "Priority: optional" >> $package_name/DEBIAN/control
echo "Architecture: i386" >> $package_name/DEBIAN/control
echo "Depends:" >> $package_name/DEBIAN/control
echo "Maintainer: TheWaveWarden info@thewavewarden.com" >> $package_name/DEBIAN/control
echo "Description: Odin 2" >> $package_name/DEBIAN/control
echo "  Semimodular VST Synthesizer Plugin" >> $package_name/DEBIAN/control

dpkg-deb --build $package_name

rm -rf $package_name



