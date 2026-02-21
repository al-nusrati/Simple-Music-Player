# 🎵 HIVE — RetroPlayer

> A lightweight, console-based music player built with C++ and SFML, powered by a custom Doubly Linked List data structure.

![C++](https://img.shields.io/badge/C++-17-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![SFML](https://img.shields.io/badge/SFML-3.0.2-8CC445?style=flat&logo=sfml&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=flat&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-Academic-blueviolet?style=flat)

---

## 📸 Preview

```
+------------------------------------------------+
|                      HIVE                      |
+------------------------------------------------+

>>> NOW PLAYING <<<
Title  : Faslon Ko Takkaluf
Artist : Atif Aslam
Status : [ PLAYING ]

--- PLAYLIST (3 Tracks) ---
[1] Faslon Ko Takkaluf by Atif Aslam
[2] Balaghal Ula Bi Kamaalihi by Ali Zafar
[3] MUSTAFA JAAN E REHMAT by Atif Aslam

+------------------------------------------------+
[1] Play/Pause    [2] Next Track    [3] Prev Track
[4] Add Song      [5] Remove Song   [6] Exit
+------------------------------------------------+
Enter choice: 
```

---

## 📋 Table of Contents

- [About](#about)
- [Features](#features)
- [Architecture](#architecture)
- [Data Structure](#data-structure)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Setup & Installation](#setup--installation)
- [Usage](#usage)
- [Class Reference](#class-reference)
- [Complexity Analysis](#complexity-analysis)
- [Assignment Rubric Coverage](#assignment-rubric-coverage)

---

## About

HIVE is a minimal, resource-efficient music player built as part of a Foundations of Programming assignment. The goal was to design and implement a backend data structure from scratch — without relying on standard containers like `std::vector` or `std::list` — and integrate it with real audio playback via SFML.

The entire playlist is managed by a custom templated **Doubly Linked List**, enabling dynamic track management, bidirectional navigation, and O(1) track switching via a stored node pointer.

---

## Features

- 🎵 Real audio playback via SFML (`sf::Music`)
- ➕ Add tracks dynamically (at beginning, end, or any position)
- ❌ Remove tracks by ID
- 🔍 Search tracks by title
- ⏭️ Next / Previous track navigation
- 🔁 Auto-advance to next track when current ends
- 📋 Live playlist display with track counter
- 🎨 Colored console UI using ANSI escape codes (via `ConsoleUtils`)
- 💡 O(1) track navigation using a stored `node<Track>*` pointer

---

## Architecture

The project follows a clean **3-layer architecture**:

```
┌─────────────────────────────────┐
│   MusicPlayer (Presentation)    │  ← Console UI, SFML audio, user input
├─────────────────────────────────┤
│      Playlist (Domain Logic)    │  ← Add, Remove, Search, Navigate
├─────────────────────────────────┤
│  DoublyLinkedList<T> (Data)     │  ← Generic DLL, memory management
└─────────────────────────────────┘
```

Each layer has a single, well-defined responsibility. `MusicPlayer` knows nothing about the list internals — it only calls `Playlist` methods. `Playlist` knows nothing about audio or rendering.

---

## Data Structure

### Why a Doubly Linked List?

| Requirement | Array | Singly Linked List | Doubly Linked List ✅ |
|---|---|---|---|
| Dynamic sizing | ❌ Fixed or costly resize | ✅ | ✅ |
| Mid-list insert/delete | ❌ O(n) shifting | ✅ O(1) rewire | ✅ O(1) rewire |
| Bidirectional navigation | ❌ Only forward | ❌ Only forward | ✅ Both directions |
| O(1) track switching | ❌ | ❌ | ✅ via `currentNode*` |

### Fix 1 — O(1) Navigation via Stored Pointer

A typical implementation stores a numeric index (`currentIndex = 2`) and calls `getNthNode(currentIndex)` on every track skip, which traverses from the head each time — **O(n) per navigation event**.

HIVE instead stores a direct `node<Track>*` pointer called `currentTrackNode` inside the `Playlist` class. Moving to the next or previous track is a **single pointer dereference** — no traversal at all:

```cpp
// O(1) — no loop, just a pointer hop
void moveNext() {
    if (currentTrackNode && currentTrackNode->next)
        currentTrackNode = currentTrackNode->next;
    else
        currentTrackNode = dll.getHead(); // wrap around
}
```

### Fix 2 — O(1) Track Count

The `DoublyLinkedList` maintains a `listSize` counter that is incremented/decremented on every insert/delete. `nodeCount()` simply returns this integer — **no loop required**:

```cpp
int nodeCount() const { return listSize; } // O(1)
```

---

## Project Structure

```
HIVE/
│
├── src/
│   ├── main.cpp                  # Entry point, MusicPlayer & Playlist classes
│   └── DoublyLinkedList.h        # Templated DLL data structure
│
├── Libraries/
│   └── ConsoleUtils/
│       ├── ConsoleUtils.h        # Console color & cursor utilities (header)
│       └── ConsoleUtils.cpp      # ConsoleUtils implementation
│
├── assets/
│   ├── music/
│   │   ├── Faslon Ko Takkaluf.mp3
│   │   ├── Balaghal Ula Bi Kamaalihi.mp3
│   │   └── MUSTAFA JAAN E REHMAT.mp3
│   ├── fonts/
│   │   └── arial.ttf
│   └── images/
│       └── image.jpg
│
└── README.md
```

---

## Prerequisites

- **Visual Studio 2022** (or any MSVC-compatible compiler)
- **SFML 3.0.2** — [Download here](https://www.sfml-dev.org/download/sfml/3.0.2/)
- **Windows 10 or later** (required for ANSI virtual terminal support in `ConsoleUtils`)

---

## Setup & Installation

### 1. Clone the repository

```bash
git clone https://github.com/yourusername/hive-music-player.git
cd hive-music-player
```

### 2. Install SFML

Follow the official SFML + Visual Studio setup guide:  
👉 https://www.sfml-dev.org/tutorials/3.0/getting-started/visual-studio/

Key steps:
- Download SFML 3.0.2 for your compiler (MSVC)
- Set **Additional Include Directories** → path to `SFML/include`
- Set **Additional Library Directories** → path to `SFML/lib`
- Link these in **Additional Dependencies**: `sfml-audio.lib`, `sfml-graphics.lib`, `sfml-window.lib`, `sfml-system.lib`
- Copy all SFML `.dll` files into your project output folder

### 3. Set up ConsoleUtils

In Visual Studio:
- Add `ConsoleUtils` as a **static library project** to your solution
- In the main project properties:
  - **References** → Add reference to ConsoleUtils
  - **C/C++ → General → Additional Include Directories** → `$(SolutionDir)..\Libraries\ConsoleUtils`

### 4. Add your music files

Place your `.mp3` files in `assets/music/` and update the paths in `main()`:

```cpp
myPlaylist.addTrack("Song Title", "Artist Name", 0, "assets/music/your_song.mp3");
```

### 5. Build & Run

Build the solution in Visual Studio (`Ctrl+Shift+B`) and run (`Ctrl+F5`).

---

## Usage

| Key | Action |
|---|---|
| `1` | Play / Pause current track |
| `2` | Skip to next track |
| `3` | Go to previous track |
| `4` | Add a new song (enter title, artist, file path) |
| `5` | Remove a song by Track ID |
| `6` | Exit the player |

When a track finishes playing, HIVE automatically advances to the next one.

---

## Class Reference

### `DoublyLinkedList<T>`

Generic templated doubly linked list. Lives in `DoublyLinkedList.h`.

| Method | Description | Complexity |
|---|---|---|
| `insertAtBeginning(val)` | Insert node at head | O(1) |
| `insertAtEnd(val)` | Insert node at tail | O(1) |
| `insertAtAnyPos(pos, val)` | Insert at position | O(n) |
| `deleteFromStart()` | Remove head node | O(1) |
| `deleteFromEnd()` | Remove tail node | O(1) |
| `deleteAtAnyPos(pos)` | Remove at position | O(n) |
| `nodeCount()` | Returns total nodes | **O(1)** via `listSize` |
| `getHead()` | Returns head pointer | O(1) |
| `isEmpty()` | Returns true if empty | O(1) |
| `traverseForward()` | Prints all nodes | O(n) |
| `freeMemory()` | Deletes all nodes | O(n) |

---

### `Playlist`

Domain logic layer wrapping `DoublyLinkedList<Track>`.

| Method | Description |
|---|---|
| `addTrack(title, artist, duration, path)` | Appends track to end |
| `removeTrack(id)` | Removes track by ID |
| `moveNext()` | Advances `currentTrackNode` — **O(1)** |
| `movePrev()` | Moves back `currentTrackNode` — **O(1)** |
| `getCurrentTrack()` | Returns pointer to active track |
| `displayPlaylist()` | Prints full playlist to console |
| `getTotalTracks()` | Returns track count — **O(1)** |

---

### `MusicPlayer`

Presentation layer managing console UI and SFML audio.

| Method | Description |
|---|---|
| `run()` | Main application loop |
| `drawDashboard()` | Renders the console UI |
| `handleInput(choice)` | Processes user input |
| `playAudio()` | Opens file and calls `music.play()` |

---

### `ConsoleUtils`

Static utility class for Windows console styling.

| Method | Description |
|---|---|
| `enableVirtualTerminal()` | Enables ANSI escape code support |
| `setForegroundColor(color)` | Sets text color (256-color ANSI) |
| `setBackgroundColor(color)` | Sets background color |
| `setDefaultColor()` | Resets to default terminal color |
| `clearConsole()` | Clears the console screen |
| `hideCursor()` / `showCursor()` | Cursor visibility control |
| `moveCursor(x, y)` | Moves cursor to position |
| `DrawRectangle(...)` | Draws a single-line box |
| `DrawDoubleLineRectangle(...)` | Draws a double-line box |

---

## Complexity Analysis

| Operation | Complexity | Notes |
|---|---|---|
| Add track (end) | O(1) | Direct tail pointer access |
| Add track (beginning) | O(1) | Direct head pointer access |
| Add track (position) | O(n) | Must traverse to position |
| Remove track by ID | O(n) | Single pass search + O(n) deleteAtAnyPos |
| Next / Prev navigation | **O(1)** | Stored `currentTrackNode*` pointer |
| Track count | **O(1)** | Maintained `listSize` counter |
| Search by title | O(n) | Linear scan |
| Display playlist | O(n) | Full traversal |

---

## Assignment Rubric Coverage

| Category | Weight | Implementation |
|---|---|---|
| Data Structure Utilization | 30% | Custom `DoublyLinkedList<T>` with all required operations |
| Object-Oriented Design | 20% | 3-class architecture: `DoublyLinkedList`, `Playlist`, `MusicPlayer` |
| Audio Library Integration | 15% | SFML `sf::Music` with play/pause/auto-advance |
| Documentation & Defense | 15% | Inline comments, this README, complexity justifications |
| Robustness & Error Handling | 10% | Null checks, invalid ID handling, safe pointer updates |
| UI & Console Experience | 10% | Colored console UI via `ConsoleUtils` with ANSI escape codes |

---

## Author

**Jawad Ahmed**  
Foundations of Programming — 2nd Semester, 2025
