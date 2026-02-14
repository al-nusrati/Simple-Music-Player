# 🎵 SFML Music Player

A simple, cross-platform music player built with C++ and SFML 3.0.2.

![C++](https://img.shields.io/badge/C%2B%2B-20-blue)
![SFML](https://img.shields.io/badge/SFML-3.0.2-green)
![License](https://img.shields.io/badge/license-MIT-orange)

## Features

- 🎼 Play/Pause music
- ⏭️ Next/Previous track navigation
- 🔄 Auto-advance to next song
- 🎨 Customizable background
- ⌨️ Keyboard controls

## Controls

| Key | Action |
|-----|--------|
| `SPACE` | Play/Pause |
| `N` | Next Song |
| `P` | Previous Song |
| `ESC` | Exit |

## Prerequisites

- C++20 compatible compiler (GCC 14.2.0+ or MSVC)
- SFML 3.0.2
- CMake 3.16+ (optional, for building)

## Installation

### Windows

1. **Download SFML 3.0.2**
   ```
   Download from: https://www.sfml-dev.org/download/sfml/3.0.2/
   Choose: GCC 14.2.0 MinGW (64-bit) version
   ```

2. **Clone this repository**
   ```bash
   git clone https://github.com/yourusername/sfml-music-player.git
   cd sfml-music-player
   ```

3. **Setup Assets**
   ```bash
   # Create asset folders
   mkdir assets\fonts assets\music assets\images
   
   # Copy a font (Arial from Windows)
   copy C:\Windows\Fonts\arial.ttf assets\fonts\
   
   # Add your MP3 files
   # Name them: song1.mp3, song2.mp3, song3.mp3
   ```

4. **Build**
   
   **Option A - Using CMake:**
   ```bash
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles" -DSFML_DIR="path/to/SFML-3.0.2/lib/cmake/SFML"
   cmake --build .
   ```
   
   **Option B - Direct compilation:**
   ```bash
   g++ -std=c++20 src/main.cpp -o bin/MusicPlayer.exe ^
     -I path/to/SFML-3.0.2/include ^
     -L path/to/SFML-3.0.2/lib ^
     -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
   ```

5. **Copy DLLs**
   ```bash
   # Copy SFML DLLs from SFML-3.0.2/bin/ to your bin/ folder
   copy path\to\SFML-3.0.2\bin\*.dll bin\
   ```

6. **Run**
   ```bash
   cd bin
   .\MusicPlayer.exe
   ```

### Linux

1. **Install SFML 3.0**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libsfml-dev
   
   # Arch Linux
   sudo pacman -S sfml
   ```

2. **Build and run**
   ```bash
   git clone https://github.com/yourusername/sfml-music-player.git
   cd sfml-music-player
   mkdir build && cd build
   cmake ..
   make
   ./MusicPlayer
   ```

## Project Structure

```
sfml-music-player/
├── src/
│   └── main.cpp          # Main source code
├── assets/
│   ├── fonts/
│   │   └── arial.ttf     # Font file (not included)
│   ├── images/
│   │   └── image.jpg     # Background image (optional)
│   └── music/
│       ├── song1.mp3     # Music files (not included)
│       ├── song2.mp3
│       └── song3.mp3
├── bin/                  # Compiled executable (gitignored)
├── CMakeLists.txt        # CMake build configuration
├── .gitignore
└── README.md
```

## Configuration

Edit `src/main.cpp` to customize:
- Playlist songs (lines 46-50)
- Window size (line 24)
- UI colors and positioning (lines 115-124)

## Dependencies

This project requires:
- **SFML 3.0.2** - Graphics, Window, Audio, System modules
- **OpenAL** - Audio library (usually included with SFML)

## Troubleshooting

### "Failed to load font"
- Ensure `assets/fonts/arial.ttf` exists
- Or use any `.ttf` font file

### "Missing openal32.dll"
- Copy `openal32.dll` from SFML bin folder to your executable directory
- Or install OpenAL from: https://www.openal.org/downloads/

### "Cannot find -lsfml-audio-3"
- Use library names WITHOUT the "-3" suffix
- Correct: `-lsfml-audio`, not `-lsfml-audio-3`

## License

MIT License - see LICENSE file for details

## Contributing

Contributions welcome! Please feel free to submit a Pull Request.

## Author

Your Name - [@yourusername](https://github.com/yourusername)

## Acknowledgments

- SFML Team for the amazing multimedia library
- Font and music assets are not included (add your own)
