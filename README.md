# Simple Delay Plugin (C++ / JUCE)

A digital delay audio effect plugin built with C++ and the JUCE framework.
This project is based on the concepts from the book **"The Complete Beginner's Guide to Audio Plug-in Development"** by Matthijs Hollemans.

I worked through this project to teach myself the practical application of C++ in an audio context and to bridge the gap between my university studies (Audio Engineering) and real-world software development.

<img width="548" height="424" alt="Bildschirmfoto 2026-01-05 um 17 03 57" src="https://github.com/user-attachments/assets/36fef40d-b6b0-43f5-bc7b-8308a10399c1" />


## Features
- **Stereo Delay Line:** Circular buffer implementation for standard echo effects.
- **Feedback Loop:** Controls the decay of the delay tail.
- **Visual Feedback:** Integrated Level Meter to visualize input/output gain.
- **Safety First:** "ProtectYourEars" module to prevent digital clipping.
- **GUI:** Custom LookAndFeel utilizing JUCE graphics.

## Tech Stack
- **Language:** C++ (C++17 standard)
- **Framework:** JUCE
- **IDE:** Xcode / Projucer

## Learning Outcomes
Through this project, I learned:
* How to structure a JUCE audio processor (`processBlock` lifecycle).
* Handling circular buffers and memory management in C++.
* Separating DSP logic from UI code (Model-View-Controller pattern).

## Status & Known Issues
* **Standalone App:** Builds and runs successfully (macOS).
* **VST3 Build:** Currently pending an update to Xcode ARC settings.

---
*Implementation by Robin. Original concept and educational guidance by Matthijs Hollemans.*
