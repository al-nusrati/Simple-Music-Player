#pragma once

// Note: Don't forget to do following in startup project: -
//		 (1) Reference this static library
//				* Links the compiled binary code from the static library into your executable
//				* Makes the library's compiled code part of your final executable
//		 (2) Set "Additional Include Directories" path to --> $(SolutionDir)..\Libraries\ConsoleUtils
//				* Tells the compiler where to look for header files (*.h or *.cpp)
//			 Its set via	Project Properties -> C/C++ -> General -> Additional Include Directories
//
// Errors to keep an eye on:
//		"unknown identifier"  -> missing include path
//		"unresolved external" -> missing library reference


// Enum for ANSI Color Codes
enum ConsoleColor {
	Black = 0,
	Red = 1,
	Green = 2,
	Yellow = 3,
	Blue = 4,
	Magenta = 5,
	Cyan = 6,
	White = 7,
	BrightBlack = 8,
	BrightRed = 9,
	BrightGreen = 10,
	BrightYellow = 11,
	BrightBlue = 12,
	BrightMagenta = 13,
	BrightCyan = 14,
	BrightWhite = 15
};

/*
	enableVirtualTerminal
	---
	Sets a console mode flag (ENABLE_VIRTUAL_TERMINAL_PROCESSING) on the console output handle.
	This enables ANSI escape sequence processing, and renders colors, cursor moves and other terminal control commands properly.
	Once, at program startup, remains available for lifetime of the console (or until you disable it)
	No need to call repeatedly.

	ANSI escape sequences
	---
	These are text-based commands that you send to a terminal (console) to control how it behaves.
	They're called escape sequences because they start with the escape character ESC,
	which tells the terminal "a special command is coming".
	e.g. \033[?25l	("Tell the terminal to hide the cursor.")
		\033	Octal for ASCII escape character (ESC or 0x1B)
		[		Introduces a control sequence (called CSI = Control Sequence Introducer)
		?25		The specific cursor control command (cursor visibility)
		l		"Reset" or "disable" the feature (hide the cursor)

	std:: cout just outputs bytes to the terminal. It doesn't know what the string means.
	It's the terminal emulator that:
	*	Receives the bytes from your program
	*	Detects escape sequences
	*	Interprets them and performs the action (like hiding the cursor)
	So std::cout << "\033[?25l"; just prints bytes � but the terminal interprets them specially.

	ANSI escape codes like "?25l" are part of the "ANSI X3.64 / ECMA-48 / ISO 6429" standard,
	which defines control sequences for terminals.


	setColor
	---
	Set Modern Color : Uses ANSI escape codes (supports 256 colors, modern terminals only i.e. Win10 and above)
	Overview of 256 Color Palette:
	Color Range		Color Codes			Description
	0�15			Basic 16 colors		Standard colors (similar to classic console)
	16�231			6�6�6 Color Cube	216 colors formed by combinations of red, green, and blue intensities
	232�255			Grayscale			24 shades of gray from black to white

*/


class ConsoleUtils {
public:
	// Enable virtual terminal i.e. enable processing of ANSI escape sequence
	static void enableVirtualTerminal();

	// Changes text foreground & background colors
	static void setColor(int foreground, int background);
	static void setForegroundColor(int foreground);
	static void setBackgroundColor(int background);
	static void setDefaultColor();

	// Changes the console background color (and optionally the foreground color) or Clear Screen (CLS)
	static void setConsoleBackgroundColor(int backgroundColor, int foregroundColor = 7);
	static void clearConsole();

	// Function to move the cursor to a specific position
	static void moveCursor(int x, int y);

	static void temp();

	// Hide & Show Cursor Code
	static void hideCursor();
	static void showCursor();


	// draw rectangles
	static void DrawRectangle(int x, int y, int lenX, int lenY);
	static void DrawDoubleLineRectangle(int x, int y, int lenX, int lenY);

	//---
	// show credits function, with default implementation
	void virtual showCredits();
};