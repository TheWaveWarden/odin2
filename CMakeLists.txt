# ================== General Info ======================
cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11 CACHE STRING "Minimum macOS version")
project(Odin2 VERSION 2.4.0 LANGUAGES C CXX ASM)

# ================== CMake Subdirs ======================
add_subdirectory(libs/clap-juce-extensions EXCLUDE_FROM_ALL)
if(UNIX AND NOT APPLE)
  add_subdirectory(libs/JUCELV2 EXCLUDE_FROM_ALL)
else()
  add_subdirectory(libs/JUCE EXCLUDE_FROM_ALL)
endif()

# ================= Git Commit Hash =====================
find_package(Git)
if (Git_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(GIT_HASH "deadbeef")
endif()
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
file(WRITE "${CMAKE_BINARY_DIR}/include/GitCommitId.h" "#define GIT_COMMIT_ID \"${GIT_HASH}\"")

option(ODIN2_COPY_PLUGIN_AFTER_BUILD "Copy JUCE Plugins after built" OFF)

add_compile_definitions(
  JUCE_MODAL_LOOPS_PERMITTED
  #JUCE_ENABLE_REPAINT_DEBUGGING
  CMAKE_SOURCE_DIRECTORY="${CMAKE_SOURCE_DIR}"
)

# ==================== Add Plugin =======================
# Build LV2 only on Linux
set(JUCE_FORMATS AU VST3 Standalone)
if(UNIX AND NOT APPLE)
    list(APPEND JUCE_FORMATS LV2)
endif()

juce_add_plugin(Odin2
  VERSION "2.4.0"
  COMPANY_NAME "TheWaveWarden"
  COMPANY_WEBSITE "www.thewavewarden.com"
  COMPANY_EMAIL "info@thewavewarden.com"
  FORMATS ${JUCE_FORMATS}
  PLUGIN_MANUFACTURER_CODE "WAWA"
  PLUGIN_CODE "ODIN"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  COPY_PLUGIN_AFTER_BUILD ${ODIN2_COPY_PLUGIN_AFTER_BUILD}
  LV2_URI https://thewavewarden.com/odin2
  LV2_SHARED_LIBRARY_NAME Odin2
)

# ==================== CLAP =======================
clap_juce_extensions_plugin(TARGET Odin2
        CLAP_ID "com.thewavewarden.odin2"
        CLAP_FEATURES instrument "virtual analog")

# ==================== JUCE Defines =======================
juce_generate_juce_header(Odin2)
target_compile_definitions(Odin2
  PUBLIC
    JUCE_STRICT_REFCOUNTEDPOINTER=1
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_WEB_BROWSER=0
    JUCE_MODAL_LOOPS_PERMITTED=1
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

# ==================== Check OS =======================
target_include_directories(Odin2 
  PRIVATE 
    ${CMAKE_BINARY_DIR}/include 
    libs/tuning-library
    libs/json/include
)

if(APPLE)
    target_compile_definitions(Odin2 PRIVATE ODIN_MAC=1)
    target_compile_options(Odin2 PRIVATE "-Wno-implicit-float-conversion")
    message("== Building for MacOS")
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(Odin2 PRIVATE ODIN_LINUX=1)
    message("== Building for Linux")
else()
    target_compile_definitions(Odin2 PRIVATE ODIN_WIN=1)
    message("== Building for Windows")
endif()

# ==================== Debug / Release =======================

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("== Building configuration Debug")
    target_compile_definitions(Odin2 PRIVATE ODIN_DEBUG=1)
else()
    message("== Building configuration Release")
endif()

# ==================== Source Files =======================
target_sources(Odin2
  PRIVATE
    "Source/ConfigFileManager.cpp"
    "Source/PluginEditor.cpp"
    "Source/PluginProcessor.cpp"
    "Source/Utilities.cpp"
    "Source/audio/ADSR.cpp"
    "Source/audio/Amplifier.cpp"
    "Source/audio/FX/Bitcrusher.cpp"
    "Source/audio/FX/Chorus.cpp"
    "Source/audio/FX/Delay.cpp"
    "Source/audio/FX/FeedbackDelayNetwork.cpp"
    "Source/audio/FX/Flanger.cpp"
    "Source/audio/FX/IntegerDelayLine.cpp"
    "Source/audio/FX/OversamplingDistortion.cpp"
    "Source/audio/FX/ParametricEQ.cpp"
    "Source/audio/FX/Phaser.cpp"
    "Source/audio/FX/RingModulator.cpp"
    "Source/audio/FX/SurgeReverb.cpp"
    "Source/audio/FX/ZitaReverb.cpp"
    "Source/audio/Filters/BiquadAllpass.cpp"
    "Source/audio/Filters/BiquadEQ.cpp"
    "Source/audio/Filters/BiquadFilter.cpp"
    "Source/audio/Filters/BiquadResonator.cpp"
    "Source/audio/Filters/CombFilter.cpp"
    "Source/audio/Filters/DiodeFilter.cpp"
    "Source/audio/Filters/OdinFilterBase.cpp"
    "Source/audio/Filters/FormantFilter.cpp"
    "Source/audio/Filters/Korg35Filter.cpp"
    "Source/audio/Filters/LadderFilter.cpp"
    "Source/audio/Filters/SEMFilter12.cpp"
    "Source/audio/Filters/VAOnePoleFilter.cpp"
    "Source/audio/ImpulseResponseCreator.cpp"
    "Source/audio/Oscillators/AnalogOscillator.cpp"
    "Source/audio/Oscillators/ChiptuneArpeggiator.cpp"
    "Source/audio/Oscillators/ChiptuneOscillator.cpp"
    "Source/audio/Oscillators/DriftGenerator.cpp"
    "Source/audio/Oscillators/FMOscillator.cpp"
    "Source/audio/Oscillators/LFO.cpp"
    "Source/audio/Oscillators/MultiOscillator.cpp"
    "Source/audio/Oscillators/NoiseOscillator.cpp"
    "Source/audio/Oscillators/Oscillator.cpp"
    "Source/audio/Oscillators/PMOscillator.cpp"
    "Source/audio/Oscillators/VectorOscillator.cpp"
    "Source/audio/Oscillators/WavetableContainer.cpp"
    "Source/audio/Oscillators/WavetableOsc1D.cpp"
    "Source/audio/Oscillators/WavetableOsc2D.cpp"
    "Source/audio/Oscillators/Wavetables/Tables/LFOTableData.cpp"
    "Source/audio/Oscillators/Wavetables/Tables/WavetableData.cpp"
    "Source/gui/JsonGuiProvider.cpp"
    "Source/gui/LiveConstrainer.cpp"
    "Source/gui/GuiDataEditor.cpp"
    "Source/gui/ADSRComponent.cpp"
    "Source/gui/AmpDistortionFlowComponent.cpp"
    "Source/gui/ArpComponent.cpp"
    "Source/gui/BrowserEntry.cpp"
    "Source/gui/ChipdrawWindow.cpp"
    "Source/gui/DelayComponent.cpp"
    "Source/gui/DrawableSlider.cpp"
    "Source/gui/FXComponent.cpp"
    "Source/gui/FilterComponent.cpp"
    "Source/gui/FixedTextGlassDropdown.cpp"
    "Source/gui/GlasDisplay.cpp"
    "Source/gui/GlassDropdown.cpp"
    "Source/gui/IntegerKnob.cpp"
    "Source/gui/OdinKnob.cpp"
    "Source/gui/LFOComponent.cpp"
    "Source/gui/LFODisplayComponent.cpp"
    "Source/gui/LFOPopupLookAndFeel.cpp"
    "Source/gui/LFOSelectorComponent.cpp"
    "Source/gui/LeftRightButton.cpp"
    "Source/gui/ModAmountComponent.cpp"
    "Source/gui/ModAmountComponentExtended.cpp"
    "Source/gui/ModMatrix.cpp"
    "Source/gui/ModMatrixComponent.cpp"
    "Source/gui/NumberSelector.cpp"
    "Source/gui/NumberSelectorWithText.cpp"
    "Source/gui/OdinArpeggiator.cpp"
    "Source/gui/OdinButton.cpp"
    "Source/gui/TitleButton.cpp"
    "Source/gui/OdinControlAttachments.cpp"
    "Source/gui/OscComponent.cpp"
    "Source/gui/PatchBrowser.cpp"
    "Source/gui/PatchBrowserScrollBar.cpp"
    "Source/gui/PatchBrowserSelector.cpp"
    "Source/gui/PhaserComponent.cpp"
    "Source/gui/ReverbComponent.cpp"
    "Source/gui/SpecdrawDisplay.cpp"
    "Source/gui/ResizeDragger.cpp"
    "Source/gui/SpectrumDisplay.cpp"
    "Source/gui/StepComponent.cpp"
    "Source/gui/SyncTimeSelector.cpp"
    "Source/gui/TuningComponent.cpp"
    "Source/gui/WavedrawDisplay.cpp"
    "Source/gui/WaveformSelectorComponent.cpp"
    "Source/gui/XYPadComponent.cpp"
    "Source/gui/XYSectionComponent.cpp"
    "Source/gui/SplineAdComponent.cpp"
    "Source/gui/UIAssetManager.cpp"
    "Source/gui/UIAssetsData.cpp"
    "Source/gui/UIAssetsSize.cpp"
    "Source/gui/RescaleProgressComponent.cpp"
)

# ==================== Binary Files =======================
add_subdirectory(assets)

set_target_properties(Odin2_BinaryData PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# ==================== Linkage =======================
target_link_libraries(Odin2
  PUBLIC
    Odin2_BinaryData
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_cryptography
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_opengl
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    #juce::juce_recommended_warning_flags
)

# ==================== Installation =======================

include(GNUInstallDirs)

if(TARGET ${PROJECT_NAME}_Standalone)
  install(TARGETS ${PROJECT_NAME}_Standalone DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

foreach(format VST3 LV2)
  if(TARGET ${PROJECT_NAME}_${format})
    get_target_property(output ${PROJECT_NAME}_${format} JUCE_PLUGIN_ARTEFACT_FILE)
    install(DIRECTORY ${output} DESTINATION ${CMAKE_INSTALL_LIBDIR}/$<LOWER_CASE:${format}>)
  endif()
endforeach()

if(TARGET ${PROJECT_NAME}_CLAP)
  install(TARGETS ${PROJECT_NAME}_CLAP DESTINATION ${CMAKE_INSTALL_LIBDIR}/clap)
endif()
