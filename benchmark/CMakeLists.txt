add_definitions(-DBENCHMARK)

add_executable(mathapprox
    mathapprox.cpp)

add_executable (odin_benchmark 
    odin_benchmark.cpp
    ../Source/audio/Oscillators/AnalogOscillator.cpp
    ../Source/audio/Oscillators/ChiptuneArpeggiator.cpp
    ../Source/audio/Oscillators/ChiptuneOscillator.cpp
    ../Source/audio/Oscillators/DriftGenerator.cpp
    ../Source/audio/Oscillators/FMOscillator.cpp
    ../Source/audio/Oscillators/LFO.cpp
    ../Source/audio/Oscillators/MultiOscillator.cpp
    ../Source/audio/Oscillators/NoiseOscillator.cpp
    ../Source/audio/Oscillators/Oscillator.cpp
    ../Source/audio/Oscillators/PitchShiftTable.h
    ../Source/audio/Oscillators/PMOscillator.cpp
    ../Source/audio/Oscillators/VectorOscillator.cpp
    ../Source/audio/Oscillators/WavetableContainer.cpp
    ../Source/audio/Oscillators/WavetableOsc1D.cpp
    ../Source/audio/Oscillators/WavetableOsc2D.cpp
    ../Source/audio/Filters/VAOnePoleFilter.cpp
    ../Source/audio/Filters/Filter.cpp
    ../Source/audio/Oscillators/Wavetables/Tables/WavetableData.cpp
    ../Source/audio/Oscillators/Wavetables/Tables/LFOTableData.cpp
)

target_link_libraries (odin_benchmark benchmark pthread)