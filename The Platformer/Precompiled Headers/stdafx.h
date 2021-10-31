#pragma once

/*
-	~ Snapi ~
- Template created: 01/10/2021
- Last update:		01/10/2021
*/

/////STANDARD/////
#include <initializer_list>
#include <functional>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
//#include <memory>
#include <array>
#include <stack>
/////STANDARD/////

///////SFML///////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
///////SFML///////

constexpr int WINDOW_WIDTH  = 1600;
constexpr int WINDOW_HEIGHT = 900;

constexpr float PHYSICS_STEP = 0.01f;

#ifdef NDEBUG
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup") // <- Dumps the console window
#define LOG(...)
#else
#define LOG(...) DEBUG_MESSAGE_LOG(__VA_ARGS__)
#endif

/* Print for debug messages */
template <typename Arg, typename... Args>
constexpr static void DEBUG_MESSAGE_LOG(Arg&& arg, Args&&... args)
{
	std::cerr << std::forward<Arg>(arg);
	((std::cerr << std::forward<Args>(args)), ...);
}

/* This here makes sounds and music work */
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }