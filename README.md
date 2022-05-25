## *This fork is my "Waldorfized" variation on Odin 2, so named because longer envelopes and slower LFOs will make it a bit more like Waldorf's Blofeld, and better suited to the long drone work I like to do.*

1. Attack, decay, and release on all envelopes increases to 30 seconds
2. The LFO minimum speed drops to 0.1Hz

# Odin 2 Synthesizer

This is the home of **Odin 2 free VST3, AU and LV2 synthesizer** plugin. This repository contains the code to build the project. **If you just want to download the synth, get it from
https://thewavewarden.com/odin2**

![alt text](screenshot.png)

# Build Instructions
## All Platforms
Clone this project **with submodules**:
```
git clone --recurse-submodules https://github.com/TheWaveWarden/odin2.git
```
Now [download JUCE 6.0.8](https://github.com/juce-framework/JUCE/releases). The project is currently build against JUCE 6.0.8. You can use more recent JUCE versions, but the code might need adjustments then. Open the file `odin2/Odin.jucer` with the `Projucer` application from the JUCE main directory. Now generate the build files by pressing `Ctrl + P` inside the `Projucer`. You can close `Projucer` now.

## Windows (Visual Studio 2019)

Open the VS2019 solution `odin2/Builds/VisualStudio2019/Odin2.sln` with `VisualStudio2019` and compile the project.

## MacOS X (XCode)

Open the XCode project `odin2/Builds/MacOSX/Odin2.xcodeproj` and compile it.

## Linux (Makefile)

Open a terminal and navigate to `odin2/Builds/LinuxMakefile`. Now compile the project with `make`. You will most likely hit some errors because header files are not found. You need to find out which packages the header belongs to and install the required packages. For Ubuntu 18.04, the required packages can be installed with
```
sudo apt install libx11-dev libwebkit2gtk-4.0-dev mesa-common-dev libasound2-dev libcurl4-gnutls-dev libcurl4-gnutls-dev
```

## Compiling as VST2 Plugin (All Platforms)
VST2 is no longer licensed by Steinberg. You will need to find the `VST2 SDK` on the internet yourself. Once you have it, open `Odin.jucer` with the `Projucer` again. Navigate to `File -> Global Paths...` and under `VST (Legacy) SDK` insert the path to the `VST2_SDK` folder. Close the window and hit the gear-icon on the top left of the `Projucer` interface. Under `Plugin Formats` enable `VST (Legacy)`. Press `Ctrl + S` and build the project again as described above.

**IMPORTANT**: Steinbergs licencing prohibits you from distributing any VST2 plugins, unless you've been a registered developer with them since 2018.

## Compiling as LV2 Plugin (Linux)
First, you'll need the LV2 headers. On Ubuntu you can install them with
```
sudo apt install lv2-dev
```
You'll also need a custom set of JUCE modules from the [LV2 Porting Project](https://github.com/lv2-porting-project/). Clone the JUCE fork and check out the `lv2` branch, e.g.:
```
git clone -b lv2 https://github.com/lv2-porting-project/JUCE/ ~/JUCELV2
```
Now open Odin in your **regular** Projucer:
```
/path/to/juce/Projucer odin2/Odin.jucer
```
Navigate to `File`->`Global Paths`. In the Field `JUCE Modules` enter, the modules from the repo you just cloned, e.g.:
```
~/JUCELV2/modules
```
Close the Dialog and generate the Project again with `Ctrl + P`. Close the Projucer.

Add these lines to `odin2/JuceLibraryCode/AppConfig.h` and make sure they are contained within the user code section:
```
// [BEGIN_USER_CODE_SECTION]

  #define JucePlugin_Build_LV2 1
  #define JucePlugin_LV2URI "https://www.thewavewarden.com/odin2"
  #define JucePlugin_MaxNumOutputChannels 2

// [END_USER_CODE_SECTION]
```
The last step is to append a line to the generated Makefile with this command:
```
echo -e "include ../../LV2.mak" >> odin2/Builds/LinuxMakefile/Makefile
```
Now you can build the project again with `make` as before. Note: Every time you export the project from the `Projucer`, you'll have to append the line to the Makefile again.
