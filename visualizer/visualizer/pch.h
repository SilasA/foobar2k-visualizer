// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define DEBUG

#define SUCCESS 0
#define FAILURE 1
#define EXIT 2

#define DEFAULT_WINDOW_WIDTH (592 * 2)
#define DEFAULT_WINDOW_HEIGHT (400 * 2)

#define RENDER_PERIOD 10

#define GRAVITY 10.0f

// Timing
#ifdef DEBUG
	#define TIME_CAST(t) std::chrono::time_point_cast<std::chrono::milliseconds>(t)
	#define TIME_MS std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>
	#define DURATION_CAST(d) std::chrono::duration_cast<std::chrono::milliseconds>(d)
#endif

// add headers that you want to pre-compile here
#include "framework.h"

#endif //PCH_H
