rm Odin2_linux_x64.zip
cp ../../Builds/LinuxMakefile/build/Odin2.so Odin2.so
mkdir Soundbanks
cp -r ../../Soundbanks/* Soundbanks/
rm Soundbanks/User\ Patches/.gitignore 
zip -r Odin2_linux_x64.zip Odin2.so Soundbanks install.sh README.txt
rm -rf Soundbanks
rm Odin2.so