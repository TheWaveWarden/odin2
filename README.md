# Odin 2 Synthesizer

This is the home of Odin 2 freeware VST and AU synthesizer plugin.

![alt text](screenshot.png)

# Build Instructions
## All Platforms
Download JUCE from www.TODO.com/getjuce. The project is currently build against JUCE 6.0.1, using newer versions might need some adjustments. Legacy versions can be found at www.github.com/juce/TODO. Open the file `odinvst/Odin.jucer` with the `Projucer` application from the JUCE main directory. Now generate the build files by pressing `ctr + P` inside the `Projucer`. You can close `Projucer` now.

## Windows (Visual Studio 2019)

Open the VS2019 solution `odinvst/Builds/VisualStudio2019/Odin2.sln` with `VisualStudio2019` and compile the project.

## MacOS X (XCode)

Open the XCode project `odinvst/Builds/MacOSX/Odin2.xcodeproj` and compile it.

## Linux (Makefile)

Open a terminal and navigate to `odinvst/Builds/LinuxMakefile`. Now compile the project with `make`. You will most likely hit some errors because header files are not found. You need to find out which packages the header belongs to and install the required packages. For Ubuntu 18.04, the required packages can be installed with
```
sudo apt install libx11-dev libwebkit2gtk-4.0-dev mesa-common-dev libasound2-dev libcurl4-gnutls-dev libcurl4-gnutls-dev
```

## Compiling as VST2 Plugin
VST2 is no longer licensed by Steinberg. You will need to find the `VST2 SDK` on the internet yourself. Once you have it, open `Odin.jucer` with the `Projucer` again. Navigate to `File -> Global Paths...` and under `VST (Legacy) SDK` insert the path to the `VST2_SDK` folder. Close the window and hit the gear-icon on the top left of the `Projucer` interface. Under `Plugin Formats` enable `VST (Legacy)`. Press `Ctr + S` and build the project again as described above.

**IMPORTANT**: Steinbergs licencing prohibits you from redistributing any VST2 plugins, unless you've been a registered developer with them since 2018.