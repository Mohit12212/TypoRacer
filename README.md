# TypoRacer

TypoRacer is a typing-practice project with two versions: a native Windows desktop application built in C++14 using the Win32/GDI APIs, and a standalone browser-based typing interface in `index.html`.

## Features

- Timed typing tests with WPM, CPM, and accuracy metrics
- AI ghost-racing difficulty options in the desktop app
- General prose, programming/code, numbers and symbols, survival sprint, and custom-text practice modes
- On-screen QWERTY keyboard with touch-typing finger guidance
- Live typing performance visualization and keystroke consistency analysis
- Weak-key/error diagnostics and session history
- Procedural typing sounds in the Windows desktop app
- Exportable HTML typing-aptitude certificate
- Standalone browser UI that can be opened directly in a modern browser

## Project structure

```text
TypoRacer/
├── src/
│   ├── main.cpp
│   ├── TypingTest.cpp
│   ├── TypingTest.h
│   └── SoundEngine.h
├── resources/
│   └── resource.h
├── docs/
│   └── Typing_Aptitude_Certificate.html
├── index.html
├── build.bat
├── .gitignore
└── README.md
```

## Run the browser version

Open `index.html` in a modern browser. It is a standalone HTML file; an internet connection may be needed to load its optional web fonts.

## Build the Windows desktop version

### Requirements

- Windows 10 or Windows 11
- MinGW-w64 with `g++` available on your `PATH`

### Build and run

From the repository root, double-click `build.bat`, or run this command in a terminal:

```bat
g++ -o TypoRacer.exe src/main.cpp src/TypingTest.cpp -mwindows -lgdi32 -lcomctl32 -luser32 -lwinmm -std=c++14 -O2
```

The build script launches `TypoRacer.exe` after successful compilation.

## Runtime files and generated output

The application may create `scores.dat`, `history.csv`, and exported certificate files while running. These are runtime/user data, so they are not included in the repository. The compiled `.exe` is also excluded; build it locally.

## Performance grading

Performance tiers are application-defined and should be treated as practice feedback, not as externally normed aptitude percentiles.

## License

No license has been added. Unless a license is added, normal copyright applies and reuse permissions are not explicitly granted.
