#ifdef __DEBUG__
#ifdef __CONSOLE_OBSOLETE_HEADER__
#ifndef __CONSOLE__HPP__
#define __CONSOLE__HPP__


#include <stdio.h>
#include "Defines.hpp"

#ifdef WINDOWS_OS
#include <windows.h>
#endif

//TODO: Port this to Omnia framework, probably inorporating part of OmniaZERO

#define BG_COL_BLACK 0
#define BG_COL_BLUE 1
#define BG_COL_GREEN 2
#define BG_COL_CYAN 3
#define BG_COL_RED 4
#define BG_COL_MAGENTA 5
#define BG_COL_BROWN 6
#define BG_COL_LIGHT_GRAY 7
#define BG_COL_DARK_GRAY 8
#define BG_COL_LIGHT_BLUE 9
#define BG_COL_LIGHT_GREEN 10
#define BG_COL_LIGHT_CYAN 11
#define BG_COL_LIGHT_RED 12
#define BG_COL_LIGHT_MAGENTA 13
#define BG_COL_YELLOW 14
#define BG_COL_WHITE 15

#define FG_COL_BLACK 0
#define FG_COL_BLUE 1
#define FG_COL_GREEN 2
#define FG_COL_CYAN 3
#define FG_COL_RED 4
#define FG_COL_MAGENTA 5
#define FG_COL_BROWN 6
#define FG_COL_LIGHT_GRAY 7
#define FG_COL_DARK_GRAY 8
#define FG_COL_LIGHT_BLUE 9
#define FG_COL_LIGHT_GREEN 10
#define FG_COL_LIGHT_CYAN 11
#define FG_COL_LIGHT_RED 12
#define FG_COL_LIGHT_MAGENTA 13
#define FG_COL_YELLOW 14
#define FG_COL_WHITE 15

namespace Omnia
{
#ifndef WINDOWS_OS
#define WORD int
#endif

	void fg_color(WORD colore);
	void bg_color(WORD colore);

	void fg_color(WORD colore)
	{
#ifdef WINDOWS_OS
		HANDLE OutputH;
		CONSOLE_SCREEN_BUFFER_INFO info;
		short int attributi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		attributi = info.wAttributes;
		attributi = attributi & 240;
		OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(OutputH, colore | attributi);
#endif
	}

	void bg_color(WORD colore)
	{
#ifdef WINDOWS_OS
		HANDLE OutputH;
		CONSOLE_SCREEN_BUFFER_INFO info;
		short int attributi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		attributi = info.wAttributes;
		attributi = attributi & 15;
		OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(OutputH, colore * 16 | attributi);
#endif
	}
}

#endif
#endif
#else
#define BG_COL_BLACK 0
#define BG_COL_BLUE 0
#define BG_COL_GREEN 0
#define BG_COL_CYAN 0
#define BG_COL_RED 0
#define BG_COL_MAGENTA 0
#define BG_COL_BROWN 0
#define BG_COL_LIGHT_GRAY 0
#define BG_COL_DARK_GRAY 0
#define BG_COL_LIGHT_BLUE 0
#define BG_COL_LIGHT_GREEN 0
#define BG_COL_LIGHT_CYAN 0
#define BG_COL_LIGHT_RED 0
#define BG_COL_LIGHT_MAGENTA 0
#define BG_COL_YELLOW 0
#define BG_COL_WHITE 0

#define FG_COL_BLACK 0
#define FG_COL_BLUE 0
#define FG_COL_GREEN 0
#define FG_COL_CYAN 0
#define FG_COL_RED 0
#define FG_COL_MAGENTA 0
#define FG_COL_BROWN 0
#define FG_COL_LIGHT_GRAY 0
#define FG_COL_DARK_GRAY 0
#define FG_COL_LIGHT_BLUE 0
#define FG_COL_LIGHT_GREEN 0
#define FG_COL_LIGHT_CYAN 0
#define FG_COL_LIGHT_RED 0
#define FG_COL_LIGHT_MAGENTA 0
#define FG_COL_YELLOW 0
#define FG_COL_WHITE 0

#define fg_color(var)
#define bg_color(var)

#endif
