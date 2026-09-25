# AudioSynth

This project is focused on learning how to build a C++ synthesizer. Early on, it was an offline sine-wave generator that wrote WAV files. Now it is becoming a real-time JUCE synthesizer that can be played though a midi keyboard.

I am currently developing this project slowly so that the audio programming, C++, and digital signal processing concepts remain understandable and digestable.

## Current features

- Generates 16-bit mono WAV files
- Sine, square, sawtooth, and triangle oscillators
- Adjustable oscillator frequency
- ADSR amplitude envelope
- Live audio output through JUCE
- MIDI keyboard input
- MIDI velocity sensitivity
- Eight-voice polyphony
- Automatic audio-device sample-rate handling
- Console selection of MIDI input devices

## Current limitations

- Up to eight voices may be played together.
- Live voices currently use a sine waveform only.
- No voice-stealing system.
- No graphical interface.
- No filter or effects.
- Synth parameters are set in code.
- Not available as a VST3 plugin for a DAW.
- Basic saw and square waves may produce aliasing at high pitches.
- No soft, middle, or sustain pedal compatibility.

## Signal path

The current live signal path is:

```text
MIDI controller
    → MIDI note and velocity
    → SynthVoice
    → Oscillator
    → ADSR envelope
    → Voice mixer
    → Audio device
