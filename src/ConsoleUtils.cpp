#include "ConsoleUtils.h"
#include <iostream>
#include <windows.h> // Add this include to resolve the undefined identifier error

using namespace std;

//----------------------------------------------------
void ConsoleUtils::enableVirtualTerminal() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hConsole, &dwMode);
	SetConsoleMode(hConsole, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

//----------------------------------------------------
void ConsoleUtils::setColor(int foreground, int background) {
	setForegroundColor(foreground);
	setBackgroundColor(background);
}
void ConsoleUtils::setForegroundColor(int foreground) {
	cout << "\x1b[38;5;" << foreground << "m";
}
void ConsoleUtils::setBackgroundColor(int background) {
	cout << "\x1b[48;5;" << background << "m";	// \x1b[48;5;4m
}
void ConsoleUtils::setDefaultColor() {
	cout << "\033[0m"; // << std::endl;
}

void ConsoleUtils::setConsoleBackgroundColor(int backgroundColor, int foregroundColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) return;

	backgroundColor = backgroundColor & 0x0F;
	foregroundColor = foregroundColor & 0x0F;

	WORD colorAttribute = (static_cast<WORD>(backgroundColor) << 4) | (static_cast<WORD>(foregroundColor) & 0x0F);
	SetConsoleTextAttribute(hConsole, colorAttribute);
}
void ConsoleUtils::clearConsole() {
	system("cls");
}

//----------------------------------------------------
void ConsoleUtils::moveCursor(int x, int y) {
	cout << "\033[" << y << ";" << x << "H";	//	\033[10;10H	
}

void ConsoleUtils::temp() {}

//----------------------------------------------------
void ConsoleUtils::hideCursor() {
	cout << "\033[?25l";
}
void ConsoleUtils::showCursor() {
	cout << "\033[?25h";
}

//----------------------------------------------------
void ConsoleUtils::DrawRectangle(int x, int y, int lenX, int lenY) {
	// Box-drawing characters (single-line style)
	const unsigned char TOP_LEFT = 218;     // ┌
	const unsigned char TOP_RIGHT = 191;    // ┐
	const unsigned char BOTTOM_LEFT = 192;  // └
	const unsigned char BOTTOM_RIGHT = 217; // ┘
	const unsigned char HORIZONTAL = 196;   // ─
	const unsigned char VERTICAL = 179;     // │

	// Move cursor to starting position (x,y)
	moveCursor(x, y);

	// Draw top border
	cout << TOP_LEFT;
	for (int i = 0; i < lenX - 2; ++i) {
		cout << HORIZONTAL;
	}
	cout << TOP_RIGHT;

	// Draw sides
	for (int i = 1; i < lenY - 1; ++i) {
		moveCursor(x, y + i);
		cout << VERTICAL;
		moveCursor(x + lenX - 1, y + i);
		cout << VERTICAL;
	}

	// Draw bottom border
	moveCursor(x, y + lenY - 1);
	cout << BOTTOM_LEFT;
	for (int i = 0; i < lenX - 2; ++i) {
		cout << HORIZONTAL;
	}
	cout << BOTTOM_RIGHT;

	cout << endl;
}
void ConsoleUtils::DrawDoubleLineRectangle(int x, int y, int lenX, int lenY) {
	// Box-drawing characters (double-line style)
	const unsigned char TOP_LEFT = 201;     // ╔
	const unsigned char TOP_RIGHT = 187;    // ╗
	const unsigned char BOTTOM_LEFT = 200;  // ╚
	const unsigned char BOTTOM_RIGHT = 188; // ╝
	const unsigned char HORIZONTAL = 205;   // ═
	const unsigned char VERTICAL = 186;     // ║

	// Move cursor to starting position (x,y)
	moveCursor(x, y);

	// Draw top border
	cout << TOP_LEFT;
	for (int i = 0; i < lenX - 2; ++i) {
		cout << HORIZONTAL;
	}
	cout << TOP_RIGHT;

	// Draw sides
	for (int i = 1; i < lenY - 1; ++i) {
		moveCursor(x, y + i);
		cout << VERTICAL;
		moveCursor(x + lenX - 1, y + i);
		cout << VERTICAL;
	}

	// Draw bottom border
	moveCursor(x, y + lenY - 1);
	cout << BOTTOM_LEFT;
	for (int i = 0; i < lenX - 2; ++i) {
		cout << HORIZONTAL;
	}
	cout << BOTTOM_RIGHT;

	cout << endl;
}

//----------------------------------------------------
void ConsoleUtils::showCredits()
{
	std::cout << "Default Credits:\n";
	std::cout << "----------------\n";
	std::cout << "Created by: Jawad Ahmed\n";
	std::cout << "Version: 1.0\n";
}



//===========================================
#pragma region Classic Colors (16) : Just incl here for info / reference
/*
// Set Classic Color : uses classic Windows Console API (limited to 16 colors).
void SetColorClassic(int foreground, int background) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (background << 4) | foreground);
}
void SetColorClassic_Foreground(int foreground) {
	int background = GetColorClassic_Background();
	SetColorClassic(foreground, background);
}
void SetColorClassic_Background(int background) {
	int foreground = GetColorClassic_Foreground();
	SetColorClassic(foreground, background);
}
*/
#pragma endregion



/*
//*****************************************
//					MAIN
//*****************************************
int main()
{
	//--- setup console
	utils.enableVirtualTerminal();
	utils.setBackgroundColor(ConsoleColor::Blue);
	utils.clearConsole();

	//--- show app title
	utils.setForegroundColor(ConsoleColor::BrightWhite);
	cout << "+-------------------------------+\n";
	cout << "| App Name                      |\n";
	cout << "+-------------------------------+\n\n";
	utils.setForegroundColor(ConsoleColor::White);
	utils.hideCursor();

	//--- App BODY --------------------------------

	//---------------------------------------------


	//--- show credits
	utils.showCredits();

	//--- return / finish
	return 0;
}
*/
