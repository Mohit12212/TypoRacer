# ⚡ TypoRacer Pro — Advanced Computer Aptitude & Typing Intelligence Suite

A professional, high-performance desktop application engineered with **Modern C++ (C++14) and the Win32 / GDI API**. 

Originally a standard typing test, this project has been upgraded into a comprehensive **Academic Computer Aptitude & Keystroke Dynamics Evaluation Platform**. Built with zero external GUI dependencies, it delivers immediate responsiveness, hardware-accelerated GDI double-buffered rendering, real-time biometric analytics, and procedural sound synthesis.

---

## 🌟 Advanced Features & Upgrades

### 1. 🏎️ Dual-Lane AI Ghost Racer
- **Real-Time Opponent**: Race side-by-side against an automated AI Ghost Bot configured to standard aptitude benchmark speeds (**Off**, **35 WPM** Novice, **60 WPM** Skilled, or **85 WPM** Grandmaster).
- **Interactive Dual Track**: Shows real-time distance and progress markers along with live lead/lag delta indicators (`+14 chars ahead` or `-6 chars behind`).
- **Finish Line Detection**: Interactive checkered flag trigger on paragraph completion.

### 2. ⌨️ Interactive Touch-Typing Keyboard & 10-Finger Guide
- **Visual On-Screen QWERTY Layout**: Complete 5-row keyboard rendered via GDI directly under the typing prompt.
- **Dynamic Active Key Illumination**: The exact next character to type lights up in neon cyan with a pulsing border.
- **Ergonomic Finger Assignment Matrix**: Each key features a color-coded indicator dot indicating which finger to use according to standard touch-typing ergonomics:
  - 🔴 **Coral Pink**: Left Pinky (`1, Q, A, Z`)
  - 🟠 **Amber**: Left Ring (`2, W, S, X`)
  - 🟡 **Yellow**: Left Middle (`3, E, D, C`)
  - 🟢 **Mint Green**: Left & Right Index (`4, 5, R, T, F, G, V, B`, `6, 7, Y, U, H, J, N, M`)
  - 🔵 **Cyan**: Thumbs (`Spacebar`)
  - 🟣 **Indigo / Purple**: Right Middle & Ring (`8, I, K, ,`, `9, O, L, .`)
  - 🔴 **Magenta**: Right Pinky (`0, P, [, ], ;, ', /`)
- **Toggle Control**: One-click keyboard visibility toggle via the top navigation bar (`[ ⌨ KEYBOARD ]`).

### 3. 📈 Real-Time GDI Speed Timeline Line Graph
- **Live Curve Plotting**: Second-by-second WPM performance graphed on the Results Screen.
- **Coordinate Grid & Axes**: Dynamic Y-axis scale up to peak WPM, dotted horizontal benchmark lines, and discrete second timestamps.
- **Accent Glow & Vertices**: Smooth anti-aliased cyan polyline with mint vertex markers highlighting acceleration and cadence.

### 4. 🧠 Computer Aptitude & Keystroke Dynamics Engine
- **Keystroke Consistency Rating (%)**: Computes inter-keystroke latency variance ($\sigma / \mu$). Rhythmic touch typing yields high consistency ($\ge 85\%$).
- **Problem Keys & Error Frequency Analyzer**: Automatically records mistyped characters into a frequency map, pinpointing the candidate's top weakest keys (e.g., `Focus Area: High error rate on ';', 'P', and 'R' keys`).
- **Performance Tier Classification**: Uses the same speed bands as the grade rubric. These are app-defined tiers, not population percentiles (no norming dataset is bundled).
  - $\ge 80\text{ WPM}$ with 97% accuracy: **Godspeed Master**
  - $\ge 65\text{ WPM}$ with 95% accuracy: **Professional Typist**
  - $\ge 50\text{ WPM}$ with 90% accuracy: **Intermediate Typist**
  - $\ge 35\text{ WPM}$ with 80% accuracy: **Developing Typist**
  - Below those bands: **Foundational / Needs Practice**

### 5. 🔊 Procedural Audio Synthesizer (Zero External Assets)
- Built-in mathematical waveform audio engine using Windows Multimedia (`winmm.dll`).
- Generates procedural 16-bit PCM waveforms directly in memory:
  - **Keypress**: Crisp 14 ms mechanical switch click (1800 Hz dampened pulse).
  - **Spacebar**: Deep 22 ms mechanical switch thump (650 Hz).
  - **Backspace**: Double-transient mechanical release clack.
  - **Error Pip**: Soft 40 ms low alert tone (260 Hz).
  - **Completion Chime**: Ascending vintage typewriter bell (2093 Hz crystal harmonic).
- Fully toggleable in real time (`[ 🔊 SOUND: ON/OFF ]`).

### 6. 💻 Specialized Practice Modes
- **General Prose**: Easy (60s), Medium (45s), and Hard (30s) vocabulary.
- **Code Aptitude Mode**: Real programming syntax across **C++ algorithms**, **Python data pipelines**, **SQL database queries**, and **JavaScript async APIs**.
- **Numbers & Symbols Mode**: Drills for IP addresses, regex expressions, math matrices, and passwords (`{}[]()$#@!*`).
- **Survival Sprint Mode**: Sudden-death precision test; falling below 90% accuracy after 20 typed characters or reaching 8 current errors causes immediate test abortion.
- **Custom Text Mode**: Paste or type custom lecture notes, essays, or code files.

