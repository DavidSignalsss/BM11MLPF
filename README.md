# BM-11M Low Pass Filter

<div align="center">

![JUCE](https://img.shields.io/badge/JUCE-8.0.10-orange)
![macOS](https://img.shields.io/badge/macOS-Compatible-success)
![Plugin](https://img.shields.io/badge/Plugin-Audio%20Filter-blue)
![License](https://img.shields.io/badge/License-MIT-green)

</div>

A professional analog-modeled low-pass filter plugin based on the classic Behringer BM-11M, developed with JUCE framework.

![Plugin Interface](screenshots/interface.png)

## 🎛️ Features

- **2-Pole / 4-Pole Filter Modes** (-12dB/oct and -24dB/oct)
- **Drive Control** for input gain saturation
- **Envelope Follower** with Smooth/Fast modes
- **Cutoff Modulation** via envelope amount
- **Resonance Control** for filter emphasis
- **Wet/Dry Mix** for parallel processing
- **Vintage-style UI** with wood texture design

## 🎚️ Controls

| Control | Range | Description |
|---------|-------|-------------|
| **Drive** | 0-10 | Input gain/pre-amplification |
| **Cutoff** | 250Hz-12kHz | Filter cutoff frequency |
| **Filter Mode** | 2-Pole/4-Pole | Filter slope selection |
| **Resonance** | 0-10 | Filter resonance amount |
| **Amount** | 0-10 | Envelope modulation intensity |
| **Envelope** | Smooth/Fast | Envelope follower response |
| **Mix** | 0-10 | Wet/Dry balance |
| **Env Mode** | Bypass/Level | Envelope modulation toggle |

## 🛠️ Development

### Project Structure
BM11MLPF/
├── Source/
│ ├── BM11MLPFAudioProcessor.h/cpp
│ ├── BM11MLPFAudioProcessorEditor.h/cpp
│ └── resources/
├── Builds/
│ └── MacOSX/ (Xcode project)
└── BM11MLPF.jucer (Projucer file) EOF
