# Bleu - Ultimate Audio Plugin Masterpiece

Bleu is a high-performance, feature-rich audio plugin designed for professional guitar processing. Built with JUCE and leveraging cutting-edge DSP libraries, it offers a comprehensive suite of tools from neural amp simulation to advanced modulation and spatial effects.

## 🚀 Key Features

- **Neural Amp Modeling (NAM)**: Powered by the NAM engine for indistinguishable-from-real tube amp tones.
- **35+ Professional Effects**: Including Rotary, Tape Saturation, Talkbox, Granular Reverb, and more.
- **High-Fidelity UI**: Glassmorphism aesthetic with real-time FFT Spectrum analysis and Interactive EQ.
- **Advanced DSP**: HIIR-based oversampling, Soft-clipping safety, and Mid-Side stereo width control.
- **A/B Comparison**: Instantly swap between two independent signal chains.
- **Crossfade Transition**: Smooth, click-free switching between effect presets.

## 🛠 Tech Stack

- **Framework**: [JUCE 8.0.x](https://juce.com/)
- **Engines**: 
  - [Neural Amp Modeler (NAM)](https://github.com/sdatkinson/NeuralAmpModelerCore)
  - [SignalSmith DSP](https://signalsmith-audio.co.uk/) (Delay, FFT)
  - [RubberBand](https://breakfastquay.com/rubberband/) (Time-stretching/Pitch-shifting)
- **Language**: C++20

## 📦 Getting Started

### Prerequisites
- CMake (3.22+) or JUCE Projucer
- C++ Compiler (GCC 11+, Clang 14+, MSVC 2022)

### Building
1. Clone the repository:
   ```bash
   git clone https://github.com/starandstar878-bit/Bleu.git
   ```
2. Open `Bleu.jucer` in Projucer or use the provided Makefile in `Builds/LinuxMakefile`.
3. Build the project.

## 📄 License
This project is for educational and artistic purposes. See individual dependency folders for their respective licenses (NAM, SignalSmith, RubberBand, etc.).

---
*Created on the "Last Day" - A Masterpiece for the future of guitar tone.*