### 7. 📜 Exportable Official Aptitude Certificate
- One-click export (`[ 📜 EXPORT CERTIFICATE ]`) generates a publication-ready HTML/CSS evaluation certificate:
  - Candidate student name
  - Test category and difficulty
  - Net WPM, CPM, Accuracy %, and Consistency %
  - Performance Grade Badge (`S+`, `A`, `B`, `C`, `D`)
  - Problem Keys Diagnostics and timestamp verification
  - Automatic browser launch with `@media print` styling for instant PDF generation.

### 8. 📊 Session History & CSV Ledger
- Records credible completed attempts to `history.csv`; empty/aborted attempts and runs shorter than 3 seconds are excluded.
- Blocks clipboard paste, cut, and file-drop input in the typing field to keep results meaningful.
- Uses active elapsed time for WPM/CPM and excludes paused time from the clock.
- Interactive in-app history viewer displaying past attempts, average speed, all-time best score, and date stamps.

---

## 📊 Performance Grading Rubric

| Grade | Speed (WPM) | Accuracy | Consistency | Tier Title |
| :---: | :---: | :---: | :---: | :---: |
| **S+** | $\ge 80\text{ WPM}$ | $\ge 97\%$ | $\ge 75\%$ | 🥇 Godspeed Master |
| **A** | $\ge 65\text{ WPM}$ | $\ge 95\%$ | $\ge 65\%$ | 🟢 Professional Typist |
| **B** | $\ge 50\text{ WPM}$ | $\ge 90\%$ | $\ge 55\%$ | 🔵 Intermediate Typist |
| **C** | $\ge 35\text{ WPM}$ | $\ge 80\%$ | $\ge 45\%$ | 🟡 Casual Typist |
| **D** | $< 35\text{ WPM}$ | Any | Any | 🔴 Novice Typist |

---

## 🛠️ Project Structure

```
Typing Speed Tester/
├── src/
│   ├── main.cpp          # Win32 WinMain, window procedure, HUD, GDI renderers, Keyboard, Graph
│   ├── TypingTest.h      # Core engine header, enums, metrics struct, latency dynamics
│   ├── TypingTest.cpp    # Real-time analytics, code pools, consistency math, certificate generator
│   └── SoundEngine.h     # Procedural 16-bit PCM audio synthesizer (zero external dependencies)
├── resources/
│   └── resource.h        # Unified Control and Menu IDs
├── build.bat             # One-click MinGW g++ build script
├── scores.dat            # Persistent high-score database (auto-generated)
├── history.csv           # Persistent tabular test history (auto-generated)
├── TypoRacer.exe         # Standalone compiled desktop executable (~430 KB)
├── Typing_Aptitude_Certificate.html # Sample generated official evaluation certificate
└── README.md             # Project documentation
```

---

## 🚀 How to Build & Run

### Build and Launch
Requirements: **Windows 10/11** and **MinGW-w64** (`g++`) with the standard Windows SDK libraries.
1. Double-click `build.bat`. It compiles the latest source and launches `TypoRacer.exe`.
2. Alternatively, run this command in PowerShell or Command Prompt:
```cmd
g++ -o "TypoRacer.exe" src/main.cpp src/TypingTest.cpp -mwindows -lgdi32 -lcomctl32 -luser32 -lwinmm -std=c++14 -O2
```

---

## 🛡️ Integrity and Reliability Improvements

- WPM/CPM use high-resolution active elapsed time instead of rounded countdown seconds; paused time is excluded.
- Paste, cut, and drag/drop are blocked in the typing input. The field is limited to the current passage length.
- Key diagnostics are recalculated from the current attempt so one old typo is not counted repeatedly on every keystroke.
- Aborted, survival-failed, too-short, and implausibly fast attempts do not affect personal records or session history. Certificates are disabled for unsaved attempts.
- Candidate names and certificate diagnostics are HTML-escaped, and certificate typography uses system fonts so the layout does not depend on Google Fonts or an internet connection.
- Survival mode now uses the documented 90% accuracy threshold after the initial 20 characters.
- The project is delivered with clean `history.csv` and `scores.dat` files for a fresh demonstration.

## 🎓 Academic Presentation & Viva Highlights

When presenting this project for your **Computer Aptitude / Computer Applications** subject:
1. **Low-Level Native Architecture**: Built in native C++ using the Win32 API and GDI without relying on Electron, Python, or bloated frameworks. Consumes $< 15\text{ MB}$ RAM.
2. **Keystroke Dynamics**: Explain the math behind the **Consistency Metric** ($CV = \sigma / \mu$) and how inter-keystroke interval analysis is used in real-world biometric security and aptitude tests.
3. **Procedural Audio Generation**: Explain how sound is synthesized using mathematical sine wave equations with exponential decay envelopes, packing raw PCM data into RIFF WAVE headers in memory.
4. **Pedagogical Value**: Highlight the **Touch-Typing Finger Placement Guide** and **Code Aptitude Mode**, demonstrating how this application serves as a complete learning and diagnostic tool for computer science students.
